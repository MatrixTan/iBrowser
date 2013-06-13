//////////////////////////////////////////////////////////////////////////////
//
//  Presence of this DLL (detoured.dll) marks a process as detoured.
//
//  Microsoft Research Detours Package, Version 2.1.
//
//  Copyright (c) Microsoft Corporation.  All rights reserved.
//

#include <windows.h>
#include "detoured.h"

static HMODULE s_hDll;



HMODULE WINAPI Detoured()
{
    return s_hDll;
}

//#pragma comment(linker, "/export:Detoured = _Detoured@0")


#ifdef VC_LIB
BOOL WINAPI DllMain_Detours(HINSTANCE hinst, DWORD dwReason, LPVOID reserved)
#else
BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved)
#endif 
{
    (void)reserved;

    if (dwReason == DLL_PROCESS_ATTACH) {
        s_hDll = hinst;
        DisableThreadLibraryCalls(hinst);
    }
    return TRUE;
}
//
///////////////////////////////////////////////////////////////// End of File.
