// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

extern"C" int show(char x)
{
	printf("%c",x);
	return 3;
}

int _tmain(int argc, _TCHAR* argv[])
{
	char x;
	while (cin>>x)
	{
		show(x);
	}

	return 0;
}

