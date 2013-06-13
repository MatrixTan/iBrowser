// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "stdafx.h"
#include "util_common.h"

namespace Util
{
	CString GetCurrentModuleFileName( void )
	{
		WCHAR buffer[MAX_PATH];
		::GetModuleFileName(NULL, buffer, sizeof(buffer));
		return CString(buffer);
	}

	CString GetCustomErrorPage( void )
	{
		CString url;
		url.Format(L"res://%s/error.html", GetCurrentModuleFileName());
		return url;
	}

	bool IsCustomErrorPage( const CString& strURL ){
		return strURL.Find(GetCustomErrorPage()) == 0;
	}
}