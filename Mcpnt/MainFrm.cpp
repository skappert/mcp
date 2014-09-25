// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "display.h"
#include "camac.h"
#include "sicl.h"

#include "Splash.h"
#include "Calculator.h"
#include "CamacTester.h"
#include "NetReader.h"
#include "NetworkWriter.h"
#include "GpibWriter.h"
#include "GpibReader.h"
#include "SiclReader.h"

#include "MinuitChild.h"
#include "minuit.h"
#include "MCP for NTDoc.h"
#include "ActionObjects.h"

#include "MainFrm.h"

#include "DataChild.h"
#include "DataView.h"
#include "MonitorChild.h"
#include "MonitorView.h"
#include "EditorChild.h"
#include "EditorView.h"
#include "hardware.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_COMMAND(CG_ID_VIEW_ISOLDEPROTONS, OnMeasProtons)
	ON_UPDATE_COMMAND_UI(CG_ID_VIEW_ISOLDEPROTONS, OnUpdateViewProtons)
	ON_COMMAND(CG_ID_VIEW_GPS_HTMEAS, OnMeasHT)
	ON_UPDATE_COMMAND_UI(CG_ID_VIEW_GPS_HTMEAS, OnUpdateViewHT)
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_DATE, OnUpdateDate)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TIME, OnUpdateTime)
	ON_COMMAND_EX(CG_ID_VIEW_LOAD, OnBarCheck)
	//ON_UPDATE_COMMAND_UI(CG_ID_VIEW_LOAD, OnUpdateViewControls)
	ON_COMMAND_EX(CG_ID_VIEW_LOADFILEBAR, OnBarCheck)
	//ON_UPDATE_COMMAND_UI(CG_ID_VIEW_LOADFILEBAR, OnUpdateViewControls)
	//ON_COMMAND_EX(CG_ID_VIEW_LIST, OnBarCheck)
	//ON_UPDATE_COMMAND_UI(CG_ID_VIEW_LIST, OnUpdateControls)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_SAVESETTINGS, OnSavesettings)
	ON_COMMAND(ID_VIEW_TEMPLATECALCULATOR, OnViewTemplatecalculator)
	ON_COMMAND(ID_VIEW_SCRIPTWINDOW, OnViewScriptwindow)
	ON_COMMAND(ID_VIEW_EMPTYPLOT, OnViewEmptyplot)
	ON_COMMAND(ID_VIEW_DISPLAYSETUP, OnViewDisplaysetup)
	ON_COMMAND(ID_VIEW_CAMACTESTER, OnViewCamactester)
	ON_COMMAND(ID_VIEW_NETREADER, OnViewNetreader)
	ON_COMMAND(ID_VIEW_SICLREADER, OnSiclreader)
	ON_COMMAND(ID_MENU_GPIBWRITER, OnGpibwriter)
	ON_COMMAND(ID_MENU_GPIBREADER, OnGpibreader)
	ON_COMMAND(ID_RESET_TB, OnReset)
	ON_COMMAND(ID_VIEW_NETWRITER, OnViewNetwriter)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_CBN_SELCHANGE(IDC_LOADCOMBO, OnSelChange)
	ON_CBN_DROPDOWN(IDC_LOADCOMBO, OnDropDown)
	ON_CBN_SETFOCUS(IDC_LOADCOMBO, OnGetFocus)
	ON_CBN_KILLFOCUS(IDC_LOADCOMBO, OnKillFocus)
	ON_COMMAND(ID_HELP_FINDER, CMDIFrameWndEx::OnHelpFinder)
	ON_COMMAND(ID_HELP, CMDIFrameWndEx::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWndEx::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWndEx::OnHelpFinder)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

void CMainFrame::OnUpdateViewProtons(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_ShowISOLDE_PROTONS);
}

void CMainFrame::OnUpdateViewHT(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_ShowGPS_HTMEAS);
}

void CMainFrame::OnMeasHT()
{
	m_ShowGPS_HTMEAS = !m_ShowGPS_HTMEAS;
	ShowPane(&m_wndGPS_HTMEAS, m_ShowGPS_HTMEAS, FALSE, TRUE); 
	if (!m_ShowGPS_HTMEAS) RecalcLayout();
	m_wndClientArea.Invalidate();
}

void CMainFrame::OnMeasProtons()
{
	m_ShowISOLDE_PROTONS = !m_ShowISOLDE_PROTONS;
	ShowPane(&m_wndISOLDE_PROTONS, m_ShowISOLDE_PROTONS, FALSE, TRUE); 
	if (!m_ShowISOLDE_PROTONS) RecalcLayout();
	m_wndClientArea.Invalidate();
}

CMainFrame::CMainFrame()
{
	CMCPforNTApp* pApp	= (CMCPforNTApp*)AfxGetApp();
	pApp->NetActive		= TRUE;
	LoadFocus			= FALSE;
	SaveFocus			= FALSE;
	IconToggle			= FALSE;
	HTBACK				= TRUE;
	IconToggle2			= FALSE;
	PROTONSBACK			= TRUE;
}

