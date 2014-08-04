// Page1RS.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page1RS.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage1RS property page

IMPLEMENT_DYNCREATE(CPage1RS, CPropertyPage)

CPage1RS::CPage1RS() : CPropertyPage(CPage1RS::IDD)
{
	//{{AFX_DATA_INIT(CPage1RS)

	//}}AFX_DATA_INIT
}

CPage1RS::~CPage1RS()
{
}

void CPage1RS::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1RS)
	DDX_Text(pDX, IDC_AMPDB, m_ampdb);
	//DDV_MinMaxFloat(pDX, m_ampdb, -137.f, 13.f);
	DDX_Text(pDX, IDC_AMPMV, m_ampmv);
	//DDV_MinMaxFloat(pDX, m_ampmv, 0.f, 10000.f);
	DDX_Check(pDX, IDC_AMPOFF, m_ampoff);
	DDX_Text(pDX, IDC_CENTERFREQ, m_centerfreq);
	//DDV_MinMaxFloat(pDX, m_centerfreq, 9.e-003f, 1000.f);
	DDX_Check(pDX, IDC_MODAM, m_modam);
	DDX_Text(pDX, IDC_MODAMPAM, m_modampam);
	//DDV_MinMaxFloat(pDX, m_modampam, 0.f, 100.f);
	DDX_Text(pDX, IDC_MODAMPFM, m_modampfm);
	//DDV_MinMaxFloat(pDX, m_modampfm, 0.f, 5000.f);
	DDX_Check(pDX, IDC_MODFM, m_modfm);
	DDX_Text(pDX, IDC_MODFREQ, m_modfreq);
	//DDV_MinMaxFloat(pDX, m_modfreq, 0.f, 400.f);
	DDX_Check(pDX, IDC_NOMOD, m_nomod);
	DDX_Text(pDX, IDC_STARTFREQ, m_startfreq);
	//DDV_MinMaxFloat(pDX, m_startfreq, 9.e-003f, 1000.f);
	DDX_Text(pDX, IDC_STEPFREQ, m_stepfreq);
	//DDV_MinMaxFloat(pDX, m_stepfreq, 9.e-003f, 1000.f);
	DDX_Text(pDX, IDC_STOPFREQ, m_stopfreq);
	//DDV_MinMaxFloat(pDX, m_stopfreq, 9.e-003f, 1000.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1RS, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1RS)
	//}}AFX_MSG_MAP
	ON_EN_UPDATE (IDC_STARTFREQ,OnStartFreq)
	ON_EN_UPDATE (IDC_STOPFREQ,OnStopFreq)
	ON_EN_UPDATE (IDC_STEPFREQ,OnStepFreq)
	ON_EN_UPDATE (IDC_CENTERFREQ,OnCenterFreq)
	
	ON_EN_SETFOCUS (IDC_STEPFREQ,OnStepFreq)
	ON_EN_SETFOCUS (IDC_CENTERFREQ,OnCenterFreq)

	ON_EN_UPDATE (IDC_AMPDB,OnAmpDb)
	ON_EN_UPDATE (IDC_AMPMV,OnAmpMv)

	ON_BN_CLICKED (IDC_AMPOFF,OnAmpoff)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1RS message handlers
void	CPage1RS::OnStartFreq()
{
	UpdateData(TRUE);
	startfreq	= atof(m_startfreq);
	stopfreq	= atof(m_stopfreq);
	stepfreq	= atof(m_stepfreq);
	centerfreq	= atof(m_centerfreq);

	stepfreq		= (stopfreq-startfreq)/(channels-1);
	if(stepfreq!=0)stepfreq		= (stepfreq/fabs(stepfreq))*resolution*((int)(0.49+fabs(stepfreq/resolution)));
	//stopfreq		= startfreq+(channels-1)*stepfreq;
	centerfreq		= startfreq+((channels-1)/2)*stepfreq;
	
	//m_startfreq.Format("%.8g",startfreq);
	m_stopfreq.Format("%.8g",stopfreq);
	m_stepfreq.Format("%.8g",stepfreq);
	m_centerfreq.Format("%.8g",centerfreq);
	UpdateData(FALSE);	
}

void	CPage1RS::OnStopFreq()
{
	UpdateData(TRUE);
	startfreq	= atof(m_startfreq);
	stopfreq	= atof(m_stopfreq);
	stepfreq	= atof(m_stepfreq);
	centerfreq	= atof(m_centerfreq);

	stepfreq		= (stopfreq-startfreq)/(channels-1);
	if(stepfreq!=0)stepfreq		= (stepfreq/fabs(stepfreq))*resolution*((int)(0.49+fabs(stepfreq/resolution)));
	//startfreq		= stopfreq-(channels-1)*stepfreq;
	centerfreq		= startfreq+((channels-1)/2)*stepfreq;

	m_startfreq.Format("%.8g",startfreq);
	//m_stopfreq.Format("%.8g",stopfreq);
	m_stepfreq.Format("%.8g",stepfreq);
	m_centerfreq.Format("%.8g",centerfreq);

	UpdateData(FALSE);
}

