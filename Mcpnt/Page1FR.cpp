// Page1FR.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "Page1FR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage1FR property page

IMPLEMENT_DYNCREATE(CPage1FR, CPropertyPage)

CPage1FR::CPage1FR() : CPropertyPage(CPage1FR::IDD)
{
	//{{AFX_DATA_INIT(CPage1FR)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPage1FR::~CPage1FR()
{
}

void CPage1FR::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1FR)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1FR, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1FR)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1FR message handlers
