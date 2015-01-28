#ifndef PITCH_H_
#define PITCH_H_

#include "rec/ftr.h"
#include "ftr_def.h"
#include <iomanip>
#include <limits>
#include <cmath>

//#pragma warning(disable:4731)

_BEGIN_NAMESPACE(ftr)

const float band_width			= 15.625000f;	//(float)param_.fs / fft_len_;
const float voice_threshold		= 0.05f;
const float pitch_emphasis_coef = 0.98f;
const float freq_step			= 1.015f;
const float harmonics_factor	= 0.94f;
const float max_allowed_ratio	= 1.0f / 1.45f;
const float tone_up_limit		= 1.60f;
const int	frame_step			= 80;
const int	high_pitch			= 480;
const int	low_pitch			= 70;
const int	pitch_buff_size		= 600;
const int	start_band			= 4;
const int	end_band			= 30;
const int	media_win_size		= 5;
const int	linear_win_size		= 5;
const int	online_delay		= 55;
const int	norm_win_size		= 100;
const int	back_voice_gap		= 4;
const int	tone_voice_scale	= 80;
const int	max_harmonics		= 12;
const int	max_harmonics_freq	= 1250;
const int	min_voice_gap		= 10;
const int	band_shift_range	= 1;
const int	tone_pitch_scale	= 50;
const int	mradius				= 2;
const int	lradius				= 2;

#pragma pack(push,1)

struct pitcher_data_element
{
	int         unvoiced_num;						//每帧之前的非浊音帧数目
	int         smoothed_f0;						//每帧平滑之后的基频
	float       normalized_f0; 						//每帧归一化后的基频
	float       voice_conf;							//每帧的浊音置信度
	float       smoothed_conf;					    //每帧平滑之后的浊音置信度
	int         rough_f0;							//每帧的初始基频
	float       cand_path_dist[pitch_band_num];		//最优路径距离
	int         cand_back_track[pitch_band_num];
	int         pitch_cand[pitch_band_num];			//每帧每个频带中的最优频率
	float       pitch_cand_conf[pitch_band_num];	//每帧每个频带中的最大置信度
};

const int size_of_element			= sizeof(pitcher_data_element);

#define offset_unvoiced_num		 0
#define offset_smoothed_f0		 4
#define offset_normalized_f0	 (4 + offset_smoothed_f0)
#define offset_voice_conf		 (4 + offset_normalized_f0)
#define offset_smoothed_conf	 (4 + offset_voice_conf)
#define offset_rough_f0			 (4 + offset_smoothed_conf)
#define offset_cand_path_dist	 (4 + offset_rough_f0)
#define offset_cand_back_track	 (4 * pitch_band_num + offset_cand_path_dist)
#define offset_pitch_cand		 (4 * pitch_band_num + offset_cand_back_track)
#define offset_pitch_cand_conf	 (4 * pitch_band_num + offset_pitch_cand)

struct pitcher_data
{
	pitcher_data_element  element[PITCH_BUFF_FRM_NUM];
};

#pragma pack(pop)

