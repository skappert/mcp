// Page3TR.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page3TR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage3TR property page

IMPLEMENT_DYNCREATE(CPage3TR, CPropertyPage)

CPage3TR::CPage3TR() : CPropertyPage(CPage3TR::IDD)
{
	//{{AFX_DATA_INIT(CPage3TR)
	m_slot = 1;
	m_subaddress = 0;
	//}}AFX_DATA_INIT
}

CPage3TR::~CPage3TR()
{
}

void CPage3TR::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3TR)
	DDX_Control(pDX, IDC_TEST, m_test);
	DDX_Text(pDX, IDC_SLOT, m_slot);
	DDV_MinMaxInt(pDX, m_slot, 1, 24);
	DDX_Text(pDX, IDC_SUBADDRESS, m_subaddress);
	DDV_MinMaxInt(pDX, m_subaddress, 0, 15);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage3TR, CPropertyPage)
	//{{AFX_MSG_MAP(CPage3TR)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3TR message handlers
