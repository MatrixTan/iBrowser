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

	/// ������ת��Ϊ�Ϸ���url. 
	/// @param text ����
	/// @param desired_tld �����Ϊ��,�������û�ϣ���ں���Զ���ȫ���.
	///						��:com net ע�ⲻҪ����'.'
	/// ���û��scheme ���Զ�����'www'
	/// ���|desired_tld| is non-empty, it represents the TLD the user wishes to
	CStringW FixupURL(CStringW text, CStringW desired_tld = L"");
	INT GetRegistryLength(CStringW host);
	CStringW ReplaceDBC(CStringW text);
#endif

}

#endif