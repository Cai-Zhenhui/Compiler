#include "Global.h"

//界符表 静态表从文件读取
std::vector <std::string> list_Delimiter;

//关键字表 静态表从文件读取
std::vector <std::string> list_Keyword;

//整型常数表 动态表从源码中识别
std::vector <int> list_int_Constant;

//实数常数表 动态表从源码中识别
std::vector <double> list_float_Constant;


//符号表 动态表从源码中识别
std::vector<Symbol> list_Symbol;

std::vector <token> list_token;//token序列


std::vector <std::string> list_String;//字符串常量表
std::vector <std::string> list_Char;//字符常量表
std::vector <std::string> list_Identifier;//标识符表 动态表从源码中识别
