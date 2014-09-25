// Page3SR.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "Page3SR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage3SR property page

IMPLEMENT_DYNCREATE(CPage3SR, CPropertyPage)

CPage3SR::CPage3SR() : CPropertyPage(CPage3SR::IDD)
{
	//{{AFX_DATA_INIT(CPage3SR)
	m_gpib = _T("");
	m_sendcommand = _T("");
	//}}AFX_DATA_INIT
}

CPage3SR::~CPage3SR()
{
}

void CPage3SR::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3SR)
	DDX_Control(pDX, IDC_TEST, m_test);
	DDX_Text(pDX, IDC_SICL, m_gpib);
	DDV_MaxChars(pDX, m_gpib, 80);
	DDX_Text(pDX, IDC_SENDCOMMAND, m_sendcommand);
	DDV_MaxChars(pDX, m_sendcommand, 80);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage3SR, CPropertyPage)
	//{{AFX_MSG_MAP(CPage3SR)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3SR message handlers
