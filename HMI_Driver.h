/*
 * HMI_Driver.h
 *
 *  Created on: 2016��1��28��
 *      Author: ZHL
 */

#ifndef HMI_DRIVER_H_
#define HMI_DRIVER_H_


#define CRC16_ENABLE 0         /*!< �����ҪCRC16У�鹦�ܣ��޸Ĵ˺�Ϊ1(��ʱ��Ҫ��VisualTFT��������CRCУ��)*/
#define CMD_MAX_SIZE 20        /*!<����ָ���С��������Ҫ�������������ô�һЩ*/
#define QUEUE_MAX_SIZE 80   /*!< ָ����ջ�������С��������Ҫ�������������ô�һЩ*/

#include "My_BSP.h"

/*!
 *  \brief  ��������Ƿ����CRC16У��
 *  \param buffer ��У������ݣ�ĩβ�洢CRC16
 *  \param n ���ݳ��ȣ�����CRC16
 *  \return У��ͨ������1�����򷵻�0
 */
Uint16 CheckCRC16(Uint8 *buffer,Uint16 n);

/*!
 *  \brief  ��ʱ
 *  \param  n ��ʱʱ��(���뵥λ)
 */
void DelayMS(unsigned int n);

/*!
 *  \brief  �����豸���ã�����֮����Ҫ�����������޸Ĳ����ʡ���������������������ʽ
 */
void LockDeviceConfig(void);

/*!
 *  \brief  �����豸����
 */
void UnlockDeviceConfig(void);

/*!
 *  \brief     �޸Ĵ������Ĳ�����
 *  \details  ������ѡ�Χ[0~14]����Ӧʵ�ʲ�����
                   {1200,2400,4800,9600,19200,38400,57600,115200,1000000,2000000,218750,437500,875000,921800,2500000}
 *  \param  option ������ѡ��
 */
void SetCommBps(Uint8 option);

/*!
 *  \brief  ������������
 */
void SetHandShake(void);

/*!
 *  \brief  ����ǰ��ɫ
 *  \param  color ǰ��ɫ
 */
void SetFcolor(Uint16 color);

/*!
 *  \brief  ���ñ���ɫ
 *  \param  color ����ɫ
 */
void SetBcolor(Uint16 color);

/*!
 *  \brief  �������
 */
void GUI_CleanScreen(void);

/*!
 *  \brief  �������ּ��
 *  \param  x_w ������
  *  \param  y_w ������
 */
void SetTextSpace(Uint8 x_w, Uint8 y_w);

/*!
 *  \brief  ����������ʾ����
 *  \param  enable �Ƿ���������
 *  \param  width ���
 *  \param  height �߶�
 */
void SetFont_Region(Uint8 enable,Uint16 width,Uint16 height );

/*!
 *  \brief  ���ù���ɫ
 *  \param  fillcolor_dwon ��ɫ�½�
 *  \param  fillcolor_up ��ɫ�Ͻ�
 */
void SetFilterColor(Uint16 fillcolor_dwon, Uint16 fillcolor_up);

/*!
 *  \brief  ���ù���ɫ
 *  \param  x λ��X����
 *  \param  y λ��Y����
 *  \param  back ��ɫ�Ͻ�
 *  \param  font ����
 *  \param  strings �ַ�������
 */
void DisText(Uint16 x, Uint16 y,Uint8 back,Uint8 font,char *strings );

/*!
 *  \brief    ��ʾ���
 *  \param  enable �Ƿ���ʾ
 *  \param  x λ��X����
 *  \param  y λ��Y����
 *  \param  width ���
 *  \param  height �߶�
 */
void DisCursor(Uint8 enable,Uint16 x, Uint16 y,Uint8 width,Uint8 height );

/*!
 *  \brief      ��ʾȫ��ͼƬ
 *  \param  image_id ͼƬ����
 *  \param  masken �Ƿ�����͸������
 */
void DisFull_Image(Uint16 image_id,Uint8 masken);

