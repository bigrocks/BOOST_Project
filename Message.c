/**
  ******************************************************************************
  * @file     Message.c
  * @author   Zhenglei
  * @version  V0.0.1
  * @date     2014.1.5
  * @brief    .
  ******************************************************************************
  * @attention
  *
  *
  *
  * <h2><center>&copy; COPYRIGHT 2013 Rockwell119</center></h2>
  ******************************************************************************
  */
#include "DSP28x_Project.h"
#include "My_VarDefined.h"
#include "Message.h"


static Uint16 MsgHead = 0;		/*position that msg will be put in*/
static Uint16 MsgTail= 0;		/*position that msg will be get out*/
static Uint16 MsgNum = 0;		/*msg number*/
static Uint32 MsgArray[MSG_ARRAY_SIZE];	/*msg queue*/
extern Uint8 KeyResult;
//EEPROM
//extern Uint8 WriteBuffer[FLASH_BLOCK_SIZE];
//extern Uint8 ReadBuffer[FLASH_BLOCK_SIZE];

//MSG
//message
Uint32 Msg;
Uint16 MSG_Count = 0;

//For MSG
Uint16 MsgValue = 0;
Uint8 MSG_Busy20msCnt = 0;
Uint8 MSG_Busy1sCnt = 0;

Uint8 MSG_BusyCnt = 0;

Uint8 Wait1s_Cnt = 0;
e_EnFlag EnWait1s = EnFlagFalse;
e_EnFlag EnChkRelay = EnFlagFalse;

Uint16 Tim2_PeriodTmp = 0;


extern volatile Uint8 ADOK_Flag;
extern Uint8 enFFT_Flag;
extern Uint8 Rs485State;

 void InitMSG(void);


 void msg_put_in(Uint32 Arg)
{
	//Uint32 cpu_sr;
	if (MsgNum >= MSG_ARRAY_SIZE) {
		return;
	}

	//OS_ENTER_CRITICAL();

	MsgArray[ MsgHead] = Arg;
	MsgHead++;
	if (MsgHead >= MSG_ARRAY_SIZE) {
		MsgHead = 0;
	}
	MsgNum++;

	//OS_EXIT_CRITICAL();
}

/**
  * @brief	����ѹ��һϵ�в����Լ�������������
  * @param  Uint16 CapType:���ݳ��,1��2
  * @retval None
  */
 e_Flag msg_sendCap_msg(Uint16 CapType)
{}

/**
  * @brief	ֻѹջ�������ջ���Ƿ����ظ�����
  * @param  Uint16 MsgType ��������
  * @param  Uint16 MsgArg ������������ֵ
  * @retval None
  */
 void msg_sendNoCheck_msg(Uint16 MsgType, Uint16 MsgArg)
{
	if (MsgNum >= MSG_ARRAY_SIZE) {
		return ;
	}

        {


          ((Uint16 *)(&(MsgArray[ MsgHead])))[1] = MsgType;
          ((Uint16 *)(&(MsgArray[ MsgHead])))[0] = MsgArg;

          MsgHead++;
          if (MsgHead >= MSG_ARRAY_SIZE) {
                  MsgHead = 0;
          }
          MsgNum++;
        }
}

 void msg_send_msg(Uint16 MsgType, Uint16 MsgArg)
{
	if (MsgNum >= MSG_ARRAY_SIZE) {
		return;
	}
        //yu add 2014.03.15 ����ֵ:0--�� 0x11--û��
        if( msg_check_msg(MsgType, MsgArg)==0x11 )
        {
          ((Uint16 *)(&(MsgArray[ MsgHead])))[1] = MsgType;
          ((Uint16 *)(&(MsgArray[ MsgHead])))[0] = MsgArg;

          MsgHead++;
          if (MsgHead >= MSG_ARRAY_SIZE) {
                  MsgHead = 0;
          }
          MsgNum++;
        }
}

 void msg_send_msg_inHead(Uint16 MsgType, Uint16 MsgArg)
{
	if (MsgNum >= MSG_ARRAY_SIZE) {
		return;
	}
        //yu add 2014.03.15 ����ֵ:0--�� 0x11--û��
        //if( msg_check_msg(MsgType, MsgArg)==0x11 )
        {
        	{//����ѹ��Ͷ������
        		if(MsgTail>=1)
        			MsgTail--;
        		else//MsgTail=0;
        			MsgTail = MSG_ARRAY_SIZE - 1;

        		//����ѹ��Ͷ������
        		  ((Uint16 *)(&(MsgArray[ MsgTail])))[1] = MsgType;
        		  ((Uint16 *)(&(MsgArray[ MsgTail])))[0] = MsgArg;

        		  MsgNum++;
        	}
        }
}


