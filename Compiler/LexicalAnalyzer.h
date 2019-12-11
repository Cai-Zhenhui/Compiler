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

struct sToken{//ʶ��������ʱʹ��
	TokenType type= TokenType::Unknow;//token type
	unsigned int pos;//The location of the token in the corresponding table.
	std::string word="";
	void print() {
		std::cout << "( ";
		switch (type){
		case Delimiter:{
			std::cout << "P,";//���
			break;
		}
		case Keyword:{
			std::cout << "K,";//�ؼ���
			break;
		}
		case Identifier:{
			std::cout << "I,";//��ʶ��
			break;
		}
		case ConstantFloat:{
			std::cout << "CF,";//����float
			break;
		}
		case ConstantInt:{
			std::cout << "CI,";//����int
			break;
		}
		case CharM:{
			std::cout << "CC,";//����char
			break;
		}
		case StringM:{
			std::cout << "CS,";//����string
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
