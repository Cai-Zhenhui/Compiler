/*
1\	(1+2)-(3-4)		������<<=���������ʶ��
2\	token.pos+20	
*/
/*
note:
1\fstream��>>����������˿հ׷� \n \t space��
2\fstream.read() ���\n(0x0a)\r(0x0d)����һ���ֽڶ��� ֻ����\n(0x0a)

*/
#include "LexicalAnalyzer.h"

#define FILE_NAME_Source "test.cpp"

using namespace std;

fstream file;


sToken sWord;//һ������
										
/*
��ʼ���ʷ������� ��ȡ�ؼ��ֱ�ͽ����
return
1	�ɹ�
-1	��Tab_Delimiter.txt����ʧ��
-2	��Tab_Keyword.txt����ʧ��
*/
int Init_LexicalAnalyzer() {
	using namespace std;
	ifstream file;
	
	//��ȡ�����
	file.open(FILENAME_DELIMITER);
	if (!file.is_open()) {
		return -1;//
	}
	while (!file.eof()) {
		string str;//���õ����ڴ�����⣬c++ STL��string���ڳ������н���ʱ�ͷ��ڴ�
		file >> str;
		list_Delimiter.push_back(str);
	}
	file.close();

	//��ȡ�ؼ��ֱ�
	file.open(FILENAME_KEYWORD);
	if (!file.is_open()) {
		return -2;//
	}
	while (!file.eof()) {
		string str;//���õ����ڴ�����⣬c++ STL��string���ڳ������н���ʱ�ͷ��ڴ�
		file >> str;
		list_Keyword.push_back(str);
	}
	file.close();

	return true;
}

//��ӡ�����������
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
0 ����
1 float
*/
int String2Number(void * number) {
	//���������ܻ��е�����
	//string�����ĵĻ� emmmm ����de
	if (sWord.word.find('.') == std::string::npos) {
		//����
		sscanf(&sWord.word[0], "%d", (int*)number);
		return 0;
	}
	else {
		sscanf(&sWord.word[0], "%f", (float*)number);
		return 1;
	}
}

/*
����ֵ��
NN	FSM��������
-1	�Ƿ��ַ�,�����ַ���
-2	FSM ״̬����
-3	�ַ����� ' ��ƥ��
-4	�ַ������� " ��ƥ��
-5	����С�������
-6	���ƥ�����
*/
int FSM(int state,char &ch) {
	switch (state){
	case 0: {
		break;
	}
	case 1: {//1״̬ ��ʼ״̬ 
		//���״̬���������ַ�
		if (isIdentifier(ch)) {
			//��ʶ����ؼ���
			sWord.word.push_back(ch);
			//sWord.type = TokenType;//���ﲻȷ��
			return 2;//change to s2
		}
		else if (ch >= '0' && ch <= '9') {
			//���� ����
			sWord.word.push_back(ch);
			sWord.type = TokenType::ConstantInt;
			return 3;
		}
		else if (ch == '\'') {
			//�ַ�����  ��ʱ�����ǺϷ���
			sWord.word.push_back(ch);
			sWord.type = TokenType::CharM;
			return 5;
		}
		else if (ch == '\"') {
			//�ַ������� 
			sWord.word.push_back(ch);
			sWord.type = TokenType::StringM;
			return 6;
		}

		else {//������ �Ͳ��ɼ�����
			
			  //������
			for (int i = 0; i < list_Delimiter.size(); ++i) {
				if (list_Delimiter[i][0] == ch) {//ƥ���һλ
					sWord.word.push_back(ch);
					sWord.type = TokenType::Delimiter;
					sWord.pos = i;
					return 4;//���
				}
			}

			//���ڽ����

			
			switch (ch){
			case 0x09: {//\t
				break;//���� ������word �ݲ�����
			}
			case 0x0a: {//\n
				break;//���� ������word �ݲ�����
			}
			case 0x20: {
				break;//���� ������word �ݲ�����
			}
			default:
				//�Ƿ��ַ�������
				return -1;//һ������²������ �����ĵ����ƻ�
			}
			return 1;//���Կհ׷�
		}
	}
	case 2: {//��ʶ����ؼ��ַ�֧ 
		//��״̬�����ǳ����ַ�������ַ� �ɽ��ܵ��ַ�={��ĸ,_,����}
		if (isIdentifier(ch) || 
			(ch == '_')||
			(ch >= '0' && ch <= '9')) {
			
			sWord.word.push_back(ch);
			return 2;//ѭ��
		}
		else {
			
			return 0;//�������
		}
	}
	case 3: {//������֧
		//��״̬�����ǳ����ַ�������ַ� �ɽ��ܵ��ַ�={����,.}//��ʱ�����ǿ�ѧ������
		if ((ch >= '0' && ch <= '9')||
			(ch=='.')) {
			if (ch == '.') {
				sWord.type = TokenType::ConstantFloat;
				sWord.word.push_back(ch);
				return 7;//�Ժ�������'.'//�Ӵ˱��float
			}
			sWord.word.push_back(ch);
			return 3;//ѭ��
		}
		else {
			return 0;//�������
		}
	}
	case 4: {//�����֧
		//������
		
		sWord.word.push_back(ch);//�Ȱ�Ҫƥ����ַ����� һƥ����ַ�����
								 //��������Ƚϣ��Ƚ����֮�����ƥ�䵽�˾Ͳ���ɾ��
								 //���û��ƥ�䵽���ͽ�������ַ�ɾ��
		int length_word = sWord.word.size();
		for (int i = 0; i < list_Delimiter.size(); ++i) {
			//ƥ�� ǰ(sWord.word.size();)���ַ�
			if (sWord.word.compare(list_Delimiter[i].substr(0, length_word)) == 0) {
				//ƥ�䵽
				sWord.pos = i;// ����ƥ�� i�Ͽ��ܻᷢ���仯
				return 4; //�����һ�����
			}
		}
		//���������֧�Ļ� ˵�������������� ')+'�������������
		//��һ�������� ����λ�ÿ��ܲ���ȷ
		sWord.word.pop_back();//ɾ��ǰ������ƥ����ַ�
		//����ƥ��λ�� ��ʱ�ǽ��ȷ����
		for (int i = 0; i < list_Delimiter.size(); ++i) {
			if (sWord.word.compare(list_Delimiter[i]) == 0) {//������ȫƥ��
				//ƥ�䵽
				sWord.pos = i;// ����ƥ�� i�Ͽ��ܻᷢ���仯
				return 0;
			}
		}
		//Error
		return -6;
	}
	case 5: {//�ַ�������֧
		//��״̬�����ǳ����ַ�������ַ� �ɽ��ܵ��ַ�={',һ�пɼ��ַ�}//��ʱ�����ǺϷ���
		if (ch == '\'') {//�ַ���������
			sWord.word.push_back(ch);  //���ﵥ������û�ж������ַ� ���º�������֧��һ��
			file.read(&ch, sizeof(char));//�����ٶ���һ���ַ�
			return 0;//�������
		}
		else if (ch >= 0x20 && ch <= 0x7E) {//�ɼ��ַ���Χ include space
			sWord.word.push_back(ch);
			return 5;
		}
		else {
			return -3;
		}
	}
	case 6: {
		//��״̬�����ǳ����ַ�������ַ� �ɽ��ܵ��ַ�={',һ�пɼ��ַ�}//��ʱ�����ǺϷ���
		if (ch == '"') {//�ַ�����������
			sWord.word.push_back(ch);  //���ﵥ������û�ж������ַ� ���º�������֧��һ��
			file.read(&ch, sizeof(char));//�����ٶ���һ���ַ�
			return 0;//�������
		}
		else if (ch >= 0x20 && ch <= 0x7E) {//�ɼ��ַ���Χ include space
			sWord.word.push_back(ch);
			return 6;
		}
		else {
			return -4;
		}
	}
	case 7: {//�����֧
		//��״̬�����ǳ����ַ�������ַ� �ɽ��ܵ��ַ�={����}
		if ((ch >= '0' && ch <= '9')) {
			sWord.word.push_back(ch);
			return 7;//ѭ��
		}
		else if(ch=='.'){
			return -5;
		}
		else {
			return 0;//�������
		}
	}
	default:
		return -2;//FSM ״̬����
	}
}

