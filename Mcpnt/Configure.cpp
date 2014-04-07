// Configure.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Configure.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigure dialog


CConfigure::CConfigure(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigure::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigure)
		// NOTE: the ClassWizard will add member initialization here
	m_lines = FALSE;
	m_points = TRUE;
	m_stairs = FALSE;
	//}}AFX_DATA_INIT
}


void CConfigure::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigure)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Check(pDX, IDC_LINES, m_lines);
	DDX_Check(pDX, IDC_POINTS, m_points);
	DDX_Check(pDX, IDC_STAIRS, m_stairs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigure, CDialog)
	//{{AFX_MSG_MAP(CConfigure)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigure message handlers
