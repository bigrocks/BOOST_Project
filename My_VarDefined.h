/*
 * My_VarDefined.h
 *
 *  Created on: 2016年1月28日
 *      Author: ZHL
 */

#ifndef MY_VARDEFINED_H_
#define MY_VARDEFINED_H_

#include "DSP28x_Project.h"


/*自定义结构*/
/** @addtogroup Process_Exported_Types
  * @{
  */



#define OVER_V 260
#define UNDER_V 100


#define BUCK 0x11   //降压
#define BUCK_1 0x13   //降压1档
#define BUCK_2 0x15   //降压2档
#define BUCK_3 0x17   //降压3档
#define BYPASS 0x55   //旁路
#define BOOST 0xAA   //升压
#define BOOST_1 0xA3   //升压1档
#define BOOST_2 0xA5   //升压2档
#define BOOST_3 0xA7   //升压3档
#define Idel 0x00   //升压旁路状态 ，是上电后的初始状态

#define TK1_ID 0x02   //TK1的ID
#define TK2_ID 0x03   //TK2的ID
#define CF_ID 0x04   //电容组CF的ID
#define MY_ID 0x01   //控制器的ID
#define Broadcast_ID 0xAA   //广播的ID

#define OPEN 0x77   //闭合66 断开77 烧结55
#define CLOSE 0x66   //闭合66 断开77 烧结55
#define Sintering 0x55   //闭合11 断开22 烧结55

#define Auto 0x55   //自动
#define Mann 0xaa   //手动

//发送标志位
typedef enum
{
	Uart_Error = 0xEE,
	Uart_Idel = 0x0,//串口空闲
	Uart_ToPull = 0x81,//将要发送轮询
	Uart_WaitPullToAck = 0x83,//轮询发送完毕，等待轮询回复
	Uart_PullAcked = 0x85,//轮询帧已回复
	Uart_ToOnK1 = 0x11,//将要发送K1闭合指令
	Uart_WaitOnK1ToAck = 0x13,//K1闭合指令已发送，等待回复
	Uart_OnK1Acked = 0x15,//K1闭合指令已回复
	Uart_ToOffK1 = 0x17,//将要发送K1断开指令
	Uart_WaitOffK1ToAck = 0x19,//K1断开指令已发送，等待回复
	Uart_OffK1Acked = 0x1A,//K1断开指令已回复
	Uart_ToOnK2 = 0x21,
	Uart_WaitOnK2ToAck = 0x23,
	Uart_OnK2Acked = 0x25,
	Uart_ToOffK2 = 0x27,
	Uart_WaitOffK2ToAck = 0x29,
	Uart_OffK2Acked = 0x2A,
	Uart_ToOnK3 = 0x31,
	Uart_WaitOnK3ToAck = 0x33,
	Uart_OnK3Acked = 0x35,
	Uart_ToOffK3 = 0x37,
	Uart_WaitOffK3ToAck = 0x39,
	Uart_OffK3Acked = 0x3A,
	Uart_ToOnK4 = 0x41,
	Uart_WaitOnK4ToAck = 0x43,
	Uart_OnK4Acked = 0x45,
	Uart_ToOffK4 = 0x47,
	Uart_WaitOffK4ToAck = 0x49,
	Uart_OffK4Acked = 0x4A,
	Uart_ToOnK5 = 0x51,
	Uart_WaitOnK5ToAck = 0x53,
	Uart_OnK5Acked = 0x55,
	Uart_ToOffK5 = 0x57,
	Uart_WaitOffK5ToAck = 0x59,
	Uart_OffK5Acked = 0x5A,
	Uart_ToOnK6 = 0x61,
	Uart_WaitOnK6ToAck = 0x63,
	Uart_OnK6Acked = 0x65,
	Uart_ToOffK6 = 0x67,
	Uart_WaitOffK6ToAck = 0x69,
	Uart_OffK6Acked = 0x6A,
} e_UartState;//用于指示串口的当前状态

typedef struct{
    Uint8    LastState;//上一个状态
    Uint8    CurrentState;//当前状态
    Uint8    Adjustment_level;//计算出来要动作的档位数
    Uint8    KAction_State; //
    Uint8    Level_IsChanged; //档位已经发生变化
}VCtrlStruct;  //升压状态结构体

