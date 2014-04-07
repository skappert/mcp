// Page2RS.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page2RS.h"
#include "hardware.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage2RS property page

IMPLEMENT_DYNCREATE(CPage2RS, CPropertyPage)

CPage2RS::CPage2RS() : CPropertyPage(CPage2RS::IDD)
{
	//{{AFX_DATA_INIT(CPage2RS)
		m_noscannomod = FALSE;
		m_noscanmodam = FALSE;
		m_noscanmodfm = FALSE;
	//}}AFX_DATA_INIT
}

CPage2RS::~CPage2RS()
{
}

void CPage2RS::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage2RS)
	DDX_Text(pDX, IDC_NOSCANAMPDB, m_noscanampdb);
	//DDV_MinMaxDouble(pDX, m_noscanampdb, -137., 18.);
	DDX_Text(pDX, IDC_NOSCANAMPMV, m_noscanampmv);
	//DDV_MinMaxDouble(pDX, m_noscanampmv, 0., 10000.);
	DDX_Check(pDX, IDC_NOSCANAMPOFF, m_noscanampoff);
	DDX_Text(pDX, IDC_NOSCANFREQ, m_noscanfreq);
	//DDV_MinMaxDouble(pDX, m_noscanfreq, 9.e-003, 2000.);
	DDX_Text(pDX, IDC_NOSCANMODAMPAM, m_noscanmodampam);
	//DDV_MinMaxDouble(pDX, m_noscanmodampam, 0., 100.);
	DDX_Text(pDX, IDC_NOSCANMODAMPFM, m_noscanmodampfm);
	//DDV_MinMaxDouble(pDX, m_noscanmodampfm, 0., 4000.);
	DDX_Text(pDX, IDC_NOSCANMODFREQ, m_noscanmodfreq);
	//DDV_MinMaxDouble(pDX, m_noscanmodfreq, 0., 4000.);
	DDX_Check(pDX, IDC_NOSCANNOMOD, m_noscannomod);
	DDX_Check(pDX, IDC_NOSCANMODAM, m_noscanmodam);
	DDX_Check(pDX, IDC_NOSCANMODFM, m_noscanmodfm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage2RS, CPropertyPage)
	//{{AFX_MSG_MAP(CPage2RS)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_EN_UPDATE (IDC_NOSCANFREQ,OnNoScanFreq)
	ON_EN_UPDATE (IDC_NOSCANAMPDB,OnAmpDb)
	ON_EN_UPDATE (IDC_NOSCANAMPMV,OnAmpMv)
	ON_EN_UPDATE (IDC_NOSCANMODFREQ,OnNoScanFreq)
	ON_EN_UPDATE (IDC_NOSCANMODAMPAM,OnNoScanFreq)
	ON_EN_UPDATE (IDC_NOSCANMODAMPFM,OnNoScanFreq)
	ON_BN_CLICKED (IDC_NOSCANAMPOFF,OnAmpoff)
	ON_BN_CLICKED (IDC_NOSCANNOMOD,OnNoScanFreq)
	ON_BN_CLICKED (IDC_NOSCANMODAM,OnNoScanFreq)
	ON_BN_CLICKED (IDC_NOSCANMODFM,OnNoScanFreq)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage2RS message handlers

void	CPage2RS::OnNoScanFreq()
{
	SetModified(TRUE);
}

void	CPage2RS::OnAmpDb()
{
	UpdateData(TRUE);
	ampdb	= atof(m_noscanampdb);
	ampmv	= atof(m_noscanampmv);

	ampmv = 1000.0*pow((double)10,(ampdb/20))*sqrt(0.05);
	//m_ampdb.Format("%.4g",ampdb);
	m_noscanampmv.Format("%.1f",ampmv);
	SetModified(TRUE);
	UpdateData(FALSE);
}

void	CPage2RS::OnAmpMv()
{
	UpdateData(TRUE);
	ampdb	= atof(m_noscanampdb);
	ampmv	= atof(m_noscanampmv);

	ampdb = 20*log10(ampmv/sqrt(0.05)/1000.0);
	if(ampdb<-137) ampdb = -137;
	m_noscanampdb.Format("%.4g",ampdb);
	//m_ampmv.Format("%.4g",ampmv);
	SetModified(TRUE);
	UpdateData(FALSE);
}


void CPage2RS::OnAmpoff() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_noscanampoff)
	{
		ampdbback	= atof(m_noscanampdb);
		ampmvback	= atof(m_noscanampmv);
		ampdb  = (double)-137;
		ampmv  = (double)0;
		m_noscanampdb.Format("%.4g",(double)-137);
		m_noscanampmv.Format("%.1f",(double)0);
		(CEdit*)GetDlgItem(IDC_NOSCANAMPDB)->EnableWindow(FALSE);
		(CEdit*)GetDlgItem(IDC_NOSCANAMPMV)->EnableWindow(FALSE);
		SetModified(TRUE);
		UpdateData(FALSE);
	}
	else
	{
		m_noscanampdb.Format("%.4g",ampdbback);
		m_noscanampmv.Format("%.1f",ampmvback);
		ampdb  = ampdbback;
		ampmv  = ampmvback;
		(CEdit*)GetDlgItem(IDC_NOSCANAMPDB)->EnableWindow(TRUE);
		(CEdit*)GetDlgItem(IDC_NOSCANAMPMV)->EnableWindow(TRUE);
		SetModified(TRUE);
		UpdateData(FALSE);
	}	
}

BOOL CPage2RS::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	ampmv = 1000.0*pow((double)10,(ampdb/20))*sqrt(0.05);
	m_noscanampdb.Format("%.4g",ampdb);
	m_noscanampmv.Format("%.1f",ampmv);
	SetModified(TRUE);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPage2RS::OnApply() 
{
	UpdateData(TRUE);
	ampdb	= atof(m_noscanampdb);
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	CString SendString;
	if(m_noscannomod)SendString = "FM:OFF;AM:OFF";
	if(m_noscanmodfm)SendString.Format("AM:OFF;AF %.8gkHz; FM:I %.8gkHz",m_noscanmodfreq,m_noscanmodampfm);
	if(m_noscanmodam)SendString.Format("FM:OFF;AF %.8gkHz; AM %g",m_noscanmodfreq,m_noscanmodampam);
	SendGPIB(SendString,pApp->Rs1Gpib,pApp->GpibSlot);
	SendString.Format("L %.8g",ampdb);
	SendGPIB(SendString,pApp->Rs1Gpib,pApp->GpibSlot);
	SendString.Format("RF %.8g",m_noscanfreq*1e6);
	SendGPIB(SendString,pApp->Rs1Gpib,pApp->GpibSlot);
	
	return CPropertyPage::OnApply();
}
