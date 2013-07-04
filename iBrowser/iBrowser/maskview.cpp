// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "maskview.h"
#include "mask_doc_host_ui_handler.h"
#include "util_common.h"
#include <wininet.h>

#ifdef _DEBUG
const WCHAR* kMaskURL = L"http://127.0.0.1/wantgame/1/ibrowser/mask.html";
//const WCHAR* kMaskURL = L"http://wantgame.sinaapp.com/ibrowser/mask.html";
#else
const WCHAR* kMaskURL = L"http://wantgame.sinaapp.com/ibrowser/mask.html";
#endif

LRESULT MaskView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);
	return 0;
}

LRESULT MaskView::OnCreate( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	this->DefWindowProc();
	HRESULT hr = QueryControl(&m_spWebBrowser2);
	m_spWebBrowser2.p->AddRef();

	CComObject<MaskDocHostUIHandler> *pObjUIHandler = NULL;
	hr = CComObject<MaskDocHostUIHandler>::CreateInstance(&pObjUIHandler);
	if (SUCCEEDED(hr) && pObjUIHandler){
		CComPtr<IDocHostUIHandlerDispatch> spUIHandler;
		hr = pObjUIHandler->QueryInterface(&spUIHandler);
		if (SUCCEEDED(hr) && spUIHandler){
			SetExternalUIHandler(spUIHandler);
		}
	}
	CString strMaskURL;
	
	BOOL bConnect = ::InternetCheckConnection(kMaskURL, FLAG_ICC_FORCE_CONNECTION, 0);
	if (bConnect){
		strMaskURL = kMaskURL;
	}else{
		CString strModule = Util::GetCurrentModuleFileName();
		strMaskURL = L"res://"+strModule+"/mask.html";
	}

	hr = m_spWebBrowser2->Navigate(strMaskURL.AllocSysString(), NULL, NULL, NULL, NULL);
	SetActiveWindow();
	return 0;
} 

MaskView::MaskView(){	
}

MaskView::~MaskView(){
}


LRESULT MaskView::OnDestory( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled ){
	return 0;
}


LRESULT MaskView::OnSize( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ ){
	return 0;
}

LRESULT MaskView::OnLButtonUp( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ ){
	
	return 0;
}

LRESULT MaskView::OnTimer( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	return 0;
}

BOOL MaskView::PreTranslateMessage( MSG* pMsg )
{
	BOOL bTranslated = FALSE;
	if(pMsg->hwnd == m_hWnd || IsChild(pMsg->hwnd))
		bTranslated = SendMessage(WM_FORWARDMSG, 0, (LPARAM)pMsg);

	return bTranslated;
}

void MaskView::OnShow( void )
{
	CComPtr<IDispatch> spDispatch;
	m_spWebBrowser2->get_Document(&spDispatch);
	CComQIPtr<IHTMLDocument2> spDocument = spDispatch;
	if (spDocument){
		spDispatch = NULL;
		spDocument->get_Script(&spDispatch);
		DISPID id = 0;
		CComBSTR bsFun(L"onEventShow");
		HRESULT hr = spDispatch->GetIDsOfNames(IID_NULL, &bsFun, 1, LOCALE_SYSTEM_DEFAULT, &id);
		if (SUCCEEDED(hr)){
			DISPPARAMS param = {0};
			CComVariant vRet;
			EXCEPINFO exInfo;
			UINT nErr;
			spDispatch->Invoke(id, IID_NULL, 0, DISPATCH_METHOD, &param, &vRet, &exInfo, &nErr);
		}
	}
}
