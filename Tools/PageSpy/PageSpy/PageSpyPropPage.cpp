// PageSpyPropPage.cpp : Implementation of the CPageSpyPropPage property page class.

#include "stdafx.h"
#include "PageSpy.h"
#include "PageSpyPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CPageSpyPropPage, COlePropertyPage)



// Message map

BEGIN_MESSAGE_MAP(CPageSpyPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CPageSpyPropPage, "PAGESPY.PageSpyPropPage.1",
	0x2f6fb2ab, 0xe317, 0x4aa9, 0xaa, 0x74, 0x85, 0x23, 0x1f, 0x71, 0xf0, 0x52)



// CPageSpyPropPage::CPageSpyPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CPageSpyPropPage

BOOL CPageSpyPropPage::CPageSpyPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_PAGESPY_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CPageSpyPropPage::CPageSpyPropPage - Constructor

CPageSpyPropPage::CPageSpyPropPage() :
	COlePropertyPage(IDD, IDS_PAGESPY_PPG_CAPTION)
{
}



// CPageSpyPropPage::DoDataExchange - Moves data between page and properties

void CPageSpyPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CPageSpyPropPage message handlers
