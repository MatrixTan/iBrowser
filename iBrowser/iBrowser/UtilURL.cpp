// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stdafx.h"
#include "UtilURL.h"



namespace URL
{
	BOOL IsURL( const CStringW &strSource)
	{
		BOOL bRet = TRUE;
		return bRet;
	}

#if 0



	static const int kExceptionRule = 1;
	static const int kWildcardRule = 2;
	/// 查看':'和'/'中间是不是有个port
	static BOOL HasPort(CStringW text, INT begin) {
			// Find the range between the ":" and the "/".
			INT port_start = begin + 1;
			INT port_end = port_start;

			LPCWSTR original_text = text;
			while ((port_end < text.GetLength()) &&
				!IsAuthorityTerminator((CHAR)original_text[port_end]))
				++port_end;
			if (port_end == port_start)
				return FALSE;

			// 查看是不是都是数字字符
			for (INT i = port_start; i < port_end; ++i) {
				WCHAR c = original_text[i];
				if (c <= '0' || c >= '9' )
					return FALSE;
			}
			return TRUE;
	}

	INT GetRegistryLength(CStringW host)
	{
		if(host.IsEmpty())
			return 0;
		host.MakeLower();
		burl::CBurl url(burl::FixupURL(host, L""));
		if(url.Host().IsEmpty())
			return 0;
		if(url.m_hostInfo.IsIPAddress())
			return 0;

		host = url.Host();
		std::wstring stdhost = (LPCWSTR)host;

		const size_t host_check_begin = stdhost.find_first_not_of(L'.');

		if (host_check_begin == -1)
			return 0;  // Host is only dots.
		// A single trailing dot isn't relevant in this determination, but does need
		// to be included in the final returned length.
		INT host_check_len = host.GetLength();
		if (host[host_check_len - 1] == L'.') {
			--host_check_len;
			ATLASSERT(host_check_len > 0);  // If this weren't true, the host would be ".",
			// and we'd have already returned above.
			if (host[host_check_len - 1] == L'.')
				return 0;  // Multiple trailing dots.
		}

		// Walk up the domain tree, most specific to least specific,
		// looking for matches at each level.
		INT prev_start = -1;
		int curr_start = static_cast<int>(host_check_begin);
		size_t next_dot = host.Find(L'.', curr_start);
		if (next_dot ==-1 )  // Catches std::string::npos as well.
			return 0;  // This can't have a registry + domain.
		CStringA domaina(host);
		while (1) {
			LPCSTR domain_str = domaina;
			domain_str += curr_start;
			size_t domain_length = host_check_len - curr_start;

			const DomainRule* rule = Perfect_Hash::__FindDomain(domain_str, static_cast<unsigned int>(domain_length));

			// We need to compare the string after finding a match because the
			// no-collisions of perfect hashing only refers to items in the set.  Since
			// we're searching for arbitrary domains, there could be collisions.
			if (rule &&
				_strnicmp(domain_str, rule->name, domain_length) == 0) {
					// Exception rules override wildcard rules when the domain is an exact
					// match, but wildcards take precedence when there's a subdomain.
					if (rule->type == kWildcardRule && (prev_start != -1)) {
						// If prev_start == host_check_begin, then the host is the registry
						// itself, so return 0.
						return (prev_start == host_check_begin) ?
							0 : (host.GetLength() - prev_start);
					}

					if (rule->type == kExceptionRule) {
						if (next_dot == -1) {
							// If we get here, we had an exception rule with no dots (e.g.
							// "!foo").  This would only be valid if we had a corresponding
							// wildcard rule, which would have to be "*".  But we explicitly
							// disallow that case, so this kind of rule is invalid.
							ATLASSERT(FALSE);
							return 0;
						}
						return host.GetLength() - static_cast<int>(next_dot) - 1;
					}

					// If curr_start == host_check_begin, then the host is the registry
					// itself, so return 0.
					return (curr_start == host_check_begin) ?
						0 : (host.GetLength() - curr_start);
			}

			if (next_dot == -1)  // Catches std::string::npos as well.
				break;

			prev_start = curr_start;
			curr_start = static_cast<int>(next_dot) + 1;
			next_dot = domaina.Find('.', curr_start);

		}

		// No rule found in the registry.  curr_start now points to the first
		// character of the last subcomponent of the host, so if we allow unknown
		// registries, return the length of this subcomponent.
		return 0;
	}

