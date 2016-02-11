#include "DSP28x_Project.h"
#include "My_BSP.h"
#include "HMICMD_Queue.h"
#include "stdio.h"
#include "HMI_Driver.h"
#include "My_VarDefined.h"

#define ADC_usDELAY  5000L
#define TIME_100MS 10

static int32 test_value = 0;//TODO Ҫ�ĵ�
static Uint8 update_en = 0;
volatile Uint32  timer_tick_count = 0; //��ʱ������

Uint8 cmd_buffer[CMD_MAX_SIZE];

Uint16 ADC_Value[8][256];
Uint16 rdataB[16];

void InitGPIO(void)
{
   EALLOW;

   // Each GPIO pin can be:
   // a) a GPIO input/output
   // b) peripheral function 1
   // c) peripheral function 2
   // d) peripheral function 3
   // By default, all are GPIO Inputs
   GpioCtrlRegs.GPAMUX1.all = 0x0000;     // GPIO functionality GPIO0-GPIO15
   GpioCtrlRegs.GPAMUX2.all = 0x0000;     // GPIO functionality GPIO16-GPIO31
   GpioCtrlRegs.GPBMUX1.all = 0x0000;     // GPIO functionality GPIO32-GPIO39
   GpioCtrlRegs.GPBMUX2.all = 0x0000;     // GPIO functionality GPIO48-GPIO63
   GpioCtrlRegs.GPCMUX1.all = 0x0000;     // GPIO functionality GPIO64-GPIO79
   GpioCtrlRegs.GPCMUX2.all = 0x0000;     // GPIO functionality GPIO80-GPIO95

   GpioCtrlRegs.GPADIR.all = 0x0000;      // GPIO0-GPIO31 are inputs
   GpioCtrlRegs.GPBDIR.all = 0x0000;      // GPIO32-GPIO63 are inputs
   GpioCtrlRegs.GPCDIR.all = 0x0000;      // GPI064-GPIO95 are inputs

   // Each input can have different qualification
   // a) input synchronized to SYSCLKOUT
   // b) input qualified by a sampling window
   // c) input sent asynchronously (valid for peripheral inputs only)
   GpioCtrlRegs.GPAQSEL1.all = 0x0000;    // GPIO0-GPIO15 Synch to SYSCLKOUT
   GpioCtrlRegs.GPAQSEL2.all = 0x0000;    // GPIO16-GPIO31 Synch to SYSCLKOUT
   GpioCtrlRegs.GPBQSEL1.all = 0x0000;    // GPIO32-GPIO39 Synch to SYSCLKOUT
   GpioCtrlRegs.GPBQSEL2.all = 0x0000;    // GPIO48-GPIO63 Synch to SYSCLKOUT

   // Pull-ups can be enabled or disabled.
   GpioCtrlRegs.GPAPUD.all = 0x0000;      // Pullup's enabled GPIO0-GPIO31
   GpioCtrlRegs.GPBPUD.all = 0x0000;      // Pullup's enabled GPIO32-GPIO63
   GpioCtrlRegs.GPCPUD.all = 0x0000;      // Pullup's enabled GPIO64-GPIO79

   EDIS;

}

void Init_LED(void)
{
	EALLOW;
	// LED_4
	GpioCtrlRegs.GPCPUD.bit.GPIO79 = 0;   // Enable pullup on GPIO79
	GpioCtrlRegs.GPCMUX1.bit.GPIO79 = 0;  // GPIO79 = GPIO79
	GpioDataRegs.GPCCLEAR.bit.GPIO79 = 1;
	GpioCtrlRegs.GPCDIR.bit.GPIO79 = 1;   // GPIO79 = output

	// LED_3 LED_BOUNDRY_UP
	GpioCtrlRegs.GPCPUD.bit.GPIO78 = 0;   // Enable pullup on GPIO78
	GpioCtrlRegs.GPCMUX1.bit.GPIO78 = 0;  // GPIO78 = GPIO78
	GpioDataRegs.GPCCLEAR.bit.GPIO78 = 1;
	GpioCtrlRegs.GPCDIR.bit.GPIO78 = 1;   // GPIO78 = output

	// LED_2 LED_TIMER
	GpioCtrlRegs.GPCPUD.bit.GPIO77 = 0;   // Enable pullup on GPIO77
	GpioCtrlRegs.GPCMUX1.bit.GPIO77 = 0;  // GPIO77 = GPIO77
	GpioDataRegs.GPCCLEAR.bit.GPIO77 = 1;
	GpioCtrlRegs.GPCDIR.bit.GPIO77 = 1;   // GPIO77 = output

	//LED_1 LED_BOUNDRY_DOWN
	GpioCtrlRegs.GPCPUD.bit.GPIO76 = 0;   // Enable pullup on GPIO76
	GpioCtrlRegs.GPCMUX1.bit.GPIO76 = 0;  // GPIO76 = GPIO76
	GpioDataRegs.GPCCLEAR.bit.GPIO76 = 1;
	GpioCtrlRegs.GPCDIR.bit.GPIO76 = 1;   // GPIO76 = output

	EDIS;
}


void Init_Relays(void)
{
	EALLOW;

	GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;   // Enable pullup on GPIO2
	GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;  // GPIO2 = GPIO2
	GpioDataRegs.GPASET.bit.GPIO1 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;   // GPIO2 = output Relay 2 ������Դ

	GpioCtrlRegs.GPAPUD.bit.GPIO2 = 0;   // Enable pullup on GPIO4
	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;  // GPIO4 = GPIO4
	GpioDataRegs.GPASET.bit.GPIO2 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;   // GPIO4 = output Relay 3 ��������Դ

	GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;   // Enable pullup on GPIO6
	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;  // GPIO6 = GPIO6
	GpioDataRegs.GPASET.bit.GPIO4 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;   // GPIO6 = output Relay 4 �Ӵ���1

	GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;   // Enable pullup on GPIO1
	GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;  // GPIO1 = GPIO1
	GpioDataRegs.GPASET.bit.GPIO5 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;   // GPIO1 = output Relay 5 �Ӵ���2

	EDIS;
}
/*
 * SCIģ��
 */
