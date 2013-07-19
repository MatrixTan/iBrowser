// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "IEEventDelegate.h"
#include "core_view.h"
#include "resource.h"
#include <tlogstg.h>
#include <atlimage.h>
#include <atlstr.h>
#include "mouse_gesture.h"
#include "UtilIECore.h"
#include "custom_client_site.h"
#include "ui_util.h"
#include "BrowserThreadManager.h"


LRESULT CoreView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	CPaintDC dc(m_hWnd);
	return 0;
}

LRESULT CoreView::OnCreate( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	AtlAxWinInit();

	CString strWindowName;
	GetWindowText(strWindowName);
	_CreateCoreServer();

	//CComObject<CustomClientSite> *pCustomSite;
	//CComObject<CustomClientSite>::CreateInstance(&pCustomSite);
	//if (pCustomSite){
	//	pCustomSite->Init(m_hWnd);
	//	CComPtr<IUnknown> spCustomSite;
	//	pCustomSite->QueryInterface(&spCustomSite);

	//	CComQIPtr<IAxWinHostWindowLic> spHostWindow = spCustomSite;
	//	if (spHostWindow){
	//		CComPtr<IUnknown> spControl;
	//		spHostWindow->CreateControlLicEx(CComBSTR(strWindowName), m_hWnd, NULL, &spControl, IID_NULL, NULL, NULL);

	//		IAxWinHostWindowLic * pAxWindow;
	//		spHostWindow->QueryInterface(&pAxWindow);
	//		::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (DWORD_PTR)pAxWindow);
	//	}
	//}	

	//HRESULT hr = QueryControl(&m_spWebBrowser2);
	HRESULT hr = E_FAIL;
	if (m_spWebBrowser2){
		m_spWebBrowser2.p->AddRef();

		CComObject<CIEEventDelegate> *objIEEventDelegate; 
		CComObject<CIEEventDelegate>::CreateInstance(&objIEEventDelegate);
		hr = objIEEventDelegate->QueryInterface(IID_IIEEventDelegate,(void**)&m_spIEEventDelegate);
		if (SUCCEEDED(hr) && m_spIEEventDelegate)
		{
			m_spIEEventDelegate->Advise(m_spWebBrowser2);
			m_spIEEventDelegate->SetCoreViewWindow(m_hWnd);
		}
		SetExternalUIHandler(NULL);

		HWND hParent = ::GetParent(m_hWnd);
		::PostMessage(hParent, WM_CHILD_WINDOW_CREATED,(WPARAM)m_hWnd, 0);
		if (NULL == m_HostProxy){
			m_HostProxy = new HostProxy(hParent);
		}		
	}
	
	return 0;
} 

CoreView::CoreView()
:m_spWebBrowser2(NULL)
,m_MouseGesture(this)
,m_bBeforeGesture(false)
,m_HostProxy(NULL)
{

}

CoreView::~CoreView()
{
	if (m_HostProxy){
		delete m_HostProxy;
		m_HostProxy = NULL;
	}
}

HRESULT CoreView::_Navegate( LPCWSTR strURL )
{
	HRESULT hr = S_FALSE;
	if (m_spWebBrowser2)
	{
		CString strURLTemp = strURL;
		if (strURLTemp.IsEmpty())
		{
			strURLTemp = m_strURL;
		}
		m_spWebBrowser2->Navigate(strURLTemp.AllocSysString(), NULL, NULL, NULL ,NULL);
	}	

	return hr;
}

