// Page2am.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page2am.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage2am property page

IMPLEMENT_DYNCREATE(CPage2am, CPropertyPage)

CPage2am::CPage2am() : CPropertyPage(CPage2am::IDD)
{
	//{{AFX_DATA_INIT(CPage2am)
	m_movecm = 0.0f;
	m_usebrake = FALSE;
	//}}AFX_DATA_INIT
}

CPage2am::~CPage2am()
{
}

void CPage2am::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage2am)
	DDX_Text(pDX, IDC_MOVECM, m_movecm);
	DDV_MinMaxFloat(pDX, m_movecm, -1.e+009f, 1.e+009f);
	DDX_Check(pDX, IDC_USEBRAKE, m_usebrake);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage2am, CPropertyPage)
	//{{AFX_MSG_MAP(CPage2am)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage2am message handlers
