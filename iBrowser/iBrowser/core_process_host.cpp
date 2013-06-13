// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include <stdlib.h>
#include <Base/CommandLine.h>

#include "core_process_host.h"
#include "switches.h"
#include "core_process_launcher.h"
#include "MessageDef.h"
#include "global_singleton.h"
#include "core_process_manager.h"
#include <Base/ipc_message.h>



CoreProcessHost::CoreProcessHost(void)
	:m_pStartContext(NULL)
	,m_hCoreMsgHWND(NULL)
	,m_ThreadId(0)
{
	RECT rect = {0,0,0,0};
	Create(NULL, rect, NULL, WS_DISABLED, 0);
}

CoreProcessHost::~CoreProcessHost()
{

}

BOOL CoreProcessHost::PreTranslateMessage( MSG* pMsg )
{
	if (!pMsg){
		return FALSE;
	}

	switch(pMsg->message){
	default:
		return FALSE;
	}	
	return FALSE;
}

int CoreProcessHost::Start(CoreProcessStartContext *pParam)
{
	m_pStartContext = pParam;

	CoreProcessLauncher launcher;
	CommandLine cl;
	cl.ParseFromString(::GetCommandLine());
	cl.AppendSwitchArg(switches::kProcessType, switches::kProcessTypeCore);

	unsigned int nEventName;
	rand_s(&nEventName);
	CString strEventName;
	strEventName.Format(L"Event:%d", nEventName);
	HANDLE hEvent = ::CreateEvent(NULL, FALSE, FALSE, strEventName);
	::ResetEvent(hEvent);
	cl.AppendSwitchArg(switches::kProcessStartEvent, strEventName);

	CString strHostHWND;
	strHostHWND.Format(L"%d", m_hWnd);
	cl.AppendSwitchArg(switches::kProcessHostHWND, strHostHWND);

	CMessageLoop loop;
	_Module.AddMessageLoop(&loop);
	loop.AddMessageFilter(this);

	HANDLE hProcess = launcher.Launch(cl.GetProgram(), cl.GetCommandLine());
	bool bRet = GlobalSingleton::GetInstance()->GetCoreProcessManager()->AssignProcessToJob(hProcess);
	::WaitForSingleObject(hEvent, INFINITE);
	::CloseHandle(hEvent);

	m_ThreadId = ::GetCurrentThreadId();
	loop.Run();
	
	return 0;
}

LRESULT CoreProcessHost::OnCoreProcessCreated( UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	m_hCoreMsgHWND = (HWND)wParam;
	::PostMessage(m_pStartContext->hostWnd, WM_CORE_PROCESS_HOST_READY, (WPARAM)this, 0);
	return 0;
}

void CoreProcessHost::CreateCore( HWND hHost )
{
	IPC::PostIPCMessage(m_hCoreMsgHWND, WM_CREATE_CORE, &hHost, sizeof(hHost));
}

void CoreProcessHost::Stop( void )
{
	if (m_ThreadId != 0){
		::PostThreadMessage(m_ThreadId, WM_QUIT, 0, 0);
	}	
}
