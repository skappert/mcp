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

#define WM_SAVEIT WM_USER +12
#define WM_SETPANE WM_USER +13

#define WM_DISABLECLOSE WM_USER +602
#define WM_ENABLECLOSE WM_USER +603

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_COMMAND_EX(CG_ID_VIEW_SAVEDIALOG, OnBarCheck)
	ON_UPDATE_COMMAND_UI(CG_ID_VIEW_SAVEDIALOG, OnUpdateControlBarMenu)
	ON_WM_CREATE()
	//ON_COMMAND_EX(CG_ID_VIEW_MYDIALOGBAR, OnBarCheck)
	//ON_UPDATE_COMMAND_UI(CG_ID_VIEW_MYDIALOGBAR, OnUpdateControlBarMenu)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_COMMAND(ID_SAVESETTINGS, OnSavesettings)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	//ON_UPDATE_COMMAND_UI(ID_TEMPLATEPANE,OnStatus)
	ON_MESSAGE(WM_SETPANE,CChildFrame::OnSetPane)
	ON_MESSAGE(WM_DISABLECLOSE,CChildFrame::OnDisableClose)
	ON_MESSAGE(WM_ENABLECLOSE,CChildFrame::OnEnableClose)

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
}

CChildFrame::~CChildFrame()
{
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
	return CMDIChildWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CDockState dockstate;
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add a menu item that will toggle the visibility of the
	// dialog bar named "My Dialog Bar":
	//   1. In ResourceView, open the menu resource that is used by
	//      the CChildFrame class
	//   2. Select the View submenu
	//   3. Double-click on the blank item at the bottom of the submenu
	//   4. Assign the new item an ID: CG_ID_VIEW_MYDIALOGBAR
	//   5. Assign the item a Caption: My Dialog Bar

	// TODO: Change the value of CG_ID_VIEW_MYDIALOGBAR to an appropriate value:
	//   1. Open the file resource.h
	//   2. Find the definition for the symbol CG_ID_VIEW_MYDIALOGBAR
	//   3. Change the value of the symbol. Use a value in the range
	//      0xE804 to 0xE81A that is not already used by another symbol

	// CG: The following block was inserted by the 'Dialog Bar' component
	{
		// Initialize dialog bar m_template

	if (!m_ToolBar.Create(this) ||
		!m_ToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_ToolBar.SetBarStyle(m_ToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_ToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_ToolBar);
		
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	    m_wndStatusBar.SetPaneStyle(0,SBPS_STRETCH);

	}

	// TODO: Add a menu item that will toggle the visibility of the
	// dialog bar named "SaveDialog":
	//   1. In ResourceView, open the menu resource that is used by
	//      the CChildFrame class
	//   2. Select the View submenu
	//   3. Double-click on the blank item at the bottom of the submenu
	//   4. Assign the new item an ID: CG_ID_VIEW_SAVEDIALOG
	//   5. Assign the item a Caption: SaveDialog

	// TODO: Change the value of CG_ID_VIEW_SAVEDIALOG to an appropriate value:
	//   1. Open the file resource.h
	//   2. Find the definition for the symbol CG_ID_VIEW_SAVEDIALOG
	//   3. Change the value of the symbol. Use a value in the range
	//      0xE804 to 0xE81A that is not already used by another symbol

	// CG: The following block was inserted by the 'Dialog Bar' component
	{
		// Initialize dialog bar m_wndSaveDialog
		if (!m_wndSaveDialog.Create(this, CG_IDD_SAVEDIALOG,
			CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,
			CG_ID_VIEW_SAVEDIALOG))
		{
			TRACE0("Failed to create dialog bar m_wndSaveDialog\n");
			return -1;		// fail to create
		}

		m_wndSaveDialog.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
		EnableDocking(CBRS_ALIGN_ANY);
		DockControlBar(&m_wndSaveDialog);
	}
	dockstate.LoadState("ChildDockbars");
	SetDockState(dockstate);
	return 0;
}

void CChildFrame::OnSavesettings() 
{
	// TODO: Add your command handler code here
	CDockState dockstate;
	CFrameWnd::GetDockState(dockstate);
	dockstate.SaveState("ChildDockbars");
}
/*
afx_msg void CChildFrame::OnStatus(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();
	pCmdUI->SetText("Test");
}
*/

BOOL CChildFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if((pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_RETURN)) 
	{
		CString FileName;
		char File[200];
		CEdit* pSaveFile = (CEdit*)m_wndSaveDialog.GetDlgItem(IDC_SAVEEDIT);
		pSaveFile->GetWindowText(FileName);
		strcpy(File,FileName);
		GetTopWindow()->SendMessage(WM_SAVEIT,0,(long)&File[0]);
		return TRUE;
	}
	return CMDIChildWnd::PreTranslateMessage(pMsg);
}

LRESULT CChildFrame::OnSetPane(WPARAM wparam,LPARAM lparam)
{
	CString Text = (char*)lparam;
	m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_TEMPLATEPANE),Text);
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
	if(CloseEnabled)CMDIChildWnd::OnClose();
}
