// TimerDelay.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "TimerDelay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTimerDelay dialog


CTimerDelay::CTimerDelay(CWnd* pParent /*=NULL*/)
	: CDialog(CTimerDelay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTimerDelay)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTimerDelay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimerDelay)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTimerDelay, CDialog)
	//{{AFX_MSG_MAP(CTimerDelay)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimerDelay message handlers
