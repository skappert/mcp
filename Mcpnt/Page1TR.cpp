// Page1TR.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page1TR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage1TR property page

IMPLEMENT_DYNCREATE(CPage1TR, CPropertyPage)

CPage1TR::CPage1TR() : CPropertyPage(CPage1TR::IDD)
{
	//{{AFX_DATA_INIT(CPage1TR)
	m_numscans = 0;
	m_numsteps = 0;
	m_protonlimit = 0;
	m_realscans = 1;
	m_noredo = FALSE;
	m_redostop = FALSE;
	m_redowait = FALSE;
	m_redowaitredo = FALSE;
	m_isotope = _T("");
	m_cycles = 0;
	m_element = _T("");
	m_wavenumber = 0.0;
	m_kepcofactor = 0.0;
	//}}AFX_DATA_INIT
}

CPage1TR::~CPage1TR()
{
}

void CPage1TR::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1TR)
	DDX_Text(pDX, IDC_NUMSCANS, m_numscans);
	DDV_MinMaxInt(pDX, m_numscans, 1, 10000);
	DDX_Text(pDX, IDC_NUMSTEPS, m_numsteps);
	DDV_MinMaxInt(pDX, m_numsteps, 1, 10000);
	DDX_Text(pDX, IDC_PROTONLIMIT, m_protonlimit);
	DDV_MinMaxLong(pDX, m_protonlimit, 0, 10000000);
	DDX_Text(pDX, IDC_REALSCANS, m_realscans);
	DDV_MinMaxInt(pDX, m_realscans, 0, 10000);
	DDX_Check(pDX, IDC_NOREDO, m_noredo);
	DDX_Check(pDX, IDC_REDOSTOP, m_redostop);
	DDX_Check(pDX, IDC_REDOWAIT, m_redowait);
	DDX_Check(pDX, IDC_REDOWAITREDO, m_redowaitredo);
	DDX_Text(pDX, IDC_ISOTOPENAME, m_isotope);
	DDV_MaxChars(pDX, m_isotope, 80);
	DDX_Text(pDX, IDC_CYCLES, m_cycles);
	DDV_MinMaxUInt(pDX, m_cycles, 1, 1000);
	DDX_Text(pDX, IDC_ELEMENTNAME, m_element);
	DDV_MaxChars(pDX, m_element, 100);
	DDX_Text(pDX, IDC_WAVENUMBER, m_wavenumber);
	DDV_MinMaxDouble(pDX, m_wavenumber, 0., 100000.);
	DDX_Text(pDX, IDC_KEPCOFACTOR, m_kepcofactor);
	DDV_MinMaxDouble(pDX, m_kepcofactor, 0., 1000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1TR, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1TR)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1TR message handlers
