#include "GrammaAnalyzer.h"
/*
!(3 ^ 4 & 5 * (7 >> 2) - 2 + 3 / 0.2 * pi + 'a' - test)
*/
/*
Gramma
E->T|Ew1T
T->F|Tw2F
F->i|(E)

Fixed Gramma
Z->E
E->TE1
E1->w1TE1|e
T->FT1
T1->w2FT1|e
F->i|(E)

note:
w1:+ -
w2:* /
i:变量或常数
e:空

2+3
E=TE1
 =Tw1TE1
 =FT1w1TE1
 =iT1w1TE1
 =2T1w1TE1
 =2ew1TE1
 =2w1TE1
 =2+TE1
 =2+FT1E1
 =2+iT1E1
 =2+3T1E1
 =2+3eE1
 =2+3E1
 =2+3e
 =2+3
*/
/*
new Gramma
E->T|Ew1T	w1={,}
T1->T2|T1w2T2	w2={=,+=,-=,/=,*=,%=,<<=,>>=,&=,^=,|=}//各类赋值号
T2->T3|T2w3T3	w3={||}//2 3之间少个三元运算符
T3->T4|T3w4T4	w4={&&}
T4->T5|T4w5T5	w5={|}
T5->T6|T5w6T6	w6={^}
T6->T7|T6w7T7	w7={&}
T7->T8|T7w8T8	w8={==,!=}
T8->T9|T8w9T9	w9={<,>,<=,>=}
T9->T10|T9w10T10	w10={<<,>>}
T10->T11|T10w11T11	w11={+,-}
T11->T12|T11w12T12	w12={*,/,%}
T12->T13|(w13)T12	w13={类型名} //暂不实现
T13->T14|++T13|--T13|w14T13|sizeofT13|sizeof(w13)	w14={+,-,!,~,&,*}//其中sizeof暂不实现
T14->T15|T14[E]|T14()|T14(参数列表)|T14.|T14->|T14++,T14--
T15->I|(E)				I={Constant,Identifier}

Fixed Grammma
Z->E
E->T1E_
E_->w1T1E_|e		w1={,}
T1->T2T1_
T1_->w2T2T1_|e		w2={=,+=,-=,/=,*=,%=,<<=,>>=,&=,^=,|=}//各类赋值号
T2->T3T2_
T2_->w3T3T2_|e		w3={||}//2 3之间少个三元运算符
T3->T4T3_
T3_->w4T4T3_|e		w4={&&}
T4->T5T4_
T4_->w5T5T4_|e		w5={|}
T5->T6T5_
T5_->w6T6T5_|e		w6={^}
T6->T7T6_
T6_->w7T7T6_|e		w7={&}
T7->T8T7_
T7_->w8T8T7_|e		w8={==,!=}
T8->T9T8_
T8_->w9T9T8_|e		w9={<,>,<=,>=}
T9->T10T9_
T9_->w10T10T9_|e	w10={<<,>>}
T10->T11T10_
T10_->w11T11T10_|e	w11={+,-}
T11->T12T11_
T11_->w12T12T11_|e	w12={*,/,%}
T12->T13|(w13)T12	w13={类型名} //暂不实现
T13->T14|++T13|--T13|w14T13|sizeofT13|sizeof(w13)	w14={+,-,!,~,&,*}//其中sizeof暂不实现
T14->T15T14_
T14_->[E]T14_|()T14_|(参数列表)T14_|.T14_|->T14_|++T14_|--T14_|e
T15->I|(E)				I={Constant,Identifier}
*/
std::vector<token>::iterator word;

