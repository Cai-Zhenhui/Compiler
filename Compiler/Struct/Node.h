#pragma once
#include <cstdlib>
//结点的定义

template <class T>
class Node
{
private:
	
public:
	//成员数据
	T data;//数据域
	Node<T>* next;//指针域

	//成员函数
	
	Node(const T & t_data, Node<T>* t_next = NULL);//构造函数
	virtual ~Node();//析构函数
	void InsertAfter(Node<T> *p);//在该节点后边插入新节点
	Node<T>* DeleteAfter();//删除当前节点 返回其地址
	Node<T>* NextNode();//获取后继结点
	const Node<T>* NextNode()const;//获取后继结点

};

//构造函数
template<class T>
inline Node<T>::Node(const T & t_data, Node<T>* t_next) :data(t_data), next(t_next)
{
	;
}

//析构函数
template<class T>
inline Node<T>::~Node()
{
	;
}

//在该节点后边插入新节点
template<class T>
inline void Node<T>::InsertAfter(Node<T>* p)
{
	p->next = next;
	next = p;
}

//删除当前节点 返回其地址
template<class T>
inline Node<T>* Node<T>::DeleteAfter()
{
	Node<T> * tempPtr = next;
	if (tempPtr == NULL)
	{
		return NULL;
	}
	next = tempPtr->next;
	return tempPtr;
}

//获取后继结点
template<class T>
inline Node<T>* Node<T>::NextNode()
{
	return next;
}

//获取后继结点
template<class T>
inline const Node<T>* Node<T>::NextNode() const
{
	return next;
}