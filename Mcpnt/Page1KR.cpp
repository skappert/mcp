// Page1KR.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "Page1KR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage1KR property page

IMPLEMENT_DYNCREATE(CPage1KR, CPropertyPage)

CPage1KR::CPage1KR() : CPropertyPage(CPage1KR::IDD)
{
	//{{AFX_DATA_INIT(CPage1KR)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPage1KR::~CPage1KR()
{
}

void CPage1KR::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1KR)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1KR, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1KR)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1KR message handlers
