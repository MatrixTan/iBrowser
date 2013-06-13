// PageSpyCtrl.cpp : Implementation of the CPageSpyCtrl ActiveX Control class.

#include "stdafx.h"
#include "PageSpy.h"
#include "PageSpyCtrl.h"
#include "PageSpyPropPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CPageSpyCtrl, COleControl)



// Message map

BEGIN_MESSAGE_MAP(CPageSpyCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// Dispatch map

BEGIN_DISPATCH_MAP(CPageSpyCtrl, COleControl)
	DISP_FUNCTION_ID(CPageSpyCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()



// Event map

BEGIN_EVENT_MAP(CPageSpyCtrl, COleControl)
END_EVENT_MAP()



// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CPageSpyCtrl, 1)
	PROPPAGEID(CPageSpyPropPage::guid)
END_PROPPAGEIDS(CPageSpyCtrl)



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CPageSpyCtrl, "PAGESPY.PageSpyCtrl.1",
	0x43e0e8f, 0xf0ab, 0x4583, 0xa0, 0x59, 0x56, 0x3f, 0xdb, 0xcd, 0xc8, 0x4e)



// Type library ID and version

IMPLEMENT_OLETYPELIB(CPageSpyCtrl, _tlid, _wVerMajor, _wVerMinor)



// Interface IDs

const IID BASED_CODE IID_DPageSpy =
		{ 0x70E082FC, 0x1A8B, 0x4575, { 0x8C, 0xEE, 0xB7, 0x35, 0xA1, 0xCA, 0x2D, 0x97 } };
const IID BASED_CODE IID_DPageSpyEvents =
		{ 0xC347D226, 0x682A, 0x44DF, { 0x84, 0x9E, 0x3E, 0x7B, 0x9A, 0xC1, 0x68, 0xC } };



// Control type information

static const DWORD BASED_CODE _dwPageSpyOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CPageSpyCtrl, IDS_PAGESPY, _dwPageSpyOleMisc)



// CPageSpyCtrl::CPageSpyCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CPageSpyCtrl

BOOL CPageSpyCtrl::CPageSpyCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_PAGESPY,
			IDB_PAGESPY,
			afxRegApartmentThreading,
			_dwPageSpyOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CPageSpyCtrl::CPageSpyCtrl - Constructor

CPageSpyCtrl::CPageSpyCtrl()
{
	InitializeIIDs(&IID_DPageSpy, &IID_DPageSpyEvents);
	// TODO: Initialize your control's instance data here.
}



// CPageSpyCtrl::~CPageSpyCtrl - Destructor

CPageSpyCtrl::~CPageSpyCtrl()
{
	// TODO: Cleanup your control's instance data here.
}



// CPageSpyCtrl::OnDraw - Drawing function

void CPageSpyCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// CPageSpyCtrl::DoPropExchange - Persistence support

void CPageSpyCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.
}



// CPageSpyCtrl::OnResetState - Reset control to default state

void CPageSpyCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}



// CPageSpyCtrl::AboutBox - Display an "About" box to the user

void CPageSpyCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_PAGESPY);
	dlgAbout.DoModal();
}



// CPageSpyCtrl message handlers

int CPageSpyCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//AboutBox();
	::SetTimer(m_hWnd, 5665, 100, NULL);
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

void CPageSpyCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 5665)
	{
		CStringW strLog;
		HWND hFocus = ::GetFocus();
		strLog.Format(L"Focus:%x\n", hFocus);
		::OutputDebugString(strLog);
	}
	COleControl::OnTimer(nIDEvent);
}
