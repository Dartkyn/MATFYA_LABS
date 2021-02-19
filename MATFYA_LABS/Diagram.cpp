#include "stdafx.h"
#include  "defs.h"
#include  "Scaner.h"
#include "Semant.h"
#include  "Diagram.h"

void Diagram::Program()
// программа
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
//функции				   
{
	TypeLex l;
	int t, uk1, uk2;
	TData type;
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
			sc->PrintError("Ожидался символ main", l, sc->GetUKS());
		}
		v = root->SemIncludeFunct(l, type.dataType);
		t = sc->Scaner(l);
		if (t != TBraceOp)
		{
			sc->PrintError("Ожидался символ (", l, sc->GetUKS());
		}
		t = sc->Scaner(l);
		if (t != TBraceCl)
		{
			sc->PrintError("Ожидался символ )", l, sc->GetUKS());
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
				sc->PrintError("Ожидался идентификатор", l, sc->GetUKS());
			}
			v = root->SemIncludeFunct(l, type.dataType);
			t = sc->Scaner(l);
			if (t != TBraceOp)
			{
				sc->PrintError("Ожидался символ (", l, sc->GetUKS());
			}
			int pm=Param();
			root->SemSetParam(v, pm);
			t = sc->Scaner(l);
			if (t != TBraceCl)
			{
				sc->PrintError("Ожидался символ )", l, sc->GetUKS());
			}
		}
	Block();
	root->SetCur(v);
}

int Diagram::Param()
{
	//параметры функции
	TypeLex l;
	int t, uk1, uk2,pm=0;
	TData type;
	uk1 = sc->GetUK();
	uk2 = sc->GetUKS();
	do {
		t = sc->Scaner(l);
		if ((t != Tsint) && (t != Tlint))
		{
			sc->PrintError("Ожидался символ short или long", l, sc->GetUKS());
		}
		type = root->SemGetDataType(t);
		t = sc->Scaner(l);
		if (t != Tint)
		{
			sc->PrintError("Ожидался символ  int", l, sc->GetUKS());
		}
		t = sc->Scaner(l);
		if (t != TIdent)
		{
			sc->PrintError("Ожидался идентификатор", l, sc->GetUKS());
		}
		root->SemIncludeVar(l, type.dataType);
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
// описание данных
{
	TypeLex l, a;;
	TData type, type1;
	int t, uk1, uk2;
	Tree* v;
	t = sc->Scaner(l);
	if ((t == Tsint) || (t == Tlint))
	{
		type = root->SemGetDataType(t);
		t = sc->Scaner(l);
		if (t != Tint)
		{
			sc->PrintError("Ожидался символ int", l, sc->GetUKS());
		}
		
		do {
			uk1 = sc->GetUK();
			uk2 = sc->GetUKS();
			t = sc->Scaner(l);
			if (t != TIdent)
			{
				sc->PrintError("Ожидался идентификатор", l, sc->GetUKS());
			}
			root->SemIncludeVar(l, type.dataType);
			strcpy(a, l);
			uk1 = sc->GetUK();
			uk2 = sc->GetUKS();
			t = sc->Scaner(l);
			if (t == TAssign)
			{
				v = root->SemGetVar(a);
				Expression(type1);
				root->SemPutValue(v, type1);
				uk1 = sc->GetUK();
				uk2 = sc->GetUKS();
				t = sc->Scaner(l);
			}

		} while (t == TComm);
		if (t != TSecolon)
		{
			sc->PrintError("Ожидается ;", l, sc->GetUKS());
		}
	}
}

void Diagram::Block()
// Блок
{
	TypeLex l;
	int t, uk1, uk2;
	t = sc->Scaner(l);
	if (t != TBracketOp)
	{
		sc->PrintError("Ожидался  символ {", l, sc->GetUKS());
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
	root->DeleteBlock();
	t = sc->Scaner(l);
	if (t != TBracketCl)
	{
		sc->PrintError("Ожидался  символ }", l, sc->GetUKS());
	}
}

void Diagram::Operator()
// Оператор
{
	TypeLex  l, a;
	int  t, uk1, uk2;
	TData type;
	uk1 = sc->GetUK();
	uk2 = sc->GetUKS();
	t = sc->Scaner(l);
	if (t == TSecolon)
		return;  //  пустой  оператор
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
				sc->PrintError("Ожидался символ (", l, sc->GetUKS());
			}
			Tree *v = root->SemGetFunct(a);
			int ct=0;
			do
			{

				Expression(type);

				ct++;
				uk1 = sc->GetUK();
				uk2 = sc->GetUKS();
				t = sc->Scaner(l);
			} while (t == TComm);
			root->SemControlParam(v, ct);
			if (t != TBraceCl)
			{
				sc->PrintError("Ожидался символ )", l, sc->GetUKS());
			}
			t = sc->Scaner(l);
			if (t != TSecolon)
			{
				sc->PrintError("Ожидался символ ;", l, sc->GetUKS());
			}
			return;
		}

	}
	sc->PutUK(uk1);
	sc->PutUKS(uk2);
	Block();
}

