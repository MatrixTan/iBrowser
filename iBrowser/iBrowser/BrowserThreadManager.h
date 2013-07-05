// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _BROWSER_THREAD_MANAGER_H__
#define _BROWSER_THREAD_MANAGER_H__

#include <Base/TSingleton.h>
#include <atlctrls.h>
#include <atlstr.h>
extern CAppModule _Module;

class CBrowserThreadManager : public TSingleton<CBrowserThreadManager>
{
public:
	// thread init param
	struct _RunData
	{
		LPTSTR lpstrCmdLine;
	};


	class ContainerWindowData
	{
	public:
		ContainerWindowData()
			:m_hParent(NULL)
		{}
		HWND m_hParent;
		CStringW m_bsURL;
	};	

	// thread proc
	static DWORD WINAPI CreateMainFrame(LPVOID lpData);
	CBrowserThreadManager();

	// Operations
	DWORD AddThread(LPTHREAD_START_ROUTINE pStartRoutine ,LPVOID param);
	void RemoveThread(DWORD dwIndex);
	int Run(LPTSTR lpstrCmdLine, int nCmdShow);
	HWND hMainFrame;

protected:
	VOID ProcessCmdLine(LPTSTR lpstrCmdLine);

private:
	DWORD m_dwCount;
	HANDLE m_hProcessJob;
	HANDLE m_arrThreadHandles[MAXIMUM_WAIT_OBJECTS - 1];
};

#endif  //_BROWSER_THREAD_MANAGER_H__