void SciB485_Init()
{
	EALLOW;

	// 485 RD
	GpioCtrlRegs.GPAPUD.bit.GPIO22 = 1;   // ��ֹ�ڲ���������Ϊ�ⲿ��������
	GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0;  // GPIO22 = GPIO22
	GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO22 = 1;   // GPIO22 = output


	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;	   // Enable pull-up for GPIO18 (SCITXDB)
	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;   // Configure GPIO18 for SCITXDB operation

    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;	   // Enable pull-up for GPIO19 (SCIRXDB)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;  // Asynch input GPIO19 (SCIRXDB)
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;   // Configure GPIO19 for SCIRXDB operation

    EDIS;

	ScibRegs.SCICCR.all =0x0007;    // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
	ScibRegs.SCICTL1.all =0x0003;   // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
	ScibRegs.SCICTL2.bit.TXINTENA =0;//��ֹ�����ж�
	ScibRegs.SCICTL2.bit.RXBKINTENA =1;//ʹ�ܽ����ն�
	#if (CPU_FRQ_150MHZ)
	      ScibRegs.SCIHBAUD    =0x0001;  // 9600 baud @LSPCLK = 37.5MHz.
	      ScibRegs.SCILBAUD    =0x00E7;
	#endif
	#if (CPU_FRQ_100MHZ)
		ScibRegs.SCIHBAUD    =0x0001;  // 9600 baud @LSPCLK = 20MHz.
		ScibRegs.SCILBAUD    =0x0044;
	#endif
		//	ScibRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back
			ScibRegs.SCIFFTX.all=0xC028;
			ScibRegs.SCIFFRX.all=0x002A;
			ScibRegs.SCIFFCT.all=0x00;

			ScibRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset
			ScibRegs.SCIFFTX.bit.TXFIFOXRESET=1;
			ScibRegs.SCIFFRX.bit.RXFIFORESET=1;
	SCIB485RX_On;
}
/*
 * RS232�ӿ�
 * */
void SciC232_Init()
{
	EALLOW;

	GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;	   // Enable pull-up for GPIO18 (SCITXDB)

    GpioCtrlRegs.GPBPUD.bit.GPIO62 = 0;	   // Enable pull-up for GPIO19 (SCIRXDB)

	GpioCtrlRegs.GPBQSEL2.bit.GPIO62 = 3;  // Asynch input GPIO19 (SCIRXDB)

	GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 1;   // Configure GPIO18 for SCITXDB operation

    GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 1;   // Configure GPIO19 for SCIRXDB operation

    EDIS;

	/*	|STOP BITS|EVEN/ODD|PARITY ENABLE|LOOP BAKC ENA|ADDR/IDLE MODE|SCI CHAR0~2|
	 * 		0		1			1			0				0			111
	 * 		0x67
	 *
	 *
	 */
	ScicRegs.SCICCR.all =0x0007;//67;    // 1 stop bit,  No loopback
                                   //  parity,8 char bits,
                                   // async mode, idle-line protocol
	ScicRegs.SCICTL1.all =0x0003;   // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
	ScicRegs.SCICTL2.all =0x0003;
	ScicRegs.SCICTL2.bit.TXINTENA =0;//��ֹ�����ж�
	ScicRegs.SCICTL2.bit.RXBKINTENA =1;
	#if (CPU_FRQ_150MHZ)
	      ScicRegs.SCIHBAUD    =0x0000;//0;  // 79H = 38400 baud @LSPCLK = 37.5MHz.
	      ScicRegs.SCILBAUD    =0x0027;//E7;//79; //1E7H = 9600 baud
	      	  	  	  	  	  	  	  	  //0027H = 115200 ZHENGLEI
	#endif
	#if (CPU_FRQ_100MHZ)
		ScicRegs.SCIHBAUD    =0x0001;  // 38400 baud @LSPCLK = 20MHz.
		ScicRegs.SCILBAUD    =0x0044;
	#endif
//	ScicRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back
	ScicRegs.SCICTL1.all =0x0023;

/*	ScicRegs.SCIFFTX.all=0xC028;
	ScicRegs.SCIFFRX.all=0x0028;
	ScicRegs.SCIFFCT.all=0x00;

	//ScicRegs.SCICCR.bit.PARITYENA = 1;	//Enable PARITY  û��У��
	//ScicRegs.SCICCR.bit.PARITY = 1;		//EVEN
	//ScicRegs.SCICCR.bit.LOOPBKENA = 1;	//Enable loop back

	ScicRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset
	ScicRegs.SCIFFTX.bit.TXFIFOXRESET=1;
	ScicRegs.SCIFFRX.bit.RXFIFORESET=1;*/

}
/*
void SciB_FIFO_Init()
{
	ScibRegs.SCICCR.all =0x0007;    // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
	ScibRegs.SCICTL1.all =0x0003;   // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
	ScibRegs.SCICTL2.bit.TXINTENA =0;//��ֹ�����ж�
	ScibRegs.SCICTL2.bit.RXBKINTENA =1;//ʹ�ܽ����ն�
	#if (CPU_FRQ_150MHZ)
	      ScibRegs.SCIHBAUD    =0x0001;  // 9600 baud @LSPCLK = 37.5MHz.
	      ScibRegs.SCILBAUD    =0x00E7;
	#endif
	#if (CPU_FRQ_100MHZ)
		ScibRegs.SCIHBAUD    =0x0001;  // 9600 baud @LSPCLK = 20MHz.
		ScibRegs.SCILBAUD    =0x0044;
	#endif
//	ScibRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back
	ScibRegs.SCIFFTX.all=0xC028;
	ScibRegs.SCIFFRX.all=0x002A;
	ScibRegs.SCIFFCT.all=0x00;

	ScibRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset
	ScibRegs.SCIFFTX.bit.TXFIFOXRESET=1;
	ScibRegs.SCIFFRX.bit.RXFIFORESET=1;
}

void SciC_FIFO_Init()
{
	/*|STOP BITS|EVEN/ODD|PARITY ENABLE|LOOP BAKC ENA|ADDR/IDLE MODE|SCI CHAR0~2|
	 * 		0		1			1			0				0			111
	 * 		0x67
	 *
	 *
	 */
