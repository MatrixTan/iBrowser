// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "cross_render_helper.h"
#include "../MessageDef.h"
#include "Base/ipc_message.h"
#include "../global_singleton.h"
#include "../api_hook.h"
#include <GdiPlus.h>

namespace CrossRender{

PFuncBitBlt CrossRenderHelper::s_BitBlt = NULL;
PFuncGetCursorPos CrossRenderHelper::s_GetCursorPos = NULL;
PFuncSetCursor CrossRenderHelper::s_SetCursor = NULL;
PFunAlphaBlend CrossRenderHelper::s_AlphaBlend = NULL;
PFunTransparentBlt CrossRenderHelper::s_TransparentBlt = NULL;
PFunGetDCEx CrossRenderHelper::s_GetDCEx = NULL;
PFunGetDC CrossRenderHelper::s_GetDC = NULL;


void CrossRenderHelper::SetHost( HWND hHost )
{
	m_hHost = hHost;
}

void CrossRenderHelper::SetCore(HWND hCore)
{
	m_hCore = hCore;
}


BOOL CrossRenderHelper::CustomBitBlt( HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop, PFuncBitBlt pfBitBlt )
{
	if (pfBitBlt == NULL){
		return FALSE;
	}
	

	if (m_hHost != NULL && m_hCore != NULL){

		HWND hTarget = ::WindowFromDC(hdc);
		if ( hTarget != m_hCore){
			//render the core and child window only.
			HWND hParent = ::GetParent(hTarget);
			while (hParent != m_hCore && (hParent = ::GetParent(hParent))){
				
			}
			if (hParent == NULL){
				return FALSE;
			}

			//add the offset of child window rect.
			RECT rectTarget = {0};
			::GetWindowRect(hTarget, &rectTarget);
			RECT rectCore = {0};
			::GetWindowRect(m_hCore, &rectCore);
			x += (rectTarget.left - rectCore.left);
			y += (rectTarget.top - rectCore.top);
		}

		HDC hDCHost = ::GetDC(m_hHost);
		BOOL bRet = pfBitBlt(hDCHost, x, y, cx, cy, hdcSrc, x1,y1, rop);
		::ReleaseDC(m_hHost, hDCHost);		
		return bRet;
	}

	return FALSE;
}

Gdiplus::ARGB s_colors[]
={
	0x2CFF0000, 0x2C00FF00, 0x2C0000FF, 0x2CFFFF00, 0x2C00FFFF
};

void CrossRenderHelper::RenderOnHost( HWND hHost, void* pScreenData )
{
	if (GlobalSingleton::GetInstance()->IsCrossRender()){
		if (pScreenData == NULL){
			return;
		}
		
		DWORD *pData = (DWORD*)pScreenData;
		DWORD x = *pData;
		DWORD y = *(pData+1);
		DWORD cx = *(pData+2);
		DWORD cy = *(pData+3);
///////////////////TEST/////////////////
		CString strLog;
		strLog.Format(L"back store: x:%d,y:%d,cx:%d,cy:%d\n", x, y, cx, cy);
		::OutputDebugStringW(strLog);
////////////////////////////////////////
		HDC h = ::GetDC(hHost);
		HBITMAP hBitmap = ::CreateCompatibleBitmap(h, cx, cy);
		::SetBitmapBits(hBitmap, sizeof(DWORD)*cx*cy, (void*)(pData+4));
		Gdiplus::Graphics g(h);
		Gdiplus::Bitmap bitmap(hBitmap, NULL);
		
		g.DrawImage(&bitmap, (INT)x, (INT)y, (INT)cx, (INT)cy);

		///////////TEST/////////////////////////
		static int colerIndex = 0;
		if (colerIndex > 4)
		{
			colerIndex = 0;
		}
		Gdiplus::SolidBrush brush(s_colors[colerIndex++]);
		g.FillRectangle(&brush,(INT)x, (INT)y, (INT)cx, (INT)cy);
		/////////////////////////////////////////////

		::DeleteObject(hBitmap);
		::ReleaseDC(hHost, h);
	}
}

BOOL WINAPI CrossRenderHelper::HOOK_BitBlt( HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop )
{
	if (GlobalSingleton::GetInstance()->IsCrossRender()){
		CrossRenderHelper::GetInstance()->CustomBitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop, s_BitBlt);
		return s_BitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop); 
	}else{
		return s_BitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
	}
}

