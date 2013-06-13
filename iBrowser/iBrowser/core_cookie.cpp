// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "core_cookie.h"

PFuncInternetSetCookieExW CoreCookie::Real_InternetSetCookieExW = NULL;
PFuncInternetSetCookieExA CoreCookie::Real_InternetSetCookieExA = NULL;

CoreCookie::CoreCookie()
{

}

CoreCookie::~CoreCookie()
{

}

DWORD CoreCookie::HOOK_InternetSetCookieExW(LPCWSTR lpszUrl
	,LPCWSTR lpszCookieName
	,LPCWSTR lpszCookieData
	,DWORD dwFlags
	,DWORD_PTR dwReserved )
{
	return Real_InternetSetCookieExW(lpszUrl, lpszCookieName, lpszCookieData, dwFlags, dwReserved);
}

DWORD CoreCookie::HOOK_InternetSetCookieExA(LPCSTR lpszUrl
	,LPCSTR lpszCookieName
	,LPCSTR lpszCookieData
	,DWORD dwFlags
	,DWORD_PTR dwReserved )
{
	return Real_InternetSetCookieExA(lpszUrl, lpszCookieName, lpszCookieData, dwFlags, dwReserved);
}