class pitcher : public pitcher_data
{
public:
	int init()
	{
		unvoiced_cnt_		= 0;
		export_frm_num_		= 0;
		next_export_pos_	= 0;
		next_track_pos_		= 0;
		next_smooth_pos_	= 0;
		cur_frm_num_		= 0;
		is_speech_end_		= false;

		/* only reset smoothed conf 'cause the first utterance will have impact on the next one*/
		for (int i = 0;i < PITCH_BUFF_FRM_NUM;i ++)
			element[i].smoothed_conf = 0.0f;

		return 0;
	}
	int fini()
	{
		return 0;
	}
	int reset()
	{
		unvoiced_cnt_		= 0;
		cur_frm_num_		= 0;
		next_track_pos_		= 0;
		next_smooth_pos_	= 0;
		next_export_pos_	= 0;
		export_frm_num_		= 0;
		is_speech_end_		= false;

		/* only reset smoothed conf 'cause the first utterance will have impact on the next one*/
		for (int i = 0;i < PITCH_BUFF_FRM_NUM;i ++)
			element[i].smoothed_conf = 0.0f;

		return 0;
	}
	int calc_pitch(ftr_data * data,pitch * p,int & out_frm_num,const int frm_state)
	{
		SP_ASSERT(data && p);
		float * buff = &(data->pitch.pitch_frm_buff[0]);
		float ALIGN_N(16) frm_data[pitch_frame_size];
		float ALIGN_N(16) spct[85];

		is_speech_end_ = (frm_state == ISR_AUDIO_SAMPLE_LAST) ? true :false; 

		//拷贝数据，可以考虑去掉一个变量
		memcpy(frm_data,buff,sizeof(float) * pitch_frame_size);

#if EP_USE_ASM
		__asm
		{
			//pre-emphasis and hamming
			mov		eax,buff
			lea		ecx,hamming_win_400
			lea		edx,tmplt
			mov		edi,400

			movaps	xmm3,[edx]
			xorps	xmm4,xmm4
			movss	xmm4,[eax]
			xorps	xmm5,xmm5
			movss	xmm5,pitch_emphasis_coef
			shufps	xmm5,xmm5,0x0
L1:
			movaps	xmm0,[eax]
			movaps	xmm6,xmm0
			shufps	xmm6,xmm6,0x90
			andps	xmm6,xmm3
			orps	xmm6,xmm4
			mulps	xmm6,xmm5
			movaps	xmm7,xmm0
			subps	xmm7,xmm6
			mulps	xmm7,[ecx]
			movaps	[eax],xmm7
			movaps	xmm4,xmm0
			shufps	xmm4,xmm4,0x03
			andps	xmm4,tmplt2

			add		eax,0x10
			add		ecx,0x10
			sub		edi,4
			jnz		L1
		}
#else
		{
			//预加重,汉明窗
			for ( int i = pitch_frame_size -1; i >0; i--)
			{
				buff[i] -= buff[i-1]*pitch_emphasis_coef;
				buff[i] *= hamming_win_400[i];
			}
			buff[0] *= (1.0f -pitch_emphasis_coef);
			buff[0] *= hamming_win_400[0];
		}
#endif

		{
			std::ostringstream oss;
			oss<<std::setprecision(8);
			for (int i = 0; i < pitch_frame_size; ++i)
				oss<<buff[i]<<" ";
			srlog_dbg("pitch::pre-emphasis and hamming: %s", oss.str().c_str());
		}

	
		{
#if EP_USE_ASM
			rfft512_op84(buff,spct);
#else
			/* 这里算fft和原汇编代码有些区别,
			汇编代码只算了需要的84个点,
			而c++的代码则算了所有的点,在结果中只取前面84个点 */
			{
				float real[pitch_fft_length] = {0};
				float image[pitch_fft_length] = {0};
				cplus_fft(buff, real, image, pitch_fft_length, 1);
				spct[0] = real[0] + image[0];
				for (int i = 1; i < 85; ++i)
				{
					spct[i] = sqrt(real[i]*real[i] + image[i]*image[i]);
				}
				std::ostringstream oss;
				oss<<std::setprecision(8);
				for (int i = 0; i < 85; ++i)
					oss<<spct[i]<<" ";
				srlog_dbg("ep::pitch calc_pitch rfft512_op84: %s", oss.str().c_str());
			}
#endif
		}

		element[cur_frm_num_].unvoiced_num = unvoiced_cnt_;
		//计算基频候选以及每个频带内的最优等
		calc_pitch_cand(&element[cur_frm_num_],spct);
		//计算该帧的浊音置信度
		calc_voice_conf(&element[cur_frm_num_],frm_data,pitch_frame_size);
		//前向搜索
		int last_frm_num = get_circular_buff_index(cur_frm_num_,-1);
		if ((cur_frm_num_ == 0) && (export_frm_num_ == 0))
		{
			for (int i = start_band;i < end_band;i++)
			{
				//计算距离信息以及跳转信息
				element[cur_frm_num_].cand_path_dist[i] = element[cur_frm_num_].pitch_cand_conf[i];
				element[cur_frm_num_].cand_back_track[i] = 0;
			}
		}
		else
		{
			search(&element[cur_frm_num_],&element[last_frm_num],unvoiced_cnt_);
		}
		
		//更新非浊音帧数
		unvoiced_cnt_ = (fabs(element[cur_frm_num_].voice_conf) > voice_threshold) ? 0 : (unvoiced_cnt_ + 1);

		//如果符合一定条件，导出pitch特征
		//线性平滑指针
		int line_cnt = 0, line_start = 0, line_end = 0;
		int rough_f0_index = 0;
		int max_delay = online_delay + norm_win_size / 2;
		int unexport_cnt = (cur_frm_num_ - next_export_pos_ < 0) ? 
			(PITCH_BUFF_FRM_NUM + cur_frm_num_ - next_export_pos_ + 1) : (cur_frm_num_ - next_export_pos_ + 1);
		if ((unexport_cnt >= online_delay 
			&& element[cur_frm_num_].unvoiced_num >= back_voice_gap)
			|| unexport_cnt >= max_delay 
			|| is_speech_end_)
		{
			int next_bp = get_circular_buff_index(cur_frm_num_,1);
			int next_sp = is_speech_end_ ? get_circular_buff_index(cur_frm_num_,1) : get_circular_buff_index(cur_frm_num_,1 - mradius - lradius);
			int next_ep = is_speech_end_ ? get_circular_buff_index(cur_frm_num_,1) : get_circular_buff_index(cur_frm_num_,2 - online_delay);
			int bp_cnt = get_circular_buff_index(next_sp,-next_track_pos_);
			int sp_cnt = get_circular_buff_index(next_sp,-next_smooth_pos_);
			int ep_cnt = get_circular_buff_index(next_ep,-next_export_pos_);

			//回溯
			back_track(element,next_track_pos_,get_circular_buff_index(next_bp,-1));
			//平滑基频
			smooth_pitch(element,next_smooth_pos_,get_circular_buff_index(next_sp,-1),sp_cnt,line_cnt,line_start,
				line_end,rough_f0_index,export_frm_num_,is_speech_end_);
			//平滑浊音置信度
			smooth_voice_conf(element,next_smooth_pos_,get_circular_buff_index(next_sp,-1),sp_cnt,line_cnt,line_start,line_end,
				rough_f0_index,is_speech_end_);
			//需要输出的特征归一化
			normalize_f0(element,next_export_pos_,get_circular_buff_index(next_ep,-1),voice_threshold,is_speech_end_);
			//伸缩
			for (int frm_num = next_export_pos_;frm_num != next_ep;frm_num = get_circular_buff_index(frm_num,1))
			{
				element[frm_num].normalized_f0 -= tone_up_limit / 2.0f;
				element[frm_num].normalized_f0 *= tone_pitch_scale;
			}

			//特征输出
			out_frm_num = ep_cnt;
			for (int frm_num = next_export_pos_,i = 0;i < ep_cnt;frm_num = get_circular_buff_index(frm_num,1),i++)
			{
				p[i].f0 = element[frm_num].normalized_f0;
				p[i].voice_degree = element[frm_num].smoothed_conf * tone_voice_scale;
			}

			//更新位置指针
			export_frm_num_ += ep_cnt;
			cur_frm_num_ = get_circular_buff_index(cur_frm_num_,1);
			next_track_pos_ = cur_frm_num_;
			next_smooth_pos_ = get_circular_buff_index(cur_frm_num_,-mradius - lradius);
			next_export_pos_ = next_ep;

			if(is_speech_end_)
				reset();
		}
		else
			cur_frm_num_ = get_circular_buff_index(cur_frm_num_,1);


		return 0;
	}

protected:
	static int get_circular_buff_index(const int cur,const int offset)
	{
		SP_ASSERT(cur >= 0 && cur < PITCH_BUFF_FRM_NUM);

		return (cur + offset + PITCH_BUFF_FRM_NUM) & (PITCH_BUFF_FRM_NUM - 1);
	}
	static void calc(float * fft_data,float * freq_conf)
	{
#if EP_USE_ASM
		__asm
		{
			/*12 points
			for (int i = 0;i< 35;i++)
			{
				freq_conf[i] = 0;
				for (int j = 0;j < 12;j++)
					freq_conf[i] += harmonics_weight[j] * fft_data[hmnc_pt_of_freq[i * 16 + j]];
			}*/
			xor     edx,edx
			mov		edi,0
			lea		eax,hmnc_pt_of_freq
			mov		ecx,fft_data
			mov		esi,freq_conf
L1:
			fldz			
			mov		dx,[eax+edi*0x08]
			fld		[ecx+edx*4]
			fld		harmonics_weight
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x02]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x04
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x04]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x08
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x6]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0xC
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x8]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x10
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x0A]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x14
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x0C]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x18
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x0E]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x1C
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x10]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x20
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x12]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x24
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x14]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x28
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x16]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x2C
			fmulp   st(1),st
			faddp	st(1),st
			fstp	[esi+edi]

			add		edi,0x4
			cmp		edi,0x23*0x4
			jne		L1

			/*11 points
			for (int i = 35;i< 44;i++)
			{
				freq_conf[i] = 0;
				for (int j = 0;j < 11;j++)
					freq_conf[i] += harmonics_weight[j] * fft_data[hmnc_pt_of_freq[i * 16 + j]];
			}*/
L2:
			fldz
			mov		dx,[eax+edi*0x08]
			fld		[ecx+edx*4]
			fld		harmonics_weight
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x02]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x04
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x04]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x08
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x06]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0xC
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x08]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x10
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x0A]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x14
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x0C]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x18
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x0E]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x1C
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x10]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x20
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x12]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x24
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x14]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x28
			fmulp   st(1),st
			faddp	st(1),st
			fstp	[esi+edi]

			add		edi,0x04
			cmp		edi,0x2C*0x04
			jne		L2

			/*10 points
			for (int i = 44;i< 56;i++)
			{
				freq_conf[i] = 0;
				for (int j = 0;j < 10;j++)
					freq_conf[i] += harmonics_weight[j] * fft_data[hmnc_pt_of_freq[i * 16 + j]];
			}*/