void Diagram::Assignment()
// Присваивание	
{
	TypeLex l, a;
	TData type;
	Tree *v;
	int t;
	t = sc->Scaner(l);
	if (t != TIdent)
	{
		sc->PrintError("Ожидался  идентификатор", l, sc->GetUKS());
	}
	strcpy(a, l);
	t = sc->Scaner(l);
	if (t != TAssign)
	{
		sc->PrintError("Ожидался символ =", l, sc->GetUKS());
	}
	else
	{
		v = root->SemGetVar(a);
		Expression(type);
		root->SemPutValue(v, type);
		t = sc->Scaner(l);
		if (t != TSecolon)
		{
			sc->PrintError("Ожидался символ ;", l, sc->GetUKS());
		}
	}
}

void Diagram::If()
// If
{
	TypeLex l;
	int t, uk1, uk2;
	t = sc->Scaner(l);
	TData type;
	if (t != Tif)
	{
		sc->PrintError("Ожидался  символ if", l, sc->GetUKS());
	}
	t = sc->Scaner(l);
	if (t != TBraceOp)
	{
		sc->PrintError("Ожидался  символ (", l, sc->GetUKS());
	}
	Expression(type);
	t = sc->Scaner(l);
	if (t != TBraceCl)
	{
		sc->PrintError("Ожидался  символ )", l, sc->GetUKS());
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

void Diagram::Expression(TData& type)
// Выражение
{
	TypeLex  l;
	int  t, uk1, uk2;
	TData type1;
	Calc(type);;
	uk1 = sc->GetUK();
	uk2 = sc->GetUKS();
	t = sc->Scaner(l);
	while ((t <= TNotEqual) && (t >= TMore))  //  знаки  сравнения  стоят  подряд
	{
		Calc(type1);
		type = root->SemResultOperation(type, type1, t);
		uk1 = sc->GetUK();
		uk2 = sc->GetUKS();
		t = sc->Scaner(l);
	}
	sc->PutUK(uk1);
	sc->PutUKS(uk2);
}

void Diagram::Calc(TData& type)
//	Подсчет сдвигом
{
	TypeLex  l;
	int  t, uk1, uk2;
	TData type1;
	Arithmetic(type);
	uk1 = sc->GetUK();
	uk2 = sc->GetUKS();
	t = sc->Scaner(l);
	while ((t == TShiftL) || (t == TShiftR))
	{
		Arithmetic(type1);
		type = root->SemResultOperation(type, type1, t);
		uk1 = sc->GetUK();
		uk2 = sc->GetUKS();
		t = sc->Scaner(l);
	}
	sc->PutUK(uk1);
	sc->PutUKS(uk2);
}

void Diagram::Arithmetic(TData& type)
// Арифметика
{
	TypeLex  l;
	int  t, uk1, uk2;
	TData type1;
	Mul_div(type);
	uk1 = sc->GetUK();
	uk2 = sc->GetUKS();
	t = sc->Scaner(l);
	while ((t == TAdd) || (t == TSub))
	{
		Mul_div(type1);
		type = root->SemResultOperation(type, type1, t);
		uk1 = sc->GetUK();
		uk2 = sc->GetUKS();
		t = sc->Scaner(l);
	}
	sc->PutUK(uk1);
	sc->PutUKS(uk2);
}

void Diagram::Mul_div(TData& type)
// Умножение/деление
{
	TypeLex  l;
	int  t, uk1, uk2;
	TData type1;
	Bracket(type);
	uk1 = sc->GetUK();
	uk2 = sc->GetUKS();
	t = sc->Scaner(l);
	while ((t == TDiv) || (t == TMult))
	{
		Bracket(type1);
		type = root->SemResultOperation(type, type1, t);
		uk1 = sc->GetUK();
		uk2 = sc->GetUKS();
		t = sc->Scaner(l);
	}
	sc->PutUK(uk1);
	sc->PutUKS(uk2);
}

void Diagram::Bracket(TData& type)
// Скобки
{
	TypeLex  l;
	int  t, uk1, uk2;
	t = sc->Scaner(l);
	if ((t == TCons10) || (t == TCons16))
	{
		int count = 0;
		for (int i = 0; l[i]!= '\0'; i++)
		{
			count++;
		}
		if (count <= 6)
		{
			type.dataType = TYPE_SHORT_INTEGER;
			type.dataValue.dataAsSInt = atoi(l);
		}
		else
		{
			type.dataType = TYPE_LONG_INTEGER;
			type.dataValue.dataAsSInt = atol(l);
		}
		return;
	}
	if (t == TBraceOp)
	{
		Expression(type);
		t = sc->Scaner(l);
		if (t != TBraceCl)
		{
			sc->PrintError("ожидался  символ  )", l, sc->GetUKS());
		}
	}
	if (t == TIdent)
	{
		Tree *v = root->SemGetVar(l);
		type = root->SemGetType(v);
	}
	else
	 {
		sc->PrintError("ожидался  идентификатор или константа, или выражение", l, sc->GetUKS());
	}	
}

bool Diagram::SetFlagIntr()
{
	return true;
}

bool Diagram::GetFlagIntr()
{
	return false;
}