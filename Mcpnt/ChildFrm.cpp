// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"

#include "MCP for NT.h"
#include "MeasThread.h"
#include "MinuitChild.h"

#include "minuit.h"

#include "camac.h"

#include "MCP for NTDoc.h"
#include "ActionObjects.h"

#include "DataChild.h"
#include "DataView.h"
#include "MonitorChild.h"
#include "MonitorView.h"
#include "LSIChild.h"
#include "LSIView.h"

#include "PropShtRS.h"
#include "PropShtHV.h"
#include "PropShtHP.h"
#include "PropShtFS.h"
#include "PropShtAM.h"

#include "MCP for NTView.h"
#include "ChildFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_SETPANE WM_USER +13

#define WM_DISABLECLOSE WM_USER +602
#define WM_ENABLECLOSE WM_USER +603

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_COMMAND_EX(CG_ID_VIEW_SAVEDIALOG, OnBarCheck)
	ON_UPDATE_COMMAND_UI(CG_ID_VIEW_SAVEDIALOG, OnUpdateControlBarMenu)
	ON_WM_CREATE()
	//ON_COMMAND_EX(CG_ID_VIEW_MYDIALOGBAR, OnBarCheck)
	//ON_UPDATE_COMMAND_UI(CG_ID_VIEW_MYDIALOGBAR, OnUpdateControlBarMenu)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_COMMAND(ID_SAVESETTINGS, OnSavesettings)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_TEMPLATEPANE,CChildFrame::OnStatus)
	ON_MESSAGE(WM_SETPANE,CChildFrame::OnSetPane)
	ON_MESSAGE(WM_DISABLECLOSE,CChildFrame::OnDisableClose)
	ON_MESSAGE(WM_ENABLECLOSE,CChildFrame::OnEnableClose)
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)

	ON_COMMAND(ID_SAVEFILE, OnSave)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

static UINT indicators[] =
{
	// status line indicator
	//ID_SEPARATOR,
	ID_TEMPLATEPANE
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
};


CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	CloseEnabled = TRUE;

	m_font.CreatePointFont(
	   100,						  // nPointSize
	   _T("MS Sans Serif"));
}

CChildFrame::~CChildFrame()
{
	OnSavesettings();
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();

  	cs.style = WS_CHILD | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	cs.cx=pApp->TemplateDx;
	cs.cy=pApp->TemplateDy;
	if(!pApp->AutoTemplate)
	{
		cs.x=pApp->TemplateX;
		cs.y=pApp->TemplateY;

	}
	else
	{
		if((pApp->LastX+pApp->TemplateDx < 1024)&&
			(pApp->LastY+pApp->TemplateDy < 768))
		{
			pApp->LastX=pApp->LastX+10;
			pApp->LastY=pApp->LastY+10;
		}
		else
		{
			pApp->LastX = 0;
			pApp->LastY = 0;
		}
		cs.x=pApp->LastX;
		cs.y=pApp->LastY;
	}
	return CMDIChildWndEx::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CDockState dockstate;
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_bEnableFloatingBars = true;

	if (!m_ToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), IDR_TOOLBAR1) ||
		!m_ToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_ToolBar.LoadToolBar(IDR_TOOLBAR1);
	m_ToolBar.SetPaneStyle( CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_TOP | CBRS_GRIPPER );
	m_ToolBar.EnableDocking( CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM );
	m_ToolBar.EnableReflections(true);
	EnableDocking(CBRS_ALIGN_ANY);
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetPaneStyle(0,SBPS_STRETCH);

	if (!m_SaveDialog.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1,1,1,1), IDR_TOOLBAR5) ||
		!m_SaveDialog.LoadToolBar(IDR_TOOLBAR5))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_SaveDialog.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_SaveDialog);
	DockPaneLeftOf(&m_ToolBar,&m_SaveDialog);

	m_SaveDialog.ShowWindow(true);
	
	dockstate.LoadState("ChildDockbars");
	SetDockState(dockstate);

	return 0;
}

LRESULT CChildFrame::OnToolbarReset(WPARAM wp,LPARAM)
{
	UINT uiToolBarId = (UINT) wp;

	switch (uiToolBarId)
	{
	case IDR_TOOLBAR5:
		{
		CMFCToolBarEditBoxButton editBox(ID_SAVEFILE, 0, ES_AUTOHSCROLL, 85);
		m_SaveDialog.ReplaceButton (ID_SAVEFILE, editBox);
		}
		break;
	default:
		break;
	}

	return 0;
}

void CChildFrame::OnSavesettings() 
{
	// TODO: Add your command handler code here
	CDockState dockstate;
	CFrameWnd::GetDockState(dockstate);
	dockstate.SaveState("ChildDockbars");
}

void CChildFrame::OnSave()
{
	CString filename;

	CMFCToolBarEditBoxButton* pEditButton = (CMFCToolBarEditBoxButton*) m_SaveDialog.GetButton(1);
	pEditButton->GetEditBox()->GetWindowText(filename);

	((CMCPforNTDoc*)GetActiveView()->GetDocument())->OnSave(filename);
}

void CChildFrame::OnStatus(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

LRESULT CChildFrame::OnSetPane(WPARAM wparam,LPARAM lparam)
{
	CString statusText;
	
	statusText = (char*)lparam;
	m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_TEMPLATEPANE),statusText);
	return TRUE;
}

LRESULT CChildFrame::OnDisableClose(WPARAM wparam,LPARAM lparam)
{
	CloseEnabled = FALSE;
	return TRUE;
}

LRESULT CChildFrame::OnEnableClose(WPARAM wparam,LPARAM lparam)
{
	CloseEnabled = TRUE;
	return TRUE;
}

void CChildFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(CloseEnabled)CMDIChildWndEx::OnClose();
}
