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
#include "core_container_manager.h"
#include "profile.h"

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{

	BOOL bTranslated = FALSE;
	//Ctrl + G or Ctrl+1
	if(WM_KEYDOWN == pMsg->message){
		if (::GetKeyState(VK_CONTROL)&0x8000 && (pMsg->lParam &0x00FF) <= 1){
			if (0x47 == pMsg->wParam||0x31 == pMsg->wParam)	{
				PostMessage(WM_SHOW_OPERATION_PANEL,0,0);
				bTranslated = TRUE;
			}
		}
	}

	bTranslated = m_wndMaskWindow.PreTranslateMessage(pMsg);
	
	if(bTranslated && (pMsg->hwnd == m_hWnd || IsChild(pMsg->hwnd)))
		bTranslated = SendMessage(WM_FORWARDMSG, 0, (LPARAM)pMsg);

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
		iter->second.spContainerWindow->MoveWindow(rectClient.left
			, rectClient.top + TabButton::kDefaultHeight
			, rectClient.Width()
			, rectClient.Height()-TabButton::kDefaultHeight
			,FALSE);
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
	
	return 0;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	RECT rect;
	GetWindowRect(&rect);
	LONG lStyle = GetWindowLong(GWL_STYLE);
	GlobalSingleton::GetInstance()->GetProfile()->SetMainFrameProfile(rect, (lStyle&WS_MAXIMIZE)!=0);

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
	if (m_wndMaskWindow.IsWindow()){
		m_wndMaskWindow.SetWindowPos(HWND_TOP,&rectClient,SWP_NOACTIVATE);
	}

	rectClient.top += TabButton::kDefaultHeight;
	TabPairMap::iterator iter = m_mapTabPairs.begin();
	for (; iter != m_mapTabPairs.end(); ++iter){
		iter->second.spContainerWindow->OnFrameMove(rectClient);
	}
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
,m_nTabButtonWidth(0)
{
	m_nTabButtonWidth = TabButton::kDefaultWidth;	
}

CMainFrame::~CMainFrame()
{
	
}

LRESULT CMainFrame::OnChildCreated( UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	CoreProxy *pCoreProxy = (CoreProxy*)lParam;
	HWND hContainer = pCoreProxy->GetContainerHWND();

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
	return 0;
}

LRESULT CMainFrame::OnLButtonDown( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	return 0;
}

LRESULT CMainFrame::OnAddTab( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	CStringW strHomeURL;
	GlobalSingleton::GetInstance()->GetProfile()->GetHomeURL(strHomeURL);
	_AddNewTab(ECCF_CreateNew, strHomeURL);
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
	if (iterOld != m_mapTabPairs.end()){
		iterOld->second.spContainerWindow->ShowWindow(SW_HIDE);
	}	
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
	RECT rect;
	::GetClientRect(m_hWnd, &rect);

	int tabButtonWidth = (rect.right - rect.left - CTabButtonAdd::kDefaultWidth)/(m_mapTabPairs.size()+1);
	if (tabButtonWidth < m_nTabButtonWidth && tabButtonWidth > TabButton::kLeftWidth+TabButton::kRightWidth){
		m_nTabButtonWidth = tabButtonWidth;
		TabButtonIter iter = m_vTabButtons.begin();
		int nCurrentTabLeft = 0;
		for (; iter != m_vTabButtons.end(); ++iter){
			(*iter)->MoveWindow(nCurrentTabLeft, 0, m_nTabButtonWidth, TabButton::kDefaultHeight);
			(*iter)->RedrawWindow();
			nCurrentTabLeft += m_nTabButtonWidth;
		}
	}

	base::CScopedRefPtr<TabButton> spTabButton;
	spTabButton.CreateInstance();
	RECT rectNewTab;
	rectNewTab.left = m_nTabButtonWidth * m_mapTabPairs.size();
	rectNewTab.top = 0;
	rectNewTab.right = rectNewTab.left + m_nTabButtonWidth;
	rectNewTab.bottom = rectNewTab.top + TabButton::kDefaultHeight;
	HWND hTabButton = spTabButton->Create(m_hWnd, rectNewTab, L"",TabButton::kStyle, TabButton::kExStyle);
	spTabButton->SetParentHWND(m_hWnd);
	m_vTabButtons.push_back(spTabButton);
	pair.nTabIndex = m_vTabButtons.size()-1;
	spTabButton->ShowWindow(SW_SHOW);

	RECT containerRect;
	::GetClientRect(m_hWnd, &containerRect);
	containerRect.top += TabButton::kDefaultHeight;
	base::CScopedRefPtr<CXWindow> spContainerWindow = CoreContainerManager::GetInstance()->CreateContainer(m_hWnd, containerRect, flag, strURL, spTabButton.get());
	pair.spContainerWindow = spContainerWindow;

	rectNewTab.top = 0;
	::SetWindowPos(m_TabButtonAdd, HWND_TOP, rectNewTab.right, rectNewTab.top, CTabButtonAdd::kDefaultWidth, CTabButtonAdd::kDefaultHeight, SWP_NOACTIVATE);
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
	if (iter == m_mapTabPairs.end()){
		return -1;
	}

	//don't close the last one!
	if (m_mapTabPairs.size() <= 1){
		return 0;
	}

	int nClosedIndex = iter->second.nTabIndex;
	RECT rectClosed;
	::GetWindowRect(hWnd,&rectClosed);
	CoreContainerManager::GetInstance()->DeleteContainer(iter->second.spContainerWindow.get());
	m_mapTabPairs.erase(iter);
	m_vTabButtons.erase(m_vTabButtons.begin()+nClosedIndex);

	RECT rect;
	GetClientRect(&rect);
	int nTabButtonWidth = (rect.right - rect.left - CTabButtonAdd::kDefaultWidth)/m_vTabButtons.size();
	if (nTabButtonWidth < TabButton::kDefaultWidth){
		m_nTabButtonWidth = nTabButtonWidth;
	}

	for (size_t i = 0; i < m_vTabButtons.size(); ++i){
		m_vTabButtons[i]->SetWindowPos(HWND_TOP, m_nTabButtonWidth*i
			, 0, m_nTabButtonWidth, TabButton::kDefaultHeight
			, SWP_NOACTIVATE|SWP_NOZORDER);
		m_vTabButtons[i]->RedrawWindow();
		TabPairMap::iterator iterCurrentTab = m_mapTabPairs.find(m_vTabButtons[i]->m_hWnd);
		iterCurrentTab->second.nTabIndex = i;
	}

	RECT rectAdd;
	rectAdd.left = m_nTabButtonWidth * m_mapTabPairs.size();
	rectAdd.right = rectAdd.left + m_nTabButtonWidth;
	rectAdd.top = 0;
	rectAdd.bottom = rectAdd.top + CTabButtonAdd::kDefaultHeight;
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
			_SwitchTo(iterCurrent->first);
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
			_AddNewTab(ECCF_NewWindow, CStringW((BSTR)lParam));
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
		iterNew->second.spContainerWindow->ShowWindow(SW_SHOW);
		iterNew->second.spContainerWindow->Focus();
		m_hCurrentTabButton = hTab;
		GlobalSingleton::GetInstance()->SetCurrentCoreProxy(iterNew->second.spCoreProxy);
	}	
}

