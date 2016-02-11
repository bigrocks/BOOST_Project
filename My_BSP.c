#include "DSP28x_Project.h"
#include "My_BSP.h"
#include "HMICMD_Queue.h"
#include "stdio.h"
#include "HMI_Driver.h"
#include "My_VarDefined.h"

#define ADC_usDELAY  5000L
#define TIME_100MS 10

static int32 test_value = 0;//TODO 要改的
static Uint8 update_en = 0;
volatile Uint32  timer_tick_count = 0; //定时器节拍

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
	GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;   // GPIO2 = output Relay 2 驱动电源

	GpioCtrlRegs.GPAPUD.bit.GPIO2 = 0;   // Enable pullup on GPIO4
	GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;  // GPIO4 = GPIO4
	GpioDataRegs.GPASET.bit.GPIO2 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;   // GPIO4 = output Relay 3 交流检测电源

	GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;   // Enable pullup on GPIO6
	GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;  // GPIO6 = GPIO6
	GpioDataRegs.GPASET.bit.GPIO4 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;   // GPIO6 = output Relay 4 接触器1

	GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;   // Enable pullup on GPIO1
	GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;  // GPIO1 = GPIO1
	GpioDataRegs.GPASET.bit.GPIO5 = 1;
	GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;   // GPIO1 = output Relay 5 接触器2

	EDIS;
}
/*
 * SCI模块
 */
