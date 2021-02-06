#include <string.h>
#include "defs.h"
#include "Scaner.h"
TScaner::TScaner(char * FileName) {
	GetData(FileName);
	PutUK(0);
	PutUKS(1);
}
TypeLex Keyword[MAX_KEYW] = {
	"main", "void", "short",
	"long", "if", "else", "int"
};
int IndexKeyword[MAX_KEYW] = {
	Tmain, Tvoid, Tsint,
	Tlint, Tif, Telse,Tint
};
void TScaner::PutUK(int i) { uk = i; } //������������ ���������
int TScaner::GetUK(void) { return uk; } // ��������� ���������
void TScaner::PutUKS(int i) { uks = i; } //������������ ���������
int TScaner::GetUKS(void) { return uks; } // ��������� ���������
void TScaner::PrintError(char * err, char * a, int i) {
	// ������ ��������� �� ������
	if (a[0] == '\0')
		printf("������ : %s %s � ������ %d\n", err, a, i);
	else
		printf("������ : %s. �������� ������ %s � ������ %d\n", err, a, i);
	system("pause");
	exit(0);
}
void TScaner::PrintError(char  *  err, char  *  a)
{
	//  ������  ���������  ��  ������
	if (a[0] == '\0')
		printf("������  :   %s  %s\n", err, a);
	else
		printf("������  :   %s.  ��������  ������  %s\n", err, a);
	system("pause");
	exit(0);
}
void TScaner::PrintError(char  *  err)
{
	printf("������  :   %s  \n", err);
	system("pause");
	exit(0);
}
int TScaner::Scaner(TypeLex l) {
	int i; // ������ ����� �������
	for (i = 0; i<MAX_LEX; i++) l[i] = 0; //�������� ���� �������
	i = 0; // ������� ��������� � ������� i
start: // ��� ������������ ��������:
	while ((t[uk] == ' ') || (t[uk] == '\n') || (t[uk] == '\t'))
	{
		if (t[uk] == '\n')
			uks++;
		uk++;
	}
	// ������� ���������� ���������
	if ((t[uk] == '/') && (t[uk + 1] == '/'))
	{ // ������ �����������, ���� ���������� ����� �� �\n�
		uk = uk + 2;
		while ((t[uk] != '\n') && (t[uk] != '#')) uk++;
		goto start;
	}
	if ((t[uk] == '/') && (t[uk + 1] == '*'))
	{ // ������ �����������, ���� ���������� ����� �� �*/�
		uk = uk + 2;
		while ((t[uk] != '*') && (t[uk + 1] != '/')) uk++;
		goto start;
	}
	if (t[uk] == '\0') { l[0] = '#'; return TEnd; }
	if ((t[uk] <= '9') && (t[uk] >= '0'))
	{
		if (t[uk] == '0' && (t[uk + 1] == 'x' || t[uk + 1] == 'X'))
		{ //������� ����������������� ���������
			l[i++] = t[uk++];
			l[i++] = t[uk++];
			while ((t[uk] <= '9') && (t[uk] >= '0') || (t[uk] <= 'f') && (t[uk] >= 'a') || (t[uk] <= 'F') && (t[uk] >= 'A'))
			{
				if (i<MAX_LEX - 1) l[i++] = t[uk++]; else uk++;
			}
			return TCons16;
		}
		l[i++] = t[uk++];
		while ((t[uk] <= '9') && (t[uk] >= '0'))
			if (i<MAX_LEX - 1) l[i++] = t[uk++];
			else
			{
				while (t[uk] != '\n') uk++;
				PrintError("������� ������� ���������", "", 0);
				return TErr;
			}
		return TCons10;
	}
	else if ((t[uk] >= 'a') && (t[uk] <= 'z') ||
		(t[uk] >= 'A') && (t[uk] <= 'Z'))
	{ // ��������� �������������
		l[i++] = t[uk++];
		while ((t[uk] <= '9') && (t[uk] >= '0') ||
			(t[uk] >= 'a') && (t[uk] <= 'z') ||
			(t[uk] >= 'A') && (t[uk] <= 'Z'))
			if (i<MAX_LEX - 1) l[i++] = t[uk++]; else uk++;
		// ������� ������������� ��������
		int j; // �������� �� �������� �����:
		for (j = 0; j<MAX_KEYW; j++)
			if (strcmp(l, Keyword[j]) == 0) return IndexKeyword[j];
		return TIdent;
	}

	else if (t[uk] == ',')
	{
		l[i++] = t[uk++]; return TComm;
	}
	else if (t[uk] == ';')
	{
		l[i++] = t[uk++]; return TSecolon;
	}
	else if (t[uk] == '(')
	{
		l[i++] = t[uk++]; return TBraceOp;
	}
	else if (t[uk] == ')')
	{
		l[i++] = t[uk++]; return TBraceCl;
	}
	else if (t[uk] == '{')
	{
		l[i++] = t[uk++]; return TBracketOp;
	}
	else if (t[uk] == '}')
	{
		l[i++] = t[uk++]; return TBracketCl;
	}
	else if (t[uk] == '+')
	{
		l[i++] = t[uk++]; return TAdd;
	}
	else if (t[uk] == '-')
	{
		l[i++] = t[uk++]; return TSub;
	}
	else if (t[uk] == '/')
	{
		l[i++] = t[uk++]; return TDiv;
	}
	else if (t[uk] == '*')
	{
		l[i++] = t[uk++]; return TMult;
	}
	else if (t[uk] == '<')
	{
		l[i++] = t[uk++];
		if (t[uk] == '=') { l[i++] = t[uk++]; return TLessOrEqual; }
		else if (t[uk] == '<') { l[i++] = t[uk++]; return TShiftL; }
		return TLess;
	}
	else if (t[uk] == '>')
	{
		l[i++] = t[uk++];
		if (t[uk] == '=') { l[i++] = t[uk++]; return TMoreOrEqual; }
		else if (t[uk] == '>') { l[i++] = t[uk++]; return TShiftR; }
		else return TMore;
	}
	else if (t[uk] == '!')
	{
		l[i++] = t[uk++];
		if (t[uk] == '=') { l[i++] = t[uk++]; return TNotEqual; }
		else {
			PrintError("�������� ������", l, uk); // ������
			return TErr;
		}
	}
	else if (t[uk] == '=')
	{
		l[i++] = t[uk++];
		if (t[uk] == '=') { l[i++] = t[uk++]; return TEqual; }
		else return TAssign;
	}
	else {
		PrintError("�������� ������", l, uk); // ������
		uk++;
		return TErr;
	}
} // ����� Scaner
void TScaner::GetData(char * FileName) {
	// ���� ����� FileName, ������� �������� ����� ��������� �����
	char aa;
	FILE * in = fopen(FileName, "r");
	if (in == NULL) { PrintError("����������� ������� ����", "", 0); /*exit(1)*/; }
	int i = 0;
	while (!feof(in))
	{
		fscanf(in, "%c", &aa);
		if (!feof(in)) t[i++] = aa;
		if (i >= MAX_TEXT - 1)
		{
			PrintError("������� ������� ������ ��������� ������", "", 0);
			break;
		}
	}
	t[i] = '\0'; // ����������� ���� �\0� � ����� ������
	fclose(in);
} // ����� GetData()