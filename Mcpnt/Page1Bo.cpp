// Page1Bo.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "Page1Bo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage1Bo property page

IMPLEMENT_DYNCREATE(CPage1Bo, CPropertyPage)

CPage1Bo::CPage1Bo() : CPropertyPage(CPage1Bo::IDD)
{
	//{{AFX_DATA_INIT(CPage1Bo)
	m_bitchannel	= 0;
	m_biton			= 0;
	m_bitoff		= 0;
	m_bittoggle		= 0;
	//}}AFX_DATA_INIT
}

CPage1Bo::~CPage1Bo()
{
}

void CPage1Bo::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1Bo)
	DDX_Text(pDX, IDC_BITCHANNEL, m_bitchannel);
	DDV_MinMaxUInt(pDX, m_bitchannel, 0, 9999);
	DDX_Check(pDX, IDC_BITON, m_biton);
	DDX_Check(pDX, IDC_BITOFF, m_bitoff);
	DDX_Check(pDX, IDC_BITTOGGLE, m_bittoggle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1Bo, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1Bo)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1Bo message handlers
/////////////////////////////////////////////////////////////////////////////
// CPage1BO property page

IMPLEMENT_DYNCREATE(CPage1BO, CPropertyPage)

CPage1BO::CPage1BO() : CPropertyPage(CPage1BO::IDD)
{
	//{{AFX_DATA_INIT(CPage1BO)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPage1BO::~CPage1BO()
{
}

void CPage1BO::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1BO)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1BO, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1BO)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1BO message handlers
