/*
1\	(1+2)-(3-4)		类似于<<=的连续界符识别
2\	token.pos+20	
*/
/*
note:
1\fstream中>>流操作会过滤空白符 \n \t space等
2\fstream.read() 会把\n(0x0a)\r(0x0d)当作一个字节读入 只读入\n(0x0a)

*/
#include "LexicalAnalyzer.h"

#define FILE_NAME_Source "test.cpp"

using namespace std;

fstream file;


sToken sWord;//一个单词
										
/*
初始化词法分析器 读取关键字表和界符表
return
1	成功
-1	“Tab_Delimiter.txt”打开失败
-2	“Tab_Keyword.txt”打开失败
*/
int Init_LexicalAnalyzer() {
	using namespace std;
	ifstream file;
	
	//读取界符表
	file.open(FILENAME_DELIMITER);
	if (!file.is_open()) {
		return -1;//
	}
	while (!file.eof()) {
		string str;//不用担心内存的问题，c++ STL中string会在程序运行结束时释放内存
		file >> str;
		list_Delimiter.push_back(str);
	}
	file.close();

	//读取关键字表
	file.open(FILENAME_KEYWORD);
	if (!file.is_open()) {
		return -2;//
	}
	while (!file.eof()) {
		string str;//不用担心内存的问题，c++ STL中string会在程序运行结束时释放内存
		file >> str;
		list_Keyword.push_back(str);
	}
	file.close();

	return true;
}

//打印各个表的内容
void printTab() {
	
	cout << "Keyword Tab " << endl;
	for (int i = 0; i < list_Keyword.size(); ++i) {
		cout << i << " " << list_Keyword[i] << endl;
	}
	cout << "\nDelimiter Tab " << endl;
	for (int i = 0; i < list_Delimiter.size(); ++i) {
		cout << i << " " << list_Delimiter[i] << endl;
	}
	cout << "\nString Tab " << endl;
	for (int i = 0; i < list_String.size(); ++i) {
		cout << i << " " << list_String[i] << endl;
	}
	cout << "\nChar Tab " << endl;
	for (int i = 0; i < list_Char.size(); ++i) {
		cout << i << " " << list_Char[i] << endl;
	}
	cout << "\nIdentifier Tab " << endl;
	for (int i = 0; i < list_Symbol.size(); ++i) {
		cout << i << ' ' ;
		list_Symbol[i].print();
		cout << endl;
	}
	cout << "\nint_Constant Tab " << endl;
	for (int i = 0; i < list_int_Constant.size(); ++i) {
		cout << i << " " << list_int_Constant[i] << endl;
	}
	cout << "\nfloat_Constant Tab " << endl;
	for (int i = 0; i < list_float_Constant.size(); ++i) {
		cout << i << " " << list_float_Constant[i] << endl;
	}
	cout << endl;
}

inline bool isIdentifier(char ch) {
	return ch >= 'a' && ch <= 'z' || 
		ch >= 'A' && ch <= 'Z'||
		ch =='_';
}

/*
0 整数
1 float
*/
int String2Number(void * number) {
	//这样做可能会有点问题
	//string过长的的话 emmmm 不会de
	if (sWord.word.find('.') == std::string::npos) {
		//整数
		sscanf(&sWord.word[0], "%d", (int*)number);
		return 0;
	}
	else {
		sscanf(&sWord.word[0], "%f", (float*)number);
		return 1;
	}
}

