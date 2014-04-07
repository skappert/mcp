// Page3HP.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page3HP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage3HP property page

IMPLEMENT_DYNCREATE(CPage3HP, CPropertyPage)

CPage3HP::CPage3HP() : CPropertyPage(CPage3HP::IDD)
{
	//{{AFX_DATA_INIT(CPage3HP)
	m_gpib = 0;
	m_sendcommand = _T("");
	//}}AFX_DATA_INIT
}

CPage3HP::~CPage3HP()
{
}

void CPage3HP::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3HP)
	DDX_Control(pDX, IDC_TEST, m_test);
	DDX_Text(pDX, IDC_GPIB, m_gpib);
	DDV_MinMaxInt(pDX, m_gpib, 1, 30);
	DDX_Text(pDX, IDC_SENDCOMMAND, m_sendcommand);
	DDV_MaxChars(pDX, m_sendcommand, 80);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage3HP, CPropertyPage)
	//{{AFX_MSG_MAP(CPage3HP)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3HP message handlers