void SciB485_Init()
{
	EALLOW;

	// 485 RD
	GpioCtrlRegs.GPAPUD.bit.GPIO22 = 1;   // 禁止内部上拉，因为外部加了下拉
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
	ScibRegs.SCICTL2.bit.TXINTENA =0;//禁止发送中断
	ScibRegs.SCICTL2.bit.RXBKINTENA =1;//使能接收终端
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
 * RS232接口
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
	ScicRegs.SCICTL2.bit.TXINTENA =0;//禁止发送中断
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

	//ScicRegs.SCICCR.bit.PARITYENA = 1;	//Enable PARITY  没有校验
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
	ScibRegs.SCICTL2.bit.TXINTENA =0;//禁止发送中断
	ScibRegs.SCICTL2.bit.RXBKINTENA =1;//使能接收终端
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

//-------SCI接收中断--------
interrupt void MySciB485Rx_IsrB(void)
{
	while(ScibRegs.SCIRXST.bit.RXRDY !=1) { }
	Uint8 ReceivedChar = ScibRegs.SCIRXBUF.all;


	ScibRegs.SCIFFRX.bit.RXFFOVRCLR=1;	//Clear Overflow flag
	ScibRegs.SCIFFRX.bit.RXFFINTCLR=1;	//Clear Interrupt flag
	PieCtrlRegs.PIEACK.all|=0x100;		//Issue PIE ack
}

//-------SCI接收中断--------
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
	asm(" RPT # 10 || NOP"); //等待12个周期，复位adc模块
	AdcRegs.ADCTRL1.bit.RESET = 0;

	//AdcRegs.ADCTRL1 = 0x3000;
	AdcRegs.ADCTRL1.bit.SUSMOD = 3;	//3 仿真挂起时，序列发生器和其他轮询程序逻辑立即停止
	AdcRegs.ADCTRL1.bit.ACQ_PS = 0; //控制SOC脉宽,采样窗口大小，SOC脉宽为1个ADCLK
	AdcRegs.ADCTRL1.bit.CPS = 1;  	//内核时钟预分频,将时钟2分频
									//0  ADCLK = HSPCLK/1
									//1  ADCLK = HSPCLK/2
	AdcRegs.ADCTRL1.bit.CONT_RUN = 0;  //启动停止模式到达EOS后序列发生器停止
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;   //
										// 0 双排序器模式，SEQ1和SEQ2是用作两个8状态排序器
										// 1 级联运行模式，级联模式，SEQ（SEQ1和SEQ2）作为1个16状态序列发生器工作
	//AdcRegs.ADCTRL3.all = 0x00E1
	AdcRegs.ADCTRL3.bit.ADCBGRFDN = 3;

	for(i=0;i<10000;i++) asm(" NOP"); // 能带隙和参考电路上电
	AdcRegs.ADCTRL3.bit.ADCPWDN=1;
	for(i=0;i<5000;i++) asm(" NOP");  // 内核内的模拟电路上电
	AdcRegs.ADCTRL3.bit.ADCCLKPS = 1;  //核心时钟分频器,ADCLK = HSPCLK/（ADCTRL1[7](就是CPS)+1）
									   //要是不等于0  ADCCLK=HSPCLK/[2x(ADCCLKPS)x（ADCTRL1[7]+1）]
	AdcRegs.ADCTRL3.bit.SMODE_SEL = 0;  	//1 同步采样模式
										    //0 顺序采样模式
	DELAY_US(ADC_usDELAY);         // Delay before converting ADC channels

	//系统时钟/(高速外设分频+1)/(ADC分频+1)/（AD本身频率选择+1）  就得到了采样时钟频率，
	//然后加上S/H时间  你就得到了转换周期，记住额，转换周期跟采样周期不一样
	//因此我的ADCLK = HSPCLK/2(1+1) = 75M/4 = 18.75Mhz  (TODO)

	//AdcRegs.MAX_CONV.bit.MAX_CONV=0x03;  //4路并发采样，共8个通道
	//TODO By Bigrocks  //0033换成0003
	AdcRegs.ADCMAXCONV.all = 0x0003;	//4路并发采样

	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;//采样ADCINA0
	AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x1;//采样ADCINA1
	AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x2;//采样ADCINA2
	AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 0x3;//采样ADCINA3

    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;//清除SEQ1中的中断标志位 INT_SEQ1

	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 0;
	AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 0;
	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;//使能SEQ1的中断请求
	AdcRegs.ADCTRL2.bit.INT_MOD_SEQ1 = 0;
	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;//允许EVA启动触发SEQ1/SEQ
	AdcRegs.ADCTRL2.bit.EXT_SOC_SEQ1 = 0;

	AdcRegs.ADCST.bit.INT_SEQ2_CLR = 1;//清除SEQ2中的中断标志位 INT_SEQ2

	AdcRegs.ADCTRL2.bit.RST_SEQ2 = 0;
	AdcRegs.ADCTRL2.bit.SOC_SEQ2 = 0;
	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ2 = 0;
	AdcRegs.ADCTRL2.bit.INT_MOD_SEQ2 = 0;
	AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ2 = 0;

	//使能ADC中断
	//PieCtrl.PIEIER1.bit.INTx6 = 1;
	//IER |= M_INT1;

	// 配置ePWM1触发ADC
	// 	Assumes ePWM1 clock is already enabled in InitSysCtrl();
	EPwm1Regs.ETSEL.bit.SOCAEN = 1;        // Enable SOC on A group  使能ePWMa触发AD转换
	EPwm1Regs.ETSEL.bit.SOCASEL = 4;       // Select SOC from from CPMA on upcount  选择当前计数值递增至CMPA作为触发事件
	EPwm1Regs.ETPS.bit.SOCAPRD = 1;        // Generate pulse on 1st event  在第一个触发事件后即产生触发脉冲
	EPwm1Regs.TBCTL.bit.CTRMODE = 0;		  // count up and start  增计数模式
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = 1;//1;     //HSPCLKDIV 选择2分频
	EPwm1Regs.TBCTL.bit.CLKDIV = 0;//0;			//CLKDIV 选择不分频

	EPwm1Regs.TBPRD = 49999;              // Set period for ePWM1  设置周期值
	 	 	 	 	 	 	 	 	 	 //采样频率为 150M/1*2/(TBPRD+1)=1500hz

	EPwm1Regs.CMPA.half.CMPA = EPwm1Regs.TBPRD/2; // Set compare A value  设置比较值
/*//原同步采样设置方式
	AdcRegs.ADCTRL1.bit.RESET = 1;
	asm(" RPT # 10 || NOP"); //等待12个周期，复位adc模块
	AdcRegs.ADCTRL1.bit.RESET = 0;

	//AdcRegs.ADCTRL1 = 0x3000;
	AdcRegs.ADCTRL1.bit.SUSMOD = 3;	//3 仿真挂起时，序列发生器和其他轮询程序逻辑立即停止
	AdcRegs.ADCTRL1.bit.ACQ_PS = 0; //控制SOC脉宽,采样窗口大小，SOC脉宽为1个ADCLK
	AdcRegs.ADCTRL1.bit.CPS = 1;  	//内核时钟预分频,将时钟2分频
									//0  ADCLK = HSPCLK/1
									//1  ADCLK = HSPCLK/2
	AdcRegs.ADCTRL1.bit.CONT_RUN = 0;  //启动停止模式到达EOS后序列发生器停止
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;   //
										// 0 双排序器模式，SEQ1和SEQ2是用作两个8状态排序器
										// 1 级联运行模式，级联模式，SEQ（SEQ1和SEQ2）作为1个16状态序列发生器工作
	//AdcRegs.ADCTRL3.all = 0x00E1
	AdcRegs.ADCTRL3.bit.ADCBGRFDN = 3;

	for(i=0;i<10000;i++) asm(" NOP"); // 能带隙和参考电路上电
	AdcRegs.ADCTRL3.bit.ADCPWDN=1;
	for(i=0;i<5000;i++) asm(" NOP");  // 内核内的模拟电路上电
	AdcRegs.ADCTRL3.bit.ADCCLKPS = 1;  //核心时钟分频器,ADCLK = HSPCLK/（ADCTRL1[7](就是CPS)+1）
									   //要是不等于0  ADCCLK=HSPCLK/[2x(ADCCLKPS)x（ADCTRL1[7]+1）]
	AdcRegs.ADCTRL3.bit.SMODE_SEL = 1;  	//1 同步采样模式
										    //0 顺序采样模式
	DELAY_US(ADC_usDELAY);         // Delay before converting ADC channels

	//系统时钟/(高速外设分频+1)/(ADC分频+1)/（AD本身频率选择+1）  就得到了采样时钟频率，
	//然后加上S/H时间  你就得到了转换周期，记住额，转换周期跟采样周期不一样
	//因此我的ADCLK = HSPCLK/2(1+1) = 75M/4 = 18.75Mhz  (TODO)

	//AdcRegs.MAX_CONV.bit.MAX_CONV=0x03;  //4路并发采样，共8个通道
	//TODO By Bigrocks  //0033换成0003
	AdcRegs.ADCMAXCONV.all = 0x0003;	//4路并发采样，共8个通道

	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;//采样ADCINA0和ADCINB0
	AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x1;//采样ADCINA1和ADCINB1
	AdcRegs.ADCCHSELSEQ1.bit.CONV02 = 0x2;//采样ADCINA2和ADCINB2
	AdcRegs.ADCCHSELSEQ1.bit.CONV03 = 0x3;//采样ADCINA3和ADCINB3

    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;//清除SEQ1中的中断标志位 INT_SEQ1

	AdcRegs.ADCTRL2.bit.RST_SEQ1 = 0;
	AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 0;
	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;//使能SEQ1的中断请求
	AdcRegs.ADCTRL2.bit.INT_MOD_SEQ1 = 0;
	AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;//允许EVA启动触发SEQ1/SEQ
	AdcRegs.ADCTRL2.bit.EXT_SOC_SEQ1 = 0;

	AdcRegs.ADCST.bit.INT_SEQ2_CLR = 1;//清除SEQ2中的中断标志位 INT_SEQ2

	AdcRegs.ADCTRL2.bit.RST_SEQ2 = 0;
	AdcRegs.ADCTRL2.bit.SOC_SEQ2 = 0;
	AdcRegs.ADCTRL2.bit.INT_ENA_SEQ2 = 0;
	AdcRegs.ADCTRL2.bit.INT_MOD_SEQ2 = 0;
	AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ2 = 0;

	//使能ADC中断
	//PieCtrl.PIEIER1.bit.INTx6 = 1;
	//IER |= M_INT1;

	// 	Assumes ePWM1 clock is already enabled in InitSysCtrl();
	EPwm1Regs.ETSEL.bit.SOCAEN = 1;        // Enable SOC on A group  使能ePWMa触发AD转换
	EPwm1Regs.ETSEL.bit.SOCASEL = 4;       // Select SOC from from CPMA on upcount  选择当前计数值递增至CMPA作为触发事件
	EPwm1Regs.ETPS.bit.SOCAPRD = 1;        // Generate pulse on 1st event  在第一个触发事件后即产生触发脉冲
	EPwm1Regs.TBCTL.bit.CTRMODE = 0;		  // count up and start  增计数模式
	EPwm1Regs.TBCTL.bit.HSPCLKDIV = 1;//1;     //HSPCLKDIV 选择2分频
	EPwm1Regs.TBCTL.bit.CLKDIV = 0;//0;			//CLKDIV 选择不分频

	EPwm1Regs.TBPRD = 49999;              // Set period for ePWM1  设置周期值
	 	 	 	 	 	 	 	 	 	 //采样频率为 150M/1*2/(TBPRD+1)=1500hz
    //采样频率  128点FFT，
	EPwm1Regs.CMPA.half.CMPA = EPwm1Regs.TBPRD/2; // Set compare A value  设置比较值
*/


	//对于TBCLK，如果没有设置就是默认，就是CLKDIV = 0，HSPCLKDIV = 1，TBCLK = SYSCLKOUT / (HSPCLKDIV × CLKDIV)  等于主时钟二分频。
	//所以ADCSOC是在PWM1的向上计数模式，计数至COMPA的值时触发，采样的频率就是PWM的周期，
	//假设采样最高输入时钟150MHZ，那EPWM的TBCLK就是75M，TBPRD为65535，那采样频率就是1/(75*1000000/65535)。
}

