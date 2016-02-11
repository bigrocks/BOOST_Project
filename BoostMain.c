//###########################################################################
// Description
//! \addtogroup f2833x_example_list
//! <h1> ADC Start of Conversion (adc_soc)</h1>
//!
//! This ADC example uses ePWM1 to generate a periodic ADC SOC on SEQ1.
//! Two channels are converted, ADCINA3 and ADCINA2.
//!
//! \b Watch \b Variables \n
//! - Voltage1[10]	- Last 10 ADCRESULT0 values
//! - Voltage2[10]	- Last 10 ADCRESULT1 values
//! - ConversionCount	- Current result number 0-9
//! - LoopCount		- Idle loop counter
//
//
//###########################################################################
// $TI Release: F2833x/F2823x Header Files and Peripheral Examples V141 $
// $Release Date: November  6, 2015 $
// $Copyright: Copyright (C) 2007-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "My_BSP.h"
#include "FFT_DIT.h"				//����ͷ�ļ�
#include "HMICMD_Queue.h"
#include "HMI_Driver.h"



// Prototype statements for functions found within this file.

// Global variables used in this example:
Uint16 LoopCount;
Uint16 ConversionCount;

// for PIE Vector
extern const struct PIE_VECT_TABLE PieVectTableInit;
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;

volatile float AdcResult[1];	//ADC�������
volatile Uint16 AdcPnt=0;			//����ADC��������õ���λ��ָ��
volatile COMPLEX Signal[1]; 	//��FFT����

main()
{
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();
   InitCpuTimers();
	#if (CPU_FRQ_150MHZ)
	// Configure CPU-Timer 0 to interrupt every 50ms:
	// 150MHz CPU Freq, 20ms Period (in uSeconds)
   	// TPR = 860ns
    // PDR = 150*400 = 60000;
    // ��ʱ���� 60000*860ns = 51.6ms
	ConfigCpuTimer(&CpuTimer0, 150, 400); // 2.5KHZ
	#endif

   EALLOW;
   #if (CPU_FRQ_150MHZ)     // Default - 150 MHz SYSCLKOUT
     #define ADC_MODCLK 0x3 // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 150/(2*3)   = 25.0 MHz
   #endif
   #if (CPU_FRQ_100MHZ)
     #define ADC_MODCLK 0x2 // HSPCLK = SYSCLKOUT/2*ADC_MODCLK2 = 100/(2*2)   = 25.0 MHz
   #endif
   EDIS;

   // Define ADCCLK clock frequency ( less than or equal to 25 MHz )
   // Assuming InitSysCtrl() has set SYSCLKOUT to 150 MHz
   EALLOW;
//   SysCtrlRegs.HISPCP.all = ADC_MODCLK;
   EDIS;

// Step 2. Initialize GPIO:
// This example function is found in the DSP2833x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.


// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
   DINT;

// Initialize the PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the DSP2833x_PieCtrl.c file.
   InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;

   PieCtrlRegs.PIECTRL.bit.ENPIE = 0;	// Disable the PIE
   	asm(" EALLOW");
   	memcpy((void *)0x000D00, &PieVectTableInit, 256);
   	asm(" EDIS");
   PieCtrlRegs.PIECTRL.bit.ENPIE = 1;
   // #4
   	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

   	InitFlash();
// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in DSP2833x_DefaultIsr.c.
// This function is found in DSP2833x_PieVect.c.
   InitPieVectTable();

   EnableInterrupts();
// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
   EALLOW;  // This is needed to write to EALLOW protected register
   //���� 2 ������������Ϊ�жϷ�ʽ����ʼ������
//   PieVectTable.SCIRXINTA = &SciARxFIFOIsrA;
   //PieVectTable.ADCINT = &ADC_Isr;
   //PieVectTable.TINT0 = &CPU_Timer0_Isr;
   PieVectTable.SCIRXINTB =&MySciB485Rx_IsrB;//-------SCI485�����ж�--------
   PieVectTable.SCIRXINTC =&MySciC232Rx_IsrC;//-------SCI232�����ж�--------

   EDIS;    // This is needed to disable write to EALLOW protected registers

// Step 4. Initialize all the Device Peripherals:
   InitGPIO();
   My_InitADC();  // For this example, init the ADC
   Init_LED();
   SciB485_Init();
   SciC232_Init();

   //WnGenerate(); 					//��ʼ����ת����

   CpuTimer0Regs.TCR.bit.TSS = 0; //������ʱ��0


// Step 5. User specific code, enable interrupts:
// ����5��Ӧ�ô��룬�ж�ʹ��
// Enable INT in PIE
   PieCtrlRegs.PIECTRL.bit.ENPIE = 1;//Enable the PIE block ʹ�����ж�ģ
   //PieCtrlRegs.PIEIER1.bit.INTx7 = 1;//ʹ��CpuTimer0�жϷ���
   //PieCtrlRegs.PIEIER1.bit.INTx6 = 1;//ADC
   PieCtrlRegs.PIEIER8.bit.INTx5 = 1;//SCIC232
   PieCtrlRegs.PIEIER9.bit.INTx3 = 1;//SCIB485

   IER |= (M_INT1 | M_INT8 | M_INT9); // Enable CPU Interrupt 1
                  // ʹ������CpuTimer0��INT1�ж�


   EINT;          // Enable Global interrupt INTM
   	   	   	   	  // ʹ��INTMȫ���ж�
   ERTM;          // Enable Global realtime interrupt DBGM
                  // �����¼�ʹ��

   LoopCount = 0;

   queue_reset();/*��մ��ڽ��ջ�����*/

   for(;;)
   {
      LoopCount++;
      LED_4_On;
      LED_3_Off;
      DELAY_US(1000);
      if(LoopCount > 1000)
      {
    	  //SciC232_TXmit(0xaa);
    	  MY_HMI_Handle();
    	     LED_2_Toggle;
    	     LED_1_Toggle;
    	  LoopCount = 0;
      }
		if(AdcPnt == FFT_NUM)		//���ɼ���FFT_NUM��󣬿�ʼ��FFT
		{
			//FFT_DIT(Signal);		//����FFT������
			//AdcPnt = 0;				//����ָ����㣬Ϊ�´βɼ���׼��
		}
   }
}

