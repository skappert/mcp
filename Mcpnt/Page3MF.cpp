// Page3MF.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page3MF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage3MF property page

IMPLEMENT_DYNCREATE(CPage3MF, CPropertyPage)

CPage3MF::CPage3MF() : CPropertyPage(CPage3MF::IDD)
{
	//{{AFX_DATA_INIT(CPage3MF)
	m_gps = FALSE;
	m_hrs = FALSE;
	m_slot = 1;
	//}}AFX_DATA_INIT
}

CPage3MF::~CPage3MF()
{
}

void CPage3MF::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3MF)
	DDX_Control(pDX, IDC_TEST, m_test);
	DDX_Check(pDX, IDC_GPS, m_gps);
	DDX_Check(pDX, IDC_HRS, m_hrs);
	DDX_Text(pDX, IDC_SLOT, m_slot);
	DDV_MinMaxInt(pDX, m_slot, 1, 24);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage3MF, CPropertyPage)
	//{{AFX_MSG_MAP(CPage3MF)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3MF message handlers
