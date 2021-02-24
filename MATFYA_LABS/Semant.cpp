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
	printf("Вершина с данными %s --->", n->id);
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
TData Tree::SemGetDataType(int a)
{
	TData data;
	if (a == Tsint)
	{
		data.dataType = TYPE_SHORT_INTEGER;
		return data;
	}
	if (a == Tlint)
	{
		data.dataType = TYPE_LONG_INTEGER;
		return data;
	}
	if (a == Tvoid)
	{
		data.dataType = TYPE_VOID;
		return data;
	}
	data.dataType = TYPE_INTEGER;
	return data;

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
	b.data.dataType = dt;
	switch (dt)
	{
	case TYPE_SHORT_INTEGER:
	{
		b.data.dataValue.dataAsSInt = 0;
		break;
	}
	case TYPE_LONG_INTEGER:
	{
		b.data.dataValue.dataAsLInt = 0;
		break;
	}
	default:
		break;
	}
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
	b.data.dataType = dt;
	b.param = 0;
	Cur->SetLeft(&b);	//сделали вершину - функцию
	Cur = Cur->Left;
	v = Cur;			//это точка возврата после выхода из функции
	memcpy(&b.id, &"", 2);
	b.typeObject = EMPT;
	b.data.dataType = EMPTY;
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
	b.data.dataType = EMPTY;
	b.param = EMPTY;
	Cur->SetLeft(&b);	//сделали вершину - функцию
	Cur = Cur->Left;
	v = Cur;
	memcpy(&b.id, &"", 2);
	b.typeObject = EMPT;
	b.data.dataType = EMPTY;
	b.param = EMPTY;
	Cur->SetRight(&b); //сделали пустую вершину
	Cur = Cur->Right;
	return v;
}


