#include "stdafx.h"
#include  <string.h>
#include "defs.h"
#include  "Scaner.h"
#include "Semant.h"
#define max(a,b) a<b? b: a

//конструктор создает узел с заданными связями и данными
Tree::Tree(Tree *l, Tree *r, Tree *u, Node *d)
{
	n = new Node();
	//установили ссылки
	Up = u;
	Left = l;
	Right = r;
	memcpy(n, d, sizeof(Node)); //установили данные
}

//конструктор создает новый узел с пустыми связями и данными
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

//создать левого потомка от текущей вершины this
void Tree::SetLeft(Node *Data)
{
	Tree *a = new Tree(NULL, NULL, this, Data);	//новая вершина
	Left = a;									//связали this с новой вершиной
}

//создать правого потомка от текущей вершины this
void Tree::SetRight(Node *Data)
{
	Tree *a = new Tree(NULL, NULL, this, Data);	//новая вершина
	Right = a;									//связали this с новой вершиной
}

//поиск данных в дереве, начиная от текущей вершины this
//до его корня вверх по связям
Tree * Tree::FindUp(TypeLex id)
{
	return FindUp(this, id);
}

//поиск данных в дереве от заданной вершины From
//до его корня вверх по связям
Tree *Tree::FindUp(Tree *From, TypeLex id)
{
	Tree *i = From;								//текущая вершина поиска
	while ((i != NULL) && (memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) != 0))
		i = i->Up;									//поднимаемся наверх по связям
	return i;
}

//поиск прямых потомков текущей вершины this
Tree *Tree::FindRightLeft(TypeLex id)
{
	return FindRightLeft(this, id);
}

//поиск прямых потомков заданной вершины From
Tree *Tree::FindRightLeft(Tree *From, TypeLex id)
{
	Tree *i = From->Right;						//  текущая  вершина  поиска
	while ((i != NULL) && (memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) != 0))
		i = i->Left;
	//обходим только соседей по левым связям
	return i;
}

//Поиск элемента id вверх по дереву от текущей вершины From.
//Поиск осуществляется на одном уровне вложенности по левым связям
Tree *Tree::FindUpOneLevel(Tree *From, TypeLex id)
{
	Tree *i = From;			//текущая вершина поиска
	while ((i != NULL) && (i->Up != NULL) && (i->Up->Right != i))
	{
		if (memcmp(id, i->n->id, max(strlen(i->n->id), strlen(id))) == 0)
			return i;		//нaшли совпадающий идентификатор
		i = i->Up;			//поднимаемся наверх по связям
	}
	return NULL;
}

//отладочная программа печати дерева
void Tree::Print(void)
{
	printf("Вершина с данными %s ----->", n->id);
	if (Left != NULL)
		printf("        слева  данные   %s тип %d", Left->n->id, Left->n->typeObject);
	if (Right != NULL)
		printf("        справа  данные  %s тип %d", Right->n->id, Right->n->typeObject);
	printf("\n");
	if (Left != NULL)
		Left->Print();
	if (Right != NULL)
		Right->Print();
}

Tree *Tree::Cur = (Tree*)NULL;

//установить текущий узел дерева
void Tree::SetCur(Tree *a)
{
	Cur = a;
}

//получить значение текущего узла дерева
Tree *Tree::GetCur(void)
{
	return Cur;
}

//получить тип значения
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

//занесение идентификатора a в таблицу с типом t
void Tree::SemIncludeVar(TypeLex a, DATA_TYPE dt)
{
	if (DupControl(Cur, a))
		sc->PrintError("Повторное описание идентификатора ", a);
	Tree *v;
	Node b;
	memcpy(b.id, a, strlen(a) + 1);
	b.typeObject = OBJ_VAR;
	b.dataType = dt;
	b.param = 0;
	Cur->SetLeft(&b);	//сделали вершину - переменную
	Cur = Cur->Left;
}