/**
  * @brief  ���MSGջ����û����ͬ��MSG
  * @param  None
  * @retval  ����ֵ:0--�� 0x11--û��
  */
 Uint8 msg_check_msg(Uint16 MsgType, Uint16 MsgArg)
{
	Uint8 MsgNumTmp = MsgNum;
        Uint8 MsgTailTmp = MsgTail;
	Uint32 pArg;

        if (MsgNumTmp == 0)
        {
          //* pArg = 0x00;
          return 0x11;
        }

        for(; MsgNumTmp > 0; MsgNumTmp --)
        {
          pArg = MsgArray[MsgTailTmp];
          if((MSG_TYPE(pArg) == MsgType) && (MSG_VALUE(pArg) == MsgArg))
            return 0;//����ͬ��MSG

          MsgTailTmp ++;
          if (MsgTailTmp >= MSG_ARRAY_SIZE) {
                  MsgTailTmp = 0;
          }

        }
        return 0x11;
}

 void msg_get_out(Uint32 *pArg)
{
	//Uint32 cpu_sr;
	if (MsgNum == 0) {
		* pArg = 0x00;
		return;
	}

	//OS_ENTER_CRITICAL();

	*pArg = MsgArray[MsgTail];
	MsgTail++;
	if (MsgTail >= MSG_ARRAY_SIZE) {
		MsgTail = 0;
	}
	MsgNum--;

	//OS_EXIT_CRITICAL();
}

/**
  * @brief  ��ʱ����MSG����ش���.
  * @param  None
  * @retval None
  */
 void DealWithMSG(void)
{

	  //ÿ��20ms����оƬ�ɼ�һ��,�������ѹ��λ
      if((TimeFlagVar.Time20msFlag == 1))  //
      {
         MSG_SEND_MSG(MSG_ADC, 0);
      }

//          2.6S����������һ��
      if((TimeFlagVar.Time1sFlag%5 == 4)&&(TimeFlagVar.Time20msFlag == 31))
      {
        MSG_SEND_MSG(MSG_CMD, 0);

      }

      if((TimeFlagVar.Time20msFlag == 22))
      {
         MSG_SEND_MSG(MSG_AnalyseRX, 0);
      }
        //ÿ��500msˢ����Ļһ��
      if((TimeFlagVar.Time20msFlag == 25))
      {
        if(KeyResult == 0)
        {
          MSG_SEND_MSG(MSG_DISPLAY, 0);
        }
      }

        //ÿ1s��һ��������
      if( (TimeFlagVar.Time1sFlag%2 == 1)&&(TimeFlagVar.Time20msFlag == 49))
      {
        MSG_SEND_MSG(MSG_HRSENT, 0);
      }

      if( ((TimeFlagVar.Time1minFlag)%3 == 2) &&(TimeFlagVar.Time1sFlag == 2))  //2���Ӻ�Ϩ��LCD
      {
        if(TimeFlagVar.Is_To_Light_Screen == 1)
        {
          MSG_SEND_MSG(MSG_LCDDIE, 0);
        }
      }

      if(EnWait1s == EnFlagTrue)
      {
        MSG_StructVar.IsBusy = FlagTrue;
        Wait1s_Cnt++;
        if(Wait1s_Cnt>=50)//1s
        {
          Wait1s_Cnt = 0;
          MSG_StructVar.IsBusy = FlagFalse;
        }
      }

      DealWithCheckMSG();

}