BOOL CoreView::PreTranslateMessage( MSG* pMsg )
{
	UINT Msg = pMsg->message;
	WPARAM wParam = pMsg->wParam;
	LPARAM lParam = pMsg->lParam;

	BOOL bTranslated = FALSE;	
	if (WM_RBUTTONDOWN == Msg){
		m_bBeforeGesture = true;
	}

	if (WM_MOUSEMOVE == Msg){
		if (pMsg->wParam & MK_RBUTTON && m_bBeforeGesture){
			m_MouseGesture.Start();
		}
		m_bBeforeGesture = false;		
	}

	if (WM_RBUTTONUP == Msg){
		if (m_MouseGesture.GetStarted()){
			m_MouseGesture.Stop();
		}		
		m_bBeforeGesture = false;
	}

	///Ctrl + G
	if (WM_KEYDOWN == Msg && 0x47 == wParam && (lParam &0x00FF) <= 1)
	{
		if(::GetKeyState(VK_CONTROL)&0x8000)
		{
			NotifyHotKey(WM_HOTKEY_NOTIFY, HOTKEY_ShowMaskWindow, 0);
		}
	}
	///Ctrl + V
	if (WM_KEYDOWN == Msg && 0x56 == wParam && (lParam &0x00FF) <= 1)
	{
		if(::GetKeyState(VK_CONTROL)&0x8000)
		{
			//NotifyHotKey(WM_HOTKEY_NOTIFY, HOTKEY_ShowMaskWindow, 0);
		}
	}

	///Ctrl + 1
	if (WM_KEYDOWN == Msg && 0x31 == wParam && (lParam &0x00FF) <= 1)
	{
		if(::GetKeyState(VK_CONTROL)&0x8000)
		{
			NotifyHotKey(WM_HOTKEY_NOTIFY, HOTKEY_ShowMaskWindow, 0);
		}
	}
	///F5
	if (WM_KEYDOWN == Msg && VK_F5 == wParam && (lParam &0x00FF) <= 1)
	{
		NotifyHotKey(WM_HOTKEY_NOTIFY, HOTKEY_Refresh, 0);
	}
	///F12
	if (WM_KEYDOWN == Msg && VK_F12 == wParam && (lParam &0x00FF) <= 1)
	{
		::PostMessage(m_hWnd, WM_HOTKEY_NOTIFY, 0, 0);
	}
	///Ctrl + <-
	if (WM_KEYDOWN == Msg && VK_LEFT == wParam && (lParam &0x00FF) <= 1)
	{
		if(::GetKeyState(VK_CONTROL)&0x8000)
		{
			::PostMessage(m_hWnd, WM_HOTKEY_NOTIFY, 0, 0);
		}
	}
	///Ctrl + ->
	if (WM_KEYDOWN ==Msg && VK_RIGHT == wParam && (lParam &0x00FF) <= 1)
	{
		if(::GetKeyState(VK_CONTROL)&0x8000)
		{
			::PostMessage(m_hWnd, WM_HOTKEY_NOTIFY, 0, 0);
		}
	}

	if(pMsg->hwnd == m_hWnd || IsChild(pMsg->hwnd))
		bTranslated = SendMessage(WM_FORWARDMSG, 0, (LPARAM)pMsg);

	return bTranslated;	
}

LRESULT CoreView::OnDestory( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	if (m_spIEEventDelegate && m_spWebBrowser2)
	{
		m_spIEEventDelegate->UnAdvise(m_spWebBrowser2);
		m_spWebBrowser2 = NULL;
		m_spIEEventDelegate = NULL;
	}
	m_hWnd = 0;
	if (m_MouseGesture.IsWindow()){
		m_MouseGesture.DestroyWindow();
	}
	PostQuitMessage(0);
	return 0;
}


LRESULT CoreView::OnSize( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	CRect rectClient;
	GetClientRect(&rectClient);
	ClientToScreen(&rectClient);
	if (m_wndConsole.IsWindow())
	{
		m_wndConsole.SetWindowPos(HWND_TOP,&rectClient,SWP_NOACTIVATE);
	}
	return 0;
}
LRESULT CoreView::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CPoint Pt = CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	CRect rectClient;
	GetClientRect(&rectClient);
	ClientToScreen(&rectClient);
	if (m_wndConsole.IsWindow())
	{
		m_wndConsole.SetWindowPos(HWND_TOP,&rectClient,SWP_NOACTIVATE);
	}
	return 0;
}

LRESULT CoreView::OnShowConsole( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	CRect rectClient;
	GetClientRect(&rectClient);
	ClientToScreen(&rectClient);

	if (!m_wndConsole.IsWindow())
	{
		HWND hConsole = m_wndConsole.Create(m_hWnd,rectClient,_T("ConsoleWindow")
			,WS_VISIBLE|WS_POPUP,WS_EX_TOPMOST|WS_EX_LAYERED);
		SetLayeredWindowAttributes(m_wndConsole.m_hWnd,0,240,LWA_ALPHA);
		m_wndConsole.SetWindowPos(HWND_TOP,&rectClient,SWP_NOACTIVATE);
	}
	if (!m_wndConsole.IsWindowVisible())
	{
		m_wndConsole.ShowWindow(SW_SHOW);
	}
	else
	{
		m_wndConsole.ShowWindow(SW_HIDE);
	}
	
	return 0;
}