CMainFrame::~CMainFrame()
{
	DataReset();
	ListReset();
	OnSavesettings();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// CG: This line was added by the Palette Support component
	CDockState dockstate;

	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
	CDockingManager::SetDockingMode(DT_SMART);
#if 0
	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | 
                          CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY );

	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
#endif

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), IDW_USER_TOOLBAR + 1) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME_TOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.SetSizes(CSize(30,20),CSize(27,15));

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndToolBar);

	// CG: The following block was inserted by the 'Dialog Bar' component

	// TODO: Add a menu item that will toggle the visibility of the
	// dialog bar named "LoadFileBar":
	//   1. In ResourceView, open the menu resource that is used by
	//      the CMainFrame class
	//   2. Select the View submenu
	//   3. Double-click on the blank item at the bottom of the submenu
	//   4. Assign the new item an ID: CG_ID_VIEW_LOADFILEBAR
	//   5. Assign the item a Caption: LoadFileBar

	// TODO: Change the value of CG_ID_VIEW_LOADFILEBAR to an appropriate value:
	//   1. Open the file resource.h
	//   2. Find the definition for the symbol CG_ID_VIEW_LOADFILEBAR
	//   3. Change the value of the symbol. Use a value in the range
	//      0xE804 to 0xE81A that is not already used by another symbol

	// CG: The following block was inserted by the 'Dialog Bar' component
	{
		// Initialize dialog bar m_wndLoadFileBar
		if (!m_wndLoadFileBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), IDW_USER_TOOLBAR + 2))
		{
			TRACE0("Failed to create dialog bar m_wndLoadFileBar\n");
			return -1;		// fail to create
		}
		m_wndLoadFileBar.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
		EnableDocking(CBRS_ALIGN_ANY);
		DockPane(&m_wndLoadFileBar);
		m_wndLoadFileBar.ShowWindow( true );
	}

	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	//DockControlBar(&m_wndToolBar);
	// CG: The following block was inserted by 'Status Bar' component.
	{
		// Find out the size of the static variable 'indicators' defined
		// by AppWizard and copy it
		int nOrigSize = sizeof(indicators) / sizeof(UINT);

		UINT* pIndicators = new UINT[nOrigSize + 2];
		memcpy(pIndicators, indicators, sizeof(indicators));

		// Call the Status Bar Component's status bar creation function
		if (!InitStatusBar(pIndicators, nOrigSize, 60))
		{
			TRACE0("Failed to initialize Status Bar\n");
			return -1;
		}
		delete[] pIndicators;
	}

	CSplashWnd::ShowSplashScreen(this);

	{
		
		if (!m_wndGPS_HTMEAS.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), IDW_USER_TOOLBAR + 3))
		{
			TRACE0("Failed to create dialog bar m_wndGPS_HTMEAS\n");
			return -1;		// fail to create
		}
		
		m_wndGPS_HTMEAS.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
		EnableDocking(CBRS_ALIGN_ANY);
		DockPane(&m_wndGPS_HTMEAS);
	}
	{
		if (!m_wndISOLDE_PROTONS.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), IDW_USER_TOOLBAR + 4))
		{
			TRACE0("Failed to create dialog bar m_wndISOLDE_PROTONS\n");
			return -1;		// fail to create
		}

		m_wndISOLDE_PROTONS.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
		EnableDocking(CBRS_ALIGN_ANY);
		DockPane(&m_wndISOLDE_PROTONS);
	}
	dockstate.LoadState("Dockbars");
	SetDockState(dockstate);

#if 0
	INST dvm;
	double res;
	int i;
	char buf[256];

	ionerror (I_ERROR_EXIT);

	dvm = iopen ("lan[A-34461A-06386]:inst0");

	if( dvm > 0 )
	{
		itimeout (dvm, 1000);
		
		//iprintf(dvm, "*RST\n");

		ipromptf(dvm, "*IDN?\n", "%t", buf);
		OutputDebugString(buf);
		OutputDebugString("\n");

		for (i=1;i<=2;i++)
		{
			OutputDebugString("measuring\n");

			/* Take a measurement */
			iprintf (dvm,"MEAS:VOLT:DC?\n");

			/* Read the results */
			iscanf (dvm,"%lf", &res);

			/* Print the results */
			sprintf (buf, "Result is %f\n",res);

			OutputDebugString(buf);
			OutputDebugString("\n");
		}

		iclose (dvm);
	}
#endif

	return 0;
}

void CMainFrame::OnGetFocus()
{
	LoadFocus=TRUE;
}

void CMainFrame::OnKillFocus()
{
	LoadFocus=FALSE;
}

