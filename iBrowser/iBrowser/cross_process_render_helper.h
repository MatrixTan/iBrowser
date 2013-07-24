// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _CROSS_PROCESS_RENDER_HELPER_H__
#define _CROSS_PROCESS_RENDER_HELPER_H__

#include "Base/t_thread_singleton.h"
#include "api_hook.h"

class CrossProcessRenderHelper 
	: public TThreadSingleton<CrossProcessRenderHelper, ETTS_CrossProcessRenderHelper>
{
public:
	void Initialize(HWND hHost);
	BOOL CustomBitBlt(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop, PFuncBitBlt pfBitBlt);
protected:
private:
	HWND m_hHost;
};


#endif //_CROSS_PROCESS_RENDER_HELPER_H__