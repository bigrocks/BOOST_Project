/*
 * Message.h
 *
 *  Created on: 2016年1月28日
 *      Author: ZHL
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

/* Includes ------------------------------------------------------------------*/
#include "DSP28x_Project.h"
#include "My_VarDefined.h"


/* Exported types ------------------------------------------------------------*/

/** @addtogroup ADC2_Exported_Types
  * @{
  */

/**
  * @brief  ADC2 clock prescaler selection
  */

/**
  * @brief  ADC2 data alignment
  */
typedef enum
{
	MSG_NULL	= 0x00,					/*have no message*/
	MSG_KEY		= 0x01,					/*key message*/
	MSG_UART	= 0x02,					/*uart message*/
	MSG_RTC		= 0x03,					/*real time counter message*/
	MSG_MSTIMER	= 0x04,					/*timer message*/
	MSG_DISPLAY	= 0x05,                 /*display message*/
        MSG_CheckVLevel	= 0x06,                 /*display message*/
        MSG_CMD         = 0x07,
	MSG_ADC 	= 0x08,             /*ADC message*/
	MSG_FFT 	= 0x09,             /*ADC message*/
	MSG_SAVE_ALL    = 0x0A,             /*save message*/
        MSG_CheckIsAck  = 0x0B,
        MSG_AnalyseRX   = 0x0C,
        MSG_RepeatCMD   = 0x0D,
        MSG_CheckErr    = 0x0E,
        MSG_HRSENT      = 0x20,             /*发送心跳包*/
        MSG_WAIT1S      = 0x21,             /*等1s*/
        MSG_LCDDIE      = 0X22,

	MSG_TEST	= 0xEF				/*special for test message*/
}MSG_TYPE;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @addtogroup MSG_ARRAY_SIZE_Constants
  * @{
  */
#define MSG_ARRAY_SIZE	40

/**
  * @}
  */

/* Exported macros ------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/** @addtogroup Message_Private_Macros
  * @{
  */

/**
  * @brief  Macro
  */
#define MSG_SEND_MSG(MsgType, MsgVal)		msg_send_msg(MsgType, MsgVal)
#define MSG_SEND_HEAD_MSG(MsgType, MsgVal)		msg_send_msg_inHead(MsgType, MsgVal)

#define MSG_SEND_DATA(Uint32) 				msg_put_in(Uint32)

#define MSG_GET_MSG(pUint32)					msg_get_out(pUint32)
#define MSG_TYPE(Uint32)						((Uint16 *)(&Uint32))[1]
#define MSG_VALUE(Uint32)						((Uint16 *)(&Uint32))[0]
//#define MSG_GET_DATA(pUint32)					msg_get_out(pUint32)

void ClearMsgRelayAction(void);
void ClearAllMsg(void);

/* Private variables ---------------------------------------------------------*/
extern Uint32 Msg;
extern Uint16  MSG_Count;

extern Uint16  Tim2_PeriodTmp;

/**
  * @}
  */


/* Exported functions ------------------------------------------------------- */

/** @addtogroup Message_Exported_Functions
  * @{
  */

 void msg_put_in(Uint32 Arg);
 void msg_send_msg_inHead(Uint16 MsgType, Uint16 MsgArg);
 e_Flag msg_sendCap_msg(Uint16  CapType);
 void msg_sendNoCheck_msg(Uint16  MsgType, Uint16  MsgArg);
 void msg_send_msg(Uint16  MsgType, Uint16  MsgArg);
 Uint8 msg_check_msg(Uint16  MsgType, Uint16  MsgArg);//yurobo
 void msg_get_out(Uint32 *pArg);

 void DealWithMSG(void);
 void DealWithCheckMSG(void);
 void DoMsgInMain(void);
 void DealWithAbnormalPF(void);
 void SendMsgK1ToK2(void);
 void SendMsgK2ToK1(void);
/**
  * @}
  */



#endif /* MESSAGE_H_ */
