// Page3PR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3PR dialog

class CPage3PR : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3PR)

// Construction
public:
	CPage3PR();
	~CPage3PR();

// Dialog Data
	//{{AFX_DATA(CPage3PR)
	enum { IDD = IDD_PAGE3PR };
	CButton	m_test;
	int		m_slot;
	int		m_subaddress;
	long	m_delay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3PR)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3PR)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
