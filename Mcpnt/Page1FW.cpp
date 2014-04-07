// Page1FW.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "Page1FW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage1FW property page

IMPLEMENT_DYNCREATE(CPage1FW, CPropertyPage)

CPage1FW::CPage1FW() : CPropertyPage(CPage1FW::IDD)
{
	//{{AFX_DATA_INIT(CPage1FW)
	m_field = 0;
	//}}AFX_DATA_INIT
}

CPage1FW::~CPage1FW()
{
}

void CPage1FW::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1FW)
	DDX_Text(pDX, IDC_FIELD, m_field);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1FW, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1FW)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1FW message handlers
