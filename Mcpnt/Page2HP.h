// Page2HP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage2HP dialog

class CPage2HP : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage2HP)

// Construction
public:
	CPage2HP();
	~CPage2HP();
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
	//{{AFX_DATA(CPage2HP)
	enum { IDD = IDD_PAGE2HP };
	CString	m_noscanampdb;
	CString	m_noscanampmv;
	double	m_noscanfreq;
	BOOL	m_noscanampoff;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage2HP)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage2HP)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
