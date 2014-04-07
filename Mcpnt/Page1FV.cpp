// Page1FV.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "Page1FV.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage1FV property page

IMPLEMENT_DYNCREATE(CPage1FV, CPropertyPage)

CPage1FV::CPage1FV() : CPropertyPage(CPage1FV::IDD)
{
	//{{AFX_DATA_INIT(CPage1FV)
	m_centervolt = _T("");
	m_slowreset = FALSE;
	m_slowresetch = 0.0;
	m_startvolt = _T("");
	m_stepvolt = _T("");
	m_stopvolt = _T("");
	//}}AFX_DATA_INIT
}

CPage1FV::~CPage1FV()
{
}

void CPage1FV::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage1FV)
	DDX_Text(pDX, IDC_CENTERVOLT, m_centervolt);
	DDV_MaxChars(pDX, m_centervolt, 30);
	DDX_Check(pDX, IDC_SLOWRESET, m_slowreset);
	DDX_Text(pDX, IDC_SLOWRESETCH, m_slowresetch);
	DDV_MinMaxDouble(pDX, m_slowresetch, 0., 30.);
	DDX_Text(pDX, IDC_STARTVOLT, m_startvolt);
	DDV_MaxChars(pDX, m_startvolt, 30);
	DDX_Text(pDX, IDC_STEPVOLT, m_stepvolt);
	DDV_MaxChars(pDX, m_stepvolt, 30);
	DDX_Text(pDX, IDC_STOPVOLT, m_stopvolt);
	DDV_MaxChars(pDX, m_stopvolt, 30);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage1FV, CPropertyPage)
	//{{AFX_MSG_MAP(CPage1FV)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_EN_UPDATE (IDC_STARTVOLT,OnStartVolt)
	ON_EN_UPDATE (IDC_STOPVOLT,OnStopVolt)
	ON_EN_UPDATE (IDC_STEPVOLT,OnStepVolt)
	ON_EN_UPDATE (IDC_CENTERVOLT,OnCenterVolt)
	
	ON_EN_SETFOCUS (IDC_STEPVOLT,OnStepVolt)
	ON_EN_SETFOCUS (IDC_CENTERVOLT,OnCenterVolt)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage1FV message handlers
void	CPage1FV::OnStartVolt()
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

void	CPage1FV::OnStopVolt()
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

void	CPage1FV::OnStepVolt()
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

void	CPage1FV::OnCenterVolt()
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

BOOL CPage1FV::OnInitDialog() 
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

