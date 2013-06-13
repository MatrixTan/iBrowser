// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "custom_client_site.h"

HRESULT STDMETHODCALLTYPE CustomClientSite::ShowMessage(HWND hwnd,
	LPOLESTR lpstrText,
	LPOLESTR lpstrCaption,
	DWORD dwType,
	LPOLESTR lpstrHelpFile,
	DWORD dwHelpContext,
	LRESULT *plResult )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::ShowHelp(HWND hwnd, 
	LPOLESTR pszHelpFile,
	UINT uCommand,
	DWORD dwData,
	POINT ptMouse,
	IDispatch *pDispatchObjectHit )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::QueryStatus(const GUID *pguidCmdGroup,
	ULONG cCmds,
	OLECMD prgCmds[ ], 
	OLECMDTEXT *pCmdText )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::Exec(
	const GUID *pguidCmdGroup, 
	DWORD nCmdID, 
	DWORD nCmdexecopt, 
	VARIANT *pvaIn, 
	VARIANT *pvaOut )
{
	return E_NOTIMPL;
}

CustomClientSite::CustomClientSite()
{
	::CoInitialize(NULL);
}

CustomClientSite::~CustomClientSite()
{

}

STDMETHODIMP CustomClientSite::QueryService( REFGUID rsid, REFIID riid, void** ppvObj )
{
	return CAxHostWindow::QueryService(rsid, riid, ppvObj);
}
