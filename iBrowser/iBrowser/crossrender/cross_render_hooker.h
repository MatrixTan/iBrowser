// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _CROSS_RENDER_HOOKER_H__
#define _CROSS_RENDER_HOOKER_H__

#include "cross_render_helper.h"

namespace CrossRender
{

	///user32.dll
	typedef HDC	(WINAPI *PFunGetDCEx)(HWND hWnd,HRGN hrgnClip,	DWORD flags);
	typedef HDC (WINAPI *PFunGetDC)(HWND hWnd);
	typedef HDC	(WINAPI *PFunBeginPaint)(HWND hWnd,	LPPAINTSTRUCT lpPaint);
	typedef HDC (WINAPI *PFunEndPaint)(HWND hWnd,	LPPAINTSTRUCT lpPaint);
	typedef int	(WINAPI	*PFunSetScrollInfo)(HWND hwnd, int nBar, LPCSCROLLINFO lpsi,BOOL redraw);	
	typedef BOOL (WINAPI *PFuncGetCursorPos)(LPPOINT lpPoint);
	typedef HCURSOR (WINAPI *PFuncSetCursor)(HCURSOR hCursor);
	typedef int (WINAPI *PFunDrawTextW)(HDC hDC, LPCTSTR lpchText, int nCount, LPRECT lpRect, UINT uFormat);
	typedef int (WINAPI *PFunDrawTextExW)(HDC hdc,LPTSTR lpchText,int cchText,LPRECT lprc,UINT dwDTFormat,LPDRAWTEXTPARAMS lpDTParams);

	///gdi32.dll
	typedef BOOL (WINAPI *PFuncBitBlt)(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);
	typedef BOOL (WINAPI *PFunStretchBlt)(HDC hdcDest
		, int xDest
		, int yDest
		, int wDest
		, int hDest
		, HDC hdcSrc
		, int xSrc
		, int ySrc
		, int wSrc
		, int hSrc
		, DWORD rop);

	///msimg.dll
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
	

	


	class CrossRenderHooker
	{
	public:

		static bool StartHooksInCoreProcess(void);

		///gdi32.dll
		static PFuncBitBlt s_BitBlt;
		static BOOL WINAPI HOOK_BitBlt( HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop );
		static PFunStretchBlt s_StretchBlt;
		static BOOL WINAPI HOOK_StretchBlt(HDC hdcDest
			, int xDest
			, int yDest
			, int wDest
			, int hDest
			, HDC hdcSrc
			, int xSrc
			, int ySrc
			, int wSrc
			, int hSrc
			, DWORD rop);

		///msimg32.dll
		static PFunAlphaBlend s_AlphaBlend;
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
		static PFunTransparentBlt s_TransparentBlt;
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

		///user32.dll
		static PFunSetScrollInfo s_SetScrollInfo;
		static int WINAPI HOOK_SetScrollInfo(HWND hwnd, int nBar, LPCSCROLLINFO lpsi,BOOL redraw);
		static PFunGetDCEx s_GetDCEx;
		static HDC WINAPI HOOK_GetDCEx(HWND hWnd, HRGN hrgnClip, DWORD flags);
		static PFunGetDC s_GetDC;
		static HDC WINAPI HOOK_GetDC(HWND hWnd);
		static PFunBeginPaint s_BeginPaint;
		static HDC WINAPI HOOK_BeginPaint(HWND hWnd, LPPAINTSTRUCT lpPaint);
		static PFunEndPaint s_EndPaint;
		static HDC WINAPI HOOK_EndPaint(HWND hWnd, PAINTSTRUCT *lpPaint);
		static PFunDrawTextW s_DrawTextW;
		static int WINAPI HOOK_DrawTextW(HDC hDC, LPCTSTR lpchText, int nCount, LPRECT lpRect, UINT uFormat);
		static PFunDrawTextExW s_DrawTextExW;
		static int WINAPI HOOK_DrawTextExW(HDC hdc,LPTSTR lpchText,int cchText,LPRECT lprc,UINT dwDTFormat,LPDRAWTEXTPARAMS lpDTParams);

		static PFuncGetCursorPos s_GetCursorPos;
		static BOOL WINAPI HOOK_GetCursorPos(LPPOINT lpPoint);
		static PFuncSetCursor s_SetCursor;
		static HCURSOR WINAPI HOOK_SetCursor(HCURSOR hCursor);

	};
}

#endif //_CROSS_RENDER_HOOKER_H__