// Page3GR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3GR dialog

class CPage3GR : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3GR)

// Construction
public:
	CPage3GR();
	~CPage3GR();

// Dialog Data
	//{{AFX_DATA(CPage3GR)
	enum { IDD = IDD_PAGE3GR };
	CButton	m_test;
	int		m_gpib;
	CString	m_sendcommand;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3GR)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3GR)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
