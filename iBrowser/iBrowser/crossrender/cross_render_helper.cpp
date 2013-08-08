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

namespace CrossRender
{

void CrossRenderHelper::SetHost( HWND hHost )
{
	m_hHost = hHost;
}

void CrossRenderHelper::SetCore(HWND hCore)
{
	m_hCore = hCore;
}
bool CrossRenderHelper::CheckCustomDraw( HDC hTarget, int& x, int& y )
{
	if (m_hHost == NULL || m_hCore == NULL){
		return false;
	}

	HWND hTargetWindow = ::WindowFromDC(hTarget);
	if ( hTargetWindow != m_hCore){
		//render the core and child window only.
		HWND hParent = ::GetParent(hTargetWindow);
		while (hParent != m_hCore && (hParent = ::GetParent(hParent))){

		}
		if (hParent == NULL){
			return false;
		}

		//add the offset of child window rect.
		RECT rectTarget = {0};
		::GetWindowRect(hTargetWindow, &rectTarget);
		RECT rectCore = {0};
		::GetWindowRect(m_hCore, &rectCore);
		x += (rectTarget.left - rectCore.left);
		y += (rectTarget.top - rectCore.top);
	}
	return true;
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

void CrossRenderHelper::SyncHostPos( HWND hCoreView )
{
	RECT rc = {0};
	::GetClientRect(hCoreView, &rc);
	::SetWindowPos(m_hHost, HWND_TOP, 0, 0, rc.right-rc.left, rc.bottom-rc.top
		, SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOREDRAW|SWP_ASYNCWINDOWPOS);
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





BOOL CrossRenderHelper::CustomBitBlt( HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop)
{
	if (CrossRenderHooker::s_BitBlt == NULL){
		return FALSE;
	}
	
	if (CheckCustomDraw(hdc, x, y)){
		HDC hDCHost = ::GetDC(m_hHost);
		BOOL bRet = CrossRenderHooker::s_BitBlt(hDCHost, x, y, cx, cy, hdcSrc, x1,y1, rop);
		::ReleaseDC(m_hHost, hDCHost);
		return bRet;
	}
	return CrossRenderHooker::s_BitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
}

BOOL CrossRenderHelper::CustomAlphaBlend( HDC hdcDest
	, int xoriginDest
	, int yoriginDest
	, int wDest
	, int hDest
	, HDC hdcSrc
	, int xoriginSrc
	, int yoriginSrc
	, int wSrc
	, int hSrc
	, BLENDFUNCTION ftn)
{
	if (CrossRenderHooker::s_AlphaBlend == NULL){
		return FALSE;
	}

	if (CheckCustomDraw(hdcDest, xoriginDest, yoriginDest)){
		HDC hDCHost = ::GetDC(m_hHost);
		BOOL bRet = CrossRenderHooker::s_AlphaBlend(hDCHost, xoriginDest, yoriginDest, wDest, hDest, hdcSrc, xoriginSrc, yoriginSrc, wSrc, hSrc, ftn);
		::ReleaseDC(m_hHost, hDCHost);		
		return bRet;
	}
	return CrossRenderHooker::s_AlphaBlend(hdcDest, xoriginDest, yoriginDest, wDest, hDest, hdcSrc, xoriginSrc, yoriginSrc, wSrc, hSrc, ftn);
}

BOOL CrossRenderHelper::CustomTransparentBlt( HDC hdcDest
	, int xoriginDest
	, int yoriginDest
	, int wDest
	, int hDest
	, HDC hdcSrc
	, int xoriginSrc
	, int yoriginSrc
	, int wSrc
	, int hSrc
	, UINT crTransparent)
{
	if (CrossRenderHooker::s_TransparentBlt == NULL){
		return FALSE;
	}

	if (CheckCustomDraw(hdcDest, xoriginDest, yoriginDest)){
		HDC hDCHost = ::GetDC(m_hHost);
		BOOL bRet = CrossRenderHooker::s_TransparentBlt(hDCHost
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
		::ReleaseDC(m_hHost, hDCHost);		
		return bRet;
	}
	return CrossRenderHooker::s_TransparentBlt(hdcDest
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
}

HDC CrossRenderHelper::CustomBeginPaint( HWND hWnd, LPPAINTSTRUCT lpPaint)
{
	HDC hdc = NULL;
	if (0/*hWnd == m_hCore*/){
		hdc = CrossRenderHooker::s_BeginPaint(m_hHost, lpPaint);
		lpPaint->rcPaint.top = 0;
		lpPaint->rcPaint.bottom = 600;
		lpPaint->rcPaint.left = 0;
		lpPaint->rcPaint.right = 800;
		lpPaint->hdc = hdc;
	}else{
		hdc = CrossRenderHooker::s_BeginPaint(hWnd, lpPaint);
	}	
	return hdc;
}

HDC CrossRenderHelper::CustomEndPaint( HWND hWnd, LPPAINTSTRUCT lpPaint )
{
	HDC hdc = NULL;
	if (/*hWnd == m_hCore*/0){
		hdc = CrossRenderHooker::s_EndPaint(m_hHost, lpPaint);
	}else{
		hdc = CrossRenderHooker::s_EndPaint(hWnd, lpPaint);
	}	
	return hdc;
}

BOOL CrossRenderHelper::CustomStretchBlt( HDC hdcDest 
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
	if (CrossRenderHooker::s_StretchBlt == NULL){
		return FALSE;
	}

	if (CheckCustomDraw(hdcDest, xDest, yDest)){
		HDC hDCHost = ::GetDC(m_hHost);
		BOOL bRet = CrossRenderHooker::s_StretchBlt(hDCHost
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
		::ReleaseDC(m_hHost, hDCHost);
		return bRet;
	}
	return CrossRenderHooker::s_StretchBlt(hdcDest
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

int CrossRenderHelper::CustomDrawTextW( HDC hDC, LPCTSTR lpchText, int nCount, LPRECT lpRect, UINT uFormat )
{
	if (CrossRenderHooker::s_DrawTextW == NULL){
		return 0;
	}
	int x = lpRect->left;
	int y = lpRect->top;
	if (CheckCustomDraw(hDC, x, y)){
		HDC hHost = ::GetDC(m_hHost);
		lpRect->left = x;
		lpRect->top = y;
		int nRet = CrossRenderHooker::s_DrawTextW(hHost, lpchText, nCount, lpRect, uFormat);
		::ReleaseDC(m_hHost, hHost);
		return nRet;
	}
	return CrossRenderHooker::s_DrawTextW(hDC, lpchText, nCount, lpRect, uFormat);
}




}