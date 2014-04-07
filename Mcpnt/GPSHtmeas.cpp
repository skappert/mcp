// GPSHtmeas.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "GPSHtmeas.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGPSHtmeas dialog


CGPSHtmeas::CGPSHtmeas(CWnd* pParent /*=NULL*/)
	: CDialog(CGPSHtmeas::IDD, pParent)
{
	Beep(1200,100);
	//{{AFX_DATA_INIT(CGPSHtmeas)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGPSHtmeas::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGPSHtmeas)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGPSHtmeas, CDialog)
	//{{AFX_MSG_MAP(CGPSHtmeas)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGPSHtmeas message handlers
