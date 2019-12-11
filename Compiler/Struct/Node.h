#pragma once
#include <cstdlib>
//���Ķ���

template <class T>
class Node
{
private:
	
public:
	//��Ա����
	T data;//������
	Node<T>* next;//ָ����

	//��Ա����
	
	Node(const T & t_data, Node<T>* t_next = NULL);//���캯��
	virtual ~Node();//��������
	void InsertAfter(Node<T> *p);//�ڸýڵ��߲����½ڵ�
	Node<T>* DeleteAfter();//ɾ����ǰ�ڵ� �������ַ
	Node<T>* NextNode();//��ȡ��̽��
	const Node<T>* NextNode()const;//��ȡ��̽��

};

//���캯��
template<class T>
inline Node<T>::Node(const T & t_data, Node<T>* t_next) :data(t_data), next(t_next)
{
	;
}

//��������
template<class T>
inline Node<T>::~Node()
{
	;
}

//�ڸýڵ��߲����½ڵ�
template<class T>
inline void Node<T>::InsertAfter(Node<T>* p)
{
	p->next = next;
	next = p;
}

//ɾ����ǰ�ڵ� �������ַ
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

//��ȡ��̽��
template<class T>
inline Node<T>* Node<T>::NextNode()
{
	return next;
}

//��ȡ��̽��
template<class T>
inline const Node<T>* Node<T>::NextNode() const
{
	return next;
}