//T15->I | (E)I = { Constant,Identifier }
bool RecT15() {
	bool RecE();
	bool flag;
	switch ((*word).type) {
	case TokenType::ConstantFloat: {
		++word;
		return true;
	}
	case TokenType::ConstantInt: {
		++word;
		return true;
	}
	case TokenType::CharM: {
		++word;
		return true;
	}
	case TokenType::StringM: {
		++word;
		return true;
	}
	case TokenType::Identifier: {
		//if()//类型匹配
		++word;
		return true;
	}
	case TokenType::Delimiter: {
		if (list_Delimiter[(*word).pos].compare("(") == 0) {
			//左括号匹配
			++word;
			if (RecE()) {//匹配括号中间表达式
				//匹配成功
				if (list_Delimiter[(*word).pos].compare(")") == 0) {
					//右括号匹配
					++word;
					return true;
				}
				else {
					//右括号不匹配
					return false;
				}
			}
			else {
				//括号中间表达式出错 
				return false;
			}
		}
		else {
			//左括号不匹配
		}
	}
	default:
		return false;
	}
}

//T14_->[E]T14_ | ()T14_ | (参数列表)T14_ | .T14_ | ->T14_ | ++T14_ | --T14_ | e
bool RecT14_() {
	bool RecE();
	if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare("[") == 0)) {
		//T14_->[E]T14_
		++word;
		if (RecE()) {//尝试匹配方括号内部表达式
				//方括号内部表达式匹配成功
			if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare("]") == 0)) {
				++word;//匹配]
				return RecT14_();
			}
			else {
				return false;
			}
		}
		else {
			//方括号内部表达式匹配 失败
			return false;
		}
	}
	else if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare("->") == 0)) {
		//T14_->wvT14_|		w={->},v={Identifier}
		++word;//匹配->
		if ((*word).type == TokenType::Identifier) {
			//if()结构类成员成员匹配
			++word;
			//需要递归
			return RecT14_();
		}
		else {
			return false;
		}
		return RecT14_();
	}
	else if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare(".") == 0)) {
		//.T14_
		++word;//匹配->
		if ((*word).type == TokenType::Identifier) {
			//if()结构类成员成员匹配
			++word;
			return RecT14_();
		}
		else {
			return false;
		}
		return RecT14_();
	}
	else if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare("()") == 0)) {
		//T14_->()T14_ 
		++word;//匹配()
		return RecT14_();
	}
	/*
	else if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare("(参数)") == 0)) {
		//T14_->()T14_ 
		++word;//匹配()
		return RecT14_();
	}*/
	else if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare("++") == 0)) {
		//++T14_
		++word;//匹配()
		return RecT14_();
	}
	else if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare("--") == 0)) {
		//++T14_
		++word;//匹配()
		return RecT14_();
	}
	else {
		return true;//maybe e
	}
	return true;
}

//T14->T15T14_
bool RecT14() {//括号或者终结符
	return RecT15() && RecT14_();
}

