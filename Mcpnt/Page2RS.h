// Page2RS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage2RS dialog

class CPage2RS : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage2RS)

// Construction
public:
	CPage2RS();
	~CPage2RS();
	afx_msg void	OnNoScanFreq();
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

// Dialog Data
	//{{AFX_DATA(CPage2RS)
	enum { IDD = IDD_PAGE2RS };
	CString	m_noscanampdb;
	CString	m_noscanampmv;
	BOOL	m_noscanampoff;
	double	m_noscanfreq;
	double	m_noscanmodampam;
	double	m_noscanmodampfm;
	double	m_noscanmodfreq;
	BOOL	m_noscanmodam;
	BOOL	m_noscanmodfm;
	BOOL	m_noscannomod;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage2RS)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage2RS)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
