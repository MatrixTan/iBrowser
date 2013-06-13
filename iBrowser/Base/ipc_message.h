// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _BASE_IPC_MESSAGE_H__
#define _BASE_IPC_MESSAGE_H__

#include <Windows.h>
#include "serialize.h"

#define WM_IPC_ASYNC_MSG WM_USER+6000

namespace IPC
{
	class Msg
	{
	public:
		Msg(){::ZeroMemory(this, sizeof(Msg));};
		~Msg(){};

		int nId;
		HANDLE hFileMapHandle;
		HWND hTargetWindow;
		UINT nWMId;
		BOOL bSync;
	};

	enum E_IPC_ERROR
	{
		EIE_NONE = 0,
		EIE_CREATE_FILE_FAILD,
		EIE_MAP_VIEW_FAILD,
	};

	static const WCHAR* kIPCMessage = L"ipc:";

	int PostIPCMessage(HWND hTarget, unsigned nMsg, void* pData, unsigned nLength);
	
	bool ParseIPCMessage(int nMsgId, HWND hTarget, UINT wmMsg, BOOL* pbSync, void** ppData, int* pSize);

	template<typename T>
	int PostIPCMessage(HWND hTarget, unsigned nMsg, const T& data){
		void* pData = NULL;
		unsigned nSize = 0;
		Serialize<T>::Write(&data, &pData, &nSize);
		PostIPCMessage(hTarget, nMsg, pData, nSize);
		return 0;
	}

	template<typename T>
	void HandleIPCMessage(T* pThis, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){
		BOOL bSync = FALSE;
		void* pData = NULL;
		int nSize = 0;
		LRESULT nResult = 0; //no use.
		if (ParseIPCMessage((int)lParam, hWnd, (UINT)wParam, &bSync, &pData, &nSize)){
			BOOL bRet = pThis->ProcessWindowMessage(hWnd, (UINT)wParam, (WPARAM)pData, 0, nResult);
			if (FALSE == bRet){
				bHandled = FALSE;
			}
			if(pData){
				::UnmapViewOfFile((BYTE*)pData-16);
			}
		}
	}

#define HANDLE_IPC_MSG( Class ) \
	if(uMsg == WM_IPC_ASYNC_MSG){\
		bHandled = TRUE; \
		IPC::HandleIPCMessage<Class>(this, hWnd, uMsg, wParam, lParam, bHandled);\
		if(bHandled) \
			return TRUE;\
	}\
	
}

#endif //_BASE_IPC_MESSAGE_H__