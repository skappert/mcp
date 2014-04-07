// Page2FV.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage2FV dialog

class CPage2FV : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage2FV)

// Construction
public:
	CPage2FV();
	~CPage2FV();
	afx_msg void	OnSetVolt();
// Dialog Data
	//{{AFX_DATA(CPage2FV)
	enum { IDD = IDD_PAGE2FV };
	double	m_noscanvolt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage2FV)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage2FV)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
