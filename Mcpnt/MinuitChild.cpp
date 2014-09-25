// MinuitChild.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "MCP for NTDoc.h"
#include "ActionObjects.h"
#include "MinuitChild.h"
#include "minuit.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMinuitChild

IMPLEMENT_DYNCREATE(CMinuitChild, CMDIChildWndEx)

CMinuitChild::CMinuitChild()
{
}

CMinuitChild::CMinuitChild(CView* dMinuitView)
{
	pMinuitView=(CView*)dMinuitView;
}


CMinuitChild::~CMinuitChild()
{
}


BEGIN_MESSAGE_MAP(CMinuitChild, CMDIChildWndEx)
	//{{AFX_MSG_MAP(CMinuitChild)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMinuitChild message handlers

void CMinuitChild::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWndEx::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMinuit* pData=(CMinuit*)pMinuitView;
	pData->ResizeWin(cx,cy);
}