__interrupt void  ADC_Isr(void)
{
	//��ȡADC����ֵ
	if(AdcPnt<FFT_NUM)
	{
		Signal[AdcPnt].real = 3.0*(AdcRegs.ADCRESULT0 >>4)/4095;
		Signal[AdcPnt].imag = 0.0;
		AdcResult[AdcPnt] =  Signal[AdcPnt].real;//���������Ի�ͼ��ʾԭʼ�����ź�
	 	AdcPnt++;
	}
/*	ADC_Value[0][ConversionCount] = AdcRegs.ADCRESULT0>>4; //���ADCINA0�Ľ��
	//((float)AdcRegs.ADCRESULT0)*3.0/65520;											   //���ADCINA0�Ľ��
	ADC_Value[1][ConversionCount] = AdcRegs.ADCRESULT1>>4; //���ADCINB0�Ľ��
	ADC_Value[2][ConversionCount] = AdcRegs.ADCRESULT2>>4; //���ADCINA1�Ľ��
	ADC_Value[3][ConversionCount] = AdcRegs.ADCRESULT3>>4; //���ADCINB1�Ľ��

	ADC_Value[4][ConversionCount] = AdcRegs.ADCRESULT4>>4; //���ADCINA2�Ľ��
	ADC_Value[5][ConversionCount] = AdcRegs.ADCRESULT5>>4; //���ADCINB2�Ľ��
	ADC_Value[6][ConversionCount] = AdcRegs.ADCRESULT6>>4; //���ADCINA3�Ľ��
	ADC_Value[7][ConversionCount] = AdcRegs.ADCRESULT7>>4; //���ADCINB3�Ľ��
*/

  // If 40 conversions have been logged, start over
  if(ConversionCount > 255)
  {
		SciB485_TXmit(ADC_Value[0][ConversionCount]>>8);SciB485_TXmit(ADC_Value[0][ConversionCount]);
		SciB485_TXmit(ADC_Value[1][ConversionCount]>>8);SciB485_TXmit(ADC_Value[1][ConversionCount]);
		SciB485_TXmit(ADC_Value[2][ConversionCount]>>8);SciB485_TXmit(ADC_Value[2][ConversionCount]);
		SciB485_TXmit(ADC_Value[3][ConversionCount]>>8);SciB485_TXmit(ADC_Value[3][ConversionCount]);
/*		SciB_TXmit(ADC_Value[4][ConversionCount]>>8);SciB_TXmit(ADC_Value[4][ConversionCount]);
		SciB_TXmit(ADC_Value[5][ConversionCount]>>8);SciB_TXmit(ADC_Value[5][ConversionCount]);
		SciB_TXmit(ADC_Value[6][ConversionCount]>>8);SciB_TXmit(ADC_Value[6][ConversionCount]);
		SciB_TXmit(ADC_Value[7][ConversionCount]>>8);SciB_TXmit(ADC_Value[7][ConversionCount]);*/
     ConversionCount = 0;
     LED_2_Toggle;
     LED_1_Toggle;
  }
  else
  {
      ConversionCount++;
  }

  // Reinitialize for next ADC sequence
  AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1 ��λ���з�����SEQ1
  AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;       // Clear INT SEQ1 bit  ���AD�жϵı�־λ
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE
                                            //��ӦPIEͬ���ж�
  return;
}


__interrupt void CPU_Timer0_Isr(void)
{
	CpuTimer0.InterruptCount ++;
	if(CpuTimer0.InterruptCount > 1)
	{
		SciB485_TXmit(0xaa);
		CpuTimer0.InterruptCount = 0;
	}
	/*Ӧ������ж��Ա�ʹPIE 1����պ�����ж�*/
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}


