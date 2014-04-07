// Page3RS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3RS dialog

class CPage3RS : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3RS)

// Construction
public:
	CPage3RS();
	~CPage3RS();

// Dialog Data
	//{{AFX_DATA(CPage3RS)
	enum { IDD = IDD_PAGE3RS };
	CButton	m_test;
	int		m_gpib;
	CString	m_sendcommand;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3RS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3RS)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
