// Page3SR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3SR dialog

class CPage3SR : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3SR)

// Construction
public:
	CPage3SR();
	~CPage3SR();

// Dialog Data
	//{{AFX_DATA(CPage3SR)
	enum { IDD = IDD_PAGE3SR };
	CButton	m_test;
	CString	m_gpib;
	CString	m_sendcommand;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3SR)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3SR)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
