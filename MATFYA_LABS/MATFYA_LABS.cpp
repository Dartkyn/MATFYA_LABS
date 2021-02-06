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
	if (argc <= 1) sc = new TScaner("input.txt");// ���� �� ���������
	else sc = new TScaner(argv[1]); // �������� ����
	Tree *root = new Tree();
	root->SetCur(root);
	Diagram *dg = new Diagram(sc, root);
	dg->Program();
	root->Print();
	type = sc->Scaner(l);
	if (type == TEnd)
		printf("��������������  ������  ��  ����������.  \n");
	else
		sc->PrintError("������  �����  �  �����  ���������.", "");
	system("pause");
	return 0;
}