// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "ui_util.h"
#include <atlstr.h>

BOOL ImageFromIDResource(HINSTANCE hInstance, UINT nID, LPCTSTR sTR, Gdiplus::Image * &pImg)
{
	HRSRC hRsrc = ::FindResource (hInstance, MAKEINTRESOURCE(nID), sTR); // type
	if (!hRsrc)
		return FALSE;

	// load resource into memory
	DWORD len = SizeofResource(hInstance, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInstance, hRsrc);
	if (!lpRsrc)
		return FALSE;

	// Allocate global memory on which to create stream
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem,lpRsrc,len);
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);

	// load from stream
	pImg = Gdiplus::Image::FromStream(pstm);

	// free/release stuff
	GlobalUnlock(m_hMem);
	pstm->Release();
	FreeResource(lpRsrc);
	return TRUE;
}

class WindowFinder
{
private:
	static BOOL CALLBACK FindWindowInternal(HWND hParent, LPARAM lThis){
		WindowFinder* pThis = (WindowFinder*)lThis;
		ATLASSERT(pThis != NULL);

		HWND hChild = ::FindWindowEx(hParent, NULL, pThis->strClassName, NULL);
		if (hChild){
			pThis->hwnd = hChild;
			return FALSE;
		}
		
		EnumChildWindows(hParent, FindWindowInternal, lThis);
		return TRUE;
	}
public:
	WindowFinder(HWND hWnd, const CStringW& strClassName)
	:hwnd(NULL)
	,strClassName(strClassName)
	{
		FindWindowInternal(hWnd, (LPARAM)this);
	}

	CStringW strClassName;
	HWND hwnd;
};

HWND GetChildWindow( HWND hParent, const CStringW& strClassName )
{
	WindowFinder finder(hParent, strClassName);
	return finder.hwnd;	
}
