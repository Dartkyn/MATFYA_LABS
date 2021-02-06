#include "stdafx.h"
#include  <string.h>
#include "defs.h"
#include  "Scaner.h"
#include "Semant.h"
#define max(a,b) a<b? b: a

//����������� ������� ���� � ��������� ������� � �������
Tree::Tree(Tree *l, Tree *r, Tree *u, Node *d)
{
	n = new Node();
	//���������� ������
	Up = u;
	Left = l;
	Right = r;
	memcpy(n, d, sizeof(Node)); //���������� ������
}

//����������� ������� ����� ���� � ������� ������� � �������
Tree::Tree(void)
{
	n = new Node();
	Up = NULL;
	Left = NULL;
	Right = NULL;
	memcpy(n, &("------"), sizeof(Node));
}

Tree::~Tree()
{
}

//������� ������ ������� �� ������� ������� this
void Tree::SetLeft(Node *Data)
{
	Tree *a = new Tree(NULL, NULL, this, Data);	//����� �������
	Left = a;									//������� this � ����� ��������
}

//������� ������� ������� �� ������� ������� this
void Tree::SetRight(Node *Data)
{
	Tree *a = new Tree(NULL, NULL, this, Data);	//����� �������
	Right = a;									//������� this � ����� ��������
}

//����� ������ � ������, ������� �� ������� ������� this
//�� ��� ����� ����� �� ������
Tree * Tree::FindUp(TypeLex id)
{
	return FindUp(this, id);
}

//����� ������ � ������ �� �������� ������� From
//�� ��� ����� ����� �� ������
Tree *Tree::FindUp(Tree *From, TypeLex id)
{
	Tree *i = From;								//������� ������� ������
	while ((i != NULL) && (memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) != 0))
		i = i->Up;									//����������� ������ �� ������
	return i;
}

//����� ������ �������� ������� ������� this
Tree *Tree::FindRightLeft(TypeLex id)
{
	return FindRightLeft(this, id);
}

//����� ������ �������� �������� ������� From
Tree *Tree::FindRightLeft(Tree *From, TypeLex id)
{
	Tree *i = From->Right;						//  �������  �������  ������
	while ((i != NULL) && (memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) != 0))
		i = i->Left;
	//������� ������ ������� �� ����� ������
	return i;
}

//����� �������� id ����� �� ������ �� ������� ������� From.
//����� �������������� �� ����� ������ ����������� �� ����� ������
Tree *Tree::FindUpOneLevel(Tree *From, TypeLex id)
{
	Tree *i = From;			//������� ������� ������
	while ((i != NULL) && (i->Up != NULL) && (i->Up->Right != i))
	{
		if (memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) == 0)
			return i;		//�a��� ����������� �������������
		i = i->Up;			//����������� ������ �� ������
	}
	return NULL;
}

//���������� ��������� ������ ������
void Tree::Print(void)
{
	printf("������� � ������� %s ----->", n->id);
	if (Left != NULL)
		printf("        �����  ������   %s ��� %d", Left->n->id, Left->n->typeObject);
	if (Right != NULL)
		printf("        ������  ������  %s ��� %d", Right->n->id, Right->n->typeObject);
	printf("\n");
	if (Left != NULL)
		Left->Print();
	if (Right != NULL)
		Right->Print();
}

Tree *Tree::Cur = (Tree*)NULL;

//���������� ������� ���� ������
void Tree::SetCur(Tree *a)
{
	Cur = a;
}

//�������� �������� �������� ���� ������
Tree *Tree::GetCur(void)
{
	return Cur;
}

//�������� ��� ��������
DATA_TYPE Tree::SemGetDataType(int a)
{
	if (a == Tsint)
	{
		return TYPE_SHORT_INTEGER;
	}
	if (a == Tlint)
	{
		return TYPE_LONG_INTEGER;
	}
	if (a == Tvoid)
	{
		return TYPE_VOID;
	}
	return TYPE_INTEGER;

}

