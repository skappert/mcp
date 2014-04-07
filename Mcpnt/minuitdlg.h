// MinuitDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMinuitDlg dialog



class CMinuitDlg : public CDialog
{
// Construction
public:
	CMinuitDlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CMinuitDlg)
	enum { IDD = IDD_MINUIT };
	BOOL	m_baseoffset;
	BOOL	m_equalspaced;
	BOOL	m_linoffset;
	BOOL	m_negativepeaks;
	int		m_numofpeaks;
	BOOL	m_gauss;
	BOOL	m_lorentz;
	BOOL	m_voigt;
	BOOL	m_gauss_exp;
	BOOL	m_lorentz_exp;
	BOOL	m_voigt_exp;
	BOOL	m_decay;
	BOOL	m_hfsfit;
	BOOL	m_linear;
	BOOL	m_user;
	BOOL	m_commonwidth;
	BOOL	m_cos_exp;
	BOOL	m_mx_bloch;
	BOOL	m_my_bloch;
	BOOL	m_mz_bloch;
	BOOL	m_mxyz_bloch;
	CString	m_info;
	//}}AFX_DATA
	void IsLinear(CCmdUI* pCmdUI);
	void IsLorentz(CCmdUI* pCmdUI);
	void IsGauss(CCmdUI* pCmdUI);
	void IsHfsFit(CCmdUI* pCmdUI);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMinuitDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMinuitDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
