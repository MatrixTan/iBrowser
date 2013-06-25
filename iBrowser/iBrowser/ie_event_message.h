// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _IE_EVENT_MESSAGE_H__
#define _IE_EVENT_MESSAGE_H__

#include <atlstr.h>

enum EventDelegateMessage
{
	EDM_TRAVELLOG_MESSAGE = 0,
	EDM_BEFORE_NAVIGATE,
	EDM_NAVIGATE_COMPLETE,
	EDM_DOCUMENT_COMPLETE,
	EDM_TITLE_CHANGE
};

class DocumentCompleteParam
{
public:
	DocumentCompleteParam()
		:bMainFrame(FALSE)
	{}
	~DocumentCompleteParam()
	{
		if (bsURL){
			::SysFreeString(bsURL);
			bsURL = NULL;
		}
	}

	BSTR bsURL;
	BOOL bMainFrame;
};

class BeforeNavigateParam
{
public:
	BeforeNavigateParam()
		:bMainFrame(FALSE)
	{}
	~BeforeNavigateParam()
	{}
	BOOL bMainFrame;
	CStringW strURL;
};

//template<>
//class Serialize<BeforeNavigateParam>{
//public:
//	static void Write(const BeforeNavigateParam* param, void** pOut, unsigned* pSize){
//		unsigned urlSize = 0;
//		void *pUrlData = NULL;
//		Serialize<CStringW>::Write(&(param->strURL), &pUrlData, &urlSize);
//
//		BYTE *pData = new BYTE[sizeof(unsigned)*2+urlSize+sizeof(BOOL)];
//		BYTE *pIter = pData;
//		*((unsigned*)pIter) = urlSize;
//		pIter += sizeof(unsigned);
//		
//	}
//	static void Read(void* pData, BeforeNavigateParam **ppOut){
//
//	}
//};

class NavigateCompleteParam
{
public:
	NavigateCompleteParam()
		:bMainFrame(FALSE)
	{}
	~NavigateCompleteParam()
	{}
	BOOL bMainFrame;
	CStringW strURL;
};

#endif //_IE_EVENT_MESSAGE_H__