/*	ScicRegs.SCICCR.all =0x0007;//67;    // 1 stop bit,  No loopback
                                   //  parity,8 char bits,
                                   // async mode, idle-line protocol
	ScicRegs.SCICTL1.all =0x0003;   // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
	ScicRegs.SCICTL2.bit.TXINTENA =1;
	ScicRegs.SCICTL2.bit.RXBKINTENA =1;
	#if (CPU_FRQ_150MHZ)
	      ScicRegs.SCIHBAUD    =0x0001;//0;  // 79H = 38400 baud @LSPCLK = 37.5MHz.
	      ScicRegs.SCILBAUD    =0x00E7;//79; //1E7H = 9600 baud yurobo
	#endif
	#if (CPU_FRQ_100MHZ)
		ScicRegs.SCIHBAUD    =0x0001;  // 38400 baud @LSPCLK = 20MHz.
		ScicRegs.SCILBAUD    =0x0044;
	#endif
//	ScicRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back
	ScicRegs.SCIFFTX.all=0xC028;
	ScicRegs.SCIFFRX.all=0x0028;
	ScicRegs.SCIFFCT.all=0x00;

	ScicRegs.SCICCR.bit.PARITYENA = 1;	//Enable PARITY
	ScicRegs.SCICCR.bit.PARITY = 1;		//EVEN
	//ScicRegs.SCICCR.bit.LOOPBKENA = 1;	//Enable loop back

	ScicRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset
	ScicRegs.SCIFFTX.bit.TXFIFOXRESET=1;
	ScicRegs.SCIFFRX.bit.RXFIFORESET=1;
}*/

// Transmit a character TO the SCI
void SciB485_TXmit(char a)
{
	SCIB485TX_On;
    while (ScibRegs.SCIFFTX.bit.TXFFST != 0) {}
    ScibRegs.SCITXBUF=a;
    SCIB485RX_On;
}

// Transmit a character TO the SCI
void SciC232_TXmit(char a)
{
	 ScicRegs.SCITXBUF=a;
	 while((ScicRegs.SCICTL2.bit.TXRDY & 0x1)==0);

/*    while (ScicRegs.SCIFFTX.bit.TXFFST != 0) {}
    ScicRegs.SCITXBUF=a;*/
}

void SciB485_TXMSG(char * msg)
{
    int i;
    i = 0;
    while(msg[i] != '\0')
    {
    	SciB485_TXmit(msg[i]);
        i++;
    }
}

void SciC232_TXMSG(char * msg)
{
    int i;
    i = 0;
    while(msg[i] != '\0')
    {
    	SciC232_TXmit(msg[i]);
        i++;
    }
}

//-------SCI�����ж�--------
interrupt void MySciB485Rx_IsrB(void)
{
	while(ScibRegs.SCIRXST.bit.RXRDY !=1) { }
	Uint8 ReceivedChar = ScibRegs.SCIRXBUF.all;


	ScibRegs.SCIFFRX.bit.RXFFOVRCLR=1;	//Clear Overflow flag
	ScibRegs.SCIFFRX.bit.RXFFINTCLR=1;	//Clear Interrupt flag
	PieCtrlRegs.PIEACK.all|=0x100;		//Issue PIE ack
}

//-------SCI�����ж�--------
interrupt void MySciC232Rx_IsrC(void)
{
	Uint8 ReceivedChar = ScicRegs.SCIRXBUF.all;
	queue_push(ReceivedChar);

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
/*
	//while(ScicRegs.SCIRXST.bit.RXRDY !=1) { }
	while(ScicRegs.SCIFFRX.bit.RXFFST !=1) { }
	Uint8 ReceivedChar = ScicRegs.SCIRXBUF.all;
	queue_push(ReceivedChar);
	ScicRegs.SCIFFRX.bit.RXFFOVRCLR=1;	//Clear Overflow flag
	ScicRegs.SCIFFRX.bit.RXFFINTCLR=1;	//Clear Interrupt flag
	PieCtrlRegs.PIEACK.all|=0x0080;		//Issue PIE ack
*/
}


