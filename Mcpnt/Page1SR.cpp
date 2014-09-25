// Page1SR.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "Page1SR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage1SR property page

IMPLEMENT_DYNCREATE(CPage1SR, CPropertyPage)

CPage1SR::CPage1SR() : CPropertyPage(CPage1SR::IDD)
{
	//{{AFX_DATA_INIT(CPage1SR)
	m_sendstring = "";
	//}}AFX_DATA_INIT
}

CPage1SR::~CPage1SR()
{
}

void CPage1SR::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1SR)
	DDX_Text(pDX, IDC_SENDSTRING, m_sendstring);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1SR, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1SR)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1SR message handlers
