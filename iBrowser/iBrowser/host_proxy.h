// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _HOST_PROXY_H__
#define _HOST_PROXY_H__

#include <atlstr.h>
#include "ie_event_message.h"


class HostProxy
{
public:;
	HostProxy(HWND hHostWindow);
	~HostProxy();

	void NotifyBeforeNavigate(const CString& strURL);
	void NotifyTitleChange(const CString& strTitle);
protected:
private:
	HWND m_hHostWindow;
};


#endif //_HOST_PROXY_H__