// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _CUSTOM_CLIENT_SITE_H__
#define _CUSTOM_CLIENT_SITE_H__

#include <atlhost.h>

class CustomClientSite
	:public IUnknown
	,public IDocHostShowUI
	//,public CAxHostWindow	
	,public IOleCommandTarget
	,public IOleClientSite
	,public IOleInPlaceSite
	,public IServiceProvider
	,public IDocHostUIHandler
	,public IDispatch
{
public:
	CustomClientSite();
	~CustomClientSite();
	/*
	BEGIN_COM_MAP(CustomClientSite)
		COM_INTERFACE_ENTRY(IDocHostShowUI)
		COM_INTERFACE_ENTRY(IOleCommandTarget)
		COM_INTERFACE_ENTRY(IOleClientSite)
		COM_INTERFACE_ENTRY(IServiceProvider)
		COM_INTERFACE_ENTRY(IOleInPlaceSite)
		//COM_INTERFACE_ENTRY_CHAIN(CAxHostWindow)
	END_COM_MAP()*/

	void Init(HWND hWnd);

	///IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid
		, void __RPC_FAR *__RPC_FAR *ppvObject) ;
	ULONG STDMETHODCALLTYPE AddRef() ;
	ULONG STDMETHODCALLTYPE Release() ;

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


	///IServiceProvider
	STDMETHOD(QueryService)(REFGUID rsid, REFIID riid, void** ppvObj);

	///IOleClientSite
	HRESULT STDMETHODCALLTYPE SaveObject(void) ;
	HRESULT STDMETHODCALLTYPE GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker **ppmk) ;
	HRESULT STDMETHODCALLTYPE GetContainer(IOleContainer **ppContainer) ;
	HRESULT STDMETHODCALLTYPE ShowObject(void) ;
	HRESULT STDMETHODCALLTYPE OnShowWindow(BOOL fShow) ;
	HRESULT STDMETHODCALLTYPE RequestNewObjectLayout(void) ;

	///IOleWindow
	virtual /* [input_sync] */ HRESULT STDMETHODCALLTYPE GetWindow( 
		/* [out] */ __RPC__deref_out_opt HWND *phwnd);

	virtual HRESULT STDMETHODCALLTYPE ContextSensitiveHelp( 
		/* [in] */ BOOL fEnterMode);

	
	///IOleInPlaceSite
	virtual HRESULT STDMETHODCALLTYPE CanInPlaceActivate( void);

	virtual HRESULT STDMETHODCALLTYPE OnInPlaceActivate( void);

	virtual HRESULT STDMETHODCALLTYPE OnUIActivate( void);

	virtual HRESULT STDMETHODCALLTYPE GetWindowContext( 
		/* [out] */ __RPC__deref_out_opt IOleInPlaceFrame **ppFrame,
		/* [out] */ __RPC__deref_out_opt IOleInPlaceUIWindow **ppDoc,
		/* [out] */ __RPC__out LPRECT lprcPosRect,
		/* [out] */ __RPC__out LPRECT lprcClipRect,
		/* [out][in] */ __RPC__inout LPOLEINPLACEFRAMEINFO lpFrameInfo);

	virtual HRESULT STDMETHODCALLTYPE Scroll( 
		/* [in] */ SIZE scrollExtant);

	virtual HRESULT STDMETHODCALLTYPE OnUIDeactivate( 
		/* [in] */ BOOL fUndoable);

	virtual HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate( void);

	virtual HRESULT STDMETHODCALLTYPE DiscardUndoState( void);

	virtual HRESULT STDMETHODCALLTYPE DeactivateAndUndo( void);

	virtual HRESULT STDMETHODCALLTYPE OnPosRectChange( 
		/* [in] */ __RPC__in LPCRECT lprcPosRect);

	///IDocHostUIHandler
	virtual HRESULT STDMETHODCALLTYPE ShowContextMenu( 
		/* [in] */ DWORD dwID,
		/* [in] */ POINT *ppt,
		/* [in] */ IUnknown *pcmdtReserved,
		/* [in] */ IDispatch *pdispReserved);

	virtual HRESULT STDMETHODCALLTYPE GetHostInfo( 
		/* [out][in] */ DOCHOSTUIINFO *pInfo);

	virtual HRESULT STDMETHODCALLTYPE ShowUI( 
		/* [in] */ DWORD dwID,
		/* [in] */ IOleInPlaceActiveObject *pActiveObject,
		/* [in] */ IOleCommandTarget *pCommandTarget,
		/* [in] */ IOleInPlaceFrame *pFrame,
		/* [in] */ IOleInPlaceUIWindow *pDoc);

	virtual HRESULT STDMETHODCALLTYPE HideUI( void);

	virtual HRESULT STDMETHODCALLTYPE UpdateUI( void);

	virtual HRESULT STDMETHODCALLTYPE EnableModeless( 
		/* [in] */ BOOL fEnable);

	virtual HRESULT STDMETHODCALLTYPE OnDocWindowActivate( 
		/* [in] */ BOOL fActivate);

	virtual HRESULT STDMETHODCALLTYPE OnFrameWindowActivate( 
		/* [in] */ BOOL fActivate);

	virtual HRESULT STDMETHODCALLTYPE ResizeBorder( 
		/* [in] */ LPCRECT prcBorder,
		/* [in] */ IOleInPlaceUIWindow *pUIWindow,
		/* [in] */ BOOL fRameWindow);

	virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator( 
		/* [in] */ LPMSG lpMsg,
		/* [in] */ const GUID *pguidCmdGroup,
		/* [in] */ DWORD nCmdID);

	virtual HRESULT STDMETHODCALLTYPE GetOptionKeyPath( 
		/* [annotation][out] */ 
		__out  LPOLESTR *pchKey,
		/* [in] */ DWORD dw);

	virtual HRESULT STDMETHODCALLTYPE GetDropTarget( 
		/* [in] */ IDropTarget *pDropTarget,
		/* [out] */ IDropTarget **ppDropTarget);

	virtual HRESULT STDMETHODCALLTYPE GetExternal( 
		/* [out] */ IDispatch **ppDispatch);

	virtual HRESULT STDMETHODCALLTYPE TranslateUrl( 
		/* [in] */ DWORD dwTranslate,
		/* [annotation][in] */ 
		__in __nullterminated  OLECHAR *pchURLIn,
		/* [annotation][out] */ 
		__out  OLECHAR **ppchURLOut);

	virtual HRESULT STDMETHODCALLTYPE FilterDataObject( 
		/* [in] */ IDataObject *pDO,
		/* [out] */ IDataObject **ppDORet);

	///IDispatch
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount( 
		/* [out] */ __RPC__out UINT *pctinfo);

	virtual HRESULT STDMETHODCALLTYPE GetTypeInfo( 
		/* [in] */ UINT iTInfo,
		/* [in] */ LCID lcid,
		/* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo);

	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames( 
		/* [in] */ __RPC__in REFIID riid,
		/* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
		/* [range][in] */ __RPC__in_range(0,16384) UINT cNames,
		/* [in] */ LCID lcid,
		/* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);

	virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke( 
		/* [in] */ DISPID dispIdMember,
		/* [in] */ REFIID riid,
		/* [in] */ LCID lcid,
		/* [in] */ WORD wFlags,
		/* [out][in] */ DISPPARAMS *pDispParams,
		/* [out] */ VARIANT *pVarResult,
		/* [out] */ EXCEPINFO *pExcepInfo,
		/* [out] */ UINT *puArgErr);

protected:
private:

	HWND m_hAxWindow;
	LONG m_nRefCount;
};


#endif //_CUSTOM_CLIENT_SITE_H__