/*
 * My_VarDefined.c
 *
 *  Created on: 2016��1��28��
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
  95,  //Ŀ��PF
  0,//��ǰ�������� *100
  0, //�����ѹ
  0,   //�������
  0,  //�����ѹ
  0,  //�������
  0,  //�޹����� ��λ:KVAR
  0,    //�й����� ��λ:KW
  0,  //�޹����� ��DTU ��λ:VAR
  0,    //�й����� ��DTU��λ:W
  205,  //Ŀ���ѹ
  235, // �г���ѹ
  0,  //��ѹ���ϴ���
  0,//���ݹ��ϴ���
  0, //��ѹ����
  0, //��ѹ�����ٷֱ�
  0,//ĸ�ߵ�ѹ
  1,//ModbusID
};
VCtrlStruct VCtrlVar;
SysParameterStruct SysParameterVar_A;
SysParameterStruct SysParameterVar_B;
SysParameterStruct SysParameterVar_C;
SysErroStateStruct CSysErroStateVar =
{
  0x33,	//��ѹ  33��ʾû�й��ϣ�55��ʾ�й���
  0x33,	//����
  0x33,	//����
  0x33,	//Ƿѹ
  0x33,	//���ع���
};
SysErroStateStruct VSysErroStateVar =
{
  0x33,	//��ѹ  33��ʾû�й��ϣ�55��ʾ�й���
  0x33,	//����
  0x33,	//����
  0x33,	//Ƿѹ
  0x55,	//���ع���
};
HarmonicStruct IAHarmonicVar;
HarmonicStruct ICHarmonicVar;
HarmonicStruct IBHarmonicVar =
{
  0,//��г�� *100
  0, //3��г�� ����100
  0,//5��г�� ����100
  0,//7��г�� ����100
  0,//9��г�� ����100
  0,//11��г�� ����100
  0,//13��г�� ����100
  0,//15��г�� ����100
  0,//17��г�� ����100
};
UIBaseStruct UIBaseVar;
HarmonicStruct UAHarmonicVar;
HarmonicStruct UCHarmonicVar;
HarmonicStruct UBHarmonicVar =
{
  0,//��г�� *100
  0, //3��г�� ����10
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
  0x0,//C1 ״̬0x33ûͶ  0x53��Ͷ
  0x0,
  0x0,
  0,//C1���� *100
  0,
  0,
  0,//�ܵĲ�������
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


