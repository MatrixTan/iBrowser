// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef _I_IE_EVENT_DELEGATE_H__
#define _I_IE_EVENT_DELEGATE_H__

#include "ie_event_message.h"

EXTERN_C const IID IID_IIEEventDelegate;



class IIEEventDelegate : public IUnknown
{
public:	
	virtual HRESULT Advise(IWebBrowser2 * pIWebBrowser2) = 0;
	virtual HRESULT UnAdvise(IWebBrowser2 * pIWebBrowser2) = 0;
	virtual HRESULT SetCoreViewWindow(HWND hCoreViewWindow) = 0;
};

#endif //_I_IE_EVENT_DELEGATE_H__