void	CPage1RS::OnStepFreq()
{
	UpdateData(TRUE);
	startfreq	= atof(m_startfreq);
	stopfreq	= atof(m_stopfreq);
	stepfreq	= atof(m_stepfreq);
	centerfreq	= atof(m_centerfreq);

	if(stepfreq!=0)
	{
		stepfreq		= (stepfreq/fabs(stepfreq))*resolution*((int)(0.49+fabs(stepfreq/resolution)));
		if(stepfreq == 0) stepfreq = resolution;

		if(2*((int)channels/2)!=channels)
		{
			startfreq		= centerfreq-((channels-1)/2)*stepfreq;
			stopfreq		= centerfreq+((channels-1)/2)*stepfreq;
		}
		else
		{
			startfreq		= centerfreq-((channels-1)/2)*stepfreq;
			stopfreq		= centerfreq+(1+((channels-1)/2))*stepfreq;
		}
	}
	else
	{
		startfreq=stopfreq=centerfreq;
	}
	m_startfreq.Format("%.8g",startfreq);
	m_stopfreq.Format("%.8g",stopfreq);
	//m_stepfreq.Format("%.8g",stepfreq);
	m_centerfreq.Format("%.8g",centerfreq);

	UpdateData(FALSE);
}

void	CPage1RS::OnCenterFreq()
{
	UpdateData(TRUE);
	startfreq	= atof(m_startfreq);
	stopfreq	= atof(m_stopfreq);
	stepfreq	= atof(m_stepfreq);
	centerfreq	= atof(m_centerfreq);
	
	if(stepfreq!=0)
	{
		stepfreq		= (stepfreq/fabs(stepfreq))*resolution*((int)(0.49+fabs(stepfreq/resolution)));
		if(stepfreq == 0) stepfreq = resolution;

		if(2*((int)channels/2)!=channels)
		{
			startfreq		= centerfreq-((channels-1)/2)*stepfreq;
			stopfreq		= centerfreq+((channels-1)/2)*stepfreq;
		}
		else
		{
			startfreq		= centerfreq-((channels-1)/2)*stepfreq;
			stopfreq		= centerfreq+(1+((channels-1)/2))*stepfreq;
		}
	}

	m_startfreq.Format("%.8g",startfreq);
	m_stopfreq.Format("%.8g",stopfreq);
	m_stepfreq.Format("%.8g",stepfreq);
	//m_centerfreq.Format("%.8g",centerfreq);

	UpdateData(FALSE);
}

void	CPage1RS::OnAmpDb()
{
	UpdateData(TRUE);
	ampdb	= atof(m_ampdb);
	ampmv	= atof(m_ampmv);

	ampmv = 1000.0*pow((double)10,(ampdb/20))*sqrt(0.05);
	//m_ampdb.Format("%.4g",ampdb);
	m_ampmv.Format("%.1f",ampmv);

	UpdateData(FALSE);
}

void	CPage1RS::OnAmpMv()
{
	UpdateData(TRUE);
	ampdb	= atof(m_ampdb);
	ampmv	= atof(m_ampmv);

	ampdb = 20*log10(ampmv/sqrt(0.05)/1000.0);
	if(ampdb<-137) ampdb = -137;
	m_ampdb.Format("%.4g",ampdb);
	//m_ampmv.Format("%.4g",ampmv);

	UpdateData(FALSE);
}


void CPage1RS::OnAmpoff() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_ampoff)
	{
		ampdbback	= atof(m_ampdb);
		ampmvback	= atof(m_ampmv);
		ampdb  = (double)-137;
		ampmv  = (double)0;
		m_ampdb.Format("%.4g",(double)-137);
		m_ampmv.Format("%.1f",(double)0);
		(CEdit*)GetDlgItem(IDC_AMPDB)->EnableWindow(FALSE);
		(CEdit*)GetDlgItem(IDC_AMPMV)->EnableWindow(FALSE);
		UpdateData(FALSE);
	}
	else
	{
		m_ampdb.Format("%.4g",ampdbback);
		m_ampmv.Format("%.1f",ampmvback);
		ampdb  = ampdbback;
		ampmv  = ampmvback;
		(CEdit*)GetDlgItem(IDC_AMPDB)->EnableWindow(TRUE);
		(CEdit*)GetDlgItem(IDC_AMPMV)->EnableWindow(TRUE);
		UpdateData(FALSE);
	}	
}

BOOL CPage1RS::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	stepfreq		= (stopfreq-startfreq)/(channels-1);
	if(stepfreq!=0)stepfreq		= (stepfreq/fabs(stepfreq))*resolution*((int)(0.49+fabs(stepfreq/resolution)));
	//stopfreq		= startfreq+(channels-1)*stepfreq;
	centerfreq		= startfreq+((channels-1)/2)*stepfreq;

	m_startfreq.Format("%.8g",startfreq);
	m_stopfreq.Format("%.8g",stopfreq);
	m_stepfreq.Format("%.8g",stepfreq);
	m_centerfreq.Format("%.8g",centerfreq);

	ampmv = 1000.0*pow((double)10,(ampdb/20))*sqrt(0.05);
	m_ampdb.Format("%.4g",ampdb);
	m_ampmv.Format("%.1f",ampmv);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
