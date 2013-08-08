// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "util_network.h"

#define DOWNLOAD_BUFFER_LENGTH 4096

namespace Util
{
	bool Util::HttpDownloadToFile( CStringW strURL,CStringW strFilePath,int nTimeout /*= 10000*/, bool bCache /*= true*/ )
	{
		bool bRet = false;
		HINTERNET	hSession = NULL;
		HINTERNET	hRequest = NULL;
		do{
			hSession = InternetOpen( _T("iBrowser"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
			if( NULL == hSession )
				break;

			DWORD dwLen = sizeof(nTimeout);
			bRet = InternetSetOption(hSession, INTERNET_OPTION_CONNECT_TIMEOUT,  (LPVOID)&nTimeout, dwLen);
			if(!bRet)
				break;

			bRet = InternetSetOption(hSession, INTERNET_OPTION_CONTROL_SEND_TIMEOUT, (LPVOID)&nTimeout, dwLen);
			if(!bRet)
				break;

			bRet = InternetSetOption(hSession, INTERNET_OPTION_SEND_TIMEOUT, (LPVOID)&nTimeout, dwLen);
			if(!bRet)
				break;

			bRet = InternetSetOption(hSession, INTERNET_OPTION_CONTROL_RECEIVE_TIMEOUT, (LPVOID)&nTimeout, dwLen);
			if(!bRet)
				break;

			bRet = InternetSetOption(hSession, INTERNET_OPTION_RECEIVE_TIMEOUT, (LPVOID)&nTimeout, dwLen);
			if(!bRet)
				break;

			if(bCache){
				hRequest = InternetOpenUrl(hSession,strURL,NULL,0,0,0);
			}else{
				hRequest = InternetOpenUrl(hSession,strURL,NULL,0,INTERNET_FLAG_RAW_DATA|INTERNET_FLAG_DONT_CACHE,0);
			}
			if(NULL == hRequest ){
				bRet = false;
				break;
			}

			DWORD dwStatus = 0;
			DWORD dwBufSize = sizeof(DWORD);
			bRet = HttpQueryInfo( hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwStatus, &dwBufSize, NULL );
			if( !bRet || 200!=dwStatus && 304!=dwStatus )
				break;

			BYTE buf[DOWNLOAD_BUFFER_LENGTH] = {0};
			DWORD dwReadBytes = 0;

			HANDLE hFile = CreateFile(strFilePath,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
			if(INVALID_HANDLE_VALUE == hFile)
				break;

			bRet = false;
			while(true)
			{
				bRet = InternetReadFile(hRequest,buf,DOWNLOAD_BUFFER_LENGTH,&dwReadBytes);
				if(!bRet )
					break;
				if(0 == dwReadBytes){
					bRet = true;
					break;
				}
				DWORD dwWriteBytes = 0;
				bRet = WriteFile(hFile, buf, dwReadBytes,&dwWriteBytes,NULL);
				if(!bRet || dwWriteBytes != dwReadBytes)
					break;
			}
			::CloseHandle(hFile);
		}while (false);

		if (hRequest)
			InternetCloseHandle(hRequest);
		if (hSession)
			InternetCloseHandle(hSession);
		return bRet;
	}

}