// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _CORE_PROXY_H__
#define _CORE_PROXY_H__

#include <Windows.h>
#include <atlstr.h>
#include <Base\RefCounted.h>

class CoreProxy : public base::RefCounted<CoreProxy>
{
public:
	CoreProxy(HWND hCoreView, HWND hContainer, UINT flag);
	~CoreProxy();

	HWND GetCoreHWND(void);
	HWND GetContainerHWND(void);
	UINT GetFlag(void);

	void Refresh(void);
	void Navigate(const CString& strURL);
	void GoBack();
	void GoForward();

	void Destroy();

protected:
private:

	HWND m_hCoreView;
	HWND m_hCortainer;
	UINT m_nFlag;
};

#endif  //_CORE_PROXY_H__