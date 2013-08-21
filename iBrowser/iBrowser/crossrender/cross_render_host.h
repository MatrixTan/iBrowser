// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef _CROSS_RENDER_HOST_H__
#define _CROSS_RENDER_HOST_H__

#include <atlwin.h>

#define HANDLE_MSG_FOR_CORE \
	bHandled = TRUE; \
	bHandled = PostToCoreForCPR(uMsg, wParam, lParam);\
	if(bHandled) \
	return TRUE;\

namespace CrossRender
{

class CrossRenderHost :public CWindowImpl<CrossRenderHost, CAxWindow>
{
public:

	BEGIN_MSG_MAP(CrossRenderHost)
		//HANDLE_MSG_FOR_CORE
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_WINDOWPOSCHANGED, OnPosChanged)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBK)
	END_MSG_MAP()
	
	static const DWORD kStyle = WS_CHILDWINDOW|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
	static const DWORD kExStyle = WS_EX_LEFT|WS_EX_LTRREADING|WS_EX_RIGHTSCROLLBAR|WS_EX_TOOLWINDOW|WS_EX_NOPARENTNOTIFY;
	
	CrossRenderHost();
	~CrossRenderHost();

	void SetCoreWindow(HWND hCore);
protected:
private:
	BOOL PostToCoreForCPR(UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT OnCreate(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPosChanged(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEraseBK(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	HWND m_hCoreWindow;
};


}

#endif //_CROSS_RENDER_HOST_H__