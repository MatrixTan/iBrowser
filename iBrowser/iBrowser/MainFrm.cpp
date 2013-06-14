// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#include "stdafx.h"
#include "core_view.h"
#include "XWindow.h"
#include "MainFrm.h"
#include "BrowserThreadManager.h"
#include "UtilDebug.h"
#include <Base/CommandLine.h>
#include "global_singleton.h"
#include "switches.h"
#include "core_process_manager.h"

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{

	BOOL bTranslated = FALSE;
	///Ctrl + G
	if (WM_KEYDOWN == pMsg->message && 0x47 == pMsg->wParam && (pMsg->lParam &0x00FF) <= 1)
	{
		if(::GetKeyState(VK_CONTROL)&0x8000)
		{
			PostMessage(WM_SHOW_OPERATION_PANEL,0,0);
			bTranslated = TRUE;
		}
	}

	if (FALSE == bTranslated)
	{
		bTranslated = CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
	}
	return bTranslated;
}

BOOL CMainFrame::OnIdle()
{
	return FALSE;
}

LRESULT CMainFrame::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	CRect rectClient;
	GetClientRect(&rectClient);
	CRect rectScreen = rectClient;
	ClientToScreen(&rectScreen);
	if (m_wndMaskWindow.IsWindow() && m_wndMaskWindow.IsWindowVisible())
	{
		m_wndMaskWindow.SetWindowPos(HWND_TOP,&rectScreen,SWP_SHOWWINDOW);
	}
	TabPairMap::iterator iter = m_mapTabPairs.begin();
	for (; iter != m_mapTabPairs.end(); ++iter)
	{
		::MoveWindow(iter->second.spContainerWindow->m_hWnd, rectClient.left, rectClient.top + CTabButton::kDefaultHeight, rectClient.Width(), rectClient.Height()-CTabButton::kDefaultHeight,FALSE);
	}
	bHandled = TRUE;
	return 0;
}

LRESULT CMainFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	bHandled = FALSE;
	return 0;

	WINDOWPLACEMENT wp = { 0 };
	wp.length = sizeof(wp);
	wp.flags = WPF_RESTORETOMAXIMIZED;

	if(SC_MAXIMIZE == wParam)
	{
 		CString strLog;
 		strLog.Format(L"mainFrame OnSysCommand MAXSIZED");
 		DEBUG_LINE(strLog);
 
 		bHandled = TRUE;
 		wp.showCmd = SW_MAXIMIZE;
 		SetWindowPlacement(&wp);
	}
	else if(SC_MINIMIZE == wParam)
	{
 		CString strLog;
 		strLog.Format(L"mainFrame OnSysCommand MINIMIZE");
 		DEBUG_LINE(strLog);
 
 		bHandled = TRUE;
 
 		wp.showCmd = SW_SHOWMINIMIZED;
 		SetWindowPlacement(&wp);
	}
	else if(SC_RESTORE == wParam)
	{
		CString strLog;
		strLog.Format(L"mainFrame OnSysCommand RESTORE");
		DEBUG_LINE(strLog);

		wp.showCmd = SW_SHOWNORMAL | SW_RESTORE;
		SetWindowPlacement(&wp);

		bHandled = FALSE;
	}
	else
	{
		bHandled = FALSE;
	}
	
	return S_OK;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{

	RECT rectTabAdd;
	rectTabAdd.top = 0;
	rectTabAdd.left = 0;
	rectTabAdd.right = CTabButtonAdd::kDefaultWidth;
	rectTabAdd.bottom = CTabButtonAdd::kDefaultHeight;

	m_TabButtonAdd.Create(m_hWnd, rectTabAdd, L"", CTabButtonAdd::kStyle, CTabButtonAdd::kExStyle);
	m_TabButtonAdd.SetParentHWND(m_hWnd);

	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	_AddNewTab(ECCF_CreateNew, L"");

	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	GlobalSingleton::GetInstance()->GetCoreProcessManager()->StopAll();

	bHandled = FALSE;
	return 1;
}