/*
 * ************************MY_HMI***************************
 */
void MY_HMI_Handle()
{
	qsize  size = 0;
	Uint32 timer_tick_last_update = 0;//TODO 要改的

	size = queue_find_cmd(cmd_buffer,CMD_MAX_SIZE); //从缓冲区中获取一条指令
	if(size>0)//接收到指令
	{
		ProcessMessage((PCTRL_MSG)cmd_buffer, size);//指令处理
	}

	/****************************************************************************************************************
	特别注意
	MCU不要频繁向串口屏发送数据，否则串口屏的内部缓存区会满，从而导致数据丢失(缓冲区大小：标准型8K，基本型4.7K)
	1) 一般情况下，控制MCU向串口屏发送数据的周期大于100ms，就可以避免数据丢失的问题；
	2) 如果仍然有数据丢失的问题，请判断串口屏的BUSY引脚，为高时不能发送数据给串口屏。
	******************************************************************************************************************/

	//TODO: 添加用户代码
	//数据有更新时，每100毫秒刷新一次
	if(update_en&&timer_tick_count-timer_tick_last_update>=TIME_100MS)
	{
		update_en = 0;
		timer_tick_last_update = timer_tick_count;

		UpdateUI();
	}
}

/*!
 *  \brief  消息处理流程，此处一般不需要更改
 *  \param msg 待处理消息
 *  \param size 消息长度
 */
