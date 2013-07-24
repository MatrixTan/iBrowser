// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "cross_process_render_helper.h"
#include "MessageDef.h"
#include "Base/ipc_message.h"


void CrossProcessRenderHelper::Initialize( HWND hHost )
{
	m_hHost = hHost;
}

BOOL CrossProcessRenderHelper::CustomBitBlt( HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop, PFuncBitBlt pfBitBlt )
{
	if (pfBitBlt == NULL){
		return FALSE;
	}
	if (m_hHost != NULL){

		HDC hdcMemory = ::CreateCompatibleDC(hdc);
		HBITMAP bmp = ::CreateCompatibleBitmap(hdc, cx, cy);
		::SelectObject(hdcMemory, bmp);
		pfBitBlt(hdcMemory, 0, 0, cx, cy, hdcSrc, x1, y1, rop);
		int nDWords = cx*cy+4;
		DWORD* pDWordData = (DWORD*)new DWORD[nDWords];
		*pDWordData = x;
		*(pDWordData+1) = y;
		*(pDWordData+2) = cx;
		*(pDWordData+3) = cy;
		int nBytes = sizeof(DWORD)*nDWords;
		LONG bytes = ::GetBitmapBits(bmp, nBytes, (void*)(pDWordData+4));
		::DeleteDC(hdcMemory);	

		IPC::PostIPCMessage(m_hHost, WM_RENDER_BACK_STORE, (void*)pDWordData, nBytes);
		delete [] pDWordData;
		return pfBitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
	}

	return FALSE;
}
