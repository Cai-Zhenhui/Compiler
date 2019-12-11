/*��ʽ����Ķ�����ʵ��
20181221
*/

#pragma once

#include "Node.h"
#include <cstdlib>
#include <cstdbool>

template <class T>
class LinkList
{
private:
	//---------------------------PRI���ݳ�Ա---------------------------
	Node<T> *front;//ͷָ��
	Node<T>	*rear;//βָ��
	
	//�ɲ����ɾ����������
	Node<T> *prePtr;//��ǰָ���ǰ��
	Node<T> *curPtr;//��ǰָ��
	
	int size;//����Ԫ�ظ���
	int position;//��ǰԪ���ڱ��е�λ����� ��Ϊ-1���� ��SetPosʹ��

	//---------------------------PRI������Ա---------------------------
	//��ʼ����س�Ա
	void Init();//���캯����clear()����
	
	//�����½ڵ�
	Node<T> * newNode(const T & item, Node<T> * ptrNext = NULL);

	//�ͷŽ��
	void freeNode(Node<T> * p);

	//������L���Ƶ���ǰ��
	//void copy(const LinkList<T> &L);
public:
	//---------------------------PUB������Ա---------------------------
	LinkList();
	LinkList(const LinkList<T> *L);
	virtual ~LinkList();
	//LinkList<T>& operator=(const LinkList<T>&L); //���������=

	//����������Ԫ�ظ���
	int GetSize() const;

	//�ж������Ƿ�Ϊ�գ��շ�����
	bool isEmpty()const;

	//�����α�λ��(��0��ʼ) ���pos����size���α꽫�ƶ�������β
	void SetPos(int pos = 0);

	//ʹ�α�������һ�����,����ǰ��β�ڵ� ���α겻��
	void next();

	//�ж��α��Ƿ�����β����β������
	bool EOL()const;

	//�����α굱ǰλ��
	int currentPosition()const;

	//�ڱ�ͷ������
	bool insertFront(const T &item);

	//�ڱ�β������
	bool insertRear(const T &item);

	//�ڵ�ǰ���ǰ�߲����㣬����Ϊ����ʧ��
	bool insertAt(const T &item);

	//�ڵ�ǰ����߲����㣬����Ϊ����ʧ��
	bool insertAfter(const T &item);

	//ɾ��ͷ�ڵ�  �����µ�ͷ���
	T deleteFront();

	//ɾ����ǰ��㣬�α���ƣ�����Ϊ�������ʧ�� �����ǰ�����β������α�����ͷ���
	bool deleteCurrent();

	//���ضԵ�ǰ����Ա���ݵ�����
	T& data();

	//���ضԵ�ǰ���ĳ�����
	//const T & data() cosnt;

	//������� �ͷ����н���ڴ�ռ� ������������operator=����
	void clear();
};

//��ʼ����س�Ա
template<class T>
inline void LinkList<T>::Init()
{
	//this->front = this->rear = this->curPtr = this->prePtr = NULL;
	front = rear = curPtr = prePtr = NULL;
	position = -1;
	size = 0;
}

template<class T>
inline Node<T>* LinkList<T>::newNode(const T & item, Node<T>* ptrNext)
{
	return new Node<T>(item, ptrNext);
}

template<class T>
inline void LinkList<T>::freeNode(Node<T>* p)
{
	delete p;
}

template<class T>
inline LinkList<T>::LinkList()
{
	Init();

}

template<class T>
inline LinkList<T>::LinkList(const LinkList<T>* L)
{
	front = L->front;
	rear = L->rear;
	//prePtr = L->prePtr;
	curPtr = L->curPtr;
	size = L->size;
	position = L->position;
}

template<class T>
inline LinkList<T>::~LinkList()
{
	clear();
}

//ɾ����ǰ��㣬�α���ƣ�����Ϊ�������ʧ�� �����ǰ�����β������α�����ͷ���
template<class T>
inline bool LinkList<T>::deleteCurrent()
{
	Node<T> *tempPtr = NULL;
	if (size == 0)
	{
		return false;//����Ϊ��
	}
	tempPtr = curPtr;//����ʱ��������Ҫɾ���Ľ��
	
	//�����α�
	if (curPtr == front)
	{
		//��ǰ�α���ͷ���
		front = curPtr = curPtr->NextNode();
		prePtr = NULL;
		//position����
	}
	else if (curPtr==rear)
	{
		//��ǰ�α���β���
		rear = prePtr;
		rear->next = NULL;
		prePtr = NULL;
		position = 0;
	}
	else
	{
		prePtr->next = curPtr->next;
		curPtr = curPtr->next;
	}
	
	/*if (size == 1)
	{
		//ֻ��һ�����ɾ��֮�� �����Ϊ��
		position = -1;
	}*/

	size--;

	freeNode(tempPtr);
	return true;
}

//���ضԵ�ǰ����Ա���ݵ�����
template<class T>
inline T & LinkList<T>::data()
{
	return curPtr->data;// TODO: �ڴ˴����� return ���
}

template<class T>
inline void LinkList<T>::clear()
{
	Node<T> *t = NULL, *p = NULL;
	if (front == NULL)
	{
		return;
	}
	for (p = front; p!= NULL; )
	{
		t = p;
		p = p->NextNode();
		delete t;
	}
	Init();//��ʼ����س�Ա

}

//����������Ԫ�ظ���
template<class T>
inline int LinkList<T>::GetSize() const
{
	return size;
}

//�ж������Ƿ�Ϊ�գ��շ�����
template<class T>
inline bool LinkList<T>::isEmpty() const
{
	//���Ϊ�յĻ�����True
	return !size;
}

