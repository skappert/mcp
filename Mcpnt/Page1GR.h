// Page1GR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1GR dialog

class CPage1GR : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1GR)

// Construction
public:
	CPage1GR();
	~CPage1GR();

// Dialog Data
	//{{AFX_DATA(CPage1GR)
	enum { IDD = IDD_PAGE1GR };
	CString	m_sendstring;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1GR)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1GR)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
