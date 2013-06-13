// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include <Base/CommandLine.h>

#include "core_process.h"
#include "core_main_thread.h"
#include "switches.h"
#include "MessageDef.h"

CoreProcess::CoreProcess(){

}

CoreProcess::~CoreProcess(){

}

int CoreProcess::Run( LPTSTR lpCmdLine ){
	CommandLine cl;
	cl.ParseFromString(lpCmdLine);
	CString strHostHWND = cl.GetSwitchValue(switches::kProcessHostHWND);
	HWND hHost = (HWND)_wtoi(strHostHWND.GetBuffer());
	CString strEvent = cl.GetSwitchValue(switches::kProcessStartEvent);	
	HANDLE hEvent = ::OpenEvent(EVENT_MODIFY_STATE, FALSE, strEvent.GetBuffer());
	::SetEvent(hEvent);
	
	CoreMainThread mainThread;
	::PostMessage(hHost, WM_CORE_PROCESS_CREATED, (WPARAM)mainThread.m_hWnd, 0);	
	mainThread.Run();
	return 0;
}
