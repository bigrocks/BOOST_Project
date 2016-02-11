#include "DSP2833x_Device.h"				//包含头文件
#include "DSP2833x_Examples.h"				//包含头文件
#include "FFT_DIT.h"						//包含头文件
#include <math.h> 							//包含库文件
COMPLEX WN[FFT_NUM/2]; 						//旋转因子
volatile float SpectrumResult[FFT_NUM];		//频谱结果数组
//---------------------------------------- 旋转因子初始化程序-----------------------------------------//
void WnGenerate( void )
{
 	Uint16 i;
 	for(i=0;i<FFT_NUM/2;i++)
 	{
	 	WN[i].real = cos(2.0L*pi/FFT_NUM*i);
	 	WN[i].imag = sin(-2.0L*pi/FFT_NUM*i); 	
 	}
	}
 //---------------------------------------- 按时间抽取FFT程序-----------------------------------------//
void FFT_DIT(volatile COMPLEX sig[])
{
	Uint16 i=0,j=0;
	Uint16 SubFftNum = 2;					//每个子fft模块所做fft的点数
	Uint16 FftPointer=0;					//当前做fft的点的位置
	Uint16 SubFftPointer=0;					//当前做fft的点在fft子模块中的位置
	Uint16 WNPointer=0;						//当前做fft所用的旋转因子所在位置
	COMPLEX x1,x2,temp;						//中间变量
	//首先码位倒置
	for(i=0;i<FFT_NUM;i++)
	{
		j=ReverseBit(i);					//调用码位倒置程序
		if(j>i)								//j>i再倒置，防止重复倒置
		{
	   		temp = sig[i] ;	
			sig[i] = sig[j];	
			sig[j] = temp; 
		} 	 
	} 
	//开始做FFT
	while(SubFftNum <= FFT_NUM )			//SubFftNum从2做到FFT_NUM为止
	{
		FftPointer = 0;
		while(FftPointer<FFT_NUM)			//计算所有点的FFT系数
		{
			for(SubFftPointer=0;SubFftPointer<SubFftNum/2;SubFftPointer++)//计算子模块的FFT
			{
				x1 = sig[FftPointer+SubFftPointer];	
				x2 = sig[FftPointer+SubFftPointer+SubFftNum/2];
				WNPointer = FFT_NUM/SubFftNum*SubFftPointer;
				temp.real = x2.real*WN[WNPointer].real - x2.imag*WN[WNPointer].imag;
				temp.imag = x2.real*WN[WNPointer].imag + x2.imag*WN[WNPointer].real;
			 
				sig[FftPointer+SubFftPointer].real = x1.real+temp.real;
				sig[FftPointer+SubFftPointer].imag = x1.imag+temp.imag;
				sig[FftPointer+SubFftPointer+SubFftNum/2].real = x1.real-temp.real;
				sig[FftPointer+SubFftPointer+SubFftNum/2].imag =x1.imag-temp.imag ;
			}
			FftPointer += SubFftNum;		//FftPointer指向下一个子FFT模块位置
	 	}
	 	SubFftNum +=SubFftNum;		 		//子FFT点数乘2，进行下一轮运算
	}
	//计算幅值谱
	for(i=0;i<FFT_NUM;i++)
 	{
	 	SpectrumResult[i] = sqrt(sig[i].real * sig[i].real  +  sig[i].imag* sig[i].imag)*2/FFT_NUM;
	}
}	 
 //---------------------------------------- 码位倒置程序-----------------------------------------//
Uint16 ReverseBit (Uint16 i)
{
	Uint16 j;
	//根据做FFT的点数，定义相应的码位倒置程序，即最高位和最低位值互换，次高位与次低位值
	//互换，以此类推，互换所有位值。
	#ifdef	ReverseBit11
	{
		j =  (i&0x400)>>10 | (i&0x200)>>8 | (i&0x100)>>6 | (i&0x080)>>4 | (i&0x040)>>2
	   		| (i&0x020)
	   		| (i&0x010)<<2 | (i&0x008)<<4 | (i&0x004)<<6 | (i&0x02)<<8  | (i&0x01)<<10;
		return(j);	
	}
	#endif
	#ifdef	ReverseBit10
	{
	 	j =  (i&0x200)>>9 | (i&0x100)>>7 | (i&0x080)>>5 | (i&0x040)>>3 | (i&0x020)>>1
			|(i&0x010)<<1 | (i&0x008)<<3 | (i&0x004)<<5 | (i&0x002)<<7 | (i&0x001)<<9;
		return(j);
	}
	#endif
	#ifdef	ReverseBit9
	{
		j =   (i&0x100)>>8 | (i&0x080)>>6 | (i&0x040)>>4 | (i&0x020)>>2| (i&0x010)
	   		| (i&0x008)<<2 | (i&0x004)<<4 | (i&0x002)<<6 | (i&0x01)<<8;
		return(j);	
	}
	#endif 
	#ifdef	ReverseBit8
	{
		j =  (i&0x80)>>7 | (i&0x40)>>5 | (i&0x20)>>3 | (i&0x10)>>1
			|(i&0x08)<<1 | (i&0x04)<<3 | (i&0x02)<<5 | (i&0x01)<<7;
		return(j);
	}
	#endif
	#ifdef	ReverseBit7
	{
		j =    (i&0x40)>>6 | (i&0x20)>>4 | (i&0x10)>>2 | (i&0x08)
	  		 | (i&0x04)<<2 | (i&0x02)<<4 | (i&0x01)<<6;
		return(j);	
	}
	#endif
	#ifdef	ReverseBit6
	{
		j = (i&0x20)>>5 | (i&0x10)>>3 | (i&0x08)>>1|(i&0x04)<<1 | (i&0x02)<<3 | (i&0x01)<<5 ;
		return(j);
	}
	#endif
	#ifdef	ReverseBit5
	{
		j = (i&0x10)>>4 | (i&0x08)>>2 | (i&0x04) | (i&0x02)<<2 | (i&0x01)<<4 ;
		return(j);	
	}
	#endif
	#ifdef	ReverseBit4
	{
		j = (i&0x08)>>3 | (i&0x04)>>1|(i&0x02)<<1 | (i&0x01)<<3 ;
		return(j);
	}
	#endif
	#ifdef	ReverseBit3
	{
		j = (i&0x04)>>2  | (i&0x02)  | (i&0x01)<<2  ;
		return(j);	
	}
	#endif
	#ifdef	ReverseBit2
	{
		j = (i&0x02)>>1|(i&0x01)<<1  ;
		return(j);
	}
	#endif
	#ifdef	Nothing
		error
	#endif
}