void CMainFrame::OnSelChange()
{
	CString FileName;
	CComboBox* pLoadFile = (CComboBox*)m_wndLoadFileBar.GetDlgItem(IDC_LOADCOMBO);
	if (CB_ERR!=pLoadFile->GetCurSel())
	{
		pLoadFile->GetLBText(pLoadFile->GetCurSel(),FileName);
		AfxGetApp()->OpenDocumentFile(FileName);
	}
}

void CMainFrame::OnDropDown()
{
	CString FileName;
	CComboBox* pLoadFile = (CComboBox*)m_wndLoadFileBar.GetDlgItem(IDC_LOADCOMBO);
	pLoadFile->GetWindowText(FileName);
	pLoadFile->ResetContent();
	pLoadFile->Dir(DDL_READWRITE,"*.*");
	pLoadFile->SetWindowText(FileName);
}



BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX|WS_MAXIMIZE;
	cs.x=0;
	cs.y=0;
	cs.cx=1024;
	cs.cy=740;

	return CMDIFrameWndEx::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnSavesettings() 
{
	// TODO: Add your command handler code here
	CDockState dockstate;
	CFrameWnd::GetDockState(dockstate);
	dockstate.SaveState("Dockbars");
}

void CMainFrame::OnReturn() 
{
	// TODO: Add your command handler code here
	CString FileName;
	if (LoadFocus)
	{
		CComboBox* pLoadFile = (CComboBox*)m_wndLoadFileBar.GetDlgItem(IDC_LOADCOMBO);
		pLoadFile->GetWindowText(FileName);
		AfxGetApp()->OpenDocumentFile(FileName);
	}
}

void CMainFrame::OnViewTemplatecalculator() 
{
	// TODO: Add your command handler code here
	CCalculator* pdlg = new CCalculator();
	pdlg->Create(IDD_TEMPLATECALCUL,NULL);
}

