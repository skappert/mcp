// Page3am.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page3am.h"
#include "camac.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage3am property page

IMPLEMENT_DYNCREATE(CPage3am, CPropertyPage)

CPage3am::CPage3am() : CPropertyPage(CPage3am::IDD)
{
	//{{AFX_DATA_INIT(CPage3am)
	m_actpos = 0.0f;
	m_donepos = 0.0f;
	m_leftpos = 0.0f;
	m_moveto = 0.0f;
	m_slot = 1;
	//}}AFX_DATA_INIT
}

CPage3am::~CPage3am()
{
}

void CPage3am::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage3am)
	DDX_Control(pDX, IDC_TEST, m_test);
	DDX_Control(pDX, IDC_REWIND, m_rewind);
	DDX_Text(pDX, IDC_ACTPOS, m_actpos);
	DDV_MinMaxFloat(pDX, m_actpos, -1.e+009f, 1.e+009f);
	DDX_Text(pDX, IDC_DONEPOS, m_donepos);
	DDV_MinMaxFloat(pDX, m_donepos, -1.e+009f, 1.e+009f);
	DDX_Text(pDX, IDC_LEFTPOS, m_leftpos);
	DDV_MinMaxFloat(pDX, m_leftpos, -1.e+009f, 1.e+009f);
	DDX_Text(pDX, IDC_MOVETO, m_moveto);
	DDV_MinMaxFloat(pDX, m_moveto, -1.e+009f, 1.e+009f);
	DDX_Text(pDX, IDC_SLOT, m_slot);
	DDV_MinMaxInt(pDX, m_slot, 1, 24);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage3am, CPropertyPage)
	//{{AFX_MSG_MAP(CPage3am)
	ON_BN_CLICKED(IDC_REWIND, OnRewind)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage3am message handlers

void CPage3am::OnRewind() 
{
	// TODO: Add your control notification handler code here
	/*
	SendNAFCamac(17,0,24);
	SendNAFCamac(17,0,25);
	SendDNAFCamac(1*1+145*256+1*65536,17,0,17);
	SendDNAFCamac(65535,17,0,16);
	*/
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	pApp->GlobalActPosition = pApp->GlobalActPosition = 0;
}