//T13->T14 | ++T13 | --T13 | w14T13 | sizeofT13 | sizeof(w13)	w14 = { +,-,!,~,&,* }
bool RecT13() {
	if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare("++") == 0)) {
		//T13->++T13
		++word;
		return RecT13();
	}
	else if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare("--") == 0)) {
		//T13->--T13
		++word;
		return RecT13();
	}
	else if (((*word).type == TokenType::Keyword) && (list_Keyword[(*word).pos].compare("sizeof") == 0)) {
		//T13->sizeofT13
		++word;
		return RecT13();
	}
	else if (((*word).type == TokenType::Keyword) && (list_Keyword[(*word).pos].compare("sizeof") == 0)) {
		//T13->sizeof(w13) //这里是否需要递归？
		++word;
		if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare("(") == 0)) {
			//匹配左括号
			++word;
			if (((*word).type == TokenType::Keyword) && (list_Delimiter[(*word).pos].compare("int") == 0)) {
				++word;
				if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare(")") == 0)) {
					++word;//匹配右括号
					return true;
				}
				else {
					return false;
				}
			}
			else if (((*word).type == TokenType::Keyword) && (list_Delimiter[(*word).pos].compare("long") == 0)) {
				++word;
				if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare(")") == 0)) {
					++word;//匹配右括号
					return true;
				}
				else {
					return false;
				}
			}
			else if (((*word).type == TokenType::Keyword) && (list_Delimiter[(*word).pos].compare("char") == 0)) {
				++word;
				if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare(")") == 0)) {
					++word;//匹配右括号
					return true;
				}
				else {
					return false;
				}
			}
			else if (((*word).type == TokenType::Keyword) && (list_Delimiter[(*word).pos].compare("short") == 0)) {
				++word;
				if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare(")") == 0)) {
					++word;//匹配右括号
					return true;
				}
				else {
					return false;
				}
			}
			else if (((*word).type == TokenType::Keyword) && (list_Delimiter[(*word).pos].compare("float") == 0)) {
				++word;
				if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare(")") == 0)) {
					++word;//匹配右括号
					return true;
				}
				else {
					return false;
				}
			}
			else if (((*word).type == TokenType::Keyword) && (list_Delimiter[(*word).pos].compare("double") == 0)) {
				++word;
				if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare(")") == 0)) {
					++word;//匹配右括号
					return true;
				}
				else {
					return false;
				}
			}
			else {
				return false;//没有要计算的类型 如果以后增加用户自定义类型 需要重新建表 
			}

		}
		else {
			return false;
		}
	}
	else if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare("+") == 0)) {
		//T13->+T13  
		++word;
		return RecT13();
	}
	else if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare("-") == 0)) {
		//T13->-T13  
		++word;
		return RecT13();
	}
	else {//else if 嵌套过多？？？ vs自动对齐失效？？？
		if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare("~") == 0)) {
			//T13->~T13  
			++word;
			return RecT13();
		}
		else if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare("&") == 0)) {
			//T13->&T13  
			++word;
			return RecT13();
		}
		else if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare("*") == 0)) {
			//T13->&T13  
			++word;
			return RecT13();
		}
		else {
			//T13->T14
			return RecT14();
		}
	}
}

//T12->T13 | (w13)T12	w13 = { 类型名 }  //只支持关键字类型转换
bool RecT12() {
	if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare("(") == 0)) {
		//左括号匹配
		++word;
		if ((*word).type == TokenType::Keyword) {//匹配一个类型名
			if (list_Keyword[(*word).pos].compare("int")) {
				//强制类型转换为int
				++word;
				if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare(")") == 0)) {
					//右括号匹配
					++word;
					return RecT12();
				}
				else {
					return false;
				}
			}
			else if (list_Keyword[(*word).pos].compare("float")) {
				//强制类型转换为float
				++word;
				if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare(")") == 0)) {
					//右括号匹配
					++word;
					return RecT12();
				}
				else {
					return false;
				}
			}
			else if (list_Keyword[(*word).pos].compare("long")) {
				//强制类型转换为long
				++word;
				if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare(")") == 0)) {
					//右括号匹配
					++word;
					return RecT12();
				}
				else {
					return false;
				}
			}
			else if (list_Keyword[(*word).pos].compare("short")) {
				//强制类型转换为short
				++word;
				if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare(")") == 0)) {
					//右括号匹配
					++word;
					return RecT12();
				}
				else {
					return false;
				}
			}
			else if (list_Keyword[(*word).pos].compare("char")) {
				//强制类型转换为char
				++word;
				if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare(")") == 0)) {
					//右括号匹配
					++word;
					return RecT12();
				}
				else {
					return false;
				}
			}
			else if (list_Keyword[(*word).pos].compare("double")) {
				//强制类型转换为
				++word;
				if (((*word).type == TokenType::Delimiter) && (list_Delimiter[(*word).pos].compare(")") == 0)) {
					//右括号匹配
					++word;
					return RecT12();
				}
				else {
					return false;
				}
			}
		}
		else {
			//匹配失败该左括号再次不是强制类型转换
			--word;
			return RecT13();
		}
	}
	else {
		return RecT13();
	}
}

