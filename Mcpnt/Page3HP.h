// Page3HP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3HP dialog

class CPage3HP : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3HP)

// Construction
public:
	CPage3HP();
	~CPage3HP();

// Dialog Data
	//{{AFX_DATA(CPage3HP)
	enum { IDD = IDD_PAGE3HP };
	CButton	m_test;
	int		m_gpib;
	CString	m_sendcommand;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3HP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3HP)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
