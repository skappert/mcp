// Page1GW.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "Page1GW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage1GW property page

IMPLEMENT_DYNCREATE(CPage1GW, CPropertyPage)

CPage1GW::CPage1GW() : CPropertyPage(CPage1GW::IDD)
{
	//{{AFX_DATA_INIT(CPage1GW)
	m_sendstring = "";
	//}}AFX_DATA_INIT
}

CPage1GW::~CPage1GW()
{
}

void CPage1GW::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1GW)
	DDX_Text(pDX, IDC_SENDSTRING, m_sendstring);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1GW, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1GW)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1GW message handlers
