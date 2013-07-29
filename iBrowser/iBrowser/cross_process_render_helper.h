// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _CROSS_PROCESS_RENDER_HELPER_H__
#define _CROSS_PROCESS_RENDER_HELPER_H__

#include "Base/t_thread_singleton.h"
#include "api_hook.h"
#include <WinGDI.h>

typedef BOOL (WINAPI *PFuncBitBlt)(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);
typedef BOOL (WINAPI *PFuncGetCursorPos)(LPPOINT lpPoint);
typedef HCURSOR (WINAPI *PFuncSetCursor)(HCURSOR hCursor);
typedef BOOL (WINAPI *PFunAlphaBlend)(
	HDC hdcDest,
	int xoriginDest,
	int yoriginDest,
	int wDest,
	int hDest,
	HDC hdcSrc,
	int xoriginSrc,
	int yoriginSrc,
	int wSrc,
	int hSrc,
	BLENDFUNCTION ftn);
typedef BOOL (WINAPI *PFunTransparentBlt)(
	HDC hdcDest,
	int xoriginDest,
	int yoriginDest,
	int wDest,
	int hDest,
	HDC hdcSrc,
	int xoriginSrc,
	int yoriginSrc,
	int wSrc,
	int hSrc,
	UINT crTransparent);


class CrossProcessRenderHelper 
	: public TThreadSingleton<CrossProcessRenderHelper, ETTS_CrossProcessRenderHelper>
{
public:
	void SetHost(HWND hHost);
	void SetCore(HWND hCore);
	BOOL CustomBitBlt(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop, PFuncBitBlt pfBitBlt);

	static bool StartHooksInCoreProcess(void);
	static BOOL WINAPI HOOK_BitBlt( HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop );
	static BOOL WINAPI HOOK_GetCursorPos(LPPOINT lpPoint);
	static HCURSOR WINAPI HOOK_SetCursor(HCURSOR hCursor);
	static BOOL WINAPI HOOK_AlphaBlend(HDC hdcDest,
		int xoriginDest,
		int yoriginDest,
		int wDest,
		int hDest,
		HDC hdcSrc,
		int xoriginSrc,
		int yoriginSrc,
		int wSrc,
		int hSrc,
		BLENDFUNCTION ftn);
	static BOOL WINAPI HOOK_TransparentBlt(HDC hdcDest,
		int xoriginDest,
		int yoriginDest,
		int wDest,
		int hDest,
		HDC hdcSrc,
		int xoriginSrc,
		int yoriginSrc,
		int wSrc,
		int hSrc,
		UINT crTransparent);

	///Main Process Method
	static void RenderOnHost(HWND hHost, void* pScreenData);

	static const int kCoreWindowOffsetX = -10000;
	static const int kCoreWindowOffsetY = 0;
protected:
private:
	HWND m_hHost;
	HWND m_hCore;
	static PFuncBitBlt s_BitBlt;
	static PFuncGetCursorPos s_GetCursorPos;
	static PFuncSetCursor s_SetCursor;
	static PFunAlphaBlend s_AlphaBlend;
	static PFunTransparentBlt s_TransparentBlt;
};


#endif //_CROSS_PROCESS_RENDER_HELPER_H__