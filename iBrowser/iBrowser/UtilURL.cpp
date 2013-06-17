// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "UtilURL.h"


CString URLUtil::GetHost( const CString &strURL )
{
	int index = strURL.Find(L"://");
	if (index > 0){
		int indexLeft = index + 3;
		int indexRight = strURL.Find(L"/", indexLeft);
		if (indexRight > indexLeft){
			return strURL.Mid(indexLeft, indexRight-indexLeft);
		}
	}
	return CString();
}
