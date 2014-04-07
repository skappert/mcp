// Page1HV.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page1HV.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage1HV property page

IMPLEMENT_DYNCREATE(CPage1HV, CPropertyPage)

CPage1HV::CPage1HV() : CPropertyPage(CPage1HV::IDD)
{
	//{{AFX_DATA_INIT(CPage1HV)
	//m_centervolt = 0.0f;
	//m_slowreset = FALSE;
	//m_slowresetch = 0.0f;
	//m_startvolt = 0.0f;
	//m_stepvolt = 0.0f;
	//m_stopvolt = 0.0f;
	//}}AFX_DATA_INIT
}

CPage1HV::~CPage1HV()
{
}

void CPage1HV::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1HV)
	DDX_Text(pDX, IDC_CENTERVOLT, m_centervolt);
	//DDV_MinMaxFloat(pDX, m_centervolt, -10.f, 10.f);
	DDX_Check(pDX, IDC_SLOWRESET, m_slowreset);
	DDX_Text(pDX, IDC_SLOWRESETCH, m_slowresetch);
	//DDV_MinMaxFloat(pDX, m_slowresetch, 0.f, 60.f);
	DDX_Text(pDX, IDC_STARTVOLT, m_startvolt);
	//DDV_MinMaxFloat(pDX, m_startvolt, -10.f, 10.f);
	DDX_Text(pDX, IDC_STEPVOLT, m_stepvolt);
	//DDV_MinMaxFloat(pDX, m_stepvolt, -10.f, 10.f);
	DDX_Text(pDX, IDC_STOPVOLT, m_stopvolt);
	//DDV_MinMaxFloat(pDX, m_stopvolt, -10.f, 10.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1HV, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1HV)
	//}}AFX_MSG_MAP
	ON_EN_UPDATE (IDC_STARTVOLT,OnStartVolt)
	ON_EN_UPDATE (IDC_STOPVOLT,OnStopVolt)
	ON_EN_UPDATE (IDC_STEPVOLT,OnStepVolt)
	ON_EN_UPDATE (IDC_CENTERVOLT,OnCenterVolt)
	
	ON_EN_SETFOCUS (IDC_STEPVOLT,OnStepVolt)
	ON_EN_SETFOCUS (IDC_CENTERVOLT,OnCenterVolt)

	//ON_EN_UPDATE (IDC_AMPDB,OnAmpDb)
	//ON_EN_UPDATE (IDC_AMPMV,OnAmpMv)

	//ON_BN_CLICKED (IDC_AMPOFF,OnAmpoff)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1HV message handlers

void	CPage1HV::OnStartVolt()
{
	UpdateData(TRUE);
	startvolt	= atof(m_startvolt);
	stopvolt	= atof(m_stopvolt);
	stepvolt	= atof(m_stepvolt);
	centervolt	= atof(m_centervolt);

	stepvolt		= (stopvolt-startvolt)/(channels-1);
	if(stepvolt!=0)stepvolt		= (stepvolt/fabs(stepvolt))*resolution*((int)(0.49+fabs(stepvolt/resolution)));
	//stopvolt		= startvolt+(channels-1)*stepvolt;
	centervolt		= startvolt+((channels-1)>>1)*stepvolt;
	
	//m_startvolt.Format("%.8g",startvolt);
	m_stopvolt.Format("%.8g",stopvolt);
	m_stepvolt.Format("%.8g",stepvolt);
	m_centervolt.Format("%.8g",centervolt);
	UpdateData(FALSE);	
}

void	CPage1HV::OnStopVolt()
{
	UpdateData(TRUE);
	startvolt	= atof(m_startvolt);
	stopvolt	= atof(m_stopvolt);
	stepvolt	= atof(m_stepvolt);
	centervolt	= atof(m_centervolt);

	stepvolt		= (stopvolt-startvolt)/(channels-1);
	if(stepvolt!=0)stepvolt		= (stepvolt/fabs(stepvolt))*resolution*((int)(0.49+fabs(stepvolt/resolution)));
	//startvolt		= stopvolt-(channels-1)*stepvolt;
	centervolt		= startvolt+((channels-1)>>1)*stepvolt;

	m_startvolt.Format("%.8g",startvolt);
	//m_stopvolt.Format("%.8g",stopvolt);
	m_stepvolt.Format("%.8g",stepvolt);
	m_centervolt.Format("%.8g",centervolt);

	UpdateData(FALSE);
}

void	CPage1HV::OnStepVolt()
{
	UpdateData(TRUE);
	startvolt	= atof(m_startvolt);
	stopvolt	= atof(m_stopvolt);
	stepvolt	= atof(m_stepvolt);
	centervolt	= atof(m_centervolt);

	if(stepvolt!=0)
	{
		stepvolt		= (stepvolt/fabs(stepvolt))*resolution*((int)(0.49+fabs(stepvolt/resolution)));
		if(stepvolt == 0) stepvolt = resolution;

		if(2*((int)channels/2)!=channels)
		{
			startvolt		= centervolt-((channels-1)>>1)*stepvolt;
			stopvolt		= centervolt+((channels-1)>>1)*stepvolt;
		}
		else
		{
			startvolt		= centervolt-((channels-1)>>1)*stepvolt;
			stopvolt		= centervolt+(1+(channels-1)>>1)*stepvolt;
		}
	}
	else
	{
		startvolt=stopvolt=centervolt;
	}

	m_startvolt.Format("%.8g",startvolt);
	m_stopvolt.Format("%.8g",stopvolt);
	//m_stepvolt.Format("%.8g",stepvolt);
	m_centervolt.Format("%.8g",centervolt);

	UpdateData(FALSE);
}

void	CPage1HV::OnCenterVolt()
{
	UpdateData(TRUE);
	startvolt	= atof(m_startvolt);
	stopvolt	= atof(m_stopvolt);
	stepvolt	= atof(m_stepvolt);
	centervolt	= atof(m_centervolt);
	
	if(stepvolt!=0)
	{
		stepvolt		= (stepvolt/fabs(stepvolt))*resolution*((int)(0.49+fabs(stepvolt/resolution)));
		if(stepvolt == 0) stepvolt = resolution;

		if(2*((int)channels/2)!=channels)
		{
			startvolt		= centervolt-((channels-1)>>1)*stepvolt;
			stopvolt		= centervolt+((channels-1)>>1)*stepvolt;
		}
		else
		{
			startvolt		= centervolt-((channels-1)>>1)*stepvolt;
			stopvolt		= centervolt+(1+(channels-1)>>1)*stepvolt;
		}
	}

	m_startvolt.Format("%.8g",startvolt);
	m_stopvolt.Format("%.8g",stopvolt);
	m_stepvolt.Format("%.8g",stepvolt);
	//m_centervolt.Format("%.8g",centervolt);

	UpdateData(FALSE);
}

BOOL CPage1HV::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	stepvolt		= (stopvolt-startvolt)/(channels-1);
	if(stepvolt!=0)stepvolt		= (stepvolt/fabs(stepvolt))*resolution*((int)(0.49+fabs(stepvolt/resolution)));
	//stopvolt		= startvolt+(channels-1)*stepvolt;
	centervolt		= startvolt+((channels-1)>>1)*stepvolt;

	m_startvolt.Format("%.8g",startvolt);
	m_stopvolt.Format("%.8g",stopvolt);
	m_stepvolt.Format("%.8g",stepvolt);
	m_centervolt.Format("%.8g",centervolt);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


