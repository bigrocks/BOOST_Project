/********************************************************************
* 文件名： 	板载资源初始化和定义
* 描述:
**********************************************************************/
/********************************************************************
程序说明：
********************************************************************/
#ifndef __MY_BSP_H__
#define __MY_BSP_H__

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

#include "HMICMD_Queue.h"

#ifdef __cplusplus
extern "C" {
#endif

extern Uint16 ADC_Value[8][256];

/**************************************宏定义************************************************/
#define Timer0_Period   0x80;

#define SCIB485TX_On		GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;//有非门，所以低发送
#define SCIB485RX_On		GpioDataRegs.GPASET.bit.GPIO22 = 1;//有非门，所以高接收

#define LED_1_On		GpioDataRegs.GPCSET.bit.GPIO76 = 1;
#define LED_1_Toggle	GpioDataRegs.GPCTOGGLE.bit.GPIO76 = 1;
#define LED_1_Off		GpioDataRegs.GPCCLEAR.bit.GPIO76 = 1;
#define LED_2_On		GpioDataRegs.GPCSET.bit.GPIO77 = 1;
#define LED_2_Toggle	GpioDataRegs.GPCTOGGLE.bit.GPIO77 = 1;
#define LED_2_Off		GpioDataRegs.GPCCLEAR.bit.GPIO77 = 1;
#define LED_3_On		GpioDataRegs.GPCSET.bit.GPIO78 = 1;
#define LED_3_Toggle	GpioDataRegs.GPCTOGGLE.bit.GPIO78 = 1;
#define LED_3_Off		GpioDataRegs.GPCCLEAR.bit.GPIO78 = 1;
#define LED_4_On		GpioDataRegs.GPCSET.bit.GPIO79 = 1;
#define LED_4_Toggle	GpioDataRegs.GPCTOGGLE.bit.GPIO79 = 1;
#define LED_4_Off		GpioDataRegs.GPCCLEAR.bit.GPIO79 = 1;

#define Relay2_On		GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;
#define Relay2_Off		GpioDataRegs.GPASET.bit.GPIO1 = 1;
#define Relay3_On		GpioDataRegs.GPACLEAR.bit.GPIO2 = 1;
#define Relay3_Off		GpioDataRegs.GPASET.bit.GPIO2 = 1;
#define Relay4_On		GpioDataRegs.GPACLEAR.bit.GPIO4 = 1;
#define Relay4_Off		GpioDataRegs.GPASET.bit.GPIO4 = 1;
#define Relay5_On		GpioDataRegs.GPACLEAR.bit.GPIO5 = 1;
#define Relay5_Off		GpioDataRegs.GPASET.bit.GPIO5 = 1;

#define NOTIFY_TOUCH_PRESS      0X01   //触摸屏按下通知
#define NOTIFY_TOUCH_RELEASE  0X03  //触摸屏松开通知
#define NOTIFY_WRITE_FLASH_OK  0X0C  //写FLASH成功
#define NOTIFY_WRITE_FLASH_FAILD  0X0D  //写FLASH失败
#define NOTIFY_READ_FLASH_OK  0X0B  //读FLASH成功
#define NOTIFY_READ_FLASH_FAILD  0X0F  //读FLASH失败
#define NOTIFY_MENU                        0X14  //菜单事件通知
#define NOTIFY_TIMER                       0X43  //定时器超时通知
#define NOTIFY_CONTROL                0XB1  //控件更新通知
#define NOTIFY_SCREEN                   0XB2  //画面切换通知

#define PTR2U16(PTR) ((((Uint8 *)(PTR))[0]<<8)|((Uint8 *)(PTR))[1])  //从缓冲区取16位数据
#define PTR2U32(PTR) ((((Uint8 *)(PTR))[0]<<24)|(((Uint8 *)(PTR))[1]<<16)|(((Uint8 *)(PTR))[2]<<8)|((Uint8 *)(PTR))[3])  //从缓冲区取32位数据


enum ctrl_msg
{
	MsgChangeScreen=0x01,//切换画面
	MsgReadState=0x11,//读取控件值
};
enum CtrlType
{
	kCtrlUnknown=0x0,
	kCtrlButton=0x10,  //按钮
	kCtrlText,  //文本
	kCtrlProgress,  //进度条
	kCtrlSlider,    //滑动条
	kCtrlMeter,  //仪表
	kCtrlDropList, //下拉列表
	kCtrlAnimation, //动画
	kCtrlRTC, //时间显示
	kCtrlGraph, //曲线图控件
	kCtrlTable, //表格控件
	kCtrlMenu,//菜单控件
	kCtrlSelector,//选择控件
	kCtrlQRCode,//二维码
};

//#pragma pack(push)
//#pragma pack(1)	//按字节对齐

typedef struct
{
	Uint8    cmd_head;  //帧头

	Uint8    cmd_type;  //命令类型(UPDATE_CONTROL)
	Uint8    ctrl_msg;   //CtrlMsgType-指示消息的类型
	Uint16   screen_id;  //产生消息的画面ID
	Uint16   control_id;  //产生消息的控件ID
	Uint8    control_type; //控件类型

	Uint8    param[256];//可变长度参数，最多256个字节

	Uint8  cmd_tail[4];   //帧尾
}CTRL_MSG,*PCTRL_MSG;

void InitGPIO(void);
void Init_LED(void);
void Init_Relays(void);
void SciB485_Init();
void SciC232_Init();
//void SciB_FIFO_Init();
//void SciC_FIFO_Init();
void SciB485_TXmit(char a);
void SciC232_TXmit(char a);
void SciB485_TXMSG(char * msg);
void SciC232_TXMSG(char * msg);
void My_InitADC(void);

__interrupt void MySciB485Rx_IsrB(void);//-------SCI485接收中断--------
__interrupt void MySciC232Rx_IsrC(void);//-------SCI485接收中断--------
__interrupt void ADC_Isr(void);
__interrupt void CPU_Timer0_Isr(void);

void MY_HMI_Handle();
/*!
 *  \brief  消息处理流程，此处一般不需要更改
 *  \param msg 待处理消息
 *  \param size 消息长度
 */
void ProcessMessage( PCTRL_MSG msg, Uint16 size );
/*!
 *  \brief  画面切换通知
 *  \details  当前画面改变时(或调用GetScreen)，执行此函数
 *  \param screen_id 当前画面ID
 */
void NotifyScreen(Uint16 screen_id);

/*!
 *  \brief  触摸坐标事件响应
 *  \param press 1按下触摸屏，3松开触摸屏
 *  \param x x坐标
 *  \param y y坐标
 */
void NotifyTouchXY(Uint8 press,Uint16 x,Uint16 y);
//文本控件显示1位小数值
void SetTextValueFloat1(Uint16 screen_id, Uint16 control_id,int32 value);
//文本控件显示2位小数值
void SetTextValueFloat2(Uint16 screen_id, Uint16 control_id,int32 value);
//文本控件显示整数值
void SetTextValueInt32(Uint16 screen_id, Uint16 control_id,int32 value);

/*!
 *  \brief  读取文本控件内容
 *  \details  当文本通过键盘更新(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 */
void ReadText(Uint16 screen_id, Uint16 control_id);
//字符串转整数
int32 StringToInt32(Uint8 *str);

//更新界面控件显示
void UpdateUI();

/*!
 *  \brief  按钮控件通知
 *  \details  当按钮状态改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param state 按钮状态：0弹起，1按下
 */
void NotifyButton(Uint16 screen_id, Uint16 control_id, Uint8  state);

/*!
 *  \brief  文本控件通知
 *  \details  当文本通过键盘更新(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param str 文本控件内容
 */
void NotifyText(Uint16 screen_id, Uint16 control_id, Uint8 *str);

/*!
 *  \brief  进度条控件通知
 *  \details  调用GetControlValue时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyProgress(Uint16 screen_id, Uint16 control_id, Uint32 value);
/*!
 *  \brief  滑动条控件通知
 *  \details  当滑动条改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifySlider(Uint16 screen_id, Uint16 control_id, Uint32 value);

/*!
 *  \brief  仪表控件通知
 *  \details  调用GetControlValue时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyMeter(Uint16 screen_id, Uint16 control_id, Uint32 value);

/*!
 *  \brief  菜单控件通知
 *  \details  当菜单项按下或松开时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param item 菜单项索引
 *  \param state 按钮状态：0松开，1按下
 */
void NotifyMenu(Uint16 screen_id, Uint16 control_id, Uint8  item, Uint8  state);

/*!
 *  \brief  选择控件通知
 *  \details  当选择控件变化时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param item 当前选项
 */
void NotifySelector(Uint16 screen_id, Uint16 control_id, Uint8  item);

/*!
 *  \brief  定时器超时通知处理
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 */
void NotifyTimer(Uint16 screen_id, Uint16 control_id);

/*!
 *  \brief  读取用户FLASH状态返回
 *  \param status 0失败，1成功
 *  \param _data 返回数据
 *  \param length 数据长度
 */
void NotifyReadFlash(Uint8 status,Uint8 *_data,Uint16 length);

/*!
 *  \brief  写用户FLASH状态返回
 *  \param status 0失败，1成功
 */
void NotifyWriteFlash(Uint8 status);

#endif // __MY_BSP_H__
