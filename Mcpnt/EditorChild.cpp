// EditorChild.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "EditorChild.h"
#include "EditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorChild

IMPLEMENT_DYNCREATE(CEditorChild, CMDIChildWndEx)

CEditorChild::CEditorChild()
{
}

CEditorChild::CEditorChild(CView* dView)
{
	pEditorView=(CView*)dView;
}

CEditorChild::~CEditorChild()
{
}


BEGIN_MESSAGE_MAP(CEditorChild, CMDIChildWndEx)
	//{{AFX_MSG_MAP(CEditorChild)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorChild message handlers

void CEditorChild::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWndEx::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CEditorView* pEditor=(CEditorView*)pEditorView;
	pEditor->ResizeWin(cx,cy);
}

BOOL CEditorChild::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();

	cs.cx=pApp->ScriptDx;
	cs.cy=pApp->ScriptDy;
	if(!pApp->AutoScript)
	{
		cs.x=pApp->ScriptX;
		cs.y=pApp->ScriptY;
	}
	else
	{
		if((pApp->LastX+pApp->ScriptDx < 1024)&&
			(pApp->LastY+pApp->ScriptDy < 768))
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

void CEditorChild::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CEditorView* pEdit = (CEditorView*)pEditorView;
	if(pEdit->GetBufferLength()!=0)
	{
		if(IDYES==AfxMessageBox("Modified buffer, quit anyway?",MB_YESNO,0))
		CMDIChildWndEx::OnClose();
	}
	else CMDIChildWndEx::OnClose();
}
