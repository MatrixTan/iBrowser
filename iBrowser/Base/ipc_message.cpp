// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ipc_message.h"
#include <stdlib.h>

namespace IPC
{
	
	static long g_MsgId = 0;
	static int GenerateNewId(){
		unsigned int nRandomPart = 0;
		rand_s(&nRandomPart);
		int nIncrementPart = ::InterlockedIncrement(&g_MsgId);
		return (nRandomPart<<8)|(nIncrementPart&0x00FF);
	}

	static void CALLBACK PostMessageCallback(HWND hWnd, UINT uMsg, ULONG_PTR pData, LRESULT ret){
		HANDLE hFileMap = (HANDLE)pData;
		::CloseHandle(hFileMap);
	}

	Msg* CreateMessage(HWND hTarget, unsigned nMsg, void* pData, unsigned nLength, BOOL bSync)
	{
		Msg *pMsg = new Msg();
		pMsg->nId = GenerateNewId();
		pMsg->hTargetWindow = hTarget;
		pMsg->nWMId = nMsg;
		pMsg->bSync = bSync;

		int nMessageSize = nLength + 16;
		WCHAR strMapName[MAX_PATH] = {0};
		wsprintf(strMapName, L"%s%d", kIPCMessage, pMsg->nId);
		HANDLE hFileMap = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, nMessageSize, strMapName);
		if (NULL == hFileMap){
			return NULL;
		}
		LPVOID pMapData = ::MapViewOfFile(hFileMap, FILE_MAP_READ|FILE_MAP_WRITE, 0, 0, nMessageSize);
		if (NULL == pMapData){
			DWORD dwErr = ::GetLastError();
			return NULL;
		}
		memcpy(pMapData, &(pMsg->nId), 4);
		memcpy((BYTE*)pMapData+4, &(pMsg->hTargetWindow), 4);
		memcpy((BYTE*)pMapData+8, &nLength, 4);
		memcpy((BYTE*)pMapData+12, &(pMsg->bSync), 4);
		memcpy((BYTE*)pMapData+16, pData, nLength);
		::UnmapViewOfFile(pMapData);
		pMsg->hFileMapHandle = hFileMap;
		return pMsg;
	}

	int PostIPCMessage( HWND hTarget, unsigned nMsg, void* pData, unsigned nLength)
	{
		Msg* pMsg = CreateMessage(hTarget, nMsg, pData, nLength, FALSE);
		if (NULL == pMsg){
			return 0;
		}

		::SendMessageCallback(hTarget, WM_IPC_ASYNC_MSG, nMsg , (LPARAM)pMsg->nId, PostMessageCallback, (ULONG_PTR)pMsg->hFileMapHandle);
		return 0;
	}

	bool ParseIPCMessage( int nMsgId, HWND hTarget, UINT wmMsg, BOOL* pbSync, void** ppData , int* pSize)
	{
		if (NULL == pbSync || NULL == ppData || NULL == pSize){
			return false;
		}

		WCHAR strMapName[MAX_PATH] = {0};
		wsprintf(strMapName, L"%s%d", kIPCMessage, nMsgId);
		HANDLE hMap = ::OpenFileMapping(FILE_MAP_READ|FILE_MAP_WRITE, FALSE, strMapName);
		if (NULL == hMap){
			return false;
		}

		LPVOID pData = ::MapViewOfFile(hMap, FILE_MAP_READ|FILE_MAP_WRITE, 0, 0, 0);
		if (NULL == pData){
			CloseHandle(hMap);
			return false;
		}
		BYTE *pByteData = (BYTE*)pData;
		*pSize = (unsigned)*(pByteData+8);
		if (*pSize <= 0){
			CloseHandle(hMap);
			return false;
		}
		*pbSync = (BOOL)*(pByteData+12);
		*ppData = (void*)(pByteData+16);
		CloseHandle(hMap);
		return true;
	}


}


