#pragma once
#ifndef __SCANER
#define __SCANER
#include "defs.h"
class TScaner {
private:
	TypeMod t; // исходный текст
	int uk; // указатель текущей позиции в исходном тексте
	int uks;
public:
	void PutUK(int i);
	int GetUK(void);
	void PutUKS(int i);
	int GetUKS(void);
	void PrintError(char *, char *, int);
	void  PrintError(char  *  err);
	void PrintError(char  *, char  *);
	int Scaner(TypeLex l);
	void GetData(char *);
	TScaner(char *);
	~TScaner() {}
};
#endif
