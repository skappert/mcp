// Page3FR.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "Page3FR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage3FR property page

IMPLEMENT_DYNCREATE(CPage3FR, CPropertyPage)

CPage3FR::CPage3FR() : CPropertyPage(CPage3FR::IDD)
{
	//{{AFX_DATA_INIT(CPage3FR)
	m_gpib = 0;
	m_sendcommand = _T("");
	//}}AFX_DATA_INIT
}

CPage3FR::~CPage3FR()
{
}

void CPage3FR::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3FR)
	DDX_Control(pDX, IDC_TEST, m_test);
	DDX_Text(pDX, IDC_GPIB, m_gpib);
	DDV_MinMaxInt(pDX, m_gpib, 1, 30);
	DDX_Text(pDX, IDC_SENDCOMMAND, m_sendcommand);
	DDV_MaxChars(pDX, m_sendcommand, 80);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage3FR, CPropertyPage)
	//{{AFX_MSG_MAP(CPage3FR)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3FR message handlers