//T11_->w12T12T11_|e	w12={*,/,%}
bool RecT11_() {
	if ((*word).type == TokenType::Delimiter) {
		if (list_Delimiter[(*word).pos].compare("*") == 0) {
			++word;
			return  RecT12() && RecT11_();
		}
		else if (list_Delimiter[(*word).pos].compare("/") == 0) {
			++word;
			return  RecT12() && RecT11_();
		}
		else if(list_Delimiter[(*word).pos].compare("%") == 0) {
			++word;
			return  RecT12() && RecT11_();
		}
		
	}
	return true;
}

//T11->T12T11_
bool RecT11() {
	return RecT12() && RecT11_();
}

//T10_->w11T11T10_|e	w11={+,-}
bool RecT10_() {
	if ((*word).type == TokenType::Delimiter) {
		if (list_Delimiter[(*word).pos].compare("+") == 0) {
			++word;
			return  RecT11() && RecT10_();
		}
		else if (list_Delimiter[(*word).pos].compare("-") == 0) {
			++word;
			return  RecT11() && RecT10_();
		}
	}
	return true;
}

//T10->T11T10_
bool RecT10() {
	return RecT11() && RecT10_();
}

//T9_->w10T10T9_|e	w10={<<,>>}
bool RecT9_() {
	if ((*word).type == TokenType::Delimiter) {
		if (list_Delimiter[(*word).pos].compare("<<") == 0) {
			++word;
			return  RecT10() && RecT9_();
		}
		else if (list_Delimiter[(*word).pos].compare(">>") == 0) {
			++word;
			return  RecT10() && RecT9_();
		}
	}
	return true;
}

//T9->T10T9_
bool RecT9() {
	return RecT10() && RecT9_();
}

//T8_->w9T9T8_|e		w9={<,>,<=,>=}
bool RecT8_() {
	if ((*word).type == TokenType::Delimiter) {
		if (list_Delimiter[(*word).pos].compare("<") == 0) {
			++word;
			return  RecT9() && RecT8_();
		}
		else if (list_Delimiter[(*word).pos].compare(">") == 0) {
			++word;
			return  RecT9() && RecT8_();
		}
		else if(list_Delimiter[(*word).pos].compare("<=") == 0) {
			++word;
			return  RecT9() && RecT8_();
		}
		else if (list_Delimiter[(*word).pos].compare(">=") == 0) {
			++word;
			return  RecT9() && RecT8_();
		}
	}
	return true;
}

//T8->T9T8_
bool RecT8() {
	return RecT9() && RecT8_();
}

//T7_->w8T8T7_|e		w8={==,!=}
bool RecT7_() {
	if ((*word).type == TokenType::Delimiter) {
		if (list_Delimiter[(*word).pos].compare("==") == 0) {
			++word;
			return  RecT8() && RecT7_();
		}
		else if (list_Delimiter[(*word).pos].compare("!=") == 0) {
			++word;
			return  RecT8() && RecT7_();
		}
	}
	return true;
}

//T7->T8T7_
bool RecT7() {
	return RecT8() && RecT7_();
}

//T6_->w7T7T6_|e		w7={&}
bool RecT6_() {
	if ((*word).type == TokenType::Delimiter) {
		if (list_Delimiter[(*word).pos].compare("&") == 0) {
			++word;
			return  RecT7() && RecT6_();
		}
	}
	return true;
}

//T6->T7T6_
bool RecT6() {
	return RecT7() && RecT6_();
}

//T5_->w6T6T5_|e		w6={^}
bool RecT5_() {
	if ((*word).type == TokenType::Delimiter) {
		if (list_Delimiter[(*word).pos].compare("^") == 0) {
			++word;
			return  RecT6() && RecT5_();
		}
	}
	return true;
}

//T5->T6T5_
bool RecT5() {
	return RecT6() && RecT5_();
}

//T4_->w5T5T4_|e		w5={|}
bool RecT4_() {
	if ((*word).type == TokenType::Delimiter) {
		if (list_Delimiter[(*word).pos].compare("|") == 0) {
			++word;
			return  RecT5() && RecT4_();
		}
	}
	return true;
}

