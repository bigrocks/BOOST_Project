#ifndef FFT_DIT_HEADER
	#define FFT_DIT_HEADER									//����FFT_DITͷ�ļ�
	#ifndef PI
		#define PI
		#define pi 3.1415926535897932384626433832795L		//����Բ����
	#endif
	#define FFT_NUM	128									//������FFT�ĵ���
	//������FFT�ĵ�����������Ӧ����λ���ú���
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
	//���帴���ṹ���������
	typedef struct _COMPLEX
	{
		float real;
		float imag;
	}COMPLEX;
	void WnGenerate( void ) ;				//������ת���Ӻ�������
	Uint16 ReverseBit (Uint16 i);			//��λ���ú�������
	void FFT_DIT(volatile COMPLEX sig[]);	//��ʱ���ȡFFT�任��������	
#endif
