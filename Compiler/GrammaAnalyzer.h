#pragma once
#ifndef _GRAMMAANALYZER_H
#define _GRAMMAANALYZER_H
#include "Global.h"
#include "LexicalAnalyzer.h"

/*
note:

*/
bool Recursive();
int Init_GA_LL1();
int LL1();
#endif // !_GRAMMAANALYZER_H
