// Page3RS.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page3RS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage3RS property page

IMPLEMENT_DYNCREATE(CPage3RS, CPropertyPage)

CPage3RS::CPage3RS() : CPropertyPage(CPage3RS::IDD)
{
	//{{AFX_DATA_INIT(CPage3RS)
	m_gpib = 0;
	m_sendcommand = _T("");
	//}}AFX_DATA_INIT
}

CPage3RS::~CPage3RS()
{
}

void CPage3RS::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3RS)
	DDX_Control(pDX, IDC_TEST, m_test);
	DDX_Text(pDX, IDC_GPIB, m_gpib);
	DDV_MinMaxInt(pDX, m_gpib, 1, 30);
	DDX_Text(pDX, IDC_SENDCOMMAND, m_sendcommand);
	DDV_MaxChars(pDX, m_sendcommand, 80);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage3RS, CPropertyPage)
	//{{AFX_MSG_MAP(CPage3RS)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3RS message handlers