LRESULT CoreView::OnEventDelegateMessage( UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/ )
{
	switch(wParam)
	{
	case EDM_TRAVELLOG_MESSAGE:
		{
			std::vector<UtilIECore::IETravelLogItem> vecTravelLog;
			BOOL bHaveNew = FALSE;
			UtilIECore::GetTravelLogInfo(m_spWebBrowser2,vecTravelLog,bHaveNew);
			//if (bHaveNew)
			{
				std::vector<UtilIECore::IETravelLogItem>::const_iterator iter = vecTravelLog.begin();
				CStringW strLog = L"--------------URL-----------------\n";
				for(;iter!= vecTravelLog.end(); ++iter)
				{
					strLog.AppendFormat(L"URL:%s | Title:%s | SerialId:%d \n", iter->Url, iter->Title, iter->SerialId);
				}
				strLog.Append(L"---------------------------\n");
				m_wndConsole.Log(strLog);
				::OutputDebugString(strLog);
			}
		}		
		break;
	case EDM_DOCUMENT_COMPLETE:
		break;
	case EDM_NAVIGATE_COMPLETE:
		{
			NavigateCompleteParam *pParam = static_cast<NavigateCompleteParam*>((void*)lParam);
			if (pParam){
				if (pParam->bMainFrame && m_HostProxy){
					m_HostProxy->NotifyNavigateComplete(pParam->strURL);
				}
				delete pParam;
				pParam = NULL;
			}
		}
		break;
	case EDM_BEFORE_NAVIGATE:
		{
			BeforeNavigateParam *pParam = static_cast<BeforeNavigateParam*>((void*)lParam);
			if (pParam){
				if (pParam->bMainFrame && m_HostProxy){
					m_HostProxy->NotifyBeforeNavigate(pParam->strURL);
				}
				delete pParam;
				pParam = NULL;
			}			
		}
		break;
	case EDM_TITLE_CHANGE:
		{
			BSTR bsTitle = (BSTR)lParam;
			m_HostProxy->NotifyTitleChange(CStringW(bsTitle));
			::SysFreeString(bsTitle);
		}
		break;
	default:
		break;
	}
	return 0;
}

LRESULT CoreView::OnForTest( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	//CComPtr<IServiceProvider> spServiceProvider;
	//m_spWebBrowser2->QueryInterface(IID_IServiceProvider, (VOID**)&spServiceProvider);

	//CComPtr<ITravelLogStg> spTravelLogStg;
	//spServiceProvider->QueryService(IID_ITravelLogStg, &spTravelLogStg);\

	//CComPtr<ITravelLogEntry> spEntry;
	//spTravelLogStg->GetRelativeEntry(1,&spEntry);

	//HRESULT hr = spTravelLogStg->TravelTo(spEntry);
	//CComVariant vURL;
	//vURL.vt = VT_BSTR;
	//vURL.bstrVal = ::SysAllocString(L"www.wantgame.net");
	//m_spWebBrowser2->Navigate2(&vURL,NULL,NULL,NULL,NULL);
	return 0;
}

BOOL CoreView::OnIdle()
{
	return FALSE;
}

void CoreView::NotifyHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hParent = GetParent();
	HWND hMainFrame = ::GetParent(hParent);
	::PostMessage(hMainFrame, uMsg, wParam, lParam);
}

void CoreView::Initialize(const CString& strURL )
{
	m_strURL = strURL;
}

HRESULT CoreView::_Refresh( void ){
	CComQIPtr<IWebBrowser> spWebBrowser = m_spWebBrowser2;
	if (spWebBrowser){
		spWebBrowser->Refresh();
	}
	return S_OK;
}

HRESULT CoreView::_GoBack( void ){
	if (m_spWebBrowser2){
		m_spWebBrowser2->GoBack();
	}
	return S_OK;
}

