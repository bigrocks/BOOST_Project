/*
 * HMICMD_Queue.h
 *
 *  Created on: 2016��1��28��
 *      Author: ZHL
 */

#ifndef HMICMD_QUEUE_H_
#define HMICMD_QUEUE_H_

#include "HMI_Driver.h"

//#pragma pack(pop)

typedef unsigned char qdata;
typedef unsigned short qsize;

/*!
 *  \brief  ���ָ������
 */
extern void queue_reset(void);

/*!
 * \brief  ���ָ������
 * \detial ���ڽ��յ����ݣ�ͨ���˺�������ָ�����
 *  \param  _data ָ������
 */
extern void queue_push(qdata _data);

/*!
 *  \brief  ��ָ�������ȡ��һ��������ָ��
 *  \param  cmd ָ����ջ�����
 *  \param  buf_len ָ����ջ�������С
 *  \return  ָ��ȣ�0��ʾ������������ָ��
 */
extern qsize queue_find_cmd(qdata *cmd,qsize buf_len);

#endif /* HMICMD_QUEUE_H_ */
