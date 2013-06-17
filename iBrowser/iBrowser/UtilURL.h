// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef _UTIL_URL_H__
#define _UTIL_URL_H__

#include <atlstr.h>

class URLUtil
{
public:
	static CString GetHost(const CString &strURL);
};

#endif