HRESULT CoreView::_GoForward( void )
{
	if (m_spWebBrowser2){
		m_spWebBrowser2->GoForward();
	}
	return S_OK;
}

LRESULT CoreView::OnCoreNavigate( UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/ )
{
	CStringW *pStrURL = NULL;
	Serialize<CStringW>::Read((void*)wParam, &pStrURL);
	_Navegate(pStrURL->GetString());
	delete pStrURL;
	pStrURL = NULL;
	return 0;
}

LRESULT CoreView::OnCoreRefresh( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	_Refresh();
	return 0;
}

LRESULT CoreView::OnGoBack( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	_GoBack();
	return 0;
}

LRESULT CoreView::OnGoForward( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	_GoForward();
	return 0;
}

HRESULT CoreView::_Close( void )
{
	return S_OK;
}

LRESULT CoreView::OnCoreDestroy( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	DestroyWindow();
	return 0;
}

LRESULT CoreView::OnCoreFocus( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	HWND hWnd = GetChildWindow(m_hWnd, L"Internet Explorer_Server");
	if (hWnd){
		::SetFocus(hWnd);
	}	
	return 0;
}

HRESULT CoreView::_ShowMaskWindow( void )
{
	::PostMessage(CBrowserThreadManager::GetInstance()->hMainFrame, WM_SHOW_OPERATION_PANEL, 0, 0);
	return S_OK;
}

HRESULT CoreView::_CreateCoreServer( void )
{
	HRESULT hr = S_OK;
	::OleInitialize(NULL) ;

	CComPtr<IClassFactory> spClassFactory ;
	if (FAILED(::CoGetClassObject(CLSID_WebBrowser
		, CLSCTX_INPROC_SERVER | CLSCTX_INPROC_HANDLER
		, NULL
		, IID_IClassFactory
		, (void**)&spClassFactory)) || 0 == spClassFactory)
		return S_FALSE ;
	
	hr = spClassFactory->CreateInstance(NULL, IID_IOleObject, (void**)&m_spOleObject);
	if (FAILED(hr) || 0 == m_spOleObject)
	{
		ATLTRACE("spClassFactory->CreateInstance hr = %x\r\n", hr);
		return S_FALSE ;
	}

	//CComObject<CustomClientSite> *pCustomSite;
	//CComObject<CustomClientSite>::CreateInstance(&pCustomSite);
	CustomClientSite* pCustomSite = new CustomClientSite();
	pCustomSite->Init(m_hWnd);
	pCustomSite->AddRef();
	CComPtr<IOleClientSite> spCustomSite;
	pCustomSite->QueryInterface(IID_IOleClientSite, (void**)&spCustomSite);
	hr = m_spOleObject->SetClientSite(spCustomSite);
	if (FAILED(hr))
	 	return S_FALSE ;

	if (FAILED(::OleSetContainedObject(m_spOleObject, TRUE)))
		return S_FALSE ;

	RECT rc ;
	GetClientRect(&rc) ;
	hr = m_spOleObject->DoVerb(OLEIVERB_INPLACEACTIVATE, NULL, spCustomSite, -1, m_hWnd, &rc);
	if (FAILED(hr))
		return S_FALSE ;

	CComPtr<IWebBrowser2> spWebBrowser2;
	if (FAILED(m_spOleObject->QueryInterface(IID_IWebBrowser2, (void**)&spWebBrowser2)) || 0 == spWebBrowser2)
		return S_FALSE ;

	spWebBrowser2->put_RegisterAsBrowser(VARIANT_TRUE) ;
	spWebBrowser2->put_RegisterAsDropTarget(VARIANT_TRUE) ;


	if (FAILED(m_spOleObject->QueryInterface(__uuidof(IOleInPlaceObject), (void**)&m_spWindowless)) || 0 == m_spWindowless)
		return S_FALSE ;

	SIZE sz = { rc.right, rc.bottom } ;
	SIZE sz2 ;
	AtlPixelToHiMetric(&sz, &sz2) ;
	m_spOleObject->SetExtent(DVASPECT_CONTENT, &sz2) ;
	m_spWindowless->SetObjectRects(&rc, &rc) ;
	m_spWebBrowser2 = spWebBrowser2 ;
	return S_OK;
}
