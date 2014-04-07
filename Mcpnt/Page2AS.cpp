// Page2AS.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page2AS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage2AS property page

IMPLEMENT_DYNCREATE(CPage2AS, CPropertyPage)

CPage2AS::CPage2AS() : CPropertyPage(CPage2AS::IDD)
{
	//{{AFX_DATA_INIT(CPage2AS)
	m_autoopen = FALSE;
	m_autoresize = FALSE;
	m_dataop = _T("");
	m_errorop = _T("");
	m_lines = FALSE;
	m_points = FALSE;
	m_stairs = FALSE;
	//}}AFX_DATA_INIT
}

CPage2AS::~CPage2AS()
{
}

void CPage2AS::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage2AS)
	DDX_Check(pDX, IDC_AUTOOPEN, m_autoopen);
	DDX_Check(pDX, IDC_AUTORESIZE, m_autoresize);
	DDX_Text(pDX, IDC_DATAOP, m_dataop);
	DDV_MaxChars(pDX, m_dataop, 80);
	DDX_Text(pDX, IDC_ERROROP, m_errorop);
	DDV_MaxChars(pDX, m_errorop, 80);
	DDX_Check(pDX, IDC_LINES, m_lines);
	DDX_Check(pDX, IDC_POINTS, m_points);
	DDX_Check(pDX, IDC_STAIRS, m_stairs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage2AS, CPropertyPage)
	//{{AFX_MSG_MAP(CPage2AS)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage2AS message handlers
