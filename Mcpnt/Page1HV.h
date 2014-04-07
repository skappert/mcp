// Page1HV.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1HV dialog

class CPage1HV : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1HV)

// Construction
public:
	CPage1HV();
	~CPage1HV();
	afx_msg void	OnStartVolt();
	afx_msg void	OnStopVolt();
	afx_msg void	OnStepVolt();
	afx_msg void	OnCenterVolt();

	//afx_msg void	OnAmpDb();
	//afx_msg void	OnAmpMv();

	//afx_msg void	OnAmpoff();

	int		channels;
	double	resolution;

	double ampdb;
	double ampmv;
	double ampdbback;
	double ampmvback;
	double ampoff;
	double centervolt;
	double modampam;
	double modampfm;
	double modvolt;
	double startvolt;
	double stepvolt;
	double stopvolt;

// Dialog Data
	//{{AFX_DATA(CPage1HV)
	enum { IDD = IDD_PAGE1HV };
	CString	m_centervolt;
	BOOL	m_slowreset;
	double	m_slowresetch;
	CString	m_startvolt;
	CString	m_stepvolt;
	CString	m_stopvolt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1HV)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1HV)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
