// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "APIHook.h"
#include <atlstr.h>
#include "UtilDebug.h"
#include "BrowserThreadManager.h"
#include "gamemode/accelerator.h"

PFuncSendMessage g_SendMessageA = NULL;
PFuncSendMessage g_SendMessageW = NULL;
PFunSetFocus g_SetFocus = NULL;
PFuncWindowProc g_DefWindowProcW = NULL;
PFuncWindowProc g_DefWindowProcA = NULL;

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
	return true;	
}
