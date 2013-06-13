#pragma once

// PageSpyPropPage.h : Declaration of the CPageSpyPropPage property page class.


// CPageSpyPropPage : See PageSpyPropPage.cpp for implementation.

class CPageSpyPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CPageSpyPropPage)
	DECLARE_OLECREATE_EX(CPageSpyPropPage)

// Constructor
public:
	CPageSpyPropPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_PAGESPY };

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	DECLARE_MESSAGE_MAP()
};

