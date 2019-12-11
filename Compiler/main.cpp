#include "LexicalAnalyzer.h"
//#include "TranslationGrammarAnalyzer.h"
int main() {
	int state = LexicalAnalyzer();
	if (state <0) {
		std::cout << "Error "<< state <<" LexicalAnalyzer()" << std::endl;
	}

	std::cout <<'\n';

	/*
	if (Recursive()) {
		std::cout << "Right!" << std::endl;
	}
	else {
		std::cout << "Error!" << std::endl;
	}
	
	if (LL1()>0) {
		std::cout << "Right!" << std::endl;
	}
	else {
		std::cout << "Error!" << std::endl;
	}*/
	return 0;
}
