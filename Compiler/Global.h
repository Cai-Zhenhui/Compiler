#pragma once
#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <iostream>
#include <vector>
#include <string>



//����������Ƶ�ָ�붼��int�� ����Ҳ��ָ��ı���

//���ͱ�(���ű���ӱ�)�ı���� ���� ��
enum TVAL {
	Unknow_TVAL = -1,
	Integer=1,//i
	Real,//r
	Char,//c
	Bool,//b
	Array,//a
	Struct//d
};

//���ͱ�(���ű���ӱ�)�ı���
struct TAPE {
	//���� 
	TVAL type;

	/*
	1\������������ int real char bool ��ָ��
	2\��������array ָ�������
	3\�ṹ������Struct ָ��ṹ��
	*/
	int pointer;
};
typedef struct TAPE TAPE;

//�����(���ű���ӱ�)�ı���
struct AINF
{
	int LOW;//�����½� Ӧ���ò���
	int UP;//�����Ͻ�
	TVAL type;//������Ԫ�ص�����

	/*
	�ɷ����͵�������ռֵ��Ԫ�ĸ���(Ҳ�����ֽ���)
	=sizeof(type)*(UP-LOW)
	*/
	int length;
};
typedef struct AINF AINF;

//�ṹ��ı���
struct RINF
{
	//�ṹ����ÿһ�������
	std::vector<TAPE>TypeList;

	//�ṹ����ÿһ��Ĵ�С
	std::vector<unsigned int>SizeList;
};
typedef struct RINF RINF;

//������ı���
struct PFINF
{
	int level;
	int offset;
	int ParameterNumber;
	int entry;
	int parameter;
};
typedef struct PFINF PFINF;

//������ı���
//ֱ��ʹ��int/double

//���ȱ�ı���
struct LEN {
	int len;//��߸���ʵ����ȷ����
};

/*���ű��б��� ����
*/
enum SymbolKind
{
	Unknow_SK = -1,
	Constant=1,
	Function,
	Type,
	Domain,
	Variable,
	TempVariable,
	PointerParameter, //��ַ
	NormalParameter //��ֵ
};

//���ű� ����
struct Symbol
{
	std::string name;
	int type;//ָ�� ָ�����ͱ�list_TAPE�е�ĳ��λ��
	SymbolKind cat;//���� �ñ�ʶ����Դ�����е������ɫ
	int addr;//ָ�� ����type��ָ���������������ȱ��е�ĳ��λ��
	Symbol(int ttype = -1, SymbolKind tcat = SymbolKind::Unknow_SK, int taddr = -1) {
		type = ttype;
		cat = tcat;
		addr = taddr;
	}
	void print() {
		std::cout << name << ' ';
	}
};
typedef struct Symbol Symbol;
extern std::vector<Symbol> list_Symbol;//���ű�

struct Delimiter
{
	std::string name;

};

enum TokenType {//token ����
	Unknow = -1,
	Delimiter = 1,
	Keyword,
	Identifier,
	CharM,
	StringM,
	ConstantFloat,
	ConstantInt
};

extern std::vector <std::string> list_Delimiter;//����� ��̬����ļ���ȡ
extern std::vector <std::string> list_Keyword;//�ؼ��ֱ� ��̬����ļ���ȡ
extern std::vector <std::string> list_String;//�ַ���������
extern std::vector <std::string> list_Char;//�ַ�������
//extern std::vector <std::string> list_Identifier;//��ʶ���� ��̬���Դ����ʶ��
extern std::vector <int> list_int_Constant;//���ͳ����� ��̬���Դ����ʶ��
extern std::vector <double> list_float_Constant;//���ͳ����� ��̬���Դ����ʶ��

struct token {
	TokenType type;//token type
	int pos;//The location of the token in the corresponding table.
	token(TokenType ttype = TokenType::Unknow, int tpos = -1) {
		type = ttype;
		pos = tpos;
	}

	bool isEmpty() {
		return (pos < 0) || (type == TokenType::Unknow);
	}
	void printValue() {
		switch (type) {
		case Unknow: {
			break;
		}
		case Delimiter: {
			std::cout << list_Delimiter[pos];
			break;
		}
		case Keyword: {
			std::cout << list_Keyword[pos];
			break;
		}
		case Identifier: {
			std::cout << list_Symbol[pos].name;
			break;
		}
		case CharM: {
			std::cout << list_Char[pos];
			break;
		}
		case StringM: {
			std::cout << list_String[pos];
			break;
		}
		case ConstantFloat: {
			std::cout << list_float_Constant[pos];
			break;
		}
		case ConstantInt: {
			std::cout << list_int_Constant[pos];
			break;
		}
		default: {
			break;
		}
		}
	}
	void print() {
		std::cout << "(";
		switch (type) {
		case Delimiter: {
			std::cout << "P,";//���
			break;
		}
		case Keyword: {
			std::cout << "K,";//�ؼ���
			break;
		}
		case Identifier: {
			std::cout << "I,";//��ʶ��
			break;
		}
		case ConstantFloat: {
			std::cout << "CF,";//����float
			break;
		}
		case ConstantInt: {
			std::cout << "CI,";//����int
			break;
		}
		case CharM: {
			std::cout << "CC,";//����char
			break;
		}
		case StringM: {
			std::cout << "CS,";//����string
			break;
		}
		default: {
			break;
		}
		}
		std::cout << pos << ")\t";
	}
private:
	template <class T>
	void PRIprint(T ttype, int tpos) {
		;
	}
};
typedef struct token token;

extern std::vector <token> list_token;//token����

/*
{op,arg1,arg2,result}
result=arg1 op arg2
*/
struct QT
{
	token op;
	token arg1;
	token arg2;
	token resule;
	bool isEmpty() {
		return true;
	}
	void print() {
		std::cout << '(';
		op.printValue(); std::cout << ',';
		arg1.printValue(); std::cout << ',';
		arg2.printValue(); std::cout << ',';
		resule.printValue();
		std::cout << ')' << std::endl;
	}
};

#endif // !_GLOBAL_H
