﻿#ifndef __SEMAN
#define __SEMAN
enum DATA_TYPE { EMPTY = 0, TYPE_INTEGER = 100, TYPE_VOID = 200, TYPE_SHORT_INTEGER = 300, TYPE_LONG_INTEGER = 400 };
enum TYPE_OBJECT { EMPT = 0, OBJ_VAR = 10, OBJ_FUNCT = 30 };

struct Node //информация в вершине дерева
{
	TYPE_OBJECT typeObject;
	TypeLex id;			//идентификатор объект
	DATA_TYPE dataType; //тип значения
	char *data;			//ссылка на значение или NULL
	int param;		//количество параметров
	int position;
};

union DataValue
{
	/*__int64 dataAsInt64;
	__int64* arrayDataAsInt64;
	float dataAsFloat;
	float* arrayDataAsFloat; */
};
struct TData
{
	DATA_TYPE dataType;
	DataValue dataValue;
};

class Tree //  элемент  семантической  таблицы
{
private:
	Node *n;					//информация об объекте таблицы
	Tree *Up, *Left, *Right;	//родитель, левый и правый потомок
	TScaner *sc;
public:
	static Tree *Cur;			//текущий элемент дерева
	Tree(Tree *l, Tree *r, Tree *u, Node *Data);
	Tree();
	~Tree();
	void SetLeft(Node *Data);
	void SetRight(Node *Data);
	Tree *FindUp(Tree *From, TypeLex id);
	Tree *FindUp(TypeLex id);
	Tree *FindRightLeft(TypeLex id);
	Tree *FindRightLeft(Tree *From, TypeLex id);
	Tree *FindUpOneLevel(Tree *From, TypeLex id);
	void Print(void);
	void SubtreeRemoval(Tree* addr);				//Удаление поддерева
	void SetCur(Tree *a);						  //установить текущий узел дерева
	Tree *GetCur(void);							  //получить значение текущего узла дерева
	DATA_TYPE SemGetDataType(int a);			  //получить тип значения
	void SemIncludeVar(TypeLex a, DATA_TYPE t);  //занесение идентификатора a в таблицу с типом t
	Tree *SemIncludeFunct(TypeLex a, DATA_TYPE t);//
	Tree *SemIncludeBlock();					  //
	void SemSetParam(Tree *Addr, int n);			//установить число формальных параметров n для функции по адресу Addr
	void SemControlParam(Tree *Addr, int n);		//проверить равенство числа формальных параметров
	DATA_TYPE SemResultOperation(DATA_TYPE t1, DATA_TYPE t2, int op);

	Tree *SemGetVar(TypeLex a);				  //найти в таблице переменную с именем a и вернуть ссылку на соответствующий элемент дерева
	Tree *SemGetFunct(TypeLex a);				  //найти в таблице функцию с именем a и вернуть ссылку на соответствующий элемент дерева
	int DupControl(Tree *Addr, TypeLex a);		  //проверка идентификатора а на повторное описание внутри блока
	DATA_TYPE SemGetType(Tree *Addr);

	int GetPosition(Tree* addr);					//Получение позиции
	void PutPosition(Tree* addr, int pos);			//Установление позиции

	void DeleteBlock();								//Удаление блока

	Tree* CopyFunc();								//Копирование функции
	void DeleteFunc();								//Удаление функции

	TData SemResultOperation(TData t1, TData t2, int op); 
};
#endif 
