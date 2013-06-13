// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "stdafx.h"
#include "mask_doc_host_ui_handler.h"
#include "mask_external.h"


MaskDocHostUIHandler::MaskDocHostUIHandler()
{

}

MaskDocHostUIHandler::~MaskDocHostUIHandler()
{

}

HRESULT STDMETHODCALLTYPE MaskDocHostUIHandler::FilterDataObject( IUnknown *pDO, IUnknown **ppDORet )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MaskDocHostUIHandler::EnableModeless( VARIANT_BOOL fEnable )
{
	return E_NOTIMPL;

}

HRESULT STDMETHODCALLTYPE MaskDocHostUIHandler::HideUI( void )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MaskDocHostUIHandler::ShowUI( DWORD dwID, IUnknown *pActiveObject, IUnknown *pCommandTarget, IUnknown *pFrame, IUnknown *pDoc, HRESULT *dwRetVal )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MaskDocHostUIHandler::OnDocWindowActivate( VARIANT_BOOL fActivate )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MaskDocHostUIHandler::UpdateUI( void )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MaskDocHostUIHandler::OnFrameWindowActivate( VARIANT_BOOL fActivate )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MaskDocHostUIHandler::ShowContextMenu( DWORD dwID, DWORD x, DWORD y, IUnknown *pcmdtReserved, IDispatch *pdispReserved, HRESULT *dwRetVal )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MaskDocHostUIHandler::TranslateAccelerator( DWORD_PTR hWnd, DWORD nMessage, DWORD_PTR wParam, DWORD_PTR lParam, BSTR bstrGuidCmdGroup, DWORD nCmdID, HRESULT *dwRetVal )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MaskDocHostUIHandler::GetHostInfo( DWORD *pdwFlags, DWORD *pdwDoubleClick )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MaskDocHostUIHandler::GetOptionKeyPath( BSTR *pbstrKey, DWORD dw )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MaskDocHostUIHandler::TranslateUrl( DWORD dwTranslate, BSTR bstrURLIn, BSTR *pbstrURLOut )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MaskDocHostUIHandler::GetExternal( IDispatch **ppDispatch )
{
	if (m_spExternal == NULL){
		CComObject<MaskExternal> *pObjExternal = NULL;
		HRESULT hr = CComObject<MaskExternal>::CreateInstance(&pObjExternal);
		if (SUCCEEDED(hr) && pObjExternal){
			hr = pObjExternal->QueryInterface(&m_spExternal);
		}			
	}
	if (m_spExternal){
		m_spExternal.CopyTo(ppDispatch);
		return S_OK;
	}
	return E_FAIL;
}

HRESULT STDMETHODCALLTYPE MaskDocHostUIHandler::ResizeBorder( long left, long top, long right, long bottom, IUnknown *pUIWindow, VARIANT_BOOL fFrameWindow )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE MaskDocHostUIHandler::GetDropTarget( IUnknown *pDropTarget, IUnknown **ppDropTarget )
{
	return E_NOTIMPL;
}