LRESULT CMainFrame::OnCommand( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	return 0;
}

void CMainFrame::Init()
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

	CommandLine cl;
	cl.ParseFromString(::GetCommandLineW());
	CStringW strUrl = cl.GetSwitchValue(switches::kURL);
	if(TRUE == strUrl.IsEmpty()){
		GlobalSingleton::GetInstance()->GetProfile()->GetHomeURL(strUrl);
	}
	_AddNewTab(ECCF_CreateNew, strUrl);

}

LRESULT CMainFrame::OnCoreNewWindow( UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/ )
{
	CStringW *pStrURL = NULL;
	Serialize<CStringW>::Read((void*)wParam, &pStrURL);
	if (pStrURL){
		_AddNewTab(ECCF_NewWindow, *pStrURL);
		delete pStrURL;
		pStrURL = NULL;
	}
	return 0;
}

LRESULT CMainFrame::OnSetFocus( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	base::CScopedRefPtr<CXWindow> spXWindow = _GetCurrentContainer();
	if (spXWindow.get() && spXWindow->IsWindow()){
		spXWindow->Focus();
	}
	return 0;
}

CXWindow* CMainFrame::_GetCurrentContainer( void )
{
	TabPairMap::iterator iter = m_mapTabPairs.find(m_hCurrentTabButton);
	if (iter != m_mapTabPairs.end()){
		return iter->second.spContainerWindow.get();
	}
	return NULL;
}
