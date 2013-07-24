// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "api_hook.h"
#include <atlstr.h>
#include "UtilDebug.h"
#include "BrowserThreadManager.h"
#include "gamemode/accelerator.h"
#include "core_cookie.h"
#include "MessageDef.h"
#include "Base/ipc_message.h"
#include "global_singleton.h"
#include "cross_process_render_helper.h"

PFuncSendMessage g_SendMessageA = NULL;
PFuncSendMessage g_SendMessageW = NULL;
PFunSetFocus g_SetFocus = NULL;
PFuncWindowProc g_DefWindowProcW = NULL;
PFuncWindowProc g_DefWindowProcA = NULL;
PFuncBitBlt g_BitBlt = NULL;

LRESULT WINAPI HOOK_SendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return g_SendMessageA(hWnd, Msg, wParam, lParam);
}

LRESULT WINAPI HOOK_SendMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return g_SendMessageW(hWnd, Msg, wParam, lParam);
}

HWND WINAPI HOOK_SetFocus(HWND hWnd)
{
	return g_SetFocus(hWnd);
}

LONG HookLibAndProc(IN LPCSTR LibName, IN LPCSTR FunctionName, IN PVOID pCallbackProc, IN PVOID *ppRealFuncProc)
{
	LONG result = ERROR_SUCCESS;
	if (!pCallbackProc || !ppRealFuncProc || !LibName || !FunctionName)
	{
		result = ERROR_INVALID_PARAMETER;
		return result;
	}
	if(!LibName || !FunctionName)
	{
		return ERROR_INVALID_PARAMETER;
	}
	if (!ppRealFuncProc)
	{
		result = ERROR_INVALID_PARAMETER;
		return result;
	}
	*ppRealFuncProc = DetourFindFunction(LibName, FunctionName);
	if(!*ppRealFuncProc)
	{
		result = GetLastError();
		return result;
	}
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(ppRealFuncProc, pCallbackProc);
	result =  DetourTransactionCommit();
	return result;
}


LRESULT CALLBACK HOOK_CallDefWindowProcW( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	LRESULT result = g_DefWindowProcW(hWnd, Msg, wParam, lParam);
	return result;
}

LRESULT CALLBACK HOOK_CallDefWindowProcA( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	LRESULT result = g_DefWindowProcA(hWnd, Msg, wParam, lParam);
	return result;
}



bool StartCoreProcessHooks( void ){
	HookLibAndProc("Kernel32.dll", "GetTickCount", (void*)&GameMode::Accelerator::HOOK_GetTickCount, (void**)&GameMode::Accelerator::Real_GetTickCount);
	HookLibAndProc("wininet.dll", "InternetSetCookieExW", (void*)CoreCookie::HOOK_InternetSetCookieExW, (void**)&CoreCookie::Real_InternetSetCookieExW);
	HookLibAndProc("wininet.dll", "InternetSetCookieExA", (void*)CoreCookie::HOOK_InternetSetCookieExA, (void**)&CoreCookie::Real_InternetSetCookieExA);
	HookLibAndProc("gdi32.dll", "BitBlt", (void*)HOOK_BitBlt, (void**)&g_BitBlt);
	return true;
}

bool StartMainProcessHooks( void ){
	HookLibAndProc("Kernel32.dll", "GetTickCount", (void*)&GameMode::Accelerator::HOOK_GetTickCount, (void**)&GameMode::Accelerator::Real_GetTickCount);
	return true;
}

BOOL WINAPI HOOK_BitBlt( HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop )
{
	if (GlobalSingleton::GetInstance()->GetCrossProcessRender()){
		CrossProcessRenderHelper::GetInstance()->CustomBitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop, g_BitBlt);
	}else{
		return g_BitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
	}
	
}
