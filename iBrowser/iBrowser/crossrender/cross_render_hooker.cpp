// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "stdafx.h"
#include "cross_render_hooker.h"
#include "global_singleton.h"
#include "api_hook.h"

namespace CrossRender
{

///gdi32.dll
PFuncBitBlt CrossRenderHooker::s_BitBlt = NULL;
PFunStretchBlt CrossRenderHooker::s_StretchBlt = NULL;

///msimg.dll
PFunAlphaBlend CrossRenderHooker::s_AlphaBlend = NULL;
PFunTransparentBlt CrossRenderHooker::s_TransparentBlt = NULL;

///user32.dll
PFunGetDCEx CrossRenderHooker::s_GetDCEx = NULL;
PFunGetDC CrossRenderHooker::s_GetDC = NULL;
PFunBeginPaint CrossRenderHooker::s_BeginPaint = NULL;
PFunSetScrollInfo CrossRenderHooker::s_SetScrollInfo = NULL;
PFunEndPaint CrossRenderHooker::s_EndPaint = NULL;
PFuncGetCursorPos CrossRenderHooker::s_GetCursorPos = NULL;
PFuncSetCursor CrossRenderHooker::s_SetCursor = NULL;
PFunDrawText CrossRenderHooker::s_DrawText = NULL;

bool CrossRenderHooker::StartHooksInCoreProcess( void )
{
	if (GlobalSingleton::GetInstance()->IsCrossRender()){

		HookLibAndProc("gdi32.dll", "BitBlt", (void*)HOOK_BitBlt, (void**)&s_BitBlt);
		HookLibAndProc("gdi32.dll", "StretchBlt", (void*)HOOK_StretchBlt, (void**)&s_StretchBlt);

		HookLibAndProc("msimg32.dll", "AlphaBlend", (void*)HOOK_AlphaBlend, (void**)&s_AlphaBlend);
		HookLibAndProc("msimg32.dll", "TransparentBlt", (void*)HOOK_TransparentBlt, (void**)&s_TransparentBlt);		

		HookLibAndProc("user32.dll", "GetDCEx", (void*)HOOK_GetDCEx, (void**)&s_GetDCEx);
		HookLibAndProc("user32.dll", "GetDC", (void*)HOOK_GetDC, (void**)&s_GetDC);
		HookLibAndProc("user32.dll", "BeginPaint", (void*)HOOK_BeginPaint, (void**)&s_BeginPaint);
		HookLibAndProc("user32.dll", "EndPaint", (void*)HOOK_EndPaint, (void**)&s_EndPaint);
		HookLibAndProc("user32.dll", "SetScrollInfo", (void*)HOOK_SetScrollInfo, (void**)&s_SetScrollInfo);
		HookLibAndProc("user32.dll", "GetCursorPos", (void*)HOOK_GetCursorPos, (void**)&s_GetCursorPos);
		HookLibAndProc("user32.dll", "SetCursor", (void*)HOOK_SetCursor, (void**)&s_SetCursor);
		HookLibAndProc("user32.dll", "DrawText", (void*)HOOK_DrawText, (void**)&s_DrawText);
	}
	return true;
}

BOOL WINAPI CrossRenderHooker::HOOK_GetCursorPos( LPPOINT lpPoint )
{
	BOOL bRet = s_GetCursorPos(lpPoint);

	//HWND hHost = CrossRenderHelper::GetInstance()->m_hHost;
	//HWND hCore = CrossRenderHelper::GetInstance()->m_hCore;
	//if (::IsWindow(hHost) && ::IsWindow(hCore)){
	//	RECT rectHost, rectCore;
	//	::GetWindowRect(hHost,&rectHost);
	//	::GetWindowRect(hCore, &rectCore);
	//	lpPoint->x = lpPoint->x - rectHost.left + rectCore.left;
	//	lpPoint->y = lpPoint->y - rectHost.top + rectCore.top;
	//}
	return bRet;
}

HCURSOR WINAPI CrossRenderHooker::HOOK_SetCursor( HCURSOR hCursor )
{
	return s_SetCursor(hCursor);
}

HDC WINAPI CrossRenderHooker::HOOK_DrawText( HDC hDC, LPCTSTR lpchText, int nCount, LPRECT lpRect, UINT uFormat )
{
	return s_DrawText(hDC, lpchText, nCount, lpRect, uFormat);
}


BOOL WINAPI CrossRenderHooker::HOOK_AlphaBlend( HDC hdcDest
	, int xoriginDest
	, int yoriginDest
	, int wDest
	, int hDest
	, HDC hdcSrc
	, int xoriginSrc
	, int yoriginSrc
	, int wSrc
	, int hSrc
	, BLENDFUNCTION ftn )
{
	if (GlobalSingleton::GetInstance()->IsCrossRender()){
		return CrossRenderHelper::GetInstance()->CustomAlphaBlend(hdcDest
			, xoriginDest
			, yoriginDest
			, wDest
			, hDest
			, hdcSrc
			, xoriginSrc
			, yoriginSrc
			, wSrc
			, hSrc
			, ftn);
	}else{
		return s_AlphaBlend(hdcDest
			,xoriginDest
			,yoriginDest
			,wDest
			,hDest
			,hdcSrc
			,xoriginSrc
			,yoriginSrc
			,wSrc
			,hSrc
			,ftn);
	}

}

BOOL WINAPI CrossRenderHooker::HOOK_TransparentBlt( HDC hdcDest,
	int xoriginDest,
	int yoriginDest,
	int wDest,
	int hDest,
	HDC hdcSrc,
	int xoriginSrc,
	int yoriginSrc,
	int wSrc,
	int hSrc,
	UINT crTransparent)
{
	if (GlobalSingleton::GetInstance()->IsCrossRender()){
		return CrossRenderHelper::GetInstance()->CustomTransparentBlt(hdcDest
			, xoriginDest
			, yoriginDest
			, wDest
			, hDest
			, hdcSrc
			, xoriginSrc
			, yoriginSrc
			, wSrc
			, hSrc
			, crTransparent);
	}else{
		return s_TransparentBlt(hdcDest
			,xoriginDest
			,yoriginDest
			,wDest
			,hDest
			,hdcSrc
			,xoriginSrc
			,yoriginSrc
			,wSrc
			,hSrc
			,crTransparent);
	}
}

HDC WINAPI CrossRenderHooker::HOOK_BeginPaint( HWND hWnd, LPPAINTSTRUCT lpPaint )
{
	return CrossRenderHelper::GetInstance()->CustomBeginPaint(hWnd, lpPaint);
}

HDC WINAPI CrossRenderHooker::HOOK_EndPaint( HWND hWnd, PAINTSTRUCT *lpPaint )
{
	return CrossRenderHelper::GetInstance()->CustomEndPaint(hWnd, lpPaint);
}


HDC WINAPI CrossRenderHooker::HOOK_GetDCEx( HWND hWnd, HRGN hrgnClip, DWORD flags )
{
	return s_GetDCEx(hWnd, hrgnClip, flags);
}

HDC WINAPI CrossRenderHooker::HOOK_GetDC( HWND hWnd )
{
	return s_GetDC(hWnd);
}



int WINAPI CrossRenderHooker::HOOK_SetScrollInfo( HWND hwnd, int nBar, LPCSCROLLINFO lpsi,BOOL redraw )
{
	return s_SetScrollInfo(hwnd, nBar, lpsi, redraw);
}

BOOL WINAPI CrossRenderHooker::HOOK_StretchBlt( HDC hdcDest 
	, int xDest 
	, int yDest 
	, int wDest 
	, int hDest 
	, HDC hdcSrc 
	, int xSrc 
	, int ySrc 
	, int wSrc 
	, int hSrc 
	, DWORD rop )
{
	if (GlobalSingleton::GetInstance()->IsCrossRender()){
		return CrossRenderHelper::GetInstance()->CustomStretchBlt(hdcDest
			, xDest
			, yDest
			, wDest
			, hDest
			, hdcSrc
			, xSrc
			, ySrc
			, wSrc
			, hSrc
			, rop);
	}
	return s_StretchBlt(hdcDest, xDest, yDest, wDest, hDest, hdcSrc, xSrc, ySrc, wSrc, hSrc, rop);
}

BOOL WINAPI CrossRenderHooker::HOOK_BitBlt( HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop )
{
	if /*(0){*/(GlobalSingleton::GetInstance()->IsCrossRender()){
		return CrossRenderHelper::GetInstance()->CustomBitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
	}else{
		return s_BitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
	}
}

}
