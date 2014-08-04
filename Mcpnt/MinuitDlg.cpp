// MinuitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "MinuitDlg.h"
#include "User.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMinuitDlg dialog

CMinuitDlg::CMinuitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMinuitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMinuitDlg)
	m_baseoffset	= TRUE;
	m_equalspaced	= FALSE;
	m_linoffset		= TRUE;
	m_negativepeaks = FALSE;
	m_numofpeaks	= 1;
	m_gauss			= FALSE;
	m_lorentz		= FALSE;
	m_voigt			= FALSE;
	m_gauss_exp		= FALSE;
	m_lorentz_exp	= FALSE;
	m_voigt_exp		= FALSE;
	m_decay			= FALSE;
	m_hfsfit		= FALSE;
	m_linear		= TRUE;
	m_commonwidth	= TRUE;
	m_cos_exp		= FALSE;
	m_mx_bloch		= FALSE;
	m_my_bloch		= FALSE;
	m_mz_bloch		= FALSE;
	m_mxyz_bloch	= FALSE;
	m_info			= _T("");
	//}}AFX_DATA_INIT
}


void CMinuitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMinuitDlg)
	DDX_Check(pDX, IDC_BASEOFFSET, m_baseoffset);
	DDX_Check(pDX, IDC_EQUALSPACED, m_equalspaced);
	DDX_Check(pDX, IDC_LINOFFSET, m_linoffset);
	DDX_Check(pDX, IDC_NEGATIVEPEAKS, m_negativepeaks);
	DDX_Text(pDX, IDC_NUMOFPEAKS, m_numofpeaks);
	DDX_Check(pDX, IDC_GAUSS, m_gauss);
	DDX_Check(pDX, IDC_LORENTZ, m_lorentz);
	DDX_Check(pDX, IDC_VOIGT, m_voigt);
	DDX_Check(pDX, IDC_GAUSS_EXP, m_gauss_exp);
	DDX_Check(pDX, IDC_LORENTZ_EXP, m_lorentz_exp);
	DDX_Check(pDX, IDC_VOIGT_EXP, m_voigt_exp);
	DDX_Check(pDX, IDC_DECAY, m_decay);
	DDX_Check(pDX, IDC_HFSFIT, m_hfsfit);
	DDX_Check(pDX, IDC_LINEAR, m_linear);
	DDX_Check(pDX, IDC_USERFIT, m_user);
	DDX_Check(pDX, IDC_COMMONWIDTH, m_commonwidth);
	DDX_Text(pDX, IDC_INFO, m_info);
	DDV_MaxChars(pDX, m_info, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMinuitDlg, CDialog)
	//{{AFX_MSG_MAP(CMinuitDlg)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDC_LINEAR,IsLinear)
	ON_UPDATE_COMMAND_UI(IDC_GAUSS,IsGauss)
	ON_UPDATE_COMMAND_UI(IDC_LORENTZ,IsLorentz)
	ON_UPDATE_COMMAND_UI(IDC_HFSFIT,IsHfsFit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMinuitDlg message handlers

BOOL CMinuitDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KILLFOCUS)
	{
		CString NumPeaks;
		UpdateData(TRUE);
		if (m_linear)
		{
			(CButton*)GetDlgItem(IDC_NEGATIVEPEAKS)->EnableWindow(FALSE);
			(CEdit*)GetDlgItem(IDC_NUMOFPEAKS)->EnableWindow(FALSE);
			(CButton*)GetDlgItem(IDC_EQUALSPACED)->EnableWindow(FALSE);
			(CButton*)GetDlgItem(IDC_LINOFFSET)->EnableWindow(TRUE);
			(CButton*)GetDlgItem(IDC_BASEOFFSET)->EnableWindow(TRUE);
			(CButton*)GetDlgItem(IDC_COMMONWIDTH)->EnableWindow(FALSE);
		}
		if (m_gauss || m_lorentz || m_voigt || m_gauss_exp || m_lorentz_exp || m_voigt_exp)// (m_linear)
		{
			(CButton*)GetDlgItem(IDC_NEGATIVEPEAKS)->EnableWindow(TRUE);
			(CEdit*)GetDlgItem(IDC_NUMOFPEAKS)->EnableWindow(TRUE);
			(CButton*)GetDlgItem(IDC_EQUALSPACED)->EnableWindow(TRUE);
			(CButton*)GetDlgItem(IDC_LINOFFSET)->EnableWindow(TRUE);
			(CButton*)GetDlgItem(IDC_BASEOFFSET)->EnableWindow(TRUE);
			if (m_numofpeaks > 1)(CButton*)GetDlgItem(IDC_COMMONWIDTH)->EnableWindow(TRUE);
			else(CButton*)GetDlgItem(IDC_COMMONWIDTH)->EnableWindow(FALSE);
		}
		if (m_decay)// (m_linear)
		{
			(CButton*)GetDlgItem(IDC_NEGATIVEPEAKS)->EnableWindow(TRUE);
			(CEdit*)GetDlgItem(IDC_NUMOFPEAKS)->EnableWindow(TRUE);
			(CButton*)GetDlgItem(IDC_EQUALSPACED)->EnableWindow(FALSE);
			(CButton*)GetDlgItem(IDC_LINOFFSET)->EnableWindow(FALSE);
			(CButton*)GetDlgItem(IDC_BASEOFFSET)->EnableWindow(TRUE);
			(CButton*)GetDlgItem(IDC_COMMONWIDTH)->EnableWindow(FALSE);
		}
		if (m_hfsfit)// (m_linear)
		{
			(CButton*)GetDlgItem(IDC_NEGATIVEPEAKS)->EnableWindow(TRUE);
			(CEdit*)GetDlgItem(IDC_NUMOFPEAKS)->EnableWindow(FALSE);
			(CButton*)GetDlgItem(IDC_EQUALSPACED)->EnableWindow(FALSE);
			(CButton*)GetDlgItem(IDC_LINOFFSET)->EnableWindow(TRUE);
			(CButton*)GetDlgItem(IDC_BASEOFFSET)->EnableWindow(TRUE);
			(CButton*)GetDlgItem(IDC_COMMONWIDTH)->EnableWindow(FALSE);
		}
		if (m_user)// (m_linear)
		{
			(CButton*)GetDlgItem(IDC_NEGATIVEPEAKS)->EnableWindow(FALSE);
			(CEdit*)GetDlgItem(IDC_NUMOFPEAKS)->EnableWindow(FALSE);
			(CButton*)GetDlgItem(IDC_EQUALSPACED)->EnableWindow(FALSE);
			(CButton*)GetDlgItem(IDC_LINOFFSET)->EnableWindow(FALSE);
			(CButton*)GetDlgItem(IDC_BASEOFFSET)->EnableWindow(FALSE);
			(CButton*)GetDlgItem(IDC_COMMONWIDTH)->EnableWindow(FALSE);
		}
		if (m_numofpeaks > 20)m_numofpeaks = 20;
		if (m_numofpeaks < 1)m_numofpeaks = 1;
		NumPeaks.Format("%u",m_numofpeaks);
		if (m_equalspaced) m_info = NumPeaks+" equalspaced";
		else m_info = NumPeaks+" not equalspaced";
		UpdateData(FALSE);
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CMinuitDlg::IsLinear(CCmdUI* pCmdUI)
{
	UpdateData(TRUE);
	pCmdUI->Enable(FALSE);
	UpdateData(FALSE);
}

void CMinuitDlg::IsLorentz(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
	UpdateData(FALSE);
}

void CMinuitDlg::IsGauss(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
	UpdateData(FALSE);
}

void CMinuitDlg::IsHfsFit(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

