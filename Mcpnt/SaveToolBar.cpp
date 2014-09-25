// SaveToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "SaveToolBar.h"


// CSaveToolBar

IMPLEMENT_DYNAMIC(CSaveToolBar, CMFCToolBar)

CSaveToolBar::CSaveToolBar()
{

}

CSaveToolBar::~CSaveToolBar()
{
}


BEGIN_MESSAGE_MAP(CSaveToolBar, CMFCToolBar)
END_MESSAGE_MAP()


// CSaveToolBar message handlers
void CSaveToolBar::OnReset()  
{  
	TRACE0("CMFCToolBar::OnReset\n");

	// Create the dialog bar  
	if (!m_wndDlgBar.Create(this, CG_IDD_SAVEDIALOG, WS_CHILD | WS_VISIBLE, IDW_USER_TOOLBAR + 7))
	{  
		TRACE0("CDialogBar Create failed\n");
		return;  
	}

	m_wndDlgBar.ShowWindow(true);
} 