L3:
			fldz
			mov		dx,[eax+edi*0x08]
			fld		[ecx+edx*4]
			fld		harmonics_weight
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x02]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x04
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x04]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x08
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x06]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0xC
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x08]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x10
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x0A]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x14
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x0C]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x18
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x0E]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x1C
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x10]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x20
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x12]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x24
			fmulp   st(1),st
			faddp	st(1),st
			fstp	[esi+edi]

			add		edi,0x04
			cmp		edi,0x38*0x04
			jne		L3

			/*9 points
			for (int i = 56;i< 67;i++)
			{
				freq_conf[i] = 0;
				for (int j = 0;j < 9;j++)
					freq_conf[i] += harmonics_weight[j] * fft_data[hmnc_pt_of_freq[i * 16 + j]];
			}*/
L4:
			fldz
			mov		dx,[eax+edi*0x08]
			fld		[ecx+edx*4]
			fld		harmonics_weight
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x02]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x04
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x04]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x08
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x06]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0xC
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x08]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x10
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x0A]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x14
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x0C]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x18
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x0E]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x1C
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x10]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x20
			fmulp   st(1),st
			faddp	st(1),st
			fstp	[esi+edi]

			add		edi,0x04
			cmp		edi,0x43*0x04
			jne		L4

			/*8 points
			for (int i = 67;i< 76;i++)
			{
				freq_conf[i] = 0;
				for (int j = 0;j < 8;j++)
					freq_conf[i] += harmonics_weight[j] * fft_data[hmnc_pt_of_freq[i * 16 + j]];
			}*/
L5:
			fldz
			mov		dx,[eax+edi*0x08]
			fld		[ecx+edx*4]
			fld		harmonics_weight
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x02]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x04
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x04]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x08
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x06]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0xC
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x08]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x10
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x0A]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x14
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x0C]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x18
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x0E]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x1C
			fmulp   st(1),st
			faddp	st(1),st
			fstp	[esi+edi]

			add		edi,0x04
			cmp		edi,0x4C*0x04
			jne		L5

			/*7 points
			for (int i = 76;i< 87;i++)
			{
				freq_conf[i] = 0;
				for (int j = 0;j < 7;j++)
					freq_conf[i] += harmonics_weight[j] * fft_data[hmnc_pt_of_freq[i * 16 + j]];
			}*/
L6:
			fldz
			mov		dx,[eax+edi*0x08]
			fld		[ecx+edx*4]
			fld		harmonics_weight
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x02]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x04
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x04]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x08
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x06]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0xC
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x08]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x10
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x0A]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x14
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x0C]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x18
			fmulp   st(1),st
			faddp	st(1),st
			fstp	[esi+edi]

			add		edi,0x04
			cmp		edi,0x57*0x04
			jne		L6

			/*6 points
			for (int i = 87;i< 101;i++)
			{
				freq_conf[i] = 0;
				for (int j = 0;j < 6;j++)
					freq_conf[i] += harmonics_weight[j] * fft_data[hmnc_pt_of_freq[i * 16 + j]];
			}*/
L7:
			fldz
			mov		dx,[eax+edi*0x08]
			fld		[ecx+edx*4]
			fld		harmonics_weight
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x02]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x04
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x04]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x08
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x06]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0xC
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x08]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x10
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x0A]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x14
			fmulp   st(1),st
			faddp	st(1),st
			fstp	[esi+edi]

			add		edi,0x04
			cmp		edi,0x65*0x04
			jne		L7

			/*5 points
			for (int i = 101;i< 115;i++)
			{
				freq_conf[i] = 0;
				for (int j = 0;j < 5;j++)
					freq_conf[i] += harmonics_weight[j] * fft_data[hmnc_pt_of_freq[i * 16 + j]];
			}*/
L8:
			fldz
			mov		dx,[eax+edi*0x08]
			fld		[ecx+edx*4]
			fld		harmonics_weight
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x02]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x04
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x04]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x08
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x06]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0xC
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x08]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x10
			fmulp   st(1),st
			faddp	st(1),st
			fstp	[esi+edi]

			add		edi,0x04
			cmp		edi,0x73*0x04
			jne		L8

			/*4 points
			for (int i = 115;i< 132;i++)
			{
				freq_conf[i] = 0;
				for (int j = 0;j < 4;j++)
					freq_conf[i] += harmonics_weight[j] * fft_data[hmnc_pt_of_freq[i * 16 + j]];
			}*/
L9:
			fldz
			mov		dx,[eax+edi*0x08]
			fld		[ecx+edx*4]
			fld		harmonics_weight
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x02]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x04
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x04]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x08
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x06]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0xC
			fmulp   st(1),st
			faddp	st(1),st
			fstp	[esi+edi]

			add		edi,0x04
			cmp		edi,0x84*0x04
			jne		L9

			/*3 points
			for (int i = 132;i< 153;i++)
			{
				freq_conf[i] = 0;
				for (int j = 0;j < 3;j++)
					freq_conf[i] += harmonics_weight[j] * fft_data[hmnc_pt_of_freq[i * 16 + j]];
			}*/
L10:
			fldz
			mov		dx,[eax+edi*0x08]
			fld		[ecx+edx*4]
			fld		harmonics_weight
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x02]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x04
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x04]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x08
			fmulp   st(1),st
			faddp	st(1),st
			fstp	[esi+edi]

			add		edi,0x04
			cmp		edi,0x99*0x04
			jne		L10

			/*2 points
			for (int i = 153;i< 163;i++)
			{
				freq_conf[i] = 0;
				for (int j = 0;j < 2;j++)
					freq_conf[i] += harmonics_weight[j] * fft_data[hmnc_pt_of_freq[i * 16 + j]];
			}*/
L11:
			fldz
			mov		dx,[eax+edi*0x08]
			fld		[ecx+edx*4]
			fld		harmonics_weight
			fmulp   st(1),st
			faddp	st(1),st

			mov		dx,[eax+edi*0x08+0x02]
			fld		[ecx+edx*4]
			fld		harmonics_weight+0x04
			fmulp   st(1),st
			faddp	st(1),st
			fstp	[esi+edi]

			add		edi,0x4
			cmp		edi,0xA3*0x4
			jne		L11
		}
