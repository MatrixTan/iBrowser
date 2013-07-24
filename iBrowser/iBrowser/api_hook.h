// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef _API_HOOK_H__
#define _API_HOOK_H__

#include <detours.h>
#include <set>

typedef LRESULT (WINAPI *PFuncSendMessage)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
extern PFuncSendMessage g_SendMessageA;
extern PFuncSendMessage g_SendMessageW;

typedef HWND (WINAPI *PFunSetFocus)(HWND hWnd);
extern PFunSetFocus g_SetFocus;

typedef LRESULT (CALLBACK *PFuncWindowProc)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
extern PFuncWindowProc g_DefWindowProcW;
extern PFuncWindowProc g_DefWindowProcA;

typedef BOOL (WINAPI *PFuncBitBlt)(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);
extern PFuncBitBlt g_BitBlt;

LONG HookLibAndProc(IN LPCSTR LibName, IN LPCSTR FunctionName, IN PVOID pCallbackProc, IN PVOID *ppRealFuncProc);
LRESULT WINAPI HOOK_SendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI HOOK_SendMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
HWND WINAPI HOOK_SetFocus(HWND hWnd);
LRESULT CALLBACK HOOK_CallDefWindowProcW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HOOK_CallDefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI HOOK_GetTickCount(void);
BOOL WINAPI HOOK_BitBlt(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);


bool StartCoreProcessHooks(void);
bool StartMainProcessHooks(void);

#endif //_API_HOOK_H__