/*
�ʷ���������ģ�� ʶ����
����ֵ��
0	�������
-1	Դ�ļ�ɨ�����
<-1	FSM���� FSM_error_index=LA_Recognizer_error_index+1
*/
int LA_Recognizer() {
	char ch = 0;
	int state = 1;
	sWord.clear();//���֮ǰ�ĵ���
	while (file.read(&ch, sizeof(char))) {
		//cout << ch ;
		state = FSM(state, ch);
		if (state == 0) {
			file.seekg(-1, std::ios::cur);//����(����˵���ܵ�ԭ�����п����ǿհ׷�)������һ������
			//�����ַ���������᷵���ϲ㡣�ٴν�����ٶ�һ���ַ� �����͵���һ���ַ���©��
			//������Ҫ��ǰ�ƶ�һ���ֽ�
			return 1;//������� 
		}
		else if (state < 0) {
			return state-1;//���ִ��� ���ϴ���
		}
	}
	return -1;//Դ�ļ�ɨ�����
}

/*
�ʷ�������

����ֵ
1	����
<0	���� �鿴�ʷ���������ģ�� ʶ����LA_Recognizer()�Ĵ����б�

*/
int LexicalAnalyzer() {
	
	Init_LexicalAnalyzer();//��ʼ���ʷ�������
	//��ȡԴ���ļ�
	file.open("test.cpp", ios::out | ios::in);
	{
		//��Դ�ļ�β�����Ͽո� ��ֹ��ȡ����
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
				//�ó����Ѵ��� ����Ҫ���
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
				//�ó����Ѵ��� ����Ҫ���
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
				//���ַ��Ѵ���
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
				//���ַ��Ѵ���
			}
			else {
				token.pos = list_String.size();
				list_String.push_back(sWord.word);
			}
			break;
		}
		default: {
			//��ʶ���͹ؼ��ֻ���Ҫ����
			//�Ȳ�ؼ��ֱ�
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
				//��ʶ����
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
					//�Ѵ���
				}
				else {
					token.pos = list_Symbol.size();
					//������ű�
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
		//Դ�ļ�ɨ�����
		{
			token temp ;//����һ����token
			list_token.push_back(temp);//��token����β������һ����token ���ں����﷨����
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