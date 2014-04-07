// Page3KW.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3KW dialog

class CPage3KW : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3KW)

// Construction
public:
	CPage3KW();
	~CPage3KW();

// Dialog Data
	//{{AFX_DATA(CPage3KW)
	enum { IDD = IDD_PAGE3KW };
	CButton	m_test;
	int		m_gpib;
	CString	m_sendcommand;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3KW)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3KW)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