	static CStringW GetValidScheme(CStringW text)
	{
		if(text.IsEmpty())
			return text;
		// 找到第一个':'
		INT coloni = text.Find(L':');
		if(coloni < 0)
			return L"";
		CStringW scheme = text.Mid(0, coloni);


		//如果有':' 例如"www.example.com:/".  不一定是shceme
		// 所以查证是否含有'.' 在scheme中
		if (scheme.Find(L'.') != -1)
			return L"";

		//  "www:123/". 后边跟着port 和一个 '/' 也不是得到的shceme
		if (HasPort(text, coloni))
			return L"";

		scheme.Replace(L" ",L"");
		scheme.Replace(L"\t",L"");
		scheme.MakeLower();
		return scheme;
	}

	CStringW ReplaceDBC(CStringW text)
	{
		LPCWSTR pchar = text;
		BOOL hasDBC = FALSE;
		do
		{
			if(IsDBC(*pchar))
			{
				hasDBC = TRUE;
				break;
			}
			pchar++;
		}while(*pchar);
		return ToSBC(text);
	}
	// 检查host 是结尾如 .com等. 没有的话自动填上
	static void AddDesiredTLD(CStringW desired_tld, CStringW& domain) 
	{
			if (desired_tld.IsEmpty() || domain.IsEmpty())
				return;
			const int registry_length = GetRegistryLength(domain);
			if (registry_length != 0)
				return;

			// Add the suffix at the end of the domain.
			const int domain_length(domain.GetLength());

			if ( domain.GetAt(domain_length) != L'.')
				domain += L'.';
			domain += desired_tld;

	}
	CStringW FixupURL(CStringW text, CStringW desired_tld)
	{
		CStringW lowerUrl = text;
		lowerUrl.MakeLower();

		if((0 == lowerUrl.Find(ABOUT_PREFIX)) && lowerUrl.CompareNoCase(ABOUT_BLANK))
		{
			return ABOUT_BLANK;
		}

		CStringW trimed = text.Trim(kWhitespaceWide);
		if(trimed.IsEmpty())
			return trimed;

		CStringA utftext;
		UNI_UTF8(text, utftext);

		/// 着个在替换全角之前做 ,因为本地路径名可以有全角字符.
		if(text.Find(L"file:/") == 0 || DoesBeginWindowsDriveSpec(utftext, 0, utftext.GetLength())
			|| DoesBeginUNCPath(utftext, 0, utftext.GetLength(),TRUE)
			)
		{
			// TO DO 这里需要以后加入处理文件url的fixup
			return text;
		}

		CStringW scheme = GetValidScheme(trimed);
		if(scheme == L"javascript" || scheme == L"about" 
			|| scheme == L"view-source" || scheme == L"data"){
				return trimed;
		}
		trimed = ReplaceDBC(trimed);
		scheme = GetValidScheme(trimed);
		trimed.Replace(L'。', L'.');
		if(!scheme.IsEmpty())
		{
			if(scheme == L"htpp" || scheme == L"hhtp")
			{
				scheme = L"http";
			}else if(scheme == L"htps" || scheme == L"hhtps" || scheme == L"htpps")
			{
				scheme = L"https";
			}
			CStringA schemeutf;
			UNI_UTF8(scheme, schemeutf);
			BOOL isStandard = FALSE;
			for (size_t i = 0; i < kNumStandardURLSchemes; i++) 
			{
				if (schemeutf == kStandardURLSchemes[i])
				{
					isStandard = TRUE;
					break;
				}
			}
			if(scheme == L"localhost")
			{
				scheme = L"http";
			}else if(!isStandard)
			{
				return trimed;
			}else
			{
				trimed = trimed.Mid(trimed.Find(L':') +1);
			} 
		}else {
			scheme = L"http";
		}
		trimed  = scheme + L"://" + trimed;

		CBurl url(trimed);
		if(!url.IsValid())
			return L"";

		if(url.HasHost())
		{
 			CStringW domain = url.Host();
			domain.Replace(L"%20", L"");
			domain.TrimLeft(L'.');
			
			// Make domain valid.
			// Strip all leading dots and all but one trailing dot, unless the user only
			// typed dots, in which case their input is totally invalid and we should just
			// leave it unchanged.
			/*int dot = domain.ReverseFind(L'.');
			if(dot == domain.GetLength()-1)
			{
				domain.Trim(L'.');
				domain += L'.';
			}	*/		
			
			//// Add any user-specified TLD, if applicable.
			AddDesiredTLD(desired_tld, domain);
			CStringW old = url.Host();
			trimed = url.Spec();
			trimed.Replace(old, domain);
		}

		return trimed;
	}
	
#endif
}