/*!
 *  \brief      ָ��λ����ʾͼƬ
 *  \param  x λ��X����
 *  \param  y λ��Y����
 *  \param  image_id ͼƬ����
 *  \param  masken �Ƿ�����͸������
 */
void DisArea_Image(Uint16 x,Uint16 y,Uint16 image_id,Uint8 masken);

/*!
 *  \brief      ��ʾ�ü�ͼƬ
 *  \param  x λ��X����
 *  \param  y λ��Y����
 *  \param  image_id ͼƬ����
 *  \param  image_x ͼƬ�ü�λ��X����
 *  \param  image_y ͼƬ�ü�λ��Y����
 *  \param  image_l ͼƬ�ü�����
 *  \param  image_w ͼƬ�ü��߶�
 *  \param  masken �Ƿ�����͸������
 */
void DisCut_Image(Uint16 x,Uint16 y,Uint16 image_id,Uint16 image_x,Uint16 image_y,
                   Uint16 image_l, Uint16 image_w,Uint8 masken);

/*!
 *  \brief      ��ʾGIF����
 *  \param  x λ��X����
 *  \param  y λ��Y����
 *  \param  flashimage_id ͼƬ����
 *  \param  enable �Ƿ���ʾ
 *  \param  playnum ���Ŵ���
 */
void DisFlashImage(Uint16 x,Uint16 y,Uint16 flashimage_id,Uint8 enable,Uint8 playnum);

/*!
 *  \brief      ����
 *  \param  x λ��X����
 *  \param  y λ��Y����
 */
void GUI_Dot(Uint16 x,Uint16 y);

/*!
 *  \brief      ����
 *  \param  x0 ��ʼλ��X����
 *  \param  y0 ��ʼλ��Y����
 *  \param  x1 ����λ��X����
 *  \param  y1 ����λ��Y����
 */
void GUI_Line(Uint16 x0, Uint16 y0, Uint16 x1, Uint16 y1);

/*!
 *  \brief      ������
 *  \param  mode ģʽ
 *  \param  dot ���ݵ�
 *  \param  dot_cnt ����
 */
void GUI_ConDots(Uint8 mode,Uint16 *dot,Uint16 dot_cnt);

/*!
 *  \brief      ������Բ
 *  \param  x0 Բ��λ��X����
 *  \param  y0 Բ��λ��Y����
 *  \param  r �뾶
 */
void GUI_Circle(Uint16 x0, Uint16 y0, Uint16 r);

/*!
 *  \brief      ��ʵ��Բ
 *  \param  x0 Բ��λ��X����
 *  \param  y0 Բ��λ��Y����
 *  \param  r �뾶
 */
void GUI_CircleFill(Uint16 x0, Uint16 y0, Uint16 r);

/*!
 *  \brief      ������
 *  \param  x0 Բ��λ��X����
 *  \param  y0 Բ��λ��Y����
 *  \param  r �뾶
 *  \param  sa ��ʼ�Ƕ�
 *  \param  ea ��ֹ�Ƕ�
 */
void GUI_Arc(Uint16 x,Uint16 y, Uint16 r,Uint16 sa, Uint16 ea);

/*!
 *  \brief      �����ľ���
 *  \param  x0 ��ʼλ��X����
 *  \param  y0 ��ʼλ��Y����
 *  \param  x1 ����λ��X����
 *  \param  y1 ����λ��Y����
 */
void GUI_Rectangle(Uint16 x0, Uint16 y0, Uint16 x1,Uint16 y1 );

/*!
 *  \brief      ��ʵ�ľ���
 *  \param  x0 ��ʼλ��X����
 *  \param  y0 ��ʼλ��Y����
 *  \param  x1 ����λ��X����
 *  \param  y1 ����λ��Y����
 */
void GUI_RectangleFill(Uint16 x0, Uint16 y0, Uint16 x1,Uint16 y1 );

/*!
 *  \brief      ��������Բ
 *  \param  x0 ��ʼλ��X����
 *  \param  y0 ��ʼλ��Y����
 *  \param  x1 ����λ��X����
 *  \param  y1 ����λ��Y����
 */
