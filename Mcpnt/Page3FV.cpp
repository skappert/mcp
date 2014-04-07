// Page3FV.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "Page3FV.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage3FV property page

IMPLEMENT_DYNCREATE(CPage3FV, CPropertyPage)

CPage3FV::CPage3FV() : CPropertyPage(CPage3FV::IDD)
{
	//{{AFX_DATA_INIT(CPage3FV)
	m_slot = 0;
	m_subaddress = 0;
	//}}AFX_DATA_INIT
}

CPage3FV::~CPage3FV()
{
}

void CPage3FV::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3FV)
	DDX_Text(pDX, IDC_SLOT, m_slot);
	DDV_MinMaxUInt(pDX, m_slot, 0, 24);
	DDX_Text(pDX, IDC_SUBADDRESS, m_subaddress);
	DDV_MinMaxUInt(pDX, m_subaddress, 0, 31);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage3FV, CPropertyPage)
	//{{AFX_MSG_MAP(CPage3FV)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3FV message handlers
