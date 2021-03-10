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
	int t, uk1, uk2, uk3;
	TData type;
	Tree *v = root->GetCur();
	uk1 = sc->GetUK();
	uk2 = sc->GetUKS();
	t = sc->Scaner(l);
	if (t == Tint)
	{
		if (root->GetFlagIntr())
		{
			type = root->SemGetDataType(t);
		}
		t = sc->Scaner(l);
		if (t != Tmain)
		{
			sc->PrintError("Ожидался символ main", l, sc->GetUKS());
		}
		if (root->GetFlagIntr())
		{
			v = root->SemIncludeFunct(l, type.dataType);
		}
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
			if (root->GetFlagIntr())
			{
				type = root->SemGetDataType(t);
			}
			uk1 = sc->GetUK();
			uk2 = sc->GetUKS();
			t = sc->Scaner(l);
			if (t != TIdent)
			{
				sc->PrintError("Ожидался идентификатор", l, sc->GetUKS());
			}
			if (root->GetFlagIntr())
			{
				v = root->SemIncludeFunct(l, type.dataType);
			}
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
			uk3 = sc->GetUK();
			root->PutPosition(v, uk3);
			root->SetFlagIntr(false);
		}
	Block();
	root->SetFlagIntr(true);
	root->SetCur(v);
}

