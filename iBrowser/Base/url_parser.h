// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _BASE_URL_PARSER_H__
#define _BASE_URL_PARSER_H__

#include <Windows.h>

namespace Base{

	class ParsedURL {
	public:
		WCHAR *scheme;               /* mandatory */
		WCHAR *host;                 /* mandatory */
		WCHAR *port;                 /* optional */
		WCHAR *path;                 /* optional */
		WCHAR *query;                /* optional */
		WCHAR *fragment;             /* optional */
		WCHAR *username;             /* optional */
		WCHAR *password;             /* optional */
	};

	ParsedURL * ParseUrl(const WCHAR *);
	void FreeParsedURL(ParsedURL *);

}


#endif //_BASE_URL_PARSER_H__