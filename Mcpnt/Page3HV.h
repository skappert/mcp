// Page3HV.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3HV dialog

class CPage3HV : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3HV)

// Construction
public:
	CPage3HV();
	~CPage3HV();

// Dialog Data
	//{{AFX_DATA(CPage3HV)
	enum { IDD = IDD_PAGE3HV };
	CButton	m_test;
	int		m_slot;
	int		m_subaddress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3HV)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3HV)
	afx_msg void OnTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