void ProcessMessage( PCTRL_MSG msg, Uint16 size )
{
	Uint8 cmd_type = msg->cmd_type;//指令类型
	Uint8 ctrl_msg = msg->ctrl_msg;   //消息的类型
	Uint8 control_type = msg->control_type;//控件类型
	Uint16 screen_id = PTR2U16(&msg->screen_id);//画面ID
	Uint16 control_id = PTR2U16(&msg->control_id);//控件ID
	Uint32 value = PTR2U32(msg->param);//数值

	switch(cmd_type)
	{
	case NOTIFY_SCREEN://切换画面
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
		NotifyReadFlash(1,cmd_buffer+2,size-6);//去除帧头帧尾
		break;
	case NOTIFY_READ_FLASH_FAILD:
		NotifyReadFlash(0,0,0);
		break;
	case NOTIFY_CONTROL:
		{
			switch(ctrl_msg)
			{
			case MsgChangeScreen://切换画面
				NotifyScreen(screen_id);
				break;
			case MsgReadState://读取控件值
				ReadText(cmd_buffer[4],cmd_buffer[6]);
				break;
			default:
				break;
			}
			switch(control_type)
			{

			case kCtrlButton: //按钮控件
				NotifyButton(screen_id,control_id,msg->param[1]);
				break;
			case kCtrlText://文本控件
				NotifyText(screen_id,control_id,msg->param);
				break;
			case kCtrlProgress: //进度条控件
				NotifyProgress(screen_id,control_id,value);
				break;
			case kCtrlSlider: //滑动条控件
				NotifySlider(screen_id,control_id,value);
				break;
			case kCtrlMeter: //仪表控件
				NotifyMeter(screen_id,control_id,value);
				break;
			case kCtrlMenu://菜单控件
				NotifyMenu(screen_id,control_id,msg->param[0],msg->param[1]);
				break;
			case kCtrlSelector://选择控件
				NotifySelector(screen_id,control_id,msg->param[0]);
				break;
			case kCtrlRTC://倒计时控件
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
 *  \brief  画面切换通知
 *  \details  当前画面改变时(或调用GetScreen)，执行此函数
 *  \param screen_id 当前画面ID
 */
void NotifyScreen(Uint16 screen_id)
{
	//TODO: 添加用户代码
	Uint8 i=0;
	if(screen_id == 0x02)//采集页面第1页
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
	else if(screen_id == 0x03)//采集页面第2页
	{
		SetTextValueFloat2(0x03,0x03,100);
		SetTextValueFloat1(0x03,0x04,90009);
		SetTextValueFloat1(0x03,0x05,9999);
		SetTextValueFloat2(0x03,0x06,92);
		SetTextValueFloat1(0x03,0x07,59999);
		SetTextValueFloat1(0x03,0x08,90);
	}
	else if(screen_id == 0x04)//采集页面第3页
	{
		for(i=0;i<32;i++)
		{
			SetTextValueFloat1(0x04,i+0x03,10);
		}

	}
	else if(screen_id == 0x05)//运行工况页
	{

	}
	else if(screen_id == 0x06)//保护信息页
	{
		if(VSysErroStateVar.OverVoltage == 0x55)//过压
		{
			SetControlVisiable(0x06,0x02,Enable);//红显示
			SetControlVisiable(0x06,0x03,Disable);//绿隐藏
		}
		else if(VSysErroStateVar.OverVoltage == 0x33)//不过压
				{
					SetControlVisiable(0x06,0x02,Disable);//红隐藏
					SetControlVisiable(0x06,0x03,Enable);	//绿	显示
				}
		if(VSysErroStateVar.OverCurrent == 0x55)//过流
				{
					SetControlVisiable(0x06,0x04,Enable);//红显示
					SetControlVisiable(0x06,0x05,Disable);//绿隐藏
				}
				else if(VSysErroStateVar.OverCurrent == 0x33)//不过流
						{
							SetControlVisiable(0x06,0x04,Disable);//红隐藏
							SetControlVisiable(0x06,0x05,Enable);	//绿	显示
						}
		if(VSysErroStateVar.OverTemp == 0x55)//过温
				{
					SetControlVisiable(0x06,0x06,Enable);//红显示
					SetControlVisiable(0x06,0x07,Disable);//绿隐藏
				}
				else if(VSysErroStateVar.OverTemp == 0x33)//不过温
						{
							SetControlVisiable(0x06,0x06,Disable);//红隐藏
							SetControlVisiable(0x06,0x07,Enable);	//绿	显示
						}
		if(VSysErroStateVar.UnderVoltage == 0x55)//欠压
				{
					SetControlVisiable(0x06,0x08,Enable);//红显示
					SetControlVisiable(0x06,0x09,Disable);//绿隐藏
				}
				else if(VSysErroStateVar.UnderVoltage == 0x33)//不欠压
						{
							SetControlVisiable(0x06,0x08,Disable);//红隐藏
							SetControlVisiable(0x06,0x09,Enable);	//绿	显示
						}
		if(VSysErroStateVar.RelayErro == 0x55)//开关故障
				{
					SetControlVisiable(0x06,0x0A,Enable);//红显示
					SetControlVisiable(0x06,0x0B,Disable);//绿隐藏
				}
				else if(VSysErroStateVar.RelayErro == 0x33)//开关无故障
						{
							SetControlVisiable(0x06,0x0A,Disable);//红隐藏
							SetControlVisiable(0x06,0x0B,Enable);	//绿	显示
						}
	}

}

/*!
 *  \brief  触摸坐标事件响应
 *  \param press 1按下触摸屏，3松开触摸屏
 *  \param x x坐标
 *  \param y y坐标
 */
void NotifyTouchXY(Uint8 press,Uint16 x,Uint16 y)
{
	//TODO: 添加用户代码
}

//文本控件显示1位小数值
void SetTextValueFloat1(Uint16 screen_id, Uint16 control_id,int32 value)
{
	char buffer[12] = {0};
	if(value < 99999)//数值小于9999.9 一位小数范围为9999.9
	{
		buffer[0] = (value/10000)%10+0x30;
		if(buffer[0] == 0x30)  //千位为0
		{
			buffer[0] = (value/1000)%10+0x30;//百位前移一位
			if(buffer[0] == 0x30) //百位为0
			{
				buffer[0] = (value/100)%10+0x30;//十位前移一位
				if(buffer[0] == 0x30) //十位为0
				{
					buffer[0] = (value/10)%10+0x30;//个位前移1位
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
//文本控件显示2位小数值
void SetTextValueFloat2(Uint16 screen_id, Uint16 control_id,int32 value)
{
	char buffer[12] = {0};
	if(value < 99999)//数值小于999.99，两位小数范围为999.99
	{
		buffer[0] = (value/10000)%10+0x30;
		if(buffer[0] == 0x30)  //百位为0
		{
			buffer[0] = (value/1000)%10+0x30;//十位前移一位
			if(buffer[0] == 0x30) //十位为0
			{
				buffer[0] = (value/100)%10+0x30;//个位前移一位
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
 *  \brief  读取文本控件内容
 *  \details  当文本通过键盘更新(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 */
void ReadText(Uint16 screen_id, Uint16 control_id)
{
	if((screen_id == 0x07)&&(control_id == 0x07))//设置页面的滑条控件
	{
		if((cmd_buffer[1]==0xb1)&&(cmd_buffer[2]==0x11)&&(cmd_buffer[4]==0x07)
				&&(cmd_buffer[6]==0x07)&&(cmd_buffer[7]==0x13))
		{
			if(cmd_buffer[11]==0x01)//MANNUL 手动控制
			{
				SetParaVar.Control_Mode = Mode_MANNUL;
			}
			else if(cmd_buffer[11]==0x00)//AUTO  自动控制
			{
				SetParaVar.Control_Mode = Mode_AUTO;
			}
		}
	}
	if((screen_id == 0x07)&&(control_id == 0x03))//设置页面的ID文本框
	{
		if((cmd_buffer[1]==0xb1)&&(cmd_buffer[2]==0x11)&&(cmd_buffer[4]==0x07)
				&&(cmd_buffer[6]==0x03)&&(cmd_buffer[7]==0x11))
		{
			if((cmd_buffer[10]==0x00)&&(cmd_buffer[11]==0xff)) //ID是两位数
				{
				SetParaVar.My_ID = (cmd_buffer[8]-0x30)*10+(cmd_buffer[9]-0x30);
				}
			else if((cmd_buffer[9]==0x00)&&(cmd_buffer[10]==0xff)) //ID是1位数
				{
				SetParaVar.My_ID = cmd_buffer[8]-0x30;
				}
		}

	}
	if((screen_id == 0x07)&&(control_id == 0x04))//设置页面的CT文本框
	{
		if((cmd_buffer[9]==0x00)&&(cmd_buffer[10]==0xff)) //CT是1位数
		{
		SetParaVar.CT = cmd_buffer[8]-0x30;
		}
		else if((cmd_buffer[10]==0x00)&&(cmd_buffer[11]==0xff)) //CT是2位数
				{
				SetParaVar.CT = (cmd_buffer[8]-0x30)*10+(cmd_buffer[9]-0x30);
				}
		else if((cmd_buffer[11]==0x00)&&(cmd_buffer[12]==0xff)) //CT是3位数
				{
				SetParaVar.CT = (cmd_buffer[8]-0x30)*100+(cmd_buffer[9]-0x30)*10+(cmd_buffer[10]-0x30);
				}
		else if((cmd_buffer[12]==0x00)&&(cmd_buffer[13]==0xff)) //CT是4位数
				{
				SetParaVar.CT = (cmd_buffer[8]-0x30)*1000+(cmd_buffer[9]-0x30)*100+(cmd_buffer[10]-0x30)*10+(cmd_buffer[11]-0x30);
				}

	}
	if((screen_id == 0x07)&&(control_id == 0x05))//设置页面的延时时间文本框
	{
		if((cmd_buffer[9]==0x00)&&(cmd_buffer[10]==0xff)) //判断延时是1位数
			{
			SetParaVar.Judg_Delay = cmd_buffer[8]-0x30;
			}
			else if((cmd_buffer[10]==0x00)&&(cmd_buffer[11]==0xff)) //判断延时是2位数
					{
					SetParaVar.Judg_Delay = (cmd_buffer[8]-0x30)*10+(cmd_buffer[9]-0x30);
					}
			else if((cmd_buffer[11]==0x00)&&(cmd_buffer[12]==0xff)) //判断延时是3位数
					{
					SetParaVar.Judg_Delay = (cmd_buffer[8]-0x30)*100+(cmd_buffer[9]-0x30)*10+(cmd_buffer[10]-0x30);
					}
	}
	if((screen_id == 0x07)&&(control_id == 0x06))//设置页面的容量设置文本框
	{
		if((cmd_buffer[11]==0x00)&&(cmd_buffer[12]==0xff)) //容量设置是1位数---- 设置容量*10就是2位数
			{
			SetParaVar.KVA_Amount = (cmd_buffer[8]-0x30)*10+(cmd_buffer[10]-0x30); //cmd_buffer[9]是小数点
			}
			else if((cmd_buffer[12]==0x00)&&(cmd_buffer[13]==0xff)) //容量设置是2位数---- 设置容量*10就是3位数
					{
					SetParaVar.KVA_Amount = (cmd_buffer[8]-0x30)*100+(cmd_buffer[9]-0x30)*10+(cmd_buffer[11]-0x30);//cmd_buffer[10]是小数点
					}
			else if((cmd_buffer[13]==0x00)&&(cmd_buffer[14]==0xff)) //容量设置是3位数---- 设置容量*10就是4位数
					{
					SetParaVar.KVA_Amount = (cmd_buffer[8]-0x30)*1000+(cmd_buffer[9]-0x30)*100+(cmd_buffer[10]-0x30)*10+(cmd_buffer[12]-0x30);//cmd_buffer[11]是小数点
					}
	}
}
//文本控件显示整数值
void SetTextValueInt32(Uint16 screen_id, Uint16 control_id,int32 value)
{
	char buffer[12] = {0};
	sprintf(buffer,"%ld",value);
	SetTextValue(screen_id,control_id,(char *)buffer);
}

//字符串转整数
int32 StringToInt32(Uint8 *str)
{
	int32 v = 0;
	sscanf((char *)str,"%ld",&v);
	return v;
}

//更新界面控件显示
void UpdateUI()
{
	SetTextValueInt32(0,2,test_value);
	SetProgressValue(0,3,test_value);
	SetSliderValue(0,4,test_value);
	SetMeterValue(0,7,test_value);
}

/*!
 *  \brief  按钮控件通知
 *  \details  当按钮状态改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param state 按钮状态：0弹起，1按下
 */
void NotifyButton(Uint16 screen_id, Uint16 control_id, Uint8  state)
{
	//TODO: 添加用户代码
	if(screen_id==0x01&&control_id==0x01)//主页面的采集数据按钮（ID=1）
	{

	}
	else if(screen_id==0&&control_id==6)//递减按钮
	{
		if(test_value>0)
			--test_value;
	}


	//UpdateUI();
	update_en = 1;
}

/*!
 *  \brief  文本控件通知
 *  \details  当文本通过键盘更新(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param str 文本控件内容
 */
void NotifyText(Uint16 screen_id, Uint16 control_id, Uint8 *str)
{
	//TODO: 添加用户代码
	test_value = StringToInt32(str);

	if(test_value>100)
		test_value = 100;
	else if(test_value<0)
		test_value = 0;

	//UpdateUI();
	update_en = 1;
}

/*!
 *  \brief  进度条控件通知
 *  \details  调用GetControlValue时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyProgress(Uint16 screen_id, Uint16 control_id, Uint32 value)
{
	//TODO: 添加用户代码
}

/*!
 *  \brief  滑动条控件通知
 *  \details  当滑动条改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifySlider(Uint16 screen_id, Uint16 control_id, Uint32 value)
{
	//TODO: 添加用户代码
	test_value = value;

	//UpdateUI();
	update_en = 1;
}

/*!
 *  \brief  仪表控件通知
 *  \details  调用GetControlValue时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyMeter(Uint16 screen_id, Uint16 control_id, Uint32 value)
{
	//TODO: 添加用户代码
}

/*!
 *  \brief  菜单控件通知
 *  \details  当菜单项按下或松开时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param item 菜单项索引
 *  \param state 按钮状态：0松开，1按下
 */
void NotifyMenu(Uint16 screen_id, Uint16 control_id, Uint8  item, Uint8  state)
{
	//TODO: 添加用户代码
}

/*!
 *  \brief  选择控件通知
 *  \details  当选择控件变化时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param item 当前选项
 */
void NotifySelector(Uint16 screen_id, Uint16 control_id, Uint8  item)
{
	//TODO: 添加用户代码
}

/*!
 *  \brief  定时器超时通知处理
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 */
void NotifyTimer(Uint16 screen_id, Uint16 control_id)
{
	//TODO: 添加用户代码
}

/*!
 *  \brief  读取用户FLASH状态返回
 *  \param status 0失败，1成功
 *  \param _data 返回数据
 *  \param length 数据长度
 */
void NotifyReadFlash(Uint8 status,Uint8 *_data,Uint16 length)
{
	//TODO: 添加用户代码
}

/*!
 *  \brief  写用户FLASH状态返回
 *  \param status 0失败，1成功
 */
void NotifyWriteFlash(Uint8 status)
{
	//TODO: 添加用户代码
}
