// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "stdafx.h"
#include "core_main_thread.h"
#include <atlstr.h>
#include "core_manager.h"


int CoreMainThread::Run( void )
{
	CMessageLoop messageLoop;
	_Module.AddMessageLoop(&messageLoop);
	messageLoop.AddMessageFilter(this);

	messageLoop.Run();

	_Module.RemoveMessageLoop();

	return 0;
}

BOOL CoreMainThread::PreTranslateMessage( MSG* pMsg )
{
	return FALSE;
}

CoreMainThread::CoreMainThread()
{
	RECT rect = {0,0,0,0};
	Create(NULL, rect, NULL, WS_DISABLED, 0);
}

CoreMainThread::~CoreMainThread()
{
	
}

LRESULT CoreMainThread::OnCreateCore( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	HWND hParent = *(HWND*)wParam;
	CoreManager::GetInstance()->CreateCoreInProcess(hParent, CString(L""), 0);
	return 0;
}