//T4->T5T4_
bool RecT4() {
	return RecT5() && RecT4_();
}

//T3_->w4T4T3_|e		w4={&&}
bool RecT3_() {
	if ((*word).type == TokenType::Delimiter) {
		if (list_Delimiter[(*word).pos].compare("&&") == 0) {
			++word;
			return  RecT4() && RecT3_();
		}
		//maybe e
	}
	//maybe e
	return true;
}

//T3->T4T3_
bool RecT3() {
	return RecT4() && RecT3_();
}

//T2_->w3T3T2_|e		w3={||}//2 3之间少个三元运算符
bool RecT2_() {
	if ((*word).type == TokenType::Delimiter) {
		if (list_Delimiter[(*word).pos].compare("||") == 0) {
			++word;
			return RecT3() && RecT2_();
		}
		//maybe e
	}
	//maybe e
	return true;
}

//T2->T3T2_
bool RecT2() {
	return RecT3() && RecT2_();
}

//T1_->w2T2T1_|e		w2={=,+=,-=,/=,*=,%=,<<=,>>=,&=,^=,|=}//各类赋值号
bool RecT1_() {
	//bool flag = false;
	if ((*word).type == TokenType::Delimiter) {
		if (list_Delimiter[(*word).pos].compare("=") == 0) {
			++word;
			return  RecT2() && RecT1_();
		}
		else if (list_Delimiter[(*word).pos].compare("+=") == 0) {
			++word;
			return  RecT2() && RecT1_();
		}
		else if(list_Delimiter[(*word).pos].compare("-=") == 0) {
			++word;
			return  RecT2() && RecT1_();
		}
		else if (list_Delimiter[(*word).pos].compare("*=") == 0) {
			++word;
			return  RecT2() && RecT1_();
		}
		else if (list_Delimiter[(*word).pos].compare("/=") == 0) {
			++word;
			return  RecT2() && RecT1_();
		}
		else if (list_Delimiter[(*word).pos].compare("%=") == 0) {
			++word;
			return  RecT2() && RecT1_();
		}
		else if (list_Delimiter[(*word).pos].compare("<<=") == 0) {
			++word;
			return  RecT2() && RecT1_();
		}
		else if (list_Delimiter[(*word).pos].compare(">>=") == 0) {
			++word;
			return  RecT2() && RecT1_();
		}
		else if (list_Delimiter[(*word).pos].compare("&=") == 0) {
			++word;
			return  RecT2() && RecT1_();
		}
		else if (list_Delimiter[(*word).pos].compare("^=") == 0) {
			++word;
			return  RecT2() && RecT1_();
		}
		else if (list_Delimiter[(*word).pos].compare("|=") == 0) {
			++word;
			return  RecT2() && RecT1_();
		}
		else {
			//maybe e
		}
	}
	////maybe e
	return true;
}
bool RecT1() {
	return RecT2()&&RecT1_();
}

//E_->w1T1E_ | e		w1 = { , }
bool RecE_() {
	//bool flag = false;
	if ((*word).type == TokenType::Delimiter) {
		if (list_Delimiter[(*word).pos].compare(",") == 0) {
			++word;
			return  RecT1() && RecE_();
		}
		//maybe e
	}
	////maybe e
	return true;
}

//E->T1E_
bool RecE() {
	return RecT1()&& RecE_();
}

//Z->E
bool Recursive() {
	bool flag=true;
	word = list_token.begin();
	flag =  RecE();
	word++;
	if (flag && (word == list_token.end())) {
		return true;
	}
	else {
		return false;
	}
}


