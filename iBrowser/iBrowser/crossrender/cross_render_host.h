// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef _CROSS_RENDER_HOST_H__
#define _CROSS_RENDER_HOST_H__

#include <atlwin.h>

namespace CrossRender
{

class CrossRenderHost :public CWindowImpl<CrossRenderHost, CAxWindow>
{
public:

	BEGIN_MSG_MAP(CrossRenderHost)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)	
	END_MSG_MAP()

	static const DWORD kStyle = WS_CHILDWINDOW|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN;
	static const DWORD kExStyle = WS_EX_LEFT|WS_EX_LTRREADING|WS_EX_RIGHTSCROLLBAR|WS_EX_TOOLWINDOW|WS_EX_NOPARENTNOTIFY;


protected:
private:

	LRESULT OnCreate(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};


}

#endif //_CROSS_RENDER_HOST_H__