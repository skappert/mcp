// DataChild.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "MinuitChild.h"
#include "minuit.h"
#include "MCP for NTDoc.h"
#include "ActionObjects.h"
#include "DataChild.h"
#include "DataView.h"
#include "MonitorChild.h"
#include "MonitorView.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DataChild

IMPLEMENT_DYNCREATE(DataChild, CMDIChildWndEx)


DataChild::DataChild()
{
}

DataChild::DataChild(CView* dDataView)
{
	pDataView=(CView*)dDataView;
}

DataChild::~DataChild()
{
}


BEGIN_MESSAGE_MAP(DataChild, CMDIChildWndEx)
	ON_COMMAND_EX(CG_ID_VIEW_ERRORBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(CG_ID_VIEW_ERRORBAR, OnUpdateControlBarMenu)
	ON_WM_CREATE()
	ON_COMMAND_EX(CG_ID_VIEW_DATASTYLE, OnBarCheck)
	ON_UPDATE_COMMAND_UI(CG_ID_VIEW_DATASTYLE, OnUpdateControlBarMenu)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(DataChild)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DataChild message handlers


void DataChild::OnContextMenu(CWnd*, CPoint point)
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

BOOL DataChild::PreTranslateMessage(MSG* pMsg)
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

	return CMDIChildWndEx::PreTranslateMessage(pMsg);
}

int DataChild::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void DataChild::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWndEx::OnSize(nType, cx, cy);
	//MoveWindow
	// TODO: Add your message handler code here
	DataView* pData=(DataView*)pDataView;
	pData->ResizeWin(cx,cy);
}

BOOL DataChild::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();

	cs.cx=pApp->PlotDx;
	cs.cy=pApp->PlotDy;
	if(!pApp->AutoPlot)
	{
		cs.x=pApp->PlotX;
		cs.y=pApp->PlotY;
	}
	else
	{
		if((pApp->LastX+pApp->PlotDx < 1024)&&
			(pApp->LastY+pApp->PlotDy < 768))
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

	return CMDIChildWndEx::PreCreateWindow(cs);
}

BOOL DataChild::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CMDIFrameWndEx* pParentWnd, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CMDIChildWndEx::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, pContext);
}
