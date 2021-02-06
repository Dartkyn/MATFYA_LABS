#include "stdafx.h"
#include  "defs.h"
#include  "Scaner.h"
#include "Semant.h"
#include  "Diagram.h"

void Diagram::Program()
// ���������
{
	TypeLex l;
	int t, uk1, uk2;
	uk1 = sc->GetUK();
	uk2 = sc->GetUKS();
	t = sc->Scaner(l);
	sc->PutUK(uk1);
	sc->PutUKS(uk2);
	while ((t == Tsint) || (t == Tlint) || (t == Tint) || (t == Tvoid))
	{
		if ((t == Tsint) || (t == Tlint))
			Data();
		else
			Funct();
		uk1 = sc->GetUK();
		uk2 = sc->GetUKS();
		t = sc->Scaner(l);
		sc->PutUK(uk1);
		sc->PutUKS(uk2);
	}
}

void Diagram::Funct()
//�������				   
{
	TypeLex l;
	int t, uk1, uk2;
	DATA_TYPE type;
	Tree *v;
	uk1 = sc->GetUK();
	uk2 = sc->GetUKS();
	t = sc->Scaner(l);
	if (t == Tint)
	{
		type = root->SemGetDataType(t);
		t = sc->Scaner(l);
		if (t != Tmain)
		{
			sc->PrintError("�������� ������ main", l, sc->GetUKS());
		}
		v = root->SemIncludeFunct(l, type);
		t = sc->Scaner(l);
		if (t != TBraceOp)
		{
			sc->PrintError("�������� ������ (", l, sc->GetUKS());
		}
		t = sc->Scaner(l);
		if (t != TBraceCl)
		{
			sc->PrintError("�������� ������ )", l, sc->GetUKS());
		}
	}
	else

		if (t == Tvoid)
		{
			type = root->SemGetDataType(t);
			uk1 = sc->GetUK();
			uk2 = sc->GetUKS();
			t = sc->Scaner(l);
			if (t != TIdent)
			{
				sc->PrintError("�������� �������������", l, sc->GetUKS());
			}
			v = root->SemIncludeFunct(l, type);
			t = sc->Scaner(l);
			if (t != TBraceOp)
			{
				sc->PrintError("�������� ������ (", l, sc->GetUKS());
			}
			int pm=Param();
			root->SemSetParam(v, pm);
			t = sc->Scaner(l);
			if (t != TBraceCl)
			{
				sc->PrintError("�������� ������ )", l, sc->GetUKS());
			}
		}
	Block();
	root->SetCur(v);
}

int Diagram::Param()
{
	//��������� �������
	TypeLex l;
	int t, uk1, uk2,pm=0;
	DATA_TYPE type;
	uk1 = sc->GetUK();
	uk2 = sc->GetUKS();
	do {
		t = sc->Scaner(l);
		if ((t != Tsint) && (t != Tlint))
		{
			sc->PrintError("�������� ������ short ��� long", l, sc->GetUKS());
		}
		type = root->SemGetDataType(t);
		t = sc->Scaner(l);
		if (t != Tint)
		{
			sc->PrintError("�������� ������  int", l, sc->GetUKS());
		}
		t = sc->Scaner(l);
		if (t != TIdent)
		{
			sc->PrintError("�������� �������������", l, sc->GetUKS());
		}
		root->SemIncludeVar(l, type);
		pm++;
		uk1 = sc->GetUK();
		uk2 = sc->GetUKS();
		t = sc->Scaner(l);
	} while (t == TComm);
	sc->PutUK(uk1);
	sc->PutUKS(uk2);
	return pm;
}
void Diagram::Data()
// �������� ������
{
	TypeLex l;
	DATA_TYPE type, type1;
	int t, uk1, uk2;
	t = sc->Scaner(l);
	if ((t == Tsint) || (t == Tlint))
	{
		type = root->SemGetDataType(t);
		t = sc->Scaner(l);
		if (t != Tint)
		{
			sc->PrintError("�������� ������ int", l, sc->GetUKS());
		}
		
		do {
			uk1 = sc->GetUK();
			uk2 = sc->GetUKS();
			t = sc->Scaner(l);
			if (t != TIdent)
			{
				sc->PrintError("�������� �������������", l, sc->GetUKS());
			}
			root->SemIncludeVar(l, type);
			uk1 = sc->GetUK();
			uk2 = sc->GetUKS();
			t = sc->Scaner(l);
			if (t == TAssign)
			{
				/*t = sc->Scaner(l);
				if (t != TCons10)
				{
					sc->PrintError("��������� ���������", l, sc->GetUKS());
				}*/
				type1 = Expression();
				uk1 = sc->GetUK();
				uk2 = sc->GetUKS();
				t = sc->Scaner(l);
			}

		} while (t == TComm);
		if (t != TSecolon)
		{
			sc->PrintError("��������� ;", l, sc->GetUKS());
		}
	}
}

