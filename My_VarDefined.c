/*
 * My_VarDefined.c
 *
 *  Created on: 2016年1月28日
 *      Author: ZHL
 */
#include "DSP28x_Project.h"
#include "My_VarDefined.h"

UartcomFlagStruct UartcomFlagVar;
UartParameterStruct UartParameterVar;
SetParaStruct SetParaVar = {0xaa,3,30,55,0x01} ;
SysParameterStruct SysParameterVar=
{
  10,  //CT
  95,  //目标PF
  0,//当前功率因数 *100
  0, //输入电压
  0,   //输入电流
  0,  //输出电压
  0,  //输出电流
  0,  //无功功率 单位:KVAR
  0,    //有功功率 单位:KW
  0,  //无功功率 给DTU 单位:VAR
  0,    //有功功率 给DTU单位:W
  205,  //目标电压
  235, // 切除电压
  0,  //升压故障代码
  0,//电容故障代码
  0, //升压档数
  0, //电压提升百分比
  0,//母线电压
  1,//ModbusID
};
VCtrlStruct VCtrlVar;
SysParameterStruct SysParameterVar_A;
SysParameterStruct SysParameterVar_B;
SysParameterStruct SysParameterVar_C;
SysErroStateStruct CSysErroStateVar =
{
  0x33,	//过压  33表示没有故障，55表示有故障
  0x33,	//过流
  0x33,	//过温
  0x33,	//欠压
  0x33,	//开关故障
};
SysErroStateStruct VSysErroStateVar =
{
  0x33,	//过压  33表示没有故障，55表示有故障
  0x33,	//过流
  0x33,	//过温
  0x33,	//欠压
  0x55,	//开关故障
};
HarmonicStruct IAHarmonicVar;
HarmonicStruct ICHarmonicVar;
HarmonicStruct IBHarmonicVar =
{
  0,//总谐波 *100
  0, //3次谐波 乘以100
  0,//5次谐波 乘以100
  0,//7次谐波 乘以100
  0,//9次谐波 乘以100
  0,//11次谐波 乘以100
  0,//13次谐波 乘以100
  0,//15次谐波 乘以100
  0,//17次谐波 乘以100
};
UIBaseStruct UIBaseVar;
HarmonicStruct UAHarmonicVar;
HarmonicStruct UCHarmonicVar;
HarmonicStruct UBHarmonicVar =
{
  0,//总谐波 *100
  0, //3次谐波 乘以10
  0,
  0,
  0,
  0,
  0,
  0,
  0,
};
CParameterStruct CParameterVar =
{
  0x0,//C1 状态0x33没投  0x53已投
  0x0,
  0x0,
  0,//C1容量 *100
  0,
  0,
  0,//总的补偿容量
};
TimeFlagStruct TimeFlagVar =
{
0,
0,
1,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
};

MSG_Struct MSG_StructVar;


