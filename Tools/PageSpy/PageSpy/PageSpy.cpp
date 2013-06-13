// PageSpy.cpp : Implementation of CPageSpyApp and DLL registration.

#include "stdafx.h"
#include "PageSpy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CPageSpyApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x11A7F50, 0xC241, 0x40FC, { 0xBD, 0xC1, 0x3F, 0xE4, 0x1B, 0x59, 0xC, 0xCE } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CPageSpyApp::InitInstance - DLL initialization

BOOL CPageSpyApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}



// CPageSpyApp::ExitInstance - DLL termination

int CPageSpyApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
