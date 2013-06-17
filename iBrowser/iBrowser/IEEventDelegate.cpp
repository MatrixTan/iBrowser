// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "stdafx.h"
#include "IEEventDelegate.h"
#include "BrowserThreadManager.h"
#include "UtilDebug.h"
#include <atlstr.h>
#include "UtilIECore.h"
#include "MessageDef.h"
#include "util_common.h"


CIEEventDelegate::CIEEventDelegate()
:m_hCoreViewWindow(NULL)
{

}

CIEEventDelegate::~CIEEventDelegate()
{

}

HRESULT CIEEventDelegate::Advise( IWebBrowser2 * pWebBrowser2 )
{
	HRESULT hr = S_FALSE;
	if (pWebBrowser2)
	{
		hr = DispEventAdvise(pWebBrowser2,&DIID_DWebBrowserEvents2);
		m_spWebBrowser2 = pWebBrowser2;
	}	
	return hr;
}

HRESULT CIEEventDelegate::UnAdvise( IWebBrowser2 * pWebBrowser2 )
{
	HRESULT hr = S_FALSE;
	if (pWebBrowser2 && m_spWebBrowser2)
	{
		hr = DispEventUnadvise(pWebBrowser2,&DIID_DWebBrowserEvents2);
		ATLASSERT(m_spWebBrowser2 == pWebBrowser2);
		m_spWebBrowser2 = NULL;
	}	
	return hr;
}


STDMETHODIMP CIEEventDelegate::Invoke(DISPID dispidMember, REFIID riid,
										 LCID lcid, WORD wFlags, DISPPARAMS * pdispparams, VARIANT * pvarResult,
										 EXCEPINFO * pexcepinfo, UINT* puArgErr)
{
	HRESULT hr= S_FALSE;
	switch (dispidMember)
	{
	case DISPID_DOCUMENTCOMPLETE:
		hr = IDispEventSimpleImpl<0, CIEEventDelegate, &DIID_DWebBrowserEvents2>::Invoke(
			dispidMember, riid, lcid, wFlags, pdispparams, pvarResult,
			pexcepinfo, puArgErr);
		break;
	case DISPID_NAVIGATECOMPLETE2:
		DEBUG_STR(L"Navigate Complete");
		hr = IDispEventSimpleImpl<0, CIEEventDelegate, &DIID_DWebBrowserEvents2>::Invoke(
			dispidMember, riid, lcid, wFlags, pdispparams, pvarResult,
			pexcepinfo, puArgErr);
		break;
	case DISPID_NEWWINDOW3:
		hr = IDispEventSimpleImpl<0, CIEEventDelegate, &DIID_DWebBrowserEvents2>::Invoke(
			dispidMember, riid, lcid, wFlags, pdispparams, pvarResult,
			pexcepinfo, puArgErr);		
		break;
	case DISPID_NAVIGATEERROR:
		hr = IDispEventSimpleImpl<0, CIEEventDelegate, &DIID_DWebBrowserEvents2>::Invoke(
			dispidMember, riid, lcid, wFlags, pdispparams, pvarResult,
			pexcepinfo, puArgErr);		
		break;
	case DISPID_WINDOWSETLEFT:
		hr = IDispEventSimpleImpl<0, CIEEventDelegate, &DIID_DWebBrowserEvents2>::Invoke(
			dispidMember, riid, lcid, wFlags, pdispparams, pvarResult,
			pexcepinfo, puArgErr);		
		break;
	case DISPID_WINDOWSETWIDTH:
		hr = IDispEventSimpleImpl<0, CIEEventDelegate, &DIID_DWebBrowserEvents2>::Invoke(
			dispidMember, riid, lcid, wFlags, pdispparams, pvarResult,
			pexcepinfo, puArgErr);		
		break;
	case DISPID_WINDOWSETTOP:
		hr = IDispEventSimpleImpl<0, CIEEventDelegate, &DIID_DWebBrowserEvents2>::Invoke(
			dispidMember, riid, lcid, wFlags, pdispparams, pvarResult,
			pexcepinfo, puArgErr);		
		break;
	case DISPID_WINDOWSETHEIGHT:
		hr = IDispEventSimpleImpl<0, CIEEventDelegate, &DIID_DWebBrowserEvents2>::Invoke(
			dispidMember, riid, lcid, wFlags, pdispparams, pvarResult,
			pexcepinfo, puArgErr);		
		break;
	default:
		hr = IDispEventSimpleImpl<0, CIEEventDelegate, &DIID_DWebBrowserEvents2>::Invoke(
			dispidMember, riid, lcid, wFlags, pdispparams, pvarResult,
			pexcepinfo, puArgErr);
		break;
	}
	return hr;
}

void STDMETHODCALLTYPE CIEEventDelegate::OnNewWindow3( IDispatch **ppDisp, VARIANT_BOOL *Cancel, DWORD dwFlags, BSTR bstrUrlContext, BSTR bstrUrl )
{
	DEBUG_STR(L"New Window3");
	*Cancel = VARIANT_TRUE;
	IDispatch *test = *ppDisp;
	*ppDisp = NULL;
		
	CString strURL(bstrUrl);
	::PostMessage(CBrowserThreadManager::GetInstance()->hMainFrame, WM_EVENT_NOTIFY, EVENT_NewWindow3, (LPARAM)strURL.AllocSysString());
	return;
}

