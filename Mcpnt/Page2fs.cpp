// Page2fs.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page2fs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage2fs property page

IMPLEMENT_DYNCREATE(CPage2fs, CPropertyPage)

CPage2fs::CPage2fs() : CPropertyPage(CPage2fs::IDD)
{
	//{{AFX_DATA_INIT(CPage2fs)
	m_fluke1 = FALSE;
	m_fluke2 = FALSE;
	m_fluke3 = FALSE;
	m_nofluke = FALSE;
	//}}AFX_DATA_INIT
}

CPage2fs::~CPage2fs()
{
}

void CPage2fs::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage2fs)
	DDX_Check(pDX, IDC_FLUKE1, m_fluke1);
	DDX_Check(pDX, IDC_FLUKE2, m_fluke2);
	DDX_Check(pDX, IDC_FLUKE3, m_fluke3);
	DDX_Check(pDX, IDC_NOFLUKE, m_nofluke);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage2fs, CPropertyPage)
	//{{AFX_MSG_MAP(CPage2fs)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage2fs message handlers
