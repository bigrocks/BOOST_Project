/********************************************************************
* �ļ����� 	������Դ��ʼ���Ͷ���
* ����:
**********************************************************************/
/********************************************************************
����˵����
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

/**************************************�궨��************************************************/
#define Timer0_Period   0x80;

#define SCIB485TX_On		GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;//�з��ţ����Եͷ���
#define SCIB485RX_On		GpioDataRegs.GPASET.bit.GPIO22 = 1;//�з��ţ����Ը߽���

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

#define NOTIFY_TOUCH_PRESS      0X01   //����������֪ͨ
#define NOTIFY_TOUCH_RELEASE  0X03  //�������ɿ�֪ͨ
#define NOTIFY_WRITE_FLASH_OK  0X0C  //дFLASH�ɹ�
#define NOTIFY_WRITE_FLASH_FAILD  0X0D  //дFLASHʧ��
#define NOTIFY_READ_FLASH_OK  0X0B  //��FLASH�ɹ�
#define NOTIFY_READ_FLASH_FAILD  0X0F  //��FLASHʧ��
#define NOTIFY_MENU                        0X14  //�˵��¼�֪ͨ
#define NOTIFY_TIMER                       0X43  //��ʱ����ʱ֪ͨ
#define NOTIFY_CONTROL                0XB1  //�ؼ�����֪ͨ
#define NOTIFY_SCREEN                   0XB2  //�����л�֪ͨ

#define PTR2U16(PTR) ((((Uint8 *)(PTR))[0]<<8)|((Uint8 *)(PTR))[1])  //�ӻ�����ȡ16λ����
#define PTR2U32(PTR) ((((Uint8 *)(PTR))[0]<<24)|(((Uint8 *)(PTR))[1]<<16)|(((Uint8 *)(PTR))[2]<<8)|((Uint8 *)(PTR))[3])  //�ӻ�����ȡ32λ����


enum ctrl_msg
{
	MsgChangeScreen=0x01,//�л�����
	MsgReadState=0x11,//��ȡ�ؼ�ֵ
};
enum CtrlType
{
	kCtrlUnknown=0x0,
	kCtrlButton=0x10,  //��ť
	kCtrlText,  //�ı�
	kCtrlProgress,  //������
	kCtrlSlider,    //������
	kCtrlMeter,  //�Ǳ�
	kCtrlDropList, //�����б�
	kCtrlAnimation, //����
	kCtrlRTC, //ʱ����ʾ
	kCtrlGraph, //����ͼ�ؼ�
	kCtrlTable, //���ؼ�
	kCtrlMenu,//�˵��ؼ�
	kCtrlSelector,//ѡ��ؼ�
	kCtrlQRCode,//��ά��
};

//#pragma pack(push)
//#pragma pack(1)	//���ֽڶ���

typedef struct
{
	Uint8    cmd_head;  //֡ͷ

	Uint8    cmd_type;  //��������(UPDATE_CONTROL)
	Uint8    ctrl_msg;   //CtrlMsgType-ָʾ��Ϣ������
	Uint16   screen_id;  //������Ϣ�Ļ���ID
	Uint16   control_id;  //������Ϣ�Ŀؼ�ID
	Uint8    control_type; //�ؼ�����

	Uint8    param[256];//�ɱ䳤�Ȳ��������256���ֽ�

	Uint8  cmd_tail[4];   //֡β
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

__interrupt void MySciB485Rx_IsrB(void);//-------SCI485�����ж�--------
__interrupt void MySciC232Rx_IsrC(void);//-------SCI485�����ж�--------
__interrupt void ADC_Isr(void);
__interrupt void CPU_Timer0_Isr(void);

void MY_HMI_Handle();
/*!
 *  \brief  ��Ϣ�������̣��˴�һ�㲻��Ҫ����
 *  \param msg ��������Ϣ
 *  \param size ��Ϣ����
 */
void ProcessMessage( PCTRL_MSG msg, Uint16 size );
/*!
 *  \brief  �����л�֪ͨ
 *  \details  ��ǰ����ı�ʱ(�����GetScreen)��ִ�д˺���
 *  \param screen_id ��ǰ����ID
 */
void NotifyScreen(Uint16 screen_id);

/*!
 *  \brief  ���������¼���Ӧ
 *  \param press 1���´�������3�ɿ�������
 *  \param x x����
 *  \param y y����
 */
void NotifyTouchXY(Uint8 press,Uint16 x,Uint16 y);
//�ı��ؼ���ʾ1λС��ֵ
void SetTextValueFloat1(Uint16 screen_id, Uint16 control_id,int32 value);
//�ı��ؼ���ʾ2λС��ֵ
void SetTextValueFloat2(Uint16 screen_id, Uint16 control_id,int32 value);
//�ı��ؼ���ʾ����ֵ
void SetTextValueInt32(Uint16 screen_id, Uint16 control_id,int32 value);

/*!
 *  \brief  ��ȡ�ı��ؼ�����
 *  \details  ���ı�ͨ�����̸���(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 */
void ReadText(Uint16 screen_id, Uint16 control_id);
//�ַ���ת����
int32 StringToInt32(Uint8 *str);

//���½���ؼ���ʾ
void UpdateUI();

/*!
 *  \brief  ��ť�ؼ�֪ͨ
 *  \details  ����ť״̬�ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param state ��ť״̬��0����1����
 */
void NotifyButton(Uint16 screen_id, Uint16 control_id, Uint8  state);

/*!
 *  \brief  �ı��ؼ�֪ͨ
 *  \details  ���ı�ͨ�����̸���(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param str �ı��ؼ�����
 */
void NotifyText(Uint16 screen_id, Uint16 control_id, Uint8 *str);

/*!
 *  \brief  �������ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyProgress(Uint16 screen_id, Uint16 control_id, Uint32 value);
/*!
 *  \brief  �������ؼ�֪ͨ
 *  \details  ���������ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifySlider(Uint16 screen_id, Uint16 control_id, Uint32 value);

/*!
 *  \brief  �Ǳ�ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyMeter(Uint16 screen_id, Uint16 control_id, Uint32 value);

/*!
 *  \brief  �˵��ؼ�֪ͨ
 *  \details  ���˵���»��ɿ�ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item �˵�������
 *  \param state ��ť״̬��0�ɿ���1����
 */
void NotifyMenu(Uint16 screen_id, Uint16 control_id, Uint8  item, Uint8  state);

/*!
 *  \brief  ѡ��ؼ�֪ͨ
 *  \details  ��ѡ��ؼ��仯ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item ��ǰѡ��
 */
void NotifySelector(Uint16 screen_id, Uint16 control_id, Uint8  item);

/*!
 *  \brief  ��ʱ����ʱ֪ͨ����
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 */
void NotifyTimer(Uint16 screen_id, Uint16 control_id);

/*!
 *  \brief  ��ȡ�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 *  \param _data ��������
 *  \param length ���ݳ���
 */
void NotifyReadFlash(Uint8 status,Uint8 *_data,Uint16 length);

/*!
 *  \brief  д�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 */
void NotifyWriteFlash(Uint8 status);

#endif // __MY_BSP_H__
