// Page1LV.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1LV dialog

class CPage1LV : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1LV)

// Construction
public:
	CPage1LV();
	~CPage1LV();
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
	//{{AFX_DATA(CPage1LV)
	enum { IDD = IDD_PAGE1LV };
	CString	m_centervolt;
	BOOL	m_slowreset;
	double	m_slowresetch;
	CString	m_startvolt;
	CString	m_stepvolt;
	CString	m_stopvolt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1LV)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1LV)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
		


};