void My_InitADC(void)
{
	unsigned int i;
	extern void DSP28x_usDelay(Uint32 Count);

	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;
	ADC_cal();
	EDIS;
	AdcRegs.ADCTRL1.bit.RESET = 1;
	asm(" RPT # 10 || NOP"); //�ȴ�12�����ڣ���λadcģ��
	AdcRegs.ADCTRL1.bit.RESET = 0;

	//AdcRegs.ADCTRL1 = 0x3000;
	AdcRegs.ADCTRL1.bit.SUSMOD = 3;	//3 �������ʱ�����з�������������ѯ�����߼�����ֹͣ
	AdcRegs.ADCTRL1.bit.ACQ_PS = 0; //����SOC����,�������ڴ�С��SOC����Ϊ1��ADCLK
	AdcRegs.ADCTRL1.bit.CPS = 1;  	//�ں�ʱ��Ԥ��Ƶ,��ʱ��2��Ƶ
									//0  ADCLK = HSPCLK/1
									//1  ADCLK = HSPCLK/2
	AdcRegs.ADCTRL1.bit.CONT_RUN = 0;  //����ֹͣģʽ����EOS�����з�����ֹͣ
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;   //
										// 0 ˫������ģʽ��SEQ1��SEQ2����������8״̬������
										// 1 ��������ģʽ������ģʽ��SEQ��SEQ1��SEQ2����Ϊ1��16״̬���з���������
	//AdcRegs.ADCTRL3.all = 0x00E1
	AdcRegs.ADCTRL3.bit.ADCBGRFDN = 3;

	for(i=0;i<10000;i++) asm(" NOP"); // �ܴ�϶�Ͳο���·�ϵ�
	AdcRegs.ADCTRL3.bit.ADCPWDN=1;
	for(i=0;i<5000;i++) asm(" NOP");  // �ں��ڵ�ģ���·�ϵ�
	AdcRegs.ADCTRL3.bit.ADCCLKPS = 1;  //����ʱ�ӷ�Ƶ��,ADCLK = HSPCLK/��ADCTRL1[7](����CPS)+1��
									   //Ҫ�ǲ�����0  ADCCLK=HSPCLK/[2x(ADCCLKPS)x��ADCTRL1[7]+1��]
	AdcRegs.ADCTRL3.bit.SMODE_SEL = 0;  	//1 ͬ������ģʽ
										    //0 ˳�����ģʽ
	DELAY_US(ADC_usDELAY);         // Delay before converting ADC channels

	//ϵͳʱ��/(���������Ƶ+1)/(ADC��Ƶ+1)/��AD����Ƶ��ѡ��+1��  �͵õ��˲���ʱ��Ƶ�ʣ�
	//Ȼ�����S/Hʱ��  ��͵õ���ת�����ڣ���ס�ת�����ڸ��������ڲ�һ��
	//����ҵ�ADCLK = HSPCLK/2(1+1) = 75M/4 = 18.75Mhz  (TODO)

	//AdcRegs.MAX_CONV.bit.MAX_CONV=0x03;  //4·������������8��ͨ��
	//TODO By Bigrocks  //0033����0003
	AdcRegs.ADCMAXCONV.all = 0x0003;	//4·��������

	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;//����ADCINA0
	AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x1;//����ADCINA1
	AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x2;//����ADCINA2
	AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 0x3;//����ADCINA3

    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;//���SEQ1�е��жϱ�־λ INT_SEQ1

	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 0;
	AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 0;
	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;//ʹ��SEQ1���ж�����
	AdcRegs.ADCTRL2.bit.INT_MOD_SEQ1 = 0;
	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;//����EVA��������SEQ1/SEQ
	AdcRegs.ADCTRL2.bit.EXT_SOC_SEQ1 = 0;

	AdcRegs.ADCST.bit.INT_SEQ2_CLR = 1;//���SEQ2�е��жϱ�־λ INT_SEQ2

	AdcRegs.ADCTRL2.bit.RST_SEQ2 = 0;
	AdcRegs.ADCTRL2.bit.SOC_SEQ2 = 0;
	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ2 = 0;
	AdcRegs.ADCTRL2.bit.INT_MOD_SEQ2 = 0;
	AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ2 = 0;

	//ʹ��ADC�ж�
	//PieCtrl.PIEIER1.bit.INTx6 = 1;
	//IER |= M_INT1;

	// ����ePWM1����ADC
	// 	Assumes ePWM1 clock is already enabled in InitSysCtrl();
	EPwm1Regs.ETSEL.bit.SOCAEN = 1;        // Enable SOC on A group  ʹ��ePWMa����ADת��
	EPwm1Regs.ETSEL.bit.SOCASEL = 4;       // Select SOC from from CPMA on upcount  ѡ��ǰ����ֵ������CMPA��Ϊ�����¼�
	EPwm1Regs.ETPS.bit.SOCAPRD = 1;        // Generate pulse on 1st event  �ڵ�һ�������¼��󼴲�����������
	EPwm1Regs.TBCTL.bit.CTRMODE = 0;		  // count up and start  ������ģʽ
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = 1;//1;     //HSPCLKDIV ѡ��2��Ƶ
	EPwm1Regs.TBCTL.bit.CLKDIV = 0;//0;			//CLKDIV ѡ�񲻷�Ƶ

	EPwm1Regs.TBPRD = 49999;              // Set period for ePWM1  ��������ֵ
	 	 	 	 	 	 	 	 	 	 //����Ƶ��Ϊ 150M/1*2/(TBPRD+1)=1500hz

	EPwm1Regs.CMPA.half.CMPA = EPwm1Regs.TBPRD/2; // Set compare A value  ���ñȽ�ֵ
/*//ԭͬ���������÷�ʽ
	AdcRegs.ADCTRL1.bit.RESET = 1;
	asm(" RPT # 10 || NOP"); //�ȴ�12�����ڣ���λadcģ��
	AdcRegs.ADCTRL1.bit.RESET = 0;

	//AdcRegs.ADCTRL1 = 0x3000;
	AdcRegs.ADCTRL1.bit.SUSMOD = 3;	//3 �������ʱ�����з�������������ѯ�����߼�����ֹͣ
	AdcRegs.ADCTRL1.bit.ACQ_PS = 0; //����SOC����,�������ڴ�С��SOC����Ϊ1��ADCLK
	AdcRegs.ADCTRL1.bit.CPS = 1;  	//�ں�ʱ��Ԥ��Ƶ,��ʱ��2��Ƶ
									//0  ADCLK = HSPCLK/1
									//1  ADCLK = HSPCLK/2
	AdcRegs.ADCTRL1.bit.CONT_RUN = 0;  //����ֹͣģʽ����EOS�����з�����ֹͣ
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;   //
										// 0 ˫������ģʽ��SEQ1��SEQ2����������8״̬������
										// 1 ��������ģʽ������ģʽ��SEQ��SEQ1��SEQ2����Ϊ1��16״̬���з���������
	//AdcRegs.ADCTRL3.all = 0x00E1
	AdcRegs.ADCTRL3.bit.ADCBGRFDN = 3;

	for(i=0;i<10000;i++) asm(" NOP"); // �ܴ�϶�Ͳο���·�ϵ�
	AdcRegs.ADCTRL3.bit.ADCPWDN=1;
	for(i=0;i<5000;i++) asm(" NOP");  // �ں��ڵ�ģ���·�ϵ�
	AdcRegs.ADCTRL3.bit.ADCCLKPS = 1;  //����ʱ�ӷ�Ƶ��,ADCLK = HSPCLK/��ADCTRL1[7](����CPS)+1��
									   //Ҫ�ǲ�����0  ADCCLK=HSPCLK/[2x(ADCCLKPS)x��ADCTRL1[7]+1��]
	AdcRegs.ADCTRL3.bit.SMODE_SEL = 1;  	//1 ͬ������ģʽ
										    //0 ˳�����ģʽ
	DELAY_US(ADC_usDELAY);         // Delay before converting ADC channels

	//ϵͳʱ��/(���������Ƶ+1)/(ADC��Ƶ+1)/��AD����Ƶ��ѡ��+1��  �͵õ��˲���ʱ��Ƶ�ʣ�
	//Ȼ�����S/Hʱ��  ��͵õ���ת�����ڣ���ס�ת�����ڸ��������ڲ�һ��
	//����ҵ�ADCLK = HSPCLK/2(1+1) = 75M/4 = 18.75Mhz  (TODO)

	//AdcRegs.MAX_CONV.bit.MAX_CONV=0x03;  //4·������������8��ͨ��
	//TODO By Bigrocks  //0033����0003
	AdcRegs.ADCMAXCONV.all = 0x0003;	//4·������������8��ͨ��

	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;//����ADCINA0��ADCINB0
	AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x1;//����ADCINA1��ADCINB1
	AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x2;//����ADCINA2��ADCINB2
	AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 0x3;//����ADCINA3��ADCINB3

    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;//���SEQ1�е��жϱ�־λ INT_SEQ1

	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 0;
	AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 0;
	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;//ʹ��SEQ1���ж�����
	AdcRegs.ADCTRL2.bit.INT_MOD_SEQ1 = 0;
	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;//����EVA��������SEQ1/SEQ
	AdcRegs.ADCTRL2.bit.EXT_SOC_SEQ1 = 0;

	AdcRegs.ADCST.bit.INT_SEQ2_CLR = 1;//���SEQ2�е��жϱ�־λ INT_SEQ2

	AdcRegs.ADCTRL2.bit.RST_SEQ2 = 0;
	AdcRegs.ADCTRL2.bit.SOC_SEQ2 = 0;
	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ2 = 0;
	AdcRegs.ADCTRL2.bit.INT_MOD_SEQ2 = 0;
	AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ2 = 0;

	//ʹ��ADC�ж�
	//PieCtrl.PIEIER1.bit.INTx6 = 1;
	//IER |= M_INT1;

	// 	Assumes ePWM1 clock is already enabled in InitSysCtrl();
	EPwm1Regs.ETSEL.bit.SOCAEN = 1;        // Enable SOC on A group  ʹ��ePWMa����ADת��
	EPwm1Regs.ETSEL.bit.SOCASEL = 4;       // Select SOC from from CPMA on upcount  ѡ��ǰ����ֵ������CMPA��Ϊ�����¼�
	EPwm1Regs.ETPS.bit.SOCAPRD = 1;        // Generate pulse on 1st event  �ڵ�һ�������¼��󼴲�����������
	EPwm1Regs.TBCTL.bit.CTRMODE = 0;		  // count up and start  ������ģʽ
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = 1;//1;     //HSPCLKDIV ѡ��2��Ƶ
	EPwm1Regs.TBCTL.bit.CLKDIV = 0;//0;			//CLKDIV ѡ�񲻷�Ƶ

	EPwm1Regs.TBPRD = 49999;              // Set period for ePWM1  ��������ֵ
	 	 	 	 	 	 	 	 	 	 //����Ƶ��Ϊ 150M/1*2/(TBPRD+1)=1500hz
    //����Ƶ��  128��FFT��
	EPwm1Regs.CMPA.half.CMPA = EPwm1Regs.TBPRD/2; // Set compare A value  ���ñȽ�ֵ
*/


	//����TBCLK�����û�����þ���Ĭ�ϣ�����CLKDIV = 0��HSPCLKDIV = 1��TBCLK = SYSCLKOUT / (HSPCLKDIV �� CLKDIV)  ������ʱ�Ӷ���Ƶ��
	//����ADCSOC����PWM1�����ϼ���ģʽ��������COMPA��ֵʱ������������Ƶ�ʾ���PWM�����ڣ�
	//��������������ʱ��150MHZ����EPWM��TBCLK����75M��TBPRDΪ65535���ǲ���Ƶ�ʾ���1/(75*1000000/65535)��
}

