// Page3GW.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3GW dialog

class CPage3GW : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3GW)

// Construction
public:
	CPage3GW();
	~CPage3GW();

// Dialog Data
	//{{AFX_DATA(CPage3GW)
	enum { IDD = IDD_PAGE3GW };
	CButton	m_test;
	int		m_gpib;
	CString	m_sendcommand;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3GW)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3GW)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
