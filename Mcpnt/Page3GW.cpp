// Page3GW.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "Page3GW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage3GW property page

IMPLEMENT_DYNCREATE(CPage3GW, CPropertyPage)

CPage3GW::CPage3GW() : CPropertyPage(CPage3GW::IDD)
{
	//{{AFX_DATA_INIT(CPage3GW)
	m_gpib = 0;
	m_sendcommand = _T("");
	//}}AFX_DATA_INIT
}

CPage3GW::~CPage3GW()
{
}

void CPage3GW::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3GW)
	DDX_Control(pDX, IDC_TEST, m_test);
	DDX_Text(pDX, IDC_GPIB, m_gpib);
	DDV_MinMaxInt(pDX, m_gpib, 1, 30);
	DDX_Text(pDX, IDC_SENDCOMMAND, m_sendcommand);
	DDV_MaxChars(pDX, m_sendcommand, 80);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage3GW, CPropertyPage)
	//{{AFX_MSG_MAP(CPage3GW)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3GW message handlers
