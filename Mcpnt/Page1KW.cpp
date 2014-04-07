// Page1KW.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "Page1KW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage1KW property page

IMPLEMENT_DYNCREATE(CPage1KW, CPropertyPage)

CPage1KW::CPage1KW() : CPropertyPage(CPage1KW::IDD)
{
	//{{AFX_DATA_INIT(CPage1KW)
	m_temp = 293;
	//}}AFX_DATA_INIT
}

CPage1KW::~CPage1KW()
{
}

void CPage1KW::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1KW)
	DDX_Text(pDX, IDC_TEMP, m_temp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1KW, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1KW)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1KW message handlers
