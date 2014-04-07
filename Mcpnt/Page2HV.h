// Page2HV.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage2HV dialog

class CPage2HV : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage2HV)

// Construction
public:
	CPage2HV();
	~CPage2HV();
	afx_msg void	OnSetVolt();
// Dialog Data
	//{{AFX_DATA(CPage2HV)
	enum { IDD = IDD_PAGE2HV };
	double	m_noscanvolt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage2HV)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage2HV)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
