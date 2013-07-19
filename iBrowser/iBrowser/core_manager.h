// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _CORE_MANAGER_H__
#define _CORE_MANAGER_H__

#include <Windows.h>
#include <atlstr.h>
#include "MessageDef.h"

class CoreManager
{
public:
	static CoreManager* GetInstance();
	~CoreManager();

	void CreateCoreInProcess(HWND hParent, const CString& strURL);
	void CreateCore(HWND hParent, const CString& strURL);
protected:
private:
	static DWORD WINAPI StartCore_CoreThread(void *pParam);
	static DWORD WINAPI StartCoreProcess(void *pParam);
	CoreManager();
	static CoreManager* s_Instance;

	class BrowserViewData
	{
	public:
		BrowserViewData()
			:hParent(NULL)
			{}
		HWND hParent;
		CString strURL;
	};
};


#endif //_CORE_MANAGER_H__