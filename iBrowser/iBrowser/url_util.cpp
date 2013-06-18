// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "url_util.h"
#include <Shlwapi.h>
#include "Base/url_parser.h"


CString URLUtil::GetHost( const CString &strURL )
{
	Base::ParsedURL *parsedURL = Base::ParseUrl(strURL);
	if (parsedURL){
		CString strHost(parsedURL->host);
		Base::FreeParsedURL(parsedURL);
		parsedURL = NULL;
		return strHost;
	}	
	return CString();
}
