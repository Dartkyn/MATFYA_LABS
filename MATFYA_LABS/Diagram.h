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
	void Program();			// ���������
	void Funct();			// ���� ��������
	void Data();			// �������� ������
	void Block();			// ����
	void Operator();		// ��������
	void Assignment();		// ������������
	void If();			    // if
	int Param();			// ��������� �������
	DATA_TYPE Expression();		// ���������
	DATA_TYPE Calc();			// �������
	DATA_TYPE Arithmetic();		// ����������
	DATA_TYPE Mul_div();			// ���������/�������
	DATA_TYPE Bracket();			// ������
};
#endif