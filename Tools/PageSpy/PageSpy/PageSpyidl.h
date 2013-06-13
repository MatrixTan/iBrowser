

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon Jan 28 23:05:42 2013
 */
/* Compiler settings for .\PageSpy.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __PageSpyidl_h__
#define __PageSpyidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DPageSpy_FWD_DEFINED__
#define ___DPageSpy_FWD_DEFINED__
typedef interface _DPageSpy _DPageSpy;
#endif 	/* ___DPageSpy_FWD_DEFINED__ */


#ifndef ___DPageSpyEvents_FWD_DEFINED__
#define ___DPageSpyEvents_FWD_DEFINED__
typedef interface _DPageSpyEvents _DPageSpyEvents;
#endif 	/* ___DPageSpyEvents_FWD_DEFINED__ */


#ifndef __PageSpy_FWD_DEFINED__
#define __PageSpy_FWD_DEFINED__

#ifdef __cplusplus
typedef class PageSpy PageSpy;
#else
typedef struct PageSpy PageSpy;
#endif /* __cplusplus */

#endif 	/* __PageSpy_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __PageSpyLib_LIBRARY_DEFINED__
#define __PageSpyLib_LIBRARY_DEFINED__

/* library PageSpyLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_PageSpyLib;

#ifndef ___DPageSpy_DISPINTERFACE_DEFINED__
#define ___DPageSpy_DISPINTERFACE_DEFINED__

/* dispinterface _DPageSpy */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DPageSpy;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("70E082FC-1A8B-4575-8CEE-B735A1CA2D97")
    _DPageSpy : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DPageSpyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DPageSpy * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DPageSpy * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DPageSpy * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DPageSpy * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DPageSpy * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DPageSpy * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DPageSpy * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DPageSpyVtbl;

    interface _DPageSpy
    {
        CONST_VTBL struct _DPageSpyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DPageSpy_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DPageSpy_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DPageSpy_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DPageSpy_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DPageSpy_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DPageSpy_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DPageSpy_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DPageSpy_DISPINTERFACE_DEFINED__ */


#ifndef ___DPageSpyEvents_DISPINTERFACE_DEFINED__
#define ___DPageSpyEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DPageSpyEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DPageSpyEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("C347D226-682A-44DF-849E-3E7B9AC1680C")
    _DPageSpyEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DPageSpyEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DPageSpyEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DPageSpyEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DPageSpyEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DPageSpyEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DPageSpyEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DPageSpyEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DPageSpyEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DPageSpyEventsVtbl;

    interface _DPageSpyEvents
    {
        CONST_VTBL struct _DPageSpyEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DPageSpyEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DPageSpyEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DPageSpyEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DPageSpyEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DPageSpyEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DPageSpyEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DPageSpyEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DPageSpyEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_PageSpy;

#ifdef __cplusplus

class DECLSPEC_UUID("043E0E8F-F0AB-4583-A059-563FDBCDC84E")
PageSpy;
#endif
#endif /* __PageSpyLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


