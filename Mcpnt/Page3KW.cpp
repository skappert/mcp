// Page3KW.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "Page3KW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CPage3KW, CPropertyPage)

CPage3KW::CPage3KW() : CPropertyPage(CPage3KW::IDD)
{
	//{{AFX_DATA_INIT(CPage3KW)
	m_gpib = 0;
	m_sendcommand = _T("");
	//}}AFX_DATA_INIT
}

CPage3KW::~CPage3KW()
{
}

void CPage3KW::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3KW)
	DDX_Control(pDX, IDC_TEST, m_test);
	DDX_Text(pDX, IDC_GPIB, m_gpib);
	DDV_MinMaxInt(pDX, m_gpib, 1, 30);
	DDX_Text(pDX, IDC_SENDCOMMAND, m_sendcommand);
	DDV_MaxChars(pDX, m_sendcommand, 80);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage3KW, CPropertyPage)
	//{{AFX_MSG_MAP(CPage3KW)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3KW message handlers
