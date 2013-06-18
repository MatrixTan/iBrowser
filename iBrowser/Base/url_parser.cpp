// Copyright (c) 2013 The iBrowser Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "url_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

namespace Base{

bool isSchemeChar(int c)
{
    return (!isalpha(c) && L'+' != c && L'-' != c && L'.' != c) ? false : true;
}

/*
 * See RFC 1738, 3986
 */
ParsedURL *ParseUrl(const WCHAR *url){
    ParsedURL *purl;
    const WCHAR *tmpstr;
    const WCHAR *curstr;
    int len;
    int i;
    int userpass_flag;
    int bracket_flag;

    /* Allocate the parsed url storage */
    purl = (ParsedURL*)malloc(sizeof(ParsedURL));
    if ( NULL == purl ) {
        return NULL;
    }
    purl->scheme = NULL;
    purl->host = NULL;
    purl->port = NULL;
    purl->path = NULL;
    purl->query = NULL;
    purl->fragment = NULL;
    purl->username = NULL;
    purl->password = NULL;

    curstr = url;

    /*
     * <scheme>:<scheme-specific-part>
     * <scheme> := [a-z\+\-\.]+
     *             upper case = lower case for resiliency
     */
    /* Read scheme */
    tmpstr = wcschr(curstr, L':');
    if ( NULL == tmpstr ) {
        /* Not found the character */
        FreeParsedURL(purl);
        return NULL;
    }
    /* Get the scheme length */
    len = (tmpstr - curstr);
    /* Check restrictions */
    for ( i = 0; i < len; i++ ) {
        if ( !isSchemeChar(curstr[i]) ) {
            /* Invalid format */
            FreeParsedURL(purl);
            return NULL;
        }
    }
    /* Copy the scheme to the storage */
    purl->scheme = (WCHAR*)malloc(sizeof(WCHAR) * (len + 1));
    if ( NULL == purl->scheme ) {
        FreeParsedURL(purl);
        return NULL;
    }
    (void)wcsncpy(purl->scheme, curstr, len);
    purl->scheme[len] = L'\0';
    /* Make the character to lower if it is upper case. */
    for ( i = 0; i < len; i++ ) {
        purl->scheme[i] = tolower(purl->scheme[i]);
    }
    /* Skip ':' */
    tmpstr++;
    curstr = tmpstr;

    /*
     * //<user>:<password>@<host>:<port>/<url-path>
     * Any ":", "@" and "/" must be encoded.
     */
    /* Eat "//" */
    for ( i = 0; i < 2; i++ ) {
        if ( '/' != *curstr ) {
            FreeParsedURL(purl);
            return NULL;
        }
        curstr++;
    }

    /* Check if the user (and password) are specified. */
    userpass_flag = 0;
    tmpstr = curstr;
    while ( L'\0' != *tmpstr ) {
        if ( L'@' == *tmpstr ) {
            /* Username and password are specified */
            userpass_flag = 1;
            break;
        } else if ( L'/' == *tmpstr ) {
            /* End of <host>:<port> specification */
            userpass_flag = 0;
            break;
        }
        tmpstr++;
    }

    /* User and password specification */
    tmpstr = curstr;
    if ( userpass_flag ) {
        /* Read username */
        while ( L'\0' != *tmpstr && L':' != *tmpstr && L'@' != *tmpstr ) {
            tmpstr++;
        }
        len = (tmpstr - curstr);
        purl->username = (WCHAR*)malloc(sizeof(WCHAR) * (len + 1));
        if ( NULL == purl->username ) {
            FreeParsedURL(purl);
            return NULL;
        }
        (void)wcsncpy(purl->username, curstr, len);
        purl->username[len] = L'\0';
        /* Proceed current pointer */
        curstr = tmpstr;
        if ( L':' == *curstr ) {
            /* Skip ':' */
            curstr++;
            /* Read password */
            tmpstr = curstr;
            while ( L'\0' != *tmpstr && L'@' != *tmpstr ) {
                tmpstr++;
            }
            len = (tmpstr - curstr);
            purl->password = (WCHAR*)malloc(sizeof(WCHAR) * (len + 1));
            if ( NULL == purl->password ) {
                FreeParsedURL(purl);
                return NULL;
            }
            (void)wcsncpy(purl->password, curstr, len);
            purl->password[len] = L'\0';
            curstr = tmpstr;
        }
        /* Skip '@' */
        if ( L'@' != *curstr ) {
            FreeParsedURL(purl);
            return NULL;
        }
        curstr++;
    }

    if ( L'[' == *curstr ) {
        bracket_flag = 1;
    } else {
        bracket_flag = 0;
    }
    /* Proceed on by delimiters with reading host */
    tmpstr = curstr;
    while ( L'\0' != *tmpstr ) {
        if ( bracket_flag && L']' == *tmpstr ) {
            /* End of IPv6 address. */
            tmpstr++;
            break;
        } else if ( !bracket_flag && (L':' == *tmpstr || L'/' == *tmpstr) ) {
            /* Port number is specified. */
            break;
        }
        tmpstr++;
    }
    len = (tmpstr - curstr);
    purl->host = (WCHAR*)malloc(sizeof(WCHAR) * (len + 1));
    if ( NULL == purl->host || len <= 0 ) {
        FreeParsedURL(purl);
        return NULL;
    }
    (void)wcsncpy(purl->host, curstr, len);
    purl->host[len] = L'\0';
    curstr = tmpstr;

    /* Is port number specified? */
    if ( L':' == *curstr ) {
        curstr++;
        /* Read port number */
        tmpstr = curstr;
        while ( L'\0' != *tmpstr && L'/' != *tmpstr ) {
            tmpstr++;
        }
        len = (tmpstr - curstr);
        purl->port = (WCHAR*)malloc(sizeof(WCHAR) * (len + 1));
        if ( NULL == purl->port ) {
            FreeParsedURL(purl);
            return NULL;
        }
        (void)wcsncpy(purl->port, curstr, len);
        purl->port[len] = L'\0';
        curstr = tmpstr;
    }

    /* End of the string */
    if ( L'\0' == *curstr ) {
        return purl;
    }

    /* Skip '/' */
    if ( L'/' != *curstr ) {
        FreeParsedURL(purl);
        return NULL;
    }
    curstr++;

    /* Parse path */
    tmpstr = curstr;
    while ( L'\0' != *tmpstr && L'#' != *tmpstr  && L'?' != *tmpstr ) {
        tmpstr++;
    }
    len = (tmpstr - curstr);
    purl->path = (WCHAR*)malloc(sizeof(WCHAR) * (len + 1));
    if ( NULL == purl->path ) {
        FreeParsedURL(purl);
        return NULL;
    }
    (void)wcsncpy(purl->path, curstr, len);
    purl->path[len] = L'\0';
    curstr = tmpstr;

    /* Is query specified? */
    if ( L'?' == *curstr ) {
        /* Skip '?' */
        curstr++;
        /* Read query */
        tmpstr = curstr;
        while ( L'\0' != *tmpstr && L'#' != *tmpstr ) {
            tmpstr++;
        }
        len = (tmpstr - curstr);
        purl->query = (WCHAR*)malloc(sizeof(WCHAR) * (len + 1));
        if ( NULL == purl->query ) {
            FreeParsedURL(purl);
            return NULL;
        }
        (void)wcsncpy(purl->query, curstr, len);
        purl->query[len] = L'\0';
        curstr = tmpstr;
    }

    /* Is fragment specified? */
    if ( L'#' == *curstr ) {
        /* Skip '#' */
        curstr++;
        /* Read fragment */
        tmpstr = curstr;
        while ( L'\0' != *tmpstr ) {
            tmpstr++;
        }
        len = (tmpstr - curstr);
        purl->fragment = (WCHAR*)malloc(sizeof(WCHAR) * (len + 1));
        if ( NULL == purl->fragment ) {
            FreeParsedURL(purl);
            return NULL;
        }
        (void)wcsncpy(purl->fragment, curstr, len);
        purl->fragment[len] = L'\0';
        curstr = tmpstr;
    }

    return purl;
}

/*
 * Free memory of parsed url
 */
void
FreeParsedURL(struct ParsedURL *purl)
{
    if ( NULL != purl ) {
        if ( NULL != purl->scheme ) {
            free(purl->scheme);
        }
        if ( NULL != purl->host ) {
            free(purl->host);
        }
        if ( NULL != purl->port ) {
            free(purl->port);
        }
        if ( NULL != purl->path ) {
            free(purl->path);
        }
        if ( NULL != purl->query ) {
            free(purl->query);
        }
        if ( NULL != purl->fragment ) {
            free(purl->fragment);
        }
        if ( NULL != purl->username ) {
            free(purl->username);
        }
        if ( NULL != purl->password ) {
            free(purl->password);
        }
        free(purl);
    }
}
}