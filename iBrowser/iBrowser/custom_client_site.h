// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _CUSTOM_CLIENT_SITE_H__
#define _CUSTOM_CLIENT_SITE_H__

#include <atlhost.h>

class CustomClientSite
	:public CAxHostWindow
	,public IDocHostShowUI
	,public IOleCommandTarget
{
public:
	CustomClientSite();
	~CustomClientSite();

	BEGIN_COM_MAP(CustomClientSite)
		COM_INTERFACE_ENTRY(IDocHostShowUI)
		COM_INTERFACE_ENTRY(IOleCommandTarget)
		COM_INTERFACE_ENTRY_CHAIN(CAxHostWindow)
	END_COM_MAP()


	//IDocHostShowUI
	virtual HRESULT STDMETHODCALLTYPE ShowMessage( 
		/* [in] */ HWND hwnd,
		/* [annotation][in] */ 
		__in __nullterminated  LPOLESTR lpstrText,
		/* [annotation][in] */ 
		__in __nullterminated  LPOLESTR lpstrCaption,
		/* [in] */ DWORD dwType,
		/* [annotation][in] */ 
		__in __nullterminated  LPOLESTR lpstrHelpFile,
		/* [in] */ DWORD dwHelpContext,
		/* [out] */ LRESULT *plResult);

	virtual HRESULT STDMETHODCALLTYPE ShowHelp( 
		/* [in] */ HWND hwnd,
		/* [annotation][in] */ 
		__in __nullterminated  LPOLESTR pszHelpFile,
		/* [in] */ UINT uCommand,
		/* [in] */ DWORD dwData,
		/* [in] */ POINT ptMouse,
		/* [out] */ IDispatch *pDispatchObjectHit);

	///IOleCommandTarget
	virtual /* [input_sync] */ HRESULT STDMETHODCALLTYPE QueryStatus( 
		/* [unique][in] */ __RPC__in_opt const GUID *pguidCmdGroup,
		/* [in] */ ULONG cCmds,
		/* [out][in][size_is] */ __RPC__inout_ecount_full(cCmds) OLECMD prgCmds[  ],
		/* [unique][out][in] */ __RPC__inout_opt OLECMDTEXT *pCmdText);

	virtual HRESULT STDMETHODCALLTYPE Exec( 
		/* [unique][in] */ __RPC__in_opt const GUID *pguidCmdGroup,
		/* [in] */ DWORD nCmdID,
		/* [in] */ DWORD nCmdexecopt,
		/* [unique][in] */ __RPC__in_opt VARIANT *pvaIn,
		/* [unique][out][in] */ __RPC__inout_opt VARIANT *pvaOut);

	STDMETHOD(QueryService)(REFGUID rsid, REFIID riid, void** ppvObj);


protected:
private:
};


#endif //_CUSTOM_CLIENT_SITE_H__