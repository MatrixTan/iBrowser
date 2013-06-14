// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "core_manager.h"
#include "core_view.h"
#include "XWindow.h"
#include "BrowserThreadManager.h"
#include "core_process_host.h"
#include "global_singleton.h"
#include "core_process_manager.h"

CoreManager* CoreManager::s_Instance = NULL;

CoreManager* CoreManager::GetInstance()
{
	if (NULL == s_Instance){
		s_Instance = new CoreManager();
	}
	return s_Instance;
}

CoreManager::~CoreManager()
{

}

CoreManager::CoreManager()
{

}

void CoreManager::CreateCoreInProcess( HWND hParent ,const CString& strURL, UINT flag)
{
	BrowserViewData *pData = new BrowserViewData();
	pData->hParent = hParent;
	pData->strURL = strURL;
	pData->nCreateFlag = flag;
	CBrowserThreadManager::GetInstance()->AddThread(StartCore_CoreThread, (void*)pData);
}

DWORD CoreManager::StartCore_CoreThread( void *pParam )
{
	BrowserViewData *pData = (BrowserViewData *)pParam;
	HWND hParent = pData->hParent;
	CString strURL = pData->strURL;

	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);	

	CoreView view;
	view.Initialize((E_CHILEWINDOW_CREATE_FLAG)pData->nCreateFlag, strURL);

	RECT rect;
	::GetClientRect(hParent, &rect);
	HWND hClient = view.Create(hParent, rect, _T("{8856F961-340A-11D0-A96B-00C04FD705A2}"), CoreView::kStyle , CoreView::kExStyle);
	ATLASSERT(hClient);
	theLoop.AddMessageFilter(&view);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return 0;
}

void CoreManager::CreateCore( HWND hParent, const CString& strURL, UINT flag )
{
	CoreProcessStartContext *pContext = new CoreProcessStartContext();
	pContext->strURL = strURL;
	pContext->hostWnd = hParent;
	CBrowserThreadManager::GetInstance()->AddThread(StartCoreProcess, (void*)pContext);
}

DWORD WINAPI CoreManager::StartCoreProcess( void *pParam )
{
	CoreProcessHost *pHost = GlobalSingleton::GetInstance()->GetCoreProcessManager()->GetProcessHost();
	if (pHost){
		return pHost->Start((CoreProcessStartContext*)pParam);
	}
	return 0;
}