typedef struct
{
  Uint8 Receive_Flag; //接收标志位、
  Uint8 Repeat_StateMachine;//需要多次发送时状态机标志位
}UartcomFlagStruct;//串口参数结构体

typedef enum
{
	Enable = 0x01,  //显示
	Disable = 0x00  //隐藏
};

typedef enum
{
	Mode_AUTO = 0xaa,
	Mode_MANNUL = 0x55
};
typedef struct
{
	Uint8 Control_Mode;//shoudong  0x55 zidong 0xaa
	Uint8 My_ID;
	Uint16 CT;//控制方式
	Uint16 Judg_Delay ; //判断延时;
	Uint16 KVA_Amount;//设备容量
}SetParaStruct;//设置参数结构体

typedef struct
{
  Uint8 C1_Flag;   //C1 状态0x33没投  0x53已投
  Uint8 C2_Flag;
  Uint8 C3_Flag;
  Uint16 C1_Cap;    //C1容量 *100
  Uint16 C2_Cap;
  Uint16 C3_Cap;
  Uint16 C_Total;  //总的补偿容量
}CParameterStruct;//电容参数结构体 状态表



//正常使用顺序RelayToOn->RelayToOn1->RelayToOn2 ->RelayOn ->RelayToOff ->RelayToOff2 -> RelayOff -> RelayToOn->......
typedef enum
{
	RelayToOn = 0x33,//下达继电器将要闭合的指令，等待过零触发
//	RelayToOn1 = 0x44,//继电器将要闭合，等待延时
//	RelayToOn2 = 0x55,//Step2保持一定的输出脉宽
	RelayOn = 0x66,//继电器已经闭合，并保持
	RelayToOff = 0x73,//下达继电器将要断开的指令，等待过零触发
//	RelayToOff1 = 0x74,//电压过零，等待电流过零触发
//	RelayToOff2 = 0x75,//继电器开始断开,保持一定的输出脉宽
	RelayOff = 0x77,//继电器已经断开，并保持
	UnWantedRelay = 0x79//error
} e_RelayState;//用于指示电容的当前状态

typedef struct
{
	volatile e_RelayState  Relay1StateIndex; /*!< 当前状态索引号  */
	volatile e_RelayState  Relay2StateIndex; /*!< 当前状态索引号  */
	volatile e_RelayState  Relay3StateIndex; /*!< 当前状态索引号  */
	volatile e_RelayState  Relay4StateIndex; /*!< 当前状态索引号  */
	volatile e_RelayState  Relay5StateIndex; /*!< 当前状态索引号  */
	volatile e_RelayState  Relay6StateIndex; /*!< 当前状态索引号  */
	volatile e_RelayState  Relay7StateIndex; /*!< 当前状态索引号  */

}RelayStruct;	//继电器结构体，存放跟继电器相关的一切参数

typedef struct
{
  Uint16 CT;	//互感器变比
  Uint8 PF_Goal;//目标功率因数
  Uint8 PF_Now;//当前功率因数
  Uint16 Voltage_Input;   //输入电压
  Uint16 Current_Input;   //输入电流
  Uint16 Voltage_Output;  //输出电压
  Uint16 Current_Output;  //输出电流
  Uint16 Reactive_Power;  //无功功率
  Uint16 Active_Power;    //有功功率
  Uint16 Reactive_PowertoDTU;  //无功功率 给DTU
  Uint16 Active_PowertoDTU;    //有功功率 给DTU
  Uint8 Voltage_Goal;    //目标电压值
  Uint8 Voltage_Cut;    //过载倍数
  Uint8 VErro_Code;  //升压故障代码
  Uint8 CErro_Code;  //电容故障代码
  Uint8 V_Level;   //升压档数  0x45升压一档  0x57升压2档
  Uint8 V_up;     //电压提升了百分比
}SysParameterStruct;//系统参数结构体

typedef struct
{
  Uint8 OverVoltage;	//过压  33表示没有故障，55表示有故障
  Uint8 OverCurrent;	//过流
  Uint8 OverTemp;	//过温
  Uint8 UnderVoltage;	//欠压
  Uint8 RelayErro;	//开关故障
}SysErroStateStruct;//系统标志结构体

