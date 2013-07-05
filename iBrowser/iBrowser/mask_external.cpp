// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "mask_external.h"
#include "gamemode/accelerator.h"
#include "global_singleton.h"
#include "MessageDef.h"
#include "core_proxy.h"
#include "bookmark_manager.h"
#include "BrowserThreadManager.h"

MaskExternal::MaskExternal()
{
	m_mapIDs.resize(ID_Count);
	m_mapIDs[ID_getAccelerator] = L"getAccelerator";
	m_mapIDs[ID_setAccelerator] = L"setAccelerator";
	m_mapIDs[ID_mainEditSubmit] = L"mainEditSubmit";
	m_mapIDs[ID_getMaskAlpha] = L"getMaskAlpha";
	m_mapIDs[ID_setMaskAlpha] = L"setMaskAlpha";
	m_mapIDs[ID_goBack] = L"goBack";
	m_mapIDs[ID_goForward] = L"goForward";
	m_mapIDs[ID_getCurrentURL] = L"getCurrentURL";
	m_mapIDs[ID_addCurrentBookmark] = L"addCurrentBookmark";
	m_mapIDs[ID_getBookmarks] = L"getBookmarks";
	m_mapIDs[ID_newWindow] = L"newWindow";
	m_mapIDs[ID_hideMask] = L"hideMask";
}

MaskExternal::~MaskExternal()
{

}

HRESULT STDMETHODCALLTYPE MaskExternal::GetTypeInfoCount(UINT *pctinfo ){
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MaskExternal::GetTypeInfo( UINT iTInfo, 
	 LCID lcid, 
	 ITypeInfo **ppTInfo ){
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MaskExternal::GetIDsOfNames( 
	REFIID riid,
	LPOLESTR *rgszNames,
	UINT cNames,
	LCID lcid,
	DISPID *rgDispId)
{    
	while (cNames){
		bool bMatch = false;
		for (int i=ID_Begin+1; i<ID_Count; ++i){
			if (wcscmp(*rgszNames, m_mapIDs[i].c_str())==0){
				*rgDispId = i;
				bMatch = true;
			}
		}
		if(!bMatch){
			return DISP_E_UNKNOWNNAME;
		}
		rgDispId++;
		rgszNames++;
		cNames--;
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE MaskExternal::Invoke(DISPID dispIdMember
	, REFIID riid
	, LCID lcid
	, WORD wFlags
	, DISPPARAMS *pDispParams
	, VARIANT *pVarResult
	, EXCEPINFO *pExcepInfo
	, UINT *puArgErr )
{
	if (wFlags & DISPATCH_METHOD){
		if (dispIdMember == ID_getAccelerator){
			pVarResult->vt = VT_R4;
			pVarResult->fltVal = GameMode::Accelerator::GetInstance()->GetRate();
			return S_OK;
		}else if (dispIdMember == ID_setAccelerator){
			float fAcc = pDispParams->rgvarg->dblVal;
			GameMode::Accelerator::GetInstance()->SetRate(fAcc);
			return S_OK;
		}else if (dispIdMember == ID_mainEditSubmit){
			CComBSTR text(pDispParams->rgvarg->bstrVal);
			HWND hMask = GlobalSingleton::GetInstance()->GetMaskHWND();
			::PostMessage(hMask,WM_MAINEDIT_SUBMIT, 0, (LPARAM)text.Detach());
			return S_OK;
		}else if (dispIdMember == ID_getMaskAlpha){
			HWND hMask = GlobalSingleton::GetInstance()->GetMaskHWND();
			BYTE byteAlpha = 0;
			::GetLayeredWindowAttributes(hMask, NULL, &byteAlpha, NULL);
			pVarResult->vt = VT_I4;
			pVarResult->intVal = (int)byteAlpha;
			return S_OK;			
		}else if (dispIdMember == ID_setMaskAlpha){
			HWND hMask = GlobalSingleton::GetInstance()->GetMaskHWND();
			BYTE byteAlpha = (BYTE)(pDispParams->rgvarg->intVal);
			::SetLayeredWindowAttributes(hMask, NULL, byteAlpha, LWA_ALPHA);
			return S_OK;
		}else if (dispIdMember == ID_goBack){
			CoreProxy* pProxy = GlobalSingleton::GetInstance()->GetCurrentCoreProxy();
			if (pProxy){
				pProxy->GoBack();
			}
			return S_OK;
		}else if (dispIdMember == ID_goForward){
			CoreProxy* pProxy = GlobalSingleton::GetInstance()->GetCurrentCoreProxy();
			if (pProxy){
				pProxy->GoForward();
			}
			return S_OK;
		}else if (dispIdMember == ID_getCurrentURL){
			CoreProxy* pProxy = GlobalSingleton::GetInstance()->GetCurrentCoreProxy();
			if (pProxy){
				CStringW strURL;
				pProxy->GetURL(strURL);
				ATLASSERT(pVarResult);
				pVarResult->vt = VT_BSTR;
				pVarResult->bstrVal = strURL.AllocSysString();
				return S_OK;
			}
			return E_FAIL;
		}else if (dispIdMember == ID_addCurrentBookmark){
			CoreProxy* pProxy = GlobalSingleton::GetInstance()->GetCurrentCoreProxy();
			if (pProxy){
				bool bRet = pProxy->AddCurrentBookmark();
				CComVariant vRet;
				vRet.vt = VT_BOOL;
				vRet.boolVal = bRet?VARIANT_TRUE:VARIANT_FALSE;
				ATLASSERT(pVarResult);
				*pVarResult = vRet;
				return S_OK;
			}
			return E_FAIL;
		}else if (dispIdMember == ID_getBookmarks){
			CStringW strBookmarkJson;
			GlobalSingleton::GetInstance()->GetBookmarkManager()->GetBookmarksJson(strBookmarkJson);
			ATLASSERT(pVarResult);
			pVarResult->vt = VT_BSTR;
			pVarResult->bstrVal = strBookmarkJson.AllocSysString();
			return S_OK;
		}else if (dispIdMember == ID_newWindow){
			ATLASSERT(pDispParams);
			BSTR bsURL = ::SysAllocString(pDispParams->rgvarg->bstrVal);
			HWND hMainFrame = CBrowserThreadManager::GetInstance()->hMainFrame;
			::PostMessage(hMainFrame, WM_EVENT_NOTIFY, EVENT_NewWindow3, (LPARAM)bsURL);
			return S_OK;
		}else if (dispIdMember == ID_hideMask){
			HWND hMainFrame = CBrowserThreadManager::GetInstance()->hMainFrame;
			::PostMessage(hMainFrame, WM_SHOW_OPERATION_PANEL, 0, 0);
			return S_OK;
		}
	}
    return E_NOTIMPL;
}
