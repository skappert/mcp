// Page1PR.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "Page1PR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage1PR property page

IMPLEMENT_DYNCREATE(CPage1PR, CPropertyPage)

CPage1PR::CPage1PR() : CPropertyPage(CPage1PR::IDD)
{
	//{{AFX_DATA_INIT(CPage1PR)
	m_autoopen = FALSE;
	m_autoresize = FALSE;
	m_lines = FALSE;
	m_points = FALSE;
	m_stairs = FALSE;
	//}}AFX_DATA_INIT
}

CPage1PR::~CPage1PR()
{
}

void CPage1PR::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1PR)
	DDX_Check(pDX, IDC_AUTOOPEN, m_autoopen);
	DDX_Check(pDX, IDC_AUTORESIZE, m_autoresize);
	DDX_Check(pDX, IDC_LINES, m_lines);
	DDX_Check(pDX, IDC_POINTS, m_points);
	DDX_Check(pDX, IDC_STAIRS, m_stairs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1PR, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1PR)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1PR message handlers
