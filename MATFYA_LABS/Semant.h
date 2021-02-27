#ifndef __SEMAN
#define __SEMAN
enum DATA_TYPE { EMPTY = 0, TYPE_INTEGER = 100, TYPE_VOID = 200, TYPE_SHORT_INTEGER = 300, TYPE_LONG_INTEGER = 400 };
enum TYPE_OBJECT { EMPT = 0, OBJ_VAR = 10, OBJ_FUNCT = 30 };

union DataValue
{
	short int dataAsSInt;
	long int dataAsLInt;
};

struct TData
{
	DATA_TYPE dataType;			//тип значения
	DataValue dataValue;		//само значение
};

struct Node //информация в вершине дерева
{
	TYPE_OBJECT typeObject; 
	TypeLex id;			//идентификатор объекта
	TData data;			//данные
	int param;		//количество параметров
	int position;	//позиция тела функции
};



class Tree //  элемент  семантической  таблицы
{
private:
	Node *n;					//информация об объекте таблицы
	Tree *Up, *Left, *Right;	//родитель, левый и правый потомок
	TScaner *sc;
	static bool flagInterpret;	//флаг интерпритации
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
	Tree *GetCur(void);							  //получить  текущий узел дерева
	TData SemGetDataType(int a);			  //получить тип значения
	void SemIncludeVar(TypeLex a, DATA_TYPE t);  //занесение идентификатора a в таблицу с типом t
	Tree *SemIncludeFunct(TypeLex a, DATA_TYPE t);//
	Tree *SemIncludeBlock();					  //
	void SemSetParam(Tree *Addr, int n);			//установить число формальных параметров n для функции по адресу Addr
	void SemControlParam(Tree *Addr, int n);		//проверить равенство числа формальных параметров
	TData SemResultOperation(TData t1, TData t2, int op);	//Вычисления типа и результата операции

	Tree *SemGetVar(TypeLex a);				  //найти в таблице переменную с именем a и вернуть ссылку на соответствующий элемент дерева
	Tree *SemGetFunct(TypeLex a);				  //найти в таблице функцию с именем a и вернуть ссылку на соответствующий элемент дерева
	int DupControl(Tree *Addr, TypeLex a);		  //проверка идентификатора а на повторное описание внутри блока
	TData SemGetType(Tree *Addr);				//Получить тип и значение по адресу

	int GetPosition(Tree* addr);					//Получение позиции
	void PutPosition(Tree* addr, int pos);			//Установление позиции

	void DeleteBlock();								//Удаление блока

	Tree* CopyFunc();								//Копирование функции
	void DeleteFunc();								//Удаление функции
	void SemPutValue(Tree *addr, TData t);			//Поместить значение по адресу
	bool SetFlagIntr(bool f1);								//Поместить флаг интерпритации
	bool GetFlagIntr();								//Получить значение флага интерпритации
	Tree *GetRight(Tree* v);								//Получить правый потомок
	Tree* GetLeft(Tree* v);								//Получить левый потомок
};
#endif 
