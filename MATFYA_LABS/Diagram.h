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
	DATA_TYPE Expression();		// выражение
	DATA_TYPE Calc();			// подсчет
	DATA_TYPE Arithmetic();		// арифметика
	DATA_TYPE Mul_div();			// умножение/деление
	DATA_TYPE Bracket();			// скобки
	bool SetFlagIntr();
	bool GetFlagIntr();
};
#endif