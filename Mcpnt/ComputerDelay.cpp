// ComputerDelay.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "ComputerDelay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComputerDelay dialog


CComputerDelay::CComputerDelay(CWnd* pParent /*=NULL*/)
	: CDialog(CComputerDelay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CComputerDelay)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CComputerDelay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComputerDelay)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CComputerDelay, CDialog)
	//{{AFX_MSG_MAP(CComputerDelay)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComputerDelay message handlers
