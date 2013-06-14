// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _UTIL_COMMON_H__
#define _UTIL_COMMON_H__

#include <Windows.h>
#include <atlstr.h>

namespace Util
{
	CString GetCurrentModuleFileName(void);
	CString GetCustomErrorPage(void);
	bool IsCustomErrorPage(const CString& strURL);
}


#endif //_UTIL_COMMON_H__