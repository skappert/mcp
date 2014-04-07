// Page1MF.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page1MF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage1MF property page

IMPLEMENT_DYNCREATE(CPage1MF, CPropertyPage)

CPage1MF::CPage1MF() : CPropertyPage(CPage1MF::IDD)
{
	//{{AFX_DATA_INIT(CPage1MF)
	m_fluke1 = FALSE;
	m_fluke2 = FALSE;
	m_fluke3 = FALSE;
	m_nofluke = FALSE;
	m_nomass = FALSE;
	m_setamu = 0.0f;
	//}}AFX_DATA_INIT
}

CPage1MF::~CPage1MF()
{
}

void CPage1MF::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1MF)
	DDX_Check(pDX, IDC_FLUKE1, m_fluke1);
	DDX_Check(pDX, IDC_FLUKE2, m_fluke2);
	DDX_Check(pDX, IDC_FLUKE3, m_fluke3);
	DDX_Check(pDX, IDC_NOFLUKE, m_nofluke);
	DDX_Check(pDX, IDC_NOMASS, m_nomass);
	DDX_Text(pDX, IDC_SETAMU, m_setamu);
	DDV_MinMaxFloat(pDX, m_setamu, 0.f, 400.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1MF, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1MF)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1MF message handlers
