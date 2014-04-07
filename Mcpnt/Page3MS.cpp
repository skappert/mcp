// Page3MS.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page3MS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage3MS property page

IMPLEMENT_DYNCREATE(CPage3MS, CPropertyPage)

CPage3MS::CPage3MS() : CPropertyPage(CPage3MS::IDD)
{
	//{{AFX_DATA_INIT(CPage3MS)
	m_gps = FALSE;
	m_hrs = FALSE;
	//}}AFX_DATA_INIT
}

CPage3MS::~CPage3MS()
{
}

void CPage3MS::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3MS)
	DDX_Control(pDX, IDC_TEST, m_test);
	DDX_Check(pDX, IDC_GPS, m_gps);
	DDX_Check(pDX, IDC_HRS, m_hrs);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPage3MS, CPropertyPage)
	//{{AFX_MSG_MAP(CPage3MS)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3MS message handlers
