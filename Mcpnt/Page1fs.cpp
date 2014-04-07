// Page1fs.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page1fs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage1fs property page

IMPLEMENT_DYNCREATE(CPage1fs, CPropertyPage)

CPage1fs::CPage1fs() : CPropertyPage(CPage1fs::IDD)
{
	//{{AFX_DATA_INIT(CPage1fs)
	m_fluke1 = FALSE;
	m_fluke2 = FALSE;
	m_fluke3 = FALSE;
	m_nofluke = FALSE;
	//}}AFX_DATA_INIT
}

CPage1fs::~CPage1fs()
{
}

void CPage1fs::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1fs)
	DDX_Check(pDX, IDC_FLUKE1, m_fluke1);
	DDX_Check(pDX, IDC_FLUKE2, m_fluke2);
	DDX_Check(pDX, IDC_FLUKE3, m_fluke3);
	DDX_Check(pDX, IDC_NOFLUKE, m_nofluke);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1fs, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1fs)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1fs message handlers