/*
 * ************************MY_HMI***************************
 */
void MY_HMI_Handle()
{
	qsize  size = 0;
	Uint32 timer_tick_last_update = 0;//TODO Ҫ�ĵ�

	size = queue_find_cmd(cmd_buffer,CMD_MAX_SIZE); //�ӻ������л�ȡһ��ָ��
	if(size>0)//���յ�ָ��
	{
		ProcessMessage((PCTRL_MSG)cmd_buffer, size);//ָ���
	}

	/****************************************************************************************************************
	�ر�ע��
	MCU��ҪƵ���򴮿����������ݣ����򴮿������ڲ��������������Ӷ��������ݶ�ʧ(��������С����׼��8K��������4.7K)
	1) һ������£�����MCU�򴮿����������ݵ����ڴ���100ms���Ϳ��Ա������ݶ�ʧ�����⣻
	2) �����Ȼ�����ݶ�ʧ�����⣬���жϴ�������BUSY���ţ�Ϊ��ʱ���ܷ������ݸ���������
	******************************************************************************************************************/

	//TODO: ����û�����
	//�����и���ʱ��ÿ100����ˢ��һ��
	if(update_en&&timer_tick_count-timer_tick_last_update>=TIME_100MS)
	{
		update_en = 0;
		timer_tick_last_update = timer_tick_count;

		UpdateUI();
	}
}

/*!
 *  \brief  ��Ϣ�������̣��˴�һ�㲻��Ҫ����
 *  \param msg ��������Ϣ
 *  \param size ��Ϣ����
 */