void STDMETHODCALLTYPE CIEEventDelegate::OnCommandStateChange( long Command, VARIANT_BOOL Enable )
{
	if (Command != CSC_UPDATECOMMANDS)
	{
		::PostMessage(m_hCoreViewWindow, WM_EVENT_DELEGATE_MESSAGE, EDM_TRAVELLOG_MESSAGE, 0);
	}	
	return;
}

HRESULT CIEEventDelegate::SetCoreViewWindow( HWND hCoreViewWindow )
{
	m_hCoreViewWindow = hCoreViewWindow;
	return S_OK;
}

void STDMETHODCALLTYPE CIEEventDelegate::OnWindowSetLeft( LONG left )
{
	CStringW strLeft;
	strLeft.Format(L"left: %d", left);
	DEBUG_STR(strLeft);
}

void STDMETHODCALLTYPE CIEEventDelegate::OnWindowSetTop( LONG top )
{
	CStringW str;
	str.Format(L"top: %d", top);
	DEBUG_STR(str);
}

void STDMETHODCALLTYPE CIEEventDelegate::OnWindowSetWidth( LONG width )
{
	CStringW str;
	str.Format(L"width: %d", width);
	DEBUG_STR(str);
}

void STDMETHODCALLTYPE CIEEventDelegate::OnWindowSetHeight( LONG height )
{
	CStringW str;
	str.Format(L"height: %d", height);
	DEBUG_STR(str);
}

void STDMETHODCALLTYPE CIEEventDelegate::OnBeforeNavigate2( IDispatch *pDisp
	, VARIANT *url
	, VARIANT *Flags
	, VARIANT *TargetFrameName
	, VARIANT *PostData
	, VARIANT *Headers
	, VARIANT_BOOL *Cancel )
{
	m_ErrorInfo.bError = false;
	m_ErrorInfo.dwErrorCode = 0;
	*Cancel = VARIANT_FALSE;

	if (url){
		CStringW strURL(url->bstrVal);
		BeforeNavigateParam *param = new BeforeNavigateParam();
		param->bMainFrame = UtilIECore::IsMainFrame(pDisp);
		param->strURL = strURL;
		::PostMessage(m_hCoreViewWindow, WM_EVENT_DELEGATE_MESSAGE, EDM_BEFORE_NAVIGATE, (LPARAM)param);
	}

	return;
}

void STDMETHODCALLTYPE CIEEventDelegate::OnNavigateError( IDispatch *pDisp
	, VARIANT *URL
	, VARIANT *TargetFrameName
	, VARIANT *StatusCode
	, VARIANT_BOOL *Cancel )
{
	if (pDisp == NULL || URL == NULL || StatusCode == NULL){
		return;
	}
	m_ErrorInfo.bError = true;
	m_ErrorInfo.dwErrorCode = StatusCode->lVal;
	return;
}

void STDMETHODCALLTYPE CIEEventDelegate::OnNavigateComplete2( IDispatch *pDisp, VARIANT *url )
{
	bool bIsMainFrame = UtilIECore::IsMainFrame(pDisp);
	if (bIsMainFrame){
		CComQIPtr<IWebBrowser2> spWebBrowser2 = pDisp;
		CComBSTR bsURL;
		spWebBrowser2->get_LocationURL(&bsURL);
		if (m_ErrorInfo.bError && UtilIECore::IsIEErrorPage(spWebBrowser2)){
			CString strErrorPage;
			strErrorPage.Format(L"%s?url=%s&reason=%d", Util::GetCustomErrorPage(), CString(bsURL), m_ErrorInfo.dwErrorCode);
			spWebBrowser2->Stop();
			spWebBrowser2->Navigate(strErrorPage.AllocSysString(), NULL, NULL, NULL, NULL);
			return;
		}
	}

	if (url){
		CString strURL(url->bstrVal);
		NavigateCompleteParam *pParam = new NavigateCompleteParam();
		pParam->bMainFrame = bIsMainFrame;
		pParam->bsURL = strURL.AllocSysString();

		::PostMessage(m_hCoreViewWindow, WM_EVENT_DELEGATE_MESSAGE, EDM_DOCUMENT_COMPLETE, (LPARAM)pParam);
	}
	


	return;
}

void STDMETHODCALLTYPE CIEEventDelegate::OnDocumentComplete( IDispatch *pDisp, VARIANT *URL )
{
	DEBUG_STR(L"Document Complete");
	if (UtilIECore::IsMainFrame(pDisp)){
		CComQIPtr<IWebBrowser2> spWebBrowser2 = pDisp;
		CString strURL;
		if (URL){
			strURL = URL->bstrVal;
		}
		if (Util::IsCustomErrorPage(strURL)){
			UtilIECore::RemoveRelativeTravelLog(spWebBrowser2, -1);
		}

		DocumentCompleteParam *pParam = new DocumentCompleteParam();
		pParam->bsURL = ::SysAllocString(URL->bstrVal);
		if (spWebBrowser2)
		{
			pParam->bMainFrame = TRUE;
		}

		::PostMessage(m_hCoreViewWindow, WM_EVENT_DELEGATE_MESSAGE, EDM_DOCUMENT_COMPLETE, (LPARAM)pParam);
	}	
}