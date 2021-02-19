#pragma once
#ifndef  __DIAGRAM
#define  __DIAGRAM
#include "Semant.h"
class  Diagram
{
private:
	TScaner  *sc;
	Tree *root;
public:
	Diagram(TScaner  *  s, Tree *r) { sc = s; root = r;}
	~Diagram() {}
	void Program();			// программа
	void Funct();			// одно описание
	void Data();			// описание данных
	void Block();			// блок
	void Operator();		// оператор
	void Assignment();		// присваивание
	void If();			    // if
	int Param();			// параметры функции
	void Expression(TData& type);		// выражение
	void Calc(TData& type);			// подсчет
	void Arithmetic(TData& type);		// арифметика
	void Mul_div(TData& type);			// умножение/деление
	void Bracket(TData& type);			// скобки
	bool SetFlagIntr();
	bool GetFlagIntr();
};
#endif