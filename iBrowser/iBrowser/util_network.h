// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _UTIL_NETWORK_H__
#define _UTIL_NETWORK_H__

#include <atlstr.h>
#include <WinInet.h>

namespace Util
{
	bool HttpDownloadToFile(CStringW strURL,CStringW strFilePath,int nTimeout = 10000, bool bCache = true);
}

#endif //_UTIL_NETWORK_H__