#else

		memset(freq_conf, 0, sizeof(float) * 0xa3);
		//这个确实很恶心
		for (int i = 0; i < 0x23; ++i)
		{
			for (int j = 0; j < 12; ++j)
			{
				freq_conf[i] +=fft_data[hmnc_pt_of_freq[i*16+j]]*harmonics_weight[j];
			}
		}
		for (int i = 0x23; i < 0x2c; ++i)
		{
			for (int j = 0; j < 11; ++j)
			{
				freq_conf[i] +=fft_data[hmnc_pt_of_freq[i*16+j]]*harmonics_weight[j];
			}
		}
		for (int i = 0x2c; i < 0x38; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				freq_conf[i] +=fft_data[hmnc_pt_of_freq[i*16+j]]*harmonics_weight[j];
			}
		}
		for (int i = 0x38; i < 0x43; ++i)
		{
			for (int j = 0; j < 9; ++j)
			{
				freq_conf[i] +=fft_data[hmnc_pt_of_freq[i*16+j]]*harmonics_weight[j];
			}
		}
		for (int i = 0x43; i < 0x4c; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				freq_conf[i] +=fft_data[hmnc_pt_of_freq[i*16+j]]*harmonics_weight[j];
			}
		}
		for (int i = 0x4c; i < 0x57; ++i)
		{
			for (int j = 0; j < 7; ++j)
			{
				freq_conf[i] +=fft_data[hmnc_pt_of_freq[i*16+j]]*harmonics_weight[j];
			}
		}
		for (int i = 0x57; i < 0x65; ++i)
		{
			for (int j = 0; j < 6; ++j)
			{
				freq_conf[i] +=fft_data[hmnc_pt_of_freq[i*16+j]]*harmonics_weight[j];
			}
		}
		for (int i = 0x65; i < 0x73; ++i)
		{
			for (int j = 0; j < 5; ++j)
			{
				freq_conf[i] +=fft_data[hmnc_pt_of_freq[i*16+j]]*harmonics_weight[j];
			}
		}
		for (int i = 0x73; i < 0x84; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				freq_conf[i] +=fft_data[hmnc_pt_of_freq[i*16+j]]*harmonics_weight[j];
			}
		}
		for (int i = 0x84; i < 0x99; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				freq_conf[i] +=fft_data[hmnc_pt_of_freq[i*16+j]]*harmonics_weight[j];
			}
		}
		for (int i = 0x99; i < 0xa3; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				freq_conf[i] +=fft_data[hmnc_pt_of_freq[i*16+j]]*harmonics_weight[j];
			}
		}
#endif
		std::ostringstream oss;
		oss<<std::setprecision(8);
		for (int i = 0; i < 163; ++i)
		{
			oss<<freq_conf[i]<<" ";
		}
		srlog_dbg("pitch::calc: %s", oss.str().c_str());
	}

#if EP_USE_ASM
#define  MAC_Calc_SHS(x,y) \
	/* x-->该F0段包含多少个谐波,y-->该F0段的截止F0(不含)数目 */ \
SHS1_L##x: \
	__asm{xorps   xmm0,xmm0} \
	__asm{mov     esi,(x-1)*2h} \
	__asm{add     eax,x*2h}\