void GUI_Ellipse (Uint16 x0, Uint16 y0, Uint16 x1,Uint16 y1 );

/*!
 *  \brief      ��ʵ����Բ
 *  \param  x0 ��ʼλ��X����
 *  \param  y0 ��ʼλ��Y����
 *  \param  x1 ����λ��X����
 *  \param  y1 ����λ��Y����
 */
void GUI_EllipseFill (Uint16 x0, Uint16 y0, Uint16 x1,Uint16 y1 );

/*!
 *  \brief      ����
 *  \param  x0 ��ʼλ��X����
 *  \param  y0 ��ʼλ��Y����
 *  \param  x1 ����λ��X����
 *  \param  y1 ����λ��Y����
 */
void SetBackLight(Uint8 light_level);

/*!
 *  \brief   ����������
 *  \time  time ����ʱ��(���뵥λ)
 */
void SetBuzzer(Uint8 time);

/*!
 *  \brief   ����������
 *  \param enable ����ʹ��
 *  \param beep_on ����������
 *  \param work_mode ��������ģʽ��0���¾��ϴ���1�ɿ����ϴ���2�����ϴ�����ֵ��3���º��ɿ����ϴ�����
 *  \param press_calibration �������������20��У׼��������0���ã�1����
 */
void SetTouchPaneOption(Uint8 enbale,Uint8 beep_on,Uint8 work_mode,Uint8 press_calibration);

/*!
 *  \brief   У׼������
 */
void	CalibrateTouchPane(void);

/*!
 *  \brief  ����������
 */
void TestTouchPane(void);

/*!
 *  \brief      ���õ�ǰд��ͼ��
 *  \details  һ������ʵ��˫����Ч��(��ͼʱ������˸)��
 *  \details  Uint8 layer = 0;
 *  \details  WriteLayer(layer);    //����д���
 *  \details  ClearLayer(layer);    //ʹͼ���͸��
 *  \details  //���һϵ�л�ͼָ��
 *  \details  //DisText(100,100,0,4,"hello hmi!!!");
 *  \details  DisplyLayer(layer);  //�л���ʾ��
 *  \details  layer = (layer+1)%2;  //˫�����л�
 *  \see DisplyLayer
 *  \see ClearLayer
 *  \param  layer ͼ����
 */
void WriteLayer(Uint8 layer);

/*!
 *  \brief      ���õ�ǰ��ʾͼ��
 *  \param  layer ͼ����
 */
void DisplyLayer(Uint8 layer);

/*!
 *  \brief      ���ͼ�㣬ʹͼ����͸��
 *  \param  layer ͼ����
 */
void ClearLayer(Uint8 layer);

/*!
 *  \brief  д���ݵ��������û��洢��
 *  \param  startAddress ��ʼ��ַ
 *  \param  length �ֽ���
 *  \param  _data ��д�������
 */
void WriteUserFlash(Uint32 startAddress,Uint16 length,Uint8 *_data);

/*!
 *  \brief  �Ӵ������û��洢����ȡ����
 *  \param  startAddress ��ʼ��ַ
 *  \param  length �ֽ���
 */
void ReadUserFlash(Uint32 startAddress,Uint16 length);

/*!
 *  \brief      ����ͼ��
 *  \param  src_layer ԭʼͼ��
 *  \param  dest_layer Ŀ��ͼ��
 */
void CopyLayer(Uint8 src_layer,Uint8 dest_layer);

/*!
 *  \brief      ���õ�ǰ����
 *  \param  screen_id ����ID
 */
void SetScreen(Uint16 screen_id);

/*!
 *  \brief      ��ȡ��ǰ����
 */
void GetScreen(void);

/*!
 *  \brief     ����\���û������
 *  \details ����\����һ��ɶ�ʹ�ã����ڱ�����˸�����ˢ���ٶ�
 *  \details �÷���
 *	\details SetScreenUpdateEnable(0);//��ֹ����
 *	\details һϵ�и��»����ָ��
 *	\details SetScreenUpdateEnable(1);//��������
 *  \param  enable 0���ã�1����
 */
