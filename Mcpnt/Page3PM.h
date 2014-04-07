// Page3PM.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3PM dialog

class CPage3PM : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3PM)

// Construction
public:
	CPage3PM();
	~CPage3PM();

// Dialog Data
	//{{AFX_DATA(CPage3PM)
	enum { IDD = IDD_PAGE3PM };
	CButton	m_test;
	int		m_slot;
	int		m_subaddress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3PM)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3PM)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
