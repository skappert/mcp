// Page3TI.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page3TI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage3TI property page

IMPLEMENT_DYNCREATE(CPage3TI, CPropertyPage)

CPage3TI::CPage3TI() : CPropertyPage(CPage3TI::IDD)
{
	//{{AFX_DATA_INIT(CPage3TI)
	m_slot = 1;
	//}}AFX_DATA_INIT
}

CPage3TI::~CPage3TI()
{
}

void CPage3TI::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3TI)
	DDX_Control(pDX, IDC_TEST, m_test);
	DDX_Text(pDX, IDC_SLOT, m_slot);
	DDV_MinMaxInt(pDX, m_slot, 1, 24);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage3TI, CPropertyPage)
	//{{AFX_MSG_MAP(CPage3TI)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3TI message handlers
