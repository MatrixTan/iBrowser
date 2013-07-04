// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _CORE_PROXY_H__
#define _CORE_PROXY_H__

#include <Windows.h>
#include <atlstr.h>
#include <Base\RefCounted.h>

class CXWindow;
class CoreProxy : public base::RefCounted<CoreProxy>
{
public:
	explicit CoreProxy(HWND hCoreView, CXWindow* pContainer, UINT flag);
	~CoreProxy();

	HWND GetCoreHWND(void);
	HWND GetContainerHWND(void);
	UINT GetFlag(void);

	void Refresh(void);
	void Navigate(const CString& strURL);
	void GoBack();
	void GoForward();
	void Focus();
	void Destroy();
	void GetURL(CStringW& strURL);

protected:
private:
	CXWindow *m_pContainer;
	HWND m_hCoreView;
	UINT m_nFlag;
};

#endif  //_CORE_PROXY_H__