void SetScreenUpdateEnable(Uint8 enable);

/*!
 *  \brief     ���ÿؼ����뽹��
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 *  \param  focus �Ƿ�������뽹��
 */
void SetControlFocus(Uint16 screen_id,Uint16 control_id,Uint8 focus);

/*!
 *  \brief     ��ʾ\���ؿؼ�
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 *  \param  visible �Ƿ���ʾ
 */
void SetControlVisiable(Uint16 screen_id,Uint16 control_id,Uint8 visible);

/*!
 *  \brief     ���ô����ؼ�ʹ��
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 *  \param  enable �ؼ��Ƿ�ʹ��
 */
void SetControlEnable(Uint16 screen_id,Uint16 control_id,Uint8 enable);

/*!
 *  \brief     ��ȡ�ؼ�ֵ
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 */
void GetControlValue(Uint16 screen_id,Uint16 control_id);

/*!
 *  \brief     ���ð�ť״̬
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 *  \param  value ��ť״̬
 */
void SetButtonValue(Uint16 screen_id,Uint16 control_id,char value);

/*!
 *  \brief     �����ı�ֵ
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 *  \param  str �ı�ֵ
 */
void SetTextValue(Uint16 screen_id,Uint16 control_id,char *str);

/*!
 *  \brief      ���ý���ֵ
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void SetProgressValue(Uint16 screen_id,Uint16 control_id,Uint32 value);

/*!
 *  \brief     �����Ǳ�ֵ
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void SetMeterValue(Uint16 screen_id,Uint16 control_id,Uint32 value);

/*!
 *  \brief      ���û�����
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void SetSliderValue(Uint16 screen_id,Uint16 control_id,Uint32 value);

/*!
 *  \brief      ����ѡ��ؼ�
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 *  \param  item ��ǰѡ��
 */
void SetSelectorValue(Uint16 screen_id,Uint16 control_id,Uint8 item);

/*!
 *  \brief      ��ʼ���Ŷ���
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 */
void AnimationStart(Uint16 screen_id,Uint16 control_id);

/*!
 *  \brief      ֹͣ���Ŷ���
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 */
void AnimationStop(Uint16 screen_id,Uint16 control_id);

/*!
 *  \brief      ��ͣ���Ŷ���
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 */
void AnimationPause(Uint16 screen_id,Uint16 control_id);

/*!
 *  \brief     �����ƶ�֡
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 *  \param  frame_id ֡ID
 */
void AnimationPlayFrame(Uint16 screen_id,Uint16 control_id,Uint8 frame_id);

/*!
 *  \brief     ������һ֡
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 */
void AnimationPlayPrev(Uint16 screen_id,Uint16 control_id);

/*!
 *  \brief     ������һ֡
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 */
void AnimationPlayNext(Uint16 screen_id,Uint16 control_id);

/*!
 *  \brief     ���߿ؼ�-���ͨ��
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 *  \param  channel ͨ����
 *  \param  color ��ɫ
 */
void GraphChannelAdd(Uint16 screen_id,Uint16 control_id,Uint8 channel,Uint16 color);

/*!
 *  \brief     ���߿ؼ�-ɾ��ͨ��
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 *  \param  channel ͨ����
 */
void GraphChannelDel(Uint16 screen_id,Uint16 control_id,Uint8 channel);

/*!
 *  \brief     ���߿ؼ�-�������
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 *  \param  channel ͨ����
 *  \param  pData ��������
 *  \param  nDataLen ���ݸ���
 */
void GraphChannelDataAdd(Uint16 screen_id,Uint16 control_id,Uint8 channel,Uint8 *pData,Uint16 nDataLen);

/*!
 *  \brief     ���߿ؼ�-�������
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 *  \param  channel ͨ����
 */
void GraphChannelDataClear(Uint16 screen_id,Uint16 control_id,Uint8 channel);

