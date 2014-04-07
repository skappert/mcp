// Page2HP.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "Page2HP.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage2HP property page

IMPLEMENT_DYNCREATE(CPage2HP, CPropertyPage)

CPage2HP::CPage2HP() : CPropertyPage(CPage2HP::IDD)
{
	//{{AFX_DATA_INIT(CPage2HP)

	//}}AFX_DATA_INIT
}

CPage2HP::~CPage2HP()
{
}

void CPage2HP::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage2HP)
	DDX_Text(pDX, IDC_NOSCANAMPDB, m_noscanampdb);
	//DDV_MinMaxDouble(pDX, m_noscanampdb, -137., 18.);
	DDX_Text(pDX, IDC_NOSCANAMPMV, m_noscanampmv);
	//DDV_MinMaxDouble(pDX, m_noscanampmv, 0., 10000.);
	DDX_Text(pDX, IDC_NOSCANFREQ, m_noscanfreq);
	//DDV_MinMaxDouble(pDX, m_noscanfreq, 0, 2000.);
	DDX_Check(pDX, IDC_NOSCANAMPOFF, m_noscanampoff);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage2HP, CPropertyPage)
	//{{AFX_MSG_MAP(CPage2HP)
	//}}AFX_MSG_MAP
	ON_EN_SETFOCUS (IDC_NOSCANFREQ,OnNoScanFreq)

	ON_EN_UPDATE (IDC_NOSCANAMPDB,OnAmpDb)
	ON_EN_UPDATE (IDC_NOSCANAMPMV,OnAmpMv)

	ON_BN_CLICKED (IDC_NOSCANAMPOFF,OnAmpoff)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage2HP message handlers

void	CPage2HP::OnNoScanFreq()
{
	UpdateData(TRUE);
	SetModified(TRUE);	
	UpdateData(FALSE);
}

void	CPage2HP::OnAmpDb()
{
	UpdateData(TRUE);
	ampdb	= atof(m_noscanampdb);
	ampmv	= atof(m_noscanampmv);

	ampmv = pow((double)10,(ampdb/20))*2000;
	//m_ampdb.Format("%.4g",ampdb);
	m_noscanampmv.Format("%.4g",ampmv);
	SetModified(TRUE);
	UpdateData(FALSE);
}

void	CPage2HP::OnAmpMv()
{
	UpdateData(TRUE);
	ampdb	= atof(m_noscanampdb);
	ampmv	= atof(m_noscanampmv);

	ampdb = 20*log10(ampmv/2000);
	if(ampdb<-137) ampdb = -137;
	m_noscanampdb.Format("%.4g",ampdb);
	SetModified(TRUE);
	UpdateData(FALSE);
}


void CPage2HP::OnAmpoff() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_noscanampoff)
	{
		ampdbback	= atof(m_noscanampdb);
		ampmvback	= atof(m_noscanampmv);
		ampdb  = (double)-137;
		ampmv  = (double)0;
		m_noscanampdb.Format("%.4g",(double)-137);
		m_noscanampmv.Format("%.4g",(double)0);
		(CEdit*)GetDlgItem(IDC_NOSCANAMPDB)->EnableWindow(FALSE);
		(CEdit*)GetDlgItem(IDC_NOSCANAMPMV)->EnableWindow(FALSE);
		SetModified(TRUE);
		UpdateData(FALSE);
	}
	else
	{
		m_noscanampdb.Format("%.4g",ampdbback);
		m_noscanampmv.Format("%.4g",ampmvback);
		ampdb  = ampdbback;
		ampmv  = ampmvback;
		(CEdit*)GetDlgItem(IDC_NOSCANAMPDB)->EnableWindow(TRUE);
		(CEdit*)GetDlgItem(IDC_NOSCANAMPMV)->EnableWindow(TRUE);
		SetModified(TRUE);
		UpdateData(FALSE);
	}	
}

BOOL CPage2HP::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	ampmv = pow((double)10,(ampdb/20))*2000;
	m_noscanampdb.Format("%.4g",ampdb);
	m_noscanampmv.Format("%.4g",ampmv);
	SetModified(TRUE);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CPage2HP::OnApply() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnApply();
}
