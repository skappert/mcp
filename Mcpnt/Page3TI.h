// Page3TI.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3TI dialog

class CPage3TI : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3TI)

// Construction
public:
	CPage3TI();
	~CPage3TI();

// Dialog Data
	//{{AFX_DATA(CPage3TI)
	enum { IDD = IDD_PAGE3TI };
	CButton	m_test;
	int		m_slot;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3TI)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3TI)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
