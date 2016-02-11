/*
 * My_VarDefined.h
 *
 *  Created on: 2016��1��28��
 *      Author: ZHL
 */

#ifndef MY_VARDEFINED_H_
#define MY_VARDEFINED_H_

#include "DSP28x_Project.h"


/*�Զ���ṹ*/
/** @addtogroup Process_Exported_Types
  * @{
  */



#define OVER_V 260
#define UNDER_V 100


#define BUCK 0x11   //��ѹ
#define BUCK_1 0x13   //��ѹ1��
#define BUCK_2 0x15   //��ѹ2��
#define BUCK_3 0x17   //��ѹ3��
#define BYPASS 0x55   //��·
#define BOOST 0xAA   //��ѹ
#define BOOST_1 0xA3   //��ѹ1��
#define BOOST_2 0xA5   //��ѹ2��
#define BOOST_3 0xA7   //��ѹ3��
#define Idel 0x00   //��ѹ��·״̬ �����ϵ��ĳ�ʼ״̬

#define TK1_ID 0x02   //TK1��ID
#define TK2_ID 0x03   //TK2��ID
#define CF_ID 0x04   //������CF��ID
#define MY_ID 0x01   //��������ID
#define Broadcast_ID 0xAA   //�㲥��ID

#define OPEN 0x77   //�պ�66 �Ͽ�77 �ս�55
#define CLOSE 0x66   //�պ�66 �Ͽ�77 �ս�55
#define Sintering 0x55   //�պ�11 �Ͽ�22 �ս�55

#define Auto 0x55   //�Զ�
#define Mann 0xaa   //�ֶ�

//���ͱ�־λ
typedef enum
{
	Uart_Error = 0xEE,
	Uart_Idel = 0x0,//���ڿ���
	Uart_ToPull = 0x81,//��Ҫ������ѯ
	Uart_WaitPullToAck = 0x83,//��ѯ������ϣ��ȴ���ѯ�ظ�
	Uart_PullAcked = 0x85,//��ѯ֡�ѻظ�
	Uart_ToOnK1 = 0x11,//��Ҫ����K1�պ�ָ��
	Uart_WaitOnK1ToAck = 0x13,//K1�պ�ָ���ѷ��ͣ��ȴ��ظ�
	Uart_OnK1Acked = 0x15,//K1�պ�ָ���ѻظ�
	Uart_ToOffK1 = 0x17,//��Ҫ����K1�Ͽ�ָ��
	Uart_WaitOffK1ToAck = 0x19,//K1�Ͽ�ָ���ѷ��ͣ��ȴ��ظ�
	Uart_OffK1Acked = 0x1A,//K1�Ͽ�ָ���ѻظ�
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
} e_UartState;//����ָʾ���ڵĵ�ǰ״̬

typedef struct{
    Uint8    LastState;//��һ��״̬
    Uint8    CurrentState;//��ǰ״̬
    Uint8    Adjustment_level;//�������Ҫ�����ĵ�λ��
    Uint8    KAction_State; //
    Uint8    Level_IsChanged; //��λ�Ѿ������仯
}VCtrlStruct;  //��ѹ״̬�ṹ��

typedef struct
{
  Uint8 Receive_Flag; //���ձ�־λ��
  Uint8 Repeat_StateMachine;//��Ҫ��η���ʱ״̬����־λ
}UartcomFlagStruct;//���ڲ����ṹ��

typedef enum
{
	Enable = 0x01,  //��ʾ
	Disable = 0x00  //����
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
	Uint16 CT;//���Ʒ�ʽ
	Uint16 Judg_Delay ; //�ж���ʱ;
	Uint16 KVA_Amount;//�豸����
}SetParaStruct;//���ò����ṹ��

typedef struct
{
  Uint8 C1_Flag;   //C1 ״̬0x33ûͶ  0x53��Ͷ
  Uint8 C2_Flag;
  Uint8 C3_Flag;
  Uint16 C1_Cap;    //C1���� *100
  Uint16 C2_Cap;
  Uint16 C3_Cap;
  Uint16 C_Total;  //�ܵĲ�������
}CParameterStruct;//���ݲ����ṹ�� ״̬��



//����ʹ��˳��RelayToOn->RelayToOn1->RelayToOn2 ->RelayOn ->RelayToOff ->RelayToOff2 -> RelayOff -> RelayToOn->......
typedef enum
{
	RelayToOn = 0x33,//�´�̵�����Ҫ�պϵ�ָ��ȴ����㴥��
//	RelayToOn1 = 0x44,//�̵�����Ҫ�պϣ��ȴ���ʱ
//	RelayToOn2 = 0x55,//Step2����һ�����������
	RelayOn = 0x66,//�̵����Ѿ��պϣ�������
	RelayToOff = 0x73,//�´�̵�����Ҫ�Ͽ���ָ��ȴ����㴥��
//	RelayToOff1 = 0x74,//��ѹ���㣬�ȴ��������㴥��
//	RelayToOff2 = 0x75,//�̵�����ʼ�Ͽ�,����һ�����������
	RelayOff = 0x77,//�̵����Ѿ��Ͽ���������
	UnWantedRelay = 0x79//error
} e_RelayState;//����ָʾ���ݵĵ�ǰ״̬

typedef struct
{
	volatile e_RelayState  Relay1StateIndex; /*!< ��ǰ״̬������  */
	volatile e_RelayState  Relay2StateIndex; /*!< ��ǰ״̬������  */
	volatile e_RelayState  Relay3StateIndex; /*!< ��ǰ״̬������  */
	volatile e_RelayState  Relay4StateIndex; /*!< ��ǰ״̬������  */
	volatile e_RelayState  Relay5StateIndex; /*!< ��ǰ״̬������  */
	volatile e_RelayState  Relay6StateIndex; /*!< ��ǰ״̬������  */
	volatile e_RelayState  Relay7StateIndex; /*!< ��ǰ״̬������  */

}RelayStruct;	//�̵����ṹ�壬��Ÿ��̵�����ص�һ�в���

