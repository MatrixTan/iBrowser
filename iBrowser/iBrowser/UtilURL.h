// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef _UTIL_URL_H__
#define _UTIL_URL_H__

#include <atlstr.h>

namespace URL
{
	BOOL IsURL(const CStringW &strSource);

#if 0

	/// 把输入转化为合法的url. 
	/// @param text 输入
	/// @param desired_tld 如果不为空,代表着用户希望在后边自动补全后边.
	///						如:com net 注意不要带上'.'
	/// 如果没有scheme 会自动加上'www'
	/// 如果|desired_tld| is non-empty, it represents the TLD the user wishes to
	CStringW FixupURL(CStringW text, CStringW desired_tld = L"");
	INT GetRegistryLength(CStringW host);
	CStringW ReplaceDBC(CStringW text);
#endif

}

#endif