typedef struct
{
  Uint32 H;         //总谐波 乘以100
  Uint8 H3;	//3次谐波 乘以100
  Uint8 H5;	//
  Uint8 H7;	//
  Uint8 H9;	//
  Uint8 H11;	//
  Uint8 H13;	//
  Uint8 H15;	//
  Uint8 H17;	//
}HarmonicStruct;//
typedef struct
{
Uint8 flag;
Uint8 Light_Count;///////////////////////背光熄灭计时,30s
Uint8 Is_To_Light_Screen;/////////屏幕背光点亮,为1点亮
Uint8 Time_Count_OverCurrent;/////////////////计时停止发送的时间
Uint8 Time_Count_Flash;/////////////////计时停止发送的时间
Uint8 Time_Count_Judg;/////////////////计时控制判断时间
Uint8 Judg_OK;
Uint8 Dis_C_Flag;
Uint8 Flash_Screen;////刷屏
Uint8 Send_Flag_Slave;
Uint8 Send_Flag_C;
Uint8 Send_Flag_V;
Uint8 Send_Flag_M;//MODBUS发送时能
Uint8 Init_Display;///////////////初始化显示屏，0表示初始化
/******************************/
Uint8 flag_longkey_start;////长按开始标志
Uint8 flag_longkey_stop;////长按结束标志
Uint8 longkey_count;///////////长按计时
Uint8 Time20msFlag;
Uint8 Time1sFlag;
Uint8 Time1minFlag;
}TimeFlagStruct;//时间点结构体

typedef struct   //基波值
{
double UABase;////////////////////基波电压
double IABase;////////////////////基波电流
double UBBase;////////////////////基波电压
double IBBase;////////////////////基波电流
double UCBase;////////////////////基波电压
double ICBase;////////////////////基波电流
}UIBaseStruct;//

typedef struct
{
  Uint8 Fuc;////////////////////功能码
  Uint8 ID;//////////////////////发送ID
  Uint8 Receive_Error;////////接收数据是否有误，为1表示接收数据错误
  Uint8 RS485_RX_CNT;//接收到的数据长度
  Uint8 RX_Start;//////开始接收
  Uint8 RS485_RX_Complete;////////数据接收完成
  e_UartState SlaveRS485_State;
  Uint8 RS485_TX_IsRepeat;
  Uint8 unack_cnt;
}UartParameterStruct;//串口参数结构体

typedef enum
{
	UnStartMSG,//尚未启动
	UnWantedMSG_State//error
} e_MSG_State;

typedef enum
{
	FlagTrue = 0x35,//True
	FlagFalse = 0x55,//False
	UnWantedFlag = 0x42//error
} e_Flag;//用于指示始能标志位

/**
  * @brief
  */
typedef enum
{
	EnFlagFalse = 0x33,//False
	EnFlagTrue = 0x55,//True
	UnWantedEnFlag = 0x72//error
} e_EnFlag;//用于指示始能标志位

typedef struct
{
  e_MSG_State MSG_StateIndex; //当前状态索引号
  e_Flag IsBusy;//当前是否空闲，可否进行压栈、出栈操作
  Uint16 Value;//MSG VALUE	MSG_VALUE(Uint32)
  e_EnFlag EnMSG;//MSG始能
}MSG_Struct;	//
typedef struct
{
  e_EnFlag EnDisplay;//进行一次刷屏
  e_EnFlag EnKeyDisplay;//进行一次按键刷屏
  e_EnFlag EnLedOn;//点亮液晶屏背光
}HMI_Struct;	//人机交互结构体，存放跟人机交互相关的一切参数

extern HarmonicStruct IAHarmonicVar;
extern HarmonicStruct UAHarmonicVar;
extern HarmonicStruct IBHarmonicVar;
extern HarmonicStruct UBHarmonicVar;
extern SetParaStruct SetParaVar;
extern SysParameterStruct SysParameterVar;
extern SysParameterStruct SysParameterVar_A;//输入通路
extern SysParameterStruct SysParameterVar_B;//输出通路
extern SysParameterStruct SysParameterVar_C;
extern UIBaseStruct UIBaseVar;
extern TimeFlagStruct TimeFlagVar;
extern UartParameterStruct UartParameterVar;
extern VCtrlStruct VCtrlVar;
extern UartcomFlagStruct UartcomFlagVar;
extern MSG_Struct MSG_StructVar;
extern CParameterStruct CParameterVar;
extern SysErroStateStruct CSysErroStateVar;
extern SysErroStateStruct VSysErroStateVar;

#endif /* MY_VARDEFINED_H_ */
