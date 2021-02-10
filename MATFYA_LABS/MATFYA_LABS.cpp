#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "Scaner.h"
#include "locale.h"
#include "Diagram.h"
int main(int argc, char * argv[]) {
	setlocale(LC_ALL, "Russian");
	TScaner * sc;
	int type; TypeLex l;
	if (argc <= 1) sc = new TScaner("input.txt");// файл по умолчанию
	else sc = new TScaner(argv[1]); // заданный файл
	Tree *root = new Tree();
	root->SetCur(root);
	Diagram *dg = new Diagram(sc, root);
	dg->Program();
	root->Print();
	type = sc->Scaner(l);
	if (type == TEnd)
		printf("Синтаксических  ошибок  не  обнаружено.  \n");
	else
		sc->PrintError("Лишний  текст  в  конце  программы.", "");
	system("pause");
	return 0;
}