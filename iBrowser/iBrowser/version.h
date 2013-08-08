// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _VERSION_H__
#define _VERSION_H__

#include <atlstr.h>

#define CUR_VERSION L"1.0.0.1"

class iVersion
{
public:
	iVersion():v1(0)
		,v2(0)
		,v3(0)
		,v4(0){}

	iVersion(int n1, int n2, int n3, int n4):v1(n1)
		,v2(n2)
		,v3(n3)
		,v4(n4){}

	bool FromString(const CStringW& strVersion);
	bool operator < (const iVersion& other);
	bool operator > (const iVersion& other);
	
	int v1;
	int v2;
	int v3;
	int v4;
};

#endif //_VERSION_H__