//
Tree *Tree::SemIncludeFunct(TypeLex a, DATA_TYPE dt)
{
	if (DupControl(Cur, a))
		sc->PrintError("Повторное описание идентификатора ", a);
	Tree *v;
	Node b;
	memcpy(b.id, a, strlen(a) + 1);
	b.typeObject = OBJ_FUNCT;
	b.dataType = dt;
	b.param = 0;
	Cur->SetLeft(&b);	//сделали вершину - функцию
	Cur = Cur->Left;
	v = Cur;			//это точка возврата после выхода из функции
	memcpy(&b.id, &"", 2);
	b.typeObject = EMPT;
	b.dataType = EMPTY;
	b.param = EMPTY;
	Cur->SetRight(&b); //сделали пустую вершину
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
	Cur->SetLeft(&b);	//сделали вершину - функцию
	Cur = Cur->Left;
	v = Cur;
	memcpy(&b.id, &"", 2);
	b.typeObject = EMPT;
	b.dataType = EMPTY;
	b.param = EMPTY;
	Cur->SetRight(&b); //сделали пустую вершину
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
// установить число формальных параметров n для функции по адресу Addr
{
	Addr->n->param = num;
}
void Tree::SemControlParam(Tree *Addr, int num)
// проверить равенство числа формальных параметров значению
// n для функции по адресу Addr
{
	if (num != Addr->n->param)
		sc->PrintError("Неверное число параметров у функции ", Addr->n->id);
}

//найти в таблице переменную с именем a
//и вернуть ссылку на соответствующий элемент дерева
Tree *Tree::SemGetVar(TypeLex a)
{
	Tree *v = FindUp(Cur, a);
	if (v == NULL)
		sc->PrintError("Отсутствует описание идентификатора ", a);
	if (v->n->typeObject != OBJ_VAR)
		sc->PrintError("Идентификатор не является переменной ", a);
	return v;
}

//найти в таблице функцию с именем a
//и вернуть ссылку на соответствующий элемент дерева.
Tree *Tree::SemGetFunct(TypeLex a)
{
	Tree *v = FindUp(Cur, a);
	if (v == NULL)
		sc->PrintError("Отсутствует описание функции ", a);
	if (v->n->typeObject != OBJ_FUNCT)
		sc->PrintError("Идентификатор не является функцией ", a);
	return v;
}

//Проверка идентификатора а на повторное описание внутри блока.
//Поиск осуществляется вверх от вершины Addr.
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

//
void Tree::SubtreeRemoval(Tree* addr)
{
	if (addr == NULL)
		return;
	if (addr->Left != NULL)
	{
		SubtreeRemoval(addr->Left);
	}
	if (addr->Right != NULL)
	{
		SubtreeRemoval(addr->Right);
	}
	delete addr;
}

//
int Tree::GetPosition(Tree* addr)
{
	return addr->n->position;
}

//
void Tree::PutPosition(Tree* addr, int pos)
{
	addr->n->position = pos;
}

//
void Tree::DeleteBlock()
{
	Tree* v = Cur->Up;
	SubtreeRemoval(Cur);
	Cur = v;
	Cur->Left = NULL;
}

//
Tree* Tree::CopyFunc()
{
	Tree* vLeft = Cur->Left;
	Cur->SetLeft(Cur->n);
	Cur = Cur->Left;
	Node d;
	memcpy(&d.id, &"", 2);
	d.typeObject = EMPT;
	Cur->SetRight(&d);
	Cur->Left = vLeft;
	Tree* v = Cur;
	Cur = Cur->Right;
	return v;
}

//
void Tree::DeleteFunc()
{
	Cur = Cur->Up;
	Tree* vUp = Cur->Up;
	Tree* vLeft = Cur->Left;
	delete Cur->Right;
	delete Cur;
	Cur = vUp;
	Cur->Left = vLeft;
}

//
TData Tree::SemResultOperation(TData t1, TData t2, int op)
{
	TData result;
	/*if (op >= MMore && op <= MNonEqual)
	{
		result.dataType = TYPE_INTEGER64;
		switch (op)
		{
		case MMore:
			if (t1.dataType == TYPE_INTEGER64 && t2.dataType == TYPE_INTEGER64)
			{
				result.dataValue.dataAsInt64 = t1.dataValue.dataAsInt64 > t2.dataValue.dataAsInt64;
			}
			else
			{
				if (t1.dataType == TYPE_INTEGER64 && t2.dataType == TYPE_FLOAT)
				{
					result.dataValue.dataAsInt64 = t1.dataValue.dataAsInt64 > t2.dataValue.dataAsFloat;
				}
				else
				{
					if (t1.dataType == TYPE_FLOAT && t2.dataType == TYPE_INTEGER64)
					{
						result.dataValue.dataAsInt64 = t1.dataValue.dataAsFloat > t2.dataValue.dataAsInt64;
					}
					else
					{
						result.dataValue.dataAsInt64 = t1.dataValue.dataAsFloat > t2.dataValue.dataAsFloat;
					}
				}
			}
			break;
		case MMoreOrEqual:
			if (t1.dataType == TYPE_INTEGER64 && t2.dataType == TYPE_INTEGER64)
			{
				result.dataValue.dataAsInt64 = t1.dataValue.dataAsInt64 >= t2.dataValue.dataAsInt64;
			}
			else
			{
				if (t1.dataType == TYPE_INTEGER64 && t2.dataType == TYPE_FLOAT)
				{
					result.dataValue.dataAsInt64 = t1.dataValue.dataAsInt64 >= t2.dataValue.dataAsFloat;
				}
				else
				{
					if (t1.dataType == TYPE_FLOAT && t2.dataType == TYPE_INTEGER64)
					{
						result.dataValue.dataAsInt64 = t1.dataValue.dataAsFloat >= t2.dataValue.dataAsInt64;
					}
					else
					{
						result.dataValue.dataAsInt64 = t1.dataValue.dataAsFloat >= t2.dataValue.dataAsFloat;
					}
				}
			}
			break;
		case MLess:
			if (t1.dataType == TYPE_INTEGER64 && t2.dataType == TYPE_INTEGER64)
			{
				result.dataValue.dataAsInt64 = t1.dataValue.dataAsInt64 < t2.dataValue.dataAsInt64;
			}
			else
			{
				if (t1.dataType == TYPE_INTEGER64 && t2.dataType == TYPE_FLOAT)
				{
					result.dataValue.dataAsInt64 = t1.dataValue.dataAsInt64 < t2.dataValue.dataAsFloat;
				}
				else
				{
					if (t1.dataType == TYPE_FLOAT && t2.dataType == TYPE_INTEGER64)
					{
						result.dataValue.dataAsInt64 = t1.dataValue.dataAsFloat < t2.dataValue.dataAsInt64;
					}
					else
					{
						result.dataValue.dataAsInt64 = t1.dataValue.dataAsFloat < t2.dataValue.dataAsFloat;
					}
				}
			}
			break;
		case MLessOrEqual:
			if (t1.dataType == TYPE_INTEGER64 && t2.dataType == TYPE_INTEGER64)
			{
				result.dataValue.dataAsInt64 = t1.dataValue.dataAsInt64 <= t2.dataValue.dataAsInt64;
			}
			else
			{
				if (t1.dataType == TYPE_INTEGER64 && t2.dataType == TYPE_FLOAT)
				{
					result.dataValue.dataAsInt64 = t1.dataValue.dataAsInt64 <= t2.dataValue.dataAsFloat;
				}
				else
				{
					if (t1.dataType == TYPE_FLOAT && t2.dataType == TYPE_INTEGER64)
					{
						result.dataValue.dataAsInt64 = t1.dataValue.dataAsFloat <= t2.dataValue.dataAsInt64;
					}
					else
					{
						result.dataValue.dataAsInt64 = t1.dataValue.dataAsFloat <= t2.dataValue.dataAsFloat;
					}
				}
			}
			break;
		case MEqual:
			if (t1.dataType == TYPE_INTEGER64 && t2.dataType == TYPE_INTEGER64)
			{
				result.dataValue.dataAsInt64 = t1.dataValue.dataAsInt64 == t2.dataValue.dataAsInt64;
			}
			else
			{
				if (t1.dataType == TYPE_INTEGER64 && t2.dataType == TYPE_FLOAT)
				{
					result.dataValue.dataAsInt64 = t1.dataValue.dataAsInt64 == t2.dataValue.dataAsFloat;
				}
				else
				{
					if (t1.dataType == TYPE_FLOAT && t2.dataType == TYPE_INTEGER64)
					{
						result.dataValue.dataAsInt64 = t1.dataValue.dataAsFloat == t2.dataValue.dataAsInt64;
					}
					else
					{
						result.dataValue.dataAsInt64 = t1.dataValue.dataAsFloat == t2.dataValue.dataAsFloat;
					}
				}
			}
			break;
		case MNonEqual:
			if (t1.dataType == TYPE_INTEGER64 && t2.dataType == TYPE_INTEGER64)
			{
				result.dataValue.dataAsInt64 = t1.dataValue.dataAsInt64 != t2.dataValue.dataAsInt64;
			}
			else
			{
				if (t1.dataType == TYPE_INTEGER64 && t2.dataType == TYPE_FLOAT)
				{
					result.dataValue.dataAsInt64 = t1.dataValue.dataAsInt64 != t2.dataValue.dataAsFloat;
				}
				else
				{
					if (t1.dataType == TYPE_FLOAT && t2.dataType == TYPE_INTEGER64)
					{
						result.dataValue.dataAsInt64 = t1.dataValue.dataAsFloat != t2.dataValue.dataAsInt64;
					}
					else
					{
						result.dataValue.dataAsInt64 = t1.dataValue.dataAsFloat != t2.dataValue.dataAsFloat;
					}
				}
			}
			break;
		default:
			break;
		}
	}
	switch (op)
	{
	case MPlus:
		if (t1.dataType == TYPE_INTEGER64 && t2.dataType == TYPE_INTEGER64)
		{
			result.dataType = TYPE_INTEGER64;
			result.dataValue.dataAsInt64 = t1.dataValue.dataAsInt64 + t2.dataValue.dataAsInt64;
		}
		else
		{
			result.dataType = TYPE_FLOAT;
			if (t1.dataType == TYPE_INTEGER64 && t2.dataType == TYPE_FLOAT)
			{
				result.dataValue.dataAsFloat = t1.dataValue.dataAsInt64 + t2.dataValue.dataAsFloat;
			}
			else
			{
				if (t1.dataType == TYPE_FLOAT && t2.dataType == TYPE_INTEGER64)
				{
					result.dataValue.dataAsFloat = t1.dataValue.dataAsFloat + t2.dataValue.dataAsInt64;
				}
				else
				{
					result.dataValue.dataAsFloat = t1.dataValue.dataAsFloat + t2.dataValue.dataAsFloat;
				}
			}
		}
		break;
	case MMinus:
		if (t1.dataType == TYPE_INTEGER64 && t2.dataType == TYPE_INTEGER64)
		{
			result.dataType = TYPE_INTEGER64;
			result.dataValue.dataAsInt64 = t1.dataValue.dataAsInt64 - t2.dataValue.dataAsInt64;
		}
		else
		{
			result.dataType = TYPE_FLOAT;
			if (t1.dataType == TYPE_INTEGER64 && t2.dataType == TYPE_FLOAT)
			{
				result.dataValue.dataAsFloat = t1.dataValue.dataAsInt64 - t2.dataValue.dataAsFloat;
			}
			else
			{
				if (t1.dataType == TYPE_FLOAT && t2.dataType == TYPE_INTEGER64)
				{
					result.dataValue.dataAsFloat = t1.dataValue.dataAsFloat - t2.dataValue.dataAsInt64;
				}
				else
				{
					result.dataValue.dataAsFloat = t1.dataValue.dataAsFloat - t2.dataValue.dataAsFloat;
				}
			}
		}
		break;
	case MMult:
		if (t1.dataType == TYPE_INTEGER64 && t2.dataType == TYPE_INTEGER64)
		{
			result.dataType = TYPE_INTEGER64;
			result.dataValue.dataAsInt64 = t1.dataValue.dataAsInt64 * t2.dataValue.dataAsInt64;
		}
		else
		{
			result.dataType = TYPE_FLOAT;
			if (t1.dataType == TYPE_INTEGER64 && t2.dataType == TYPE_FLOAT)
			{
				result.dataValue.dataAsFloat = t1.dataValue.dataAsInt64 * t2.dataValue.dataAsFloat;
			}
			else
			{
				if (t1.dataType == TYPE_FLOAT && t2.dataType == TYPE_INTEGER64)
				{
					result.dataValue.dataAsFloat = t1.dataValue.dataAsFloat * t2.dataValue.dataAsInt64;
				}
				else
				{
					result.dataValue.dataAsFloat = t1.dataValue.dataAsFloat * t2.dataValue.dataAsFloat;
				}
			}
		}
		break;
	case MDivide:
		if (t2.dataValue.dataAsInt64 != 0 || t2.dataValue.dataAsFloat != 0)
		{
			if (t1.dataType == TYPE_INTEGER64 && t2.dataType == TYPE_INTEGER64)
			{
				result.dataType = TYPE_INTEGER64;
				result.dataValue.dataAsInt64 = t1.dataValue.dataAsInt64 / t2.dataValue.dataAsInt64;
			}
			else
			{
				result.dataType = TYPE_FLOAT;
				if (t1.dataType == TYPE_INTEGER64 && t2.dataType == TYPE_FLOAT)
				{
					result.dataValue.dataAsFloat = t1.dataValue.dataAsInt64 / t2.dataValue.dataAsFloat;
				}
				else
				{
					if (t1.dataType == TYPE_FLOAT && t2.dataType == TYPE_INTEGER64)
					{
						result.dataValue.dataAsFloat = t1.dataValue.dataAsFloat / t2.dataValue.dataAsInt64;
					}
					else
					{
						result.dataValue.dataAsFloat = t1.dataValue.dataAsFloat / t2.dataValue.dataAsFloat;
					}
				}
			}
		}
		else
		{
			sc->PrintError("Äåëåíèå íà 0");
		}
		break;
	case MAssign:
		result = t2;
		if (t1.dataType != t2.dataType)
		{
			if (t2.dataType == TYPE_INTEGER64)
			{
				result.dataValue.dataAsFloat = (float)t2.dataValue.dataAsInt64;
			}
			else
			{
				result.dataValue.dataAsInt64 = (int)t2.dataValue.dataAsFloat;
			}
			result.dataType = t1.dataType;
		}
		break;
	default:
		break;
	}*/
	return result;
}