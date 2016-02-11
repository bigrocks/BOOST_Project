#include "DSP2833x_Device.h"				//����ͷ�ļ�
#include "DSP2833x_Examples.h"				//����ͷ�ļ�
#include "FFT_DIT.h"						//����ͷ�ļ�
#include <math.h> 							//�������ļ�
COMPLEX WN[FFT_NUM/2]; 						//��ת����
volatile float SpectrumResult[FFT_NUM];		//Ƶ�׽������
//---------------------------------------- ��ת���ӳ�ʼ������-----------------------------------------//
void WnGenerate( void )
{
 	Uint16 i;
 	for(i=0;i<FFT_NUM/2;i++)
 	{
	 	WN[i].real = cos(2.0L*pi/FFT_NUM*i);
	 	WN[i].imag = sin(-2.0L*pi/FFT_NUM*i); 	
 	}
	}
 //---------------------------------------- ��ʱ���ȡFFT����-----------------------------------------//
void FFT_DIT(volatile COMPLEX sig[])
{
	Uint16 i=0,j=0;
	Uint16 SubFftNum = 2;					//ÿ����fftģ������fft�ĵ���
	Uint16 FftPointer=0;					//��ǰ��fft�ĵ��λ��
	Uint16 SubFftPointer=0;					//��ǰ��fft�ĵ���fft��ģ���е�λ��
	Uint16 WNPointer=0;						//��ǰ��fft���õ���ת��������λ��
	COMPLEX x1,x2,temp;						//�м����
	//������λ����
	for(i=0;i<FFT_NUM;i++)
	{
		j=ReverseBit(i);					//������λ���ó���
		if(j>i)								//j>i�ٵ��ã���ֹ�ظ�����
		{
	   		temp = sig[i] ;	
			sig[i] = sig[j];	
			sig[j] = temp; 
		} 	 
	} 
	//��ʼ��FFT
	while(SubFftNum <= FFT_NUM )			//SubFftNum��2����FFT_NUMΪֹ
	{
		FftPointer = 0;
		while(FftPointer<FFT_NUM)			//�������е��FFTϵ��
		{
			for(SubFftPointer=0;SubFftPointer<SubFftNum/2;SubFftPointer++)//������ģ���FFT
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
			FftPointer += SubFftNum;		//FftPointerָ����һ����FFTģ��λ��
	 	}
	 	SubFftNum +=SubFftNum;		 		//��FFT������2��������һ������
	}
	//�����ֵ��
	for(i=0;i<FFT_NUM;i++)
 	{
	 	SpectrumResult[i] = sqrt(sig[i].real * sig[i].real  +  sig[i].imag* sig[i].imag)*2/FFT_NUM;
	}
}	 
 //---------------------------------------- ��λ���ó���-----------------------------------------//
Uint16 ReverseBit (Uint16 i)
{
	Uint16 j;
	//������FFT�ĵ�����������Ӧ����λ���ó��򣬼����λ�����λֵ�������θ�λ��ε�λֵ
	//�������Դ����ƣ���������λֵ��
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
