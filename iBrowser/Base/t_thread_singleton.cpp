// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "t_thread_singleton.h"

DWORD gTLSKey;

void InitializeTLS( void )
{
	if (gTLSKey == NULL){
		gTLSKey = ::TlsAlloc();
	}	
}

void UnInitializeTLS( void )
{
	if (gTLSKey != NULL){
		::TlsFree(gTLSKey);
		gTLSKey = NULL;
	}	
}
