// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _PROFILE_H__
#define _PROFILE_H__

#include <string>

class Profile
{
public:

	Profile();
	~Profile();

	bool GetMainFrameRect(RECT *pRect , bool* pIsMax);
	bool SetMainFrameProfile(RECT rect , bool bIsMax);

protected:
private:

	bool SetValue(const char *db, const char* key, const char* value);
	bool GetValue(const char *db, const char* key, std::string& strValue);
};


#endif //_PROFILE_H__