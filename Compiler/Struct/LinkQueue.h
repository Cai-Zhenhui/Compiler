#pragma once
#include <stdio.h>
#include <stdlib.h>

//�������е�ʵ��

typedef int QElemType;

typedef struct QNode {
	QElemType data;
	struct QNode *next;
}QNode;

typedef struct {
	QNode * front;//��ͷָ��
	QNode * rear;//��βָ��
}LinkQueue;

//��������--------------------------------

//��ʼ��һ�����С��ɹ�����1��ʧ�ܷ���-1
int InitQueue_L(LinkQueue &Q);

//������в���Ԫ�ء�
int InsertQueue_L(LinkQueue &Q, QElemType e);

//����ɾ��Ԫ��
QElemType DeleteQueue_L(LinkQueue Q);

//�ж϶����Ƿ�Ϊ�գ��շ���1,�ǿշ���0
int isEmptyQueue_L(LinkQueue Q);

//��ȡ���г���
int GetLengthQueue_L(LinkQueue Q);

//��������
void VisitQueue_L(LinkQueue Q);

//Clear queue
int ClearQueue_L(LinkQueue Q);