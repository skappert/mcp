// Page3FR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3FR dialog

class CPage3FR : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3FR)

// Construction
public:
	CPage3FR();
	~CPage3FR();

// Dialog Data
	//{{AFX_DATA(CPage3FR)
	enum { IDD = IDD_PAGE3FR };
	CButton	m_test;
	int		m_gpib;
	CString	m_sendcommand;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3FR)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3FR)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
