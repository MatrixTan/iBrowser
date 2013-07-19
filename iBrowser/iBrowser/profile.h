// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _PROFILE_H__
#define _PROFILE_H__

#include <string>
#include <atlstr.h>
#include "storage.h"

class Profile
	:public Storage
{
public:

	Profile();
	~Profile();

	bool GetMainFrameRect(RECT *pRect , bool* pIsMax);
	bool SetMainFrameProfile(RECT rect , bool bIsMax);
	bool GetHomeURL(CStringW &strURL);
};


#endif //_PROFILE_H__