/*!
 *  \brief     ���߿ؼ�-������ͼ����
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 *  \param  x_offset ˮƽƫ��
 *  \param  x_mul ˮƽ����ϵ��
 *  \param  y_offset ��ֱƫ��
 *  \param  y_mul ��ֱ����ϵ��
 */
void GraphSetViewport(Uint16 screen_id,Uint16 control_id,int16 x_offset,Uint16 x_mul,int16 y_offset,Uint16 y_mul);

/*!
 *  \brief     ��ʼ��������
 *  \param  screen_id ����ID
 */
void BatchBegin(Uint16 screen_id);

/*!
 *  \brief     �������°�ť�ؼ�
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void BatchSetButtonValue(Uint16 control_id,Uint8 state);

/*!
 *  \brief     �������½������ؼ�
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void BatchSetProgressValue(Uint16 control_id,Uint32 value);

/*!
 *  \brief     �������»������ؼ�
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void BatchSetSliderValue(Uint16 control_id,Uint32 value);

/*!
 *  \brief     ���������Ǳ�ؼ�
 *  \param  control_id �ؼ�ID
 *  \param  value ��ֵ
 */
void BatchSetMeterValue(Uint16 control_id,Uint32 value);

/*!
 *  \brief     ���������ı��ؼ�
 *  \param  control_id �ؼ�ID
 *  \param  strings �ַ���
 */
void BatchSetText(Uint16 control_id,char *strings);

/*!
 *  \brief     �������¶���\ͼ��ؼ�
 *  \param  control_id �ؼ�ID
 *  \param  frame_id ֡ID
 */
void BatchSetFrame(Uint16 control_id,Uint16 frame_id);

/*!
 *  \brief    ������������
 */
void BatchEnd(void);

/*!
 *  \brief     ���õ���ʱ�ؼ�
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 *  \param  timeout ����ʱ(��)
 */
void SeTimer(Uint16 screen_id,Uint16 control_id,Uint32 timeout);

/*!
 *  \brief     ��������ʱ�ؼ�
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 */
void StartTimer(Uint16 screen_id,Uint16 control_id);

/*!
 *  \brief     ֹͣ����ʱ�ؼ�
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 */
void StopTimer(Uint16 screen_id,Uint16 control_id);

/*!
 *  \brief     ��ͣ����ʱ�ؼ�
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 */
void PauseTimer(Uint16 screen_id,Uint16 control_id);

/*!
 *  \brief     ���ÿؼ�����ɫ
 *  \details  ֧�ֿؼ������������ı�
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 *  \param  color ����ɫ
 */
void SetControlBackColor(Uint16 screen_id,Uint16 control_id,Uint16 color);

/*!
 *  \brief     ���ÿؼ�ǰ��ɫ
  * \details  ֧�ֿؼ���������
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 *  \param  color ǰ��ɫ
 */
void SetControlForeColor(Uint16 screen_id,Uint16 control_id,Uint16 color);

/*!
 *  \brief     ��ʾ\���ص����˵��ؼ�
 *  \param  screen_id ����ID
 *  \param  control_id �ؼ�ID
 *  \param  show �Ƿ���ʾ��Ϊ0ʱfocus_control_id��Ч
 *  \param  focus_control_id �������ı��ؼ�(�˵��ؼ�������������ı��ؼ�)
 */
void ShowPopupMenu(Uint16 screen_id,Uint16 control_id,Uint8 show,Uint16 focus_control_id);

/*!
 *  \brief     ��ʾ\����ϵͳ����
 *  \param  show 0���أ�1��ʾ
 *  \param  x ������ʾλ��X����
 *  \param  y ������ʾλ��Y����
 *  \param  type 0С���̣�1ȫ����
 *  \param  option 0�����ַ���1���룬2ʱ������
 *  \param  max_len ����¼���ַ���������
 */
void ShowKeyboard(Uint8 show,Uint16 x,Uint16 y,Uint8 type,Uint8 option,Uint8 max_len);

#endif /* HMI_DRIVER_H_ */
