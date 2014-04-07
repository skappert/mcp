// Page3fs.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page3fs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage3fs property page

IMPLEMENT_DYNCREATE(CPage3fs, CPropertyPage)

CPage3fs::CPage3fs() : CPropertyPage(CPage3fs::IDD)
{
	//{{AFX_DATA_INIT(CPage3fs)
	m_slot = 1;
	//}}AFX_DATA_INIT
}

CPage3fs::~CPage3fs()
{
}

void CPage3fs::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3fs)
	DDX_Control(pDX, IDC_TEST, m_test);
	DDX_Text(pDX, IDC_SLOT, m_slot);
	DDV_MinMaxInt(pDX, m_slot, 1, 24);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage3fs, CPropertyPage)
	//{{AFX_MSG_MAP(CPage3fs)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3fs message handlers
