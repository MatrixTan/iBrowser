// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "update_manager.h"
#include <WinInet.h>
#include <jsoncpp/json.h>
#include <atlstr.h>
#include "version.h"
#include "global_singleton.h"
#include "profile.h"
#include "util_common.h"
#include "util_network.h"

static const WCHAR* kCheckVersionURL = L"http://127.0.0.1/wantgame/1/ibrowser/version_info.json";

void UpdateManager::StartCheck( void )
{
	FirstRunCheck();
	InstallCheck();
	TempCheck();

	CStringW strDownloadURL;
	if (VersionCheck(strDownloadURL)){
		int nRet = ::MessageBox(NULL, L"有新版本，是否更新？", L"Update", MB_YESNO);
		if (nRet == IDYES){
			WCHAR buffer[512] = {0};
			::GetTempPath(512, buffer);
			CStringW strTempPath = buffer;
			int nPos = strDownloadURL.ReverseFind(L'/');
			if (nPos > 0){
				strTempPath += strDownloadURL.Right(strDownloadURL.GetLength()-nPos-1);
				if (Util::HttpDownloadToFile(strDownloadURL, strTempPath)){	
					SetInstallPath();
					nRet = ::MessageBox(NULL, L"新版本已下载完毕，是否重启浏览器？", L"Update", MB_YESNO);
					if (nRet == IDYES){
						ExecuteNext(strTempPath);
					}else{
						GlobalSingleton::GetInstance()->GetProfile()->SetInstallTempPath(strTempPath);
					}
				}
			}
		}
	}
}

void UpdateManager::FirstRunCheck( void )
{
	iVersion version;
	if (!GlobalSingleton::GetInstance()->GetProfile()->GetVersion(version)){
		GlobalSingleton::GetInstance()->GetProfile()->SetVersion(
			GlobalSingleton::GetInstance()->GetCurrentVersion()
			);
	}
}

bool UpdateManager::VersionCheck( CStringW &strDownLoadURL )
{
	bool bRet = false;
	HINTERNET hInternet = ::InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(hInternet){
		HINTERNET hInternetFile = ::InternetOpenUrl(hInternet, kCheckVersionURL, NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE, NULL);
		if (hInternetFile){
			BYTE buffer[1024]= {0};
			DWORD dwBytes = 0;
			if (::InternetReadFile(hInternetFile, (void*)buffer, 1024, &dwBytes)){
				Json::Reader jsonReader;
				Json::Value jsonValue;
				if (jsonReader.parse((char*)buffer, jsonValue)){
					CStringW strVersion = CA2W(jsonValue["version"].asCString());
					iVersion version;
					version.FromString(strVersion);
					iVersion localVersion;
					if (GlobalSingleton::GetInstance()->GetProfile()->GetVersion(localVersion)){
						if (version > localVersion){
							strDownLoadURL = CA2W(jsonValue["download"].asCString());
							bRet = true;
						}
					}
				}
			}
			::InternetCloseHandle(hInternetFile);
		}else{
			::InternetCloseHandle(hInternet);
			bRet = false;
		}
	}

	return bRet;
}

void UpdateManager::ExecuteNext( const CStringW& strExeFile )
{
	::ShellExecute(NULL, L"open", strExeFile, NULL, NULL, SW_SHOWNORMAL);
	_exit(0);
}

void UpdateManager::InstallCheck()
{
	iVersion localVersion;
	GlobalSingleton::GetInstance()->GetProfile()->GetVersion(localVersion);
	iVersion currentVersion = GlobalSingleton::GetInstance()->GetCurrentVersion();
	if (currentVersion > localVersion){
		CStringW strCurrentPath = Util::GetCurrentModuleFileName();
		WCHAR buffer[MAX_PATH] = {0};
		::GetTempPath(MAX_PATH, buffer);
		//WCHAR longBuffer[1024] ={0};
		//::GetLongPathName(buffer, longBuffer, 1024 );
		if (strCurrentPath.Find(buffer) >= 0){
			CStringW strInstallPath = GlobalSingleton::GetInstance()->GetProfile()->GetInstallPath();
			if (::CopyFile(strCurrentPath, strInstallPath, FALSE)){
				GlobalSingleton::GetInstance()->GetProfile()->SetVersion(currentVersion);
				ExecuteNext(strInstallPath);
			}
		}
	}
}

void UpdateManager::TempCheck()
{
	CStringW strTempPath;
	if (GlobalSingleton::GetInstance()->GetProfile()->GetInstallTempPath(strTempPath)){
		if (::PathFileExists(strTempPath)){
			GlobalSingleton::GetInstance()->GetProfile()->DeleteInstallTempPath();
			SetInstallPath();
			ExecuteNext(strTempPath);
		}
	}
}

void UpdateManager::SetInstallPath( void )
{
	CStringW strInstallPath = Util::GetCurrentModuleFileName();
	GlobalSingleton::GetInstance()->GetProfile()->SetInstallPath(strInstallPath);
}