//��������� �������������� a � ������� � ����� t
void Tree::SemIncludeVar(TypeLex a, DATA_TYPE dt)
{
	if (DupControl(Cur, a))
		sc->PrintError("��������� �������� �������������� ", a);
	Tree *v;
	Node b;
	memcpy(b.id, a, strlen(a) + 1);
	b.typeObject = OBJ_VAR;
	b.dataType = dt;
	b.param = 0;
	Cur->SetLeft(&b);	//������� ������� - ����������
	Cur = Cur->Left;
}

//
Tree *Tree::SemIncludeFunct(TypeLex a, DATA_TYPE dt)
{
	if (DupControl(Cur, a))
		sc->PrintError("��������� �������� �������������� ", a);
	Tree *v;
	Node b;
	memcpy(b.id, a, strlen(a) + 1);
	b.typeObject = OBJ_FUNCT;
	b.dataType = dt;
	b.param = 0;
	Cur->SetLeft(&b);	//������� ������� - �������
	Cur = Cur->Left;
	v = Cur;			//��� ����� �������� ����� ������ �� �������
	memcpy(&b.id, &"", 2);
	b.typeObject = EMPT;
	b.dataType = EMPTY;
	b.param = EMPTY;
	Cur->SetRight(&b); //������� ������ �������
	Cur = Cur->Right;
	return v;
}

//
Tree *Tree::SemIncludeBlock()
{
	Tree *v;
	Node b;
	memcpy(&b.id, &"", 2);
	b.typeObject = EMPT;
	b.dataType = EMPTY;
	b.param = EMPTY;
	Cur->SetLeft(&b);	//������� ������� - �������
	Cur = Cur->Left;
	v = Cur;
	memcpy(&b.id, &"", 2);
	b.typeObject = EMPT;
	b.dataType = EMPTY;
	b.param = EMPTY;
	Cur->SetRight(&b); //������� ������ �������
	Cur = Cur->Right;
	return v;
}


//
DATA_TYPE Tree::SemResultOperation(DATA_TYPE t1, DATA_TYPE t2, int op)
{
		if (t1 == TYPE_LONG_INTEGER && t2 == TYPE_LONG_INTEGER)
			return TYPE_LONG_INTEGER;
		else
		{
			if ((t1 == TYPE_SHORT_INTEGER && t2 == TYPE_LONG_INTEGER)|| (t2 == TYPE_SHORT_INTEGER && t1 == TYPE_LONG_INTEGER))
				return TYPE_LONG_INTEGER;
			return TYPE_SHORT_INTEGER;
		}

}
void Tree::SemSetParam(Tree* Addr, int num)
// ���������� ����� ���������� ���������� n ��� ������� �� ������ Addr
{
	Addr->n->param = num;
}
void Tree::SemControlParam(Tree *Addr, int num)
// ��������� ��������� ����� ���������� ���������� ��������
// n ��� ������� �� ������ Addr
{
	if (num != Addr->n->param)
		sc->PrintError("�������� ����� ���������� � ������� ", Addr->n->id);
}

//����� � ������� ���������� � ������ a
//� ������� ������ �� ��������������� ������� ������
Tree *Tree::SemGetVar(TypeLex a)
{
	Tree *v = FindUp(Cur, a);
	if (v == NULL)
		sc->PrintError("����������� �������� �������������� ", a);
	if (v->n->typeObject != OBJ_VAR)
		sc->PrintError("������������� �� �������� ���������� ", a);
	return v;
}

//����� � ������� ������� � ������ a
//� ������� ������ �� ��������������� ������� ������.
Tree *Tree::SemGetFunct(TypeLex a)
{
	Tree *v = FindUp(Cur, a);
	if (v == NULL)
		sc->PrintError("����������� �������� ������� ", a);
	if (v->n->typeObject != OBJ_FUNCT)
		sc->PrintError("������������� �� �������� �������� ", a);
	return v;
}

//�������� �������������� � �� ��������� �������� ������ �����.
//����� �������������� ����� �� ������� Addr.
int Tree::DupControl(Tree *Addr, TypeLex a)
{
	if (FindUpOneLevel(Addr, a) == NULL)
		return 0;
	return 1;
}

//
DATA_TYPE Tree::SemGetType(Tree *Addr)
{
	return Addr->n->dataType;
}