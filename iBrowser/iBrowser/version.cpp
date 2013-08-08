// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "version.h"


bool iVersion::FromString( const CStringW& strVersion )
{
	int nextPos = strVersion.Find(L".", 0);
	if (nextPos < 0)
		return false;		
	int n1 = _wtoi(strVersion.Mid(0, nextPos));

	int startPos = nextPos+1;
	nextPos = strVersion.Find(L".", startPos);
	if (nextPos < 0)
		return false;
	int n2 = _wtoi(strVersion.Mid(startPos, nextPos-startPos));

	startPos = nextPos+1;
	nextPos = strVersion.Find(L".", startPos);
	if (nextPos < 0)
		return false;
	int n3 = _wtoi(strVersion.Mid(startPos, nextPos-startPos));
	int n4 = _wtoi(strVersion.Right(strVersion.GetLength()-nextPos-1));

	v1 = n1;
	v2 = n2;
	v3 = n3;
	v4 = n4;
	return true;

}

bool iVersion::operator < (const iVersion& other)
{
	if (other.v1 != v1)
		return v1 < other.v1;
	if (other.v2 != v2)
		return v2 < other.v2;
	if (other.v3 != v3)
		return v3 < other.v3;
	if (other.v4 != v4)
		return v4 < other.v4;

	return false;
}

bool iVersion::operator > (const iVersion& other)
{
	if (other.v1 != v1)
		return v1 > other.v1;
	if (other.v2 != v2)
		return v2 > other.v2;
	if (other.v3 != v3)
		return v3 > other.v3;
	if (other.v4 != v4)
		return v4 > other.v4;

	return false;
}