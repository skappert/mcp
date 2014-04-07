// HfsFitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "HfsFitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHfsFitDlg dialog


CHfsFitDlg::CHfsFitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHfsFitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHfsFitDlg)
	m_a_a			= 0.0;
	m_a_g			= 0.0;
	m_accelvoltage	= 0.0;
	m_b_a			= 0.0;
	m_b_g			= 0.0;
	m_i				= 0.0;
	m_isv			= 0.0;
	m_j_a			= 0.0;
	m_j_g			= 0.0;
	m_kepco			= 0.0;
	m_linewidth		= 0.0;
	m_mass			= 0.0;
	m_wavelength	= 0.0;
	m_gauss			= TRUE;
	m_lorentz		= FALSE;
	m_voigt			= FALSE;
	m_gaussexp		= FALSE;
	m_lorentzexp	= FALSE;
	m_voigtexp		= FALSE;
	//}}AFX_DATA_INIT
}


void CHfsFitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHfsFitDlg)
	DDX_Text(pDX, IDC_A_A, m_a_a);
	DDX_Text(pDX, IDC_A_G, m_a_g);
	DDX_Text(pDX, IDC_ACCELVOLTAGE, m_accelvoltage);
	DDX_Text(pDX, IDC_B_A, m_b_a);
	DDX_Text(pDX, IDC_B_G, m_b_g);
	DDX_Text(pDX, IDC_I, m_i);
	DDX_Text(pDX, IDC_ISV, m_isv);
	DDX_Text(pDX, IDC_J_A, m_j_a);
	DDX_Text(pDX, IDC_J_G, m_j_g);
	DDX_Text(pDX, IDC_KEPCO, m_kepco);
	DDX_Text(pDX, IDC_LINEWIDTH, m_linewidth);
	DDX_Text(pDX, IDC_MASS, m_mass);
	DDX_Text(pDX, IDC_WAVELENGTH, m_wavelength);
	DDX_Check(pDX, IDC_GAUSS, m_gauss);
	DDX_Check(pDX, IDC_LORENTZ, m_lorentz);
	DDX_Check(pDX, IDC_VOIGT, m_voigt);
	DDX_Check(pDX, IDC_GAUSSEXP, m_gaussexp);
	DDX_Check(pDX, IDC_LORENTZEXP, m_lorentzexp);
	DDX_Check(pDX, IDC_VOIGTEXP, m_voigtexp);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHfsFitDlg, CDialog)
	//{{AFX_MSG_MAP(CHfsFitDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHfsFitDlg message handlers