//�����α�λ��(��0��ʼ) ���pos����size���α꽫�ƶ�������β
template<class T>
inline void LinkList<T>::SetPos(int pos)
{
	//���Կ������س�[]�����
	int i = 0;
	Node<T> *tempPtr = NULL;
	Node<T> *p=NULL, *q = NULL;

	for (i = 0, p = front, q = NULL; i < (size - 1) && i < pos; i++)
	{
		q = p;
		p = p->NextNode();
	}
	curPtr = p;
	prePtr = q;
	position = pos;
	/*
	if (pos >= size)
	{
		//��������Χ
		pos = size - 1;
		curPtr = rear;
	}
	else
	{
		for (i = 0, tempPtr == front; i < pos; i++)
		{
			tempPtr = tempPtr->next;
		}
		curPtr = tempPtr;
		position = pos;
	}*/
}

//ʹ�α�������һ�����,����ǰ��β�ڵ� ���α겻��
template<class T>
inline void LinkList<T>::next()
{
	/*�������һ�����⣬���������ͷ���� ��ʼ��֮�� �α�ָ��NULLPtr ��Ҫ�ڲ�����ĺ����������ж�����*/
	if (curPtr == NULL)
	{
		//��ǰ�α���β�ڵ� �α�λ�ò�����or���Կ���ʹ�α�ָ��ͷ��㣩
		//curPtr == NULL;
		;

	}
	else
	{
		prePtr = curPtr;
		curPtr = curPtr->NextNode();//�α����
		
		position++;
	}
}

//�ж��α��Ƿ�����β����β������
template<class T>
inline bool LinkList<T>::EOL() const
{
	//�ж��α��Ƿ�����β����β������
	return curPtr==NULL&&prePtr==rear;
}

//�����α굱ǰλ��
template<class T>
inline int LinkList<T>::currentPosition() const
{
	return position;
}

//�ڱ�ͷ������
template<class T>
inline bool LinkList<T>::insertFront(const T & item)
{
	Node<T> *tempPtr = NULL;
	tempPtr = newNode(item, front);
	if (tempPtr == NULL)
	{
		return false;//error
	}
	front = tempPtr;//ͷָ��ǰ��
	size++;
	if (front->NextNode() == NULL)
	{
		//���ʾ�ǵ�һ�β����Ԫ�� ,֮ǰ����Ϊ�� ������Ҫ���α�ָ�루curPtr��ָ���ͷ
		curPtr = front;
		prePtr = NULL;
		position = 0;
		rear = front;
	}
	return true;
}

//�ڱ�β������
template<class T>
inline bool LinkList<T>::insertRear(const T & item)
{
	Node<T> *tempPtr = NULL;
	tempPtr = newNode(item, NULL);
	if (tempPtr == NULL)
	{
		return false;//error
	}
	if (size==0)
	{
		//˵����ǰ����Ϊ�� ���ǵ�һ�β�����
		front = rear = tempPtr;
		curPtr = front;//��Ϊ�ǵ�һ�β���ڵ������������α�ָ���ͷ
		prePtr = NULL;
		position = 0;
	}
	else
	{
		//��ǰ����ǿ�
		rear->next = tempPtr;//���²���Ľ��ҽӵ�����β��
		rear = tempPtr;//βָ�����
	}
	size++;
	return true;
}

//�ڵ�ǰ���ǰ�߲����㣬����Ϊ����ʧ��,������α겻��
template<class T>
inline bool LinkList<T>::insertAt(const T & item)
{
	Node<T> *tempPtr = NULL;
	if (curPtr == NULL)
	{
		//����Ϊ��
		return false;
	}
	tempPtr = newNode(item, curPtr);
	if (tempPtr == NULL)
	{
		return false;//error
	}
	if (curPtr == front)
	{
		//��ǰ�α���ͷ���
		prePtr = front = tempPtr;
	}
	else
	{
		//�α겻��ͷ���
		prePtr->next = tempPtr;
		prePtr = prePtr->NextNode();
	}
	size++;
	return true;
}

//�ڵ�ǰ����߲����㣬����Ϊ����ʧ�� ������α겻��
template<class T>
inline bool LinkList<T>::insertAfter(const T & item)
{
	//����֮���α�䲻�䣿������ �Ժ���˵��
	Node<T>*tempPtr = NULL;
	if (curPtr == NULL)
	{
		return false;//����Ϊ��
	}
	tempPtr = newNode(item, curPtr->next);//�����߹��ؽ��ʱ ʹ�ú���IsertAfter()������ �ƺ�û�ж����
	//��Ϊ��InsertAfter()�� �Ѹýڵ����߽��ҽӵ����
	
	if (tempPtr == NULL)
	{
		return false;//error
	}
	curPtr->InsertAfter(tempPtr);
	size++;
	return true;
}

template<class T>
inline T LinkList<T>::deleteFront()
{
	Node<T> *tempPtr = NULL;
	T t =front->data;
	switch (size)
	{
	case 0:return T(0);
	case 1:
	{
		freeNode(front);
		LinkList();
		break;
	}
	default:
	{
		//����Ҫɾ����ͷ���
		tempPtr = front;

		//ͷ����ض���
		if (curPtr == front )
		{
			//�α굱ǰ��ͷ���
			front = curPtr = curPtr->NextNode();
			prePtr = NULL;
			position = 0;
		}
		else if (prePtr == front)
		{
			//�α굱ǰ�ڵڶ����
			front =front->NextNode();
			prePtr = NULL;
			position = 0;
		}
		else
		{
			//����λ��
			front = front->NextNode();
			position--;
		}
		
		freeNode(tempPtr);
	}
	}

	size--;
	return t;
}