void Diagram::Block()
// ����
{
	TypeLex l;
	int t, uk1, uk2;
	t = sc->Scaner(l);
	if (t != TBracketOp)
	{
		sc->PrintError("��������  ������ {", l, sc->GetUKS());
	}
	Tree *vb = root->SemIncludeBlock();
	uk1 = sc->GetUK();
	uk2 = sc->GetUKS();
	t = sc->Scaner(l);
	sc->PutUK(uk1);
	sc->PutUKS(uk2);
	while (t == Tsint || t == Tlint || t == Tif || t == TBracketOp || t == TIdent||t==TSecolon)
	{
		if (t == Tsint || t == Tlint)
		{
			Data();
		}
		else
		{
			Operator();
		}
		uk1 = sc->GetUK();
		uk2 = sc->GetUKS();
		t = sc->Scaner(l);
		sc->PutUK(uk1);
		sc->PutUKS(uk2);
	}
	root->SetCur(vb);
	t = sc->Scaner(l);
	if (t != TBracketCl)
	{
		sc->PrintError("��������  ������ }", l, sc->GetUKS());
	}
}

void Diagram::Operator()
// ��������
{
	TypeLex  l, a;
	int  t, uk1, uk2;
	DATA_TYPE type;
	uk1 = sc->GetUK();
	uk2 = sc->GetUKS();
	t = sc->Scaner(l);
	if (t == TSecolon)
		return;  //  ������  ��������
	if (t == Tif)
	{
		sc->PutUK(uk1);
		sc->PutUKS(uk2);
		If();
		return;
	}
	if (t == TIdent)
	{
		strcpy(a, l);
		t = sc->Scaner(l);
		
		if (t == TAssign)
		{
			sc->PutUK(uk1);
			sc->PutUKS(uk2);
			Assignment();
			return;
		}
		else
		{
			if (t != TBraceOp)
			{
				sc->PrintError("�������� ������ (", l, sc->GetUKS());
			}
			Tree *v = root->SemGetFunct(a);
			int ct=0;
			do
			{
				uk1 = sc->GetUK();
				uk2 = sc->GetUKS();
				t = sc->Scaner(l);
				/*if (t != TIdent)
				{
					sc->PrintError("�������� �������������", l, sc->GetUKS());
				}*/
				type = Expression();
				//Tree *vb = root->SemGetVar(l);
				ct++;
				uk1 = sc->GetUK();
				uk2 = sc->GetUKS();
				t = sc->Scaner(l);
			} while (t == TComm);
			root->SemControlParam(v, ct);
			if (t != TBraceCl)
			{
				sc->PrintError("�������� ������ )", l, sc->GetUKS());
			}
			t = sc->Scaner(l);
			if (t != TSecolon)
			{
				sc->PrintError("�������� ������ ;", l, sc->GetUKS());
			}
			return;
		}

	}
	sc->PutUK(uk1);
	sc->PutUKS(uk2);
	Block();
}

void Diagram::Assignment()
// ������������	
{
	TypeLex l, a;
	DATA_TYPE type;
	Tree *v;
	int t;
	t = sc->Scaner(l);
	if (t != TIdent)
	{
		sc->PrintError("��������  �������������", l, sc->GetUKS());
	}
	strcpy(a, l);
	t = sc->Scaner(l);
	if (t != TAssign)
	{
		sc->PrintError("�������� ������ =", l, sc->GetUKS());
	}
	else
	{
		v = root->SemGetVar(a);
		Expression();
		t = sc->Scaner(l);
		if (t != TSecolon)
		{
			sc->PrintError("�������� ������ ;", l, sc->GetUKS());
		}
	}
}

