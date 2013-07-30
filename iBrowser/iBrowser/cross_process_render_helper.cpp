// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "cross_process_render_helper.h"
#include "MessageDef.h"
#include "Base/ipc_message.h"
#include "global_singleton.h"
#include "api_hook.h"
#include <GdiPlus.h>

PFuncBitBlt CrossProcessRenderHelper::s_BitBlt = NULL;
PFuncGetCursorPos CrossProcessRenderHelper::s_GetCursorPos = NULL;
PFuncSetCursor CrossProcessRenderHelper::s_SetCursor = NULL;
PFunAlphaBlend CrossProcessRenderHelper::s_AlphaBlend = NULL;
PFunTransparentBlt CrossProcessRenderHelper::s_TransparentBlt = NULL;


void CrossProcessRenderHelper::SetHost( HWND hHost )
{
	m_hHost = hHost;
}

void CrossProcessRenderHelper::SetCore(HWND hCore)
{
	m_hCore = hCore;
}


BOOL CrossProcessRenderHelper::CustomBitBlt( HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop, PFuncBitBlt pfBitBlt )
{
	if (pfBitBlt == NULL){
		return FALSE;
	}
	/////////////////TEST/////////////////////////
	//HDC hDCHost = ::GetDC(m_hHost);
	//pfBitBlt(hDCHost, x, y, cx, cy, hdcSrc, x1,y1, rop);
	//::ReleaseDC(m_hHost, hDCHost);
	//return TRUE;
	/////////////////////////////////////////////

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
		

		HDC hdcMemory = ::CreateCompatibleDC(hdc);
		HBITMAP hBmp = ::CreateCompatibleBitmap(hdc, cx, cy);
		::SelectObject(hdcMemory, hBmp);
		BOOL bRet = pfBitBlt(hdcMemory, 0, 0, cx, cy, hdcSrc, x1, y1, rop);
		int nDWords = cx*cy+4;
		DWORD* pDWordData = (DWORD*)new DWORD[nDWords];
		*pDWordData = x;
		*(pDWordData+1) = y;
		*(pDWordData+2) = cx;
		*(pDWordData+3) = cy;
		int nBytes = sizeof(DWORD)*nDWords;
		LONG bytes = ::GetBitmapBits(hBmp, nBytes, (void*)(pDWordData+4));
		
		IPC::PostIPCMessage(m_hHost, WM_RENDER_BACK_STORE, (void*)pDWordData, nBytes);
		delete [] pDWordData;
		::DeleteObject(hBmp);
		::DeleteDC(hdcMemory);	
		return bRet;
	}

	return FALSE;
}

Gdiplus::ARGB s_colors[]
={
	0x2CFF0000, 0x2C00FF00, 0x2C0000FF, 0x2CFFFF00, 0x2C00FFFF
};

void CrossProcessRenderHelper::RenderOnHost( HWND hHost, void* pScreenData )
{
	if (GlobalSingleton::GetInstance()->GetCrossProcessRender()){
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

BOOL WINAPI CrossProcessRenderHelper::HOOK_BitBlt( HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop )
{
	if (GlobalSingleton::GetInstance()->GetCrossProcessRender()){
		return CrossProcessRenderHelper::GetInstance()->CustomBitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop, s_BitBlt);
		return s_BitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop); 
	}else{
		return s_BitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
	}
}

bool CrossProcessRenderHelper::StartHooksInCoreProcess( void )
{
	if (GlobalSingleton::GetInstance()->GetCrossProcessRender()){
		HookLibAndProc("gdi32.dll", "BitBlt", (void*)HOOK_BitBlt, (void**)&s_BitBlt);
		HookLibAndProc("user32.dll", "GetCursorPos", (void*)HOOK_GetCursorPos, (void**)&s_GetCursorPos);
		HookLibAndProc("user32.dll", "SetCursor", (void*)HOOK_SetCursor, (void**)&s_SetCursor);
		HookLibAndProc("msimg32.dll", "AlphaBlend", (void*)HOOK_AlphaBlend, (void**)&s_AlphaBlend);
		HookLibAndProc("msimg32.dll", "TransparentBlt", (void*)HOOK_TransparentBlt, (void**)&s_TransparentBlt);
		
	}
	return true;
}

BOOL WINAPI CrossProcessRenderHelper::HOOK_GetCursorPos( LPPOINT lpPoint )
{
	BOOL bRet = s_GetCursorPos(lpPoint);
	
	HWND hHost = CrossProcessRenderHelper::GetInstance()->m_hHost;
	HWND hCore = CrossProcessRenderHelper::GetInstance()->m_hCore;
	if (::IsWindow(hHost) && ::IsWindow(hCore)){
		RECT rectHost, rectCore;
		::GetWindowRect(hHost,&rectHost);
		::GetWindowRect(hCore, &rectCore);
		lpPoint->x = lpPoint->x - rectHost.left + rectCore.left;
		lpPoint->y = lpPoint->y - rectHost.top + rectCore.top;
	}
	return bRet;
}

HCURSOR WINAPI CrossProcessRenderHelper::HOOK_SetCursor( HCURSOR hCursor )
{
	return s_SetCursor(hCursor);
}

BOOL WINAPI CrossProcessRenderHelper::HOOK_AlphaBlend( HDC hdcDest
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

BOOL WINAPI CrossProcessRenderHelper::HOOK_TransparentBlt( HDC hdcDest,
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




