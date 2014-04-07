// Page1RS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1RS dialog

class CPage1RS : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1RS)

// Construction
public:
	CPage1RS();
	~CPage1RS();
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
	double startfreq;
	double stepfreq;
	double stopfreq;

// Dialog Data
	//{{AFX_DATA(CPage1RS)
	enum { IDD = IDD_PAGE1RS };
	CString	m_ampdb;
	CString	m_ampmv;
	BOOL	m_ampoff;
	BOOL	m_modam;
	double	m_modampam;
	double	m_modampfm;
	BOOL	m_modfm;
	double	m_modfreq;
	BOOL	m_nomod;
	CString	m_centerfreq;
	CString	m_startfreq;
	CString	m_stepfreq;
	CString	m_stopfreq;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1RS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1RS)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
