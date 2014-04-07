// Display.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Display.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDisplay dialog


CDisplay::CDisplay(CWnd* pParent /*=NULL*/)
	: CDialog(CDisplay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDisplay)
	m_axisblue = 0;
	m_axisgreen = 0;
	m_axisred = 0;
	m_axislines = 0;
	m_axistext = 0;
	m_datablue = 0;
	m_datalines = 0;
	m_dataxmax = 0;
	m_dataxmin = 0;
	m_dataymax = 0;
	m_dataymin = 0;
	m_errorbars = 0;
	m_fitblue = 0;
	m_fitgreen = 0;
	m_fitred = 0;
	m_fitfunres = 0;
	m_fitlines = 0;
	m_halfticklenx = 0;
	m_numbers = 0;
	m_points = 0;
	m_selectblue = 0;
	m_selectgreen = 0;
	m_halftickleny = 0;
	m_selectred = 0;
	m_textblue = 0;
	m_textgreen = 0;
	m_textred = 0;
	m_ticklabelsx = 0;
	m_ticklabelsy = 0;
	m_ticklenx = 0;
	m_tickleny = 0;
	m_ticklines = 0;
	m_titleposx = 0;
	m_titleposy = 0;
	m_xaxistitlex = 0;
	m_xaxistitley = 0;
	m_yaxistitlex = 0;
	m_yaxistitley = 0;
	m_datared = 0;
	m_datagreen = 0;
	m_autolsi = FALSE;
	m_autoplot = FALSE;
	m_autoscript = FALSE;
	m_autotemplate = FALSE;
	m_backblue = 0;
	m_backgreen = 0;
	m_backred = 0;
	m_lsidx = 0;
	m_lsidy = 0;
	m_lsix = 0;
	m_lsiy = 0;
	m_numberblue = 0;
	m_numbergreen = 0;
	m_numberred = 0;
	m_plotdx = 0;
	m_plotdy = 0;
	m_plotx = 0;
	m_ploty = 0;
	m_scriptdx = 0;
	m_scriptdy = 0;
	m_scriptx = 0;
	m_scripty = 0;
	m_templatedx = 0;
	m_templatedy = 0;
	m_templatex = 0;
	m_templatey = 0;
	m_automonitor = FALSE;
	m_monitordx = 0;
	m_monitordy = 0;
	m_monitorx = 0;
	m_monitory = 0;
	//}}AFX_DATA_INIT
}


void CDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDisplay)
	DDX_Text(pDX, IDC_AXIS_BLUE, m_axisblue);
	DDV_MinMaxInt(pDX, m_axisblue, 0, 255);
	DDX_Text(pDX, IDC_AXIS_GREEN, m_axisgreen);
	DDV_MinMaxInt(pDX, m_axisgreen, 0, 255);
	DDX_Text(pDX, IDC_AXIS_RED, m_axisred);
	DDV_MinMaxInt(pDX, m_axisred, 0, 255);
	DDX_Text(pDX, IDC_AXISLINES, m_axislines);
	DDV_MinMaxInt(pDX, m_axislines, 0, 1000);
	DDX_Text(pDX, IDC_AXISTEXT, m_axistext);
	DDV_MinMaxInt(pDX, m_axistext, 0, 1000);
	DDX_Text(pDX, IDC_DATA_BLUE, m_datablue);
	DDV_MinMaxInt(pDX, m_datablue, 0, 255);
	DDX_Text(pDX, IDC_DATALINES, m_datalines);
	DDV_MinMaxInt(pDX, m_datalines, 1, 1000);
	DDX_Text(pDX, IDC_DATAXMAX, m_dataxmax);
	DDV_MinMaxInt(pDX, m_dataxmax, 1, 1000);
	DDX_Text(pDX, IDC_DATAXMIN, m_dataxmin);
	DDV_MinMaxInt(pDX, m_dataxmin, 1, 1000);
	DDX_Text(pDX, IDC_DATAYMAX, m_dataymax);
	DDV_MinMaxInt(pDX, m_dataymax, 1, 1000);
	DDX_Text(pDX, IDC_DATAYMIN, m_dataymin);
	DDV_MinMaxInt(pDX, m_dataymin, 1, 1000);
	DDX_Text(pDX, IDC_ERRORBARS, m_errorbars);
	DDV_MinMaxInt(pDX, m_errorbars, 1, 1000);
	DDX_Text(pDX, IDC_FIT_BLUE, m_fitblue);
	DDV_MinMaxInt(pDX, m_fitblue, 0, 255);
	DDX_Text(pDX, IDC_FIT_GREEN, m_fitgreen);
	DDV_MinMaxInt(pDX, m_fitgreen, 0, 255);
	DDX_Text(pDX, IDC_FIT_RED, m_fitred);
	DDV_MinMaxInt(pDX, m_fitred, 0, 255);
	DDX_Text(pDX, IDC_FITFUNRES, m_fitfunres);
	DDV_MinMaxInt(pDX, m_fitfunres, 2, 500);
	DDX_Text(pDX, IDC_FITLINES, m_fitlines);
	DDV_MinMaxInt(pDX, m_fitlines, 1, 1000);
	DDX_Text(pDX, IDC_HALFTICKLENX, m_halfticklenx);
	DDV_MinMaxInt(pDX, m_halfticklenx, 1, 1000);
	DDX_Text(pDX, IDC_NUMBERS, m_numbers);
	DDV_MinMaxInt(pDX, m_numbers, 1, 1000);
	DDX_Text(pDX, IDC_POINTS, m_points);
	DDV_MinMaxInt(pDX, m_points, 1, 1000);
	DDX_Text(pDX, IDC_SELECT_BLUE, m_selectblue);
	DDV_MinMaxInt(pDX, m_selectblue, 0, 255);
	DDX_Text(pDX, IDC_SELECT_GREEN, m_selectgreen);
	DDV_MinMaxInt(pDX, m_selectgreen, 0, 255);
	DDX_Text(pDX, IDC_HALFTICKLENY, m_halftickleny);
	DDV_MinMaxInt(pDX, m_halftickleny, 1, 1000);
	DDX_Text(pDX, IDC_SELECT_RED, m_selectred);
	DDV_MinMaxInt(pDX, m_selectred, 0, 255);
	DDX_Text(pDX, IDC_TEXT_BLUE, m_textblue);
	DDV_MinMaxInt(pDX, m_textblue, 0, 255);
	DDX_Text(pDX, IDC_TEXT_GREEN, m_textgreen);
	DDV_MinMaxInt(pDX, m_textgreen, 0, 255);
	DDX_Text(pDX, IDC_TEXT_RED, m_textred);
	DDV_MinMaxInt(pDX, m_textred, 0, 255);
	DDX_Text(pDX, IDC_TICKLABELSX, m_ticklabelsx);
	DDV_MinMaxInt(pDX, m_ticklabelsx, 1, 1000);
	DDX_Text(pDX, IDC_TICKLABELSY, m_ticklabelsy);
	DDX_Text(pDX, IDC_TICKLENX, m_ticklenx);
	DDV_MinMaxInt(pDX, m_ticklenx, 1, 1000);
	DDX_Text(pDX, IDC_TICKLENY, m_tickleny);
	DDV_MinMaxInt(pDX, m_tickleny, 1, 1000);
	DDX_Text(pDX, IDC_TICKLINES, m_ticklines);
	DDV_MinMaxInt(pDX, m_ticklines, 1, 1000);
	DDX_Text(pDX, IDC_TITLEPOSX, m_titleposx);
	DDV_MinMaxInt(pDX, m_titleposx, 0, 1000);
	DDX_Text(pDX, IDC_TITLEPOSY, m_titleposy);
	DDV_MinMaxInt(pDX, m_titleposy, 0, 1000);
	DDX_Text(pDX, IDC_XAXISTITLEX, m_xaxistitlex);
	DDV_MinMaxInt(pDX, m_xaxistitlex, 0, 1000);
	DDX_Text(pDX, IDC_XAXISTITLEY, m_xaxistitley);
	DDV_MinMaxInt(pDX, m_xaxistitley, 0, 1000);
	DDX_Text(pDX, IDC_YAXISTITLEX, m_yaxistitlex);
	DDV_MinMaxInt(pDX, m_yaxistitlex, 0, 1000);
	DDX_Text(pDX, IDC_YAXISTITLEY, m_yaxistitley);
	DDV_MinMaxInt(pDX, m_yaxistitley, 0, 1000);
	DDX_Text(pDX, IDC_DATA_RED, m_datared);
	DDV_MinMaxInt(pDX, m_datared, 0, 255);
	DDX_Text(pDX, IDC_DATA_GREEN, m_datagreen);
	DDV_MinMaxInt(pDX, m_datagreen, 0, 255);
	DDX_Check(pDX, IDC_AUTOLSI, m_autolsi);
	DDX_Check(pDX, IDC_AUTOPLOT, m_autoplot);
	DDX_Check(pDX, IDC_AUTOSCRIPT, m_autoscript);
	DDX_Check(pDX, IDC_AUTOTEMPLATE, m_autotemplate);
	DDX_Text(pDX, IDC_BACK_BLUE, m_backblue);
	DDV_MinMaxInt(pDX, m_backblue, 0, 255);
	DDX_Text(pDX, IDC_BACK_GREEN, m_backgreen);
	DDV_MinMaxInt(pDX, m_backgreen, 0, 255);
	DDX_Text(pDX, IDC_BACK_RED, m_backred);
	DDV_MinMaxInt(pDX, m_backred, 0, 255);
	DDX_Text(pDX, IDC_LSI_DX, m_lsidx);
	DDV_MinMaxInt(pDX, m_lsidx, 1, 1024);
	DDX_Text(pDX, IDC_LSI_DY, m_lsidy);
	DDV_MinMaxInt(pDX, m_lsidy, 1, 768);
	DDX_Text(pDX, IDC_LSI_X, m_lsix);
	DDV_MinMaxInt(pDX, m_lsix, 1, 1024);
	DDX_Text(pDX, IDC_LSI_Y, m_lsiy);
	DDV_MinMaxInt(pDX, m_lsiy, 1, 768);
	DDX_Text(pDX, IDC_NUMBER_BLUE, m_numberblue);
	DDV_MinMaxInt(pDX, m_numberblue, 0, 255);
	DDX_Text(pDX, IDC_NUMBER_GREEN, m_numbergreen);
	DDV_MinMaxInt(pDX, m_numbergreen, 0, 255);
	DDX_Text(pDX, IDC_NUMBER_RED, m_numberred);
	DDV_MinMaxInt(pDX, m_numberred, 0, 255);
	DDX_Text(pDX, IDC_PLOT_DX, m_plotdx);
	DDV_MinMaxInt(pDX, m_plotdx, 1, 1024);
	DDX_Text(pDX, IDC_PLOT_DY, m_plotdy);
	DDV_MinMaxInt(pDX, m_plotdy, 1, 768);
	DDX_Text(pDX, IDC_PLOT_X, m_plotx);
	DDV_MinMaxInt(pDX, m_plotx, 1, 1024);
	DDX_Text(pDX, IDC_PLOT_Y, m_ploty);
	DDV_MinMaxInt(pDX, m_ploty, 1, 768);
	DDX_Text(pDX, IDC_SCRIPT_DX, m_scriptdx);
	DDV_MinMaxInt(pDX, m_scriptdx, 1, 1024);
	DDX_Text(pDX, IDC_SCRIPT_DY, m_scriptdy);
	DDV_MinMaxInt(pDX, m_scriptdy, 1, 768);
	DDX_Text(pDX, IDC_SCRIPT_X, m_scriptx);
	DDV_MinMaxInt(pDX, m_scriptx, 1, 1024);
	DDX_Text(pDX, IDC_SCRIPT_Y, m_scripty);
	DDV_MinMaxInt(pDX, m_scripty, 1, 1024);
	DDX_Text(pDX, IDC_TEMPLATE_DX, m_templatedx);
	DDV_MinMaxInt(pDX, m_templatedx, 1, 1024);
	DDX_Text(pDX, IDC_TEMPLATE_DY, m_templatedy);
	DDV_MinMaxInt(pDX, m_templatedy, 1, 1024);
	DDX_Text(pDX, IDC_TEMPLATE_X, m_templatex);
	DDV_MinMaxInt(pDX, m_templatex, 1, 1024);
	DDX_Text(pDX, IDC_TEMPLATE_Y, m_templatey);
	DDV_MinMaxInt(pDX, m_templatey, 1, 1024);
	DDX_Check(pDX, IDC_AUTOMONITOR, m_automonitor);
	DDX_Text(pDX, IDC_MONITOR_DX, m_monitordx);
	DDV_MinMaxInt(pDX, m_monitordx, 10, 1024);
	DDX_Text(pDX, IDC_MONITOR_DY, m_monitordy);
	DDV_MinMaxInt(pDX, m_monitordy, 10, 768);
	DDX_Text(pDX, IDC_MONITOR_X, m_monitorx);
	DDV_MinMaxInt(pDX, m_monitorx, 0, 1024);
	DDX_Text(pDX, IDC_MONITOR_Y, m_monitory);
	DDV_MinMaxInt(pDX, m_monitory, 0, 768);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDisplay, CDialog)
	//{{AFX_MSG_MAP(CDisplay)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDisplay message handlers
