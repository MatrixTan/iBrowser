// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _MASK_EXTERNAL_H__
#define _MASK_EXTERNAL_H__

#include <vector>
#include <string>

class MaskExternal : public IDispatch
	,public CComObjectRootEx<CComSingleThreadModel>
{
public:
	MaskExternal();
	virtual ~MaskExternal();

	BEGIN_COM_MAP(MaskExternal)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

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
	enum ExternalIds{
		ID_Begin = 0,
		ID_getAccelerator,
		ID_setAccelerator,
		ID_mainEditSubmit,
		ID_getMaskAlpha,
		ID_setMaskAlpha,
		ID_goBack,
		ID_goForward,
		ID_getCurrentURL,
		ID_addCurrentBookmark,
		///add items before this
		ID_Count,
	};
	
	std::vector<std::wstring> m_mapIDs;
};

#endif  //_MASK_EXTERNAL_H__