// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _UTIL_DEBUG_H__
#define _UTIL_DEBUG_H__

#ifdef _ENABLE_DEBUG_STRING
#define DEBUG_STR(x) ::OutputDebugString( x )
#define DEBUG_LINE(x) ::OutputDebugString(x);OutputDebugString(_T("\n"));
#else
#define DEBUG_STR(x)
#endif

#endif //_UTIL_DEBUG_H__