/*
返回值：
NN	FSM正常运行
-1	非法字符,不在字符集
-2	FSM 状态错误
-3	字符常量 ' 不匹配
-4	字符串常量 " 不匹配
-5	常数小数点错误
-6	界符匹配错误
*/
int FSM(int state,char &ch) {
	switch (state){
	case 0: {
		break;
	}
	case 1: {//1状态 初始状态 
		//这个状态检查的是首字符
		if (isIdentifier(ch)) {
			//标识符或关键字
			sWord.word.push_back(ch);
			//sWord.type = TokenType;//这里不确定
			return 2;//change to s2
		}
		else if (ch >= '0' && ch <= '9') {
			//数字 常数
			sWord.word.push_back(ch);
			sWord.type = TokenType::ConstantInt;
			return 3;
		}
		else if (ch == '\'') {
			//字符常量  暂时不考虑合法性
			sWord.word.push_back(ch);
			sWord.type = TokenType::CharM;
			return 5;
		}
		else if (ch == '\"') {
			//字符串常量 
			sWord.word.push_back(ch);
			sWord.type = TokenType::StringM;
			return 6;
		}

		else {//处理界符 和不可见符号
			
			  //查界符表
			for (int i = 0; i < list_Delimiter.size(); ++i) {
				if (list_Delimiter[i][0] == ch) {//匹配第一位
					sWord.word.push_back(ch);
					sWord.type = TokenType::Delimiter;
					sWord.pos = i;
					return 4;//界符
				}
			}

			//不在界符表

			
			switch (ch){
			case 0x09: {//\t
				break;//接受 不加入word 暂不处理
			}
			case 0x0a: {//\n
				break;//接受 不加入word 暂不处理
			}
			case 0x20: {
				break;//接受 不加入word 暂不处理
			}
			default:
				//非法字符不接受
				return -1;//一般情况下不会出现 除非文档被破坏
			}
			return 1;//忽略空白符
		}
	}
	case 2: {//标识符或关键字分支 
		//该状态检查的是除首字符以外的字符 可接受的字符={字母,_,数字}
		if (isIdentifier(ch) || 
			(ch == '_')||
			(ch >= '0' && ch <= '9')) {
			
			sWord.word.push_back(ch);
			return 2;//循环
		}
		else {
			
			return 0;//单词完结
		}
	}
	case 3: {//常数分支
		//该状态检查的是除首字符以外的字符 可接受的字符={数字,.}//暂时不考虑科学记数法
		if ((ch >= '0' && ch <= '9')||
			(ch=='.')) {
			if (ch == '.') {
				sWord.type = TokenType::ConstantFloat;
				sWord.word.push_back(ch);
				return 7;//以后不能再有'.'//从此变成float
			}
			sWord.word.push_back(ch);
			return 3;//循环
		}
		else {
			return 0;//单词完结
		}
	}
	case 4: {//界符分支
		//查界符表
		
		sWord.word.push_back(ch);//先把要匹配的字符加入 一匹配的字符串中
								 //方便整体比较，比较完成之后。如果匹配到了就不用删除
								 //如果没有匹配到，就将加入的字符删掉
		int length_word = sWord.word.size();
		for (int i = 0; i < list_Delimiter.size(); ++i) {
			//匹配 前(sWord.word.size();)个字符
			if (sWord.word.compare(list_Delimiter[i].substr(0, length_word)) == 0) {
				//匹配到
				sWord.pos = i;// 重新匹配 i肯可能会发生变化
				return 4; //界符不一定完结
			}
		}
		//如果流出分支的话 说明出现了类似于 ')+'的连续界符序列
		//上一个界符完结 但是位置可能不正确
		sWord.word.pop_back();//删除前边用于匹配的字符
		//重新匹配位置 此时是界符确定的
		for (int i = 0; i < list_Delimiter.size(); ++i) {
			if (sWord.word.compare(list_Delimiter[i]) == 0) {//进行完全匹配
				//匹配到
				sWord.pos = i;// 重新匹配 i肯可能会发生变化
				return 0;
			}
		}
		//Error
		return -6;
	}
	case 5: {//字符常量分支
		//该状态检查的是除首字符以外的字符 可接受的字符={',一切可见字符}//暂时不考虑合法性
		if (ch == '\'') {//字符常量结束
			sWord.word.push_back(ch);  //这里单词完结斌没有读入新字符 导致和其他分支不一致
			file.read(&ch, sizeof(char));//所以再读入一个字符
			return 0;//单词完结
		}
		else if (ch >= 0x20 && ch <= 0x7E) {//可见字符范围 include space
			sWord.word.push_back(ch);
			return 5;
		}
		else {
			return -3;
		}
	}
	case 6: {
		//该状态检查的是除首字符以外的字符 可接受的字符={',一切可见字符}//暂时不考虑合法性
		if (ch == '"') {//字符串常量结束
			sWord.word.push_back(ch);  //这里单词完结斌没有读入新字符 导致和其他分支不一致
			file.read(&ch, sizeof(char));//所以再读入一个字符
			return 0;//单词完结
		}
		else if (ch >= 0x20 && ch <= 0x7E) {//可见字符范围 include space
			sWord.word.push_back(ch);
			return 6;
		}
		else {
			return -4;
		}
	}
	case 7: {//浮点分支
		//该状态检查的是除首字符以外的字符 可接受的字符={数字}
		if ((ch >= '0' && ch <= '9')) {
			sWord.word.push_back(ch);
			return 7;//循环
		}
		else if(ch=='.'){
			return -5;
		}
		else {
			return 0;//单词完结
		}
	}
	default:
		return -2;//FSM 状态错误
	}
}

