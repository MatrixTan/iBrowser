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
	if ((NULL != pguidCmdGroup) && InlineIsEqualGUID(*pguidCmdGroup, CGID_DocHostCommandHandler))
	{
		switch(nCmdID)
		{
		case OLECMDID_SHOWSCRIPTERROR:
			{
				// More Information: http://support.microsoft.com/kb/261003
				(*pvaOut).vt = VT_BOOL;
				(*pvaOut).boolVal = VARIANT_TRUE;
				return S_OK;
			}
		default:
			break;
		}
	}

	if(OLECMDID_ONUNLOAD == nCmdID)
	{
		(*pvaOut).vt = VT_BOOL;
		(*pvaOut).boolVal = VARIANT_TRUE;
		return S_OK;
	}

	return OLECMDERR_E_NOTSUPPORTED;
}

CustomClientSite::CustomClientSite()
	:m_hAxWindow(NULL)
	,m_nRefCount(0)
{
	::CoInitialize(NULL);
}

CustomClientSite::~CustomClientSite()
{

}


STDMETHODIMP CustomClientSite::QueryService( REFGUID rsid, REFIID riid, void** ppvObj )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::SaveObject( void )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::GetMoniker( DWORD dwAssign, DWORD dwWhichMoniker, IMoniker **ppmk )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::GetContainer( IOleContainer **ppContainer )
{
	return E_NOINTERFACE;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::ShowObject( void )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::OnShowWindow( BOOL fShow )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::RequestNewObjectLayout( void )
{
	return E_NOTIMPL;
}


///IOleInPlaceSite
HRESULT STDMETHODCALLTYPE CustomClientSite::CanInPlaceActivate( void )
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::OnInPlaceActivate( void )
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::OnUIActivate( void )
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::GetWindowContext(IOleInPlaceFrame **ppFrame
	,IOleInPlaceUIWindow **ppDoc
	, LPRECT lprcPosRect
	, LPRECT lprcClipRect
	, LPOLEINPLACEFRAMEINFO lpFrameInfo )
{
	lpFrameInfo->fMDIApp = FALSE;
	lpFrameInfo->hwndFrame = m_hAxWindow;
	lpFrameInfo->haccel = 0;
	lpFrameInfo->cAccelEntries = 0;
	*ppDoc = NULL;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::Scroll(SIZE scrollExtant )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::OnUIDeactivate(BOOL fUndoable )
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::OnInPlaceDeactivate( void )
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::DiscardUndoState( void )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::DeactivateAndUndo( void )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::OnPosRectChange( LPCRECT lprcPosRect )
{
	return S_FALSE;
}

///IOleWindow
HRESULT STDMETHODCALLTYPE CustomClientSite::GetWindow( HWND *phwnd )
{
	*phwnd = m_hAxWindow;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::ContextSensitiveHelp(BOOL fEnterMode )
{
	return E_NOTIMPL;
}

/*

STDMETHODIMP CustomClientSite::QueryService( REFGUID rsid, REFIID riid, void** ppvObj )
{
	return CAxHostWindow::QueryService(rsid, riid, ppvObj);
}

HRESULT STDMETHODCALLTYPE CustomClientSite::SaveObject( void )
{
	return CAxHostWindow::SaveObject();
}

HRESULT STDMETHODCALLTYPE CustomClientSite::GetMoniker( DWORD dwAssign, DWORD dwWhichMoniker, IMoniker **ppmk )
{
	return CAxHostWindow::GetMoniker(dwAssign, dwWhichMoniker, ppmk);
}

HRESULT STDMETHODCALLTYPE CustomClientSite::GetContainer( IOleContainer **ppContainer )
{
	return CAxHostWindow::GetContainer(ppContainer);
}

HRESULT STDMETHODCALLTYPE CustomClientSite::ShowObject( void )
{
	return CAxHostWindow::ShowObject();
}

HRESULT STDMETHODCALLTYPE CustomClientSite::OnShowWindow( BOOL fShow )
{
	return CAxHostWindow::ShowWindow(fShow);
}

HRESULT STDMETHODCALLTYPE CustomClientSite::RequestNewObjectLayout( void )
{
	return CAxHostWindow::RequestNewObjectLayout();
}


///IOleInPlaceSite
HRESULT STDMETHODCALLTYPE CustomClientSite::CanInPlaceActivate( void )
{
	return CAxHostWindow::CanInPlaceActivate();
}

HRESULT STDMETHODCALLTYPE CustomClientSite::OnInPlaceActivate( void )
{
	return CAxHostWindow::OnInPlaceActivate();
}

HRESULT STDMETHODCALLTYPE CustomClientSite::OnUIActivate( void )
{
	return CAxHostWindow::OnUIActivate();
}

HRESULT STDMETHODCALLTYPE CustomClientSite::GetWindowContext(IOleInPlaceFrame **ppFrame
	,IOleInPlaceUIWindow **ppDoc
	, LPRECT lprcPosRect
	, LPRECT lprcClipRect
	, LPOLEINPLACEFRAMEINFO lpFrameInfo )
{
	return CAxHostWindow::GetWindowContext(ppFrame, ppDoc, lprcPosRect, lprcClipRect, lpFrameInfo);
}

HRESULT STDMETHODCALLTYPE CustomClientSite::Scroll(SIZE scrollExtant )
{
	return CAxHostWindow::Scroll(scrollExtant);
}

HRESULT STDMETHODCALLTYPE CustomClientSite::OnUIDeactivate(BOOL fUndoable )
{
	return CAxHostWindow::OnUIDeactivate(fUndoable);
}

HRESULT STDMETHODCALLTYPE CustomClientSite::OnInPlaceDeactivate( void )
{
	return CAxHostWindow::OnInPlaceDeactivate();
}

HRESULT STDMETHODCALLTYPE CustomClientSite::DiscardUndoState( void )
{
	return CAxHostWindow::DiscardUndoState();
}

HRESULT STDMETHODCALLTYPE CustomClientSite::DeactivateAndUndo( void )
{
	return CAxHostWindow::DeactivateAndUndo();
}

HRESULT STDMETHODCALLTYPE CustomClientSite::OnPosRectChange( LPCRECT lprcPosRect )
{
	return CAxHostWindow::OnPosRectChange(lprcPosRect);
}

///IOleWindow
HRESULT STDMETHODCALLTYPE CustomClientSite::GetWindow( HWND *phwnd )
{
	return CAxHostWindow::GetWindow(phwnd);
}

HRESULT STDMETHODCALLTYPE CustomClientSite::ContextSensitiveHelp(BOOL fEnterMode )
{
	return CAxHostWindow::ContextSensitiveHelp(fEnterMode);
}
*/

void CustomClientSite::Init( HWND hWnd )
{
	m_hAxWindow = hWnd;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::QueryInterface( REFIID riid , void __RPC_FAR *__RPC_FAR *ppvObject )
{
	if (riid == IID_IUnknown) 
	{ 
		AddRef() ;
		*ppvObject = (void*)(this); 
		return S_OK; 
	} 
	else if (riid == IID_IDispatch)
	{
		AddRef() ;
		*ppvObject = (IDispatch*)(this); 
		return S_OK; 
	}
	else if (riid == IID_IOleClientSite)
	{
		AddRef() ;
		*ppvObject = (IOleClientSite*)(this); 
		return S_OK;
	}
	else if (riid == IID_IOleCommandTarget) 
	{ 
		AddRef() ;
		*ppvObject = (IOleCommandTarget*)(this); 
		return S_OK; 
	}
	else if (riid == IID_IDocHostUIHandler) 
	{ 
		AddRef() ;
		*ppvObject = (IDocHostUIHandler*)(this); 
		return S_OK;
	}
	else if (riid == IID_IOleInPlaceSite)
	{
		AddRef() ;
		*ppvObject = (IOleInPlaceSite*)(this); 
		return S_OK;
	}
	else if (riid == IID_IServiceProvider)
	{
		AddRef();
		*ppvObject = (IServiceProvider*)(this); 
		return S_OK;
	}

	*ppvObject = NULL ;
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE CustomClientSite::AddRef()
{
	return ::InterlockedIncrement(&m_nRefCount);
}

ULONG STDMETHODCALLTYPE CustomClientSite::Release()
{
	LONG nCount = ::InterlockedDecrement(&m_nRefCount);
	if (nCount <= 0){
		delete this;
	}
	return nCount;
}


///IDocHostUIHandler
HRESULT STDMETHODCALLTYPE CustomClientSite::ShowContextMenu( DWORD dwID, 
	POINT *ppt, 
	IUnknown *pcmdtReserved, 
	IDispatch *pdispReserved )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::GetHostInfo( /* [out][in] */ DOCHOSTUIINFO *pInfo )
{
	pInfo->cbSize = sizeof(DOCHOSTUIINFO);
	pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;
	pInfo->dwFlags |= DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_THEME | DOCHOSTUIFLAG_ENABLE_FORMS_AUTOCOMPLETE;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::ShowUI( /* [in] */ DWORD dwID
	, /* [in] */ IOleInPlaceActiveObject *pActiveObject
	, /* [in] */ IOleCommandTarget *pCommandTarget
	, /* [in] */ IOleInPlaceFrame *pFrame
	, /* [in] */ IOleInPlaceUIWindow *pDoc )
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::HideUI( void )
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::UpdateUI( void )
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::EnableModeless( /* [in] */ BOOL fEnable )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::OnDocWindowActivate( /* [in] */ BOOL fActivate )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::OnFrameWindowActivate( /* [in] */ BOOL fActivate )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::ResizeBorder( /* [in] */ LPCRECT prcBorder
	, /* [in] */ IOleInPlaceUIWindow *pUIWindow
	, /* [in] */ BOOL fRameWindow )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::TranslateAccelerator( /* [in] */ LPMSG lpMsg
	, /* [in] */ const GUID *pguidCmdGroup
	, /* [in] */ DWORD nCmdID )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::GetOptionKeyPath( /* [annotation][out] */ __out LPOLESTR *pchKey
	, /* [in] */ DWORD dw )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::GetDropTarget( /* [in] */ IDropTarget *pDropTarget
	, /* [out] */ IDropTarget **ppDropTarget )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::GetExternal( /* [out] */ IDispatch **ppDispatch )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::TranslateUrl( /* [in] */ DWORD dwTranslate
	, /* [annotation][in] */ __in __nullterminated OLECHAR *pchURLIn
	, /* [annotation][out] */ __out OLECHAR **ppchURLOut )
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::FilterDataObject( /* [in] */ IDataObject *pDO
	, /* [out] */ IDataObject **ppDORet )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::GetTypeInfoCount( UINT *pctinfo )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::GetTypeInfo(UINT iTInfo
	,LCID lcid
	, ITypeInfo **ppTInfo )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::GetIDsOfNames( REFIID riid
	, LPOLESTR *rgszNames
	, UINT cNames
	, LCID lcid
	, DISPID *rgDispId )
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CustomClientSite::Invoke( DISPID dispIdMember
	, REFIID riid
	, LCID lcid
	, WORD wFlags
	, DISPPARAMS *pDispParams
	, VARIANT *pVarResult
	, EXCEPINFO *pExcepInfo
	, UINT *puArgErr )
{
	return E_NOTIMPL;
}
