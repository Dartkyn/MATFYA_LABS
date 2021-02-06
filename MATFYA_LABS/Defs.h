#pragma once
#ifndef __DEFS
#define __DEFS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TEXT 10000 // ������������ ����� ������
#define MAX_LEX 20 // ������������ ����� �������
#define MAX_KEYW 8 // ����� �������� ����
typedef char TypeLex[MAX_LEX];
typedef char TypeMod[MAX_TEXT];

// �������������� � ���������
#define TIdent 1
#define TCons10 10 //10 c/c
#define TCons16 11 //16 c/c
// �������� �����
#define Tmain 20 
#define Tvoid 21
#define Tsint 22	//short int
#define Tlint 23	//long int
#define Tif 24
#define Telse 25
#define Tint 26
// ����� ��������
#define TAdd 30 //+
#define TSub 31	//-
#define TMult 32	//*
#define TDiv 33	// /
#define TAssign 34	//=
#define TMore 35	// >
#define TMoreOrEqual 36 // >=
#define TLess 37      // <
#define TLessOrEqual 38 // <=
#define TEqual 39		//==
#define TNotEqual 40	// !=
#define TShiftL 41		// <<	
#define TShiftR 42		//>>
// ����������� �����
#define TComm 50		// ,
#define TSecolon 51		// ;
// ����� ������
#define TBraceOp 60		// (
#define TBraceCl 61		// )
#define TBracketOp 62	// {
#define TBracketCl 63	// }
// ����� ��������� ������
#define TEnd 100
// ��������� ������
#define TErr 666
#endif