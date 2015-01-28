#ifndef  __RFFT_H__
#define  __RFFT_H__

#ifndef SSE_ALIGN
#define SSE_ALIGN ALIGN_N(16) 
#endif
#include <limits>

/*********************************************
函数调用:

inline void rfft256(float*x,float* sqrt_x);

//输入256点实数组x，输出128点幅度谱 -> sqrt_x

inline void rfft512_op84(float*x,float* sqrt_x)

//输入512点实数组x，输出0-84点幅度谱 -> sqrt_x
**********************************************/
#if EP_USE_ASM

#define L1_Atom_P256(x)      \
	__asm{mov     ecx,400h}  \
L1_P256##x:					 \
	__asm{movaps  xmm0,[eax+ecx-10h]} \
	__asm{movaps  xmm1,[eax+ecx+3F0h]} \
	__asm{movaps  xmm2,xmm0} \
	__asm{addps   xmm0,xmm1} \
	__asm{subps   xmm2,xmm1} \
	__asm{movaps  [eax+ecx-10h],xmm0} \
	__asm{movaps  [eax+ecx+3F0h],xmm2} \
	__asm{sub     ecx,10h} \
	__asm{jnz     L1_P256##x}

#define L1_Atom_P128(x)                  \
	__asm{mov     ecx,200h}              \
L1_P128##x:                              \
	__asm{movaps  xmm0,[eax+ecx-10h+x]}  \
	__asm{movaps  xmm1,[eax+ecx+1F0h+x]} \
	__asm{movaps  xmm2,xmm0}             \
	__asm{addps   xmm0,xmm1}             \
	__asm{subps   xmm2,xmm1}             \
	__asm{movaps  [eax+ecx-10h+x],xmm0}  \
	__asm{movaps  [eax+ecx+1F0h+x],xmm2} \
	__asm{sub     ecx,10h}               \
	__asm{jnz     L1_P128##x}


const float SSE_ALIGN W_P128[] = 
{
	1.000000f,
	0.000000f,
	0.999699f,
	-0.024541f,
	0.998795f,
	-0.049068f,
	0.997290f,
	-0.073565f,
	0.995185f,
	-0.098017f,
	0.992480f,
	-0.122411f,
	0.989177f,
	-0.146730f,
	0.985278f,
	-0.170962f,
	0.980785f,
	-0.195090f,
	0.975702f,
	-0.219101f,
	0.970031f,
	-0.242980f,
	0.963776f,
	-0.266713f,
	0.956940f,
	-0.290285f,
	0.949528f,
	-0.313682f,
	0.941544f,
	-0.336890f,
	0.932993f,
	-0.359895f,
	0.923880f,
	-0.382683f,
	0.914210f,
	-0.405241f,
	0.903989f,
	-0.427555f,
	0.893224f,
	-0.449611f,
	0.881921f,
	-0.471397f,
	0.870087f,
	-0.492898f,
	0.857729f,
	-0.514103f,
	0.844854f,
	-0.534998f,
	0.831470f,
	-0.555570f,
	0.817585f,
	-0.575808f,
	0.803208f,
	-0.595699f,
	0.788346f,
	-0.615232f,
	0.773010f,
	-0.634393f,
	0.757209f,
	-0.653173f,
	0.740951f,
	-0.671559f,
	0.724247f,
	-0.689541f,
	0.707107f,
	-0.707107f,
	0.689541f,
	-0.724247f,
	0.671559f,
	-0.740951f,
	0.653173f,
	-0.757209f,
	0.634393f,
	-0.773010f,
	0.615232f,
	-0.788346f,
	0.595699f,
	-0.803208f,
	0.575808f,
	-0.817585f,
	0.555570f,
	-0.831470f,
	0.534998f,
	-0.844854f,
	0.514103f,
	-0.857729f,
	0.492898f,
	-0.870087f,
	0.471397f,
	-0.881921f,
	0.449611f,
	-0.893224f,
	0.427555f,
	-0.903989f,
	0.405241f,
	-0.914210f,
	0.382683f,
	-0.923880f,
	0.359895f,
	-0.932993f,
	0.336890f,
	-0.941544f,
	0.313682f,
	-0.949528f,
	0.290285f,
	-0.956940f,
	0.266713f,
	-0.963776f,
	0.242980f,
	-0.970031f,
	0.219101f,
	-0.975702f,
	0.195090f,
	-0.980785f,
	0.170962f,
	-0.985278f,
	0.146730f,
	-0.989177f,
	0.122411f,
	-0.992480f,
	0.098017f,
	-0.995185f,
	0.073565f,
	-0.997290f,
	0.049068f,
	-0.998795f,
	0.024541f,
	-0.999699f,
	1.000000f,
	0.000000f,
	0.997290f,
	-0.073565f,
	0.989177f,
	-0.146730f,
	0.975702f,
	-0.219101f,
	0.956940f,
	-0.290285f,
	0.932993f,
	-0.359895f,
	0.903989f,
	-0.427555f,
	0.870087f,
	-0.492898f,
	0.831470f,
	-0.555570f,
	0.788346f,
	-0.615232f,
	0.740951f,
	-0.671559f,
	0.689541f,
	-0.724247f,
	0.634393f,
	-0.773010f,
	0.575808f,
	-0.817585f,
	0.514103f,
	-0.857729f,
	0.449611f,
	-0.893224f,
	0.382683f,
	-0.923880f,
	0.313682f,
	-0.949528f,
	0.242980f,
	-0.970031f,
	0.170962f,
	-0.985278f,
	0.098017f,
	-0.995185f,
	0.024541f,
	-0.999699f,
	-0.049068f,
	-0.998795f,
	-0.122411f,
	-0.992480f,
	-0.195090f,
	-0.980785f,
	-0.266713f,
	-0.963776f,
	-0.336890f,
	-0.941544f,
	-0.405241f,
	-0.914210f,
	-0.471397f,
	-0.881921f,
	-0.534998f,
	-0.844854f,
	-0.595699f,
	-0.803207f,
	-0.653173f,
	-0.757209f,
	-0.707107f,
	-0.707107f,
	-0.757209f,
	-0.653173f,
	-0.803208f,
	-0.595699f,
	-0.844854f,
	-0.534998f,
	-0.881921f,
	-0.471397f,
	-0.914210f,
	-0.405241f,
	-0.941544f,
	-0.336890f,
	-0.963776f,
	-0.266713f,
	-0.980785f,
	-0.195090f,
	-0.992480f,
	-0.122411f,
	-0.998795f,
	-0.049068f,
	-0.999699f,
	0.024541f,
	-0.995185f,
	0.098017f,
	-0.985278f,
	0.170962f,
	-0.970031f,
	0.242980f,
	-0.949528f,
	0.313682f,
	-0.923879f,
	0.382684f,
	-0.893224f,
	0.449611f,
	-0.857729f,
	0.514103f,
	-0.817585f,
	0.575808f,
	-0.773010f,
	0.634393f,
	-0.724247f,
	0.689541f,
	-0.671559f,
	0.740951f,
	-0.615232f,
	0.788346f,
	-0.555570f,
	0.831470f,
	-0.492898f,
	0.870087f,
	-0.427555f,
	0.903989f,
	-0.359895f,
	0.932993f,
	-0.290285f,
	0.956940f,
	-0.219101f,
	0.975702f,
	-0.146730f,
	0.989177f,
	-0.073564f,
	0.997290f
};
/***************************************************
L2_Atom的基本思想:
1.该操作是分裂基FFT算法的组成部分
2.两个输入 a + bj  c+dj
  则输出分别为  [a + bj - j * (c + dj)] * W(N,k)
				[a + bj + j * (c + dj)] * W(N,3k)
3.程序的具体实现思想
3.1 基于SSE指令，分别取出两对输入 ->xmm0,xmm1
3.2 利用SSE移位指令 计算出a + bj - j * (c + dj) 和 a + bj + j * (c + dj)
    即  a+d + (b-c)j  & a-d + (b+c)j
****************************************************/

#define L2_Atom_P128(x)                    \
	__asm{xor      ecx,ecx}                \
	__asm{lea      edx,W_P128}             \
L2_P128##x:								   \
	__asm{movaps   xmm0,[eax+ecx+x]}       \
	__asm{movaps   xmm1,[eax+ecx+200h+x]}  \
	__asm{movaps   xmm2,xmm0}              \
	__asm{shufps   xmm1,xmm1,0B1h}         \
	__asm{addps    xmm0,xmm1}              \
	__asm{subps    xmm2,xmm1}     		   \
	__asm{shufps   xmm0,xmm0,0D8h}         \
	__asm{shufps   xmm2,xmm2,8Dh}		   \
	__asm{movaps   xmm1,xmm0}			   \
	__asm{movlhps  xmm0,xmm2}              \
	__asm{movhlps  xmm2,xmm1}              \
	__asm{shufps   xmm0,xmm0,0D8h}         \
	__asm{shufps   xmm2,xmm2,72h}		   \
	__asm{movaps   xmm1,xmm0}  \
	__asm{movaps   xmm3,xmm2}  \
	__asm{movaps   xmm4,[edx+ecx]}         \
	__asm{mulps    xmm0,xmm4}  \
	__asm{shufps   xmm4,xmm4,0B1h}		   \
	__asm{mulps    xmm1,xmm4}  \
	__asm{movaps   xmm4,[edx+ecx+200h]}	   \
	__asm{mulps    xmm2,xmm4}  \
	__asm{shufps   xmm4,xmm4,0B1h}  \
	__asm{mulps    xmm3,xmm4}  \
	__asm{shufps   xmm0,xmm0,0D8h}  \
	__asm{shufps   xmm1,xmm1,0D8h}  \
	__asm{shufps   xmm2,xmm2,0D8h}  \
	__asm{shufps   xmm3,xmm3,0D8h}  \
	__asm{movaps   xmm4,xmm0}  \
	__asm{movlhps  xmm0,xmm2}  \
	__asm{movhlps  xmm2,xmm4}  \
	__asm{movaps   xmm4,xmm1}  \
	__asm{movlhps  xmm1,xmm3}  \
	__asm{movhlps  xmm3,xmm4}  \
	__asm{subps    xmm0,xmm2}  \
	__asm{addps    xmm1,xmm3}  \
	__asm{movaps   xmm4,xmm0}  \
	__asm{movlhps  xmm0,xmm1}  \
	__asm{movhlps  xmm1,xmm4}  \
	__asm{shufps   xmm0,xmm0,0D8h}  \
	__asm{shufps   xmm1,xmm1,0D8h}  \
	__asm{movaps   [eax+ecx+x],xmm0}  \
	__asm{movaps   [eax+ecx+200h+x],xmm1}  \
	__asm{add      ecx,10h}  \
	__asm{cmp      ecx,200h}  \
	__asm{jnz      L2_P128##x}

#define L1_Atom_P64(x)          \
	__asm{mov     ecx,100h}  	\
L1_P64##x:						\
	__asm{movaps  xmm0,[eax+ecx-10h+x]}	\
	__asm{movaps  xmm1,[eax+ecx+0F0h+x]}	\
	__asm{movaps  xmm2,xmm0}	\
	__asm{addps   xmm0,xmm1}	\
	__asm{subps   xmm2,xmm1}	\
	__asm{movaps  [eax+ecx-10h+x],xmm0}	\
	__asm{movaps  [eax+ecx+0F0h+x],xmm2}	\
	__asm{sub     ecx,10h}	\
	__asm{jnz     L1_P64##x}


const float SSE_ALIGN W_P64[] = 
{
	1.000000f,
	0.000000f,
	0.998795f,
	-0.049068f,
	0.995185f,
	-0.098017f,
	0.989177f,
	-0.146730f,
	0.980785f,
	-0.195090f,
	0.970031f,
	-0.242980f,
	0.956940f,
	-0.290285f,
	0.941544f,
	-0.336890f,
	0.923880f,
	-0.382683f,
	0.903989f,
	-0.427555f,
	0.881921f,
	-0.471397f,
	0.857729f,
	-0.514103f,
	0.831470f,
	-0.555570f,
	0.803208f,
	-0.595699f,
	0.773010f,
	-0.634393f,
	0.740951f,
	-0.671559f,
	0.707107f,
	-0.707107f,
	0.671559f,
	-0.740951f,
	0.634393f,
	-0.773010f,
	0.595699f,
	-0.803208f,
	0.555570f,
	-0.831470f,
	0.514103f,
	-0.857729f,
	0.471397f,
	-0.881921f,
	0.427555f,
	-0.903989f,
	0.382683f,
	-0.923880f,
	0.336890f,
	-0.941544f,
	0.290285f,
	-0.956940f,
	0.242980f,
	-0.970031f,
	0.195090f,
	-0.980785f,
	0.146730f,
	-0.989177f,
	0.098017f,
	-0.995185f,
	0.049068f,
	-0.998795f,
	1.000000f,
	0.000000f,
	0.989177f,
	-0.146730f,
	0.956940f,
	-0.290285f,
	0.903989f,
	-0.427555f,
	0.831470f,
	-0.555570f,
	0.740951f,
	-0.671559f,
	0.634393f,
	-0.773010f,
	0.514103f,
	-0.857729f,
	0.382683f,
	-0.923880f,
	0.242980f,
	-0.970031f,
	0.098017f,
	-0.995185f,
	-0.049068f,
	-0.998795f,
	-0.195090f,
	-0.980785f,
	-0.336890f,
	-0.941544f,
	-0.471397f,
	-0.881921f,
	-0.595699f,
	-0.803207f,
	-0.707107f,
	-0.707107f,
	-0.803208f,
	-0.595699f,
	-0.881921f,
	-0.471397f,
	-0.941544f,
	-0.336890f,
	-0.980785f,
	-0.195090f,
	-0.998795f,
	-0.049068f,
	-0.995185f,
	0.098017f,
	-0.970031f,
	0.242980f,
	-0.923879f,
	0.382684f,
	-0.857729f,
	0.514103f,
	-0.773010f,
	0.634393f,
	-0.671559f,
	0.740951f,
	-0.555570f,
	0.831470f,
	-0.427555f,
	0.903989f,
	-0.290285f,
	0.956940f,
	-0.146730f,
	0.989177f,
};

#define L2_Atom_P64(x)	\
	__asm{xor      ecx,ecx}	\
	__asm{lea      edx,W_P64}	\
L2_P64##x:							\
	__asm{movaps   xmm0,[eax+ecx+x]}	\
	__asm{movaps   xmm1,[eax+ecx+100h+x]}	\
	__asm{movaps   xmm2,xmm0}	\
	__asm{shufps   xmm1,xmm1,0B1h}	\
	__asm{addps    xmm0,xmm1}	\
	__asm{subps    xmm2,xmm1}	\
	__asm{shufps   xmm0,xmm0,0D8h}	\
	__asm{shufps   xmm2,xmm2,8Dh}	\
	__asm{movaps   xmm1,xmm0}	\
	__asm{movlhps  xmm0,xmm2}	\
	__asm{movhlps  xmm2,xmm1}	\
	__asm{shufps   xmm0,xmm0,0D8h}	\
	__asm{shufps   xmm2,xmm2,72h}	\
	__asm{movaps   xmm1,xmm0}	\
	__asm{movaps   xmm3,xmm2}	\
	__asm{movaps   xmm4,[edx+ecx]}	\
	__asm{mulps    xmm0,xmm4}	\
	__asm{shufps   xmm4,xmm4,0B1h}	\
	__asm{mulps    xmm1,xmm4}	\
	__asm{movaps   xmm4,[edx+ecx+100h]}	\
	__asm{mulps    xmm2,xmm4}	\
	__asm{shufps   xmm4,xmm4,0B1h}	\
	__asm{mulps    xmm3,xmm4}	\
	__asm{shufps   xmm0,xmm0,0D8h}	\
	__asm{shufps   xmm1,xmm1,0D8h}	\
	__asm{shufps   xmm2,xmm2,0D8h}	\
	__asm{shufps   xmm3,xmm3,0D8h}	\
	__asm{movaps   xmm4,xmm0}	\
	__asm{movlhps  xmm0,xmm2}	\
	__asm{movhlps  xmm2,xmm4}	\
	__asm{movaps   xmm4,xmm1}	\
	__asm{movlhps  xmm1,xmm3}	\
	__asm{movhlps  xmm3,xmm4}	\
	__asm{subps    xmm0,xmm2}	\
	__asm{addps    xmm1,xmm3}	\
	__asm{movaps   xmm4,xmm0}	\
	__asm{movlhps  xmm0,xmm1}	\
	__asm{movhlps  xmm1,xmm4}	\
	__asm{shufps	 xmm0,xmm0,0D8h}	\
	__asm{shufps   xmm1,xmm1,0D8h}	\
	__asm{movaps   [eax+ecx+x],xmm0}	\
	__asm{movaps   [eax+ecx+100h+x],xmm1}	\
	__asm{add      ecx,10h}	\
	__asm{cmp      ecx,100h}	\
	__asm{jnz      L2_P64##x}

#define L1_Atom_P32(x)        \
	__asm{mov     ecx,80h}	\
L1_P32##x:						\
	__asm{movaps  xmm0,[eax+ecx-10h+x]}	\
	__asm{movaps  xmm1,[eax+ecx+70h+x]}	\
	__asm{movaps  xmm2,xmm0}	\
	__asm{addps   xmm0,xmm1}	\
	__asm{subps   xmm2,xmm1}	\
	__asm{movaps  [eax+ecx-10h+x],xmm0}	\
	__asm{movaps  [eax+ecx+70h+x],xmm2}	\
	__asm{sub     ecx,10h}	\
	__asm{jnz     L1_P32##x}


const float SSE_ALIGN W_P32[] = 
{
	1.000000f,
	0.000000f,
	0.995185f,
	-0.098017f,
	0.980785f,
	-0.195090f,
	0.956940f,
	-0.290285f,
	0.923880f,
	-0.382683f,
	0.881921f,
	-0.471397f,
	0.831470f,
	-0.555570f,
	0.773010f,
	-0.634393f,
	0.707107f,
	-0.707107f,
	0.634393f,
	-0.773010f,
	0.555570f,
	-0.831470f,
	0.471397f,
	-0.881921f,
	0.382683f,
	-0.923880f,
	0.290285f,
	-0.956940f,
	0.195090f,
	-0.980785f,
	0.098017f,
	-0.995185f,
	1.000000f,
	0.000000f,
	0.956940f,
	-0.290285f,
	0.831470f,
	-0.555570f,
	0.634393f,
	-0.773010f,
	0.382683f,
	-0.923880f,
	0.098017f,
	-0.995185f,
	-0.195090f,
	-0.980785f,
	-0.471397f,
	-0.881921f,
	-0.707107f,
	-0.707107f,
	-0.881921f,
	-0.471397f,
	-0.980785f,
	-0.195090f,
	-0.995185f,
	0.098017f,
	-0.923879f,
	0.382684f,
	-0.773010f,
	0.634393f,
	-0.555570f,
	0.831470f,
	-0.290285f,
	0.956940f,
};

#define L2_Atom_P32(x)	\
	__asm{xor      ecx,ecx}	\
	__asm{lea      edx,W_P32}	\
L2_P32##x:							\
	__asm{movaps   xmm0,[eax+ecx+x]}	\
	__asm{movaps   xmm1,[eax+ecx+80h+x]}	\
	__asm{movaps   xmm2,xmm0}	\
	__asm{shufps   xmm1,xmm1,0B1h}	\
	__asm{addps    xmm0,xmm1}	\
	__asm{subps    xmm2,xmm1}	\
	__asm{shufps   xmm0,xmm0,0D8h}	\
	__asm{shufps   xmm2,xmm2,8Dh}	\
	__asm{movaps   xmm1,xmm0}	\
	__asm{movlhps  xmm0,xmm2}	\
	__asm{movhlps  xmm2,xmm1}	\
	__asm{shufps   xmm0,xmm0,0D8h}	\
	__asm{shufps   xmm2,xmm2,72h}	\
	__asm{movaps   xmm1,xmm0}	\
	__asm{movaps   xmm3,xmm2}	\
	__asm{movaps   xmm4,[edx+ecx]}	\
	__asm{mulps    xmm0,xmm4}	\
	__asm{shufps   xmm4,xmm4,0B1h}	\
	__asm{mulps    xmm1,xmm4}	\
	__asm{movaps   xmm4,[edx+ecx+80h]}	\
	__asm{mulps    xmm2,xmm4}	\
	__asm{shufps   xmm4,xmm4,0B1h}	\
	__asm{mulps    xmm3,xmm4}	\
	__asm{shufps   xmm0,xmm0,0D8h}	\
	__asm{shufps   xmm1,xmm1,0D8h}	\
	__asm{shufps   xmm2,xmm2,0D8h}	\
	__asm{shufps   xmm3,xmm3,0D8h}	\
	__asm{movaps   xmm4,xmm0}	\
	__asm{movlhps  xmm0,xmm2}	\
	__asm{movhlps  xmm2,xmm4}	\
	__asm{movaps   xmm4,xmm1}	\
	__asm{movlhps  xmm1,xmm3}	\
	__asm{movhlps  xmm3,xmm4}	\
	__asm{subps    xmm0,xmm2}	\
	__asm{addps    xmm1,xmm3}	\
	__asm{movaps   xmm4,xmm0}	\
	__asm{movlhps  xmm0,xmm1}	\
	__asm{movhlps  xmm1,xmm4}	\
	__asm{shufps   xmm0,xmm0,0D8h}	\
	__asm{shufps   xmm1,xmm1,0D8h}	\
	__asm{movaps   [eax+ecx+x],xmm0}	\
	__asm{movaps   [eax+ecx+80h+x],xmm1}	\
	__asm{add      ecx,10h}	\
	__asm{cmp      ecx,80h}	\
	__asm{jnz      L2_P32##x}

#define L1_Atom_P16(x)        \
	__asm{mov     ecx,40h}	\
L1_P16##x:						\
	__asm{movaps  xmm0,[eax+ecx-10h+x]}	\
	__asm{movaps  xmm1,[eax+ecx+30h+x]}	\
	__asm{movaps  xmm2,xmm0}	\
	__asm{addps   xmm0,xmm1}	\
	__asm{subps   xmm2,xmm1}	\
	__asm{movaps  [eax+ecx-10h+x],xmm0}	\
	__asm{movaps  [eax+ecx+30h+x],xmm2}	\
	__asm{sub     ecx,10h}	\
	__asm{jnz     L1_P16##x}


const float SSE_ALIGN W_P16[] = 
{
	1.000000f,
	0.000000f,
	0.980785f,
	-0.195090f,
	0.923880f,
	-0.382683f,
	0.831470f,
	-0.555570f,
	0.707107f,
	-0.707107f,
	0.555570f,
	-0.831470f,
	0.382683f,
	-0.923880f,
	0.195090f,
	-0.980785f,
	1.000000f,
	0.000000f,
	0.831470f,
	-0.555570f,
	0.382683f,
	-0.923880f,
	-0.195090f,
	-0.980785f,
	-0.707107f,
	-0.707107f,
	-0.980785f,
	-0.195090f,
	-0.923879f,
	0.382684f,
	-0.555570f,
	0.831470f,
};

#define L2_Atom_P16(x)	\
	__asm{xor      ecx,ecx}	\
	__asm{lea      edx,W_P16}	\
L2_P16##x:							\
	__asm{movaps   xmm0,[eax+ecx+x]}	\
	__asm{movaps   xmm1,[eax+ecx+40h+x]}	\
	__asm{movaps   xmm2,xmm0}	\
	__asm{shufps   xmm1,xmm1,0B1h}	\
	__asm{addps    xmm0,xmm1}	\
	__asm{subps    xmm2,xmm1}	\
	__asm{shufps   xmm0,xmm0,0D8h}	\
	__asm{shufps   xmm2,xmm2,8Dh}	\
	__asm{movaps   xmm1,xmm0}	\
	__asm{movlhps  xmm0,xmm2}	\
	__asm{movhlps  xmm2,xmm1}	\
	__asm{shufps   xmm0,xmm0,0D8h}	\
	__asm{shufps   xmm2,xmm2,72h}	\
	__asm{movaps   xmm1,xmm0}	\
	__asm{movaps   xmm3,xmm2}	\
	__asm{movaps   xmm4,[edx+ecx]}	\
	__asm{mulps    xmm0,xmm4}	\
	__asm{shufps   xmm4,xmm4,0B1h}	\
	__asm{mulps    xmm1,xmm4}	\
	__asm{movaps   xmm4,[edx+ecx+40h]}	\
	__asm{mulps    xmm2,xmm4}	\
	__asm{shufps   xmm4,xmm4,0B1h}	\
	__asm{mulps    xmm3,xmm4}	\
	__asm{shufps   xmm0,xmm0,0D8h}	\
	__asm{shufps   xmm1,xmm1,0D8h}	\
	__asm{shufps   xmm2,xmm2,0D8h}	\
	__asm{shufps   xmm3,xmm3,0D8h}	\
	__asm{movaps   xmm4,xmm0}	\
	__asm{movlhps  xmm0,xmm2}	\
	__asm{movhlps  xmm2,xmm4}	\
	__asm{movaps   xmm4,xmm1}	\
	__asm{movlhps  xmm1,xmm3}	\
	__asm{movhlps  xmm3,xmm4}	\
	__asm{subps    xmm0,xmm2}	\
	__asm{addps    xmm1,xmm3}	\
	__asm{movaps   xmm4,xmm0}	\
	__asm{movlhps  xmm0,xmm1}	\
	__asm{movhlps  xmm1,xmm4}	\
	__asm{shufps   xmm0,xmm0,0D8h}	\
	__asm{shufps   xmm1,xmm1,0D8h}	\
	__asm{movaps   [eax+ecx+x],xmm0}	\
	__asm{movaps   [eax+ecx+40h+x],xmm1}	\
	__asm{add      ecx,10h}	\
	__asm{cmp      ecx,40h}	\
	__asm{jnz      L2_P16##x}

const float SSE_ALIGN W_P8[] = 
{
	1.000000f,
	0.000000f,
	0.923880f,
	-0.382683f,
	0.707107f,
	-0.707107f,
	0.382683f,
	-0.923880f,
	1.000000f,
	0.000000f,
	0.382683f,
	-0.923880f,
	-0.707107f,
	-0.707107f,
	-0.923879f,
	0.382684f,
};

const float SSE_ALIGN W_P4[] = 
{
	1.0f,
	0.0f,
	0.707107f,
	-0.707107f,
	1.0f,
	0.0f,
	-0.707107f,
	-0.707107f
};

#define L1_Atom_P8_4_2(x)           \
	__asm{movaps  xmm0,[eax+x]}	    \
	__asm{movaps  xmm1,[eax+10h+x]}	\
	__asm{movaps  xmm4,[eax+20h+x]}	\
	__asm{movaps  xmm5,[eax+30h+x]}	\
	__asm{movaps  xmm2,xmm0}	    \
	__asm{addps   xmm0,xmm4}	    \
	__asm{subps   xmm2,xmm4}	    \
	__asm{movaps  xmm3,xmm1}	    \
	__asm{addps   xmm1,xmm5}	    \
	__asm{subps   xmm3,xmm5}	    \
	/* L1_P8计算完毕，分别存储在xmm0[0],xmm1[10h],xmm2[20h],xmm3[30h] */  \
	__asm{movaps  xmm4,xmm0}	    \
	__asm{addps   xmm0,xmm1}	    \
	__asm{subps   xmm4,xmm1}	    \
	__asm{shufps  xmm4,xmm4,0B4h}	\
	__asm{movaps  xmm1,xmm0}		\
	__asm{movlhps xmm0,xmm4}		\
	__asm{movhlps xmm4,xmm1}		\
	__asm{movaps  xmm1,xmm0}        \
	__asm{addps   xmm0,xmm4}        \
	__asm{subps   xmm1,xmm4}        \
	__asm{movaps  xmm4,xmm0}        \
	__asm{movlhps xmm0,xmm1}        \
	__asm{movhlps xmm1,xmm4}        \
	__asm{shufps  xmm1,xmm1,6Ch}    \
	/* L1_P4_2计算完毕，存储在xmm0[0],xmm1[10h] */  \
	__asm{lea     edx,W_P4}			\
	__asm{movaps  xmm4,xmm2}		\
	__asm{shufps  xmm3,xmm3,0B1h}	\
	__asm{addps   xmm2,xmm3}	    \
	__asm{subps   xmm4,xmm3}	    \
	__asm{shufps  xmm2,xmm2,0D8h}	\
	__asm{shufps  xmm4,xmm4,8Dh}	\
	__asm{movaps  xmm3,xmm2}	    \
	__asm{movlhps xmm2,xmm4}		\
	__asm{movhlps xmm4,xmm3}		\
	__asm{shufps  xmm2,xmm2,0D8h}	\
	__asm{shufps  xmm4,xmm4,72h}	\
	__asm{movaps  xmm3,xmm2}		\
	__asm{movaps  xmm5,xmm4}		\
	__asm{movaps  xmm6,[edx]}		\
	__asm{mulps   xmm2,xmm6}		\
	__asm{shufps  xmm6,xmm6,0B1h}	\
	__asm{mulps   xmm3,xmm6}		\
	__asm{movaps  xmm6,[edx+10h]}	\
	__asm{mulps   xmm4,xmm6}		\
	__asm{shufps  xmm6,xmm6,0B1h}	\
	__asm{mulps   xmm5,xmm6}		\
	__asm{shufps  xmm2,xmm2,0D8h}	\
	__asm{shufps  xmm3,xmm3,0D8h}	\
	__asm{shufps  xmm4,xmm4,0D8h}	\
	__asm{shufps  xmm5,xmm5,0D8h}	\
	__asm{movaps  xmm6,xmm2}		\
	__asm{movlhps xmm2,xmm4}		\
	__asm{movhlps xmm4,xmm6}		\
	__asm{movaps  xmm6,xmm3}		\
	__asm{movlhps xmm3,xmm5}		\
	__asm{movhlps xmm5,xmm6}		\
	__asm{subps   xmm2,xmm4}		\
	__asm{addps   xmm3,xmm5}		\
	__asm{movaps  xmm6,xmm2}		\
	__asm{movlhps xmm2,xmm3}		\
	__asm{movhlps xmm3,xmm6}		\
	__asm{shufps  xmm2,xmm2,0D8h}	\
	__asm{shufps  xmm3,xmm3,0D8h}	\
	__asm{movaps  xmm4,xmm2}		\
	__asm{movlhps xmm2,xmm3}		\
	__asm{movhlps xmm3,xmm4}		\
	__asm{movaps  xmm4,xmm2}		\
	__asm{addps   xmm2,xmm3}		\
	__asm{subps   xmm4,xmm3}		\
	__asm{movaps  xmm3,xmm2}		\
	__asm{movlhps xmm2,xmm4}		\
	__asm{movhlps xmm4,xmm3}		\
	__asm{movaps  [eax+x],xmm0}     \
	__asm{movaps  [eax+10h+x],xmm1} \
	__asm{movaps  [eax+20h+x],xmm2}	\
	__asm{movaps  [eax+30h+x],xmm4}  

#define L2_Atom_P8_4_2(x)				    \
	__asm{lea      edx,W_P8}			    \
	__asm{movaps   xmm0,[eax+x]}			\
	__asm{movaps   xmm2,[eax+10h+x]}		\
	__asm{movaps   xmm1,[eax+20h+x]}		\
	__asm{movaps   xmm3,[eax+30h+x]}		\
	__asm{movaps   xmm4,xmm0}				\
	__asm{shufps   xmm1,xmm1,0B1h}			\
	__asm{addps    xmm0,xmm1}				\
	__asm{subps    xmm4,xmm1}				\
	__asm{shufps   xmm0,xmm0,0D8h}			\
	__asm{shufps   xmm4,xmm4,8Dh}			\
	__asm{movaps   xmm1,xmm0}				\
	__asm{movlhps  xmm0,xmm4}				\
	__asm{movhlps  xmm4,xmm1}				\
	__asm{shufps   xmm0,xmm0,0D8h}			\
	__asm{shufps   xmm4,xmm4,72h}			\
	__asm{movaps   xmm1,xmm0}				\
	__asm{movaps   xmm5,xmm4}				\
	__asm{movaps   xmm6,[edx]}				\
	__asm{mulps    xmm0,xmm6}				\
	__asm{shufps   xmm6,xmm6,0B1h}			\
	__asm{mulps    xmm1,xmm6}				\
	__asm{movaps   xmm6,[edx+20h]}			\
	__asm{mulps    xmm4,xmm6}				\
	__asm{shufps   xmm6,xmm6,0B1h}			\
	__asm{mulps    xmm5,xmm6}				\
	__asm{shufps   xmm0,xmm0,0D8h}			\
	__asm{shufps   xmm1,xmm1,0D8h}			\
	__asm{shufps   xmm4,xmm4,0D8h}			\
	__asm{shufps   xmm5,xmm5,0D8h}			\
	__asm{movaps   xmm6,xmm0}				\
	__asm{movlhps  xmm0,xmm4}				\
	__asm{movhlps  xmm4,xmm6}				\
	__asm{movaps   xmm6,xmm1}				\
	__asm{movlhps  xmm1,xmm5}				\
	__asm{movhlps  xmm5,xmm6}				\
	__asm{subps    xmm0,xmm4}				\
	__asm{addps    xmm1,xmm5}				\
	__asm{movaps   xmm6,xmm0}				\
	__asm{movlhps  xmm0,xmm1}				\
	__asm{movhlps  xmm1,xmm6}				\
	__asm{shufps   xmm0,xmm0,0D8h}			\
	__asm{shufps   xmm1,xmm1,0D8h}			\
	__asm{movaps   xmm4,xmm2}				\
	__asm{shufps   xmm3,xmm3,0B1h}			\
	__asm{addps    xmm2,xmm3}				\
	__asm{subps    xmm4,xmm3}				\
	__asm{shufps   xmm2,xmm2,0D8h}			\
	__asm{shufps   xmm4,xmm4,8Dh}			\
	__asm{movaps   xmm3,xmm2}				\
	__asm{movlhps  xmm2,xmm4}				\
	__asm{movhlps  xmm4,xmm3}				\
	__asm{shufps   xmm2,xmm2,0D8h}			\
	__asm{shufps   xmm4,xmm4,72h}			\
	__asm{movaps   xmm3,xmm2}				\
	__asm{movaps   xmm5,xmm4}				\
	__asm{movaps   xmm6,[edx+10h]}			\
	__asm{mulps    xmm2,xmm6}				\
	__asm{shufps   xmm6,xmm6,0B1h}			\
	__asm{mulps    xmm3,xmm6}				\
	__asm{movaps   xmm6,[edx+30h]}			\
	__asm{mulps    xmm4,xmm6}				\
	__asm{shufps   xmm6,xmm6,0B1h}			\
	__asm{mulps    xmm5,xmm6}				\
	__asm{shufps   xmm2,xmm2,0D8h}			\
	__asm{shufps   xmm3,xmm3,0D8h}			\
	__asm{shufps   xmm4,xmm4,0D8h}			\
	__asm{shufps   xmm5,xmm5,0D8h}			\
	__asm{movaps   xmm6,xmm2}				\
	__asm{movlhps  xmm2,xmm4}				\
	__asm{movhlps  xmm4,xmm6}				\
	__asm{movaps   xmm6,xmm3}				\
	__asm{movlhps  xmm3,xmm5}				\
	__asm{movhlps  xmm5,xmm6}				\
	__asm{subps    xmm2,xmm4}				\
	__asm{addps    xmm3,xmm5}				\
	__asm{movaps   xmm6,xmm2}				\
	__asm{movlhps  xmm2,xmm3}				\
	__asm{movhlps  xmm3,xmm6}				\
	__asm{shufps   xmm2,xmm2,0D8h}			\
	__asm{shufps   xmm3,xmm3,0D8h}			\
	/* L2_P8完毕，分别存储在xmm0[0],xmm2[10h],xmm1[20h],xmm3[30h] */ \
	__asm{movaps  xmm4,xmm0}	    \
	__asm{addps   xmm0,xmm2}	    \
	__asm{subps   xmm4,xmm2}	    \
	__asm{shufps  xmm4,xmm4,0B4h}   \
	__asm{movaps  xmm2,xmm0}        \
	__asm{movlhps xmm0,xmm4}        \
	__asm{movhlps xmm4,xmm2}        \
	__asm{movaps  xmm2,xmm0}        \
	__asm{addps   xmm0,xmm4}        \
	__asm{subps   xmm2,xmm4}        \
	__asm{movaps  xmm4,xmm0}        \
	__asm{movlhps xmm0,xmm2}        \
	__asm{movhlps xmm2,xmm4}        \
	__asm{shufps  xmm2,xmm2,6Ch}    \
	/* 第一个L2_P4_2完毕，分别存储在xmm0[0],xmm2[10h] */ \
	__asm{movaps  xmm4,xmm1}	    \
	__asm{addps   xmm1,xmm3}	    \
	__asm{subps   xmm4,xmm3}	    \
	__asm{shufps  xmm4,xmm4,0B4h}   \
	__asm{movaps  xmm3,xmm1}        \
	__asm{movlhps xmm1,xmm4}        \
	__asm{movhlps xmm4,xmm3}        \
	__asm{movaps  xmm3,xmm1}        \
	__asm{addps   xmm1,xmm4}        \
	__asm{subps   xmm3,xmm4}        \
	__asm{movaps  xmm4,xmm1}        \
	__asm{movlhps xmm1,xmm3}        \
	__asm{movhlps xmm3,xmm4}        \
	__asm{shufps  xmm3,xmm3,6Ch}    \
	/* 第一个L2_P4_2完毕，分别存储在xmm1[20h],xmm3[30h] */ \
	__asm{movaps  [eax+x],xmm0}     \
	__asm{movaps  [eax+10h+x],xmm2} \
	__asm{movaps  [eax+20h+x],xmm1} \
	__asm{movaps  [eax+30h+x],xmm3} 

#define P128_Reverse           \
	__asm{movsd xmm0,[eax+128]} \
	__asm{movsd xmm1,[eax+32]} \
	__asm{movsd [eax+32],xmm0} \
	__asm{movsd [eax+128],xmm1} \
	__asm{movsd xmm0,[eax+192]} \
	__asm{movsd xmm1,[eax+96]} \
	__asm{movsd [eax+96],xmm0} \
	__asm{movsd [eax+192],xmm1} \
	__asm{movsd xmm0,[eax+256]} \
	__asm{movsd xmm1,[eax+16]} \
	__asm{movsd [eax+16],xmm0} \
	__asm{movsd [eax+256],xmm1} \
	__asm{movsd xmm0,[eax+288]} \
	__asm{movsd xmm1,[eax+144]} \
	__asm{movsd [eax+144],xmm0} \
	__asm{movsd [eax+288],xmm1} \
	__asm{movsd xmm0,[eax+320]} \
	__asm{movsd xmm1,[eax+80]} \
	__asm{movsd [eax+80],xmm0} \
	__asm{movsd [eax+320],xmm1} \
	__asm{movsd xmm0,[eax+352]} \
	__asm{movsd xmm1,[eax+208]} \
	__asm{movsd [eax+208],xmm0} \
	__asm{movsd [eax+352],xmm1} \
	__asm{movsd xmm0,[eax+384]} \
	__asm{movsd xmm1,[eax+48]} \
	__asm{movsd [eax+48],xmm0} \
	__asm{movsd [eax+384],xmm1} \
	__asm{movsd xmm0,[eax+400]} \
	__asm{movsd xmm1,[eax+304]} \
	__asm{movsd [eax+304],xmm0} \
	__asm{movsd [eax+400],xmm1} \
	__asm{movsd xmm0,[eax+416]} \
	__asm{movsd xmm1,[eax+176]} \
	__asm{movsd [eax+176],xmm0} \
	__asm{movsd [eax+416],xmm1} \
	__asm{movsd xmm0,[eax+448]} \
	__asm{movsd xmm1,[eax+112]} \
	__asm{movsd [eax+112],xmm0} \
	__asm{movsd [eax+448],xmm1} \
	__asm{movsd xmm0,[eax+464]} \
	__asm{movsd xmm1,[eax+368]} \
	__asm{movsd [eax+368],xmm0} \
	__asm{movsd [eax+464],xmm1} \
	__asm{movsd xmm0,[eax+480]} \
	__asm{movsd xmm1,[eax+240]} \
	__asm{movsd [eax+240],xmm0} \
	__asm{movsd [eax+480],xmm1} \
	__asm{movsd xmm0,[eax+512]} \
	__asm{movsd xmm1,[eax+8]} \
	__asm{movsd [eax+8],xmm0} \
	__asm{movsd [eax+512],xmm1} \
	__asm{movsd xmm0,[eax+528]} \
	__asm{movsd xmm1,[eax+264]} \
	__asm{movsd [eax+264],xmm0} \
	__asm{movsd [eax+528],xmm1} \
	__asm{movsd xmm0,[eax+544]} \
	__asm{movsd xmm1,[eax+136]} \
	__asm{movsd [eax+136],xmm0} \
	__asm{movsd [eax+544],xmm1} \
	__asm{movsd xmm0,[eax+560]} \
	__asm{movsd xmm1,[eax+392]} \
	__asm{movsd [eax+392],xmm0} \
	__asm{movsd [eax+560],xmm1} \
	__asm{movsd xmm0,[eax+576]} \
	__asm{movsd xmm1,[eax+72]} \
	__asm{movsd [eax+72],xmm0} \
	__asm{movsd [eax+576],xmm1} \
	__asm{movsd xmm0,[eax+592]} \
	__asm{movsd xmm1,[eax+328]} \
	__asm{movsd [eax+328],xmm0} \
	__asm{movsd [eax+592],xmm1} \
	__asm{movsd xmm0,[eax+608]} \
	__asm{movsd xmm1,[eax+200]} \
	__asm{movsd [eax+200],xmm0} \
	__asm{movsd [eax+608],xmm1} \
	__asm{movsd xmm0,[eax+624]} \
	__asm{movsd xmm1,[eax+456]} \
	__asm{movsd [eax+456],xmm0} \
	__asm{movsd [eax+624],xmm1} \
	__asm{movsd xmm0,[eax+640]} \
	__asm{movsd xmm1,[eax+40]} \
	__asm{movsd [eax+40],xmm0} \
	__asm{movsd [eax+640],xmm1} \
	__asm{movsd xmm0,[eax+648]} \
	__asm{movsd xmm1,[eax+552]} \
	__asm{movsd [eax+552],xmm0} \
	__asm{movsd [eax+648],xmm1} \
	__asm{movsd xmm0,[eax+656]} \
	__asm{movsd xmm1,[eax+296]} \
	__asm{movsd [eax+296],xmm0} \
	__asm{movsd [eax+656],xmm1} \
	__asm{movsd xmm0,[eax+672]} \
	__asm{movsd xmm1,[eax+168]} \
	__asm{movsd [eax+168],xmm0} \
	__asm{movsd [eax+672],xmm1} \
	__asm{movsd xmm0,[eax+688]} \
	__asm{movsd xmm1,[eax+424]} \
	__asm{movsd [eax+424],xmm0} \
	__asm{movsd [eax+688],xmm1} \
	__asm{movsd xmm0,[eax+704]} \
	__asm{movsd xmm1,[eax+104]} \
	__asm{movsd [eax+104],xmm0} \
	__asm{movsd [eax+704],xmm1} \
	__asm{movsd xmm0,[eax+712]} \
	__asm{movsd xmm1,[eax+616]} \
	__asm{movsd [eax+616],xmm0} \
	__asm{movsd [eax+712],xmm1} \
	__asm{movsd xmm0,[eax+720]} \
	__asm{movsd xmm1,[eax+360]} \
	__asm{movsd [eax+360],xmm0} \
	__asm{movsd [eax+720],xmm1} \
	__asm{movsd xmm0,[eax+736]} \
	__asm{movsd xmm1,[eax+232]} \
	__asm{movsd [eax+232],xmm0} \
	__asm{movsd [eax+736],xmm1} \
	__asm{movsd xmm0,[eax+752]} \
	__asm{movsd xmm1,[eax+488]} \
	__asm{movsd [eax+488],xmm0} \
	__asm{movsd [eax+752],xmm1} \
	__asm{movsd xmm0,[eax+768]} \
	__asm{movsd xmm1,[eax+24]} \
	__asm{movsd [eax+24],xmm0} \
	__asm{movsd [eax+768],xmm1} \
	__asm{movsd xmm0,[eax+776]} \
	__asm{movsd xmm1,[eax+536]} \
	__asm{movsd [eax+536],xmm0} \
	__asm{movsd [eax+776],xmm1} \
	__asm{movsd xmm0,[eax+784]} \
	__asm{movsd xmm1,[eax+280]} \
	__asm{movsd [eax+280],xmm0} \
	__asm{movsd [eax+784],xmm1} \
	__asm{movsd xmm0,[eax+800]} \
	__asm{movsd xmm1,[eax+152]} \
	__asm{movsd [eax+152],xmm0} \
	__asm{movsd [eax+800],xmm1} \
	__asm{movsd xmm0,[eax+808]} \
	__asm{movsd xmm1,[eax+664]} \
	__asm{movsd [eax+664],xmm0} \
	__asm{movsd [eax+808],xmm1} \
	__asm{movsd xmm0,[eax+816]} \
	__asm{movsd xmm1,[eax+408]} \
	__asm{movsd [eax+408],xmm0} \
	__asm{movsd [eax+816],xmm1} \
	__asm{movsd xmm0,[eax+832]} \
	__asm{movsd xmm1,[eax+88]} \
	__asm{movsd [eax+88],xmm0} \
	__asm{movsd [eax+832],xmm1} \
	__asm{movsd xmm0,[eax+840]} \
	__asm{movsd xmm1,[eax+600]} \
	__asm{movsd [eax+600],xmm0} \
	__asm{movsd [eax+840],xmm1} \
	__asm{movsd xmm0,[eax+848]} \
	__asm{movsd xmm1,[eax+344]} \
	__asm{movsd [eax+344],xmm0} \
	__asm{movsd [eax+848],xmm1} \
	__asm{movsd xmm0,[eax+864]} \
	__asm{movsd xmm1,[eax+216]} \
	__asm{movsd [eax+216],xmm0} \
	__asm{movsd [eax+864],xmm1} \
	__asm{movsd xmm0,[eax+872]} \
	__asm{movsd xmm1,[eax+728]} \
	__asm{movsd [eax+728],xmm0} \
	__asm{movsd [eax+872],xmm1} \
	__asm{movsd xmm0,[eax+880]} \
	__asm{movsd xmm1,[eax+472]} \
	__asm{movsd [eax+472],xmm0} \
	__asm{movsd [eax+880],xmm1} \
	__asm{movsd xmm0,[eax+896]} \
	__asm{movsd xmm1,[eax+56]} \
	__asm{movsd [eax+56],xmm0} \
	__asm{movsd [eax+896],xmm1} \
	__asm{movsd xmm0,[eax+904]} \
	__asm{movsd xmm1,[eax+568]} \
	__asm{movsd [eax+568],xmm0} \
	__asm{movsd [eax+904],xmm1} \
	__asm{movsd xmm0,[eax+912]} \
	__asm{movsd xmm1,[eax+312]} \
	__asm{movsd [eax+312],xmm0} \
	__asm{movsd [eax+912],xmm1} \
	__asm{movsd xmm0,[eax+920]} \
	__asm{movsd xmm1,[eax+824]} \
	__asm{movsd [eax+824],xmm0} \
	__asm{movsd [eax+920],xmm1} \
	__asm{movsd xmm0,[eax+928]} \
	__asm{movsd xmm1,[eax+184]} \
	__asm{movsd [eax+184],xmm0} \
	__asm{movsd [eax+928],xmm1} \
	__asm{movsd xmm0,[eax+936]} \
	__asm{movsd xmm1,[eax+696]} \
	__asm{movsd [eax+696],xmm0} \
	__asm{movsd [eax+936],xmm1} \
	__asm{movsd xmm0,[eax+944]} \
	__asm{movsd xmm1,[eax+440]} \
	__asm{movsd [eax+440],xmm0} \
	__asm{movsd [eax+944],xmm1} \
	__asm{movsd xmm0,[eax+960]} \
	__asm{movsd xmm1,[eax+120]} \
	__asm{movsd [eax+120],xmm0} \
	__asm{movsd [eax+960],xmm1} \
	__asm{movsd xmm0,[eax+968]} \
	__asm{movsd xmm1,[eax+632]} \
	__asm{movsd [eax+632],xmm0} \
	__asm{movsd [eax+968],xmm1} \
	__asm{movsd xmm0,[eax+976]} \
	__asm{movsd xmm1,[eax+376]} \
	__asm{movsd [eax+376],xmm0} \
	__asm{movsd [eax+976],xmm1} \
	__asm{movsd xmm0,[eax+984]} \
	__asm{movsd xmm1,[eax+888]} \
	__asm{movsd [eax+888],xmm0} \
	__asm{movsd [eax+984],xmm1} \
	__asm{movsd xmm0,[eax+992]} \
	__asm{movsd xmm1,[eax+248]} \
	__asm{movsd [eax+248],xmm0} \
	__asm{movsd [eax+992],xmm1} \
	__asm{movsd xmm0,[eax+1000]} \
	__asm{movsd xmm1,[eax+760]} \
	__asm{movsd [eax+760],xmm0} \
	__asm{movsd [eax+1000],xmm1} \
	__asm{movsd xmm0,[eax+1008]} \
	__asm{movsd xmm1,[eax+504]} \
	__asm{movsd [eax+504],xmm0} \
	__asm{movsd [eax+1008],xmm1}

const float SSE_ALIGN half_vector[] =
{
	0.5f,
	0.5f,
	0.5f,
	0.5f
};
const float SSE_ALIGN CTR_W_P128[] = 
{
	0.999699f,
	0.998795f,
	-0.999699f,
	-0.998795f,
	-0.024541f,
	-0.049068f,
	-0.024541f,
	-0.049068f,
	-0.999699f,
	-0.998795f,
	0.999699f,
	0.998795f,
	0.024541f,
	0.049068f,
	0.024541f,
	0.049068f,
	0.997290f,
	0.995185f,
	-0.997290f,
	-0.995185f,
	-0.073565f,
	-0.098017f,
	-0.073565f,
	-0.098017f,
	-0.997290f,
	-0.995185f,
	0.997290f,
	0.995185f,
	0.073564f,
	0.098017f,
	0.073564f,
	0.098017f,
	0.992480f,
	0.989177f,
	-0.992480f,
	-0.989177f,
	-0.122411f,
	-0.146730f,
	-0.122411f,
	-0.146730f,
	-0.992480f,
	-0.989177f,
	0.992480f,
	0.989177f,
	0.122411f,
	0.146730f,
	0.122411f,
	0.146730f,
	0.985278f,
	0.980785f,
	-0.985278f,
	-0.980785f,
	-0.170962f,
	-0.195090f,
	-0.170962f,
	-0.195090f,
	-0.985278f,
	-0.980785f,
	0.985278f,
	0.980785f,
	0.170962f,
	0.195090f,
	0.170962f,
	0.195090f,
	0.975702f,
	0.970031f,
	-0.975702f,
	-0.970031f,
	-0.219101f,
	-0.242980f,
	-0.219101f,
	-0.242980f,
	-0.975702f,
	-0.970031f,
	0.975702f,
	0.970031f,
	0.219101f,
	0.242980f,
	0.219101f,
	0.242980f,
	0.963776f,
	0.956940f,
	-0.963776f,
	-0.956940f,
	-0.266713f,
	-0.290285f,
	-0.266713f,
	-0.290285f,
	-0.963776f,
	-0.956940f,
	0.963776f,
	0.956940f,
	0.266713f,
	0.290285f,
	0.266713f,
	0.290285f,
	0.949528f,
	0.941544f,
	-0.949528f,
	-0.941544f,
	-0.313682f,
	-0.336890f,
	-0.313682f,
	-0.336890f,
	-0.949528f,
	-0.941544f,
	0.949528f,
	0.941544f,
	0.313682f,
	0.336890f,
	0.313682f,
	0.336890f,
	0.932993f,
	0.923880f,
	-0.932993f,
	-0.923880f,
	-0.359895f,
	-0.382683f,
	-0.359895f,
	-0.382683f,
	-0.932993f,
	-0.923880f,
	0.932993f,
	0.923880f,
	0.359895f,
	0.382683f,
	0.359895f,
	0.382683f,
	0.914210f,
	0.903989f,
	-0.914210f,
	-0.903989f,
	-0.405241f,
	-0.427555f,
	-0.405241f,
	-0.427555f,
	-0.914210f,
	-0.903989f,
	0.914210f,
	0.903989f,
	0.405241f,
	0.427555f,
	0.405241f,
	0.427555f,
	0.893224f,
	0.881921f,
	-0.893224f,
	-0.881921f,
	-0.449611f,
	-0.471397f,
	-0.449611f,
	-0.471397f,
	-0.893224f,
	-0.881921f,
	0.893224f,
	0.881921f,
	0.449611f,
	0.471397f,
	0.449611f,
	0.471397f,
	0.870087f,
	0.857729f,
	-0.870087f,
	-0.857729f,
	-0.492898f,
	-0.514103f,
	-0.492898f,
	-0.514103f,
	-0.870087f,
	-0.857729f,
	0.870087f,
	0.857729f,
	0.492898f,
	0.514103f,
	0.492898f,
	0.514103f,
	0.844854f,
	0.831470f,
	-0.844854f,
	-0.831470f,
	-0.534998f,
	-0.555570f,
	-0.534998f,
	-0.555570f,
	-0.844854f,
	-0.831470f,
	0.844854f,
	0.831470f,
	0.534998f,
	0.555570f,
	0.534998f,
	0.555570f,
	0.817585f,
	0.803208f,
	-0.817585f,
	-0.803208f,
	-0.575808f,
	-0.595699f,
	-0.575808f,
	-0.595699f,
	-0.817585f,
	-0.803208f,
	0.817585f,
	0.803208f,
	0.575808f,
	0.595699f,
	0.575808f,
	0.595699f,
	0.788346f,
	0.773010f,
	-0.788346f,
	-0.773010f,
	-0.615232f,
	-0.634393f,
	-0.615232f,
	-0.634393f,
	-0.788346f,
	-0.773010f,
	0.788346f,
	0.773010f,
	0.615232f,
	0.634393f,
	0.615232f,
	0.634393f,
	0.757209f,
	0.740951f,
	-0.757209f,
	-0.740951f,
	-0.653173f,
	-0.671559f,
	-0.653173f,
	-0.671559f,
	-0.757209f,
	-0.740951f,
	0.757209f,
	0.740951f,
	0.653173f,
	0.671559f,
	0.653173f,
	0.671559f,
	0.724247f,
	0.707107f,
	-0.724247f,
	-0.707107f,
	-0.689541f,
	-0.707107f,
	-0.689541f,
	-0.707107f,
	-0.724247f,
	-0.707107f,
	0.724247f,
	0.707107f,
	0.689540f,
	0.707107f,
	0.689540f,
	0.707107f,
	0.689541f,
	0.671559f,
	-0.689541f,
	-0.671559f,
	-0.724247f,
	-0.740951f,
	-0.724247f,
	-0.740951f,
	-0.689541f,
	-0.671559f,
	0.689541f,
	0.671559f,
	0.724247f,
	0.740951f,
	0.724247f,
	0.740951f,
	0.653173f,
	0.634393f,
	-0.653173f,
	-0.634393f,
	-0.757209f,
	-0.773010f,
	-0.757209f,
	-0.773010f,
	-0.653173f,
	-0.634393f,
	0.653173f,
	0.634393f,
	0.757209f,
	0.773010f,
	0.757209f,
	0.773010f,
	0.615232f,
	0.595699f,
	-0.615232f,
	-0.595699f,
	-0.788346f,
	-0.803208f,
	-0.788346f,
	-0.803208f,
	-0.615232f,
	-0.595699f,
	0.615232f,
	0.595699f,
	0.788346f,
	0.803207f,
	0.788346f,
	0.803207f,
	0.575808f,
	0.555570f,
	-0.575808f,
	-0.555570f,
	-0.817585f,
	-0.831470f,
	-0.817585f,
	-0.831470f,
	-0.575808f,
	-0.555570f,
	0.575808f,
	0.555570f,
	0.817585f,
	0.831470f,
	0.817585f,
	0.831470f,
	0.534998f,
	0.514103f,
	-0.534998f,
	-0.514103f,
	-0.844854f,
	-0.857729f,
	-0.844854f,
	-0.857729f,
	-0.534998f,
	-0.514103f,
	0.534998f,
	0.514103f,
	0.844854f,
	0.857729f,
	0.844854f,
	0.857729f,
	0.492898f,
	0.471397f,
	-0.492898f,
	-0.471397f,
	-0.870087f,
	-0.881921f,
	-0.870087f,
	-0.881921f,
	-0.492898f,
	-0.471397f,
	0.492898f,
	0.471397f,
	0.870087f,
	0.881921f,
	0.870087f,
	0.881921f,
	0.449611f,
	0.427555f,
	-0.449611f,
	-0.427555f,
	-0.893224f,
	-0.903989f,
	-0.893224f,
	-0.903989f,
	-0.449611f,
	-0.427555f,
	0.449611f,
	0.427555f,
	0.893224f,
	0.903989f,
	0.893224f,
	0.903989f,
	0.405241f,
	0.382683f,
	-0.405241f,
	-0.382683f,
	-0.914210f,
	-0.923880f,
	-0.914210f,
	-0.923880f,
	-0.405241f,
	-0.382683f,
	0.405241f,
	0.382683f,
	0.914210f,
	0.923880f,
	0.914210f,
	0.923880f,
	0.359895f,
	0.336890f,
	-0.359895f,
	-0.336890f,
	-0.932993f,
	-0.941544f,
	-0.932993f,
	-0.941544f,
	-0.359895f,
	-0.336890f,
	0.359895f,
	0.336890f,
	0.932993f,
	0.941544f,
	0.932993f,
	0.941544f,
	0.313682f,
	0.290285f,
	-0.313682f,
	-0.290285f,
	-0.949528f,
	-0.956940f,
	-0.949528f,
	-0.956940f,
	-0.313682f,
	-0.290285f,
	0.313682f,
	0.290285f,
	0.949528f,
	0.956940f,
	0.949528f,
	0.956940f,
	0.266713f,
	0.242980f,
	-0.266713f,
	-0.242980f,
	-0.963776f,
	-0.970031f,
	-0.963776f,
	-0.970031f,
	-0.266713f,
	-0.242980f,
	0.266713f,
	0.242980f,
	0.963776f,
	0.970031f,
	0.963776f,
	0.970031f,
	0.219101f,
	0.195090f,
	-0.219101f,
	-0.195090f,
	-0.975702f,
	-0.980785f,
	-0.975702f,
	-0.980785f,
	-0.219101f,
	-0.195090f,
	0.219101f,
	0.195090f,
	0.975702f,
	0.980785f,
	0.975702f,
	0.980785f,
	0.170962f,
	0.146730f,
	-0.170962f,
	-0.146730f,
	-0.985278f,
	-0.989177f,
	-0.985278f,
	-0.989177f,
	-0.170962f,
	-0.146731f,
	0.170962f,
	0.146731f,
	0.985278f,
	0.989177f,
	0.985278f,
	0.989177f,
	0.122411f,
	0.098017f,
	-0.122411f,
	-0.098017f,
	-0.992480f,
	-0.995185f,
	-0.992480f,
	-0.995185f,
	-0.122411f,
	-0.098017f,
	0.122411f,
	0.098017f,
	0.992480f,
	0.995185f,
	0.992480f,
	0.995185f,
	0.073565f,
	0.049068f,
	-0.073565f,
	-0.049068f,
	-0.997290f,
	-0.998795f,
	-0.997290f,
	-0.998795f,
	-0.073565f,
	-0.049068f,
	0.073565f,
	0.049068f,
	0.997290f,
	0.998795f,
	0.997290f,
	0.998795f,
	0.024541f,
	-0.000000f,
	-0.024541f,
	0.000000f,
	-0.999699f,
	-1.000000f,
	-0.999699f,
	-1.000000f,
	-0.024541f,
	-0.000000f,
	0.024541f,
	0.000000f,
	0.999699f,
	1.000000f,
	0.999699f,
	1.000000f
};

#define P128_ctr_trans  \
		__asm{mov     ecx,04h} \
		__asm{mov     edx,1F8h} \
		__asm{lea     esi,CTR_W_P128} \
		__asm{movaps  xmm7,half_vector} \
		__asm{fld     [eax]} \
		__asm{fadd    [eax+4h]} \
		__asm{fstp    [edi]} \
L_P128_ctr_trans:		     \
		__asm{movups  xmm0,[eax+ecx*2]} \
		__asm{movups  xmm5,[eax+ecx*2+10h]} \
		__asm{movaps  xmm1,[eax+edx*2]} \
		__asm{movaps  xmm4,[eax+edx*2-10h]} \
		__asm{movaps  xmm2,xmm0} \
		__asm{movaps  xmm6,xmm5} \
		__asm{shufps  xmm1,xmm1,4Eh} \
		__asm{shufps  xmm4,xmm4,4Eh} \
		__asm{addps   xmm0,xmm1} \
		__asm{addps   xmm5,xmm4} \
		__asm{subps   xmm2,xmm1} \
		__asm{subps   xmm6,xmm4} \
		__asm{mulps   xmm0,xmm7} \
		__asm{mulps   xmm5,xmm7} \
		__asm{mulps   xmm2,xmm7} \
		__asm{mulps   xmm6,xmm7} \
		__asm{shufps  xmm0,xmm0,0D8h} \
		__asm{shufps  xmm2,xmm2,8Dh} \
		__asm{shufps  xmm5,xmm5,0D8h} \
		__asm{shufps  xmm6,xmm6,8Dh} \
		__asm{movaps  xmm3,xmm0} \
		__asm{movlhps xmm0,xmm2} \
		__asm{movhlps xmm2,xmm3} \
		__asm{movaps  xmm3,xmm5} \
		__asm{movlhps xmm5,xmm6} \
		__asm{movhlps xmm6,xmm3} \
		__asm{movaps  xmm1,xmm2} \
		__asm{shufps  xmm1,xmm1,4Eh} \
		__asm{movaps  xmm3,xmm0} \
		__asm{movaps  xmm4,[ecx*8+esi-20h]} \
		__asm{mulps   xmm4,xmm2} \
		__asm{addps   xmm0,xmm4} \
		__asm{movaps  xmm4,[ecx*8+esi-10h]} \
		__asm{mulps   xmm4,xmm1} \
		__asm{addps   xmm0,xmm4} \
		__asm{movaps  xmm4,[ecx*8+esi]} \
		__asm{mulps   xmm4,xmm2} \
		__asm{addps   xmm3,xmm4} \
		__asm{movaps  xmm4,[ecx*8+esi+10h]} \
		__asm{mulps   xmm4,xmm1} \
		__asm{addps   xmm3,xmm4} \
		__asm{movaps  xmm1,xmm6} \
		__asm{shufps  xmm1,xmm1,4Eh} \
		__asm{movaps  xmm2,xmm5} \
		__asm{movaps  xmm4,[ecx*8+esi+20h]} \
		__asm{mulps   xmm4,xmm6} \
		__asm{addps   xmm5,xmm4} \
		__asm{movaps  xmm4,[ecx*8+esi+30h]} \
		__asm{mulps   xmm4,xmm1} \
		__asm{addps   xmm5,xmm4} \
		__asm{movaps  xmm4,[ecx*8+esi+40h]} \
		__asm{mulps   xmm4,xmm6} \
		__asm{addps   xmm2,xmm4} \
		__asm{movaps  xmm4,[ecx*8+esi+50h]} \
		__asm{mulps   xmm4,xmm1} \
		__asm{addps   xmm2,xmm4} \
		__asm{mulps   xmm0,xmm0} \
		__asm{mulps   xmm3,xmm3} \
		__asm{mulps   xmm5,xmm5} \
		__asm{mulps   xmm2,xmm2} \
		__asm{movaps  xmm4,xmm0} \
		__asm{movlhps xmm0,xmm3} \
		__asm{movhlps xmm3,xmm4} \
		__asm{addps   xmm0,xmm3} \
		__asm{sqrtps  xmm0,xmm0} \
		__asm{movaps  xmm4,xmm5} \
		__asm{movlhps xmm5,xmm2} \
		__asm{movhlps xmm2,xmm4} \
		__asm{addps   xmm5,xmm2} \
		__asm{sqrtps  xmm5,xmm5} \
		__asm{movaps  xmm3,xmm0} \
		__asm{movlhps xmm0,xmm5} \
		__asm{movhlps xmm5,xmm3} \
		__asm{shufps  xmm5,xmm5,1Bh} \
		__asm{movups  [edi+ecx],xmm0} \
		__asm{movaps  [edi+edx-8h],xmm5} \
		__asm{add     ecx,10h} \
		__asm{sub     edx,10h} \
		__asm{cmp	  ecx,104h} \
		__asm{jnz L_P128_ctr_trans} 


inline void rfft256(float* x,float* sqrt_x)
{
	/* 依次把 7个碟形展开! */
	/* 第一级碟形 L1 only */
	__asm{mov eax,x}
	__asm{mov edi,sqrt_x}
	L1_Atom_P128(0);
	/* 第二级碟形 L1 L2 */	
	L1_Atom_P64(0);
	L2_Atom_P64(512);
	/* 第三级碟形 L1 L2 L1 L1*/
	L1_Atom_P32(0);
	L2_Atom_P32(256);
	L1_Atom_P32(512);
	L1_Atom_P32(768);
	/* 第四级碟形 L1 L2 L1 L1 L1 L2 L1 L2 */
	L1_Atom_P16(0);
	L2_Atom_P16(128);
	L1_Atom_P16(256);
	L1_Atom_P16(384);
	L1_Atom_P16(512);
	L2_Atom_P16(640);
	L1_Atom_P16(768);
	L2_Atom_P16(896);
	/* 第五、六、七级碟形 L1 L2 L1 L1
	              L1 L2 L1 L2  
				  L1 L2 L1 L1  
				  L1 L2 L1 L1 */
	L1_Atom_P8_4_2(0);
	L2_Atom_P8_4_2(64);
	L1_Atom_P8_4_2(128);
	L1_Atom_P8_4_2(192);
	L1_Atom_P8_4_2(256);
	L2_Atom_P8_4_2(320);
	L1_Atom_P8_4_2(384);
	L2_Atom_P8_4_2(448);
	L1_Atom_P8_4_2(512);
	L2_Atom_P8_4_2(576);
	L1_Atom_P8_4_2(640);
	L1_Atom_P8_4_2(704);
	L1_Atom_P8_4_2(768);
	L2_Atom_P8_4_2(832);
	L1_Atom_P8_4_2(896);
	L1_Atom_P8_4_2(960);
	/* 最后一个transform */
	P128_Reverse;
	P128_ctr_trans;
}

#define P256_Reverse \
		__asm{movsd xmm0,[eax+128]} \
		__asm{movsd xmm1,[eax+64]} \
		__asm{movsd [eax+64],xmm0} \
		__asm{movsd [eax+128],xmm1} \
		__asm{movsd xmm0,[eax+256]} \
		__asm{movsd xmm1,[eax+32]} \
		__asm{movsd [eax+32],xmm0} \
		__asm{movsd [eax+256],xmm1} \
		__asm{movsd xmm0,[eax+320]} \
		__asm{movsd xmm1,[eax+160]} \
		__asm{movsd [eax+160],xmm0} \
		__asm{movsd [eax+320],xmm1} \
		__asm{movsd xmm0,[eax+384]} \
		__asm{movsd xmm1,[eax+96]} \
		__asm{movsd [eax+96],xmm0} \
		__asm{movsd [eax+384],xmm1} \
		__asm{movsd xmm0,[eax+416]} \
		__asm{movsd xmm1,[eax+352]} \
		__asm{movsd [eax+352],xmm0} \
		__asm{movsd [eax+416],xmm1} \
		__asm{movsd xmm0,[eax+448]} \
		__asm{movsd xmm1,[eax+224]} \
		__asm{movsd [eax+224],xmm0} \
		__asm{movsd [eax+448],xmm1} \
		__asm{movsd xmm0,[eax+512]} \
		__asm{movsd xmm1,[eax+16]} \
		__asm{movsd [eax+16],xmm0} \
		__asm{movsd [eax+512],xmm1} \
		__asm{movsd xmm0,[eax+544]} \
		__asm{movsd xmm1,[eax+272]} \
		__asm{movsd [eax+272],xmm0} \
		__asm{movsd [eax+544],xmm1} \
		__asm{movsd xmm0,[eax+576]} \
		__asm{movsd xmm1,[eax+144]} \
		__asm{movsd [eax+144],xmm0} \
		__asm{movsd [eax+576],xmm1} \
		__asm{movsd xmm0,[eax+608]} \
		__asm{movsd xmm1,[eax+400]} \
		__asm{movsd [eax+400],xmm0} \
		__asm{movsd [eax+608],xmm1} \
		__asm{movsd xmm0,[eax+640]} \
		__asm{movsd xmm1,[eax+80]} \
		__asm{movsd [eax+80],xmm0} \
		__asm{movsd [eax+640],xmm1} \
		__asm{movsd xmm0,[eax+656]} \
		__asm{movsd xmm1,[eax+592]} \
		__asm{movsd [eax+592],xmm0} \
		__asm{movsd [eax+656],xmm1} \
		__asm{movsd xmm0,[eax+672]} \
		__asm{movsd xmm1,[eax+336]} \
		__asm{movsd [eax+336],xmm0} \
		__asm{movsd [eax+672],xmm1} \
		__asm{movsd xmm0,[eax+704]} \
		__asm{movsd xmm1,[eax+208]} \
		__asm{movsd [eax+208],xmm0} \
		__asm{movsd [eax+704],xmm1} \
		__asm{movsd xmm0,[eax+736]} \
		__asm{movsd xmm1,[eax+464]} \
		__asm{movsd [eax+464],xmm0} \
		__asm{movsd [eax+736],xmm1} \
		__asm{movsd xmm0,[eax+768]} \
		__asm{movsd xmm1,[eax+48]} \
		__asm{movsd [eax+48],xmm0} \
		__asm{movsd [eax+768],xmm1} \
		__asm{movsd xmm0,[eax+784]} \
		__asm{movsd xmm1,[eax+560]} \
		__asm{movsd [eax+560],xmm0} \
		__asm{movsd [eax+784],xmm1} \
		__asm{movsd xmm0,[eax+800]} \
		__asm{movsd xmm1,[eax+304]} \
		__asm{movsd [eax+304],xmm0} \
		__asm{movsd [eax+800],xmm1} \
		__asm{movsd xmm0,[eax+832]} \
		__asm{movsd xmm1,[eax+176]} \
		__asm{movsd [eax+176],xmm0} \
		__asm{movsd [eax+832],xmm1} \
		__asm{movsd xmm0,[eax+848]} \
		__asm{movsd xmm1,[eax+688]} \
		__asm{movsd [eax+688],xmm0} \
		__asm{movsd [eax+848],xmm1} \
		__asm{movsd xmm0,[eax+864]} \
		__asm{movsd xmm1,[eax+432]} \
		__asm{movsd [eax+432],xmm0} \
		__asm{movsd [eax+864],xmm1} \
		__asm{movsd xmm0,[eax+896]} \
		__asm{movsd xmm1,[eax+112]} \
		__asm{movsd [eax+112],xmm0} \
		__asm{movsd [eax+896],xmm1} \
		__asm{movsd xmm0,[eax+912]} \
		__asm{movsd xmm1,[eax+624]} \
		__asm{movsd [eax+624],xmm0} \
		__asm{movsd [eax+912],xmm1} \
		__asm{movsd xmm0,[eax+928]} \
		__asm{movsd xmm1,[eax+368]} \
		__asm{movsd [eax+368],xmm0} \
		__asm{movsd [eax+928],xmm1} \
		__asm{movsd xmm0,[eax+944]} \
		__asm{movsd xmm1,[eax+880]} \
		__asm{movsd [eax+880],xmm0} \
		__asm{movsd [eax+944],xmm1} \
		__asm{movsd xmm0,[eax+960]} \
		__asm{movsd xmm1,[eax+240]} \
		__asm{movsd [eax+240],xmm0} \
		__asm{movsd [eax+960],xmm1} \
		__asm{movsd xmm0,[eax+976]} \
		__asm{movsd xmm1,[eax+752]} \
		__asm{movsd [eax+752],xmm0} \
		__asm{movsd [eax+976],xmm1} \
		__asm{movsd xmm0,[eax+992]} \
		__asm{movsd xmm1,[eax+496]} \
		__asm{movsd [eax+496],xmm0} \
		__asm{movsd [eax+992],xmm1} \
		__asm{movsd xmm0,[eax+1024]} \
		__asm{movsd xmm1,[eax+8]} \
		__asm{movsd [eax+8],xmm0} \
		__asm{movsd [eax+1024],xmm1} \
		__asm{movsd xmm0,[eax+1040]} \
		__asm{movsd xmm1,[eax+520]} \
		__asm{movsd [eax+520],xmm0} \
		__asm{movsd [eax+1040],xmm1} \
		__asm{movsd xmm0,[eax+1056]} \
		__asm{movsd xmm1,[eax+264]} \
		__asm{movsd [eax+264],xmm0} \
		__asm{movsd [eax+1056],xmm1} \
		__asm{movsd xmm0,[eax+1072]} \
		__asm{movsd xmm1,[eax+776]} \
		__asm{movsd [eax+776],xmm0} \
		__asm{movsd [eax+1072],xmm1} \
		__asm{movsd xmm0,[eax+1088]} \
		__asm{movsd xmm1,[eax+136]} \
		__asm{movsd [eax+136],xmm0} \
		__asm{movsd [eax+1088],xmm1} \
		__asm{movsd xmm0,[eax+1104]} \
		__asm{movsd xmm1,[eax+648]} \
		__asm{movsd [eax+648],xmm0} \
		__asm{movsd [eax+1104],xmm1} \
		__asm{movsd xmm0,[eax+1120]} \
		__asm{movsd xmm1,[eax+392]} \
		__asm{movsd [eax+392],xmm0} \
		__asm{movsd [eax+1120],xmm1} \
		__asm{movsd xmm0,[eax+1136]} \
		__asm{movsd xmm1,[eax+904]} \
		__asm{movsd [eax+904],xmm0} \
		__asm{movsd [eax+1136],xmm1} \
		__asm{movsd xmm0,[eax+1152]} \
		__asm{movsd xmm1,[eax+72]} \
		__asm{movsd [eax+72],xmm0} \
		__asm{movsd [eax+1152],xmm1} \
		__asm{movsd xmm0,[eax+1160]} \
		__asm{movsd xmm1,[eax+1096]} \
		__asm{movsd [eax+1096],xmm0} \
		__asm{movsd [eax+1160],xmm1} \
		__asm{movsd xmm0,[eax+1168]} \
		__asm{movsd xmm1,[eax+584]} \
		__asm{movsd [eax+584],xmm0} \
		__asm{movsd [eax+1168],xmm1} \
		__asm{movsd xmm0,[eax+1184]} \
		__asm{movsd xmm1,[eax+328]} \
		__asm{movsd [eax+328],xmm0} \
		__asm{movsd [eax+1184],xmm1} \
		__asm{movsd xmm0,[eax+1200]} \
		__asm{movsd xmm1,[eax+840]} \
		__asm{movsd [eax+840],xmm0} \
		__asm{movsd [eax+1200],xmm1} \
		__asm{movsd xmm0,[eax+1216]} \
		__asm{movsd xmm1,[eax+200]} \
		__asm{movsd [eax+200],xmm0} \
		__asm{movsd [eax+1216],xmm1} \
		__asm{movsd xmm0,[eax+1232]} \
		__asm{movsd xmm1,[eax+712]} \
		__asm{movsd [eax+712],xmm0} \
		__asm{movsd [eax+1232],xmm1} \
		__asm{movsd xmm0,[eax+1248]} \
		__asm{movsd xmm1,[eax+456]} \
		__asm{movsd [eax+456],xmm0} \
		__asm{movsd [eax+1248],xmm1} \
		__asm{movsd xmm0,[eax+1264]} \
		__asm{movsd xmm1,[eax+968]} \
		__asm{movsd [eax+968],xmm0} \
		__asm{movsd [eax+1264],xmm1} \
		__asm{movsd xmm0,[eax+1280]} \
		__asm{movsd xmm1,[eax+40]} \
		__asm{movsd [eax+40],xmm0} \
		__asm{movsd [eax+1280],xmm1} \
		__asm{movsd xmm0,[eax+1288]} \
		__asm{movsd xmm1,[eax+1064]} \
		__asm{movsd [eax+1064],xmm0} \
		__asm{movsd [eax+1288],xmm1} \
		__asm{movsd xmm0,[eax+1296]} \
		__asm{movsd xmm1,[eax+552]} \
		__asm{movsd [eax+552],xmm0} \
		__asm{movsd [eax+1296],xmm1} \
		__asm{movsd xmm0,[eax+1312]} \
		__asm{movsd xmm1,[eax+296]} \
		__asm{movsd [eax+296],xmm0} \
		__asm{movsd [eax+1312],xmm1} \
		__asm{movsd xmm0,[eax+1328]} \
		__asm{movsd xmm1,[eax+808]} \
		__asm{movsd [eax+808],xmm0} \
		__asm{movsd [eax+1328],xmm1} \
		__asm{movsd xmm0,[eax+1344]} \
		__asm{movsd xmm1,[eax+168]} \
		__asm{movsd [eax+168],xmm0} \
		__asm{movsd [eax+1344],xmm1} \
		__asm{movsd xmm0,[eax+1352]} \
		__asm{movsd xmm1,[eax+1192]} \
		__asm{movsd [eax+1192],xmm0} \
		__asm{movsd [eax+1352],xmm1} \
		__asm{movsd xmm0,[eax+1360]} \
		__asm{movsd xmm1,[eax+680]} \
		__asm{movsd [eax+680],xmm0} \
		__asm{movsd [eax+1360],xmm1} \
		__asm{movsd xmm0,[eax+1376]} \
		__asm{movsd xmm1,[eax+424]} \
		__asm{movsd [eax+424],xmm0} \
		__asm{movsd [eax+1376],xmm1} \
		__asm{movsd xmm0,[eax+1392]} \
		__asm{movsd xmm1,[eax+936]} \
		__asm{movsd [eax+936],xmm0} \
		__asm{movsd [eax+1392],xmm1} \
		__asm{movsd xmm0,[eax+1408]} \
		__asm{movsd xmm1,[eax+104]} \
		__asm{movsd [eax+104],xmm0} \
		__asm{movsd [eax+1408],xmm1} \
		__asm{movsd xmm0,[eax+1416]} \
		__asm{movsd xmm1,[eax+1128]} \
		__asm{movsd [eax+1128],xmm0} \
		__asm{movsd [eax+1416],xmm1} \
		__asm{movsd xmm0,[eax+1424]} \
		__asm{movsd xmm1,[eax+616]} \
		__asm{movsd [eax+616],xmm0} \
		__asm{movsd [eax+1424],xmm1} \
		__asm{movsd xmm0,[eax+1440]} \
		__asm{movsd xmm1,[eax+360]} \
		__asm{movsd [eax+360],xmm0} \
		__asm{movsd [eax+1440],xmm1} \
		__asm{movsd xmm0,[eax+1448]} \
		__asm{movsd xmm1,[eax+1384]} \
		__asm{movsd [eax+1384],xmm0} \
		__asm{movsd [eax+1448],xmm1} \
		__asm{movsd xmm0,[eax+1456]} \
		__asm{movsd xmm1,[eax+872]} \
		__asm{movsd [eax+872],xmm0} \
		__asm{movsd [eax+1456],xmm1} \
		__asm{movsd xmm0,[eax+1472]} \
		__asm{movsd xmm1,[eax+232]} \
		__asm{movsd [eax+232],xmm0} \
		__asm{movsd [eax+1472],xmm1} \
		__asm{movsd xmm0,[eax+1480]} \
		__asm{movsd xmm1,[eax+1256]} \
		__asm{movsd [eax+1256],xmm0} \
		__asm{movsd [eax+1480],xmm1} \
		__asm{movsd xmm0,[eax+1488]} \
		__asm{movsd xmm1,[eax+744]} \
		__asm{movsd [eax+744],xmm0} \
		__asm{movsd [eax+1488],xmm1} \
		__asm{movsd xmm0,[eax+1504]} \
		__asm{movsd xmm1,[eax+488]} \
		__asm{movsd [eax+488],xmm0} \
		__asm{movsd [eax+1504],xmm1} \
		__asm{movsd xmm0,[eax+1520]} \
		__asm{movsd xmm1,[eax+1000]} \
		__asm{movsd [eax+1000],xmm0} \
		__asm{movsd [eax+1520],xmm1} \
		__asm{movsd xmm0,[eax+1536]} \
		__asm{movsd xmm1,[eax+24]} \
		__asm{movsd [eax+24],xmm0} \
		__asm{movsd [eax+1536],xmm1} \
		__asm{movsd xmm0,[eax+1544]} \
		__asm{movsd xmm1,[eax+1048]} \
		__asm{movsd [eax+1048],xmm0} \
		__asm{movsd [eax+1544],xmm1} \
		__asm{movsd xmm0,[eax+1552]} \
		__asm{movsd xmm1,[eax+536]} \
		__asm{movsd [eax+536],xmm0} \
		__asm{movsd [eax+1552],xmm1} \
		__asm{movsd xmm0,[eax+1568]} \
		__asm{movsd xmm1,[eax+280]} \
		__asm{movsd [eax+280],xmm0} \
		__asm{movsd [eax+1568],xmm1} \
		__asm{movsd xmm0,[eax+1576]} \
		__asm{movsd xmm1,[eax+1304]} \
		__asm{movsd [eax+1304],xmm0} \
		__asm{movsd [eax+1576],xmm1} \
		__asm{movsd xmm0,[eax+1584]} \
		__asm{movsd xmm1,[eax+792]} \
		__asm{movsd [eax+792],xmm0} \
		__asm{movsd [eax+1584],xmm1} \
		__asm{movsd xmm0,[eax+1600]} \
		__asm{movsd xmm1,[eax+152]} \
		__asm{movsd [eax+152],xmm0} \
		__asm{movsd [eax+1600],xmm1} \
		__asm{movsd xmm0,[eax+1608]} \
		__asm{movsd xmm1,[eax+1176]} \
		__asm{movsd [eax+1176],xmm0} \
		__asm{movsd [eax+1608],xmm1} \
		__asm{movsd xmm0,[eax+1616]} \
		__asm{movsd xmm1,[eax+664]} \
		__asm{movsd [eax+664],xmm0} \
		__asm{movsd [eax+1616],xmm1} \
		__asm{movsd xmm0,[eax+1632]} \
		__asm{movsd xmm1,[eax+408]} \
		__asm{movsd [eax+408],xmm0} \
		__asm{movsd [eax+1632],xmm1} \
		__asm{movsd xmm0,[eax+1640]} \
		__asm{movsd xmm1,[eax+1432]} \
		__asm{movsd [eax+1432],xmm0} \
		__asm{movsd [eax+1640],xmm1} \
		__asm{movsd xmm0,[eax+1648]} \
		__asm{movsd xmm1,[eax+920]} \
		__asm{movsd [eax+920],xmm0} \
		__asm{movsd [eax+1648],xmm1} \
		__asm{movsd xmm0,[eax+1664]} \
		__asm{movsd xmm1,[eax+88]} \
		__asm{movsd [eax+88],xmm0} \
		__asm{movsd [eax+1664],xmm1} \
		__asm{movsd xmm0,[eax+1672]} \
		__asm{movsd xmm1,[eax+1112]} \
		__asm{movsd [eax+1112],xmm0} \
		__asm{movsd [eax+1672],xmm1} \
		__asm{movsd xmm0,[eax+1680]} \
		__asm{movsd xmm1,[eax+600]} \
		__asm{movsd [eax+600],xmm0} \
		__asm{movsd [eax+1680],xmm1} \
		__asm{movsd xmm0,[eax+1688]} \
		__asm{movsd xmm1,[eax+1624]} \
		__asm{movsd [eax+1624],xmm0} \
		__asm{movsd [eax+1688],xmm1} \
		__asm{movsd xmm0,[eax+1696]} \
		__asm{movsd xmm1,[eax+344]} \
		__asm{movsd [eax+344],xmm0} \
		__asm{movsd [eax+1696],xmm1} \
		__asm{movsd xmm0,[eax+1704]} \
		__asm{movsd xmm1,[eax+1368]} \
		__asm{movsd [eax+1368],xmm0} \
		__asm{movsd [eax+1704],xmm1} \
		__asm{movsd xmm0,[eax+1712]} \
		__asm{movsd xmm1,[eax+856]} \
		__asm{movsd [eax+856],xmm0} \
		__asm{movsd [eax+1712],xmm1} \
		__asm{movsd xmm0,[eax+1728]} \
		__asm{movsd xmm1,[eax+216]} \
		__asm{movsd [eax+216],xmm0} \
		__asm{movsd [eax+1728],xmm1} \
		__asm{movsd xmm0,[eax+1736]} \
		__asm{movsd xmm1,[eax+1240]} \
		__asm{movsd [eax+1240],xmm0} \
		__asm{movsd [eax+1736],xmm1} \
		__asm{movsd xmm0,[eax+1744]} \
		__asm{movsd xmm1,[eax+728]} \
		__asm{movsd [eax+728],xmm0} \
		__asm{movsd [eax+1744],xmm1} \
		__asm{movsd xmm0,[eax+1760]} \
		__asm{movsd xmm1,[eax+472]} \
		__asm{movsd [eax+472],xmm0} \
		__asm{movsd [eax+1760],xmm1} \
		__asm{movsd xmm0,[eax+1768]} \
		__asm{movsd xmm1,[eax+1496]} \
		__asm{movsd [eax+1496],xmm0} \
		__asm{movsd [eax+1768],xmm1} \
		__asm{movsd xmm0,[eax+1776]} \
		__asm{movsd xmm1,[eax+984]} \
		__asm{movsd [eax+984],xmm0} \
		__asm{movsd [eax+1776],xmm1} \
		__asm{movsd xmm0,[eax+1792]} \
		__asm{movsd xmm1,[eax+56]} \
		__asm{movsd [eax+56],xmm0} \
		__asm{movsd [eax+1792],xmm1} \
		__asm{movsd xmm0,[eax+1800]} \
		__asm{movsd xmm1,[eax+1080]} \
		__asm{movsd [eax+1080],xmm0} \
		__asm{movsd [eax+1800],xmm1} \
		__asm{movsd xmm0,[eax+1808]} \
		__asm{movsd xmm1,[eax+568]} \
		__asm{movsd [eax+568],xmm0} \
		__asm{movsd [eax+1808],xmm1} \
		__asm{movsd xmm0,[eax+1816]} \
		__asm{movsd xmm1,[eax+1592]} \
		__asm{movsd [eax+1592],xmm0} \
		__asm{movsd [eax+1816],xmm1} \
		__asm{movsd xmm0,[eax+1824]} \
		__asm{movsd xmm1,[eax+312]} \
		__asm{movsd [eax+312],xmm0} \
		__asm{movsd [eax+1824],xmm1} \
		__asm{movsd xmm0,[eax+1832]} \
		__asm{movsd xmm1,[eax+1336]} \
		__asm{movsd [eax+1336],xmm0} \
		__asm{movsd [eax+1832],xmm1} \
		__asm{movsd xmm0,[eax+1840]} \
		__asm{movsd xmm1,[eax+824]} \
		__asm{movsd [eax+824],xmm0} \
		__asm{movsd [eax+1840],xmm1} \
		__asm{movsd xmm0,[eax+1856]} \
		__asm{movsd xmm1,[eax+184]} \
		__asm{movsd [eax+184],xmm0} \
		__asm{movsd [eax+1856],xmm1} \
		__asm{movsd xmm0,[eax+1864]} \
		__asm{movsd xmm1,[eax+1208]} \
		__asm{movsd [eax+1208],xmm0} \
		__asm{movsd [eax+1864],xmm1} \
		__asm{movsd xmm0,[eax+1872]} \
		__asm{movsd xmm1,[eax+696]} \
		__asm{movsd [eax+696],xmm0} \
		__asm{movsd [eax+1872],xmm1} \
		__asm{movsd xmm0,[eax+1880]} \
		__asm{movsd xmm1,[eax+1720]} \
		__asm{movsd [eax+1720],xmm0} \
		__asm{movsd [eax+1880],xmm1} \
		__asm{movsd xmm0,[eax+1888]} \
		__asm{movsd xmm1,[eax+440]} \
		__asm{movsd [eax+440],xmm0} \
		__asm{movsd [eax+1888],xmm1} \
		__asm{movsd xmm0,[eax+1896]} \
		__asm{movsd xmm1,[eax+1464]} \
		__asm{movsd [eax+1464],xmm0} \
		__asm{movsd [eax+1896],xmm1} \
		__asm{movsd xmm0,[eax+1904]} \
		__asm{movsd xmm1,[eax+952]} \
		__asm{movsd [eax+952],xmm0} \
		__asm{movsd [eax+1904],xmm1} \
		__asm{movsd xmm0,[eax+1920]} \
		__asm{movsd xmm1,[eax+120]} \
		__asm{movsd [eax+120],xmm0} \
		__asm{movsd [eax+1920],xmm1} \
		__asm{movsd xmm0,[eax+1928]} \
		__asm{movsd xmm1,[eax+1144]} \
		__asm{movsd [eax+1144],xmm0} \
		__asm{movsd [eax+1928],xmm1} \
		__asm{movsd xmm0,[eax+1936]} \
		__asm{movsd xmm1,[eax+632]} \
		__asm{movsd [eax+632],xmm0} \
		__asm{movsd [eax+1936],xmm1} \
		__asm{movsd xmm0,[eax+1944]} \
		__asm{movsd xmm1,[eax+1656]} \
		__asm{movsd [eax+1656],xmm0} \
		__asm{movsd [eax+1944],xmm1} \
		__asm{movsd xmm0,[eax+1952]} \
		__asm{movsd xmm1,[eax+376]} \
		__asm{movsd [eax+376],xmm0} \
		__asm{movsd [eax+1952],xmm1} \
		__asm{movsd xmm0,[eax+1960]} \
		__asm{movsd xmm1,[eax+1400]} \
		__asm{movsd [eax+1400],xmm0} \
		__asm{movsd [eax+1960],xmm1} \
		__asm{movsd xmm0,[eax+1968]} \
		__asm{movsd xmm1,[eax+888]} \
		__asm{movsd [eax+888],xmm0} \
		__asm{movsd [eax+1968],xmm1} \
		__asm{movsd xmm0,[eax+1976]} \
		__asm{movsd xmm1,[eax+1912]} \
		__asm{movsd [eax+1912],xmm0} \
		__asm{movsd [eax+1976],xmm1} \
		__asm{movsd xmm0,[eax+1984]} \
		__asm{movsd xmm1,[eax+248]} \
		__asm{movsd [eax+248],xmm0} \
		__asm{movsd [eax+1984],xmm1} \
		__asm{movsd xmm0,[eax+1992]} \
		__asm{movsd xmm1,[eax+1272]} \
		__asm{movsd [eax+1272],xmm0} \
		__asm{movsd [eax+1992],xmm1} \
		__asm{movsd xmm0,[eax+2000]} \
		__asm{movsd xmm1,[eax+760]} \
		__asm{movsd [eax+760],xmm0} \
		__asm{movsd [eax+2000],xmm1} \
		__asm{movsd xmm0,[eax+2008]} \
		__asm{movsd xmm1,[eax+1784]} \
		__asm{movsd [eax+1784],xmm0} \
		__asm{movsd [eax+2008],xmm1} \
		__asm{movsd xmm0,[eax+2016]} \
		__asm{movsd xmm1,[eax+504]} \
		__asm{movsd [eax+504],xmm0} \
		__asm{movsd [eax+2016],xmm1} \
		__asm{movsd xmm0,[eax+2024]} \
		__asm{movsd xmm1,[eax+1528]} \
		__asm{movsd [eax+1528],xmm0} \
		__asm{movsd [eax+2024],xmm1} \
		__asm{movsd xmm0,[eax+2032]} \
		__asm{movsd xmm1,[eax+1016]} \
		__asm{movsd [eax+1016],xmm0} \
		__asm{movsd [eax+2032],xmm1}

const float SSE_ALIGN CTR_W_P256[] =
{
	0.999925f,
	0.999699f,
	-0.999925f,
	-0.999699f,
	-0.012272f,
	-0.024541f,
	-0.012272f,
	-0.024541f,
	-0.999925f,
	-0.999699f,
	0.999925f,
	0.999699f,
	0.012271f,
	0.024541f,
	0.012271f,
	0.024541f,
	0.999322f,
	0.998795f,
	-0.999322f,
	-0.998795f,
	-0.036807f,
	-0.049068f,
	-0.036807f,
	-0.049068f,
	-0.999322f,
	-0.998795f,
	0.999322f,
	0.998795f,
	0.036807f,
	0.049068f,
	0.036807f,
	0.049068f,
	0.998118f,
	0.997290f,
	-0.998118f,
	-0.997290f,
	-0.061321f,
	-0.073565f,
	-0.061321f,
	-0.073565f,
	-0.998118f,
	-0.997290f,
	0.998118f,
	0.997290f,
	0.061321f,
	0.073564f,
	0.061321f,
	0.073564f,
	0.996313f,
	0.995185f,
	-0.996313f,
	-0.995185f,
	-0.085797f,
	-0.098017f,
	-0.085797f,
	-0.098017f,
	-0.996313f,
	-0.995185f,
	0.996313f,
	0.995185f,
	0.085797f,
	0.098017f,
	0.085797f,
	0.098017f,
	0.993907f,
	0.992480f,
	-0.993907f,
	-0.992480f,
	-0.110222f,
	-0.122411f,
	-0.110222f,
	-0.122411f,
	-0.993907f,
	-0.992480f,
	0.993907f,
	0.992480f,
	0.110222f,
	0.122411f,
	0.110222f,
	0.122411f,
	0.990903f,
	0.989177f,
	-0.990903f,
	-0.989177f,
	-0.134581f,
	-0.146730f,
	-0.134581f,
	-0.146730f,
	-0.990903f,
	-0.989177f,
	0.990903f,
	0.989177f,
	0.134581f,
	0.146730f,
	0.134581f,
	0.146730f,
	0.987301f,
	0.985278f,
	-0.987301f,
	-0.985278f,
	-0.158858f,
	-0.170962f,
	-0.158858f,
	-0.170962f,
	-0.987301f,
	-0.985278f,
	0.987301f,
	0.985278f,
	0.158858f,
	0.170962f,
	0.158858f,
	0.170962f,
	0.983105f,
	0.980785f,
	-0.983105f,
	-0.980785f,
	-0.183040f,
	-0.195090f,
	-0.183040f,
	-0.195090f,
	-0.983106f,
	-0.980785f,
	0.983106f,
	0.980785f,
	0.183040f,
	0.195090f,
	0.183040f,
	0.195090f,
	0.978317f,
	0.975702f,
	-0.978317f,
	-0.975702f,
	-0.207111f,
	-0.219101f,
	-0.207111f,
	-0.219101f,
	-0.978317f,
	-0.975702f,
	0.978317f,
	0.975702f,
	0.207111f,
	0.219101f,
	0.207111f,
	0.219101f,
	0.972940f,
	0.970031f,
	-0.972940f,
	-0.970031f,
	-0.231058f,
	-0.242980f,
	-0.231058f,
	-0.242980f,
	-0.972940f,
	-0.970031f,
	0.972940f,
	0.970031f,
	0.231058f,
	0.242980f,
	0.231058f,
	0.242980f,
	0.966976f,
	0.963776f,
	-0.966976f,
	-0.963776f,
	-0.254866f,
	-0.266713f,
	-0.254866f,
	-0.266713f,
	-0.966976f,
	-0.963776f,
	0.966976f,
	0.963776f,
	0.254866f,
	0.266713f,
	0.254866f,
	0.266713f,
	0.960431f,
	0.956940f,
	-0.960431f,
	-0.956940f,
	-0.278520f,
	-0.290285f,
	-0.278520f,
	-0.290285f,
	-0.960431f,
	-0.956940f,
	0.960431f,
	0.956940f,
	0.278520f,
	0.290285f,
	0.278520f,
	0.290285f,
	0.953306f,
	0.949528f,
	-0.953306f,
	-0.949528f,
	-0.302006f,
	-0.313682f,
	-0.302006f,
	-0.313682f,
	-0.953306f,
	-0.949528f,
	0.953306f,
	0.949528f,
	0.302006f,
	0.313682f,
	0.302006f,
	0.313682f,
	0.945607f,
	0.941544f,
	-0.945607f,
	-0.941544f,
	-0.325310f,
	-0.336890f,
	-0.325310f,
	-0.336890f,
	-0.945607f,
	-0.941544f,
	0.945607f,
	0.941544f,
	0.325310f,
	0.336890f,
	0.325310f,
	0.336890f,
	0.937339f,
	0.932993f,
	-0.937339f,
	-0.932993f,
	-0.348419f,
	-0.359895f,
	-0.348419f,
	-0.359895f,
	-0.937339f,
	-0.932993f,
	0.937339f,
	0.932993f,
	0.348419f,
	0.359895f,
	0.348419f,
	0.359895f,
	0.928506f,
	0.923880f,
	-0.928506f,
	-0.923880f,
	-0.371317f,
	-0.382683f,
	-0.371317f,
	-0.382683f,
	-0.928506f,
	-0.923880f,
	0.928506f,
	0.923880f,
	0.371317f,
	0.382683f,
	0.371317f,
	0.382683f,
	0.919114f,
	0.914210f,
	-0.919114f,
	-0.914210f,
	-0.393992f,
	-0.405241f,
	-0.393992f,
	-0.405241f,
	-0.919114f,
	-0.914210f,
	0.919114f,
	0.914210f,
	0.393992f,
	0.405241f,
	0.393992f,
	0.405241f,
	0.909168f,
	0.903989f,
	-0.909168f,
	-0.903989f,
	-0.416430f,
	-0.427555f,
	-0.416430f,
	-0.427555f,
	-0.909168f,
	-0.903989f,
	0.909168f,
	0.903989f,
	0.416429f,
	0.427555f,
	0.416429f,
	0.427555f,
	0.898674f,
	0.893224f,
	-0.898674f,
	-0.893224f,
	-0.438616f,
	-0.449611f,
	-0.438616f,
	-0.449611f,
	-0.898674f,
	-0.893224f,
	0.898674f,
	0.893224f,
	0.438616f,
	0.449611f,
	0.438616f,
	0.449611f,
	0.887640f,
	0.881921f,
	-0.887640f,
	-0.881921f,
	-0.460539f,
	-0.471397f,
	-0.460539f,
	-0.471397f,
	-0.887640f,
	-0.881921f,
	0.887640f,
	0.881921f,
	0.460539f,
	0.471397f,
	0.460539f,
	0.471397f,
	0.876070f,
	0.870087f,
	-0.876070f,
	-0.870087f,
	-0.482184f,
	-0.492898f,
	-0.482184f,
	-0.492898f,
	-0.876070f,
	-0.870087f,
	0.876070f,
	0.870087f,
	0.482184f,
	0.492898f,
	0.482184f,
	0.492898f,
	0.863973f,
	0.857729f,
	-0.863973f,
	-0.857729f,
	-0.503538f,
	-0.514103f,
	-0.503538f,
	-0.514103f,
	-0.863973f,
	-0.857729f,
	0.863973f,
	0.857729f,
	0.503538f,
	0.514103f,
	0.503538f,
	0.514103f,
	0.851355f,
	0.844854f,
	-0.851355f,
	-0.844854f,
	-0.524590f,
	-0.534998f,
	-0.524590f,
	-0.534998f,
	-0.851355f,
	-0.844854f,
	0.851355f,
	0.844854f,
	0.524590f,
	0.534998f,
	0.524590f,
	0.534998f,
	0.838225f,
	0.831470f,
	-0.838225f,
	-0.831470f,
	-0.545325f,
	-0.555570f,
	-0.545325f,
	-0.555570f,
	-0.838225f,
	-0.831470f,
	0.838225f,
	0.831470f,
	0.545325f,
	0.555570f,
	0.545325f,
	0.555570f,
	0.824589f,
	0.817585f,
	-0.824589f,
	-0.817585f,
	-0.565732f,
	-0.575808f,
	-0.565732f,
	-0.575808f,
	-0.824589f,
	-0.817585f,
	0.824589f,
	0.817585f,
	0.565732f,
	0.575808f,
	0.565732f,
	0.575808f,
	0.810457f,
	0.803208f,
	-0.810457f,
	-0.803208f,
	-0.585798f,
	-0.595699f,
	-0.585798f,
	-0.595699f,
	-0.810457f,
	-0.803208f,
	0.810457f,
	0.803208f,
	0.585798f,
	0.595699f,
	0.585798f,
	0.595699f,
	0.795837f,
	0.788346f,
	-0.795837f,
	-0.788346f,
	-0.605511f,
	-0.615232f,
	-0.605511f,
	-0.615232f,
	-0.795837f,
	-0.788346f,
	0.795837f,
	0.788346f,
	0.605511f,
	0.615232f,
	0.605511f,
	0.615232f,
	0.780737f,
	0.773010f,
	-0.780737f,
	-0.773010f,
	-0.624860f,
	-0.634393f,
	-0.624860f,
	-0.634393f,
	-0.780737f,
	-0.773010f,
	0.780737f,
	0.773010f,
	0.624859f,
	0.634393f,
	0.624859f,
	0.634393f,
	0.765167f,
	0.757209f,
	-0.765167f,
	-0.757209f,
	-0.643832f,
	-0.653173f,
	-0.643832f,
	-0.653173f,
	-0.765167f,
	-0.757209f,
	0.765167f,
	0.757209f,
	0.643831f,
	0.653173f,
	0.643831f,
	0.653173f,
	0.749136f,
	0.740951f,
	-0.749136f,
	-0.740951f,
	-0.662416f,
	-0.671559f,
	-0.662416f,
	-0.671559f,
	-0.749136f,
	-0.740951f,
	0.749136f,
	0.740951f,
	0.662416f,
	0.671559f,
	0.662416f,
	0.671559f,
	0.732654f,
	0.724247f,
	-0.732654f,
	-0.724247f,
	-0.680601f,
	-0.689541f,
	-0.680601f,
	-0.689541f,
	-0.732654f,
	-0.724247f,
	0.732654f,
	0.724247f,
	0.680601f,
	0.689540f,
	0.680601f,
	0.689540f,
	0.715731f,
	0.707107f,
	-0.715731f,
	-0.707107f,
	-0.698376f,
	-0.707107f,
	-0.698376f,
	-0.707107f,
	-0.715731f,
	-0.707107f,
	0.715731f,
	0.707107f,
	0.698376f,
	0.707107f,
	0.698376f,
	0.707107f,
	0.698376f,
	0.689541f,
	-0.698376f,
	-0.689541f,
	-0.715731f,
	-0.724247f,
	-0.715731f,
	-0.724247f,
	-0.698376f,
	-0.689541f,
	0.698376f,
	0.689541f,
	0.715731f,
	0.724247f,
	0.715731f,
	0.724247f,
	0.680601f,
	0.671559f,
	-0.680601f,
	-0.671559f,
	-0.732654f,
	-0.740951f,
	-0.732654f,
	-0.740951f,
	-0.680601f,
	-0.671559f,
	0.680601f,
	0.671559f,
	0.732654f,
	0.740951f,
	0.732654f,
	0.740951f,
	0.662416f,
	0.653173f,
	-0.662416f,
	-0.653173f,
	-0.749136f,
	-0.757209f,
	-0.749136f,
	-0.757209f,
	-0.662416f,
	-0.653173f,
	0.662416f,
	0.653173f,
	0.749136f,
	0.757209f,
	0.749136f,
	0.757209f,
	0.643832f,
	0.634393f,
	-0.643832f,
	-0.634393f,
	-0.765167f,
	-0.773010f,
	-0.765167f,
	-0.773010f,
	-0.643832f,
	-0.634393f,
	0.643832f,
	0.634393f,
	0.765167f,
	0.773010f,
	0.765167f,
	0.773010f,
	0.624859f,
	0.615232f,
	-0.624859f,
	-0.615232f,
	-0.780737f,
	-0.788346f,
	-0.780737f,
	-0.788346f,
	-0.624860f,
	-0.615232f,
	0.624860f,
	0.615232f,
	0.780737f,
	0.788346f,
	0.780737f,
	0.788346f,
	0.605511f,
	0.595699f,
	-0.605511f,
	-0.595699f,
	-0.795837f,
	-0.803208f,
	-0.795837f,
	-0.803208f,
	-0.605511f,
	-0.595699f,
	0.605511f,
	0.595699f,
	0.795837f,
	0.803207f,
	0.795837f,
	0.803207f,
	0.585798f,
	0.575808f,
	-0.585798f,
	-0.575808f,
	-0.810457f,
	-0.817585f,
	-0.810457f,
	-0.817585f,
	-0.585798f,
	-0.575808f,
	0.585798f,
	0.575808f,
	0.810457f,
	0.817585f,
	0.810457f,
	0.817585f,
	0.565732f,
	0.555570f,
	-0.565732f,
	-0.555570f,
	-0.824589f,
	-0.831470f,
	-0.824589f,
	-0.831470f,
	-0.565732f,
	-0.555570f,
	0.565732f,
	0.555570f,
	0.824589f,
	0.831470f,
	0.824589f,
	0.831470f,
	0.545325f,
	0.534998f,
	-0.545325f,
	-0.534998f,
	-0.838225f,
	-0.844854f,
	-0.838225f,
	-0.844854f,
	-0.545325f,
	-0.534998f,
	0.545325f,
	0.534998f,
	0.838225f,
	0.844854f,
	0.838225f,
	0.844854f,
	0.524590f,
	0.514103f,
	-0.524590f,
	-0.514103f,
	-0.851355f,
	-0.857729f,
	-0.851355f,
	-0.857729f,
	-0.524590f,
	-0.514103f,
	0.524590f,
	0.514103f,
	0.851355f,
	0.857729f,
	0.851355f,
	0.857729f,
	0.503538f,
	0.492898f,
	-0.503538f,
	-0.492898f,
	-0.863973f,
	-0.870087f,
	-0.863973f,
	-0.870087f,
	-0.503538f,
	-0.492898f,
	0.503538f,
	0.492898f,
	0.863973f,
	0.870087f,
	0.863973f,
	0.870087f,
	0.482184f,
	0.471397f,
	-0.482184f,
	-0.471397f,
	-0.876070f,
	-0.881921f,
	-0.876070f,
	-0.881921f,
	-0.482184f,
	-0.471397f,
	0.482184f,
	0.471397f,
	0.876070f,
	0.881921f,
	0.876070f,
	0.881921f,
	0.460539f,
	0.449611f,
	-0.460539f,
	-0.449611f,
	-0.887640f,
	-0.893224f,
	-0.887640f,
	-0.893224f,
	-0.460539f,
	-0.449611f,
	0.460539f,
	0.449611f,
	0.887640f,
	0.893224f,
	0.887640f,
	0.893224f,
	0.438616f,
	0.427555f,
	-0.438616f,
	-0.427555f,
	-0.898674f,
	-0.903989f,
	-0.898674f,
	-0.903989f,
	-0.438616f,
	-0.427555f,
	0.438616f,
	0.427555f,
	0.898674f,
	0.903989f,
	0.898674f,
	0.903989f,
	0.416430f,
	0.405241f,
	-0.416430f,
	-0.405241f,
	-0.909168f,
	-0.914210f,
	-0.909168f,
	-0.914210f,
	-0.416430f,
	-0.405241f,
	0.416430f,
	0.405241f,
	0.909168f,
	0.914210f,
	0.909168f,
	0.914210f,
	0.393992f,
	0.382683f,
	-0.393992f,
	-0.382683f,
	-0.919114f,
	-0.923880f,
	-0.919114f,
	-0.923880f,
	-0.393992f,
	-0.382683f,
	0.393992f,
	0.382683f,
	0.919114f,
	0.923880f,
	0.919114f,
	0.923880f,
	0.371317f,
	0.359895f,
	-0.371317f,
	-0.359895f,
	-0.928506f,
	-0.932993f,
	-0.928506f,
	-0.932993f,
	-0.371317f,
	-0.359895f,
	0.371317f,
	0.359895f,
	0.928506f,
	0.932993f,
	0.928506f,
	0.932993f,
	0.348419f,
	0.336890f,
	-0.348419f,
	-0.336890f,
	-0.937339f,
	-0.941544f,
	-0.937339f,
	-0.941544f,
	-0.348419f,
	-0.336890f,
	0.348419f,
	0.336890f,
	0.937339f,
	0.941544f,
	0.937339f,
	0.941544f,
	0.325310f,
	0.313682f,
	-0.325310f,
	-0.313682f,
	-0.945607f,
	-0.949528f,
	-0.945607f,
	-0.949528f,
	-0.325310f,
	-0.313682f,
	0.325310f,
	0.313682f,
	0.945607f,
	0.949528f,
	0.945607f,
	0.949528f,
	0.302006f,
	0.290285f,
	-0.302006f,
	-0.290285f,
	-0.953306f,
	-0.956940f,
	-0.953306f,
	-0.956940f,
	-0.302006f,
	-0.290285f,
	0.302006f,
	0.290285f,
	0.953306f,
	0.956940f,
	0.953306f,
	0.956940f,
	0.278520f,
	0.266713f,
	-0.278520f,
	-0.266713f,
	-0.960431f,
	-0.963776f,
	-0.960431f,
	-0.963776f,
	-0.278520f,
	-0.266713f,
	0.278520f,
	0.266713f,
	0.960431f,
	0.963776f,
	0.960431f,
	0.963776f,
	0.254866f,
	0.242980f,
	-0.254866f,
	-0.242980f,
	-0.966976f,
	-0.970031f,
	-0.966976f,
	-0.970031f,
	-0.254866f,
	-0.242980f,
	0.254866f,
	0.242980f,
	0.966976f,
	0.970031f,
	0.966976f,
	0.970031f,
	0.231058f,
	0.219101f,
	-0.231058f,
	-0.219101f,
	-0.972940f,
	-0.975702f,
	-0.972940f,
	-0.975702f,
	-0.231058f,
	-0.219101f,
	0.231058f,
	0.219101f,
	0.972940f,
	0.975702f,
	0.972940f,
	0.975702f,
	0.207111f,
	0.195090f,
	-0.207111f,
	-0.195090f,
	-0.978317f,
	-0.980785f,
	-0.978317f,
	-0.980785f,
	-0.207111f,
	-0.195090f,
	0.207111f,
	0.195090f,
	0.978317f,
	0.980785f,
	0.978317f,
	0.980785f,
	0.183040f,
	0.170962f,
	-0.183040f,
	-0.170962f,
	-0.983105f,
	-0.985278f,
	-0.983105f,
	-0.985278f,
	-0.183040f,
	-0.170962f,
	0.183040f,
	0.170962f,
	0.983105f,
	0.985278f,
	0.983105f,
	0.985278f,
	0.158858f,
	0.146730f,
	-0.158858f,
	-0.146730f,
	-0.987301f,
	-0.989177f,
	-0.987301f,
	-0.989177f,
	-0.158858f,
	-0.146731f,
	0.158858f,
	0.146731f,
	0.987301f,
	0.989177f,
	0.987301f,
	0.989177f,
	0.134581f,
	0.122411f,
	-0.134581f,
	-0.122411f,
	-0.990903f,
	-0.992480f,
	-0.990903f,
	-0.992480f,
	-0.134581f,
	-0.122411f,
	0.134581f,
	0.122411f,
	0.990903f,
	0.992480f,
	0.990903f,
	0.992480f,
	0.110222f,
	0.098017f,
	-0.110222f,
	-0.098017f,
	-0.993907f,
	-0.995185f,
	-0.993907f,
	-0.995185f,
	-0.110222f,
	-0.098017f,
	0.110222f,
	0.098017f,
	0.993907f,
	0.995185f,
	0.993907f,
	0.995185f,
	0.085797f,
	0.073565f,
	-0.085797f,
	-0.073565f,
	-0.996313f,
	-0.997290f,
	-0.996313f,
	-0.997290f,
	-0.085797f,
	-0.073565f,
	0.085797f,
	0.073565f,
	0.996313f,
	0.997290f,
	0.996313f,
	0.997290f,
	0.061321f,
	0.049068f,
	-0.061321f,
	-0.049068f,
	-0.998118f,
	-0.998795f,
	-0.998118f,
	-0.998795f,
	-0.061321f,
	-0.049068f,
	0.061321f,
	0.049068f,
	0.998118f,
	0.998795f,
	0.998118f,
	0.998795f,
	0.036807f,
	0.024541f,
	-0.036807f,
	-0.024541f,
	-0.999322f,
	-0.999699f,
	-0.999322f,
	-0.999699f,
	-0.036807f,
	-0.024541f,
	0.036807f,
	0.024541f,
	0.999322f,
	0.999699f,
	0.999322f,
	0.999699f,
	0.012271f,
	-0.000000f,
	-0.012271f,
	0.000000f,
	-0.999925f,
	-1.000000f,
	-0.999925f,
	-1.000000f,
	-0.012272f,
	-0.000000f,
	0.012272f,
	0.000000f,
	0.999925f,
	1.000000f,
	0.999925f,
	1.000000f
};

#define P256_ctr_trans_op84  \
    __asm{mov	  ecx,04h}   \
    __asm{mov	  edx,3F8h}  \
    __asm{lea     esi,CTR_W_P256} \
    __asm{movaps  xmm7,half_vector} \
    __asm{fld     [eax]} \
    __asm{fadd    [eax+4h]} \
    __asm{fstp    [edi]} \
L_P256_ctr_trans_op84:   \
    __asm{movups  xmm0,[eax+ecx*2]} \
    __asm{movups  xmm5,[eax+ecx*2+10h]} \
    __asm{movaps  xmm1,[eax+edx*2]} \
    __asm{movaps  xmm4,[eax+edx*2-10h]} \
    __asm{movaps  xmm2,xmm0} \
    __asm{movaps  xmm6,xmm5} \
    __asm{shufps  xmm1,xmm1,4Eh} \
    __asm{shufps  xmm4,xmm4,4Eh} \
    __asm{addps   xmm0,xmm1} \
    __asm{addps   xmm5,xmm4} \
    __asm{subps   xmm2,xmm1} \
    __asm{subps   xmm6,xmm4} \
    __asm{mulps   xmm0,xmm7} \
    __asm{mulps   xmm5,xmm7} \
    __asm{mulps   xmm2,xmm7} \
    __asm{mulps   xmm6,xmm7} \
    __asm{shufps  xmm0,xmm0,0D8h} \
    __asm{shufps  xmm2,xmm2,8Dh} \
    __asm{shufps  xmm5,xmm5,0D8h	} \
    __asm{shufps  xmm6,xmm6,8Dh	} \
    __asm{movaps  xmm3,xmm0	} \
    __asm{movlhps xmm0,xmm2} \
    __asm{movhlps xmm2,xmm3	} \
    __asm{movaps  xmm3,xmm5} \
    __asm{movlhps xmm5,xmm6} \
    __asm{movhlps xmm6,xmm3} \
    __asm{movaps  xmm1,xmm2} \
    __asm{shufps  xmm1,xmm1,4Eh} \
    __asm{movaps  xmm3,xmm0} \
    __asm{movaps  xmm4,[ecx*8+esi-20h]} \
    __asm{mulps   xmm4,xmm2} \
    __asm{addps   xmm0,xmm4	} \
    __asm{movaps  xmm4,[ecx*8+esi-10h]} \
    __asm{mulps   xmm4,xmm1} \
    __asm{addps   xmm0,xmm4	} \
    __asm{movaps  xmm1,xmm6} \
    __asm{shufps  xmm1,xmm1,4Eh} \
    __asm{movaps  xmm2,xmm5} \
    __asm{movaps  xmm4,[ecx*8+esi+20h]} \
    __asm{mulps   xmm4,xmm6} \
    __asm{addps   xmm5,xmm4} \
    __asm{movaps  xmm4,[ecx*8+esi+30h]} \
    __asm{mulps   xmm4,xmm1} \
    __asm{addps   xmm5,xmm4} \
    __asm{mulps   xmm0,xmm0} \
    __asm{mulps   xmm5,xmm5} \
    __asm{movaps  xmm4,xmm0} \
    __asm{movlhps xmm0,xmm5} \
    __asm{movhlps xmm5,xmm4} \
    __asm{addps   xmm0,xmm5} \
    __asm{sqrtps  xmm0,xmm0} \
    __asm{movups  [edi+ecx],xmm0} \
    __asm{add     ecx,10h} \
    __asm{sub     edx,10h} \
    __asm{cmp	  ecx,154h} \
    __asm{jnz     L_P256_ctr_trans_op84}

/*********************************************************************
inline void P256_ctr_trans_op84(float* x,float* sqrt_x)
{
	__asm
	{
		mov		eax,x
		mov		ecx,04h
		mov		edx,3F8h
		lea     esi,CTR_W_P256
		mov     edi,sqrt_x
		movaps  xmm7,half_vector
		fld     [eax]
		fadd    [eax+4h]
		fstp    [edi]
L1:
		movups  xmm0,[eax+ecx*2]              ;xmm0 <---  a1b1a2b2
		movups  xmm5,[eax+ecx*2+10h]          ;xmm5 <---  a3b3a4b4
		movaps  xmm1,[eax+edx*2]              ;xmm1 <---  aN-2bN-2aN-1bN-1
		movaps  xmm4,[eax+edx*2-10h]          ;xmm4 <---  aN-4bN-4aN-3bN-3
		movaps  xmm2,xmm0                     ;xmm2 <---  a1b1a2b2
		movaps	xmm6,xmm5                     ;xmm6 <---  a3b3a4b4
		shufps  xmm1,xmm1,4Eh                 ;xmm1 <---  aN-1bN-1aN-2bN-2
		shufps  xmm4,xmm4,4Eh                 ;xmm4 <---  aN-3bN-3aN-4bN-4
		addps   xmm0,xmm1                     ;xmm0 <---  [a1+aN-1][b1+bN-1][a2+aN-2][b2+bN-2]
		addps   xmm5,xmm4
		subps   xmm2,xmm1                     ;xmm2 <---  [a1-aN-1][b1-bN-1][a2-aN-2][b2-bN-2]
		subps   xmm6,xmm4
		mulps   xmm0,xmm7                     
		mulps   xmm5,xmm7
		mulps   xmm2,xmm7
		mulps   xmm6,xmm7                     ;xmm0,xmm2,xmm5,xmm6乘以0.5
		shufps  xmm0,xmm0,0D8h                ;xmm0 <---  [a1+aN-1][a2+aN-2][b1+bN-1][b2+bN-2]
		shufps  xmm2,xmm2,8Dh				  ;xmm2 <---  [b1-bN-1][b2-bN-2][a1-aN-1][a2-aN-2]
		shufps  xmm5,xmm5,0D8h				  ;abcd -> acbd
		shufps  xmm6,xmm6,8Dh	              ;efgh -> fheg	
		movaps  xmm3,xmm0					  ;xmm2 <-> xmm2 高低交换
		movlhps xmm0,xmm2                     ;xmm0 <---  [a1+aN-1][a2+aN-2][b1-bN-1][b2-bN-2]
		movhlps xmm2,xmm3					  ;xmm2 <---  [b1+bN-1][b2+bN-2][a1-aN-1][a2-aN-2]
		movaps  xmm3,xmm5
		movlhps xmm5,xmm6
		movhlps xmm6,xmm3					  ;xmm5 <-> xmm6 高低交换
		movaps  xmm1,xmm2                     ;shufps 4E: bdeg -> egbd
		shufps  xmm1,xmm1,4Eh				  ;xmm1 <---  [a1-aN-1][a2-aN-2][b1+bN-1][b2+bN-2] 
		movaps  xmm3,xmm0                     ;xmm3 <---  [a1+aN-1][a2+aN-2][b1-bN-1][b2-bN-2]
		movaps  xmm4,[ecx*8+esi-20h]
		mulps   xmm4,xmm2
		addps   xmm0,xmm4					  ;xmm0 <---  [a1+aN-1+cos_2n_k*(b1+bN-1)][a2+aN-2+cos_2n_k+1*(b2+bN-2)][b1-bN-1-cos_2n_k*(a1-aN-1)][b2-bN-2-1-cos_2n_k+1*(a2-a2N-2)]
		movaps  xmm4,[ecx*8+esi-10h]
		mulps   xmm4,xmm1
		addps   xmm0,xmm4					  ;xmm0 <---  [a1+aN-1+cos_2n_k*(b1+bN-1)-sin_2n_k*(a1-aN-1)][a2+aN-2+cos_2n_k+1*(b2+bN-2)-sin_2n_k+1*(a2-aN-2)][b1-bN-1-cos_2n_k*(a1-aN-1)-sin_2n_k*(b1+bN-1)]
#if 0   // 将此条件编译开关设置为1的时候 则输出全部256个频率点,而不是仅仅84点
		movaps  xmm4,[ecx*8+esi]
		mulps   xmm4,xmm2
		addps   xmm3,xmm4					  ;xmm3 <---  [a1+aN-1+cos_2n_n-k*(b1+bN-1)][a2+aN-2+cos_2n_n-k-1*(b2+bN-2)][b1-bN-1-cos_2n_n-k*(a1-aN-1)][b2-bN-2-1-cos_2n_n-k-1*(a2-a2N-2)]
		movaps  xmm4,[ecx*8+esi+10h]
		mulps   xmm4,xmm1
		addps   xmm3,xmm4					  ;xmm3 <---  [a1+aN-1+cos_2n_n-k*(b1+bN-1)+sin_2n_n-k*(a1-aN-1)][a2+aN-2+cos_2n_n-k-1*(b2+bN-2)+sin_2n_n-k-1*(a2-aN-2)][b1-bN-1-cos_2n_k*(a1-aN-1)+sin_2n_n-k*(b1+bN-1)]
		movaps  xmm1,xmm6
		shufps  xmm1,xmm1,4Eh
		movaps  xmm2,xmm5
		movaps  xmm4,[ecx*8+esi+20h]
		mulps   xmm4,xmm6
		addps   xmm5,xmm4
		movaps  xmm4,[ecx*8+esi+30h]
		mulps   xmm4,xmm1
		addps   xmm5,xmm4
		movaps  xmm4,[ecx*8+esi+40h]
		mulps   xmm4,xmm6
		addps   xmm2,xmm4
		movaps  xmm4,[ecx*8+esi+50h]
		mulps   xmm4,xmm1
		addps   xmm2,xmm4
		mulps   xmm0,xmm0
		mulps   xmm5,xmm5
		mulps   xmm3,xmm3
		mulps   xmm2,xmm2
		movaps  xmm4,xmm0
		movlhps xmm0,xmm3
		movhlps xmm3,xmm4
		addps   xmm0,xmm3
		sqrtps  xmm0,xmm0
		movaps  xmm4,xmm5
		movlhps xmm5,xmm2
		movhlps xmm2,xmm4
		addps   xmm5,xmm2
		sqrtps  xmm5,xmm5
		movaps  xmm3,xmm0
		movlhps xmm0,xmm5
		movhlps xmm5,xmm3
		shufps  xmm5,xmm5,1Bh
		movups  [edi+ecx],xmm0
		movaps  [edi+edx-8h],xmm5
		add     ecx,10h
		sub     edx,10h
		cmp		ecx,204h
		jnz L1
#else
		movaps  xmm1,xmm6
		shufps  xmm1,xmm1,4Eh
		movaps  xmm2,xmm5
		movaps  xmm4,[ecx*8+esi+20h]
		mulps   xmm4,xmm6
		addps   xmm5,xmm4
		movaps  xmm4,[ecx*8+esi+30h]
		mulps   xmm4,xmm1
		addps   xmm5,xmm4
		mulps   xmm0,xmm0
		mulps   xmm5,xmm5
		movaps  xmm4,xmm0
		movlhps xmm0,xmm5
		movhlps xmm5,xmm4
		addps   xmm0,xmm5
		sqrtps  xmm0,xmm0
		movups  [edi+ecx],xmm0
		add     ecx,10h
		sub     edx,10h
		cmp		ecx,154h
		jnz L1
#endif		
	}
}
****************************************************************************/
inline void rfft512_op84(float* x,float* sqrt_x)
{

	/* 依次把 8个碟形展开! */
	__asm{mov eax,x}
	__asm{mov edi,sqrt_x}

	/* 第一级碟形 L1 only */
	L1_Atom_P256(0);
	/* 第二级碟形 L1 L2 */	
	L1_Atom_P128(0);
	L2_Atom_P128(1024);
	/* 第三级碟形 L1 L2 L1 L1*/

	L1_Atom_P64(0);
	L2_Atom_P64(512);
	L1_Atom_P64(1024);
	L1_Atom_P64(1536);
	/* 第四级碟形 L1 L2 L1 L1 L1 L2 L1 L2 */
	L1_Atom_P32(0);
	L2_Atom_P32(256);
	L1_Atom_P32(512);
	L1_Atom_P32(768);
	L1_Atom_P32(1024);
	L2_Atom_P32(1280);
	L1_Atom_P32(1536);
	L2_Atom_P32(1792);
	/* 第五级碟形 L1 L2 L1 L1
	              L1 L2 L1 L2  
				  L1 L2 L1 L1  
				  L1 L2 L1 L1 */
	L1_Atom_P16(0);
	L2_Atom_P16(128);
	L1_Atom_P16(256);
	L1_Atom_P16(384);
	L1_Atom_P16(512);
	L2_Atom_P16(640);
	L1_Atom_P16(768);
	L2_Atom_P16(896);
	L1_Atom_P16(1024);
	L2_Atom_P16(1152);
	L1_Atom_P16(1280);
	L1_Atom_P16(1408);
	L1_Atom_P16(1536);
	L2_Atom_P16(1664);
	L1_Atom_P16(1792);
	L1_Atom_P16(1920);
	/* 第六、七、八级碟形 L1 L2 L1 L1 L1 L2 L1 L2 
	              L1 L2 L1 L1 L1 L2 L1 L1 
				  L1 L2 L1 L1 L1 L2 L1 L2
				  L1 L2 L1 L1 L1 L2 L1 L2 */
	L1_Atom_P8_4_2(0);
	L2_Atom_P8_4_2(64);
	L1_Atom_P8_4_2(128);
	L1_Atom_P8_4_2(192);
	L1_Atom_P8_4_2(256);
	L2_Atom_P8_4_2(320);
	L1_Atom_P8_4_2(384);
	L2_Atom_P8_4_2(448);
	L1_Atom_P8_4_2(512);
	L2_Atom_P8_4_2(576);
	L1_Atom_P8_4_2(640);
	L1_Atom_P8_4_2(704);
	L1_Atom_P8_4_2(768);
	L2_Atom_P8_4_2(832);
	L1_Atom_P8_4_2(896);
	L1_Atom_P8_4_2(960);
	L1_Atom_P8_4_2(1024);
	L2_Atom_P8_4_2(1088);
	L1_Atom_P8_4_2(1152);
	L1_Atom_P8_4_2(1216);
	L1_Atom_P8_4_2(1280);
	L2_Atom_P8_4_2(1344);
	L1_Atom_P8_4_2(1408);
	L2_Atom_P8_4_2(1472);
	L1_Atom_P8_4_2(1536);
	L2_Atom_P8_4_2(1600);
	L1_Atom_P8_4_2(1664);
	L1_Atom_P8_4_2(1728);
	L1_Atom_P8_4_2(1792);
	L2_Atom_P8_4_2(1856);
	L1_Atom_P8_4_2(1920);
	L2_Atom_P8_4_2(1984);
	/* 最后一个transform */
	P256_Reverse;
	P256_ctr_trans_op84;
}

#else

inline size_t reverse_bits(size_t idx, size_t num_bits)
{
	size_t i = 0;
	size_t ret = 0;
	for (; i < num_bits; ++i)
	{
		ret = (ret << 1) | (idx & 0x1);
		idx >>= 1;
	}

	return ret;
}

inline int cplus_fft(float* data, float* real, float* image, size_t fft_num, int flag)
{
	size_t num_bits = 0;
	size_t i = fft_num;
	while ((i & 0x1) == 0)
	{
		++num_bits;
		i >>= 1;
	}
	memset(image, 0, sizeof(float) * fft_num);
	for (i = 0; i != fft_num; ++i)
	{
		real[reverse_bits(i, num_bits)] = data[i];
	}

	for (unsigned int s = 2; s <= fft_num; s <<= 1)
	{	
		float theta = 2 * PI * flag / s;
		float wr = cos(theta);
		float wi = sin(theta);
		float twr = 1.0f;
		float twi = 0.0f;
		for (unsigned int m = 0; m < s/2; m++)
		{
			for (unsigned int i = m; i < fft_num ; i += s)
			{
				unsigned int j = i + s/2;
				float tmpr = twr * real[j] - twi * image[j];
				float tmpi = twr * image[j] + twi * real[j];
				real[j] = real[i] - tmpr;
				image[j] = image[i] - tmpi;
				real[i] += tmpr;
				image[i] += tmpi;
			}
			float tmp = twr;
			twr = tmp * wr - twi * wi;
			twi = twi * wr + tmp * wi;
		}
	}
	if (flag < 0)
	{
		float br = 1.0f / fft_num;
		for (int i=0; i != fft_num; ++i)
		{
			real[i] *= br;
			image[i] *= br;
		}
	}

	return 0;
}
#endif

#endif