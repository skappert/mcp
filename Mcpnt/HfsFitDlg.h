// HfsFitDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHfsFitDlg dialog

class CHfsFitDlg : public CDialog
{
// Construction
public:
	CHfsFitDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHfsFitDlg)
	enum { IDD = IDD_HFSFIT };
	double	m_a_a;
	double	m_a_g;
	double	m_accelvoltage;
	double	m_b_a;
	double	m_b_g;
	double	m_i;
	double	m_isv;
	double	m_j_a;
	double	m_j_g;
	double	m_kepco;
	double	m_linewidth;
	double	m_mass;
	double	m_wavelength;
	BOOL	m_gauss;
	BOOL	m_lorentz;
	BOOL	m_voigt;
	BOOL	m_gaussexp;
	BOOL	m_lorentzexp;
	BOOL	m_voigtexp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHfsFitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHfsFitDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
