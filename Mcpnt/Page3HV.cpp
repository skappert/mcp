// Page3HV.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page3HV.h"
#include "Hardware.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
static ULONG Con = 0; 
/////////////////////////////////////////////////////////////////////////////
// CPage3HV property page

IMPLEMENT_DYNCREATE(CPage3HV, CPropertyPage)

CPage3HV::CPage3HV() : CPropertyPage(CPage3HV::IDD)
{
	//{{AFX_DATA_INIT(CPage3HV)
	m_slot = 1;
	m_subaddress = 0;
	//}}AFX_DATA_INIT
}

CPage3HV::~CPage3HV()
{
}

void CPage3HV::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3HV)
	DDX_Control(pDX, IDC_TEST, m_test);
	DDX_Text(pDX, IDC_SLOT, m_slot);
	DDV_MinMaxInt(pDX, m_slot, 1, 24);
	DDX_Text(pDX, IDC_SUBADDRESS, m_subaddress);
	DDV_MinMaxInt(pDX, m_subaddress, 0, 15);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage3HV, CPropertyPage)
	//{{AFX_MSG_MAP(CPage3HV)
	ON_BN_CLICKED(IDC_TEST, OnTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3HV message handlers

void CPage3HV::OnTest() 
{
	// TODO: Add your control notification handler code here
	SendGPIB("*RST",8/*GPIB-Address*/,10/*CAMAC-Slot*/);
}

