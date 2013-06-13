// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef _BASE_SERIALIZE_H__
#define _BASE_SERIALIZE_H__

#include <atlstr.h>

template<class T>
class Serialize
{
public:
	static void Write(const T* src, void**ppData, unsigned* pSize){
		int size = sizeof(T);
		BYTE *pData = new BYTE[size];
		memcpy(pData, src, size);
		*ppData = pData;
		*pSize = size;
	}
	static void Read(void *pData, T **ppOut){

	}
};

template<>
class Serialize<CStringW>
{
public:
	static void Write(const CStringW* src, void**ppData, unsigned* pSize){
		int size = (src->GetLength()+1) * sizeof(WCHAR);
		BYTE *pData = new BYTE[size];
		ZeroMemory(pData, size);
		memcpy(pData, (void*)src->GetString(), size);
		*ppData = pData;
		*pSize = size;
	}
	static void Read(void *pData, CStringW **ppOut){
		*ppOut = new CStringW((WCHAR*)pData);
	}
};



#endif //_BASE_SERIALIZE_H__
