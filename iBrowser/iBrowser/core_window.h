// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef _CORE_WINDOW_H__
#define _CORE_WINDOW_H__

class CoreView;
class CoreWindow : public CWindowImpl<CoreWindow, CAxWindow>
{
public:
	CoreWindow(CoreView* pHost);
	~CoreWindow();

	void Init(HWND hCore);

	BEGIN_MSG_MAP(CoreWindow)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()

	LRESULT OnPaint(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandler);
protected:
private:
	CoreView* m_pHost;
};

#endif //_CORE_WINDOW_H__