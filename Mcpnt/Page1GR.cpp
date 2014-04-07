// Page1GR.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "Page1GR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage1GR property page

IMPLEMENT_DYNCREATE(CPage1GR, CPropertyPage)

CPage1GR::CPage1GR() : CPropertyPage(CPage1GR::IDD)
{
	//{{AFX_DATA_INIT(CPage1GR)
	m_sendstring = "";
	//}}AFX_DATA_INIT
}

CPage1GR::~CPage1GR()
{
}

void CPage1GR::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1GR)
	DDX_Text(pDX, IDC_SENDSTRING, m_sendstring);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1GR, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1GR)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1GR message handlers