bool CrossRenderHelper::StartHooksInCoreProcess( void )
{
	if (GlobalSingleton::GetInstance()->IsCrossRender()){
		HookLibAndProc("gdi32.dll", "BitBlt", (void*)HOOK_BitBlt, (void**)&s_BitBlt);
		HookLibAndProc("user32.dll", "GetCursorPos", (void*)HOOK_GetCursorPos, (void**)&s_GetCursorPos);
		HookLibAndProc("user32.dll", "SetCursor", (void*)HOOK_SetCursor, (void**)&s_SetCursor);
		HookLibAndProc("msimg32.dll", "AlphaBlend", (void*)HOOK_AlphaBlend, (void**)&s_AlphaBlend);
		HookLibAndProc("msimg32.dll", "TransparentBlt", (void*)HOOK_TransparentBlt, (void**)&s_TransparentBlt);
		HookLibAndProc("user32.dll", "GetDCEx", (void*)HOOK_GetDCEx, (void**)&s_GetDCEx);
		HookLibAndProc("user32.dll", "GetDC", (void*)HOOK_GetDC, (void**)&s_GetDC);
	}
	return true;
}

BOOL WINAPI CrossRenderHelper::HOOK_GetCursorPos( LPPOINT lpPoint )
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

HCURSOR WINAPI CrossRenderHelper::HOOK_SetCursor( HCURSOR hCursor )
{
	return s_SetCursor(hCursor);
}

BOOL WINAPI CrossRenderHelper::HOOK_AlphaBlend( HDC hdcDest
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

BOOL WINAPI CrossRenderHelper::HOOK_TransparentBlt( HDC hdcDest,
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
	return s_TransparentBlt(hdcDest,
		xoriginDest,
		yoriginDest,
		wDest,
		hDest,
		hdcSrc,
		xoriginSrc,
		yoriginSrc,
		wSrc,
		hSrc,
		crTransparent);
}

CrossRenderHelper::CrossRenderHelper()
	:m_hCore(NULL)
	,m_hHost(NULL)
	,m_hContainer(NULL)
{}

CrossRenderHelper::~CrossRenderHelper()
{
	m_hHost = NULL;
	m_hCore = NULL;
	m_hContainer = NULL;
}

HDC WINAPI CrossRenderHelper::HOOK_GetDCEx( HWND hWnd, HRGN hrgnClip, DWORD flags )
{
	return s_GetDCEx(hWnd, hrgnClip, flags);
}

HDC WINAPI CrossRenderHelper::HOOK_GetDC( HWND hWnd )
{
	return s_GetDC(hWnd);
}

void CrossRenderHelper::ResizeHost( int cx, int cy )
{
	::SetWindowPos(m_hHost, HWND_TOP, 0, 0, cx, cy
		, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOREDRAW|SWP_ASYNCWINDOWPOS);
}

void CrossRenderHelper::SetContainer( HWND hContainer )
{
	m_hContainer = hContainer;
}

void CrossRenderHelper::SyncCoreWinPos( HWND hHostContianer )
{
	if (!GlobalSingleton::GetInstance()->IsCrossRender()){
		return;
	}
	ATLASSERT(::IsWindow(hHostContianer));
	RECT rect = {0};
	::GetClientRect(hHostContianer, &rect);
	if(!::ClientToScreen(hHostContianer, (LPPOINT)&rect))
		return;
	::ClientToScreen(hHostContianer, ((LPPOINT)&rect)+1);
	::SetWindowPos(m_hContainer, HWND_TOP, rect.left, rect.top+3, rect.right-rect.left, rect.bottom-rect.top
		, SWP_NOACTIVATE|SWP_ASYNCWINDOWPOS);
	return;
}

void CrossRenderHelper::OnCoreVisibleChange( bool bVisible )
{
	if (!GlobalSingleton::GetInstance()->IsCrossRender()){
		return;
	}

	::ShowWindow(m_hContainer, bVisible?SW_SHOW:SW_HIDE );
}


}


