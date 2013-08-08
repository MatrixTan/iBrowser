// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _PROFILE_H__
#define _PROFILE_H__

#include <string>
#include <atlstr.h>
#include "storage.h"
#include "version.h"

class Profile
	:public Storage
{
public:

	Profile();
	~Profile();

	bool GetMainFrameRect(RECT *pRect , bool* pIsMax);
	bool SetMainFrameProfile(RECT rect , bool bIsMax);
	bool GetHomeURL(CStringW &strURL);
	bool GetVersion(iVersion& version);
	bool SetVersion(const iVersion& strVersion);
	CStringW GetInstallPath(void);
	bool SetInstallPath(const CStringW& strPath);
	bool SetInstallTempPath(const CStringW& strTemp);
	bool GetInstallTempPath(CStringW& strTempPath);
	void DeleteInstallTempPath(void);
};


#endif //_PROFILE_H__