#pragma once

// PageSpyCtrl.h : Declaration of the CPageSpyCtrl ActiveX Control class.


// CPageSpyCtrl : See PageSpyCtrl.cpp for implementation.

class CPageSpyCtrl : public COleControl
{
	DECLARE_DYNCREATE(CPageSpyCtrl)

// Constructor
public:
	CPageSpyCtrl();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// Implementation
protected:
	~CPageSpyCtrl();

	DECLARE_OLECREATE_EX(CPageSpyCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CPageSpyCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CPageSpyCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CPageSpyCtrl)		// Type name and misc status

// Message maps
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	};
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

