#pragma once
#include <stdio.h>
#include <stdlib.h>

//单链队列的实现

typedef int QElemType;

typedef struct QNode {
	QElemType data;
	struct QNode *next;
}QNode;

typedef struct {
	QNode * front;//队头指针
	QNode * rear;//队尾指针
}LinkQueue;

//操作申明--------------------------------

//初始化一个队列。成功返回1，失败返回-1
int InitQueue_L(LinkQueue &Q);

//向队列中插入元素。
int InsertQueue_L(LinkQueue &Q, QElemType e);

//队列删除元素
QElemType DeleteQueue_L(LinkQueue Q);

//判断队列是否为空，空返回1,非空返回0
int isEmptyQueue_L(LinkQueue Q);

//获取队列长度
int GetLengthQueue_L(LinkQueue Q);

//遍历队列
void VisitQueue_L(LinkQueue Q);

//Clear queue
int ClearQueue_L(LinkQueue Q);