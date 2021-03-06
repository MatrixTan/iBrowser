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
#include "crossrender/cross_render_helper.h"
#include "crossrender/cross_render_host.h"
#include "crossrender/cross_render_core_container.h"

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

void CoreManager::CreateCoreInProcess( HWND hParent ,const CString& strURL)
{
	
	BrowserViewData *pData = new BrowserViewData();
	pData->hParent = hParent;
	pData->strURL = strURL;

	if (GlobalSingleton::GetInstance()->IsCrossRender()){
		CrossRender::CrossRenderHost * pRenderHost = new CrossRender::CrossRenderHost();
		RECT hostRect ={0};
		::GetClientRect(hParent, &hostRect);
		pRenderHost->Create(hParent, hostRect, NULL
			, CrossRender::CrossRenderHost::kStyle
			, CrossRender::CrossRenderHost::kExStyle);
		ATLASSERT(pRenderHost->IsWindow());
		pData->pCrossRenderHost = pRenderHost;
	}

	CBrowserThreadManager::GetInstance()->AddThread(StartCore_CoreThread, (void*)pData);
}

DWORD CoreManager::StartCore_CoreThread( void *pParam )
{
	::CoInitialize(NULL);

	BrowserViewData *pData = (BrowserViewData *)pParam;
	HWND hParent = pData->hParent;
	CString strURL = pData->strURL;

	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);	

	CoreView view;
	view.Initialize(hParent, strURL);

	RECT rect;
	::GetClientRect(hParent, &rect);
	
	HWND hClient = NULL;
	CrossRender::CrossRenderCoreContainer container;
	if (GlobalSingleton::GetInstance()->IsCrossRender()){
		CRect rectContainer(rect);
		::ClientToScreen(hParent, (LPPOINT)&rectContainer);
		::ClientToScreen(hParent, ((LPPOINT)&rectContainer)+1);
		HWND hContainer = container.Create(hParent, rectContainer, L"ie container"
			, CrossRender::CrossRenderCoreContainer::kStyle
			, CrossRender::CrossRenderCoreContainer::kExStyle);
		SetLayeredWindowAttributes(hContainer,0,1,LWA_ALPHA);

		hClient = view.Create(hContainer, rect, _T("ie host"), CoreView::kStyle , CoreView::kExStyle);
		pData->pCrossRenderHost->SetCoreWindow(hClient);
		CrossRender::CrossRenderHelper::GetInstance()->SetHost(pData->pCrossRenderHost->m_hWnd);
		CrossRender::CrossRenderHelper::GetInstance()->SetContainer(hContainer);
	}else{
		//HWND hClient = view.Create(hParent, rect, _T("{8856F961-340A-11D0-A96B-00C04FD705A2}"), CoreView::kStyle , CoreView::kExStyle);
		hClient = view.Create(hParent, rect, _T("ie host"), CoreView::kStyle , CoreView::kExStyle);
	}	
	ATLASSERT(hClient);
	theLoop.AddMessageFilter(&view);
	theLoop.AddIdleHandler(&view);

	int nRet = theLoop.Run();

	::PostMessage(hParent, WM_CORE_DESTROYED, 0, 0);
	_Module.RemoveMessageLoop();
	delete pData;
	pData = NULL;
	if (container.IsWindow()){
		container.DestroyWindow();
	}
	return 0;
}

void CoreManager::CreateCore( HWND hParent, const CString& strURL)
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
