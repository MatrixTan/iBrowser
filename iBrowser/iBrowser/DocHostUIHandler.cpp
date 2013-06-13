// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "DocHostUIHandler.h"

HRESULT STDMETHODCALLTYPE CDocHostUIHandlerImpl::FilterDataObject( IUnknown *pDO, IUnknown **ppDORet )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDocHostUIHandlerImpl::EnableModeless( VARIANT_BOOL fEnable )
{
	return E_NOTIMPL;
	
}

HRESULT STDMETHODCALLTYPE CDocHostUIHandlerImpl::HideUI( void )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDocHostUIHandlerImpl::ShowUI( DWORD dwID, IUnknown *pActiveObject, IUnknown *pCommandTarget, IUnknown *pFrame, IUnknown *pDoc, HRESULT *dwRetVal )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDocHostUIHandlerImpl::OnDocWindowActivate( VARIANT_BOOL fActivate )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDocHostUIHandlerImpl::UpdateUI( void )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDocHostUIHandlerImpl::OnFrameWindowActivate( VARIANT_BOOL fActivate )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDocHostUIHandlerImpl::ShowContextMenu( DWORD dwID, DWORD x, DWORD y, IUnknown *pcmdtReserved, IDispatch *pdispReserved, HRESULT *dwRetVal )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDocHostUIHandlerImpl::TranslateAccelerator( DWORD_PTR hWnd, DWORD nMessage, DWORD_PTR wParam, DWORD_PTR lParam, BSTR bstrGuidCmdGroup, DWORD nCmdID, HRESULT *dwRetVal )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDocHostUIHandlerImpl::GetHostInfo( DWORD *pdwFlags, DWORD *pdwDoubleClick )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDocHostUIHandlerImpl::GetOptionKeyPath( BSTR *pbstrKey, DWORD dw )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDocHostUIHandlerImpl::TranslateUrl( DWORD dwTranslate, BSTR bstrURLIn, BSTR *pbstrURLOut )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDocHostUIHandlerImpl::GetExternal( IDispatch **ppDispatch )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDocHostUIHandlerImpl::ResizeBorder( long left, long top, long right, long bottom, IUnknown *pUIWindow, VARIANT_BOOL fFrameWindow )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDocHostUIHandlerImpl::GetDropTarget( IUnknown *pDropTarget, IUnknown **ppDropTarget )
{
	return E_NOTIMPL;
}
