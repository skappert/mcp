// Page3PM.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page3PM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage3PM property page

IMPLEMENT_DYNCREATE(CPage3PM, CPropertyPage)

CPage3PM::CPage3PM() : CPropertyPage(CPage3PM::IDD)
{
	//{{AFX_DATA_INIT(CPage3PM)
	m_slot = 1;
	m_subaddress = 0;
	//}}AFX_DATA_INIT
}

CPage3PM::~CPage3PM()
{
}

void CPage3PM::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3PM)
	DDX_Control(pDX, IDC_TEST, m_test);
	DDX_Text(pDX, IDC_SLOT, m_slot);
	DDV_MinMaxInt(pDX, m_slot, 1, 24);
	DDX_Text(pDX, IDC_SUBADDRESS, m_subaddress);
	DDV_MinMaxInt(pDX, m_subaddress, 0, 15);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage3PM, CPropertyPage)
	//{{AFX_MSG_MAP(CPage3PM)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3PM message handlers
