// Page3AS.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page3AS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage3AS property page

IMPLEMENT_DYNCREATE(CPage3AS, CPropertyPage)

CPage3AS::CPage3AS() : CPropertyPage(CPage3AS::IDD)
{
	//{{AFX_DATA_INIT(CPage3AS)
	m_slot = 1;
	m_subaddress = 0;
	//}}AFX_DATA_INIT
}

CPage3AS::~CPage3AS()
{
}

void CPage3AS::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3AS)
	DDX_Text(pDX, IDC_SLOT, m_slot);
	DDV_MinMaxInt(pDX, m_slot, 1, 24);
	DDX_Text(pDX, IDC_SUBADDRESS, m_subaddress);
	DDV_MinMaxInt(pDX, m_subaddress, 17, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage3AS, CPropertyPage)
	//{{AFX_MSG_MAP(CPage3AS)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3AS message handlers
