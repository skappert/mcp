// Page1TI.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1TI dialog

class CPage1TI : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1TI)

// Construction
public:
	CPage1TI();
	~CPage1TI();
	afx_msg void	OnCloseScaler();
	afx_msg void	OnCloseBeamgate();
	afx_msg void	OnOpenBeamgate();
	afx_msg void	OnOpenScaler();
	afx_msg void	OnStartBeamgate();
	afx_msg void	OnStopBeamgate();
	double closescaler;
	double closebeamgate;
	double openbeamgate;
	double openscaler;
	USHORT startbeamgate;
	USHORT stopbeamgate;
// Dialog Data
	//{{AFX_DATA(CPage1TI)
	enum { IDD = IDD_PAGE1TI };
	BOOL	m_sweeptrigger;
	BOOL	m_steptrigger;
	BOOL	m_notrigger;
	CString	m_closebeamgate;
	CString	m_closescaler;
	CString	m_openbeamgate;
	CString	m_openscaler;
	CString	m_startbeamgate;
	CString	m_stopbeamgate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1TI)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1TI)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowprotons();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
