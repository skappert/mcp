// Page3Bo.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "Page3Bo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage3Bo property page

IMPLEMENT_DYNCREATE(CPage3Bo, CPropertyPage)

CPage3Bo::CPage3Bo() : CPropertyPage(CPage3Bo::IDD)
{
	//{{AFX_DATA_INIT(CPage3Bo)
	m_slot = 0;
	m_subaddress = 0;
	//}}AFX_DATA_INIT
}

CPage3Bo::~CPage3Bo()
{
}

void CPage3Bo::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3Bo)
	DDX_Text(pDX, IDC_SLOT, m_slot);
	DDV_MinMaxUInt(pDX, m_slot, 0, 24);
	DDX_Text(pDX, IDC_SUBADDRESS, m_subaddress);
	DDV_MinMaxUInt(pDX, m_subaddress, 0, 31);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage3Bo, CPropertyPage)
	//{{AFX_MSG_MAP(CPage3Bo)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3Bo message handlers
