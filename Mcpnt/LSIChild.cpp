// LSIChild.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "LSIChild.h"
#include "LSIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLSIChild

IMPLEMENT_DYNCREATE(CLSIChild, CMDIChildWnd)

CLSIChild::CLSIChild()
{
}


CLSIChild::CLSIChild(CView* dLSIView)
{
	pCLSIView=(CView*)dLSIView;
}

CLSIChild::~CLSIChild()
{
}


BEGIN_MESSAGE_MAP(CLSIChild, CMDIChildWnd)
	//{{AFX_MSG_MAP(CLSIChild)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLSIChild message handlers

void CLSIChild::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	//MoveWindow
	// TODO: Add your message handler code here
	CLSIView* pData=(CLSIView*)pCLSIView;
	pData->ResizeWin(cx,cy);
}


BOOL CLSIChild::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();

	cs.cx=pApp->LsiDx;
	cs.cy=pApp->ScriptDy;
	if(!pApp->AutoLsi)
	{
		cs.x=pApp->LsiX;
		cs.y=pApp->LsiY;
	}
	else
	{
		if((pApp->LastX+pApp->LsiDx < 1024)&&
			(pApp->LastY+pApp->LsiDy < 768))
		{
			pApp->LastX=pApp->LastX+10;
			pApp->LastY=pApp->LastY+10;
		}
		else
		{
			pApp->LastX = 0;
			pApp->LastY = 0;
		}
		cs.x=pApp->LastX+300;
		cs.y=pApp->LastY;
	}
	return CMDIChildWnd::PreCreateWindow(cs);
}
