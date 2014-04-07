// Page3PR.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "Page3PR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage3PR property page

IMPLEMENT_DYNCREATE(CPage3PR, CPropertyPage)

CPage3PR::CPage3PR() : CPropertyPage(CPage3PR::IDD)
{
	//{{AFX_DATA_INIT(CPage3PR)
		m_slot = 1;
		m_subaddress = 0;
	m_delay = 0;
	//}}AFX_DATA_INIT
}

CPage3PR::~CPage3PR()
{
}

void CPage3PR::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3PR)
	DDX_Control(pDX, IDC_TEST, m_test);
	DDX_Text(pDX, IDC_SLOT, m_slot);
	DDV_MinMaxInt(pDX, m_slot, 1, 24);
	DDX_Text(pDX, IDC_SUBADDRESS, m_subaddress);
	DDV_MinMaxInt(pDX, m_subaddress, 0, 15);
	DDX_Text(pDX, IDC_DELAY, m_delay);
	DDV_MinMaxLong(pDX, m_delay, 0, 10000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage3PR, CPropertyPage)
	//{{AFX_MSG_MAP(CPage3PR)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3PR message handlers
