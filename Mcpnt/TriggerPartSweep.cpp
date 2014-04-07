// TriggerPartSweep.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "TriggerPartSweep.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTriggerPartSweep dialog


CTriggerPartSweep::CTriggerPartSweep(CWnd* pParent /*=NULL*/)
	: CDialog(CTriggerPartSweep::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTriggerPartSweep)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTriggerPartSweep::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTriggerPartSweep)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTriggerPartSweep, CDialog)
	//{{AFX_MSG_MAP(CTriggerPartSweep)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTriggerPartSweep message handlers
