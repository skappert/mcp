// StepTimer.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "StepTimer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStepTimer dialog


CStepTimer::CStepTimer(CWnd* pParent /*=NULL*/)
	: CDialog(CStepTimer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStepTimer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CStepTimer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStepTimer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStepTimer, CDialog)
	//{{AFX_MSG_MAP(CStepTimer)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStepTimer message handlers
