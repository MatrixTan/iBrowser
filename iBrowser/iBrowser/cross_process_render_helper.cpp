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
	if (m_hHost != NULL){

		HWND hTarget = ::WindowFromDC(hdc);
		if ( hTarget != m_hCore){
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
		HDC h = ::GetDC(hHost);
		HBITMAP hBitmap = ::CreateCompatibleBitmap(h, cx, cy);
		::SetBitmapBits(hBitmap, sizeof(DWORD)*cx*cy, (void*)(pData+4));
		Gdiplus::Graphics g(h);
		Gdiplus::Bitmap bitmap(hBitmap, NULL);

		///////////////TEST////////////////
		//for (int w = 0; w < bitmap.GetWidth(); ++w){
		//	for (int h = 0; h < bitmap.GetHeight(); ++h){
		//		Gdiplus::Color color;
		//		bitmap.GetPixel(w,h,&color);
		//		static int s=0;
		//		s = (s+1)%3;
		//		if (s == 0)
		//		{
		//			Gdiplus::Color newColor(255, 255, color.GetG(), color.GetB());
		//			bitmap.SetPixel(w,h, newColor);
		//		}else if (s == 1)
		//		{
		//			Gdiplus::Color newColor(255, color.GetR(), 255, color.GetB());
		//			bitmap.SetPixel(w,h, newColor);
		//		}else if (s == 2)
		//		{
		//			Gdiplus::Color newColor(255, color.GetR(), color.GetG(), 255);
		//			bitmap.SetPixel(w,h, newColor);
		//		}				
		//	}
		//}
		///////////////////////////////
		g.DrawImage(&bitmap, (INT)x, (INT)y, (INT)cx, (INT)cy);
		::DeleteObject(hBitmap);
		::ReleaseDC(hHost, h);
	}
}

BOOL WINAPI CrossProcessRenderHelper::HOOK_BitBlt( HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop )
{
	if (GlobalSingleton::GetInstance()->GetCrossProcessRender()){
		CrossProcessRenderHelper::GetInstance()->CustomBitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop, s_BitBlt);
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


