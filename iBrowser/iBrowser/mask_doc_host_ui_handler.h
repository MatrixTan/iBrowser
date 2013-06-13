// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _MASK_DOC_HOST_UI_HANDLER_H__
#define _MASK_DOC_HOST_UI_HANDLER_H__

class MaskDocHostUIHandler
	:public IDispatchImpl<IDocHostUIHandlerDispatch>
	,public CComObjectRootEx<CComSingleThreadModel>
{
public:
	MaskDocHostUIHandler();
	virtual ~MaskDocHostUIHandler();

	DECLARE_NO_REGISTRY()

	BEGIN_COM_MAP(MaskDocHostUIHandler)
		COM_INTERFACE_ENTRY(IDocHostUIHandlerDispatch)
	END_COM_MAP()

	virtual HRESULT STDMETHODCALLTYPE FilterDataObject( IUnknown *pDO, IUnknown **ppDORet );

	virtual HRESULT STDMETHODCALLTYPE EnableModeless( VARIANT_BOOL fEnable );

	virtual HRESULT STDMETHODCALLTYPE HideUI( void );

	virtual HRESULT STDMETHODCALLTYPE ShowUI( DWORD dwID, IUnknown *pActiveObject, IUnknown *pCommandTarget, IUnknown *pFrame, IUnknown *pDoc, HRESULT *dwRetVal );

	virtual HRESULT STDMETHODCALLTYPE OnDocWindowActivate( VARIANT_BOOL fActivate );

	virtual HRESULT STDMETHODCALLTYPE UpdateUI( void );

	virtual HRESULT STDMETHODCALLTYPE OnFrameWindowActivate( VARIANT_BOOL fActivate );

	virtual HRESULT STDMETHODCALLTYPE ShowContextMenu( DWORD dwID, DWORD x, DWORD y, IUnknown *pcmdtReserved, IDispatch *pdispReserved, HRESULT *dwRetVal );

	virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator( DWORD_PTR hWnd, DWORD nMessage, DWORD_PTR wParam, DWORD_PTR lParam, BSTR bstrGuidCmdGroup, DWORD nCmdID, HRESULT *dwRetVal );

	virtual HRESULT STDMETHODCALLTYPE GetHostInfo( DWORD *pdwFlags, DWORD *pdwDoubleClick );

	virtual HRESULT STDMETHODCALLTYPE GetOptionKeyPath( BSTR *pbstrKey, DWORD dw );

	virtual HRESULT STDMETHODCALLTYPE TranslateUrl( DWORD dwTranslate, BSTR bstrURLIn, BSTR *pbstrURLOut );

	virtual HRESULT STDMETHODCALLTYPE GetExternal( IDispatch **ppDispatch );

	virtual HRESULT STDMETHODCALLTYPE ResizeBorder( long left, long top, long right, long bottom, IUnknown *pUIWindow, VARIANT_BOOL fFrameWindow );

	virtual HRESULT STDMETHODCALLTYPE GetDropTarget( IUnknown *pDropTarget, IUnknown **ppDropTarget );

protected:
private:
	CComPtr<IDispatch> m_spExternal;
};

#endif  //_MASK_DOC_HOST_UI_HANDLER_H__