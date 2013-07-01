// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include "resource.h"

#include "core_view.h"
#include "aboutdlg.h"
#include "MainFrm.h"
#include "BrowserThreadManager.h"
#include "ui_util.h"
#include "APIHook.h"
#include <Base/CommandLine.h>
#include "switches.h"
#include "global_singleton.h"
#include "core_process.h"
#include "UtilIECore.h"

CAppModule _Module;


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));
	
	GdiplusInitializeTracker gdiplusIniTracker;

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	GUID guid;
	hRes = _Module.Init(NULL, hInstance,&guid);
	ATLASSERT(SUCCEEDED(hRes));

	AtlAxWinInit();

	UtilIECore::SetUA();
	int nRet = 0;
	CommandLine cl;
	cl.ParseFromString(::GetCommandLine());
	if (cl.GetSwitchValue(switches::kProcessType) == switches::kProcessTypeCore){
		if (cl.GetSwitchValue(switches::kCoreWaitDebugger) == switches::kTrue){
			DWORD dwPid = ::GetCurrentProcessId();
			CString strText;
			strText.Format(L"PID:%d", dwPid);
			::MessageBox(NULL, L"Wait for debugger", strText, MB_OK);
		}
		GlobalSingleton::GetInstance()->SetProcessMode(EPM_MULTIPLE);
		GlobalSingleton::GetInstance()->SetProcessType(EPT_CORE);
		StartCoreProcessHooks();
		CoreProcess coreProcess;
		coreProcess.Run(lpstrCmdLine);

	}else{
		GlobalSingleton::GetInstance()->SetProcessType(EPT_MAIN);
		if (cl.GetSwitchValue(switches::kProcessMode) == switches::kMultiple){
			GlobalSingleton::GetInstance()->SetProcessMode(EPM_MULTIPLE);
		}
		StartMainProcessHooks();
		nRet = CBrowserThreadManager::GetInstance()->Run(lpstrCmdLine, nCmdShow);
	}

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
