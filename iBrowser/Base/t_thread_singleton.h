// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _T_THREAD_SINGLETON_H__
#define _T_THREAD_SINGLETON_H__

#include <Windows.h>
#include <assert.h>


extern DWORD gTLSKey;

enum ETTS_CLASS
{
	ETTS_CrossProcessRenderHelper = 0,
	ETTS_COUNT
};

template<class T, int tlsIndex>
class TThreadSingleton
{
public:
	static T* GetInstance()
	{
		void** pInstance = (void**)::TlsGetValue(gTLSKey);
		if (pInstance == NULL)
		{
			pInstance = (void**)malloc(sizeof(void*)*ETTS_COUNT);
			if (pInstance != NULL)
			{
				::ZeroMemory(pInstance, sizeof(void*)*ETTS_COUNT);
				::TlsSetValue(gTLSKey, pInstance);
			}
		}
		if (pInstance != NULL)
		{
			if (NULL == pInstance[tlsIndex])
			{
				pInstance[tlsIndex] = (void*)new T();
			}
			return (T*)pInstance[tlsIndex];
		}
		assert(false);
		return NULL;
	}
	static void Destory()
	{
		void** pInstance = (void**)::TlsGetValue(gTLSKey);
		if (pInstance)
		{
			if (NULL != pInstance[tlsIndex])
			{
				delete (T*)pInstance[tlsIndex];
				pInstance[tlsIndex] = NULL;
			}
			for (int i =0; i<ETTS_COUNT; ++i)
			{
				if (NULL != pInstance[i])
				{
					return;
				}
			}

			free(pInstance);
			pInstance = NULL;
		}
		::TlsSetValue(gTLSKey, NULL);
	}
	TThreadSingleton(){};
	virtual ~TThreadSingleton(){};
};

void InitializeTLS(void);
void UnInitializeTLS(void);

#endif //_T_THREAD_SINGLETON_H__