// Page3fs.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3fs dialog

class CPage3fs : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3fs)

// Construction
public:
	CPage3fs();
	~CPage3fs();

// Dialog Data
	//{{AFX_DATA(CPage3fs)
	enum { IDD = IDD_PAGE3FS };
	CButton	m_test;
	int		m_slot;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3fs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3fs)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