void CMainFrame::OnUpdateDate(CCmdUI* pCmdUI)
{
	// CG: This function was inserted by 'Status Bar' component.

	// Get current date and format it
	CTime time = CTime::GetCurrentTime();
	CString strDate = time.Format(_T("%x"));

	// BLOCK: compute the width of the date string
	CSize size;
	{
		HGDIOBJ hOldFont = NULL;
		HFONT hFont = (HFONT)m_wndStatusBar.SendMessage(WM_GETFONT);
		CClientDC dc(NULL);
		if (hFont != NULL) 
			hOldFont = dc.SelectObject(hFont);
		size = dc.GetTextExtent(strDate);
		if (hOldFont != NULL) 
			dc.SelectObject(hOldFont);
	}

	// Update the pane to reflect the current date
	UINT nID, nStyle;
	int nWidth;
	m_wndStatusBar.GetPaneInfo(m_nDatePaneNo, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(m_nDatePaneNo, nID, nStyle, size.cx);
	pCmdUI->SetText(strDate);
	pCmdUI->Enable(TRUE);

}

void CMainFrame::OnUpdateTime(CCmdUI* pCmdUI)
{
	// CG: This function was inserted by 'Status Bar' component.

	// Get current date and format it
	CTime time = CTime::GetCurrentTime();
	CString strTime = time.Format(_T("%X"));

	// BLOCK: compute the width of the date string
	CSize size;
	{
		HGDIOBJ hOldFont = NULL;
		HFONT hFont = (HFONT)m_wndStatusBar.SendMessage(WM_GETFONT);
		CClientDC dc(NULL);
		if (hFont != NULL) 
			hOldFont = dc.SelectObject(hFont);
		size = dc.GetTextExtent(strTime);
		if (hOldFont != NULL) 
			dc.SelectObject(hOldFont);
	}

	// Update the pane to reflect the current time
	UINT nID, nStyle;
	int nWidth;
	m_wndStatusBar.GetPaneInfo(m_nTimePaneNo, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(m_nTimePaneNo, nID, nStyle, size.cx);
	pCmdUI->SetText(strTime);
	pCmdUI->Enable(TRUE);

}

BOOL CMainFrame::InitStatusBar(UINT *pIndicators, int nSize, int nSeconds)
{
	// CG: This function was inserted by 'Status Bar' component.

	// Create an index for the DATE pane
	m_nDatePaneNo = nSize++;
	pIndicators[m_nDatePaneNo] = ID_INDICATOR_DATE;
	// Create an index for the TIME pane
	m_nTimePaneNo = nSize++;
	nSeconds = 1;
	pIndicators[m_nTimePaneNo] = ID_INDICATOR_TIME;

	// TODO: Select an appropriate time interval for updating
	// the status bar when idling.
	m_wndStatusBar.SetTimer(0x1000, nSeconds * 1000, NULL);

	return m_wndStatusBar.SetIndicators(pIndicators, nSize);

}

void CMainFrame::OnViewScriptwindow() 
{
	// TODO: Add your command handler code here
	CEditorView* pEdit = new CEditorView();
}


void CMainFrame::OnViewEmptyplot() 
{
	// TODO: Add your command handler code here
	DataView* pDataView = new DataView();
}

void CMainFrame::OnViewDisplaysetup() 
{
	// TODO: Add your command handler code here
	CDisplay* pDisplay = new CDisplay;

	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	
	pDisplay->m_axisblue		= pApp->GetProfileInt("Display","axisblue",		0);
	pDisplay->m_axisgreen		= pApp->GetProfileInt("Display","axisgreen",	0);
	pDisplay->m_axisred			= pApp->GetProfileInt("Display","axisred",		0);
	pDisplay->m_axislines		= pApp->GetProfileInt("Display","axislines",	4);
	pDisplay->m_axistext		= pApp->GetProfileInt("Display","axistext",		32);
	pDisplay->m_datablue		= pApp->GetProfileInt("Display","datablue",		0);
	pDisplay->m_datared			= pApp->GetProfileInt("Display","datared",		0);
	pDisplay->m_datagreen		= pApp->GetProfileInt("Display","datagreen",	0);
	pDisplay->m_datalines		= pApp->GetProfileInt("Display","datalines",	2);
	pDisplay->m_dataxmax		= pApp->GetProfileInt("Display","dataxmax",		980);
	pDisplay->m_dataxmin		= pApp->GetProfileInt("Display","dataxmin",		100);
	pDisplay->m_dataymax		= pApp->GetProfileInt("Display","dataymax",		900);
	pDisplay->m_dataymin		= pApp->GetProfileInt("Display","dataymin",		20);
	pDisplay->m_errorbars		= pApp->GetProfileInt("Display","errorbars",	5);
	pDisplay->m_fitblue			= pApp->GetProfileInt("Display","fitblue",		250);
	pDisplay->m_fitgreen		= pApp->GetProfileInt("Display","fitgreen",		0);
	pDisplay->m_fitred			= pApp->GetProfileInt("Display","fitred",		0);
	pDisplay->m_fitfunres		= pApp->GetProfileInt("Display","fitfunres",	250);
	pDisplay->m_fitlines		= pApp->GetProfileInt("Display","fitlines",		2);
	pDisplay->m_halfticklenx	= pApp->GetProfileInt("Display","halfticklenx",	8);
	pDisplay->m_numbers			= pApp->GetProfileInt("Display","numbers",		28);
	pDisplay->m_points			= pApp->GetProfileInt("Display","points",		5);
	pDisplay->m_selectblue		= pApp->GetProfileInt("Display","selectblue",	0);
	pDisplay->m_selectgreen		= pApp->GetProfileInt("Display","selectgreen",	0);
	pDisplay->m_halftickleny	= pApp->GetProfileInt("Display","halftickleny",	5);
	pDisplay->m_selectred		= pApp->GetProfileInt("Display","selectred",	250);
	pDisplay->m_textblue		= pApp->GetProfileInt("Display","textblue",		0);
	pDisplay->m_textgreen		= pApp->GetProfileInt("Display","textgreen",	0);
	pDisplay->m_textred			= pApp->GetProfileInt("Display","textred",		0);
	pDisplay->m_ticklabelsx		= pApp->GetProfileInt("Display","ticklabelsx",	905);
	pDisplay->m_ticklabelsy		= pApp->GetProfileInt("Display","ticklabelsy",	95);
	pDisplay->m_ticklenx		= pApp->GetProfileInt("Display","ticklenx",		15);
	pDisplay->m_tickleny		= pApp->GetProfileInt("Display","tickleny",		10);
	pDisplay->m_ticklines		= pApp->GetProfileInt("Display","ticklines",	3);
	pDisplay->m_titleposx		= pApp->GetProfileInt("Display","titleposx",	500);
	pDisplay->m_titleposy		= pApp->GetProfileInt("Display","titleposy",	100);
	pDisplay->m_xaxistitlex		= pApp->GetProfileInt("Display","xaxistitlex",	500);
	pDisplay->m_xaxistitley		= pApp->GetProfileInt("Display","xaxistitley",	970);
	pDisplay->m_yaxistitlex		= pApp->GetProfileInt("Display","yaxistitlex",	30);
	pDisplay->m_yaxistitley		= pApp->GetProfileInt("Display","yaxistitley",	500);

	pDisplay->m_autolsi			= pApp->GetProfileInt("Display","autolsi",		TRUE);
	pDisplay->m_autoplot		= pApp->GetProfileInt("Display","autoplot",		TRUE);
	pDisplay->m_automonitor		= pApp->GetProfileInt("Display","automonitor",	FALSE);
	pDisplay->m_autoscript		= pApp->GetProfileInt("Display","autoscript",	TRUE);
	pDisplay->m_autotemplate	= pApp->GetProfileInt("Display","autotemplate",	TRUE);
	
	pDisplay->m_backblue		= pApp->GetProfileInt("Display","backblue",		255);
	pDisplay->m_backgreen		= pApp->GetProfileInt("Display","backgreen",	255);
	pDisplay->m_backred			= pApp->GetProfileInt("Display","backred",		255);
	
	pDisplay->m_numberblue		= pApp->GetProfileInt("Display","numberblue",	0);
	pDisplay->m_numbergreen		= pApp->GetProfileInt("Display","numbergreen",	0);
	pDisplay->m_numberred		= pApp->GetProfileInt("Display","numberred",	0);

	pDisplay->m_lsidx			= pApp->GetProfileInt("Display","lsidx",		500);
	pDisplay->m_lsidy			= pApp->GetProfileInt("Display","lsidy",		300);
	pDisplay->m_lsix			= pApp->GetProfileInt("Display","lsix",			100);
	pDisplay->m_lsiy			= pApp->GetProfileInt("Display","lsiy",			100);

	pDisplay->m_plotdx			= pApp->GetProfileInt("Display","plotdx",		600);
	pDisplay->m_plotdy			= pApp->GetProfileInt("Display","plotdy",		400);
	pDisplay->m_plotx			= pApp->GetProfileInt("Display","plotx",		300);
	pDisplay->m_ploty			= pApp->GetProfileInt("Display","ploty",		10);

	pDisplay->m_monitordx		= pApp->GetProfileInt("Display","monitordx",	600);
	pDisplay->m_monitordy		= pApp->GetProfileInt("Display","monitordy",	400);
	pDisplay->m_monitorx		= pApp->GetProfileInt("Display","monitorx",		300);
	pDisplay->m_monitory		= pApp->GetProfileInt("Display","monitory",		10);

	pDisplay->m_scriptdx		= pApp->GetProfileInt("Display","scriptdx",		550);
	pDisplay->m_scriptdy		= pApp->GetProfileInt("Display","scriptdy",		500);
	pDisplay->m_scriptx			= pApp->GetProfileInt("Display","scriptx",		300);
	pDisplay->m_scripty			= pApp->GetProfileInt("Display","scripty",		100);

	pDisplay->m_templatedx		= pApp->GetProfileInt("Display","templatedx",	335);
	pDisplay->m_templatedy		= pApp->GetProfileInt("Display","templatedy",	350);
	pDisplay->m_templatex		= pApp->GetProfileInt("Display","templatex",	30);
	pDisplay->m_templatey		= pApp->GetProfileInt("Display","templatey",	30);

	if(IDOK == pDisplay->DoModal())
	{	
		pApp->WriteProfileInt("Display","axisblue",		pDisplay->m_axisblue);
		pApp->AxisBlue		= pDisplay->m_axisblue;
		pApp->WriteProfileInt("Display","axisgreen",	pDisplay->m_axisgreen);
		pApp->AxisGreen		= pDisplay->m_axisgreen;
		pApp->WriteProfileInt("Display","axisred",		pDisplay->m_axisred);
		pApp->AxisRed		= pDisplay->m_axisred;
		pApp->WriteProfileInt("Display","axislines",	pDisplay->m_axislines);
		pApp->AxisLine		= (float)pDisplay->m_axislines/1000;
		pApp->WriteProfileInt("Display","axistext",		pDisplay->m_axistext);
		pApp->AxisTextSize	= (float)pDisplay->m_axistext/1000;
		pApp->WriteProfileInt("Display","datablue",		pDisplay->m_datablue);
		pApp->DataBlue		= pDisplay->m_datablue;
		pApp->WriteProfileInt("Display","datared",		pDisplay->m_datared);
		pApp->DataGreen		= pDisplay->m_datagreen;
		pApp->WriteProfileInt("Display","datagreen",	pDisplay->m_datagreen);
		pApp->DataRed		= pDisplay->m_datared;
		pApp->WriteProfileInt("Display","datalines",	pDisplay->m_datalines);
		pApp->DataLine		= (float)pDisplay->m_datalines/1000;
		pApp->WriteProfileInt("Display","dataxmax",		pDisplay->m_dataxmax);
		pApp->DataxMax		= (float)pDisplay->m_dataxmax/1000;
		pApp->WriteProfileInt("Display","dataxmin",		pDisplay->m_dataxmin);
		pApp->DataxMin		= (float)pDisplay->m_dataxmin/1000;
		pApp->WriteProfileInt("Display","dataymax",		pDisplay->m_dataymax);
		pApp->DatayMax		= (float)pDisplay->m_dataymax/1000;
		pApp->WriteProfileInt("Display","dataymin",		pDisplay->m_dataymin);
		pApp->DatayMin		= (float)pDisplay->m_dataymin/1000;
		pApp->WriteProfileInt("Display","errorbars",	pDisplay->m_errorbars);
		pApp->ErrorBarSize	= (float)pDisplay->m_errorbars/1000;
		pApp->WriteProfileInt("Display","fitblue",		pDisplay->m_fitblue);
		pApp->FitBlue		= pDisplay->m_fitblue;
		pApp->WriteProfileInt("Display","fitgreen",		pDisplay->m_fitgreen);
		pApp->FitGreen		= pDisplay->m_fitgreen;
		pApp->WriteProfileInt("Display","fitred",		pDisplay->m_fitred);
		pApp->FitRed		= pDisplay->m_fitred;
		pApp->WriteProfileInt("Display","fitfunres",	pDisplay->m_fitfunres);
		pApp->FitFunRes		= pDisplay->m_fitfunres;
		pApp->WriteProfileInt("Display","fitlines",		pDisplay->m_fitlines);
		pApp->FitFunc		= (float)pDisplay->m_fitlines/1000;
		pApp->WriteProfileInt("Display","halfticklenx",	pDisplay->m_halfticklenx);
		pApp->HalfTickLenX	= (float)pDisplay->m_halfticklenx/1000;
		pApp->WriteProfileInt("Display","numbers",		pDisplay->m_numbers);
		pApp->NumberSize	= (float)pDisplay->m_numbers/1000;
		pApp->WriteProfileInt("Display","points",		pDisplay->m_points);
		pApp->PointSize		= (float)pDisplay->m_points/1000;
		pApp->WriteProfileInt("Display","selectblue",	pDisplay->m_selectblue);
		pApp->SelectBlue	= pDisplay->m_selectblue;
		pApp->WriteProfileInt("Display","selectgreen",	pDisplay->m_selectgreen);
		pApp->SelectGreen	= pDisplay->m_selectgreen;
		pApp->WriteProfileInt("Display","selectred",	pDisplay->m_selectred);
		pApp->SelectRed		= pDisplay->m_selectred;
		pApp->WriteProfileInt("Display","textblue",		pDisplay->m_textblue);
		pApp->TextBlue		= pDisplay->m_textblue;
		pApp->WriteProfileInt("Display","textgreen",	pDisplay->m_textgreen);
		pApp->TextGreen		= pDisplay->m_textgreen;
		pApp->WriteProfileInt("Display","textred",		pDisplay->m_textred);
		pApp->TextRed		= pDisplay->m_textred;
		pApp->WriteProfileInt("Display","halftickleny",	pDisplay->m_halftickleny);
		pApp->HalfTickLenY	= (float)pDisplay->m_halftickleny/1000;
		pApp->WriteProfileInt("Display","ticklabelsx",	pDisplay->m_ticklabelsx);
		pApp->TickLabelsX	= (float)pDisplay->m_ticklabelsx/1000;
		pApp->WriteProfileInt("Display","ticklabelsy",	pDisplay->m_ticklabelsy);
		pApp->TickLabelsY	= (float)pDisplay->m_ticklabelsy/1000;
		pApp->WriteProfileInt("Display","ticklenx",		pDisplay->m_ticklenx);
		pApp->TickLenX		= (float)pDisplay->m_ticklenx/1000;
		pApp->WriteProfileInt("Display","tickleny",		pDisplay->m_tickleny);
		pApp->TickLenY		= (float)pDisplay->m_tickleny/1000;
		pApp->WriteProfileInt("Display","ticklines",	pDisplay->m_ticklines);
		pApp->TickLine		= (float)pDisplay->m_ticklines/1000;
		pApp->WriteProfileInt("Display","titleposx",	pDisplay->m_titleposx);
		pApp->MainTitleX	= (float)pDisplay->m_titleposx/1000;
		pApp->WriteProfileInt("Display","titleposy",	pDisplay->m_titleposy);
		pApp->MainTitleY	= (float)pDisplay->m_titleposy/1000;
		pApp->WriteProfileInt("Display","xaxistitlex",	pDisplay->m_xaxistitlex);
		pApp->XTitleX		= (float)pDisplay->m_xaxistitlex/1000;
		pApp->WriteProfileInt("Display","xaxistitley",	pDisplay->m_xaxistitley);
		pApp->XTitleY		= (float)pDisplay->m_xaxistitley/1000;
		pApp->WriteProfileInt("Display","yaxistitlex",	pDisplay->m_yaxistitlex);
		pApp->YTitleX		= (float)pDisplay->m_yaxistitlex/1000;
		pApp->WriteProfileInt("Display","yaxistitley",	pDisplay->m_yaxistitley);
		pApp->YTitleY		= (float)pDisplay->m_yaxistitley/1000;

		pApp->WriteProfileInt("Display","autolsi",pDisplay->m_autolsi);
		pApp->WriteProfileInt("Display","autoplot",pDisplay->m_autoplot);
		pApp->WriteProfileInt("Display","automonitor",pDisplay->m_automonitor);
		pApp->WriteProfileInt("Display","autoscript",pDisplay->m_autoscript);
		pApp->WriteProfileInt("Display","autotemplate",pDisplay->m_autotemplate);
	
		pApp->WriteProfileInt("Display","backblue",pDisplay->m_backblue);
		pApp->WriteProfileInt("Display","backgreen",pDisplay->m_backgreen);
		pApp->WriteProfileInt("Display","backred",pDisplay->m_backred);
	
		pApp->WriteProfileInt("Display","numberblue",pDisplay->m_numberblue);
		pApp->WriteProfileInt("Display","numbergreen",pDisplay->m_numbergreen);
		pApp->WriteProfileInt("Display","numberred",pDisplay->m_numberred);

		pApp->WriteProfileInt("Display","lsidx",pDisplay->m_lsidx);	
		pApp->WriteProfileInt("Display","lsidy",	pDisplay->m_lsidy);
		pApp->WriteProfileInt("Display","lsix",pDisplay->m_lsix);
		pApp->WriteProfileInt("Display","lsiy",pDisplay->m_lsiy);

		pApp->WriteProfileInt("Display","plotdx",pDisplay->m_plotdx);
		pApp->WriteProfileInt("Display","plotdy",pDisplay->m_plotdy);
		pApp->WriteProfileInt("Display","plotx",pDisplay->m_plotx	);
		pApp->WriteProfileInt("Display","ploty",	pDisplay->m_ploty);

		pApp->WriteProfileInt("Display","monitordx",pDisplay->m_monitordx);
		pApp->WriteProfileInt("Display","monitordy",pDisplay->m_monitordy);
		pApp->WriteProfileInt("Display","monitorx",pDisplay->m_monitorx	);
		pApp->WriteProfileInt("Display","monitory",	pDisplay->m_monitory);

		pApp->WriteProfileInt("Display","scriptdx",pDisplay->m_scriptdx);
		pApp->WriteProfileInt("Display","scriptdy",pDisplay->m_scriptdy);
		pApp->WriteProfileInt("Display","scriptx",pDisplay->m_scriptx);	
		pApp->WriteProfileInt("Display","scripty",pDisplay->m_scripty);	

		pApp->WriteProfileInt("Display","templatedx",pDisplay->m_templatedx);
		pApp->WriteProfileInt("Display","templatedy",pDisplay->m_templatedy);
		pApp->WriteProfileInt("Display","templatex",pDisplay->m_templatex);	
		pApp->WriteProfileInt("Display","templatey",pDisplay->m_templatey);
		
		pApp->AutoLsi		= pDisplay->m_autolsi;
		pApp->AutoPlot		= pDisplay->m_autoplot;
		pApp->AutoMonitor	= pDisplay->m_automonitor;
		pApp->AutoScript	= pDisplay->m_autoscript;
		pApp->AutoTemplate	= pDisplay->m_autotemplate;
	
		pApp->BackBlue		= pDisplay->m_backblue;
		pApp->BackGreen		= pDisplay->m_backgreen;
		pApp->BackRed		= pDisplay->m_backred;
	
		pApp->NumberBlue	= pDisplay->m_numberblue;
		pApp->NumberGreen	= pDisplay->m_numbergreen;
		pApp->NumberRed		= pDisplay->m_numberred;

		pApp->LsiDx			= pDisplay->m_lsidx;	
		pApp->LsiDy			= pDisplay->m_lsidy;
		pApp->LsiX			= pDisplay->m_lsix;
		pApp->LsiY			= pDisplay->m_lsiy;

		pApp->PlotDx		= pDisplay->m_plotdx;
		pApp->PlotDy		= pDisplay->m_plotdy;
		pApp->PlotX			= pDisplay->m_plotx;
		pApp->PlotY			= pDisplay->m_ploty;

		pApp->MonitorDx		= pDisplay->m_monitordx;
		pApp->MonitorDy		= pDisplay->m_monitordy;
		pApp->MonitorX		= pDisplay->m_monitorx;
		pApp->MonitorY		= pDisplay->m_monitory;

		pApp->ScriptDx		= pDisplay->m_scriptdx;
		pApp->ScriptDy		= pDisplay->m_scriptdy;
		pApp->ScriptX		= pDisplay->m_scriptx;	
		pApp->ScriptY		= pDisplay->m_scripty;	

		pApp->TemplateDx	= pDisplay->m_templatedx;
		pApp->TemplateDy	= pDisplay->m_templatedy;
		pApp->TemplateX		= pDisplay->m_templatex;	
		pApp->TemplateY		= pDisplay->m_templatey;
	}
	delete pDisplay;
	

}


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	{
		// Shift+F10: show pop-up menu.
		if (pMsg->message == WM_KEYDOWN)
		{// If we hit a key and
			if(pMsg->wParam == VK_RETURN)		//Natural keyboard key
			{
				OnReturn();
			}
		}
	}
	return CMDIFrameWndEx::PreTranslateMessage(pMsg);
}

void CMainFrame::OnViewCamactester() 
{
	// TODO: Add your command handler code here
	CCamacTester* pdlg = new CCamacTester();
	pdlg->Create(IDD_CAMACSEND,NULL);
}


void CMainFrame::OnViewNetreader() 
{
	// TODO: Add your command handler code here
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	if(pApp->NetActive)
	{
		CNetReader* pdlg = new CNetReader();
		pdlg->Create(IDD_NETREAD,NULL);
	}
	else Beep(1000,100);
}

void CMainFrame::OnViewNetwriter() 
{
	// TODO: Add your command handler code here
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	if(pApp->NetActive)
	{
		CNetworkWriter* pdlg = new CNetworkWriter();
		pdlg->Create(IDD_NETWRITE,NULL);
	}
	else Beep(1000,100);	
}

void CMainFrame::OnGpibwriter() 
{
	// TODO: Add your command handler code here
	CGpibWriter* pdlg = new CGpibWriter();
	pdlg->Create(IDD_GPIBWRITE,NULL);
}

void CMainFrame::OnGpibreader() 
{
	// TODO: Add your command handler code here
	CGpibReader* pdlg = new CGpibReader();
	pdlg->Create(IDD_GPIBREAD,NULL);
}

void CMainFrame::OnSiclreader() 
{
	CSiclReader* pdlg = new CSiclReader();
	pdlg->Create(IDD_SICLREAD,NULL);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	short IDs[20];
	double Data[20];
	char  User[255];
	CString GPS_Voltage;
	CString Isolde_Protons;
	USHORT i;

	//if (GetRPCHotlinkIDs(&IDs[0])==0)
	if(0)
	{
		CMDIFrameWndEx::OnSize(nType, cx, cy);
	}
	else
	{
		i=0;
		do
		{
			//GetRPCHotlinkData(IDs[i],&Data[i],sizeof(double),CF_DOUBLE,&Cycle,&User[0]);
			i++;
		}
		while(IDs[i]!=0);

		for(i=0;i<2;i++)
		{
			if(IDs[i]==GPS_HTMEAS)
			{
				GPS_Voltage.Format("%.2f V",Data[i]*1000);
				if(Data[i] < 60.030||Data[i] > 60.065)
				{
					CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
					CStatic* pStat = (CStatic*)m_wndGPS_HTMEAS.GetDlgItem(IDC_HTMEASICON);
					if(IconToggle==TRUE)
					{
						pStat->SetIcon(pApp->LoadIcon(IDI_ICON4));
						IconToggle	= FALSE;
						HTBACK		= TRUE;
					}
					else
					{
						pStat->SetIcon(pApp->LoadIcon(IDI_ICON3));
						IconToggle	= TRUE;
						HTBACK		= TRUE;
					}
				}
				else
				{
					if(HTBACK)
					{
						CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
						CStatic* pStat = (CStatic*)m_wndGPS_HTMEAS.GetDlgItem(IDC_HTMEASICON);
						HTBACK		= FALSE;
						pStat->SetIcon(pApp->LoadIcon(IDI_ICON1));
					}
				}
				m_wndGPS_HTMEAS.GetDlgItem(IDC_GPS_HTMEAS)->SetWindowText(GPS_Voltage);
			}
		}

		for(i=0;i<2;i++)
		{
			if(IDs[i]==ISOLDE_PROTONS&&(strcmp(User,"ISOGPS")==0||strcmp(User,"ISOHRS")==0))
			{
				Isolde_Protons.Format("%.3fe13",Data[i]/1000);
				if(Data[i] < 1000||Data[i] > 4000)
				{
					CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
					CStatic* pStat = (CStatic*)m_wndISOLDE_PROTONS.GetDlgItem(IDC_PROTONICON);
					if(IconToggle2==TRUE)
					{
						pStat->SetIcon(pApp->LoadIcon(IDI_ICON4));
						IconToggle2	= FALSE;
						PROTONSBACK	= TRUE;
					}
					else
					{
						pStat->SetIcon(pApp->LoadIcon(IDI_ICON3));
						IconToggle2	= TRUE;
						PROTONSBACK	= TRUE;
					}
				}
				else
				{
					if(PROTONSBACK)
					{
						CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
						CStatic* pStat = (CStatic*)m_wndISOLDE_PROTONS.GetDlgItem(IDC_PROTONICON);
						PROTONSBACK	= FALSE;
						pStat->SetIcon(pApp->LoadIcon(IDI_ICON1));
					}
				}
				m_wndISOLDE_PROTONS.GetDlgItem(IDC_GPS_PROTONS)->SetWindowText(Isolde_Protons);
			}
		}
	}
}


void CMainFrame::OnReset() 
{
	CamacReset();
}

