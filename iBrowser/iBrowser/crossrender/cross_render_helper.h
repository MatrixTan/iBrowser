// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _CROSS_PROCESS_RENDER_HELPER_H__
#define _CROSS_PROCESS_RENDER_HELPER_H__

#include "Base/t_thread_singleton.h"
#include "cross_render_hooker.h"
#include <WinGDI.h>




namespace CrossRender
{

class CrossRenderHelper 
	: public TThreadSingleton<CrossRenderHelper, ETTS_CrossProcessRenderHelper>
{
public:
	void SetHost(HWND hHost);
	void SetCore(HWND hCore);
	void SetContainer(HWND hContainer);
	BOOL CustomBitBlt(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);
	BOOL CustomStretchBlt( HDC hdcDest 
		, int xDest 
		, int yDest 
		, int wDest 
		, int hDest 
		, HDC hdcSrc 
		, int xSrc 
		, int ySrc 
		, int wSrc 
		, int hSrc 
		, DWORD rop );
	BOOL CustomAlphaBlend(HDC hdcDest,
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
	BOOL CustomTransparentBlt(HDC hdcDest,
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

	HDC CustomBeginPaint(HWND hWnd,	LPPAINTSTRUCT lpPaint);
	HDC CustomEndPaint(HWND hWnd, LPPAINTSTRUCT lpPaint);

	void ResizeHost(int cx, int cy);
	void SyncHostPos(HWND hCoreView);
	void SyncCoreWinPos(HWND hHostContianer);
	void OnCoreVisibleChange(bool bVisible);
	

	///Main Process Method
	static void RenderOnHost(HWND hHost, void* pScreenData);

	static const int kCoreWindowOffsetX = -10000;
	static const int kCoreWindowOffsetY = 0;

	CrossRenderHelper();
	~CrossRenderHelper();
protected:

	bool CheckCustomDraw(HDC hTarget, int& x, int& y);
private:
	HWND m_hHost;
	HWND m_hCore;
	HWND m_hContainer;
	
};

}

#endif //_CROSS_PROCESS_RENDER_HELPER_H__