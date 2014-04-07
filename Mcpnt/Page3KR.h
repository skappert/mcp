// Page3KR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3KR dialog

class CPage3KR : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3KR)

// Construction
public:
	CPage3KR();
	~CPage3KR();

// Dialog Data
	//{{AFX_DATA(CPage3KR)
	enum { IDD = IDD_PAGE3KR };
	CButton	m_test;
	int		m_gpib;
	CString	m_sendcommand;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3KR)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3KR)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
