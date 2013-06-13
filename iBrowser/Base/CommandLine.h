// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _COMMAND_LINE_H__
#define _COMMAND_LINE_H__


#include <atlstr.h>
#include <map>
#include <vector>

#include "TSingleton.h"

class CommandLine
{
public:
	CommandLine(const CString & strProgram = CString(""))  : m_bCmdLineChange(FALSE)
	{
		if (!strProgram.IsEmpty())
		{
			m_strProgram = strProgram;
		}

		m_appPath[0] = 0;
	}

	inline const CString&	GetCommandLine()
	{
		if (m_bCmdLineChange || m_strArgv.IsEmpty())
		{
			m_strArgv += L'"' + m_strProgram + L'"';
			for (std::map<CString, CString>::const_iterator iter = m_mapSwitchValues.begin(); iter != m_mapSwitchValues.end(); ++iter)
			{
				m_strArgv.AppendFormat(TEXT(" %s%s%s%s"), 
					kPrefix,
					iter->first,
					kDelimeter,
					iter->second
					);
			}

			for (std::vector<CString>::const_iterator iter = m_vNonSwitchArgs.begin(); iter != m_vNonSwitchArgs.end(); ++iter)
			{
				m_strArgv.AppendFormat(TEXT(" %s \"%s\""), 
					kPrefix,
					*iter
					);
			}

			m_strArgv.Trim();
			m_bCmdLineChange = FALSE;
		}

		return m_strArgv;
	}

	//////////////////////////////////////////////////////////
	VOID ParseFromString(const CString & strCommandLine);

	CString GetProgram() const
	{
		return m_strProgram;
	}

	const std::vector<CString> & GetNoneSwitchArgs() const { return m_vNonSwitchArgs; }

	CString GetSwitchValue(const CString & strSwitchName);

	VOID AppendSwitchArg(const CString & strSwitchName, const CString & strSwitchValue);

private:
	static BOOL IsSwitch(const CString & strParameterString,
		CString & strSwitchName,
		CString & strSwitchValue);

public:
	// Define const switch names, just instead of macros
	static LPCTSTR kPrefix;
	static LPCTSTR kDelimeter;

private:
	CString		m_strArgv;
	TCHAR		m_appPath[MAX_PATH];
	BOOL		m_bCmdLineChange;

	CString m_strProgram;
	std::map<CString, CString> m_mapSwitchValues;
	// Non-switch command-line arguments.
	std::vector<CString> m_vNonSwitchArgs;
};

class CommandLineSingleton : public TSingleton<CommandLineSingleton>
{
public:
	CommandLine* GetCommand();
	~CommandLineSingleton();

protected:
	CommandLineSingleton();
	CommandLine* m_CommandLine;
};


#endif //_COMMAND_LINE_H__