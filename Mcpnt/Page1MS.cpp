// Page1MS.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page1MS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage1MS property page

IMPLEMENT_DYNCREATE(CPage1MS, CPropertyPage)

CPage1MS::CPage1MS() : CPropertyPage(CPage1MS::IDD)
{
	//{{AFX_DATA_INIT(CPage1MS)
	m_setamu = "";
	m_setamu2 = "";
	//}}AFX_DATA_INIT
}

CPage1MS::~CPage1MS()
{
}

void CPage1MS::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1MS)
	DDX_Text(pDX, IDC_SETAMU, m_setamu);
	DDX_Text(pDX, IDC_SETAMU2, m_setamu2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1MS, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1MS)
	//}}AFX_MSG_MAP
	ON_EN_UPDATE (IDC_SETAMU,OnSetAmu)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1MS message handlers
BOOL isnumber(CString TheString)
{
	TCHAR TheChar;
	USHORT i;
	for(i=0;i<strlen(TheString);i++)
	{
		TheChar = (TCHAR)TheString.GetAt(i);
		if(isalpha((int)TheChar)) return FALSE;
	}
	return TRUE;
}

void CPage1MS::OnSetAmu()
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	UpdateData(TRUE);
	setamu	= atof(m_setamu);
	m_setamu2 = m_setamu;
	if(!isnumber(m_setamu))
	{
		if(!pApp->MassesMap.IsEmpty())
		{
			CString low;
			CString Mass;
			low=m_setamu;
			low.MakeLower();
			if(pApp->MassesMap.Lookup(low,Mass))
			{
				m_setamu2 = Mass;
				setamu	= atof(m_setamu2);
			}
			else m_setamu2 = "???";
		}
	}
	UpdateData(FALSE);
}

BOOL CPage1MS::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_setamu.Format("%.8g",setamu);
	m_setamu2 = m_setamu;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
