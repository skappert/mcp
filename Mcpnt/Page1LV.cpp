// Page1LV.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page1LV.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage1LV property page

IMPLEMENT_DYNCREATE(CPage1LV, CPropertyPage)

CPage1LV::CPage1LV() : CPropertyPage(CPage1LV::IDD)
{
	//{{AFX_DATA_INIT(CPage1LV)
	//m_centervolt = 0.0f;
	//m_slowreset = FALSE;
	//m_slowresetch = 0.0f;
	//m_startvolt = 0.0f;
	//m_stepvolt = 0.0f;
	//m_stopvolt = 0.0f;
	//}}AFX_DATA_INIT
}

CPage1LV::~CPage1LV()
{
}

void CPage1LV::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1LV)
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


BEGIN_MESSAGE_MAP(CPage1LV, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1LV)
	//}}AFX_MSG_MAP
	ON_EN_UPDATE (IDC_STARTVOLT,OnStartVolt)
	ON_EN_UPDATE (IDC_STOPVOLT,OnStopVolt)
	ON_EN_UPDATE (IDC_STEPVOLT,OnStepVolt)
	ON_EN_UPDATE (IDC_CENTERVOLT,OnCenterVolt)
	
	ON_EN_SETFOCUS (IDC_STEPVOLT,OnStepVolt)
	ON_EN_SETFOCUS (IDC_CENTERVOLT,OnCenterVolt)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1LV message handlers

void	CPage1LV::OnStartVolt()
{
	UpdateData(TRUE);
	startvolt	= atof(m_startvolt);
	stopvolt	= atof(m_stopvolt);
	stepvolt	= atof(m_stepvolt);
	centervolt	= atof(m_centervolt);

	stepvolt		= (stopvolt-startvolt)/(channels-1);
	if(stepvolt!=0)	stepvolt = (stepvolt/fabs(stepvolt))*resolution*((int)(0.49+fabs(stepvolt/resolution)));
	stopvolt		= startvolt+(channels-1)*stepvolt;
	centervolt		= startvolt+((channels-1)>>1)*stepvolt;

	//m_startvolt.Format("%.3f",startvolt);
	m_stopvolt.Format("%.3f",stopvolt);
	m_stepvolt.Format("%.3f",stepvolt);
	m_centervolt.Format("%.3f",centervolt);
	UpdateData(FALSE);	
}

void	CPage1LV::OnStopVolt()
{
	UpdateData(TRUE);
	startvolt	= atof(m_startvolt);
	stopvolt	= atof(m_stopvolt);
	stepvolt	= atof(m_stepvolt);
	centervolt	= atof(m_centervolt);

	stepvolt		= (stopvolt-startvolt)/(channels-1);
	if(stepvolt!=0)stepvolt		= (stepvolt/fabs(stepvolt))*resolution*((int)(0.49+fabs(stepvolt/resolution)));
	centervolt		= startvolt+((channels-1)>>1)*stepvolt;
	
	//m_startvolt.Format("%.3g",startvolt);
	//m_stopvolt.Format("%.3f",stopvolt);
	m_stepvolt.Format("%.3f",stepvolt);
	m_centervolt.Format("%.3f",centervolt);

	UpdateData(FALSE);
}

void	CPage1LV::OnStepVolt()
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

	m_startvolt.Format("%.3f",startvolt);
	m_stopvolt.Format("%.3f",stopvolt);
	//m_stepvolt.Format("%.3f",stepvolt);
	m_centervolt.Format("%.3f",centervolt);

	UpdateData(FALSE);
}

void	CPage1LV::OnCenterVolt()
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

	m_startvolt.Format("%.3f",startvolt);
	m_stopvolt.Format("%.3f",stopvolt);
	m_stepvolt.Format("%.3f",stepvolt);
	//m_centervolt.Format("%.3f",centervolt);

	UpdateData(FALSE);
}

BOOL CPage1LV::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	stepvolt		= (stopvolt-startvolt)/(channels-1);
	if(stepvolt!=0)stepvolt		= (stepvolt/fabs(stepvolt))*resolution*((int)(0.49+fabs(stepvolt/resolution)));
	//stopvolt		= startvolt+(channels-1)*stepvolt;
	centervolt		= startvolt+((channels-1)>>1)*stepvolt;

	m_startvolt.Format("%.3f",startvolt);
	m_stopvolt.Format("%.3f",stopvolt);
	m_stepvolt.Format("%.3f",stepvolt);
	m_centervolt.Format("%.3f",centervolt);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPage1LV::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	startvolt	= atof(m_startvolt);
	stopvolt	= atof(m_stopvolt);

	if(startvolt!=0)startvolt		= (startvolt/fabs(startvolt))*resolution*((int)(0.49+fabs(startvolt/resolution)));

	stepvolt		= (stopvolt-startvolt)/(channels-1);
	if(stepvolt!=0)stepvolt		= (stepvolt/fabs(stepvolt))*resolution*((int)(0.49+fabs(stepvolt/resolution)));
	stopvolt		= startvolt+(channels-1)*stepvolt;

	CPropertyPage::OnOK();
}