void Diagram::If()
// If
{
	TypeLex l;
	int t, uk1, uk2;
	t = sc->Scaner(l);
	if (t != Tif)
	{
		sc->PrintError("��������  ������ if", l, sc->GetUKS());
	}
	t = sc->Scaner(l);
	if (t != TBraceOp)
	{
		sc->PrintError("��������  ������ (", l, sc->GetUKS());
	}
	Expression();
	t = sc->Scaner(l);
	if (t != TBraceCl)
	{
		sc->PrintError("��������  ������ )", l, sc->GetUKS());
	}
	Operator();
	uk1 = sc->GetUK();
	uk2 = sc->GetUKS();
	t = sc->Scaner(l);
	sc->PutUK(uk1);
	sc->PutUKS(uk2);
	if (t == Telse)
	{
		t = sc->Scaner(l);
		Operator();
	}
}

DATA_TYPE Diagram::Expression()
// ���������
{
	TypeLex  l;
	int  t, uk1, uk2;
	DATA_TYPE type, type1;
	type = Calc();;
	uk1 = sc->GetUK();
	uk2 = sc->GetUKS();
	t = sc->Scaner(l);
	while ((t <= TNotEqual) && (t >= TMore))  //  �����  ���������  �����  ������
	{
		type1 = Calc();
		type = root->SemResultOperation(type, type1, t);
		uk1 = sc->GetUK();
		uk2 = sc->GetUKS();
		t = sc->Scaner(l);
	}
	sc->PutUK(uk1);
	sc->PutUKS(uk2);
	return type;
}

DATA_TYPE Diagram::Calc()
//	������� �������
{
	TypeLex  l;
	int  t, uk1, uk2;
	DATA_TYPE type, type1;
	type=Arithmetic();
	uk1 = sc->GetUK();
	uk2 = sc->GetUKS();
	t = sc->Scaner(l);
	while ((t == TShiftL) || (t == TShiftR))
	{
		type1 = Arithmetic();
		type = root->SemResultOperation(type, type1, t);
		uk1 = sc->GetUK();
		uk2 = sc->GetUKS();
		t = sc->Scaner(l);
	}
	sc->PutUK(uk1);
	sc->PutUKS(uk2);
	return type;
}
DATA_TYPE Diagram::Arithmetic()
// ����������
{
	TypeLex  l;
	int  t, uk1, uk2;
	DATA_TYPE type, type1;
	type = Mul_div();
	uk1 = sc->GetUK();
	uk2 = sc->GetUKS();
	t = sc->Scaner(l);
	while ((t == TAdd) || (t == TSub))
	{
		type1 = Mul_div();
		type = root->SemResultOperation(type, type1, t);
		uk1 = sc->GetUK();
		uk2 = sc->GetUKS();
		t = sc->Scaner(l);
	}
	sc->PutUK(uk1);
	sc->PutUKS(uk2);
	return type;
}

DATA_TYPE Diagram::Mul_div()
// ���������/�������
{
	TypeLex  l;
	int  t, uk1, uk2;
	DATA_TYPE type, type1;
	type = Bracket();
	uk1 = sc->GetUK();
	uk2 = sc->GetUKS();
	t = sc->Scaner(l);
	while ((t == TDiv) || (t == TMult))
	{
		type1 = Bracket();
		type = root->SemResultOperation(type, type1, t);
		uk1 = sc->GetUK();
		uk2 = sc->GetUKS();
		t = sc->Scaner(l);
	}
	sc->PutUK(uk1);
	sc->PutUKS(uk2);
	return type;
}

DATA_TYPE Diagram::Bracket()
// ������
{
	TypeLex  l;
	int  t, uk1, uk2;
	DATA_TYPE type;
	t = sc->Scaner(l);
	if ((t == TCons10) || (t == TCons16))
	{
		return TYPE_SHORT_INTEGER;
	}
	if (t == TBraceOp)
	{
		type=Expression();
		t = sc->Scaner(l);
		if (t != TBraceCl)
		{
			sc->PrintError("��������  ������  )", l, sc->GetUKS());
		}
		return type;
	}
	if (t == TIdent)
	{
		Tree *v = root->SemGetVar(l);
		return root->SemGetType(v);
	}
	else
	 {
		sc->PrintError("��������  ������������� ��� ���������, ��� ���������", l, sc->GetUKS());
	}
	
}