// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef _CORE_COOKIE_H__
#define _CORE_COOKIE_H__

typedef DWORD(WINAPI *PFuncInternetSetCookieExW)(
	LPCWSTR lpszUrl,
	LPCWSTR lpszCookieName,
	LPCWSTR lpszCookieData,
	DWORD dwFlags,
	DWORD_PTR dwReserved
	);

typedef DWORD(WINAPI *PFuncInternetSetCookieExA)(
	__in LPCSTR lpszUrl,
	__in_opt LPCSTR lpszCookieName,
	__in LPCSTR lpszCookieData,
	__in DWORD dwFlags,
	__in_opt DWORD_PTR dwReserved
	);

class CoreCookie
{
public:
	CoreCookie();
	~CoreCookie();

	static DWORD WINAPI HOOK_InternetSetCookieExW(__in LPCWSTR lpszUrl,
		__in_opt LPCWSTR lpszCookieName,
		__in LPCWSTR lpszCookieData,
		__in DWORD dwFlags,
		__in_opt DWORD_PTR dwReserved);
	
	static DWORD WINAPI HOOK_InternetSetCookieExA(__in LPCSTR lpszUrl,
		__in_opt LPCSTR lpszCookieName,
		__in LPCSTR lpszCookieData,
		__in DWORD dwFlags,
		__in_opt DWORD_PTR dwReserved);
	
	static PFuncInternetSetCookieExW Real_InternetSetCookieExW;
	static PFuncInternetSetCookieExA Real_InternetSetCookieExA;
private:
};


#endif //_CORE_COOKIE_H__