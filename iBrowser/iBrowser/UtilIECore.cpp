// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "UtilIECore.h"

BOOL UtilIECore::GetTravelLogInfo( IWebBrowser2* pWebBrowser2, std::vector<IETravelLogItem> &travelLogList , BOOL &bHaveNew)
{
	static LONGLONG s_nTravelLogSerailId = 0;
	CComPtr<IWebBrowser2> spWebBrowser2 = pWebBrowser2;
	bHaveNew = FALSE;
	if (spWebBrowser2)
	{
		CComPtr<IServiceProvider> spServiceProvider;
		spWebBrowser2->QueryInterface(IID_IServiceProvider, (VOID**)&spServiceProvider);
		if (spServiceProvider)
		{
			CComPtr<ITravelLogStg> spTravelLogStg;
			spServiceProvider->QueryService(IID_ITravelLogStg, &spTravelLogStg);

			CComQIPtr<IEnumTravelLogEntry> spEnumTravelLog;
			UINT nFlag = TLENUMF(TLEF_RELATIVE_INCLUDE_CURRENT | TLEF_RELATIVE_BACK | TLEF_RELATIVE_FORE);
			spTravelLogStg->EnumEntries(nFlag, &spEnumTravelLog);
			if (spEnumTravelLog)
			{
				spEnumTravelLog->Reset();
				CComPtr<ITravelLogEntry> spEntry;
				ULONG nFetching = 1;
				ULONG nFetched = 0;
				while (S_OK == spEnumTravelLog->Next(nFetching, &spEntry, &nFetched) && 1 == nFetched)
				{
					nFetched = 0;
					WCHAR * pTitle = NULL;
					WCHAR * pURL = NULL;
					spEntry->GetTitle(&pTitle);
					spEntry->GetURL(&pURL);

					CComPtr<IPropertyBag> pPropertyBag;
					spEntry->QueryInterface(IID_IPropertyBag, (VOID **)&pPropertyBag);

					INT32 nSerialID = 0;
					CComVariant varSerialID;
					HRESULT hr = pPropertyBag->Read(L"SerialId", &varSerialID, NULL);
					if (SUCCEEDED(hr))
					{
						nSerialID = varSerialID.lVal;
					}
					else
					{
						varSerialID.vt = VT_UI8;
						varSerialID.llVal = s_nTravelLogSerailId;
						nSerialID = s_nTravelLogSerailId;
						pPropertyBag->Write(L"SerialId", &varSerialID);
						s_nTravelLogSerailId ++;
						bHaveNew = TRUE;
					}
					IETravelLogItem item;
					item.Title = pTitle;
					item.Url = pURL;
					item.SerialId = nSerialID;
					travelLogList.push_back(item);
					if (pTitle){
						CoTaskMemFree(pTitle);
					}
					if (pURL){
						CoTaskMemFree(pURL);
					}
					spEntry = NULL;
				}
			}			
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE; 
}


//How to get document? 
//CComPtr<IDispatch> spDispatch;
//pWebBrowser2->get_Document(&spDispatch);
//CComQIPtr<IHTMLDocument2> spDocument(spDispatch);
//ATLASSERT(spDocument);
BOOL UtilIECore::GetAllImagesFromDocument( IHTMLDocument2* pDocument, std::vector<CComPtr<IHTMLImgElement> >& imageList )
{
	ATLASSERT(pDocument);
	HRESULT hr;
	CComPtr<IHTMLFramesCollection2> spHTMLFramesCollection2;
	pDocument->get_frames(&spHTMLFramesCollection2);
	if (spHTMLFramesCollection2)
	{
		LONG nLength;
		spHTMLFramesCollection2->get_length(&nLength);
		for(LONG i = 0; i < nLength ;i++)
		{
			CComPtr<IHTMLDocument2> spHTMLDoc2;
			CComVariant varDisp=NULL;
			CComVariant varIndex=i;
			HRESULT atthr =spHTMLFramesCollection2->item(&varIndex, &varDisp);
			if (FAILED(atthr))
			{
				continue;
			}
			CComQIPtr<IHTMLWindow2> spWindow= varDisp.pdispVal;
			if (spWindow)
			{
				spWindow->get_document(&spHTMLDoc2);
				if (spHTMLDoc2)
					GetAllImagesFromDocument(spHTMLDoc2, imageList);
			}
		}
	}

	CComPtr<IHTMLElementCollection> spImgElementCollection;
	hr = pDocument->get_images(&spImgElementCollection);
	if (FAILED(hr) || !spImgElementCollection)
	{
		return FALSE;
	}
	LONG nNumber = 0;
	spImgElementCollection->get_length(&nNumber);
	if (nNumber == 0)
		return FALSE;

	for (LONG i = 0; i < nNumber; ++ i)
	{
		CComVariant varIndex = i;
		CComPtr<IDispatch> pElemDisp = NULL;
		hr = spImgElementCollection->item(varIndex, varIndex, &pElemDisp);
		if (FAILED(hr))
		{
			break;
		}

		CComQIPtr<IHTMLImgElement> spHTMLImgElement(pElemDisp);
		if (!spHTMLImgElement)
			continue;

		imageList.push_back(spHTMLImgElement);
	}
	return TRUE;
}

bool UtilIECore::IsMainFrame( IDispatch* pDisp ){
	CComQIPtr<IWebBrowser2> spBrowser = pDisp;
	if (spBrowser){
		return true;
	}
	return false;
}

bool UtilIECore::IsIEErrorPage( IWebBrowser2* pWebBrowser2 )
{
	CComPtr<IDispatch> spDispatch;
	pWebBrowser2->get_Document(&spDispatch);
	CComQIPtr<IHTMLDocument2> spDocument = spDispatch;
	if (spDocument){
		CComBSTR bsDomURL;
		spDocument->get_URL(&bsDomURL);
		CString strURL(bsDomURL);
		if (strURL.Find(L"res://ieframe.dll") >= 0){
			return true;
		}
	}
	return false;
}

bool UtilIECore::RemoveRelativeTravelLog( IWebBrowser2* pWebBrowser2, int nIndex )
{
	if (!pWebBrowser2)
		return false;
		
	CComPtr<ITravelLogStg> spTravelLog;
	GetTravelLog(pWebBrowser2, &spTravelLog);
	if (spTravelLog){
		CComPtr<ITravelLogEntry> spEntry;
		spTravelLog->GetRelativeEntry(nIndex, &spEntry);
		if (spEntry){
			return spTravelLog->RemoveEntry(spEntry) == S_OK;
		}		
	}	
	return false;
}

void UtilIECore::GetTravelLog( IWebBrowser2* pWebBrowser2, ITravelLogStg** ppTravelLog )
{
	if (!pWebBrowser2 || !ppTravelLog)
		return;
	
	CComQIPtr<IServiceProvider> spServiceProvider = pWebBrowser2;
	if (spServiceProvider){
		CComPtr<ITravelLogStg> spTravelLog;
		spServiceProvider->QueryService(IID_ITravelLogStg, &spTravelLog);
		if (spTravelLog){
			*ppTravelLog = spTravelLog.Detach();
		}		
	}
}

