// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"

#include <Base/CommandLine.h>

#include "XWindow.h"
#include "BrowserThreadManager.h"
#include "core_view.h"
#include "MessageDef.h"
#include "UtilDebug.h"
#include "global_singleton.h"
#include "switches.h"
#include "core_process_host.h"
#include "core_process_manager.h"
#include "CoreProxy.h"
#include "core_manager.h"

#define CHECK_TIMER_ID 1843
#define HEART_BEAT_TIME 100


CXWindow::CXWindow()
:m_hChildWindow(NULL)
,m_hParentWindow(NULL)
,m_nLastHeartBeatClock(0)
,m_bFreezing(FALSE)
,m_nCreateFlag(ECCF_CreateNew)
{

}

CXWindow::~CXWindow()
{
	if (IsWindow())
	{
		DestroyWindow();
	}	
}

LRESULT CXWindow::OnCreate( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{

	int nProcessMode = GlobalSingleton::GetInstance()->GetProcessMode();
	if (nProcessMode == EPM_SINGLE){
		CoreManager::GetInstance()->CreateCoreInProcess(m_hWnd, m_strURL, m_nCreateFlag);
	}else if (nProcessMode == EPM_MULTIPLE){

		CoreManager::GetInstance()->CreateCore(m_hWnd, m_strURL, m_nCreateFlag);
	}

	m_hParentWindow = GetParent().m_hWnd;
	return 0;
}

BOOL CXWindow::PreTranslateMessage( MSG* pMsg )
{
	return FALSE;
}

BOOL CXWindow::OnIdle()
{
	return FALSE;
}

LRESULT CXWindow::OnDestory( UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	return 0;
}

LRESULT CXWindow::OnClose( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	bHandled = FALSE;
	return 0;
}

LRESULT CXWindow::OnTimer( UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}

LRESULT CXWindow::OnChildWindowCreated( UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/ )
{
	m_hChildWindow = (HWND)wParam;
	UINT nFlag = (UINT)lParam;
	m_spCoreProxy.Reset(new CoreProxy(m_hChildWindow, m_hWnd, nFlag));
	m_spCoreProxy->AddRef();

	::PostMessage(m_hParentWindow,WM_CHILD_WINDOW_CREATED, wParam, (LPARAM)m_spCoreProxy.get());
	return 0;
}

LRESULT CXWindow::OnSize( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	CRect rectClient;
	GetClientRect(&rectClient);
	::MoveWindow(m_hChildWindow, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), FALSE);
	return 0;
}


void CXWindow::Initialize( E_CHILEWINDOW_CREATE_FLAG flag, const CString& strURL )
{
	m_strURL = strURL;
	m_nCreateFlag = flag;
}

LRESULT CXWindow::OnCoreProcessHostReady( UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CoreProcessHost* pHost = (CoreProcessHost*)wParam;
	if (pHost){
		pHost->CreateCore(m_hWnd);
	}
	
	return 0;
}

LRESULT CXWindow::OnBeforeNavigate( UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CStringW *pURL = NULL;
	Serialize<CStringW>::Read((void*)wParam, &pURL);
	if (pURL){

	}	
	return 0;
}
