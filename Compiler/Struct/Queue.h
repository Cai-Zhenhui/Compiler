/*链式模板队列的定义与实现
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
	//---------------------------PRI数据成员---------------------------
	int front;//队头下标
	int rear;//队尾下标
	int count;//元素个数
	//int size;//队列容量
	LinkList<T> list;//队列存储
	
	
	//---------------------------PRI函数成员---------------------------
	//初始化相关参数,供构造函数和clear()使用
	void Init();

	//---------------------------PUB函数成员---------------------------
public:
	//构造函数，初始相关成员
	Queue();

	//析构函数
	~Queue();

	//入队
	bool enqueue(const T &item);

	//出队
	T dequeue();

	//队列是否为空，空返回真
	bool isEmpty();

	//返回队列长度
	int getLength();

	//清空队列
	void clear();

	//访问队首元素
	const T &getFront();

};

//初始化相关参数,供构造函数和clear()使用
template<class T>
inline void Queue<T>::Init()
{
	front = rear = count = 0;
}

//构造函数，初始相关成员
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

//入队
template<class T>
inline bool Queue<T>::enqueue(const T & item)
{
	return list.insertRear(item);
}

//出队
template<class T>
inline T Queue<T>::dequeue()
{
	return list.deleteFront();
}

//队列是否为空，空返回真
template<class T>
inline bool Queue<T>::isEmpty()
{
	return list.isEmpty();
}

//返回队列长度
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

//访问队首元素
template<class T>
inline const T & Queue<T>::getFront()
{
	if (count == 0)
	{
		//队列空
		return T();
	}
	list.SetPos();//到链表头
	return list.data();
	// TODO: 在此处插入 return 语句
}
