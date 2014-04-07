// FastTimer.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "FastTimer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFastTimer dialog


CFastTimer::CFastTimer(CWnd* pParent /*=NULL*/)
	: CDialog(CFastTimer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFastTimer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFastTimer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFastTimer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFastTimer, CDialog)
	//{{AFX_MSG_MAP(CFastTimer)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFastTimer message handlers
