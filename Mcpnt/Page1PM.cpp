// Page1PM.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page1PM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage1PM property page

IMPLEMENT_DYNCREATE(CPage1PM, CPropertyPage)

CPage1PM::CPage1PM() : CPropertyPage(CPage1PM::IDD)
{
	//{{AFX_DATA_INIT(CPage1PM)
	m_autoopen = FALSE;
	m_autoresize = FALSE;
	m_lines = FALSE;
	m_points = FALSE;
	m_stairs = FALSE;
	m_sumvoltage = FALSE;
	//}}AFX_DATA_INIT
}

CPage1PM::~CPage1PM()
{
}

void CPage1PM::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1PM)
	DDX_Check(pDX, IDC_AUTOOPEN, m_autoopen);
	DDX_Check(pDX, IDC_AUTORESIZE, m_autoresize);
	DDX_Check(pDX, IDC_LINES, m_lines);
	DDX_Check(pDX, IDC_POINTS, m_points);
	DDX_Check(pDX, IDC_STAIRS, m_stairs);
	DDX_Check(pDX, IDC_SUMVOLTAGE, m_sumvoltage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1PM, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1PM)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1PM message handlers
