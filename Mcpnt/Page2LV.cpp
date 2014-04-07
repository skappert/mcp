// Page2LV.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page2LV.h"
#include "camac.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage2LV property page

IMPLEMENT_DYNCREATE(CPage2LV, CPropertyPage)

CPage2LV::CPage2LV() : CPropertyPage(CPage2LV::IDD)
{
	//{{AFX_DATA_INIT(CPage2LV)
	m_noscanvolt = 0.0;
	//}}AFX_DATA_INIT
}

CPage2LV::~CPage2LV()
{
}

void CPage2LV::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage2LV)
	DDX_Text(pDX, IDC_NOSCANVOLT, m_noscanvolt);
	DDV_MinMaxDouble(pDX, m_noscanvolt, -10000., 10000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage2LV, CPropertyPage)
	//{{AFX_MSG_MAP(CPage2LV)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_EN_UPDATE (IDC_NOSCANVOLT,OnSetVolt)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage2LV message handlers

BOOL CPage2LV::OnApply() 
{
	long Data;
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	if(m_noscanvolt!=0)m_noscanvolt		= (m_noscanvolt/fabs(m_noscanvolt))*resolution*((int)(0.49+fabs(m_noscanvolt/resolution)));
	//long Data = (long)((-m_noscanvolt/1e1)*8388608)+8388608;
	//SendNAFCamac(pApp->HvSlot,(UCHAR)1,(UCHAR)10);
	//SendDNAFCamac(Data,pApp->HvSlot,(UCHAR)0,(UCHAR)16);
	//ReadENCLCamac();
	Data = long((m_noscanvolt/1e1)*131072.);
	if(m_noscanvolt<0) Data = Data | 1<<17;
	SendDNAFCamac(Data,pApp->HvSlot,(UCHAR)0,(UCHAR)16);
	UpdateData(FALSE);
	return CPropertyPage::OnApply();
}

void CPage2LV::OnSetVolt()
{
	SetModified(TRUE);
}


void CPage2LV::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	if(m_noscanvolt!=0)m_noscanvolt		= (m_noscanvolt/fabs(m_noscanvolt))*resolution*((int)(0.49+fabs(m_noscanvolt/resolution)));
	pApp->LV0 = m_noscanvolt;	
	CPropertyPage::OnOK();
}