/*
//F->i | (E)
bool RecF() {
	bool RecE();
	bool flag = false;
	switch ((*word).type) {
	case TokenType::ConstantInt: {//常数 int
		//if(list_int_Constant[(*word).pos])//不需要进一步判断
		++word;
		return true;
	}
	case TokenType::ConstantFloat: {//常数 float
		++word;
		return true;
	}
	case TokenType::Identifier: {//变量  标识符中
		++word;
		return true;
	}
	case TokenType::Delimiter: {
		if (list_Delimiter[(*word).pos].compare("(") == 0) {
			++word;
			flag = true;
			flag = RecE();//flag = flag && RecE();
			if (!flag) {
				//不用继续识别右括号
				return false;
			}
			//识别右括号
			switch ((*word).type) {
			case TokenType::Delimiter: {
				if (list_Delimiter[(*word).pos].compare(")") == 0) {
					++word;
					return true;
				}
				else {
					return false;
				}
			}
			default: {
				return false;
			}
			}
		}
		else {
			return false;
		}
	}
	default: {
		return false;
	}
	}
}

//T1->w2FT1|e
bool RecT1() {
	bool flag = false;
	switch ((*word).type) {
	case TokenType::Delimiter: {
		if (list_Delimiter[(*word).pos].compare("*") == 0) {
			//是+号
			++word;
			flag = true;
		}
		else if (list_Delimiter[(*word).pos].compare("/") == 0) {
			//是-号
			++word;
			flag = true;
		}
		else {
			return true;//e???//return false;
		}
		break;
	}
	default:
		return true;//e???//return false;
	}

	flag = flag && RecF();
	flag = flag && RecT1();
	return flag;
}

//T->FT1
bool RecT() {
	return RecF() && RecT1();
}

//E1->w1TE1|e
bool RecE1() {
	bool flag = false;
	switch ((*word).type){
	case TokenType::Delimiter: {
		if (list_Delimiter[(*word).pos].compare("+") == 0) {
			//是+号
			++word;
			flag = true;
		}
		else if (list_Delimiter[(*word).pos].compare("-") == 0) {
			//是-号
			++word;
			flag = true;
		}
		else {
			return true;//e???//return false;
		}
		break;
	}
	default:
		return true;//e???//return false;
	}

	flag = flag && RecT();
	flag = flag && RecE1();
	return flag;
}

//E->TE1
bool RecE() {
	return RecT()&& RecE1();
}
*/

/*
Gramma
E->T|Ew1T
T->F|Tw2F
F->i|(E)

Fixed Gramma
0	E->TE1
1	E1->w1TE1
2	E1->e
3	T->FT1
4	F1->w2FT1
5	T1->e
6	F->i
7	F->(E)

note:
w1:+ -
w2:* /
i:变量或常数
e:空
*/

std::vector<std::string> VN;
std::vector<std::string> VT;
std::vector<std::vector<int>>LL1Table = {
	{0,-1,-1,0,-1,-1},
	{-1,1,-1,-1,-2,-2},
	{3,-1,-1,3,-1,-1},
	{-1,-2,4,-1,-2,-2},
	{6,-1,-1,7,-1,-1}
};
 std::vector<std::vector<std::string>> P;
