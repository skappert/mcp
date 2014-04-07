// Page1FV.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1FV dialog

class CPage1FV : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1FV)

// Construction
public:
	CPage1FV();
	~CPage1FV();
	afx_msg void	OnStartVolt();
	afx_msg void	OnStopVolt();
	afx_msg void	OnStepVolt();
	afx_msg void	OnCenterVolt();

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
	//{{AFX_DATA(CPage1FV)
	enum { IDD = IDD_PAGE1FV };
	CString	m_centervolt;
	BOOL	m_slowreset;
	double	m_slowresetch;
	CString	m_startvolt;
	CString	m_stepvolt;
	CString	m_stopvolt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1FV)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1FV)
		virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
