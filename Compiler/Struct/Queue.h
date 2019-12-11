/*��ʽģ����еĶ�����ʵ��
20181221
*/

#pragma once
#include <cstdlib>
#include <cstdbool>
#include "Node.h"
#include "LinkList.h"


template<class T>
class Queue {
private:
	//---------------------------PRI���ݳ�Ա---------------------------
	int front;//��ͷ�±�
	int rear;//��β�±�
	int count;//Ԫ�ظ���
	//int size;//��������
	LinkList<T> list;//���д洢
	
	
	//---------------------------PRI������Ա---------------------------
	//��ʼ����ز���,�����캯����clear()ʹ��
	void Init();

	//---------------------------PUB������Ա---------------------------
public:
	//���캯������ʼ��س�Ա
	Queue();

	//��������
	~Queue();

	//���
	bool enqueue(const T &item);

	//����
	T dequeue();

	//�����Ƿ�Ϊ�գ��շ�����
	bool isEmpty();

	//���ض��г���
	int getLength();

	//��ն���
	void clear();

	//���ʶ���Ԫ��
	const T &getFront();

};

//��ʼ����ز���,�����캯����clear()ʹ��
template<class T>
inline void Queue<T>::Init()
{
	front = rear = count = 0;
}

//���캯������ʼ��س�Ա
template<class T>
inline Queue<T>::Queue()
{
	Init();
}

template<class T>
inline Queue<T>::~Queue()
{
	clear();
}

//���
template<class T>
inline bool Queue<T>::enqueue(const T & item)
{
	return list.insertRear(item);
}

//����
template<class T>
inline T Queue<T>::dequeue()
{
	return list.deleteFront();
}

//�����Ƿ�Ϊ�գ��շ�����
template<class T>
inline bool Queue<T>::isEmpty()
{
	return list.isEmpty();
}

//���ض��г���
template<class T>
inline int Queue<T>::getLength()
{
	return list.GetSize();
}

template<class T>
inline void Queue<T>::clear()
{
	list.clear();
	Init();
}

//���ʶ���Ԫ��
template<class T>
inline const T & Queue<T>::getFront()
{
	if (count == 0)
	{
		//���п�
		return T();
	}
	list.SetPos();//������ͷ
	return list.data();
	// TODO: �ڴ˴����� return ���
}