/*
词法分析器子模块 识别器
返回值：
0	单词完结
-1	源文件扫描完毕
<-1	FSM错误 FSM_error_index=LA_Recognizer_error_index+1
*/
int LA_Recognizer() {
	char ch = 0;
	int state = 1;
	sWord.clear();//清空之前的单词
	while (file.read(&ch, sizeof(char))) {
		//cout << ch ;
		state = FSM(state, ch);
		if (state == 0) {
			file.seekg(-1, std::ios::cur);//可能(这里说可能的原因是有可能是空白符)读到下一个单词
			//的首字符，而这里会返回上层。再次进入会再读一个字符 这样就导致一个字符被漏掉
			//所以需要向前移动一个字节
			return 1;//单词完结 
		}
		else if (state < 0) {
			return state-1;//出现错误 向上传递
		}
	}
	return -1;//源文件扫描完毕
}

/*
词法分析器

返回值
1	正常
<0	错误 查看词法分析器子模块 识别器LA_Recognizer()的错误列表

*/
int LexicalAnalyzer() {
	
	Init_LexicalAnalyzer();//初始化词法分析器
	//读取源码文件
	file.open("test.cpp", ios::out | ios::in);
	{
		//在源文件尾部加上空格 防止读取错误
		file.seekp(-1, std::fstream::end);
		char temp=0;
		file.read(&temp, sizeof(char));
		if (temp != ' ') {
			file << ' ';
		}
		file.seekp(0, std::fstream::beg);
	}
	int state = 0;
	do
	{
		state = LA_Recognizer();
		if (state < 0) {
			break;
		}
		token token;
		token.type = sWord.type;
		switch (sWord.type)
		{
		case Delimiter:
		{
			//token token;
			//token.type = sWord.type;
			token.pos = sWord.pos;
			//list_token.push_back(token);
			break;
		}
		case ConstantInt:
		{
			//token.type = sWord.type;
			int *buf=new int;
			String2Number(buf);
			bool flag = false;
			for (int i = 0; i < list_int_Constant.size(); ++i) {
				if (list_int_Constant[i] == *buf) {
					token.pos = i;
					flag = true;
					break;
				}
			}
			if (flag) {
				//该常数已存在 不需要添加
			}
			else {
				token.pos = list_int_Constant.size();
				list_int_Constant.push_back(*buf);
			}
			delete buf;
			break;
		}
		case ConstantFloat:
		{
			//token.type = sWord.type;
			float* buf = new float;
			String2Number(buf);
			bool flag = false;
			for (int i = 0; i < list_float_Constant.size(); ++i) {
				if (abs(*buf - list_float_Constant[0]) < 1e-5) {
					token.pos = i;
					flag = true;
					break;
				}
			}
			if (flag) {
				//该常数已存在 不需要添加
			}
			else {
				token.pos = list_float_Constant.size();
				list_float_Constant.push_back(*buf);
			}
			delete buf;
			break;
		}
		case CharM: {
			//token token;
			bool flag = false;
			for (int i = 0; i < list_Char.size(); ++i) {
				if (list_Char[i].compare(sWord.word)==0) {
					flag = true;
					token.pos = i;
					break;
				}
			}
			if (flag) {
				//该字符已存在
			}
			else {
				token.pos = list_Char.size();
				list_Char.push_back(sWord.word);
			}
			break;
		}
		case StringM: {
			bool flag = false;
			for (int i = 0; i < list_String.size(); ++i) {
				if (list_String[i].compare(sWord.word) == 0) {
					flag = true;
					token.pos = i;
					break;
				}
			}
			if (flag) {
				//该字符已存在
			}
			else {
				token.pos = list_String.size();
				list_String.push_back(sWord.word);
			}
			break;
		}
		default: {
			//标识符和关键字还需要区分
			//先查关键字表
			bool flag = false;
			for (int i = 0; i < list_Keyword.size(); ++i) {
				if (list_Keyword[i].compare(sWord.word) == 0) {
					token.pos = i;
					token.type = TokenType::Keyword;
					flag = true;
					break;
				}
			}
			if (flag) {
				break;
			}
			else {
				//标识符表
				token.type = TokenType::Identifier;
				bool flags = false;
				for (int i = 0; i < list_Symbol.size(); ++i) {
					if (list_Symbol[i].name.compare(sWord.word) == 0) {
						token.pos = i;
						flags = true;
						break;
					}
				}
				if (flags) {
					//已存在
				}
				else {
					token.pos = list_Symbol.size();
					//加入符号表
					Symbol temp;
					temp.name = sWord.word;
					list_Symbol.push_back(temp);
				}
			}
			break;
		}
		}
		list_token.push_back(token);
		token.print();
	} while (state>0);
	std::cout << std::endl;
	file.close();
	if (state == -1) {
		//源文件扫描完毕
		{
			token temp ;//构造一个空token
			list_token.push_back(temp);//在token序列尾部加入一个空token 便于后续语法分析
		}
		printTab();
		return 1;
	}
	else {
		return state;
	}
}

void test() {
	for (int i = 0; i < list_Keyword.size(); ++i) {
		std::cout << list_Keyword[i] << std::endl;
	}
}