//std::stack<std::string> AnalysisStack;//stack pop之后string会被析构
std::vector<std::string> AnalysisStack;
int v_find(std::string x,bool is_VN) {
	if (is_VN) {
		for (int i = 0; i < VN.size(); ++i) {
			if (VN[i].compare(x) == 0) {
				return i;
			}
		}
	}
	else {
		for (int i = 0; i < VT.size(); ++i) {
			if (VT[i].compare(x) == 0) {
				return i;
			}
		}
	}
	return -1;
}
int LUT(int x_pos) {
	int pos = 0;
	switch ((*word).type){
	case TokenType::Identifier:{
		;
	}
	case TokenType::CharM: {
		;
	}
	case TokenType::ConstantFloat : {
		;
	}
	case TokenType::ConstantInt: {
		//constant
		pos = 0;
		break;
	}
	case TokenType::Delimiter: {
		if ((list_Delimiter[(*word).pos].compare("+") == 0)||
			(list_Delimiter[(*word).pos].compare("-") == 0)) {
			pos = 1;
			break;
		}
		else if ((list_Delimiter[(*word).pos].compare("*") == 0) ||
			(list_Delimiter[(*word).pos].compare("/") == 0)) {
			pos = 2;
			break;
		}
		else if (list_Delimiter[(*word).pos].compare("(") == 0) {
			pos = 3;
			break;
		}
		else if (list_Delimiter[(*word).pos].compare(")") == 0) {
			pos = 4;
			break;
		}
		else {
			pos = 5;
			break;
		}
	}
	default:
		pos = 5;
		break;
	}
	return LL1Table[x_pos][pos];
}
int Init_GA_LL1() {
	using namespace std;
	fstream file("Gramma.txt", ios::in | ios::out);
	//FILE* file;
	string temp;
	while (file >> temp) {//读取VN
		if (temp.compare("-----") == 0) {
			break;
		}
		//VN.insert(temp);
		VN.push_back(temp);
		//cout << temp;
	}
	while (file >> temp) {//读入VT
		if (temp.compare("-----") == 0) {
			break;
		}
		//VT.insert(temp);
		VT.push_back(temp);
		//cout << temp;
	}
	//int i = 0;
	getline(file, temp);
	while (!file.eof()) {//读入P
		//file.getline(&temp[0], 128);
		vector<string> fun ;
		getline(file, temp);
		std::stringstream ss(temp);
		std::string buf;
		while (ss >> buf) {
			//P[i].push_back(buf);
			fun.push_back(buf);
		}
		//++i;
		P.push_back(fun);
		//cout << P[i];
	}

	file.close();
	return 0;
}
void PrintStack() {
	for (int i = 0; i < AnalysisStack.size(); ++i) {
		std::cout << AnalysisStack[i] << " " ;
	}
	std::cout << "\n";
}
int LL1() {
	Init_GA_LL1();
	using namespace std;
	std::string x;
	AnalysisStack.push_back("E");//开始字符
	word = list_token.begin();

	while (word != list_token.end()) {
		
		if (AnalysisStack.empty()&& (*word).type == TokenType::Unknow) {
			return 1;//
		}
		
		x = AnalysisStack[AnalysisStack.size()-1];
		//x.push_back('0');
		//printf("%s\t", x);
		(*word).print();
		PrintStack();
		//x.pop_back();
		if (v_find(x,false)>-1) {
			//x∈VT
			//x==w
			++word;
			AnalysisStack.pop_back();
			continue;
		}
		int pos = v_find(x, true);//x∈VN
		if (pos>-1) {
			//look LL1 table
			int index_P = LUT(pos);
			if (index_P == -2) {
				//空产生式
				AnalysisStack.pop_back();//
				continue;
			}
			if (index_P >= 0) {
				//push iR
				vector<string> fun = P[index_P];
				vector<string> funR(fun.rbegin(),fun.rend());//r
				//vector<string> funR(fun.begin(), fun.end());
				vector<string>::iterator it= funR.begin();
				//AnalysisStack[AnalysisStack.size() - 1] = (*it);//覆盖栈顶
				AnalysisStack.pop_back();//
				while (it != funR.end()) {
					AnalysisStack.push_back(*it);
					++it;
				}
				//while()
				/*
				int itemp=0;
				for (int i = fun.size() - 1,itemp=i; i > 0; --i) {//分割时不需要判断到0
					if (fun[i] == ' ') {
						funR = funR.append(fun.substr(i + 1, itemp).append(" "));
						itemp = i;
					}
				}
				funR = funR.append(fun.substr(0, itemp+1));
				//string funR(fun.rbegin(), fun.rend());//逆序字串
				stringstream ss(funR);
				string sub;
				ss >> sub;
				//覆盖栈顶
				AnalysisStack[AnalysisStack.size() - 1] = sub;
				while (ss >> sub) {//压入剩余部分
					AnalysisStack.push_back(sub);
				}*/
				continue;
			}
			else {
				return -2;
			}
		}
		if (word != list_token.end()) {
			return -1;
		}
		else {
			return 1;//right
		}
	}

	return 0;
}