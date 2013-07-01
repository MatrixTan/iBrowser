// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _UI_UTIL_H__
#define _UI_UTIL_H__

#include <GdiPlus.h>
#include <GdiPlusHeaders.h>
#include <GdiPlusImaging.h>
#include <windows.h>
#include <atlstr.h>

class GdiplusInitializeTracker
{
public:
	GdiplusInitializeTracker()
	{
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;//启动时初始化gdi+
		Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	}
	~GdiplusInitializeTracker()
	{
		Gdiplus::GdiplusShutdown(m_gdiplusToken); //在程序终止时释放gdi+
	}
private:
	ULONG_PTR m_gdiplusToken;
};

BOOL ImageFromIDResource(HINSTANCE hInstance, UINT nID, LPCTSTR sTR, Gdiplus::Image * &pImg);
HWND GetChildWindow(HWND hParent, const CStringW& strClassName);

#endif //_UI_UTIL_H__