int Diagram::Param()
{
	//параметры функции
	TypeLex l;
	int t, uk1, uk2,pm=0;
	TData type;
	type.dataType = TYPE_INTEGER;
	uk1 = sc->GetUK();
	uk2 = sc->GetUKS();
	do {
		t = sc->Scaner(l);
		if ((t != Tsint) && (t != Tlint))
		{
			sc->PrintError("Ожидался символ short или long", l, sc->GetUKS());
		}
		if (root->GetFlagIntr())
		{
			type = root->SemGetDataType(t);
		}
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
	type.dataType = TYPE_INTEGER;
	int t, uk1, uk2;
	Tree* v = root->GetCur();
	t = sc->Scaner(l);
	if ((t == Tsint) || (t == Tlint))
	{
		if (root->GetFlagIntr())
		{
			type = root->SemGetDataType(t);
		}
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
				if (root->GetFlagIntr())
				{
					v = root->SemGetVar(a);
				}
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
	Tree* vb = root->GetCur();
	t = sc->Scaner(l);
	if (t != TBracketOp)
	{
		sc->PrintError("Ожидался  символ {", l, sc->GetUKS());
	}
	if (root->GetFlagIntr())
	{
		vb = root->SemIncludeBlock();
	}
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
	int  t, uk1, uk2, uk3;
	TData type;
	Tree* v;
	Tree* p;
	Tree* fundot = new Tree();
	Tree* vc = new Tree();
	uk1 = sc->GetUK();
	uk2 = sc->GetUKS();
	t = sc->Scaner(l);
	if (t == TSecolon)
		return;				//  пустой  оператор
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
			if (root->GetFlagIntr())
			{
				v = root->SemGetFunct(a);
				fundot = root->GetCur();
				Tree* vc = root->CopyFunc(v);
				p = root->GetLeft(root->GetRight(fundot));
			}
			int ct=0;
			do
			{
				Expression(type);
				root->SemPutValue(p, type);
				ct++;
				if (root->GetLeft(p)!=NULL)
				{
					p = root->GetLeft(p);
				}
				uk1 = sc->GetUK();
				uk2 = sc->GetUKS();
				t = sc->Scaner(l);
			} while (t == TComm);
			root->SemControlParam(v, ct);
			if (t != TBraceCl)
			{
				sc->PrintError("Ожидался символ )", l, sc->GetUKS());
			}
			uk3 = sc->GetUK();
			sc->PutUK(root->GetPosition(v));
			root->SetCur(p);
			Block();
			root->SetCur(fundot);
			sc->PutUK(uk3);
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
	Tree *v = root->GetCur();
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
		if (root->GetFlagIntr())
		{
			v = root->SemGetVar(a);
		}
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
	bool localFlInt = root ->GetFlagIntr();
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
	if (root->GetFlagIntr()&&type.dataValue.dataAsSInt > 0)
	{
		root->SetFlagIntr(true);
	}
	else
	{
		root->SetFlagIntr(false);
	}
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
	if (localFlInt)
	{
		root->SetFlagIntr(1 - root->GetFlagIntr());
	}
	if (t == Telse)
	{
		t = sc->Scaner(l);
		Operator();
	}
	root->SetFlagIntr(localFlInt);
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
		if (root->GetFlagIntr())
		{
			type = root->SemResultOperation(type, type1, t);
		}
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
		if (root->GetFlagIntr())
		{
			type = root->SemResultOperation(type, type1, t);
		}
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
		if (root->GetFlagIntr())
		{
			type = root->SemResultOperation(type, type1, t);
		}
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
		if (root->GetFlagIntr())
		{
			type = root->SemResultOperation(type, type1, t);
		}
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
	TypeLex  l, l1;
	int  t, uk1, uk2;
	long int s = 0;
	t = sc->Scaner(l);
	if ((t == TCons10) || (t == TCons16))
	{
		int count = 0;
		for (int i = 0; l[i]!= '\0'; i++)
		{
			count++;
		}
		if (t == TCons16)
		{
			//проблема перевести из символьного представления в числовое - далее дебри воспаленного разума
			count = count - 2;
			for (int i = 1; i <= count; i++)
			{
				int k = i + 1;
				switch (l[k])
				{
				case '1': {s = s + PwrHex(count - i); break; }
				case '2': {s = s + 2 * PwrHex(count - i); break; }
				case '3': {s = s + 3 * PwrHex(count - i); break; }
				case '4': {s = s + 4 * PwrHex(count - i); break; }
				case '5': {s = s + 5 * PwrHex(count - i); break; }
				case '6': {s = s + 6 * PwrHex(count - i); break; }
				case '7': {s = s + 7 * PwrHex(count - i); break; }
				case '8': {s = s + 8 * PwrHex(count - i); break; }
				case '9': {s = s + 9 * PwrHex(count - i); break; }
				case 'A': {s = s + 10 * PwrHex(count - i); break; }
				case 'B': {s = s + 11 * PwrHex(count - i); break; }
				case 'C': {s = s + 12 * PwrHex(count - i); break; }
				case 'D': {s = s + 13 * PwrHex(count - i); break; }
				case 'E': {s = s + 14 * PwrHex(count - i); break; }
				case 'F': {s = s + 15 * PwrHex(count - i); break; }
				}
			}
			if ((s <= 32767) && (s >= -32767))
			{
				type.dataType = TYPE_SHORT_INTEGER;
				type.dataValue.dataAsSInt = s;
				type.dataValue.dataAsLInt = s;
			}
			else
			{
				type.dataType = TYPE_LONG_INTEGER;
				type.dataValue.dataAsLInt = s;
				type.dataValue.dataAsSInt = s;
			}

		}
		else
		{
			if (count <= 6)
			{
				type.dataType = TYPE_SHORT_INTEGER;
				type.dataValue.dataAsSInt = atoi(l);
				type.dataValue.dataAsLInt = atoi(l);
			}
			else
			{
				type.dataType = TYPE_LONG_INTEGER;
				type.dataValue.dataAsLInt = atol(l);
				type.dataValue.dataAsSInt = atol(l);
			}
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
	else
	{
		if (t == TIdent)
		{
			if (root->GetFlagIntr())
			{
				Tree* v = root->SemGetVar(l);
				type = root->SemGetType(v);
			}
		}
		else
		{
			sc->PrintError("ожидался  идентификатор или константа, или выражение", l, sc->GetUKS());
		}
	}
}

long int Diagram::PwrHex(int count)
{
	long int result = 1;
	for (int i = 0; i < count; i++)
	{
		result *= 16;
	}
	return result;
}