#pragma once
#include "afxdialogex.h"
#include "framework.h"
class sqlscripts
{
public:
	CString sql1;


	bool sqlscripts::readsqldata(UINT uID, int i);

	CMap<int, int, CString, CString>sqlmap;

	void ReadStringCharToUnicode(CString &str);
};