//Вычисления типа и результата операции
TData Tree::SemResultOperation(TData t1, TData t2, int op)
{
	TData result;
	switch (op)
	{
	case TMore:
	{
		if (t1.dataType == TYPE_SHORT_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
		{
			result = t1;
			result.dataValue.dataAsSInt = t1.dataValue.dataAsSInt > t2.dataValue.dataAsSInt;
			result.dataValue.dataAsLInt = result.dataValue.dataAsSInt;
			return result;
		}
		else
		{
			if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_LONG_INTEGER)
			{
				result = t1;
				result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt > t2.dataValue.dataAsLInt;
				result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
				return result;
			}
			else
			{
				if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
				{
					result = t1;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt > t2.dataValue.dataAsSInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
				else
				{
					result = t2;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsSInt > t2.dataValue.dataAsLInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
			}
		}
		break;
	}
	case TMoreOrEqual:
	{
		if (t1.dataType == TYPE_SHORT_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
		{
			result = t1;
			result.dataValue.dataAsSInt = t1.dataValue.dataAsSInt >= t2.dataValue.dataAsSInt;
			result.dataValue.dataAsLInt = result.dataValue.dataAsSInt;
			return result;
		}
		else
		{
			if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_LONG_INTEGER)
			{
				result = t1;
				result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt >= t2.dataValue.dataAsLInt;
				result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
				return result;
			}
			else
			{
				if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
				{
					result = t1;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt >= t2.dataValue.dataAsSInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
				else
				{
					result = t2;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsSInt >= t2.dataValue.dataAsLInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
			}
		}
		break;
	}
	case TLess:
	{
		if (t1.dataType == TYPE_SHORT_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
		{
			result = t1;
			result.dataValue.dataAsSInt = t1.dataValue.dataAsSInt < t2.dataValue.dataAsSInt;
			result.dataValue.dataAsLInt = result.dataValue.dataAsSInt;
			return result;
		}
		else
		{
			if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_LONG_INTEGER)
			{
				result = t1;
				result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt < t2.dataValue.dataAsLInt;
				result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
				return result;
			}
			else
			{
				if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
				{
					result = t1;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt < t2.dataValue.dataAsSInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
				else
				{
					result = t2;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsSInt < t2.dataValue.dataAsLInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
			}
		}
		break;
	}
	case TLessOrEqual:
	{
		if (t1.dataType == TYPE_SHORT_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
		{
			result = t1;
			result.dataValue.dataAsSInt = t1.dataValue.dataAsSInt <= t2.dataValue.dataAsSInt;
			result.dataValue.dataAsLInt = result.dataValue.dataAsSInt;
			return result;
		}
		else
		{
			if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_LONG_INTEGER)
			{
				result = t1;
				result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt <= t2.dataValue.dataAsLInt;
				result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
				return result;
			}
			else
			{
				if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
				{
					result = t1;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt <= t2.dataValue.dataAsSInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
				else
				{
					result = t2;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsSInt <= t2.dataValue.dataAsLInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
			}
		}
		break;
	}
	case TEqual:
	{
		if (t1.dataType == TYPE_SHORT_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
		{
			result = t1;
			result.dataValue.dataAsSInt = t1.dataValue.dataAsSInt == t2.dataValue.dataAsSInt;
			result.dataValue.dataAsLInt = result.dataValue.dataAsSInt;
			return result;
		}
		else
		{
			if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_LONG_INTEGER)
			{
				result = t1;
				result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt == t2.dataValue.dataAsLInt;
				result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
				return result;
			}
			else
			{
				if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
				{
					result = t1;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt == t2.dataValue.dataAsSInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
				else
				{
					result = t2;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsSInt == t2.dataValue.dataAsLInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
			}
		}
		break;
	}
	case TNotEqual:
	{
		if (t1.dataType == TYPE_SHORT_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
		{
			result = t1;
			result.dataValue.dataAsSInt = t1.dataValue.dataAsSInt != t2.dataValue.dataAsSInt;
			result.dataValue.dataAsLInt = result.dataValue.dataAsSInt;
			return result;
		}
		else
		{
			if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_LONG_INTEGER)
			{
				result = t1;
				result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt != t2.dataValue.dataAsLInt;
				result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
				return result;
			}
			else
			{
				if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
				{
					result = t1;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt != t2.dataValue.dataAsSInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
				else
				{
					result = t2;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsSInt != t2.dataValue.dataAsLInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
			}
		}
		break;
	}
	case TShiftL:
	{
		if (t1.dataType == TYPE_SHORT_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
		{
			result = t1;
			result.dataValue.dataAsSInt = t1.dataValue.dataAsSInt << t2.dataValue.dataAsSInt;
			result.dataValue.dataAsLInt = result.dataValue.dataAsSInt;
			return result;
		}
		else
		{
			if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_LONG_INTEGER)
			{
				result = t1;
				result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt << t2.dataValue.dataAsLInt;
				result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
				return result;
			}
			else
			{
				if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
				{
					result = t1;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt << t2.dataValue.dataAsSInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
				else
				{
					result = t2;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsSInt << t2.dataValue.dataAsLInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
			}
		}
		break;
	}
	case TShiftR:
	{
		if (t1.dataType == TYPE_SHORT_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
		{
			result = t1;
			result.dataValue.dataAsSInt = t1.dataValue.dataAsSInt >> t2.dataValue.dataAsSInt;
			result.dataValue.dataAsLInt = result.dataValue.dataAsSInt;
			return result;
		}
		else
		{
			if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_LONG_INTEGER)
			{
				result = t1;
				result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt >> t2.dataValue.dataAsLInt;
				result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
				return result;
			}
			else
			{
				if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
				{
					result = t1;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt >> t2.dataValue.dataAsSInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
				else
				{
					result = t2;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsSInt >> t2.dataValue.dataAsLInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
			}
		}
		break;
	}
	case TAdd:
	{
		if (t1.dataType == TYPE_SHORT_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
		{
			result = t1;
			result.dataValue.dataAsSInt = t1.dataValue.dataAsSInt + t2.dataValue.dataAsSInt;
			result.dataValue.dataAsLInt = result.dataValue.dataAsSInt;
			return result;
		}
		else
		{
			if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_LONG_INTEGER)
			{
				result = t1;
				result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt + t2.dataValue.dataAsLInt;
				result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
				return result;
			}
			else
			{
				if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
				{
					result = t1;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt + t2.dataValue.dataAsSInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
				else
				{
					result = t2;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsSInt + t2.dataValue.dataAsLInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
			}
		}
		break;
	}
	case TSub:
	{
		if (t1.dataType == TYPE_SHORT_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
		{
			result = t1;
			result.dataValue.dataAsSInt = t1.dataValue.dataAsSInt - t2.dataValue.dataAsSInt;
			result.dataValue.dataAsLInt = result.dataValue.dataAsSInt;
			return result;
		}
		else
		{
			if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_LONG_INTEGER)
			{
				result = t1;
				result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt - t2.dataValue.dataAsLInt;
				result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
				return result;
			}
			else
			{
				if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
				{
					result = t1;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt - t2.dataValue.dataAsSInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
				else
				{
					result = t2;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsSInt - t2.dataValue.dataAsLInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
			}
		}
		break;
	}
	case TMult:
	{
		if (t1.dataType == TYPE_SHORT_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
		{
			result = t1;
			result.dataValue.dataAsSInt = t1.dataValue.dataAsSInt * t2.dataValue.dataAsSInt;
			result.dataValue.dataAsLInt = result.dataValue.dataAsSInt;
			return result;
		}
		else
		{
			if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_LONG_INTEGER)
			{
				result = t1;
				result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt * t2.dataValue.dataAsLInt;
				result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
				return result;
			}
			else
			{
				if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
				{
					result = t1;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt * t2.dataValue.dataAsSInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
				else
				{
					result = t2;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsSInt * t2.dataValue.dataAsLInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
			}
		}
		break;
	}
	case TDiv:
	{
		if (t1.dataType == TYPE_SHORT_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
		{
			result = t1;
			result.dataValue.dataAsSInt = t1.dataValue.dataAsSInt / t2.dataValue.dataAsSInt;
			result.dataValue.dataAsLInt = result.dataValue.dataAsSInt;
			return result;
		}
		else
		{
			if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_LONG_INTEGER)
			{
				result = t1;
				result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt / t2.dataValue.dataAsLInt;
				result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
				return result;
			}
			else
			{
				if (t1.dataType == TYPE_LONG_INTEGER && t2.dataType == TYPE_SHORT_INTEGER)
				{
					result = t1;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsLInt / t2.dataValue.dataAsSInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
				else
				{
					result = t2;
					result.dataValue.dataAsLInt = t1.dataValue.dataAsSInt / t2.dataValue.dataAsLInt;
					result.dataValue.dataAsSInt = result.dataValue.dataAsLInt;
					return result;
				}
			}
		}
		break;
	}
	default:
		break;
	}

}

// установить число формальных параметров n для функции по адресу Addr
void Tree::SemSetParam(Tree* Addr, int num)
{
	Addr->n->param = num;
}

// проверить равенство числа формальных параметров значению
// n для функции по адресу Addr
void Tree::SemControlParam(Tree *Addr, int num)
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

//Получить тип
TData Tree::SemGetType(Tree *Addr)
{
	return Addr->n->data;
}

//Удалить поддерево
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

//Получить позицию
int Tree::GetPosition(Tree* addr)
{
	return addr->n->position;
}

//Запомнить позицию
void Tree::PutPosition(Tree* addr, int pos)
{
	addr->n->position = pos;
}

//Удалить блок
void Tree::DeleteBlock()
{
	Tree* v = Cur->Up;
	SubtreeRemoval(Cur);
	Cur = v;
	Cur->Left = NULL;
}

//Скопировать функцию
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

//Удаление функции
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

//Занести значение в переменную
void Tree::SemPutValue(Tree* addr, TData t)
{
	
	if (addr->n->typeObject == OBJ_VAR)
	{
		if (addr->n->data.dataType == TYPE_SHORT_INTEGER)
		{
			printf("Переменной %s присвоено значение %d \n", addr->n->id, t.dataValue.dataAsSInt);
		}
		else
		{
			printf("Переменной %s присвоено значение %d \n", addr->n->id, t.dataValue.dataAsLInt);
		}
	}
	addr->n->data = t;
}

//Поместить значение флага интерпритации
bool Tree::SetFlagIntr(bool f1)
{
	flagInterpret = f1;
}

//Получить значение флага интерпритации
bool Tree::GetFlagIntr()
{
	return flagInterpret;
}