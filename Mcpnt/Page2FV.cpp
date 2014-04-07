// Page2FV.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page2FV.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage2FV property page

IMPLEMENT_DYNCREATE(CPage2FV, CPropertyPage)

CPage2FV::CPage2FV() : CPropertyPage(CPage2FV::IDD)
{
	//{{AFX_DATA_INIT(CPage2FV)
	m_noscanvolt = 0.0;
	//}}AFX_DATA_INIT
}

CPage2FV::~CPage2FV()
{
}

void CPage2FV::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage2FV)
	DDX_Text(pDX, IDC_NOSCANVOLT, m_noscanvolt);
	DDV_MinMaxDouble(pDX, m_noscanvolt, -10., 10.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage2FV, CPropertyPage)
	//{{AFX_MSG_MAP(CPage2FV)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_EN_UPDATE (IDC_NOSCANVOLT,OnSetVolt)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage2FV message handlers

BOOL CPage2FV::OnApply() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnApply();
}

void CPage2FV::OnSetVolt()
{
	SetModified(TRUE);
}
