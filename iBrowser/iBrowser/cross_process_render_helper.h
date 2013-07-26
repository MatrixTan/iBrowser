// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _CROSS_PROCESS_RENDER_HELPER_H__
#define _CROSS_PROCESS_RENDER_HELPER_H__

#include "Base/t_thread_singleton.h"
#include "api_hook.h"

typedef BOOL (WINAPI *PFuncBitBlt)(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);
typedef BOOL (WINAPI *PFuncGetCursorPos)(LPPOINT lpPoint);
typedef HCURSOR (WINAPI *PFuncSetCursor)(HCURSOR hCursor);

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

	///Main Process Method
	static void RenderOnHost(HWND hHost, void* pScreenData);
protected:
private:
	HWND m_hHost;
	HWND m_hCore;

	static PFuncBitBlt s_BitBlt;
	static PFuncGetCursorPos s_GetCursorPos;
	static PFuncSetCursor s_SetCursor;
};


#endif //_CROSS_PROCESS_RENDER_HELPER_H__