typedef struct
{
  Uint16 CT;	//���������
  Uint8 PF_Goal;//Ŀ�깦������
  Uint8 PF_Now;//��ǰ��������
  Uint16 Voltage_Input;   //�����ѹ
  Uint16 Current_Input;   //�������
  Uint16 Voltage_Output;  //�����ѹ
  Uint16 Current_Output;  //�������
  Uint16 Reactive_Power;  //�޹�����
  Uint16 Active_Power;    //�й�����
  Uint16 Reactive_PowertoDTU;  //�޹����� ��DTU
  Uint16 Active_PowertoDTU;    //�й����� ��DTU
  Uint8 Voltage_Goal;    //Ŀ���ѹֵ
  Uint8 Voltage_Cut;    //���ر���
  Uint8 VErro_Code;  //��ѹ���ϴ���
  Uint8 CErro_Code;  //���ݹ��ϴ���
  Uint8 V_Level;   //��ѹ����  0x45��ѹһ��  0x57��ѹ2��
  Uint8 V_up;     //��ѹ�����˰ٷֱ�
}SysParameterStruct;//ϵͳ�����ṹ��

typedef struct
{
  Uint8 OverVoltage;	//��ѹ  33��ʾû�й��ϣ�55��ʾ�й���
  Uint8 OverCurrent;	//����
  Uint8 OverTemp;	//����
  Uint8 UnderVoltage;	//Ƿѹ
  Uint8 RelayErro;	//���ع���
}SysErroStateStruct;//ϵͳ��־�ṹ��

typedef struct
{
  Uint32 H;         //��г�� ����100
  Uint8 H3;	//3��г�� ����100
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
Uint8 Light_Count;///////////////////////����Ϩ���ʱ,30s
Uint8 Is_To_Light_Screen;/////////��Ļ�������,Ϊ1����
Uint8 Time_Count_OverCurrent;/////////////////��ʱֹͣ���͵�ʱ��
Uint8 Time_Count_Flash;/////////////////��ʱֹͣ���͵�ʱ��
Uint8 Time_Count_Judg;/////////////////��ʱ�����ж�ʱ��
Uint8 Judg_OK;
Uint8 Dis_C_Flag;
Uint8 Flash_Screen;////ˢ��
Uint8 Send_Flag_Slave;
Uint8 Send_Flag_C;
Uint8 Send_Flag_V;
Uint8 Send_Flag_M;//MODBUS����ʱ��
Uint8 Init_Display;///////////////��ʼ����ʾ����0��ʾ��ʼ��
/******************************/
Uint8 flag_longkey_start;////������ʼ��־
Uint8 flag_longkey_stop;////����������־
Uint8 longkey_count;///////////������ʱ
Uint8 Time20msFlag;
Uint8 Time1sFlag;
Uint8 Time1minFlag;
}TimeFlagStruct;//ʱ���ṹ��

typedef struct   //����ֵ
{
double UABase;////////////////////������ѹ
double IABase;////////////////////��������
double UBBase;////////////////////������ѹ
double IBBase;////////////////////��������
double UCBase;////////////////////������ѹ
double ICBase;////////////////////��������
}UIBaseStruct;//

typedef struct
{
  Uint8 Fuc;////////////////////������
  Uint8 ID;//////////////////////����ID
  Uint8 Receive_Error;////////���������Ƿ�����Ϊ1��ʾ�������ݴ���
  Uint8 RS485_RX_CNT;//���յ������ݳ���
  Uint8 RX_Start;//////��ʼ����
  Uint8 RS485_RX_Complete;////////���ݽ������
  e_UartState SlaveRS485_State;
  Uint8 RS485_TX_IsRepeat;
  Uint8 unack_cnt;
}UartParameterStruct;//���ڲ����ṹ��

typedef enum
{
	UnStartMSG,//��δ����
	UnWantedMSG_State//error
} e_MSG_State;

typedef enum
{
	FlagTrue = 0x35,//True
	FlagFalse = 0x55,//False
	UnWantedFlag = 0x42//error
} e_Flag;//����ָʾʼ�ܱ�־λ

/**
  * @brief
  */
typedef enum
{
	EnFlagFalse = 0x33,//False
	EnFlagTrue = 0x55,//True
	UnWantedEnFlag = 0x72//error
} e_EnFlag;//����ָʾʼ�ܱ�־λ

typedef struct
{
  e_MSG_State MSG_StateIndex; //��ǰ״̬������
  e_Flag IsBusy;//��ǰ�Ƿ���У��ɷ����ѹջ����ջ����
  Uint16 Value;//MSG VALUE	MSG_VALUE(Uint32)
  e_EnFlag EnMSG;//MSGʼ��
}MSG_Struct;	//
typedef struct
{
  e_EnFlag EnDisplay;//����һ��ˢ��
  e_EnFlag EnKeyDisplay;//����һ�ΰ���ˢ��
  e_EnFlag EnLedOn;//����Һ��������
}HMI_Struct;	//�˻������ṹ�壬��Ÿ��˻�������ص�һ�в���

extern HarmonicStruct IAHarmonicVar;
extern HarmonicStruct UAHarmonicVar;
extern HarmonicStruct IBHarmonicVar;
extern HarmonicStruct UBHarmonicVar;
extern SetParaStruct SetParaVar;
extern SysParameterStruct SysParameterVar;
extern SysParameterStruct SysParameterVar_A;//����ͨ·
extern SysParameterStruct SysParameterVar_B;//���ͨ·
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
