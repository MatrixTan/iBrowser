// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _CROSS_RENDER_CORE_CONTAINER_H__
#define _CROSS_RENDER_CORE_CONTAINER_H__

#include <atlwin.h>

namespace CrossRender
{

class CrossRenderCoreContainer
	: public CWindowImpl<CrossRenderCoreContainer, CAxWindow>
{
public:

	BEGIN_MSG_MAP(CrossRenderCoreContainer)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()

	CrossRenderCoreContainer();
	~CrossRenderCoreContainer();

	static const int kStyle = WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_POPUP;
	static const int kExStyle = WS_EX_LAYERED;

protected:
private:
	LRESULT OnCreate(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};

}

#endif //_CROSS_RENDER_CORE_CONTAINER_H__