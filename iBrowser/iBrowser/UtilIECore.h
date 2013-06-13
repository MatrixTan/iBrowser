// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _IBROWSER_UTIL_IE_CORE_H__
#define _IBROWSER_UTIL_IE_CORE_H__

#include <atlbase.h>
#include <atlcom.h>
#include <ExDispid.h>
#include <atlstr.h>
#include <vector>
#include <tlogstg.h>

class UtilIECore
{
public:
	class IETravelLogItem
	{
	public:
		CStringW Url;
		CStringW Title;
		LONGLONG SerialId;
	};
	static BOOL GetTravelLogInfo(IWebBrowser2* pWebBrowser2, std::vector<IETravelLogItem> &travelLogList, BOOL &bHaveNew);
	static BOOL GetAllImagesFromDocument(IHTMLDocument2* pDocument, std::vector<CComPtr<IHTMLImgElement> >& imageList);
	static bool IsMainFrame(IDispatch* pDisp);
	static bool IsIEErrorPage(IWebBrowser2* pWebBrowser2);
	static bool RemoveRelativeTravelLog(IWebBrowser2* pWebBrowser2, int nIndex);
private:
	static void GetTravelLog(IWebBrowser2* pWebBrowser2, ITravelLogStg** ppTravelLog);
};

#endif