/**
  * @brief  switch(MSG_TYPE(Msg))��ش���.
  * @param  None
  * @retval None
  */
 void DealWithCheckMSG(void)
{
  static Uint8 i=0,j=0;

    MSG_Count ++;
    if(MSG_Count >= 1)
    {
          MSG_Count = 0;
          MSG_GET_MSG(&Msg);
          switch(MSG_TYPE(Msg))
          {
          case MSG_ADC:
//            Sample_ATT();  //�ɼ���֮��˳����ж�һ�µ�ѹ�ȼ�
//            if( VCtrlVar.KAction_State == KAction_Idel)  //ֻ�����м̵���Ͷ�н��������ж���һ�ε�λ
//			  {
//				VCtrlVar.LastState = Level_Calculate(SysParameterVar_A.Voltage_Input,SysParameterVar_B.Voltage_Input);
//				VAdjustment_Algorithm();//�жϵ�ѹ���ڵĵ�λ
//			  }
//            VSysStateFlagVar.OverCurrent = Check_Err(SetParaVar.KVA_Amount,SysParameterVar_B.Current_Input);//�жϴ���������
            break;

          case MSG_CMD:
//        	if(((UartParameterVar.SlaveRS485_State&0x0F) == 0x03)||((UartParameterVar.SlaveRS485_State&0x0F) == 0x09)) //����֮ǰ���Ҫ��Ҫ���·�����ǰ��֡
//                    {
//                            UartParameterVar.RS485_TX_IsRepeat = 1;
//                    }
//        	else UartParameterVar.RS485_TX_IsRepeat = 0;
//
//                if(((UartParameterVar.SlaveRS485_State&0x0F) == 0x05)||((UartParameterVar.SlaveRS485_State&0x0F) == 0x0A))  //�Ѿ��ظ�
//                    {
//                        UartParameterVar.unack_cnt = 0;
//                        if(VCtrlVar.Level_IsChanged == 0x55)
//                        {
//                                KAction_Algorithm();   //��һ�η�ָ��
//                        }
//                    }
//                if(UartParameterVar.RS485_TX_IsRepeat == 1)
//                    {
//                      UartParameterVar.unack_cnt ++;
//                      if((Cmd_ToK_Last[3]  == 0xA9)&&(Cmd_ToK_Last[2]  == 0x42)&&(Cmd_ToK_Last[1]  == 0x48)) //�㲥֡��Ҫ�ظ�
//                      {
//                         UartParameterVar.RS485_TX_IsRepeat = 0;
//                         UartParameterVar.unack_cnt = 0;
//                         UartParameterVar.SlaveRS485_State = Uart_Idel;		//�ָ���־λ
//                       }
//                      else if(UartParameterVar.unack_cnt < 2)
//                      {
//                        USART_Send(USART1,Cmd_ToK_Last);  //�ط�ָ��
//                        UartParameterVar.RS485_TX_IsRepeat = 0;
//                      }
//                      else if(UartParameterVar.unack_cnt >1 )        //�ط�����1�Σ������ط�
//                      {
//                        UartParameterVar.unack_cnt = 0;
//                        UartParameterVar.SlaveRS485_State = Uart_Idel;		//�ָ���־λ
//                        UartParameterVar.RS485_TX_IsRepeat = 0;
//                      }
//                    }

            break;

          case MSG_AnalyseRX:
        	  //UartcomFlagVar.Receive_Flag = data_Analyse();
            break;

          case MSG_DISPLAY:
            break;

          case MSG_HRSENT:
            break;

          case  MSG_LCDDIE:
            break;
            //���� �ȴ�1s
          case  MSG_WAIT1S:
            EnWait1s = EnFlagTrue;
            Wait1s_Cnt = 0;
            MSG_StructVar.IsBusy = FlagTrue;
            break;

            //��������ι����
           default:
                  break;
          }//switch end
    }
}

/**
  * @brief  ��Main�н��е����MSG������.
  * @param  None
  * @retval None
  */
 void DoMsgInMain(void)
{


    ////////////////////////////////For MSG end///////////////////////////
}

/**
  * @brief  PF�쳣ʱ������ش���.
  * @param  None
  * @retval None
  */
 void DealWithAbnormalPF(void)
{}

/**
  * @brief  MSG�����쳣����MSG��ر������г�ʼ��.
  * @param  None
  * @retval None
  */
 void InitMSG(void)
{


}

/**
  * @brief  ���Msgջ��ȫ���̵���������Msg
  * @param  None
  * @retval None
  */
 void ClearMsgRelayAction(void)
{

}

/**
  * @brief  ���Msgջ��ȫ��Msg
  * @param  None
  * @retval None
  */
 void ClearAllMsg(void)
{
	if (MsgNum == 0) {
		//* pArg = 0x00;
		return;
	}

	while(MsgNum != 0)
	{
		//*pArg = MsgArray[MsgTail];
		MsgTail++;
		if (MsgTail >= MSG_ARRAY_SIZE) {
			MsgTail = 0;
		}
		MsgNum--;
	}
}



