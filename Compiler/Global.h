#pragma once
#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <iostream>
#include <vector>
#include <string>



//以下所有设计的指针都是int型 而这也是指针的本质

//类型表(符号表的子表)的表项的 类码 域
enum TVAL {
	Unknow_TVAL = -1,
	Integer=1,//i
	Real,//r
	Char,//c
	Bool,//b
	Array,//a
	Struct//d
};

//类型表(符号表的子表)的表项
struct TAPE {
	//类码 
	TVAL type;

	/*
	1\基本数据类型 int real char bool 空指针
	2\数组类型array 指向数组表
	3\结构体类型Struct 指向结构表
	*/
	int pointer;
};
typedef struct TAPE TAPE;

//数组表(符号表的子表)的表项
struct AINF
{
	int LOW;//数组下界 应该用不到
	int UP;//数组上界
	TVAL type;//数组中元素的类型

	/*
	成分类型的数据所占值单元的个数(也就是字节数)
	=sizeof(type)*(UP-LOW)
	*/
	int length;
};
typedef struct AINF AINF;

//结构体的表项
struct RINF
{
	//结构体中每一项的类型
	std::vector<TAPE>TypeList;

	//结构体中每一项的大小
	std::vector<unsigned int>SizeList;
};
typedef struct RINF RINF;

//函数表的表项
struct PFINF
{
	int level;
	int offset;
	int ParameterNumber;
	int entry;
	int parameter;
};
typedef struct PFINF PFINF;

//常量表的表项
//直接使用int/double

//长度表的表项
struct LEN {
	int len;//后边根据实际在确定吧
};

/*符号表中表项 种类
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
	PointerParameter, //传址
	NormalParameter //传值
};

//符号表 表项
struct Symbol
{
	std::string name;
	int type;//指针 指向类型表list_TAPE中的某个位置
	SymbolKind cat;//种类 该标识符在源程序中的语义角色
	int addr;//指针 根据type域指向函数表、常量表、长度表中的某个位置
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
extern std::vector<Symbol> list_Symbol;//符号表

struct Delimiter
{
	std::string name;

};

enum TokenType {//token 类型
	Unknow = -1,
	Delimiter = 1,
	Keyword,
	Identifier,
	CharM,
	StringM,
	ConstantFloat,
	ConstantInt
};

extern std::vector <std::string> list_Delimiter;//界符表 静态表从文件读取
extern std::vector <std::string> list_Keyword;//关键字表 静态表从文件读取
extern std::vector <std::string> list_String;//字符串常量表
extern std::vector <std::string> list_Char;//字符常量表
//extern std::vector <std::string> list_Identifier;//标识符表 动态表从源码中识别
extern std::vector <int> list_int_Constant;//整型常数表 动态表从源码中识别
extern std::vector <double> list_float_Constant;//整型常数表 动态表从源码中识别

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
			std::cout << "P,";//界符
			break;
		}
		case Keyword: {
			std::cout << "K,";//关键字
			break;
		}
		case Identifier: {
			std::cout << "I,";//标识符
			break;
		}
		case ConstantFloat: {
			std::cout << "CF,";//常数float
			break;
		}
		case ConstantInt: {
			std::cout << "CI,";//常数int
			break;
		}
		case CharM: {
			std::cout << "CC,";//常数char
			break;
		}
		case StringM: {
			std::cout << "CS,";//常数string
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

extern std::vector <token> list_token;//token序列

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
