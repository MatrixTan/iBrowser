// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


///@file IEEventDelegate.h
///@param 接管IE产生的Event
///@author hongsong.tan@gmail.com
///@date 2011.12.28'
#pragma once

#include <atlbase.h>
#include <atlcom.h>
#include <ExDispid.h>

#include "IIEEventDelegate.h"

EXTERN_C const CLSID CLSID_IEEventDelegate;

class ATL_NO_VTABLE CIEEventDelegate:
public CComObjectRootEx<CComSingleThreadModel>,
public CComCoClass<CIEEventDelegate,&CLSID_IEEventDelegate>,
public IIEEventDelegate,
public IDispEventImpl<0, CIEEventDelegate, &DIID_DWebBrowserEvents2,
&LIBID_SHDocVw, 1, 1>
{
public:
	BEGIN_COM_MAP(CIEEventDelegate)
		COM_INTERFACE_ENTRY_IID(IID_IIEEventDelegate,IIEEventDelegate)
	END_COM_MAP()

	BEGIN_SINK_MAP(CIEEventDelegate)
		SINK_ENTRY_EX(0, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE,
		OnDocumentComplete)
		SINK_ENTRY_EX(0, DIID_DWebBrowserEvents2, DISPID_BEFORENAVIGATE2,
		OnBeforeNavigate2)
		SINK_ENTRY_EX(0, DIID_DWebBrowserEvents2, DISPID_NAVIGATEERROR,
		OnNavigateError)
		SINK_ENTRY_EX(0, DIID_DWebBrowserEvents2, DISPID_NEWWINDOW3,
		OnNewWindow3)
		SINK_ENTRY_EX(0, DIID_DWebBrowserEvents2, DISPID_COMMANDSTATECHANGE,
		OnCommandStateChange)
		SINK_ENTRY_EX(0, DIID_DWebBrowserEvents2, DISPID_WINDOWSETLEFT,
		OnWindowSetLeft)
		SINK_ENTRY_EX(0, DIID_DWebBrowserEvents2, DISPID_WINDOWSETWIDTH,
		OnWindowSetWidth)
		SINK_ENTRY_EX(0, DIID_DWebBrowserEvents2, DISPID_WINDOWSETTOP,
		OnWindowSetTop)
		SINK_ENTRY_EX(0, DIID_DWebBrowserEvents2, DISPID_WINDOWSETHEIGHT,
		OnWindowSetHeight)
		SINK_ENTRY_EX(0, DIID_DWebBrowserEvents2, DISPID_NAVIGATECOMPLETE2,
		OnNavigateComplete2)
		SINK_ENTRY_EX(0, DIID_DWebBrowserEvents2, DISPID_TITLECHANGE,
		OnTitleChange)
	END_SINK_MAP()
	
	CIEEventDelegate();
	~CIEEventDelegate();
	virtual HRESULT Advise(IWebBrowser2 * pWebBrowser2);
	virtual HRESULT UnAdvise(IWebBrowser2 * pWebBrowser2);
	virtual HRESULT SetCoreViewWindow(HWND hCoreViewWindow);
	STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid,
		LCID lcid, WORD wFlags, DISPPARAMS * pdispparams, VARIANT * pvarResult,
		EXCEPINFO * pexcepinfo, UINT* puArgErr);

	// Fires when a document is completely loaded and initialized.
	void STDMETHODCALLTYPE OnDocumentComplete(IDispatch *pDisp,VARIANT *URL);
	void STDMETHODCALLTYPE OnBeforeNavigate2(IDispatch *pDisp,
		VARIANT *url,
		VARIANT *Flags,
		VARIANT *TargetFrameName,
		VARIANT *PostData,
		VARIANT *Headers,
		VARIANT_BOOL *Cancel
		);
	void STDMETHODCALLTYPE OnNavigateError(IDispatch *pDisp,
		VARIANT *URL,
		VARIANT *TargetFrameName,
		VARIANT *StatusCode,
		VARIANT_BOOL *Cancel
		);
	void STDMETHODCALLTYPE OnNewWindow3(IDispatch **ppDisp,
		VARIANT_BOOL *Cancel,
		DWORD dwFlags,
		BSTR bstrUrlContext,
		BSTR bstrUrl
		);
	void STDMETHODCALLTYPE OnCommandStateChange(long Command,
		VARIANT_BOOL Enable);
	void STDMETHODCALLTYPE OnWindowSetLeft(LONG left);
	void STDMETHODCALLTYPE OnWindowSetWidth(LONG width);
	void STDMETHODCALLTYPE OnWindowSetTop(LONG top);
	void STDMETHODCALLTYPE OnWindowSetHeight(LONG height);
	void STDMETHODCALLTYPE OnNavigateComplete2(IDispatch *pDisp, VARIANT *url);
	void STDMETHODCALLTYPE OnTitleChange(BSTR bsTitle);

private:

	class NavigateErrorInfo{
	public:
		NavigateErrorInfo()
			:dwErrorCode(0)
			,bError(false)
			,bDeleteTravelEntry(false)
		{}
		DWORD dwErrorCode;
		bool bError;
		//redundant travel log entries of error page should be deleted when custom error page loaded.
		//we delete the travel log entry with index -1. but it's should not happen sometime. go back or forward eg.
		//so we need the flag for controlling the delete action.
		bool bDeleteTravelEntry;
	};

	CComPtr<IWebBrowser2> m_spWebBrowser2;
	HWND m_hCoreViewWindow;
	NavigateErrorInfo m_ErrorInfo;

};