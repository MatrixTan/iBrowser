// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "stdafx.h"
#include "BrowserThreadManager.h"
#include "MainFrm.h"
#include "core_view.h"
#include <Base/CommandLine.h>
#include "switches.h"

DWORD WINAPI CBrowserThreadManager::CreateMainFrame(LPVOID lpData)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	_RunData* pData = (_RunData*)lpData;
	CMainFrame wndFrame;

	HWND hWnd = wndFrame.CreateEx(NULL, 0, CMainFrame::kStyle, CMainFrame::kExStyle);
	//HWND hWnd = wndFrame.CreateEx();
	if(hWnd == NULL)
	{
		ATLTRACE(_T("Frame window creation failed!\n"));
		return 0;
	}
	CBrowserThreadManager::GetInstance()->hMainFrame = hWnd;

	wndFrame.ShowWindow(SW_SHOWMAXIMIZED);
	::SetForegroundWindow(wndFrame);	// Win95 needs this
	wndFrame.Init();

	if (pData != NULL)
	{
		delete pData;
		pData = NULL;
	}

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return 0;
}

CBrowserThreadManager::CBrowserThreadManager() 
:m_dwCount(0)
,hMainFrame(NULL)
{
	m_hProcessJob = CreateJobObject(NULL, NULL);
	ATLASSERT(m_hProcessJob);

	JOBOBJECT_EXTENDED_LIMIT_INFORMATION jobExtLimitInfo = { 0 };
	jobExtLimitInfo.BasicLimitInformation.LimitFlags = 
		JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;

	SetInformationJobObject(m_hProcessJob, 
		JobObjectExtendedLimitInformation,
		&jobExtLimitInfo,
		sizeof(jobExtLimitInfo)
		);
}

// Operations
DWORD CBrowserThreadManager::AddThread(LPTHREAD_START_ROUTINE pStartRoutine,PVOID param)
{
	if(m_dwCount == (MAXIMUM_WAIT_OBJECTS - 1))
	{
		::MessageBox(NULL, _T("ERROR: Cannot create ANY MORE threads!!!"), _T("iBrowser"), MB_OK);
		return 0;
	}

	DWORD dwThreadID;
	HANDLE hThread = ::CreateThread(NULL, 0, pStartRoutine, param, 0, &dwThreadID);
	if(hThread == NULL)
	{
		::MessageBox(NULL, _T("ERROR: Cannot create thread!!!"), _T("iBrowser"), MB_OK);
		return 0;
	}

	m_arrThreadHandles[m_dwCount] = hThread;
	m_dwCount++;
	return dwThreadID;
}

void CBrowserThreadManager::RemoveThread(DWORD dwIndex)
{
	::CloseHandle(m_arrThreadHandles[dwIndex]);
	if(dwIndex != (m_dwCount - 1))
		m_arrThreadHandles[dwIndex] = m_arrThreadHandles[m_dwCount - 1];
	m_dwCount--;
}

VOID CBrowserThreadManager::ProcessCmdLine(LPTSTR lpstrCmdLine)
{
	CommandLine cl;
	cl.ParseFromString(::GetCommandLineW());

	CStringW strParengHwnd = cl.GetSwitchValue(switches::kHWND);
	
	if(strParengHwnd.IsEmpty())
	{
		_RunData *pStartRoutine = new _RunData;
		pStartRoutine->lpstrCmdLine = lpstrCmdLine;
		CreateMainFrame(pStartRoutine);
	}
	else
	{
		HWND hParent = (HWND)_wtoi64((LPCWSTR)strParengHwnd);
		ATLASSERT(NULL != hParent);
	}
}

int CBrowserThreadManager::Run(LPTSTR lpstrCmdLine, int nCmdShow)
{
	MSG msg;
	// force message queue to be created
	::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

	ProcessCmdLine(lpstrCmdLine);

// 	int nRet = m_dwCount;
// 	DWORD dwRet;
// 	while(m_dwCount > 0)
// 	{
// 		dwRet = ::MsgWaitForMultipleObjects(m_dwCount, m_arrThreadHandles, FALSE, INFINITE, QS_ALLINPUT);
// 
// 		if(dwRet == 0xFFFFFFFF)
// 		{
// 			::MessageBox(NULL, _T("ERROR: Wait for multiple objects failed!!!"), _T("iBrowser"), MB_OK);
// 		}
// 		else if(dwRet >= WAIT_OBJECT_0 && dwRet <= (WAIT_OBJECT_0 + m_dwCount - 1))
// 		{
// 			RemoveThread(dwRet - WAIT_OBJECT_0);
// 		}
// 		else if(dwRet == (WAIT_OBJECT_0 + m_dwCount))
// 		{
// 			if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
// 			{
// 			
// 			}
// 		}
// 		else
// 		{
// 			::MessageBeep((UINT)-1);
// 		}
// 	}

	return 0;
}

