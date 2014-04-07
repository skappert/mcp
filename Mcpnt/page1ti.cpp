// Page1TI.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page1TI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage1TI property page

IMPLEMENT_DYNCREATE(CPage1TI, CPropertyPage)

CPage1TI::CPage1TI() : CPropertyPage(CPage1TI::IDD)
{
	//{{AFX_DATA_INIT(CPage1TI)
	//}}AFX_DATA_INIT
}

CPage1TI::~CPage1TI()
{
}

void CPage1TI::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1TI)
	DDX_Check(pDX, IDC_SWEEPTRIGGER, m_sweeptrigger);
	DDX_Check(pDX, IDC_STEPTRIGGER, m_steptrigger);
	DDX_Check(pDX, IDC_NOTRIGGER, m_notrigger);
	DDX_Text(pDX, IDC_CLOSESCALER, m_closescaler);
	DDX_Text(pDX, IDC_CLOSEBEAMGATE, m_closebeamgate);
	DDX_Text(pDX, IDC_OPENBEAMGATE, m_openbeamgate);
	DDX_Text(pDX, IDC_OPENSCALER, m_openscaler);
	DDX_Text(pDX, IDC_STARTBEAMGATE, m_startbeamgate);
	DDX_Text(pDX, IDC_STOPBEAMGATE, m_stopbeamgate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1TI, CPropertyPage)
	ON_EN_UPDATE (IDC_CLOSESCALER,OnCloseScaler)
	ON_EN_UPDATE (IDC_CLOSEBEAMGATE,OnCloseBeamgate)
	ON_EN_UPDATE (IDC_OPENBEAMGATE,OnOpenBeamgate)
	ON_EN_UPDATE (IDC_OPENSCALER,OnOpenScaler)
	ON_EN_UPDATE (IDC_STARTBEAMGATE,OnStartBeamgate)
	ON_EN_UPDATE (IDC_STOPBEAMGATE,OnStopBeamgate)
	//{{AFX_MSG_MAP(CPage1TI)
	ON_BN_CLICKED(IDC_SHOWPROTONS, OnShowprotons)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1TI message handlers

void CPage1TI::OnCloseScaler()
{
	UpdateData(TRUE);

	openscaler		= atof(m_openscaler);
	closescaler		= atof(m_closescaler);
	openbeamgate	= atof(m_openbeamgate);
	closebeamgate	= atof(m_closebeamgate);

	m_openscaler.Format("%.8g",openscaler);
	//m_closescaler.Format("%.8g",closescaler);
	m_openbeamgate.Format("%.8g",openbeamgate);

	UpdateData(FALSE);
}

void CPage1TI::OnCloseBeamgate()
{
	UpdateData(TRUE);

	openscaler		= atof(m_openscaler);
	closescaler		= atof(m_closescaler);
	openbeamgate	= atof(m_openbeamgate);
	closebeamgate	= atof(m_closebeamgate);

	m_openscaler.Format("%.8g",openscaler);
	//m_closescaler.Format("%.8g",closescaler);
	m_openbeamgate.Format("%.8g",openbeamgate);

	UpdateData(FALSE);
}

void CPage1TI::OnOpenBeamgate()
{
	UpdateData(TRUE);

	openscaler		= atof(m_openscaler);
	closescaler		= atof(m_closescaler);
	openbeamgate	= atoi(m_openbeamgate);
	closebeamgate	= atof(m_closebeamgate);

	m_openscaler.Format("%.8g",openscaler);
	m_closescaler.Format("%.8g",closescaler);
	//m_openbeamgate.Format("%.8g",openbeamgate);

	UpdateData(FALSE);
}

void CPage1TI::OnOpenScaler()
{
	UpdateData(TRUE);

	openscaler		= atof(m_openscaler);
	closescaler		= atof(m_closescaler);
	openbeamgate	= atof(m_openbeamgate);
	closebeamgate	= atof(m_closebeamgate);

	if(closescaler<openscaler) closescaler = openscaler;

	//m_openscaler.Format("%.8g",openscaler);
	m_closescaler.Format("%.8g",closescaler);
	m_openbeamgate.Format("%.8g",openbeamgate);

	UpdateData(FALSE);
}

void CPage1TI::OnStartBeamgate()
{
	UpdateData(TRUE);

	startbeamgate	= atoi(m_startbeamgate);
	stopbeamgate	= atoi(m_stopbeamgate);

	if(startbeamgate < 1)startbeamgate = 1;
	if(stopbeamgate<startbeamgate) stopbeamgate = startbeamgate;

	m_startbeamgate.Format("%u",startbeamgate);
	m_stopbeamgate.Format("%u",stopbeamgate);

	UpdateData(FALSE);
}

void CPage1TI::OnStopBeamgate()
{
	UpdateData(TRUE);

	startbeamgate	= atoi(m_startbeamgate);
	stopbeamgate	= atoi(m_stopbeamgate);

	if(startbeamgate < 1)startbeamgate = 1;
	if(stopbeamgate<startbeamgate) stopbeamgate = startbeamgate;

	m_startbeamgate.Format("%u",startbeamgate);
	m_stopbeamgate.Format("%u",stopbeamgate);

	UpdateData(FALSE);
}


BOOL CPage1TI::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_openscaler.Format("%.8g",openscaler);
	m_closescaler.Format("%.8g",closescaler);
	m_openbeamgate.Format("%.8g",openbeamgate);
	m_closebeamgate.Format("%.8g",closebeamgate);

	m_startbeamgate.Format("%u",startbeamgate);
	m_stopbeamgate.Format("%u",stopbeamgate);
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
 

void CPage1TI::OnShowprotons() 
{
	// TODO: Add your control notification handler code here
	CPropertySheet* pP = (CPropertySheet*)GetParent();
	pP->EndDialog(IDC_SHOWPROTONS);	
}

