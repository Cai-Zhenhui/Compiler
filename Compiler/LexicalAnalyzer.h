#pragma once
#ifndef _LEXICAL_ANALYZER_H
#define _LEXICAL_ANALYZER_H

#include <fstream>
#include <string>
#include <vector>
#include "Struct/LinkList.h"
#include "Global.h"

//constexpr auto LENGTH_DELIMITER = 5;
#define FILENAME_DELIMITER "Tab_Delimiter.txt"
#define FILENAME_KEYWORD "Tab_Keyword.txt"

struct sToken{//识别器中临时使用
	TokenType type= TokenType::Unknow;//token type
	unsigned int pos;//The location of the token in the corresponding table.
	std::string word="";
	void print() {
		std::cout << "( ";
		switch (type){
		case Delimiter:{
			std::cout << "P,";//界符
			break;
		}
		case Keyword:{
			std::cout << "K,";//关键字
			break;
		}
		case Identifier:{
			std::cout << "I,";//标识符
			break;
		}
		case ConstantFloat:{
			std::cout << "CF,";//常数float
			break;
		}
		case ConstantInt:{
			std::cout << "CI,";//常数int
			break;
		}
		case CharM:{
			std::cout << "CC,";//常数char
			break;
		}
		case StringM:{
			std::cout << "CS,";//常数string
			break;
		}
		default: {
			break;
		}
		}
		std::cout << word << " )" << std::endl;
	}

	void clear(){
		type = TokenType::Unknow;
		word.clear();
		pos = -1;
	}
};
typedef struct sToken sToken;

int Init_LexicalAnalyzer();
int LexicalAnalyzer();
void test();
#endif // !_LEXICAL_ANALYZER_H
