// TemplateAdder.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "TemplateAdder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTemplateAdder dialog


CTemplateAdder::CTemplateAdder(CWnd* pParent /*=NULL*/)
	: CDialog(CTemplateAdder::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTemplateAdder)
	m_filelist = _T("");
	//}}AFX_DATA_INIT
}


void CTemplateAdder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTemplateAdder)
	DDX_Control(pDX, IDC_ADDLIST, m_addlist);
	DDX_LBString(pDX, IDC_FILELIST, m_filelist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTemplateAdder, CDialog)
	//{{AFX_MSG_MAP(CTemplateAdder)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTemplateAdder message handlers
