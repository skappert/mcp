// Page2HV.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page2HV.h"
#include "camac.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage2HV property page

IMPLEMENT_DYNCREATE(CPage2HV, CPropertyPage)

CPage2HV::CPage2HV() : CPropertyPage(CPage2HV::IDD)
{
	//{{AFX_DATA_INIT(CPage2HV)
	m_noscanvolt = 0.0;
	//}}AFX_DATA_INIT
}

CPage2HV::~CPage2HV()
{
}

void CPage2HV::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage2HV)
	DDX_Text(pDX, IDC_NOSCANVOLT, m_noscanvolt);
	DDV_MinMaxDouble(pDX, m_noscanvolt, -10., 10.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage2HV, CPropertyPage)
	//{{AFX_MSG_MAP(CPage2HV)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_EN_UPDATE (IDC_NOSCANVOLT,OnSetVolt)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage2HV message handlers

BOOL CPage2HV::OnApply() 
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	ULONG Value	= (ULONG)(1000.0*fabs(m_noscanvolt));
	ULONG i,NoDigits,Digit;
    unsigned long Buffer;

    if (Value!=0)
    {
		Buffer = 10;
		for (NoDigits=1; Value>=Buffer; NoDigits++)           // Anzahl der Stellen bestimmen
		{ 
			Buffer = Buffer*10; 
		};
		Buffer = 0;                                           // reset Buffer
		for (i=1; i<=NoDigits; i++)
		{
			Digit = Value % 10;
			Value = Value / 10;
			Buffer = Buffer + ( Digit << ((i-1)*4) );
		}
	}
    else
	{
		Buffer = 0; 
	};
     
	if(m_noscanvolt<0)SendDNAFCamac((ULONG)(~0-1),pApp->HpSlot,1,16);
	else SendDNAFCamac((ULONG)~0,pApp->HpSlot,1,16);
	SendDNAFCamac((ULONG)~Buffer,pApp->HpSlot,0,16);
	return CPropertyPage::OnApply();
}

void CPage2HV::OnSetVolt()
{
	SetModified(TRUE);
}

