/** 
* @file	ftr_mfcc.h
* @brief	mfcc特征提取类头文件
* 
* detail...
* 
* @author	xllu
* @version	1.0
* @date	2007-7-18
* 
* @see		
* 
* <b>History:</b><br>
* <table>
*  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
*  <tr> <td>1.0		<td>2007-7-18	<td>xllu	<td>Create this file</tr>
* </table>
* 
*/
#ifndef MFCC_H_
#define MFCC_H_
#include "rec/ftr.h"
#include "ftr_def.h"
#include "rfft.h"

_BEGIN_NAMESPACE(ftr)
#if EP_USE_ASM
class mfcc
{
public:
	mfcc(){}
	~mfcc(){}

	/** 
	* @brief 	calc_mfcc
	* 
	* 计算mfcc特征	
	* 
	* @author	xllu
	* @date	2007-8-14
	* @return	virtual int	- Return 0 in success, otherwise return error code.
	* @param	float * buff	- [in] 
	* @param	float * mfcc	- [in] 
	* @param	int & out_frm_num	- [out] 
	* @param	const int frm_state	- [in] 
	* @see		
	*/
	int calc_mfcc(ftr_data * data,const int frm_state)
	{
		SP_ASSERT(0);
		srlog_wmsg("ep::mfcc.h calc_mfcc 这个函数不会调用到才对");
		SP_ASSERT(data);
		float ALIGN_N(16) bank[channel_num];
		float * buff = &(data->mfcc.mfcc_frm_buff[0]);
		float * mfcc = &(data->mfcc.mfcc[0]);

		__asm
		{
			//zero mean
			mov		eax,buff
			lea		ecx,[eax + 0xC8 * 0x04]
			xorps	xmm0,xmm0
P1:
			addps	xmm0,[eax]
			add		eax,0x10
			cmp		eax,ecx
			jne		P1
			xorps	xmm1,xmm1
			movlhps	xmm1,xmm0
			addps	xmm0,xmm1
			shufps	xmm1,xmm0,0x80
			addps	xmm0,xmm1
			shufps	xmm0,xmm1,0x03
			divss   xmm0,mfcc_frame_len
			
			// energy, pre-emphasis, hamming window etc.
			mov		eax,buff
			lea		edx,tmplt			
			lea		ecx,hamming_win
			mov		edi,200
			xorps	xmm1,xmm1				//mean
			movss	xmm1,xmm0
			shufps	xmm1,xmm1,0x0
			xorps	xmm2,xmm2				//energy
			movaps	xmm3,[edx]				//template
			xorps	xmm4,xmm4				//prepare for pre-emphasis
			movss	xmm4,[eax]
			subss	xmm4,xmm0
			xorps	xmm5,xmm5
			movss	xmm5,pre_emphasis_coef
			shufps	xmm5,xmm5,0x0			//pre-emphasis coefficient
P2:
			movaps	xmm0,[eax]
			subps	xmm0,xmm1
			//------------------
			movaps	xmm6,xmm0
			shufps	xmm6,xmm6,0x90			//低位三个float移到高位
			andps	xmm6,xmm3
			orps	xmm6,xmm4
			mulps	xmm6,xmm5
			movaps	xmm7,xmm0
			subps	xmm7,xmm6
			mulps	xmm7,[ecx]
			movaps	[eax],xmm7
			movaps	xmm4,xmm0				//将上一个循环的高位给xmm4做低位
			shufps	xmm4,xmm4,0x03
			andps	xmm4,tmplt2
			//------------------
			mulps	xmm0,xmm0
			addps	xmm2,xmm0
			add		eax,0x10
			add		ecx,0x10
			sub		edi,4
			jnz		P2
			xorps	xmm0,xmm0
			movlhps	xmm0,xmm2
			addps	xmm2,xmm0
			shufps	xmm0,xmm2,0x80
			addps	xmm2,xmm0
			shufps	xmm2,xmm0,0x03
			addss	xmm2,MINIMUM_ENERGY
			mov		eax,mfcc
			movss	[eax + 0x30],xmm2

			/* log */
#if 1
			fld		log_const
			fld		[eax+0x30]
			fyl2x	
			fstp	[eax+0x30]
#endif
		}

#if 0
		//normalize energy
		energy_sum_ += mfcc[ceps_num];
		total_engy_ += mfcc[ceps_num];
		frame_sum_ ++;
		float tmp_e = total_engy_ / (online_energy_init_len + frame_sum_);
		__asm
		{
			fld		log_const
			fld		tmp_e
			fyl2x	

			mov		eax,mfcc
			fld		log_const
			fld		[eax+0x30]
			fyl2x	
			fsub	st,st(1)
			fmul	engy_norm_coef
			fstp	[eax+0x30]
			fstp	tmp_e			/*pop the useless one*/
		}
#endif

		//filter bank
		filter_bank(buff,bank);

		float cplus_mfcc[ceps_num] = {0};
		for (int i = 0; i < ceps_num; ++i)
		{
			cplus_mfcc[i] = .0f;
			for (int j = 0; j < channel_num; ++j)
				cplus_mfcc[i] += bank[j] * cosin[i * channel_num + j];

			cplus_mfcc[i] *= bank2mfcc_coef;

			cplus_mfcc[i] *= ceps_win_coef[i];
		}
		__asm
		{
			//DCT & weight cepstrum
			mov		esi,0
			lea		eax,cosin
			lea		ecx,ceps_win_coef
L3:
			mov		edi,channel_num
			lea		edx,bank
			xorps	xmm0,xmm0
L4:
			movaps	xmm1,[edx]
			movaps	xmm2,[eax]
			mulps	xmm1,xmm2
			addps	xmm0,xmm1
			add		eax,16
			add		edx,16
			sub		edi,4
			jnz		L4

			xorps	xmm1,xmm1
			movlhps	xmm1,xmm0
			addps	xmm0,xmm1
			shufps	xmm1,xmm0,0x80
			addps	xmm0,xmm1
			shufps	xmm0,xmm1,0x03
			mulss	xmm0,[bank2mfcc_coef]
			mulss	xmm0,[ecx + esi * 0x04]		//weight cepstrum

			mov		edx,mfcc
			movss	[edx + esi * 0x4],xmm0
			inc		esi
			cmp		esi,ceps_num
			jnz		L3
		}

		SP_ASSERT(memcmp(cplus_mfcc, mfcc, sizeof(float) * ceps_num) == 0);

		return 0;
	}

protected:
	/** 
	* @brief 	filter_bank
	* 
	* filter bank	
	* 
	* @author	xllu
	* @date	2007-8-14
	* @return	virtual int	- Return 0 in success, otherwise return error code.
	* @param	float * buff	- [in] 
	* @param	float * bank	- [in] 
	* @see		
	*/
	int filter_bank(float * buff,float * bank)
	{
		SP_ASSERT(0);
		srlog_wmsg("ep::mfcc.h filter_bank这个函数不会调用到才对");
		SP_ASSERT(buff && bank);

		int index = 0;
		float tmp = 0.0f;

		//先进行fft变换
		float ALIGN_N(16) spct[mfcc_fft_length / 2];
		
		rfft256(buff,spct);

		__asm
		{
			mov		eax,bank
			lea		ecx,spct

			fld		log_floor
			//zero bank {0, 1, 2,}
			fld		log_const
			lea		edx,fpw_in_fb1
			fld		[edx]
			fmul	[ecx + 4]
			fld		[edx + 4]
			fmul	[ecx + 2 * 4]
			faddp	st(1),st
			fld		[edx + 8]
			fmul	[ecx + 3 * 4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax]

			//1st bank {1, 2, 3, 4,}
			fld		log_const
			lea		edx,fpw_in_fb2
			fld		[edx]
			fmul	[ecx+2*4]
			fld		[edx+4]
			fmul	[ecx+3*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+4*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+5*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+4]

			//2nd bank {3, 4, 5, 6,}
			fld		log_const
			lea		edx,fpw_in_fb3
			fld		[edx]
			fmul	[ecx+4*4]
			fld		[edx+4]
			fmul	[ecx+5*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+6*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+7*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+2*4]

			//3rd bank {5, 6, 7, 8, 9,}
			fld		log_const
			lea		edx,fpw_in_fb4
			fld		[edx]
			fmul	[ecx+6*4]
			fld		[edx+4]
			fmul	[ecx+7*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+8*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+9*4]
			faddp	st(1),st
			fld		[edx+16]
			fmul	[ecx+10*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+3*4]

			//4th bank {7, 8, 9, 10, 11,}
			fld		log_const
			lea		edx,fpw_in_fb5
			fld		[edx]
			fmul	[ecx+8*4]
			fld		[edx+4]
			fmul	[ecx+9*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+10*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+11*4]
			faddp	st(1),st
			fld		[edx+16]
			fmul	[ecx+12*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+4*4]

			//5th bank {10, 11, 12, 13, 14,}
			fld		log_const
			lea		edx,fpw_in_fb6
			fld		[edx]
			fmul	[ecx+11*4]
			fld		[edx+4]
			fmul	[ecx+12*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+13*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+14*4]
			faddp	st(1),st
			fld		[edx+16]
			fmul	[ecx+15*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+5*4]

			//6th bank {12, 13, 14, 15, 16, 17,}
			fld		log_const
			lea		edx,fpw_in_fb7
			fld		[edx]
			fmul	[ecx+13*4]
			fld		[edx+4]
			fmul	[ecx+14*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+15*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+16*4]
			faddp	st(1),st
			fld		[edx+16]
			fmul	[ecx+17*4]
			faddp	st(1),st
			fld		[edx+20]
			fmul	[ecx+18*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+6*4]

			//7th bank {15, 16, 17, 18, 19, 20, 21,}
			fld		log_const
			lea		edx,fpw_in_fb8
			fld		[edx]
			fmul	[ecx+16*4]
			fld		[edx+4]
			fmul	[ecx+17*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+18*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+19*4]
			faddp	st(1),st
			fld		[edx+16]
			fmul	[ecx+20*4]
			faddp	st(1),st
			fld		[edx+20]
			fmul	[ecx+21*4]
			faddp	st(1),st
			fld		[edx+24]
			fmul	[ecx+22*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+7*4]

			//8th bank {18, 19, 20, 21, 22, 23, 24,}
			fld		log_const
			lea		edx,fpw_in_fb9
			fld		[edx]
			fmul	[ecx+19*4]
			fld		[edx+4]
			fmul	[ecx+20*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+21*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+22*4]
			faddp	st(1),st
			fld		[edx+16]
			fmul	[ecx+23*4]
			faddp	st(1),st
			fld		[edx+20]
			fmul	[ecx+24*4]
			faddp	st(1),st
			fld		[edx+24]
			fmul	[ecx+25*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+8*4]

			//9th bank {22, 23, 24, 25, 26, 27, 28,}
			fld		log_const
			lea		edx,fpw_in_fb10
			fld		[edx]
			fmul	[ecx+23*4]
			fld		[edx+4]
			fmul	[ecx+24*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+25*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+26*4]
			faddp	st(1),st
			fld		[edx+16]
			fmul	[ecx+27*4]
			faddp	st(1),st
			fld		[edx+20]
			fmul	[ecx+28*4]
			faddp	st(1),st
			fld		[edx+24]
			fmul	[ecx+29*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+9*4]

			//10th bank {25, 26, 27, 28, 29, 30, 31, 32,}
			fld		log_const
			lea		edx,fpw_in_fb11
			fld		[edx]
			fmul	[ecx+26*4]
			fld		[edx+4]
			fmul	[ecx+27*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+28*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+29*4]
			faddp	st(1),st
			fld		[edx+16]
			fmul	[ecx+30*4]
			faddp	st(1),st
			fld		[edx+20]
			fmul	[ecx+31*4]
			faddp	st(1),st
			fld		[edx+24]
			fmul	[ecx+32*4]
			faddp	st(1),st
			fld		[edx+28]
			fmul	[ecx+33*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+10*4]

			//11th bank {29, 30, 31, 32, 33, 34, 35, 36,}
			fld		log_const
			lea		edx,fpw_in_fb12
			fld		[edx]
			fmul	[ecx+30*4]
			fld		[edx+4]
			fmul	[ecx+31*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+32*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+33*4]
			faddp	st(1),st
			fld		[edx+16]
			fmul	[ecx+34*4]
			faddp	st(1),st
			fld		[edx+20]
			fmul	[ecx+35*4]
			faddp	st(1),st
			fld		[edx+24]
			fmul	[ecx+36*4]
			faddp	st(1),st
			fld		[edx+28]
			fmul	[ecx+37*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+11*4]

			//12th bank {33, 34, 35, 36, 37, 38, 39, 40, 41,}
			fld		log_const
			lea		edx,fpw_in_fb13
			fld		[edx]
			fmul	[ecx+34*4]
			fld		[edx+4]
			fmul	[ecx+35*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+36*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+37*4]
			faddp	st(1),st
			fld		[edx+16]
			fmul	[ecx+38*4]
			faddp	st(1),st
			fld		[edx+20]
			fmul	[ecx+39*4]
			faddp	st(1),st
			fld		[edx+24]
			fmul	[ecx+40*4]
			faddp	st(1),st
			fld		[edx+28]
			fmul	[ecx+41*4]
			faddp	st(1),st
			fld		[edx+32]
			fmul	[ecx+42*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+12*4]

			//13th bank {37, 38, 39, 40, 41, 42, 43, 44, 45, 46,}
			fld		log_const
			lea		edx,fpw_in_fb14
			fld		[edx]
			fmul	[ecx+38*4]
			fld		[edx+4]
			fmul	[ecx+39*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+40*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+41*4]
			faddp	st(1),st
			fld		[edx+16]
			fmul	[ecx+42*4]
			faddp	st(1),st
			fld		[edx+20]
			fmul	[ecx+43*4]
			faddp	st(1),st
			fld		[edx+24]
			fmul	[ecx+44*4]
			faddp	st(1),st
			fld		[edx+28]
			fmul	[ecx+45*4]
			faddp	st(1),st
			fld		[edx+32]
			fmul	[ecx+46*4]
			faddp	st(1),st
			fld		[edx+36]
			fmul	[ecx+47*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+13*4]

			//14th bank {42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,}
			fld		log_const
			lea		edx,fpw_in_fb15
			fld		[edx]
			fmul	[ecx+43*4]
			fld		[edx+4]
			fmul	[ecx+44*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+45*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+46*4]
			faddp	st(1),st
			fld		[edx+16]
			fmul	[ecx+47*4]
			faddp	st(1),st
			fld		[edx+20]
			fmul	[ecx+48*4]
			faddp	st(1),st
			fld		[edx+24]
			fmul	[ecx+49*4]
			faddp	st(1),st
			fld		[edx+28]
			fmul	[ecx+50*4]
			faddp	st(1),st
			fld		[edx+32]
			fmul	[ecx+51*4]
			faddp	st(1),st
			fld		[edx+36]
			fmul	[ecx+52*4]
			faddp	st(1),st
			fld		[edx+40]
			fmul	[ecx+53*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+14*4]

			//15th bank {47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58,}
			fld		log_const
			lea		edx,fpw_in_fb16
			fld		[edx]
			fmul	[ecx+48*4]
			fld		[edx+4]
			fmul	[ecx+49*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+50*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+51*4]
			faddp	st(1),st
			fld		[edx+16]
			fmul	[ecx+52*4]
			faddp	st(1),st
			fld		[edx+20]
			fmul	[ecx+53*4]
			faddp	st(1),st
			fld		[edx+24]
			fmul	[ecx+54*4]
			faddp	st(1),st
			fld		[edx+28]
			fmul	[ecx+55*4]
			faddp	st(1),st
			fld		[edx+32]
			fmul	[ecx+56*4]
			faddp	st(1),st
			fld		[edx+36]
			fmul	[ecx+57*4]
			faddp	st(1),st
			fld		[edx+40]
			fmul	[ecx+58*4]
			faddp	st(1),st
			fld		[edx+44]
			fmul	[ecx+59*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+15*4]

			//16th bank {53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64,}
			fld		log_const
			lea		edx,fpw_in_fb17
			fld		[edx]
			fmul	[ecx+54*4]
			fld		[edx+4]
			fmul	[ecx+55*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+56*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+57*4]
			faddp	st(1),st
			fld		[edx+16]
			fmul	[ecx+58*4]
			faddp	st(1),st
			fld		[edx+20]
			fmul	[ecx+59*4]
			faddp	st(1),st
			fld		[edx+24]
			fmul	[ecx+60*4]
			faddp	st(1),st
			fld		[edx+28]
			fmul	[ecx+61*4]
			faddp	st(1),st
			fld		[edx+32]
			fmul	[ecx+62*4]
			faddp	st(1),st
			fld		[edx+36]
			fmul	[ecx+63*4]
			faddp	st(1),st
			fld		[edx+40]
			fmul	[ecx+64*4]
			faddp	st(1),st
			fld		[edx+44]
			fmul	[ecx+65*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+16*4]

			//17th bank {59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,}
			fld		log_const
			lea		edx,fpw_in_fb18
			fld		[edx]
			fmul	[ecx+60*4]
			fld		[edx+4]
			fmul	[ecx+61*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+62*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+63*4]
			faddp	st(1),st
			fld		[edx+16]
			fmul	[ecx+64*4]
			faddp	st(1),st
			fld		[edx+20]
			fmul	[ecx+65*4]
			faddp	st(1),st
			fld		[edx+24]
			fmul	[ecx+66*4]
			faddp	st(1),st
			fld		[edx+28]
			fmul	[ecx+67*4]
			faddp	st(1),st
			fld		[edx+32]
			fmul	[ecx+68*4]
			faddp	st(1),st
			fld		[edx+36]
			fmul	[ecx+69*4]
			faddp	st(1),st
			fld		[edx+40]
			fmul	[ecx+70*4]
			faddp	st(1),st
			fld		[edx+44]
			fmul	[ecx+71*4]
			faddp	st(1),st
			fld		[edx+48]
			fmul	[ecx+72*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+17*4]

			//18th bank {65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,}
			fld		log_const
			lea		edx,fpw_in_fb19
			fld		[edx]
			fmul	[ecx+66*4]
			fld		[edx+4]
			fmul	[ecx+67*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+68*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+69*4]
			faddp	st(1),st
			fld		[edx+16]
			fmul	[ecx+70*4]
			faddp	st(1),st
			fld		[edx+20]
			fmul	[ecx+71*4]
			faddp	st(1),st
			fld		[edx+24]
			fmul	[ecx+72*4]
			faddp	st(1),st
			fld		[edx+28]
			fmul	[ecx+73*4]
			faddp	st(1),st
			fld		[edx+32]
			fmul	[ecx+74*4]
			faddp	st(1),st
			fld		[edx+36]
			fmul	[ecx+75*4]
			faddp	st(1),st
			fld		[edx+40]
			fmul	[ecx+76*4]
			faddp	st(1),st
			fld		[edx+44]
			fmul	[ecx+77*4]
			faddp	st(1),st
			fld		[edx+48]
			fmul	[ecx+78*4]
			faddp	st(1),st
			fld		[edx+52]
			fmul	[ecx+79*4]
			faddp	st(1),st
			fld		[edx+56]
			fmul	[ecx+80*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+18*4]

			//19th bank {72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,}
			fld		log_const
			lea		edx,fpw_in_fb20
			fld		[edx]
			fmul	[ecx+73*4]
			fld		[edx+4]
			fmul	[ecx+74*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+75*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+76*4]
			faddp	st(1),st
			fld		[edx+16]
			fmul	[ecx+77*4]
			faddp	st(1),st
			fld		[edx+20]
			fmul	[ecx+78*4]
			faddp	st(1),st
			fld		[edx+24]
			fmul	[ecx+79*4]
			faddp	st(1),st
			fld		[edx+28]
			fmul	[ecx+80*4]
			faddp	st(1),st
			fld		[edx+32]
			fmul	[ecx+81*4]
			faddp	st(1),st
			fld		[edx+36]
			fmul	[ecx+82*4]
			faddp	st(1),st
			fld		[edx+40]
			fmul	[ecx+83*4]
			faddp	st(1),st
			fld		[edx+44]
			fmul	[ecx+84*4]
			faddp	st(1),st
			fld		[edx+48]
			fmul	[ecx+85*4]
			faddp	st(1),st
			fld		[edx+52]
			fmul	[ecx+86*4]
			faddp	st(1),st
			fld		[edx+56]
			fmul	[ecx+87*4]
			faddp	st(1),st
			fld		[edx+60]
			fmul	[ecx+88*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+19*4]

			//20th bank {80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96,}
			fld		log_const
			lea		edx,fpw_in_fb21
			fld		[edx]
			fmul	[ecx+81*4]
			fld		[edx+4]
			fmul	[ecx+82*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+83*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+84*4]
			faddp	st(1),st
			fld		[edx+16]
			fmul	[ecx+85*4]
			faddp	st(1),st
			fld		[edx+20]
			fmul	[ecx+86*4]
			faddp	st(1),st
			fld		[edx+24]
			fmul	[ecx+87*4]
			faddp	st(1),st
			fld		[edx+28]
			fmul	[ecx+88*4]
			faddp	st(1),st
			fld		[edx+32]
			fmul	[ecx+89*4]
			faddp	st(1),st
			fld		[edx+36]
			fmul	[ecx+90*4]
			faddp	st(1),st
			fld		[edx+40]
			fmul	[ecx+91*4]
			faddp	st(1),st
			fld		[edx+44]
			fmul	[ecx+92*4]
			faddp	st(1),st
			fld		[edx+48]
			fmul	[ecx+93*4]
			faddp	st(1),st
			fld		[edx+52]
			fmul	[ecx+94*4]
			faddp	st(1),st
			fld		[edx+56]
			fmul	[ecx+95*4]
			faddp	st(1),st
			fld		[edx+60]
			fmul	[ecx+96*4]
			faddp	st(1),st
			fld		[edx+64]
			fmul	[ecx+97*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+20*4]

			//21st bank {88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105,}
			fld		log_const
			lea		edx,fpw_in_fb22
			fld		[edx]
			fmul	[ecx+89*4]
			fld		[edx+4]
			fmul	[ecx+90*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+91*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+92*4]
			faddp	st(1),st
			fld		[edx+16]
			fmul	[ecx+93*4]
			faddp	st(1),st
			fld		[edx+20]
			fmul	[ecx+94*4]
			faddp	st(1),st
			fld		[edx+24]
			fmul	[ecx+95*4]
			faddp	st(1),st
			fld		[edx+28]
			fmul	[ecx+96*4]
			faddp	st(1),st
			fld		[edx+32]
			fmul	[ecx+97*4]
			faddp	st(1),st
			fld		[edx+36]
			fmul	[ecx+98*4]
			faddp	st(1),st
			fld		[edx+40]
			fmul	[ecx+99*4]
			faddp	st(1),st
			fld		[edx+44]
			fmul	[ecx+100*4]
			faddp	st(1),st
			fld		[edx+48]
			fmul	[ecx+101*4]
			faddp	st(1),st
			fld		[edx+52]
			fmul	[ecx+102*4]
			faddp	st(1),st
			fld		[edx+56]
			fmul	[ecx+103*4]
			faddp	st(1),st
			fld		[edx+60]
			fmul	[ecx+104*4]
			faddp	st(1),st
			fld		[edx+64]
			fmul	[ecx+105*4]
			faddp	st(1),st
			fld		[edx+68]
			fmul	[ecx+106*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+21*4]

			//22nd bank {97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115,}
			fld		log_const
			lea		edx,fpw_in_fb23
			fld		[edx]
			fmul	[ecx+98*4]
			fld		[edx+4]
			fmul	[ecx+99*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+100*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+101*4]
			faddp	st(1),st
			fld		[edx+16]
			fmul	[ecx+102*4]
			faddp	st(1),st
			fld		[edx+20]
			fmul	[ecx+103*4]
			faddp	st(1),st
			fld		[edx+24]
			fmul	[ecx+104*4]
			faddp	st(1),st
			fld		[edx+28]
			fmul	[ecx+105*4]
			faddp	st(1),st
			fld		[edx+32]
			fmul	[ecx+106*4]
			faddp	st(1),st
			fld		[edx+36]
			fmul	[ecx+107*4]
			faddp	st(1),st
			fld		[edx+40]
			fmul	[ecx+108*4]
			faddp	st(1),st
			fld		[edx+44]
			fmul	[ecx+109*4]
			faddp	st(1),st
			fld		[edx+48]
			fmul	[ecx+110*4]
			faddp	st(1),st
			fld		[edx+52]
			fmul	[ecx+111*4]
			faddp	st(1),st
			fld		[edx+56]
			fmul	[ecx+112*4]
			faddp	st(1),st
			fld		[edx+60]
			fmul	[ecx+113*4]
			faddp	st(1),st
			fld		[edx+64]
			fmul	[ecx+114*4]
			faddp	st(1),st
			fld		[edx+68]
			fmul	[ecx+115*4]
			faddp	st(1),st
			fld		[edx+72]
			fmul	[ecx+116*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+22*4]

			//23rd bank {106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,}
			fld		log_const
			lea		edx,fpw_in_fb24
			fld		[edx]
			fmul	[ecx+107*4]
			fld		[edx+4]
			fmul	[ecx+108*4]
			faddp	st(1),st
			fld		[edx+8]
			fmul	[ecx+109*4]
			faddp	st(1),st
			fld		[edx+12]
			fmul	[ecx+110*4]
			faddp	st(1),st
			fld		[edx+16]
			fmul	[ecx+111*4]
			faddp	st(1),st
			fld		[edx+20]
			fmul	[ecx+112*4]
			faddp	st(1),st
			fld		[edx+24]
			fmul	[ecx+113*4]
			faddp	st(1),st
			fld		[edx+28]
			fmul	[ecx+114*4]
			faddp	st(1),st
			fld		[edx+32]
			fmul	[ecx+115*4]
			faddp	st(1),st
			fld		[edx+36]
			fmul	[ecx+116*4]
			faddp	st(1),st
			fld		[edx+40]
			fmul	[ecx+117*4]
			faddp	st(1),st
			fld		[edx+44]
			fmul	[ecx+118*4]
			faddp	st(1),st
			fld		[edx+48]
			fmul	[ecx+119*4]
			faddp	st(1),st
			fld		[edx+52]
			fmul	[ecx+120*4]
			faddp	st(1),st
			fld		[edx+56]
			fmul	[ecx+121*4]
			faddp	st(1),st
			fld		[edx+60]
			fmul	[ecx+122*4]
			faddp	st(1),st
			fld		[edx+64]
			fmul	[ecx+123*4]
			faddp	st(1),st
			fld		[edx+68]
			fmul	[ecx+124*4]
			faddp	st(1),st
			fld		[edx+72]
			fmul	[ecx+125*4]
			faddp	st(1),st
			fld		[edx+76]
			fmul	[ecx+126*4]
			faddp	st(1),st
			fld		[edx+80]
			fmul	[ecx+127*4]
			faddp	st(1),st
			fcomi	st,st(2)
			fcmovb	st,st(2)
			fyl2x
			fstp	[eax+23*4]

			fstp	tmp
		}

		return 0;
	}
};

#else
//这个类不会用到,所以就不转了
#endif	// EP_USE_ASM

_END_NAMESPACE

#endif