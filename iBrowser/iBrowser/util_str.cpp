// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "util_str.h"

namespace Util
{
	void Stringify( const CStringW& strIn, CStringW& strOut )
	{
		int nLength = strIn.GetLength();
		for (int i = 0; i < nLength; i++){
			WCHAR chr = strIn.GetAt(i);
			if (chr == L'"' || chr == L'\\' || chr == L'/'){
				strOut += L'\\';
				strOut += chr;
			}else if (chr == L'\b'){
				strOut += L"\\b";
			}else if (chr == L'\f'){
				strOut += L"\\f";
			}else if (chr == L'\n'){
				strOut += L"\\n";
			}else if (chr == L'\r'){
				strOut += L"\\r";
			}else if (chr == L'\t'){
				strOut += L"\\t";
			}else if (chr < L' '){
				strOut += L"\\u";
				for (int i = 0; i < 4; i++){
					int value = (chr >> 12) & 0xf;
					if (value >= 0 && value <= 9)
						strOut += (wchar_t)('0' + value);
					else if (value >= 10 && value <= 15)
						strOut += (wchar_t)('A' + (value - 10));
					chr <<= 4;
				}
			}else{
				strOut += chr;
			}
		}
	}
}