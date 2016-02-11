#ifndef FFT_DIT_HEADER
	#define FFT_DIT_HEADER									//定义FFT_DIT头文件
	#ifndef PI
		#define PI
		#define pi 3.1415926535897932384626433832795L		//定义圆周率
	#endif
	#define FFT_NUM	128									//定义做FFT的点数
	//根据做FFT的点数，定义相应的码位倒置函数
	#if	FFT_NUM==2048
		#define  ReverseBit11
	#elif	FFT_NUM==1024
		#define  ReverseBit10
	#elif	FFT_NUM==512
		#define  ReverseBit9
	#elif	FFT_NUM==256
		#define  ReverseBit8
	#elif	FFT_NUM==128
		#define  ReverseBit7
	#elif	FFT_NUM==64
		#define  ReverseBit6
	#elif	FFT_NUM==32
		#define  ReverseBit5
	#elif	FFT_NUM==16
		#define  ReverseBit4
	#elif	FFT_NUM==8
		#define  ReverseBit3
	#elif	FFT_NUM==4
		#define  ReverseBit2
	#elif	FFT_NUM==2
		#define  ReverseBit1
	#else
		#define  Nothing
	#endif
	//定义复数结构体变量类型
	typedef struct _COMPLEX
	{
		float real;
		float imag;
	}COMPLEX;
	void WnGenerate( void ) ;				//产生旋转因子函数声明
	Uint16 ReverseBit (Uint16 i);			//码位倒置函数声明
	void FFT_DIT(volatile COMPLEX sig[]);	//按时间抽取FFT变换函数声明	
#endif
