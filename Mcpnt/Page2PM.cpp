// Page2PM.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page2PM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage2PM property page

IMPLEMENT_DYNCREATE(CPage2PM, CPropertyPage)

CPage2PM::CPage2PM() : CPropertyPage(CPage2PM::IDD)
{
	//{{AFX_DATA_INIT(CPage2PM)
	m_autoopen = FALSE;
	m_autoresize = FALSE;
	m_lines = FALSE;
	m_points = FALSE;
	m_stairs = FALSE;
	//}}AFX_DATA_INIT
}

CPage2PM::~CPage2PM()
{
}

void CPage2PM::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage2PM)
	DDX_Check(pDX, IDC_AUTOOPEN, m_autoopen);
	DDX_Check(pDX, IDC_AUTORESIZE, m_autoresize);
	DDX_Check(pDX, IDC_LINES, m_lines);
	DDX_Check(pDX, IDC_POINTS, m_points);
	DDX_Check(pDX, IDC_STAIRS, m_stairs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage2PM, CPropertyPage)
	//{{AFX_MSG_MAP(CPage2PM)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage2PM message handlers
