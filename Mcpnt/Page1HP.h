// Page1HP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1HP dialog

class CPage1HP : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1HP)

// Construction
public:
	CPage1HP();
	~CPage1HP();
	afx_msg void	OnStartFreq();
	afx_msg void	OnStopFreq();
	afx_msg void	OnStepFreq();
	afx_msg void	OnCenterFreq();

	afx_msg void	OnAmpDb();
	afx_msg void	OnAmpMv();

	afx_msg void	OnAmpoff();

	int		channels;
	double	resolution;

	double ampdb;
	double ampmv;
	double ampdbback;
	double ampmvback;
	double ampoff;
	double centerfreq;
	double modampam;
	double modampfm;
	double modfreq;
	double startfreq;
	double stepfreq;
	double stopfreq;

// Dialog Data
	//{{AFX_DATA(CPage1HP)
	enum { IDD = IDD_PAGE1HP };
	CString	m_ampdb;
	CString	m_ampmv;
	BOOL	m_ampoff;
	CString	m_centerfreq;
	CString	m_modampam;
	CString	m_modampfm;
	CString	m_modfreq;
	CString	m_startfreq;
	CString	m_stepfreq;
	CString	m_stopfreq;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1HP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1HP)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
