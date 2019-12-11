/*链式链表的定义与实现
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
	//---------------------------PRI数据成员---------------------------
	Node<T> *front;//头指针
	Node<T>	*rear;//尾指针
	
	//由插入和删除操作更新
	Node<T> *prePtr;//当前指针的前驱
	Node<T> *curPtr;//当前指针
	
	int size;//表中元素个数
	int position;//当前元素在表中的位置序号 若为-1则表空 由SetPos使用

	//---------------------------PRI函数成员---------------------------
	//初始化相关成员
	void Init();//构造函数和clear()调用
	
	//生成新节点
	Node<T> * newNode(const T & item, Node<T> * ptrNext = NULL);

	//释放结点
	void freeNode(Node<T> * p);

	//将链表L复制到当前表
	//void copy(const LinkList<T> &L);
public:
	//---------------------------PUB函数成员---------------------------
	LinkList();
	LinkList(const LinkList<T> *L);
	virtual ~LinkList();
	//LinkList<T>& operator=(const LinkList<T>&L); //重载运算符=

	//返回链表中元素个数
	int GetSize() const;

	//判断链表是否为空，空返回真
	bool isEmpty()const;

	//设置游标位置(从0开始) 如果pos大于size则游标将移动到链表尾
	void SetPos(int pos = 0);

	//使游标移向下一个结点,若当前是尾节点 则游标不动
	void next();

	//判断游标是否到链表尾，到尾返回真
	bool EOL()const;

	//返回游标当前位置
	int currentPosition()const;

	//在表头插入结点
	bool insertFront(const T &item);

	//在表尾插入结点
	bool insertRear(const T &item);

	//在当前结点前边插入结点，链表为空则失败
	bool insertAt(const T &item);

	//在当前结点后边插入结点，链表为空则失败
	bool insertAfter(const T &item);

	//删除头节点  返回新的头结点
	T deleteFront();

	//删除当前结点，游标后移，链表为空则操作失败 如果当前结点是尾结点则游标移向头结点
	bool deleteCurrent();

	//返回对当前结点成员数据的引用
	T& data();

	//返回对当前结点的常引用
	//const T & data() cosnt;

	//清空链表 释放所有结点内存空间 被析构函数和operator=调用
	void clear();
};

//初始化相关成员
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

//删除当前结点，游标后移，链表为空则操作失败 如果当前结点是尾结点则游标移向头结点
template<class T>
inline bool LinkList<T>::deleteCurrent()
{
	Node<T> *tempPtr = NULL;
	if (size == 0)
	{
		return false;//链表为空
	}
	tempPtr = curPtr;//用临时变量保存要删除的结点
	
	//调整游标
	if (curPtr == front)
	{
		//当前游标在头结点
		front = curPtr = curPtr->NextNode();
		prePtr = NULL;
		//position不变
	}
	else if (curPtr==rear)
	{
		//当前游标在尾结点
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
		//只有一个结点删完之后 链表就为空
		position = -1;
	}*/

	size--;

	freeNode(tempPtr);
	return true;
}

//返回对当前结点成员数据的引用
template<class T>
inline T & LinkList<T>::data()
{
	return curPtr->data;// TODO: 在此处插入 return 语句
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
	Init();//初始化相关成员

}

//返回链表中元素个数
template<class T>
inline int LinkList<T>::GetSize() const
{
	return size;
}

//判断链表是否为空，空返回真
template<class T>
inline bool LinkList<T>::isEmpty() const
{
	//如果为空的话返回True
	return !size;
}

//设置游标位置(从0开始) 如果pos大于size则游标将移动到链表尾
template<class T>
inline void LinkList<T>::SetPos(int pos)
{
	//可以考虑重载成[]运算符
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
		//超出链表范围
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

//使游标移向下一个结点,若当前是尾节点 则游标不动
template<class T>
inline void LinkList<T>::next()
{
	/*这里存在一个问题，链表采用无头链表 初始化之后 游标指向NULLPtr 需要在插入结点的函数中增加判断条件*/
	if (curPtr == NULL)
	{
		//当前游标在尾节点 游标位置不动（or可以考虑使游标指向头结点）
		//curPtr == NULL;
		;

	}
	else
	{
		prePtr = curPtr;
		curPtr = curPtr->NextNode();//游标后移
		
		position++;
	}
}

//判断游标是否到链表尾，到尾返回真
template<class T>
inline bool LinkList<T>::EOL() const
{
	//判断游标是否到链表尾，到尾返回真
	return curPtr==NULL&&prePtr==rear;
}

//返回游标当前位置
template<class T>
inline int LinkList<T>::currentPosition() const
{
	return position;
}

//在表头插入结点
template<class T>
inline bool LinkList<T>::insertFront(const T & item)
{
	Node<T> *tempPtr = NULL;
	tempPtr = newNode(item, front);
	if (tempPtr == NULL)
	{
		return false;//error
	}
	front = tempPtr;//头指针前移
	size++;
	if (front->NextNode() == NULL)
	{
		//这表示是第一次插入的元素 ,之前链表为空 这里需要让游标指针（curPtr）指向表头
		curPtr = front;
		prePtr = NULL;
		position = 0;
		rear = front;
	}
	return true;
}

//在表尾插入结点
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
		//说明当前链表为空 这是第一次插入结点
		front = rear = tempPtr;
		curPtr = front;//因为是第一次插入节点所以这里让游标指向表头
		prePtr = NULL;
		position = 0;
	}
	else
	{
		//当前链表非空
		rear->next = tempPtr;//把新插入的结点挂接到链表尾部
		rear = tempPtr;//尾指针后移
	}
	size++;
	return true;
}

//在当前结点前边插入结点，链表为空则失败,插入后游标不变
template<class T>
inline bool LinkList<T>::insertAt(const T & item)
{
	Node<T> *tempPtr = NULL;
	if (curPtr == NULL)
	{
		//链表为空
		return false;
	}
	tempPtr = newNode(item, curPtr);
	if (tempPtr == NULL)
	{
		return false;//error
	}
	if (curPtr == front)
	{
		//当前游标在头结点
		prePtr = front = tempPtr;
	}
	else
	{
		//游标不在头结点
		prePtr->next = tempPtr;
		prePtr = prePtr->NextNode();
	}
	size++;
	return true;
}

//在当前结点后边插入结点，链表为空则失败 插入后游标不变
template<class T>
inline bool LinkList<T>::insertAfter(const T & item)
{
	//插入之后游标变不变？？？？ 以后再说吧
	Node<T>*tempPtr = NULL;
	if (curPtr == NULL)
	{
		return false;//链表为空
	}
	tempPtr = newNode(item, curPtr->next);//如果后边挂载结点时 使用函数IsertAfter()参数二 似乎没有多大用
	//因为在InsertAfter()有 把该节点与后边结点挂接的语句
	
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
		//保存要删除的头结点
		tempPtr = front;

		//头结点重定向
		if (curPtr == front )
		{
			//游标当前在头结点
			front = curPtr = curPtr->NextNode();
			prePtr = NULL;
			position = 0;
		}
		else if (prePtr == front)
		{
			//游标当前在第二结点
			front =front->NextNode();
			prePtr = NULL;
			position = 0;
		}
		else
		{
			//其他位置
			front = front->NextNode();
			position--;
		}
		
		freeNode(tempPtr);
	}
	}

	size--;
	return t;
}

