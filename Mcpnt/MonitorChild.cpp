// MonitorChild.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "MinuitChild.h"
#include "minuit.h"
#include "MCP for NTDoc.h"
#include "ActionObjects.h"
#include "MonitorChild.h"
#include "MonitorView.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MonitorChild

IMPLEMENT_DYNCREATE(MonitorChild, CMDIChildWnd)


MonitorChild::MonitorChild()
{
}

MonitorChild::MonitorChild(CView* dDataView)
{
	pDataView=(CView*)dDataView;
}

MonitorChild::~MonitorChild()
{
}


BEGIN_MESSAGE_MAP(MonitorChild, CMDIChildWnd)
	ON_COMMAND_EX(CG_ID_VIEW_ERRORBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(CG_ID_VIEW_ERRORBAR, OnUpdateControlBarMenu)
	ON_WM_CREATE()
	ON_COMMAND_EX(CG_ID_VIEW_DATASTYLE, OnBarCheck)
	ON_UPDATE_COMMAND_UI(CG_ID_VIEW_DATASTYLE, OnUpdateControlBarMenu)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(MonitorChild)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MonitorChild message handlers


void MonitorChild::OnContextMenu(CWnd*, CPoint point)
{
	// CG: This function was added by the Pop-up Menu component

	CMenu menu;
	VERIFY(menu.LoadMenu(CG_IDR_POPUP_DATA_CHILD));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);
}

BOOL MonitorChild::PreTranslateMessage(MSG* pMsg)
{
	// CG: This block was added by the Pop-up Menu component
	{
		// Shift+F10: show pop-up menu.
		if ((((pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN) && // If we hit a key and
			(pMsg->wParam == VK_F10) && (GetKeyState(VK_SHIFT) & ~1)) != 0) ||	// it's Shift+F10 OR
			(pMsg->message == WM_CONTEXTMENU))									// Natural keyboard key
		{
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			CPoint point = rect.TopLeft();
			point.Offset(5, 5);
			OnContextMenu(NULL, point);

			return TRUE;
		}
	}

	return CMDIChildWnd::PreTranslateMessage(pMsg);
}

int MonitorChild::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void MonitorChild::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	//MoveWindow
	// TODO: Add your message handler code here
	MonitorView* pData=(MonitorView*)pDataView;
	pData->ResizeWin(cx,cy);
}

BOOL MonitorChild::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();

	cs.cx=pApp->MonitorDx;
	cs.cy=pApp->MonitorDy;
	if(!pApp->AutoMonitor)
	{
		cs.x=pApp->MonitorX;
		cs.y=pApp->MonitorY;
	}
	else
	{
		if((pApp->LastX+pApp->MonitorDx < 1024)&&
			(pApp->LastY+pApp->MonitorDy < 768))
		{
			pApp->LastX=pApp->LastX+10;
			pApp->LastY=pApp->LastY+10;
		}
		else
		{
			pApp->LastX = 0;
			pApp->LastY = 0;
		}
		cs.x=pApp->LastX+250;
		cs.y=pApp->LastY;
	}

	return CMDIChildWnd::PreCreateWindow(cs);
}

BOOL MonitorChild::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CMDIFrameWnd* pParentWnd, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CMDIChildWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, pContext);
}