LRESULT CMainFrame::OnMove(UINT uMsg, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
{
	CPoint Pt = CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	CRect rectClient;
	GetClientRect(&rectClient);
	ClientToScreen(&rectClient);
	if (m_wndMaskWindow.IsWindow())
	{
		m_wndMaskWindow.SetWindowPos(HWND_TOP,&rectClient,SWP_NOACTIVATE);
	}
	bHandled = TRUE;
	return 0;
}

LRESULT CMainFrame::OnForTest( UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{	
	return 0;
}

HRESULT CMainFrame::_ShowOprationPanel( void )
{

	m_bShowMask = TRUE;
	CRect rectClient;
	GetClientRect(&rectClient);
	if (!m_wndMaskWindow.IsWindow())
	{
		HWND hWndMask = m_wndMaskWindow.Create(m_hWnd,rectClient,_T("MaskWindow")
			,WS_VISIBLE|WS_POPUP,WS_EX_LAYERED);		
	}
	TabPairMap::iterator iter = m_mapTabPairs.find(m_hCurrentTabButton);
	ATLASSERT(iter != m_mapTabPairs.end());		
	m_wndMaskWindow.Ini(iter->second.spCoreProxy);

	ClientToScreen(&rectClient);
	m_wndMaskWindow.ShowMaskWindow(rectClient,NULL);
	SetLayeredWindowAttributes(m_wndMaskWindow.m_hWnd,0,200,LWA_ALPHA);
	return S_OK;
}

LRESULT CMainFrame::OnShowOprationPanel( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	if (m_wndMaskWindow.IsWindow() && m_wndMaskWindow.IsWindowVisible())
	{
		m_wndMaskWindow.ShowWindow(SW_HIDE);
	}
	else
	{
		_ShowOprationPanel();
	}
	return 0;
}

CMainFrame::CMainFrame()
:m_bShowMask(FALSE)
,m_hCurrentTabButton(NULL)
{
	m_strHomeURL.LoadString(IDS_HOME_URL);	
}

CMainFrame::~CMainFrame()
{
	
}

LRESULT CMainFrame::OnChildCreated( UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CoreProxy *pCoreProxy = (CoreProxy*)lParam;
	HWND hContainer = pCoreProxy->GetContainerHWND();
	UINT nFlag = pCoreProxy->GetFlag();
	TabPairMap::iterator iter = m_mapTabPairs.begin();
	for (; iter != m_mapTabPairs.end(); ++iter)
	{
		if (iter->second.spContainerWindow->m_hWnd == hContainer)
		{
			iter->second.spCoreProxy = pCoreProxy;
			_SwitchTo(iter->first);
		}		
	}
	_ShowCurrentContainer();
	if(nFlag == ECCF_CreateNew)
	{
		CString strFirstNavigate = m_strHomeURL;
		//��һ�δ�ʱ���û�������У�����Ĭ�ϵ���ҳ��
		if(m_mapTabPairs.empty())
		{
			//the first
			CommandLine cl;
			cl.ParseFromString(::GetCommandLineW());
			CStringW strUrl = cl.GetSwitchValue(switches::kURL);
			if(FALSE == strUrl.IsEmpty())
			{
				strFirstNavigate = strUrl;
			}
		}
		pCoreProxy->Navigate(strFirstNavigate);
	}
	else if (nFlag == ECCF_NewWindow)
	{
		pCoreProxy->Navigate(L"");
	}
	return 0;
}

LRESULT CMainFrame::OnLButtonDown( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	return 0;
}

LRESULT CMainFrame::OnAddTab( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	_AddNewTab(ECCF_CreateNew, m_strHomeURL);
	return 0;
}

LRESULT CMainFrame::OnClickTab( UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	HWND hWnd = (HWND)wParam;
	if (m_hCurrentTabButton == hWnd)
	{
		return 0;
	}
	TabPairMap::iterator iterOld = m_mapTabPairs.find(m_hCurrentTabButton);
	::ShowWindow(iterOld->second.spContainerWindow->m_hWnd, SW_HIDE);
	_SwitchTo(hWnd);
	return 0;
}

VOID CMainFrame::_AddNewTab(E_CHILEWINDOW_CREATE_FLAG flag , const CString &strUrl )
{
	_CreateContainer(flag, strUrl);
}

void CMainFrame::_CreateContainer(E_CHILEWINDOW_CREATE_FLAG flag,const CString& strURL)
{
	TabPair pair;
	base::CScopedRefPtr<CXWindow> spContainerWindow;
	spContainerWindow.CreateInstance();
	ATLASSERT(spContainerWindow);
	spContainerWindow->Initialize(flag, strURL);

	RECT rect;
	::GetClientRect(m_hWnd, &rect);
	rect.top += CTabButton::kDefaultHeight;
	rect.bottom -= CTabButton::kDefaultHeight;
	HWND hContainedWnd = spContainerWindow->Create(m_hWnd, rect, _T("XWindow"),CXWindow::kNormalStyle	,CXWindow::kNormalExStyle);
	ATLASSERT(hContainedWnd);
	pair.spContainerWindow = spContainerWindow;

	base::CScopedRefPtr<CTabButton> spTabButton;
	spTabButton.CreateInstance();
	rect.left = CTabButton::kDefaultWidth * m_mapTabPairs.size();
	rect.top = 0;
	rect.right = rect.left + CTabButton::kDefaultWidth;
	rect.bottom = rect.top + CTabButton::kDefaultHeight;
	HWND hTabButton = spTabButton->Create(m_hWnd, rect, L"",CTabButton::kStyle, CTabButton::kExStyle);
	spTabButton->SetParentHWND(m_hWnd);
	pair.spTabButton = spTabButton;
	::ShowWindow(hTabButton, SW_SHOW);
	::SetWindowPos(m_TabButtonAdd, HWND_TOP, rect.right, rect.top, CTabButtonAdd::kDefaultWidth, CTabButtonAdd::kDefaultHeight, SWP_NOACTIVATE);
	TabPairMap::iterator iter = m_mapTabPairs.begin();
	for (; iter != m_mapTabPairs.end(); ++iter)
	{
		::ShowWindow(iter->second.spContainerWindow->m_hWnd, SW_HIDE);
	}

	m_mapTabPairs.insert(TabPairPair(hTabButton, pair));
	//CoreProxy did not ready this time. so the tab should not be switched.
	//_SwitchTo(hTabButton);
}

LRESULT CMainFrame::OnCloseTab( UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	HWND hWnd = (HWND)wParam;	
	TabPairMap::iterator iter = m_mapTabPairs.find(hWnd);
	RECT rectClosed;
	::GetWindowRect(hWnd,&rectClosed);

	m_mapTabPairs.erase(iter);

 	iter = m_mapTabPairs.begin();
	for (; iter != m_mapTabPairs.end(); ++iter)
	{
		RECT rect;
		::GetWindowRect(iter->second.spTabButton->m_hWnd,&rect);		
		if (rect.left > rectClosed.left)
		{
			ScreenToClient(&rect);
			::SetWindowPos(iter->second.spTabButton->m_hWnd, HWND_TOP, rect.left-CTabButton::kDefaultWidth, rect.top, CTabButton::kDefaultWidth, CTabButton::kDefaultHeight, SW_SHOWNORMAL);
		}
	}
	
	RECT rectAdd;
	m_TabButtonAdd.GetWindowRect(&rectAdd);
	rectAdd.left -= CTabButton::kDefaultWidth;
	rectAdd.right -= CTabButton::kDefaultWidth;
	ScreenToClient(&rectAdd);
	m_TabButtonAdd.SetWindowPos(HWND_TOP, &rectAdd, SWP_SHOWWINDOW);

	if (m_hCurrentTabButton == hWnd)
	{
		TabPairMap::iterator iterCurrent = m_mapTabPairs.begin();
		if (iterCurrent == m_mapTabPairs.end())
		{
			m_hCurrentTabButton = NULL;
		}
		else
		{
			_SwitchTo(iterCurrent->second.spTabButton->m_hWnd);
		}
	}
	
	return 0;
}

LRESULT CMainFrame::OnNotifyHotKey(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	switch(wParam)
	{
	case HOTKEY_ShowMaskWindow:
		PostMessage(WM_SHOW_OPERATION_PANEL,0,0);
		break;
	case HOTKEY_Refresh:
		{
			TabPairMap::iterator iter = m_mapTabPairs.find(m_hCurrentTabButton);
			ATLASSERT(iter != m_mapTabPairs.end());	
			iter->second.spCoreProxy->Refresh();
		}		
		break;

	}
	return 0;
}

LRESULT CMainFrame::OnTimer( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	DEBUG_LINE(L"�һ��ţ�");
	return 0;
}

LRESULT CMainFrame::OnSetContext( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	return 0;
}

LRESULT CMainFrame::OnEventNotify(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch(wParam)
	{
	case EVENT_NewWindow3:
		{
			BSTR bsURL = (BSTR)lParam;
			_AddNewTab(ECCF_NewWindow, CString(bsURL));
			::SysFreeString(bsURL);
		}
		break;
	default:
		break;
	}
	return 0;
}

void CMainFrame::_ShowCurrentContainer( void )
{
	TabPairMap::iterator iter = m_mapTabPairs.begin();
	for(; iter != m_mapTabPairs.end(); ++iter)
	{
		if (iter->first == m_hCurrentTabButton)
		{
			iter->second.spContainerWindow->ShowWindow(SW_SHOW);
		}
		else
		{
			iter->second.spContainerWindow->ShowWindow(SW_HIDE);
		}
	}
}

void CMainFrame::_SwitchTo( HWND hTab )
{
	TabPairMap::iterator iterNew = m_mapTabPairs.find(hTab);
	if (iterNew != m_mapTabPairs.end()){
		::ShowWindow(iterNew->second.spContainerWindow->m_hWnd, SW_SHOW);
		m_hCurrentTabButton = hTab;
		GlobalSingleton::GetInstance()->SetCurrentCoreProxy(iterNew->second.spCoreProxy);
	}	
}