// Page3MF.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3MF dialog

class CPage3MF : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3MF)

// Construction
public:
	CPage3MF();
	~CPage3MF();

// Dialog Data
	//{{AFX_DATA(CPage3MF)
	enum { IDD = IDD_PAGE3MF };
	CButton	m_test;
	BOOL	m_gps;
	BOOL	m_hrs;
	int		m_slot;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3MF)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3MF)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
