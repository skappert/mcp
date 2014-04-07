// Page1am.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page1am.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage1am property page

IMPLEMENT_DYNCREATE(CPage1am, CPropertyPage)

CPage1am::CPage1am() : CPropertyPage(CPage1am::IDD)
{
	//{{AFX_DATA_INIT(CPage1am)
	m_movecm = 0.0f;
	m_usebrake = FALSE;
	m_backwards = FALSE;
	m_forward = TRUE;
	//}}AFX_DATA_INIT
}

CPage1am::~CPage1am()
{
}

void CPage1am::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1am)
	DDX_Text(pDX, IDC_MOVECM, m_movecm);
	DDV_MinMaxFloat(pDX, m_movecm, -1.e+009f, 1.e+009f);
	DDX_Check(pDX, IDC_USEBRAKE, m_usebrake);
	DDX_Check(pDX, IDC_BACKWARDS, m_backwards);
	DDX_Check(pDX, IDC_FORWARD, m_forward);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1am, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1am)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1am message handlers

