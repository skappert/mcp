// DataReadout.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "DataReadout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataReadout dialog


CDataReadout::CDataReadout(CWnd* pParent /*=NULL*/)
	: CDialog(CDataReadout::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataReadout)
	m_texterr = _T("");
	m_textx = _T("");
	m_texty = _T("");
	m_numerr = _T("");
	m_numx = _T("");
	m_numy = _T("");
	//}}AFX_DATA_INIT
}


void CDataReadout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataReadout)
	DDX_Control(pDX, IDC_CENTERINSWEEP, m_centerinsweep);
	DDX_Text(pDX, IDC_TEXTERR, m_texterr);
	DDV_MaxChars(pDX, m_texterr, 40);
	DDX_Text(pDX, IDC_TEXTX, m_textx);
	DDV_MaxChars(pDX, m_textx, 40);
	DDX_Text(pDX, IDC_TEXTY, m_texty);
	DDV_MaxChars(pDX, m_texty, 40);
	DDX_Text(pDX, IDC_NUMERR, m_numerr);
	DDV_MaxChars(pDX, m_numerr, 30);
	DDX_Text(pDX, IDC_NUMX, m_numx);
	DDV_MaxChars(pDX, m_numx, 30);
	DDX_Text(pDX, IDC_NUMY, m_numy);
	DDV_MaxChars(pDX, m_numy, 30);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataReadout, CDialog)
	//{{AFX_MSG_MAP(CDataReadout)
	ON_BN_CLICKED(IDC_CENTERINSWEEP, OnCenterinsweep)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataReadout message handlers

void CDataReadout::OnCenterinsweep() 
{
	// TODO: Add your control notification handler code here
	EndDialog(IDC_CENTERINSWEEP);
}
