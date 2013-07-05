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
#include "core_proxy.h"
#include "core_manager.h"
#include "tab_button.h"
#include "tab_color_manager.h"
#include "url_util.h"
#include "core_container_manager.h"
#include "global_singleton.h"
#include "bookmark_manager.h"

#define CHECK_TIMER_ID 1843
#define HEART_BEAT_TIME 100


CXWindow::CXWindow()
:m_hChildWindow(NULL)
,m_hParentWindow(NULL)
,m_bFreezing(FALSE)
,m_nCreateFlag(ECCF_CreateNew)
{

}

CXWindow::~CXWindow()
{
	if (IsWindow())
	{
		DestroyWindow();
		m_hWnd = NULL;
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
	m_spCoreProxy.Reset(new CoreProxy(m_hChildWindow, this, nFlag));
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


void CXWindow::Initialize( E_CHILEWINDOW_CREATE_FLAG flag
	, const CString& strURL
	, TabButton* pTabButton)
{
	m_strURL = strURL;
	m_nCreateFlag = flag;
	m_spTabButton = pTabButton;
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
	CStringW strURL;
	ReadStrFromIPC((void*)wParam, strURL);
	if (!strURL.IsEmpty()){		
		m_strURL = strURL;
		m_strTitle = m_strURL;
		ShowURL();
		ShowTitle();
		UpdateTabColor();
	}
	return 0;
}

LRESULT CXWindow::OnTitleChange( UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CStringW strTitle;
	ReadStrFromIPC((void*)wParam, strTitle);
	if (!strTitle.IsEmpty()){
		m_strTitle = strTitle;
		ShowTitle();
	}
	return 0;
}

const CStringW& CXWindow::GetTitle( void ) const
{
	return m_strTitle;
}

const CStringW& CXWindow::GetURL( void ) const
{
	return m_strURL;
}

BOOL CXWindow::ShowWindow( int nCmd )
{
	if (nCmd == SW_SHOW){
		ShowURL();
	}	
	return CWindow::ShowWindow(nCmd);
}

void CXWindow::ShowURL( void ) const
{
	::SetWindowText(m_hParentWindow, m_strURL);
}

void CXWindow::ShowTitle( void ) const
{
	m_spTabButton->SetText(m_strTitle);
	m_spTabButton->RedrawWindow();
}

LRESULT CXWindow::OnNavigateComplete( UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CStringW strURL;
	ReadStrFromIPC((void*)wParam, strURL);
	m_strURL = strURL;
	UpdateTabColor();
	return 0;
}

void CXWindow::ReadStrFromIPC( void *pData, CStringW &str ) const
{
	CStringW * pStr = NULL;
	Serialize<CStringW>::Read(pData, &pStr);
	if (pStr){
		str = *pStr;
		delete pStr;
		pStr = NULL;
	}
}

void CXWindow::UpdateTabColor( void )
{
	CString strHost = URLUtil::GetHost(m_strURL);		
	if (!strHost.IsEmpty()){
		DWORD dwTabColor = TabColorManager::GetInstance()->GetColor(strHost);
		m_spTabButton->SetMaskColor(dwTabColor);			
	}
}

void CXWindow::Uninitialize( void )
{
	if (m_spCoreProxy.get()){
		m_spCoreProxy->Destroy();
		m_spCoreProxy = NULL;
	}	
	m_spTabButton = NULL;
}

LRESULT CXWindow::OnCoreDestroyed( UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	::PostMessage(m_hParentWindow, WM_CORE_DESTROYED, (WPARAM)m_hWnd, 0);
	return 0;
}

void CXWindow::Focus( void )
{
	m_spCoreProxy->Focus();
}

bool CXWindow::AddCurrentBookmark( void )
{
	return GlobalSingleton::GetInstance()->GetBookmarkManager()->AddBookmark(m_strURL, m_strTitle, L"");
}
