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
  * @brief	用来压入一系列测量自己电容量的命令
  * @param  Uint16 CapType:电容充号,1或2
  * @retval None
  */
 e_Flag msg_sendCap_msg(Uint16 CapType)
{}

/**
  * @brief	只压栈，不检查栈中是否有重复内容
  * @param  Uint16 MsgType 任务类型
  * @param  Uint16 MsgArg 任务所需数据值
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
        //yu add 2014.03.15 返回值:0--有 0x11--没有
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
        //yu add 2014.03.15 返回值:0--有 0x11--没有
        //if( msg_check_msg(MsgType, MsgArg)==0x11 )
        {
        	{//优先压入投切命令
        		if(MsgTail>=1)
        			MsgTail--;
        		else//MsgTail=0;
        			MsgTail = MSG_ARRAY_SIZE - 1;

        		//优先压入投切命令
        		  ((Uint16 *)(&(MsgArray[ MsgTail])))[1] = MsgType;
        		  ((Uint16 *)(&(MsgArray[ MsgTail])))[0] = MsgArg;

        		  MsgNum++;
        	}
        }
}


/**
  * @brief  检测MSG栈中有没有相同的MSG
  * @param  None
  * @retval  返回值:0--有 0x11--没有
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
            return 0;//有相同的MSG

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
  * @brief  定时发送MSG的相关代码.
  * @param  None
  * @retval None
  */
 void DealWithMSG(void)
{

	  //每隔20ms计量芯片采集一次,并计算电压档位
      if((TimeFlagVar.Time20msFlag == 1))  //
      {
         MSG_SEND_MSG(MSG_ADC, 0);
      }

//          2.6S发控制命令一次
      if((TimeFlagVar.Time1sFlag%5 == 4)&&(TimeFlagVar.Time20msFlag == 31))
      {
        MSG_SEND_MSG(MSG_CMD, 0);

      }

      if((TimeFlagVar.Time20msFlag == 22))
      {
         MSG_SEND_MSG(MSG_AnalyseRX, 0);
      }
        //每隔500ms刷新屏幕一次
      if((TimeFlagVar.Time20msFlag == 25))
      {
        if(KeyResult == 0)
        {
          MSG_SEND_MSG(MSG_DISPLAY, 0);
        }
      }

        //每1s发一次心跳包
      if( (TimeFlagVar.Time1sFlag%2 == 1)&&(TimeFlagVar.Time20msFlag == 49))
      {
        MSG_SEND_MSG(MSG_HRSENT, 0);
      }

      if( ((TimeFlagVar.Time1minFlag)%3 == 2) &&(TimeFlagVar.Time1sFlag == 2))  //2分钟后熄灭LCD
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
  * @brief  switch(MSG_TYPE(Msg))相关代码.
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
//            Sample_ATT();  //采集，之后顺便就判断一下电压等级
//            if( VCtrlVar.KAction_State == KAction_Idel)  //只有所有继电器投切结束后再判断下一次档位
//			  {
//				VCtrlVar.LastState = Level_Calculate(SysParameterVar_A.Voltage_Input,SysParameterVar_B.Voltage_Input);
//				VAdjustment_Algorithm();//判断电压调节的档位
//			  }
//            VSysStateFlagVar.OverCurrent = Check_Err(SetParaVar.KVA_Amount,SysParameterVar_B.Current_Input);//判断大于容量了
            break;

          case MSG_CMD:
//        	if(((UartParameterVar.SlaveRS485_State&0x0F) == 0x03)||((UartParameterVar.SlaveRS485_State&0x0F) == 0x09)) //发送之前检查要不要重新发送以前的帧
//                    {
//                            UartParameterVar.RS485_TX_IsRepeat = 1;
//                    }
//        	else UartParameterVar.RS485_TX_IsRepeat = 0;
//
//                if(((UartParameterVar.SlaveRS485_State&0x0F) == 0x05)||((UartParameterVar.SlaveRS485_State&0x0F) == 0x0A))  //已经回复
//                    {
//                        UartParameterVar.unack_cnt = 0;
//                        if(VCtrlVar.Level_IsChanged == 0x55)
//                        {
//                                KAction_Algorithm();   //第一次发指令
//                        }
//                    }
//                if(UartParameterVar.RS485_TX_IsRepeat == 1)
//                    {
//                      UartParameterVar.unack_cnt ++;
//                      if((Cmd_ToK_Last[3]  == 0xA9)&&(Cmd_ToK_Last[2]  == 0x42)&&(Cmd_ToK_Last[1]  == 0x48)) //广播帧不要回复
//                      {
//                         UartParameterVar.RS485_TX_IsRepeat = 0;
//                         UartParameterVar.unack_cnt = 0;
//                         UartParameterVar.SlaveRS485_State = Uart_Idel;		//恢复标志位
//                       }
//                      else if(UartParameterVar.unack_cnt < 2)
//                      {
//                        USART_Send(USART1,Cmd_ToK_Last);  //重发指令
//                        UartParameterVar.RS485_TX_IsRepeat = 0;
//                      }
//                      else if(UartParameterVar.unack_cnt >1 )        //重发次数1次，不再重发
//                      {
//                        UartParameterVar.unack_cnt = 0;
//                        UartParameterVar.SlaveRS485_State = Uart_Idel;		//恢复标志位
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
            //测试 等待1s
          case  MSG_WAIT1S:
            EnWait1s = EnFlagTrue;
            Wait1s_Cnt = 0;
            MSG_StructVar.IsBusy = FlagTrue;
            break;

            //建议用于喂狗等
           default:
                  break;
          }//switch end
    }
}

/**
  * @brief  在Main中进行的相关MSG任务处理.
  * @param  None
  * @retval None
  */
 void DoMsgInMain(void)
{


    ////////////////////////////////For MSG end///////////////////////////
}

/**
  * @brief  PF异常时，的相关处理.
  * @param  None
  * @retval None
  */
 void DealWithAbnormalPF(void)
{}

/**
  * @brief  MSG出现异常，对MSG相关变量进行初始化.
  * @param  None
  * @retval None
  */
 void InitMSG(void)
{


}

/**
  * @brief  清除Msg栈中全部继电器动作的Msg
  * @param  None
  * @retval None
  */
 void ClearMsgRelayAction(void)
{

}

/**
  * @brief  清除Msg栈中全部Msg
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



