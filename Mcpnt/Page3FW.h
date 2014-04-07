// Page3FW.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3FW dialog

class CPage3FW : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3FW)

// Construction
public:
	CPage3FW();
	~CPage3FW();

// Dialog Data
	//{{AFX_DATA(CPage3FW)
	enum { IDD = IDD_PAGE3FW };
	CButton	m_test;
	int		m_gpib;
	CString	m_sendcommand;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3FW)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3FW)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