void ProcessMessage( PCTRL_MSG msg, Uint16 size )
{
	Uint8 cmd_type = msg->cmd_type;//ָ������
	Uint8 ctrl_msg = msg->ctrl_msg;   //��Ϣ������
	Uint8 control_type = msg->control_type;//�ؼ�����
	Uint16 screen_id = PTR2U16(&msg->screen_id);//����ID
	Uint16 control_id = PTR2U16(&msg->control_id);//�ؼ�ID
	Uint32 value = PTR2U32(msg->param);//��ֵ

	switch(cmd_type)
	{
	case NOTIFY_SCREEN://�л�����
		//NotifyScreen(screen_id);
		break;
	case NOTIFY_TOUCH_PRESS:
	case NOTIFY_TOUCH_RELEASE:
		NotifyTouchXY(cmd_buffer[1],PTR2U16(cmd_buffer+2),PTR2U16(cmd_buffer+4));
		break;
	case NOTIFY_WRITE_FLASH_OK:
		NotifyWriteFlash(1);
		break;
	case NOTIFY_WRITE_FLASH_FAILD:
		NotifyWriteFlash(0);
		break;
	case NOTIFY_READ_FLASH_OK:
		NotifyReadFlash(1,cmd_buffer+2,size-6);//ȥ��֡ͷ֡β
		break;
	case NOTIFY_READ_FLASH_FAILD:
		NotifyReadFlash(0,0,0);
		break;
	case NOTIFY_CONTROL:
		{
			switch(ctrl_msg)
			{
			case MsgChangeScreen://�л�����
				NotifyScreen(screen_id);
				break;
			case MsgReadState://��ȡ�ؼ�ֵ
				ReadText(cmd_buffer[4],cmd_buffer[6]);
				break;
			default:
				break;
			}
			switch(control_type)
			{

			case kCtrlButton: //��ť�ؼ�
				NotifyButton(screen_id,control_id,msg->param[1]);
				break;
			case kCtrlText://�ı��ؼ�
				NotifyText(screen_id,control_id,msg->param);
				break;
			case kCtrlProgress: //�������ؼ�
				NotifyProgress(screen_id,control_id,value);
				break;
			case kCtrlSlider: //�������ؼ�
				NotifySlider(screen_id,control_id,value);
				break;
			case kCtrlMeter: //�Ǳ�ؼ�
				NotifyMeter(screen_id,control_id,value);
				break;
			case kCtrlMenu://�˵��ؼ�
				NotifyMenu(screen_id,control_id,msg->param[0],msg->param[1]);
				break;
			case kCtrlSelector://ѡ��ؼ�
				NotifySelector(screen_id,control_id,msg->param[0]);
				break;
			case kCtrlRTC://����ʱ�ؼ�
				NotifyTimer(screen_id,control_id);
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
}
/*!
 *  \brief  �����л�֪ͨ
 *  \details  ��ǰ����ı�ʱ(�����GetScreen)��ִ�д˺���
 *  \param screen_id ��ǰ����ID
 */
void NotifyScreen(Uint16 screen_id)
{
	//TODO: ����û�����
	Uint8 i=0;
	if(screen_id == 0x02)//�ɼ�ҳ���1ҳ
	{
		/*(Uint16 screen_id, Uint16 control_id,int32 value)*/
		SetTextValueFloat1(0x02,0x03,2001);
		SetTextValueFloat1(0x02,0x04,205);
		SetTextValueFloat1(0x02,0x05,1);
		SetTextValueFloat1(0x02,0x06,45);
		SetTextValueFloat1(0x02,0x07,2301);
		SetTextValueFloat1(0x02,0x08,1205);
		SetTextValueFloat1(0x02,0x09,15);
		SetTextValueFloat1(0x02,0x0a,9);

	}
	else if(screen_id == 0x03)//�ɼ�ҳ���2ҳ
	{
		SetTextValueFloat2(0x03,0x03,100);
		SetTextValueFloat1(0x03,0x04,90009);
		SetTextValueFloat1(0x03,0x05,9999);
		SetTextValueFloat2(0x03,0x06,92);
		SetTextValueFloat1(0x03,0x07,59999);
		SetTextValueFloat1(0x03,0x08,90);
	}
	else if(screen_id == 0x04)//�ɼ�ҳ���3ҳ
	{
		for(i=0;i<32;i++)
		{
			SetTextValueFloat1(0x04,i+0x03,10);
		}

	}
	else if(screen_id == 0x05)//���й���ҳ
	{

	}
	else if(screen_id == 0x06)//������Ϣҳ
	{
		if(VSysErroStateVar.OverVoltage == 0x55)//��ѹ
		{
			SetControlVisiable(0x06,0x02,Enable);//����ʾ
			SetControlVisiable(0x06,0x03,Disable);//������
		}
		else if(VSysErroStateVar.OverVoltage == 0x33)//����ѹ
				{
					SetControlVisiable(0x06,0x02,Disable);//������
					SetControlVisiable(0x06,0x03,Enable);	//��	��ʾ
				}
		if(VSysErroStateVar.OverCurrent == 0x55)//����
				{
					SetControlVisiable(0x06,0x04,Enable);//����ʾ
					SetControlVisiable(0x06,0x05,Disable);//������
				}
				else if(VSysErroStateVar.OverCurrent == 0x33)//������
						{
							SetControlVisiable(0x06,0x04,Disable);//������
							SetControlVisiable(0x06,0x05,Enable);	//��	��ʾ
						}
		if(VSysErroStateVar.OverTemp == 0x55)//����
				{
					SetControlVisiable(0x06,0x06,Enable);//����ʾ
					SetControlVisiable(0x06,0x07,Disable);//������
				}
				else if(VSysErroStateVar.OverTemp == 0x33)//������
						{
							SetControlVisiable(0x06,0x06,Disable);//������
							SetControlVisiable(0x06,0x07,Enable);	//��	��ʾ
						}
		if(VSysErroStateVar.UnderVoltage == 0x55)//Ƿѹ
				{
					SetControlVisiable(0x06,0x08,Enable);//����ʾ
					SetControlVisiable(0x06,0x09,Disable);//������
				}
				else if(VSysErroStateVar.UnderVoltage == 0x33)//��Ƿѹ
						{
							SetControlVisiable(0x06,0x08,Disable);//������
							SetControlVisiable(0x06,0x09,Enable);	//��	��ʾ
						}
		if(VSysErroStateVar.RelayErro == 0x55)//���ع���
				{
					SetControlVisiable(0x06,0x0A,Enable);//����ʾ
					SetControlVisiable(0x06,0x0B,Disable);//������
				}
				else if(VSysErroStateVar.RelayErro == 0x33)//�����޹���
						{
							SetControlVisiable(0x06,0x0A,Disable);//������
							SetControlVisiable(0x06,0x0B,Enable);	//��	��ʾ
						}
	}

}

/*!
 *  \brief  ���������¼���Ӧ
 *  \param press 1���´�������3�ɿ�������
 *  \param x x����
 *  \param y y����
 */
void NotifyTouchXY(Uint8 press,Uint16 x,Uint16 y)
{
	//TODO: ����û�����
}

//�ı��ؼ���ʾ1λС��ֵ
void SetTextValueFloat1(Uint16 screen_id, Uint16 control_id,int32 value)
{
	char buffer[12] = {0};
	if(value < 99999)//��ֵС��9999.9 һλС����ΧΪ9999.9
	{
		buffer[0] = (value/10000)%10+0x30;
		if(buffer[0] == 0x30)  //ǧλΪ0
		{
			buffer[0] = (value/1000)%10+0x30;//��λǰ��һλ
			if(buffer[0] == 0x30) //��λΪ0
			{
				buffer[0] = (value/100)%10+0x30;//ʮλǰ��һλ
				if(buffer[0] == 0x30) //ʮλΪ0
				{
					buffer[0] = (value/10)%10+0x30;//��λǰ��1λ
					buffer[1] = 0x2E;
					buffer[2] = value%10+0x30;
				}
				else
				{
					buffer[1] = (value/10)%10+0x30;
					buffer[2] = 0x2E;
					buffer[3] = value%10+0x30;
				}
			}
			else
			{
				buffer[1] = (value/100)%10+0x30;
				buffer[2] = (value/10)%10+0x30;
				buffer[3] = 0x2E;
				buffer[4] = value%10+0x30;
			}
		}
		else
		{
			buffer[1] = (value/1000)%10+0x30;
			buffer[2] = (value/100)%10+0x30;
			buffer[3] = (value/10)%10+0x30;
			buffer[4] = 0x2E;
			buffer[5] = value%10+0x30;
		}
	}
	else
	{
		buffer[0] = 0x30;
		buffer[1] = 0x38;
		buffer[2] = 0x38;
		buffer[3] = 0x38;
		buffer[4] = 0x2E;
		buffer[5] = 0x38;
	}
	SetTextValue(screen_id,control_id,(char *)buffer);
}
//�ı��ؼ���ʾ2λС��ֵ
void SetTextValueFloat2(Uint16 screen_id, Uint16 control_id,int32 value)
{
	char buffer[12] = {0};
	if(value < 99999)//��ֵС��999.99����λС����ΧΪ999.99
	{
		buffer[0] = (value/10000)%10+0x30;
		if(buffer[0] == 0x30)  //��λΪ0
		{
			buffer[0] = (value/1000)%10+0x30;//ʮλǰ��һλ
			if(buffer[0] == 0x30) //ʮλΪ0
			{
				buffer[0] = (value/100)%10+0x30;//��λǰ��һλ
				buffer[1] = 0x2E;
				buffer[2] = (value/10)%10+0x30;
				buffer[3] = value%10+0x30;
			}
			else
			{
				buffer[1] = (value/100)%10+0x30;
				buffer[2] = 0x2E;
				buffer[3] = (value/10)%10+0x30;
				buffer[4] = value%10+0x30;
			}
		}
		else
		{
			buffer[1] = (value/1000)%10+0x30;
			buffer[2] = (value/100)%10+0x30;
			buffer[3] = 0x2E;
			buffer[4] = (value/10)%10+0x30;
			buffer[5] = value%10+0x30;
		}

	}
	else
	{
		buffer[0] = 0x30;
		buffer[1] = 0x30;
		buffer[2] = 0x38;
		buffer[3] = 0x2E;
		buffer[4] = 0x38;
		buffer[5] = 0x38;
	}
	SetTextValue(screen_id,control_id,(char *)buffer);
}
/*!
 *  \brief  ��ȡ�ı��ؼ�����
 *  \details  ���ı�ͨ�����̸���(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 */
void ReadText(Uint16 screen_id, Uint16 control_id)
{
	if((screen_id == 0x07)&&(control_id == 0x07))//����ҳ��Ļ����ؼ�
	{
		if((cmd_buffer[1]==0xb1)&&(cmd_buffer[2]==0x11)&&(cmd_buffer[4]==0x07)
				&&(cmd_buffer[6]==0x07)&&(cmd_buffer[7]==0x13))
		{
			if(cmd_buffer[11]==0x01)//MANNUL �ֶ�����
			{
				SetParaVar.Control_Mode = Mode_MANNUL;
			}
			else if(cmd_buffer[11]==0x00)//AUTO  �Զ�����
			{
				SetParaVar.Control_Mode = Mode_AUTO;
			}
		}
	}
	if((screen_id == 0x07)&&(control_id == 0x03))//����ҳ���ID�ı���
	{
		if((cmd_buffer[1]==0xb1)&&(cmd_buffer[2]==0x11)&&(cmd_buffer[4]==0x07)
				&&(cmd_buffer[6]==0x03)&&(cmd_buffer[7]==0x11))
		{
			if((cmd_buffer[10]==0x00)&&(cmd_buffer[11]==0xff)) //ID����λ��
				{
				SetParaVar.My_ID = (cmd_buffer[8]-0x30)*10+(cmd_buffer[9]-0x30);
				}
			else if((cmd_buffer[9]==0x00)&&(cmd_buffer[10]==0xff)) //ID��1λ��
				{
				SetParaVar.My_ID = cmd_buffer[8]-0x30;
				}
		}

	}
	if((screen_id == 0x07)&&(control_id == 0x04))//����ҳ���CT�ı���
	{
		if((cmd_buffer[9]==0x00)&&(cmd_buffer[10]==0xff)) //CT��1λ��
		{
		SetParaVar.CT = cmd_buffer[8]-0x30;
		}
		else if((cmd_buffer[10]==0x00)&&(cmd_buffer[11]==0xff)) //CT��2λ��
				{
				SetParaVar.CT = (cmd_buffer[8]-0x30)*10+(cmd_buffer[9]-0x30);
				}
		else if((cmd_buffer[11]==0x00)&&(cmd_buffer[12]==0xff)) //CT��3λ��
				{
				SetParaVar.CT = (cmd_buffer[8]-0x30)*100+(cmd_buffer[9]-0x30)*10+(cmd_buffer[10]-0x30);
				}
		else if((cmd_buffer[12]==0x00)&&(cmd_buffer[13]==0xff)) //CT��4λ��
				{
				SetParaVar.CT = (cmd_buffer[8]-0x30)*1000+(cmd_buffer[9]-0x30)*100+(cmd_buffer[10]-0x30)*10+(cmd_buffer[11]-0x30);
				}

	}
	if((screen_id == 0x07)&&(control_id == 0x05))//����ҳ�����ʱʱ���ı���
	{
		if((cmd_buffer[9]==0x00)&&(cmd_buffer[10]==0xff)) //�ж���ʱ��1λ��
			{
			SetParaVar.Judg_Delay = cmd_buffer[8]-0x30;
			}
			else if((cmd_buffer[10]==0x00)&&(cmd_buffer[11]==0xff)) //�ж���ʱ��2λ��
					{
					SetParaVar.Judg_Delay = (cmd_buffer[8]-0x30)*10+(cmd_buffer[9]-0x30);
					}
			else if((cmd_buffer[11]==0x00)&&(cmd_buffer[12]==0xff)) //�ж���ʱ��3λ��
					{
					SetParaVar.Judg_Delay = (cmd_buffer[8]-0x30)*100+(cmd_buffer[9]-0x30)*10+(cmd_buffer[10]-0x30);
					}
	}
	if((screen_id == 0x07)&&(control_id == 0x06))//����ҳ������������ı���
	{
		if((cmd_buffer[11]==0x00)&&(cmd_buffer[12]==0xff)) //����������1λ��---- ��������*10����2λ��
			{
			SetParaVar.KVA_Amount = (cmd_buffer[8]-0x30)*10+(cmd_buffer[10]-0x30); //cmd_buffer[9]��С����
			}
			else if((cmd_buffer[12]==0x00)&&(cmd_buffer[13]==0xff)) //����������2λ��---- ��������*10����3λ��
					{
					SetParaVar.KVA_Amount = (cmd_buffer[8]-0x30)*100+(cmd_buffer[9]-0x30)*10+(cmd_buffer[11]-0x30);//cmd_buffer[10]��С����
					}
			else if((cmd_buffer[13]==0x00)&&(cmd_buffer[14]==0xff)) //����������3λ��---- ��������*10����4λ��
					{
					SetParaVar.KVA_Amount = (cmd_buffer[8]-0x30)*1000+(cmd_buffer[9]-0x30)*100+(cmd_buffer[10]-0x30)*10+(cmd_buffer[12]-0x30);//cmd_buffer[11]��С����
					}
	}
}
//�ı��ؼ���ʾ����ֵ
void SetTextValueInt32(Uint16 screen_id, Uint16 control_id,int32 value)
{
	char buffer[12] = {0};
	sprintf(buffer,"%ld",value);
	SetTextValue(screen_id,control_id,(char *)buffer);
}

//�ַ���ת����
int32 StringToInt32(Uint8 *str)
{
	int32 v = 0;
	sscanf((char *)str,"%ld",&v);
	return v;
}

//���½���ؼ���ʾ
void UpdateUI()
{
	SetTextValueInt32(0,2,test_value);
	SetProgressValue(0,3,test_value);
	SetSliderValue(0,4,test_value);
	SetMeterValue(0,7,test_value);
}

/*!
 *  \brief  ��ť�ؼ�֪ͨ
 *  \details  ����ť״̬�ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param state ��ť״̬��0����1����
 */
void NotifyButton(Uint16 screen_id, Uint16 control_id, Uint8  state)
{
	//TODO: ����û�����
	if(screen_id==0x01&&control_id==0x01)//��ҳ��Ĳɼ����ݰ�ť��ID=1��
	{

	}
	else if(screen_id==0&&control_id==6)//�ݼ���ť
	{
		if(test_value>0)
			--test_value;
	}


	//UpdateUI();
	update_en = 1;
}

/*!
 *  \brief  �ı��ؼ�֪ͨ
 *  \details  ���ı�ͨ�����̸���(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param str �ı��ؼ�����
 */
void NotifyText(Uint16 screen_id, Uint16 control_id, Uint8 *str)
{
	//TODO: ����û�����
	test_value = StringToInt32(str);

	if(test_value>100)
		test_value = 100;
	else if(test_value<0)
		test_value = 0;

	//UpdateUI();
	update_en = 1;
}

/*!
 *  \brief  �������ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyProgress(Uint16 screen_id, Uint16 control_id, Uint32 value)
{
	//TODO: ����û�����
}

/*!
 *  \brief  �������ؼ�֪ͨ
 *  \details  ���������ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifySlider(Uint16 screen_id, Uint16 control_id, Uint32 value)
{
	//TODO: ����û�����
	test_value = value;

	//UpdateUI();
	update_en = 1;
}

/*!
 *  \brief  �Ǳ�ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyMeter(Uint16 screen_id, Uint16 control_id, Uint32 value)
{
	//TODO: ����û�����
}

/*!
 *  \brief  �˵��ؼ�֪ͨ
 *  \details  ���˵���»��ɿ�ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item �˵�������
 *  \param state ��ť״̬��0�ɿ���1����
 */
void NotifyMenu(Uint16 screen_id, Uint16 control_id, Uint8  item, Uint8  state)
{
	//TODO: ����û�����
}

/*!
 *  \brief  ѡ��ؼ�֪ͨ
 *  \details  ��ѡ��ؼ��仯ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item ��ǰѡ��
 */
void NotifySelector(Uint16 screen_id, Uint16 control_id, Uint8  item)
{
	//TODO: ����û�����
}

/*!
 *  \brief  ��ʱ����ʱ֪ͨ����
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 */
void NotifyTimer(Uint16 screen_id, Uint16 control_id)
{
	//TODO: ����û�����
}

/*!
 *  \brief  ��ȡ�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 *  \param _data ��������
 *  \param length ���ݳ���
 */
void NotifyReadFlash(Uint8 status,Uint8 *_data,Uint16 length)
{
	//TODO: ����û�����
}

/*!
 *  \brief  д�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 */
void NotifyWriteFlash(Uint8 status)
{
	//TODO: ����û�����
}
