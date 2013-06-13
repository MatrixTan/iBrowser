// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "CommandLine.h"
#include <ShellAPI.h>


LPCTSTR CommandLine::kPrefix = TEXT("--");
LPCTSTR	CommandLine::kDelimeter = TEXT("=");


VOID CommandLine::ParseFromString(const CString & strCommandLine)
{
	CString strCommandLineCopy = strCommandLine;
	strCommandLineCopy.Trim();

	if (strCommandLineCopy.IsEmpty())
		return;

	INT nNumArgs = 0;
	WCHAR ** pszArgs = NULL;

	pszArgs = ::CommandLineToArgvW(strCommandLineCopy.GetBuffer(), &nNumArgs);

	// Populate program_ with the trimmed version of the first arg.
	m_strProgram = pszArgs[0];
	m_strProgram.Trim();

	BOOL bParseSwitches = TRUE;
	for (INT i = 1; i < nNumArgs; ++i) 
	{
		CString strArg = pszArgs[i];
		strArg.Trim();

		if (!bParseSwitches) 
		{
			m_vNonSwitchArgs.push_back(strArg);
			continue;
		}

		if (strArg == kPrefix) 
		{
			bParseSwitches = FALSE;
			continue;
		}

		CString strSwitchName;
		CString strSwitchValue;
		if (IsSwitch(strArg, strSwitchName, strSwitchValue))
		{
			m_mapSwitchValues[strSwitchName] = strSwitchValue;
		} else {
			m_vNonSwitchArgs.push_back(strArg);
		}
	}

	if (pszArgs)
	{
		LocalFree(pszArgs);
	}

	m_bCmdLineChange = TRUE;
}

BOOL CommandLine::IsSwitch(const CString & strParameterString,
	CString & strSwitchName,
	CString & strSwitchValue)
{
	strSwitchName.Empty();
	strSwitchValue.Empty();

	CString strPrefix = kPrefix;
	if (strParameterString.Find(strPrefix) != 0)
		return FALSE;

	const INT nSwitchStart = strPrefix.GetLength();
	const INT nEqualsPosition = strParameterString.Find(
		kDelimeter, nSwitchStart);

	if (nEqualsPosition == -1) 
	{
		strSwitchName = strParameterString.Mid(nSwitchStart);
	} 
	else 
	{
		strSwitchName = strParameterString.Mid(
			nSwitchStart, nEqualsPosition - nSwitchStart);
		strSwitchValue = strParameterString.Mid(nEqualsPosition + 1);
	}

	strSwitchName.MakeLower();

	return TRUE;
}

CString CommandLine::GetSwitchValue(const CString & strSwitchName)
{
	CString strSwitchValue;

	CString strLowerCaseSwitchName = strSwitchName;
	strLowerCaseSwitchName.MakeLower();

	if (m_mapSwitchValues.count(strLowerCaseSwitchName) > 0)
		strSwitchValue = m_mapSwitchValues[strLowerCaseSwitchName];

	return strSwitchValue;
}

VOID CommandLine::AppendSwitchArg(const CString & strSwitchName, const CString & strSwitchValue)
{
	ATLASSERT(!strSwitchName.IsEmpty());
	ATLASSERT(!strSwitchValue.IsEmpty());

	m_mapSwitchValues[strSwitchName] = strSwitchValue;
	m_bCmdLineChange = TRUE;
}

CommandLine* CommandLineSingleton::GetCommand()
{
	return m_CommandLine;
}

CommandLineSingleton::CommandLineSingleton()
:m_CommandLine(NULL)
{
	m_CommandLine = new CommandLine();
}

CommandLineSingleton::~CommandLineSingleton()
{
	if (m_CommandLine)
	{
		delete m_CommandLine;
		m_CommandLine = 0;
	}
}
