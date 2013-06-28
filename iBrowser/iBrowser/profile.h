// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _PROFILE_H__
#define _PROFILE_H__

class Profile
{
public:

	Profile();
	~Profile();

	bool GetMainFrameRect(RECT *pRect);
	bool SetMainFrameRect(RECT rect);

protected:
private:
};


#endif //_PROFILE_H__