SHS2_L##x: \
	__asm{sub     eax,2}	\
	__asm{movss	  xmm2,[harmonics_weight+esi*2h]}\
	__asm{mov	  dx,[eax+edi*8h]}\
	__asm{movss	  xmm1,[ecx+edx*4h]}\
	__asm{sub     esi,2h}\
	__asm{mulss   xmm1,xmm2}\
	__asm{addss   xmm0,xmm1}\
	__asm{jae     SHS2_L##x}\
	__asm{movss	  [freq_conf+edi],xmm0}\
	__asm{add	  edi,4h}\
	__asm{cmp	  edi,y*4h}\
	__asm{jnz	  SHS1_L##x}

#define  MAC_Calc_Pitch_Cand(x,y) 			\
	__asm{mov	edi,esi}          			\
	__asm{fld	[edx+esi*0x04]}   			\
	__asm{inc	esi}              			\
L##x:					          			\
	__asm{fld	[edx+esi*0x04]}   			\
	__asm{fcomi	st,st(1)}         			\
	__asm{jna   D##x}             			\
	__asm{fst   st(1)}            			\
	__asm{mov   edi,esi}          			\
D##x:                             			\
	__asm{fsubp st,st}            			\
	__asm{inc   esi}              			\
	__asm{cmp	esi,y}            			\
	__asm{jne	L##x}             			\
	__asm{fcomi st,st(1)}         			\
	__asm{movss xmm1,[ecx+edi*4]}			\
	__asm{jna   E##x}						\
	__asm{fst   st(1)}						\
	__asm{movss xmm0,xmm1}					\
E##x:										\
	__asm{fstp	[eax+offset_pitch_cand_conf+(0x04+x)*0x04]}		\
	__asm{movss [eax+offset_pitch_cand+(0x04+x)*0x04],xmm1}

#else
//不会用到这些宏
#endif

	//基频及置信度计算以及平滑等处理
	static void calc_pitch_cand(pitcher_data_element* cur_block,float * fft_data)
	{
		float freq_conf[freq_cnt];
		//计算候选频率点及其置信度

		calc(fft_data,freq_conf);
#if 0
		__asm
		{
			xor     edx,edx
			mov		edi,0
			lea		eax,hmnc_pt_of_freq
			mov		ecx,fft_data
		}
		MAC_Calc_SHS(12,35);
		MAC_Calc_SHS(11,44);
		MAC_Calc_SHS(10,56);
		MAC_Calc_SHS(9,67);
		MAC_Calc_SHS(8,76);
		MAC_Calc_SHS(7,87);
		MAC_Calc_SHS(6,101);
		MAC_Calc_SHS(5,115);
		MAC_Calc_SHS(4,132);
		MAC_Calc_SHS(3,153);
		MAC_Calc_SHS(2,163);
#endif
		//在基音候选中选出最优基音
#if 0
		float max_in_band;
		float max_in_frm = -1.0f;
		rough_f0[cur_frm] = 0;
		const int offset = cur_frm * pitch_band_num;
		for (int band = start_band;band < end_band;band++)
		{
			max_in_band = -1.0f;
			for (int i = 1;i <= fp_in_band[band - start_band][0];i++)
			{
				const int pos = fp_in_band[band - start_band][i];
				if (freq_conf[pos] > max_in_band)
				{
					max_in_band = freq_conf[pos];
					pitch_cand[offset + band] = freq_point[pos];
				}
			}
			SP_ASSERT(max_in_band >= 0);
			pitch_cand_conf[offset + band] = max_in_band;
			if (max_in_band > max_in_frm)
			{
				max_in_frm = max_in_band;
				rough_f0[cur_frm] = pitch_cand[offset + band];
			}
		}
		SP_ASSERT(rough_f0[cur_frm] != 0);
#endif

#if EP_USE_ASM
		__asm
		{
			mov		esi,0		
			lea		edx,freq_conf
			lea     ecx,freq_point
			mov		eax,cur_block
			movss	xmm0,[ecx+esi*0x04]		
			fldz
		}        			

		//band 4,{0, 1, 2, 3, 4, 5, 6, 7,}
		MAC_Calc_Pitch_Cand(0,8);
		//band 5,{8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,}
		MAC_Calc_Pitch_Cand(1,24);
		//band 6,{24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,}
		MAC_Calc_Pitch_Cand(2,39);
		//band 7,{39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,}
		MAC_Calc_Pitch_Cand(3,55);
		//band 8,{55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,}
		MAC_Calc_Pitch_Cand(4,68);
		//band 9,{68, 69, 70, 71, 72, 73, 74,}
		MAC_Calc_Pitch_Cand(5,75);
		//band 10,{75, 76, 77, 78, 79, 80, 81, 82,}
		MAC_Calc_Pitch_Cand(6,83);
		//band 11,{83, 84, 85, 86, 87, 88, 89, 90,}
		MAC_Calc_Pitch_Cand(7,91);
		//band 12,{91, 92, 93, 94, 95, 96, 97, 98,}
		MAC_Calc_Pitch_Cand(8,99);
		//band 13,{99, 100, 101, 102, 103,}
		MAC_Calc_Pitch_Cand(9,104);
		//band 14,{104, 105, 106, 107, 108,}
		MAC_Calc_Pitch_Cand(10,109);
		//band 15,{109, 110, 111, 112, 113,}
		MAC_Calc_Pitch_Cand(11,114);
		//band 16,{114, 115, 116, 117, 118,119}
		MAC_Calc_Pitch_Cand(12,120);
		//band 17,{120, 121, 122, 123,}
		MAC_Calc_Pitch_Cand(13,124);
		//band 18,{124, 125, 126, 127,}
		MAC_Calc_Pitch_Cand(14,128);
		//band 19,{128, 129, 130, 131,}
		MAC_Calc_Pitch_Cand(15,132);
		//band 20,{132, 133, 134,}
		MAC_Calc_Pitch_Cand(16,135);
		//band 21,{135, 136, 137, 138,}
		MAC_Calc_Pitch_Cand(17,139);
		//band 22,{139, 140, 141,}
		MAC_Calc_Pitch_Cand(18,142);
		//band 23,{142, 143, 144,}
		MAC_Calc_Pitch_Cand(19,145);
		//band 24,{145, 146, 147,}
		MAC_Calc_Pitch_Cand(20,148);
		//band 25,{148, 149, 150,}
		MAC_Calc_Pitch_Cand(21,151);
		//band 26,{151, 152, 153,}
		MAC_Calc_Pitch_Cand(22,154);
		//band 27,{154, 155, 156,}
		MAC_Calc_Pitch_Cand(23,157);
		//band 28,{157, 158,}
		MAC_Calc_Pitch_Cand(24,159);
		//band 29,{159, 160, 161,}
		MAC_Calc_Pitch_Cand(25,162);
		__asm
		{	
			movss   [eax + offset_rough_f0],xmm0
			fsubp   st,st
		}
#else
		//cplus
/*		pitcher_data_element pde= {0};*/
		//4 == start_band
		float max_value = -1.0f;
		for (int i = 0; i < sizeof(fp_in_band) / sizeof(fp_in_band[0]); ++i)
		{
			float max_in_band = -1.0f;
			for (int j = fp_in_band[i][1]; j < fp_in_band[i][0] + fp_in_band[i][1]; ++j)
			{
				if (freq_conf[j] > max_in_band)
				{
					max_in_band = freq_conf[j];
					cur_block->pitch_cand_conf[4+i] = max_in_band;
					cur_block->pitch_cand[4+i] = freq_point[j];
				}
			}
			if (max_in_band > max_value)
			{
				max_value = max_in_band;
				cur_block->rough_f0 = cur_block->pitch_cand[4+i];
			}
		}
#endif

		std::ostringstream oss;
		oss<<std::setprecision(8);
		for (int i = start_band; i < end_band; ++i)
			oss<<cur_block->pitch_cand_conf[i]<<" ";
		for (int i = start_band; i < end_band; ++i)
			oss<<cur_block->pitch_cand[i]<<" ";
		srlog_dbg("pitch::calc_pitch_cand: %s", oss.str().c_str());


	}
	static void calc_voice_conf(pitcher_data_element * cur_block,float * pcm_data,const int pcm_len)
	{
		int lag = frequency / cur_block->rough_f0;
#if EP_USE_ASM
		__asm
		{
			mov		esi,0
			mov		eax,pcm_data
			xorps	xmm3,xmm3			/*corr*/
			xorps	xmm4,xmm4			/*energy1*/
			xorps	xmm5,xmm5			/*energy2*/
L1:
			movaps	xmm0,[eax+esi*0x04]
			mov		edi,esi
			add		edi,lag
			movups	xmm1,[eax+edi*0x04]
			movaps	xmm2,xmm0
			mulps	xmm0,xmm1
			addps	xmm3,xmm0
			mulps	xmm1,xmm1
			addps	xmm4,xmm1
			mulps	xmm2,xmm2
			addps	xmm5,xmm2

			add		esi,0x04
			cmp		esi,calc_vc_sample_num
			jne		L1

			/*horizontal add*/
			xorps	xmm0,xmm0
			movlhps	xmm0,xmm3
			addps	xmm3,xmm0
			shufps	xmm0,xmm3,0x80
			addps	xmm3,xmm0
			shufps	xmm3,xmm0,0x03

			xorps	xmm0,xmm0
			movlhps	xmm0,xmm4
			addps	xmm4,xmm0
			shufps	xmm0,xmm4,0x80
			addps	xmm4,xmm0
			shufps	xmm4,xmm0,0x03

			xorps	xmm0,xmm0
			movlhps	xmm0,xmm5
			addps	xmm5,xmm0
			shufps	xmm0,xmm5,0x80
			addps	xmm5,xmm0
			shufps	xmm5,xmm0,0x03

			/*voice confidence*/
			mulss	xmm4,xmm5
			addss	xmm4,calc_vc_basic_engy
			sqrtss	xmm0,xmm4
			divss	xmm3,xmm0
			mov		eax,cur_block
			add		eax,offset_voice_conf
			movss	[eax],xmm3
		}
#else
		float val = 0.f;
		float e1 = 0.f;
		float e2 = 0.f;
		for (int i = 0; i < calc_vc_sample_num; ++i)
		{
			val += pcm_data[i] * pcm_data[lag+i];
			e1 += pcm_data[lag+i] * pcm_data[lag+i];
			e2 += pcm_data[i] * pcm_data[i]; 
		}
		e1 *= e2;
		e1 += calc_vc_basic_engy;
		val /= sqrt(e1);
		cur_block->voice_conf = val;
		int break_p = 1;
		{
			srlog_dbg("pitch::calc_voice_conf: %f", cur_block->voice_conf);
		}
#endif
	}

#if EP_USE_ASM
#define  MAC_Search_Band(x) \
	/*cur frame data*/ \
	__asm{cvtsi2ss	xmm0,[eax+offset_pitch_cand+x*0x04]}\
	__asm{movss		xmm1,[eax+offset_pitch_cand_conf+x*0x04]}\
	/*last frame data, band*/ \
	__asm{movss		    xmm2,xmm4}\
	__asm{movss			xmm3,xmm6}\
	__asm{comiss	xmm0,xmm2}\
	__asm{ja		D##x}\
	__asm{movss	xmm5,xmm0}\
	__asm{divss	xmm5,xmm2}\
	__asm{jmp     E##x}\
D##x:\
	__asm{movss	xmm5,xmm2}\
	__asm{divss	xmm5,xmm0}\
E##x:\
	__asm{mulss	xmm5,xmm1}\
	__asm{addss	xmm5,xmm3}\
	__asm{mov		edi,x}\
	/*last frame data, band - 1*/\
	__asm{movhlps	xmm2,xmm4}\
	__asm{movhlps	xmm3,xmm6}\
	__asm{divss		xmm2,xmm0}\
	__asm{comiss	xmm2,xmm7}\
	__asm{jb	    F##x}\
	__asm{mulss	xmm2,xmm1}\
	__asm{addss	xmm2,xmm3}\
	__asm{comiss	xmm2,xmm5}\
	__asm{jbe		F##x}\
	__asm{movss	xmm5,xmm2}\
	__asm{mov		edi,x-1}\
F##x:\
	/*last frame data, band + 1*/\
	__asm{movlhps		xmm4,xmm4}\
	__asm{movlhps		xmm6,xmm6}\
	__asm{cvtsi2ss	xmm2,[edx+offset_pitch_cand+(x+1)*0x04]}\
	__asm{movss	xmm3,[edx+offset_cand_path_dist+(x+1)*0x04]}\
	__asm{movss		xmm4,xmm2}\
	__asm{movss		xmm6,xmm3}\
	__asm{divss	xmm0,xmm2}\
	__asm{comiss	xmm0,xmm7}\
	__asm{jb		G##x}\
	__asm{mulss	xmm0,xmm1}\
	__asm{addss	xmm0,xmm3}\
	__asm{comiss	xmm0,xmm5}\
	__asm{jbe		G##x}\
	__asm{movss	xmm5,xmm0}\
	__asm{mov		edi,x+1}\
G##x:\
	__asm{movss	[eax+offset_cand_path_dist+x*0x04],xmm5}\
	__asm{mov		[eax+offset_cand_back_track+x*0x04],edi}

#else
	//不会用到这些宏
#endif

	static void search(pitcher_data_element* cur_block,pitcher_data_element* last_block,int unvoice_cnt)
	{
		float max_trnas_score, tmp_trans_score;
		int back_band;

		//重新计算
		if (unvoice_cnt >= min_voice_gap && cur_block->voice_conf > voice_threshold)
		{
			for (int band = start_band;band < end_band;band++)
			{
				max_trnas_score = -1.0f;
				back_band = -1;

				for (int last_band = band - band_shift_range;last_band <= band + band_shift_range;last_band++)
				{
					if(last_band < start_band || last_band >= end_band)
						continue;

					const int cand_last = last_block->pitch_cand[last_band];
					const int cand_cur  = cur_block->pitch_cand[band];
					tmp_trans_score = (float)(std::min)(cand_last, cand_cur) / (std::max)(cand_last, cand_cur);
					if(tmp_trans_score < max_allowed_ratio)
						continue;

					if (last_block->cand_path_dist[last_band] > max_trnas_score)
					{
						max_trnas_score = last_block->cand_path_dist[last_band];
						back_band = last_band;
					}
				}

				//记录距离信息以及跳转信息
				cur_block->cand_path_dist[band] = cur_block->pitch_cand_conf[band];
				cur_block->cand_back_track[band] = back_band;
			}
		}
		else
		{
#if 0
			for (int band = start_band;band < end_band;band++)
			{
				max_trnas_score = -1.0f;
				back_band = -1;
				
				for (int last_band = band - band_shift_range;last_band <= band + band_shift_range;last_band++)
				{
					if(last_band < start_band || last_band >= end_band)
						continue;

					const int cand_last = last_block->pitch_cand[last_band];
					const int cand_cur  = cur_block->pitch_cand[band];
					tmp_trans_score = (float)(std::min)(cand_last, cand_cur) / (std::max)(cand_last, cand_cur);
					if(tmp_trans_score < max_allowed_ratio)
						continue;

					tmp_trans_score *= cur_block->pitch_cand_conf[band];
					tmp_trans_score += last_block->cand_path_dist[last_band];

					if (tmp_trans_score > max_trnas_score)
					{
						max_trnas_score = tmp_trans_score;
						back_band = last_band;
					}
				}
				//记录距离信息以及跳转信息
				cur_block->cand_path_dist[band] = max_trnas_score;
				cur_block->cand_back_track[band] = back_band;
			}
			FILE* fp = fopen("e:\\1.txt","a+");
			for (int i = 4;i < 30;i++)
			{
				fprintf(fp,"%7.5f *** %d\n",cur_block->cand_path_dist[i],cur_block->cand_back_track[i]);
			}
			fclose(fp);
#endif
		
#if EP_USE_ASM
			__asm
			{
				mov		eax,cur_block			//cur frame cand
				mov		edx,last_block			//last frame dist
				movss	xmm7,max_allowed_ratio
				//4th band
				
				/*cur frame data*/
				cvtsi2ss	xmm0,[eax+offset_pitch_cand+4*0x04]
				movss		xmm1,[eax+offset_pitch_cand_conf+4*0x04]
				/*last frame data, band*/
				cvtsi2ss	xmm2,[edx+offset_pitch_cand+4*0x04]
				movss		xmm3,[edx+offset_cand_path_dist+4*0x04]
				movlhps	xmm4,xmm2
				movlhps	xmm6,xmm3
				comiss	xmm0,xmm2
				ja		L1
				movss	xmm5,xmm0
				divss	xmm5,xmm2
				jmp     D1
L1:
				movss	xmm5,xmm2
				divss	xmm5,xmm0
D1:
				mulss	xmm5,xmm1
				addss	xmm5,xmm3
				mov		edi,4
				/*last frame data, band + 1*/
				cvtsi2ss	xmm2,[edx+offset_pitch_cand+5*0x04]
				movss	xmm3,[edx+offset_cand_path_dist+5*0x04]
				movss	xmm4,xmm2
				movss	xmm6,xmm3
				divss	xmm0,xmm2
				comiss	xmm0,xmm7
				jb		L2
				mulss	xmm0,xmm1
				addss	xmm0,xmm3
				comiss	xmm0,xmm5
				jbe		L2
				movss	xmm5,xmm0
				mov		edi,0x04+1
L2:
				movss	[eax+offset_cand_path_dist+0x04*0x04],xmm5
				mov		[eax+offset_cand_back_track+0x04*0x04],edi				
			}
			//band 5
			MAC_Search_Band(5);
			MAC_Search_Band(6);
			MAC_Search_Band(7);
			MAC_Search_Band(8);
			MAC_Search_Band(9);
			MAC_Search_Band(10);
			MAC_Search_Band(11);
			MAC_Search_Band(12);
			MAC_Search_Band(13);
			MAC_Search_Band(14);
			MAC_Search_Band(15);
			MAC_Search_Band(16);
			MAC_Search_Band(17);
			MAC_Search_Band(18);
			MAC_Search_Band(19);
			MAC_Search_Band(20);
			MAC_Search_Band(21);
			MAC_Search_Band(22);
			MAC_Search_Band(23);
			MAC_Search_Band(24);
			MAC_Search_Band(25);
			MAC_Search_Band(26);
			MAC_Search_Band(27);
			MAC_Search_Band(28);

			__asm
			{
				//29 band
				/*cur frame data*/
				cvtsi2ss	xmm0,[eax+offset_pitch_cand+29*0x04]
				movss		xmm1,[eax+offset_pitch_cand_conf+29*0x04]
				/*last frame data, band*/
				movss		xmm2,xmm4
				movss		xmm3,xmm6
				comiss	xmm0,xmm2
				ja		L3
				movss	xmm5,xmm0
				divss	xmm5,xmm2
				jmp     D3
L3:
				movss	xmm5,xmm2
				divss	xmm5,xmm0
D3:
				mulss	xmm5,xmm1
				addss	xmm5,xmm3
				mov		edi,29
				/*last frame data, band - 1*/
				movhlps	xmm2,xmm4
				movhlps	xmm3,xmm6
				divss	xmm2,xmm0
				comiss	xmm2,xmm7
				jb		L4
				mulss	xmm2,xmm1
				addss	xmm2,xmm3
				comiss	xmm2,xmm5
				jbe		L4
				movss	xmm5,xmm2
				mov		edi,28
L4:
				movss	[eax+offset_cand_path_dist+29*0x04],xmm5
				mov		[eax+offset_cand_back_track+29*0x04],edi	
			}
#else
// 			pitcher_data_element pde = {0};
// 			memcpy(&pde, cur_block, sizeof(pde));
			{
				float max_trnas_score, tmp_trans_score;
				int back_band;
				for (int band = start_band;band < end_band;band++)
				{
					max_trnas_score = -1.0f;
					back_band = -1;

					for (int last_band = band - band_shift_range;last_band <= band + band_shift_range;last_band++)
					{
						if(last_band < start_band || last_band >= end_band)
							continue;

						const int cand_last = last_block->pitch_cand[last_band];
						const int cand_cur  = cur_block->pitch_cand[band];
						tmp_trans_score = (float)(std::min)(cand_last, cand_cur) / (std::max)(cand_last, cand_cur);
						if(tmp_trans_score < max_allowed_ratio)
							continue;

						tmp_trans_score *= cur_block->pitch_cand_conf[band];
						tmp_trans_score += last_block->cand_path_dist[last_band];

						if (tmp_trans_score > max_trnas_score)
						{
							max_trnas_score = tmp_trans_score;
							back_band = last_band;
						}
					}
					//记录距离信息以及跳转信息
					cur_block->cand_path_dist[band] = max_trnas_score;
					cur_block->cand_back_track[band] = back_band;
				}
			}	
#endif

			std::ostringstream oss;
			oss<<std::setprecision(8);
			for (int i = start_band; i < end_band; ++i)
				oss<<cur_block->cand_path_dist[i]<<" ";
			for (int i = start_band; i < end_band; ++i)
				oss<<cur_block->cand_back_track[i]<<" ";
			srlog_dbg("pitch::search: %s", oss.str().c_str());
		}


	}
	static void back_track(pitcher_data_element* elem,const int start_frm,const int end_frm)
	{
		SP_ASSERT(start_frm >= 0 && start_frm < PITCH_BUFF_FRM_NUM);
		SP_ASSERT(end_frm >= 0 && end_frm < PITCH_BUFF_FRM_NUM);

		int   last_best_cand = start_band; 
		int   cur_best_cand  = 0, count = 0;
		float max_dist       = -1.0f;

		//先找到最后一帧的最优bank
		for (int band = start_band;band < end_band;band++)
		{
			if(elem[end_frm].cand_path_dist[band] > max_dist)
			{
				max_dist = elem[end_frm].cand_path_dist[band];
				last_best_cand = band;
				count ++;
			}
		}
		SP_ASSERT(count != 0);

		//回溯，更新各个帧的rough f0
		elem[end_frm].rough_f0 = elem[end_frm].pitch_cand[last_best_cand];
		for (int frm = get_circular_buff_index(end_frm,-1);frm != get_circular_buff_index(start_frm,-1);frm = get_circular_buff_index(frm,-1))
		{
			SP_ASSERT(frm >= 0 && frm < PITCH_BUFF_FRM_NUM);
			int frm_num = get_circular_buff_index(frm,1);
			//根据历史信息获取当前帧对应的cand
			cur_best_cand = elem[frm_num].cand_back_track[last_best_cand];
			//更新该帧rough f0
			elem[frm].rough_f0 = elem[frm].pitch_cand[cur_best_cand];
			last_best_cand = cur_best_cand;
		}
	}
	static void smooth_pitch(pitcher_data_element* elem,const int start_frm,const int end_frm,const int frm_num,int & buff_len,int & line_start,
		int & line_end,int & rough_f0_index,const int export_frm_num,bool is_speech_end)
	{
		SP_ASSERT(start_frm >= 0 && start_frm < PITCH_BUFF_FRM_NUM);
		SP_ASSERT(end_frm >= 0 && end_frm < PITCH_BUFF_FRM_NUM);

		//平滑半径
		int media_radius = (media_win_size - 1) / 2;
		int linear_radius = (linear_win_size - 1) / 2;

		buff_len = frm_num;
		for (int i = 1;(i <= export_frm_num) && (i <= media_radius + linear_radius);i++,line_start++);
		buff_len += line_start;
		if (!is_speech_end)
		{
			rough_f0_index = get_circular_buff_index(end_frm,media_radius + linear_radius);
			buff_len += (media_radius + linear_radius);
			line_end = buff_len - media_radius - linear_radius - 1;
		}
		else
		{
			rough_f0_index = end_frm;
			line_end = buff_len - 1;
		}

		//数据拷贝
		//int * tmp_data = new int[buff_len];
		int tmp_data[max_temp_buff_size];
		memset(tmp_data,0,sizeof(int) * buff_len);
		for (int i = 0;i < buff_len;i++)
		{
			tmp_data[buff_len - i - 1] = elem[get_circular_buff_index(rough_f0_index,-i)].rough_f0;
		}

		//中值平滑和线性平滑
		smooth(tmp_data,elem,buff_len,media_radius,linear_radius,line_start,line_end,start_frm,frm_num,is_speech_end);
	}
	static void smooth_voice_conf(pitcher_data_element* elem,const int start_frm,const int end_frm,const int frm_cnt,int & buff_len,int & line_start,
		int & line_end,int rough_index,bool is_speech_end)
	{
		SP_ASSERT(start_frm >= 0 && start_frm < PITCH_BUFF_FRM_NUM);
		SP_ASSERT(end_frm >= 0 && end_frm < PITCH_BUFF_FRM_NUM);

		//平滑半径
		int media_radius = (media_win_size - 1) / 2;
		int linear_radius = (linear_win_size - 1) / 2;

		//数据拷贝
		//float * tmp_data = new float[buff_len];
		float tmp_data[max_temp_buff_size];
		for (int i = 0;i < buff_len;i++)
		{
			tmp_data[buff_len - i - 1] = fabs(elem[get_circular_buff_index(rough_index,-i)].voice_conf);
		}

		//中值平滑和线性平滑
		smooth(tmp_data,elem,buff_len,media_radius,linear_radius,line_start,line_end,start_frm,frm_cnt,is_speech_end);
	}
	static void normalize_f0(pitcher_data_element* elem,const int start_frm,const int end_frm,float voice_thre,bool is_speech_end)
	{
		SP_ASSERT(start_frm >= 0 && start_frm < PITCH_BUFF_FRM_NUM);
		SP_ASSERT(end_frm >= 0 && end_frm < PITCH_BUFF_FRM_NUM);

		int voiced_frm_num = 0;
		int voiced_frm_sum = 0;
		int pitch_sum = 0;
		int last_frm = get_circular_buff_index(end_frm,1);
		for (int frm_num = start_frm;frm_num != last_frm;frm_num = get_circular_buff_index(frm_num,1))
		{
			int start_pos = get_circular_buff_index(frm_num,-norm_win_size / 2 - 1);
			int end_pos = get_circular_buff_index(frm_num,norm_win_size / 2 + 1);
			pitch_sum = 0;
			voiced_frm_num = 0;
			voiced_frm_sum = 0;
			//frm_num左边的帧
			for (int i = get_circular_buff_index(frm_num,-1);i != start_pos;i = get_circular_buff_index(i,-1))
			{
				pitch_sum += elem[i].smoothed_f0;
				if (elem[i].smoothed_conf > voice_thre)
				{
					voiced_frm_sum += elem[i].smoothed_f0;
					voiced_frm_num ++;
				}
			}
			//frm_num右边的帧
			if (is_speech_end)
			{
				for (int i = frm_num;i != end_pos;i = get_circular_buff_index(i,1))
				{
					if(i == last_frm)
						break;
					pitch_sum += elem[i].smoothed_f0;
					if (elem[i].smoothed_conf > voice_thre)
					{
						voiced_frm_sum += elem[i].smoothed_f0;
						voiced_frm_num ++;
					}
				}
			}
			else
			{
				for (int i = frm_num;i != end_pos;i = get_circular_buff_index(i,1))
				{
					pitch_sum += elem[i].smoothed_f0;
					if (elem[i].smoothed_conf > voice_thre)
					{
						voiced_frm_sum += elem[i].smoothed_f0;
						voiced_frm_num ++;
					}
				}
			}

			SP_ASSERT(pitch_sum != 0);
			float normalizer = 0.0f;
			normalizer = (voiced_frm_num == 0) ? ((float)pitch_sum / 
				(norm_win_size + 1)) : ((float)voiced_frm_sum / voiced_frm_num);

			//归一化
			elem[frm_num].normalized_f0 = elem[frm_num].smoothed_f0 / normalizer;
		}	
	}

	static void smooth(int * rought_data,pitcher_data_element * elem,const int data_size,const int media_radius,const int linear_radius,
		const int line_start,const int line_end,const int start_frm,const int frm_cnt,bool is_speech_end)
	{
		SP_ASSERT(data_size >= 0);

		//临时内存
		//int * tmp_data = new int[data_size];
		int tmp_data[max_temp_buff_size];
		int tmp_win[media_win_size];

		//处理句首
		for(int i = 0;i < media_radius;i++)
			tmp_data[i] = rought_data[i];
		//处理句尾
		for(int i = data_size - media_radius;i < data_size;i++)
			tmp_data[i] = rought_data[i];
		//中值滤波处理句中数据
		for (int frm = media_radius;frm < data_size - media_radius;frm++)
		{
			for(int i = frm - media_radius;i <= frm + media_radius;i++)
				tmp_win[i - (frm - media_radius)] = rought_data[i];
			//排序，求取中间值
			for (int i = 0;i <= media_radius;i++)
			{
				for (int j = i + 1;j < media_win_size;j++)
				{
					if (tmp_win[i] > tmp_win[j])
						std::swap(tmp_win[i],tmp_win[j]);
				}
			}
			tmp_data[frm] = tmp_win[media_radius];
		}

		//中值滤波
		int calc_cnt = 0;
		int offset = (line_start >= 2) ? 2 : line_start;
		offset *= linear_win_size;
		for (int frm = line_start;frm <= line_end;frm++)
		{
			float smoothed_sample = 0.0f;
			for (int i = frm - linear_radius;i <= frm + linear_radius;i++)
			{
				if(i < 0 || i > data_size - 1)
					continue;
				smoothed_sample += tmp_data[i] * sin_win[offset + i - (frm - linear_radius)];
			}
			elem[get_circular_buff_index(start_frm,calc_cnt)].smoothed_f0 = (int)smoothed_sample;
			if (!is_speech_end)
			{
				if(frm < linear_radius)
					offset += linear_win_size;
			}
			else
			{
				if(frm < linear_radius || frm > data_size - linear_radius - 2)
					offset += linear_win_size;
			}
			calc_cnt++;
		}

		SP_ASSERT(calc_cnt == frm_cnt);
	}
	static void smooth(float * rought_data,pitcher_data_element* elem,const int data_size,const int media_radius,const int linear_radius,
		const int line_start,const int line_end,const int start_frm,const int frm_cnt,bool is_speech_end)
	{
		SP_ASSERT(data_size >= 0);

		//临时内存
		//float * tmp_data = new float[data_size];
		float tmp_data[max_temp_buff_size];
		float tmp_win[media_win_size];

		//处理句首
		for(int i = 0;i < media_radius;i++)
			tmp_data[i] = rought_data[i];
		//处理句尾
		for(int i = data_size - media_radius;i < data_size;i++)
			tmp_data[i] = rought_data[i];
		//中值滤波处理句中数据
		for (int frm = media_radius;frm < data_size - media_radius;frm++)
		{
			for(int i = frm - media_radius;i <= frm + media_radius;i++)
				tmp_win[i - (frm - media_radius)] = rought_data[i];
			//排序，求取中间值
			for (int i = 0;i <= media_radius;i++)
			{
				for (int j = i + 1;j < media_win_size;j++)
				{
					if (tmp_win[i] > tmp_win[j])
						std::swap(tmp_win[i],tmp_win[j]);
				}
			}
			tmp_data[frm] = tmp_win[media_radius];
		}

		//中值滤波
		int calc_cnt = 0;
		int offset = (line_start >= 2) ? 2 : line_start;
		offset *= linear_win_size;
		for (int frm = line_start;frm <= line_end;frm++)
		{
			float smoothed_sample = 0.0f;
			for (int i = frm - linear_radius;i <= frm + linear_radius;i++)
			{
				if(i < 0 || i > data_size - 1)
					continue;
				smoothed_sample += tmp_data[i] * sin_win[offset + i - (frm - linear_radius)];
			}
			elem[get_circular_buff_index(start_frm,calc_cnt)].smoothed_conf = (float)smoothed_sample;
			if (!is_speech_end)
			{
				if(frm < linear_radius)
					offset += linear_win_size;
			}
			else
			{
				if(frm < linear_radius || frm > data_size - linear_radius - 2)
					offset += linear_win_size;
			}
			calc_cnt++;
		}

		SP_ASSERT(calc_cnt == frm_cnt);
	}

protected:
	int         unvoiced_cnt_;
	int         cur_frm_num_;
	int         next_export_pos_;
	int         next_track_pos_;
	int         next_smooth_pos_;
	int         export_frm_num_;										//导出帧数目
	bool        is_speech_end_;	
};

_END_NAMESPACE

#endif