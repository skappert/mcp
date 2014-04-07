// Page3TR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3TR dialog

class CPage3TR : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3TR)

// Construction
public:
	CPage3TR();
	~CPage3TR();

// Dialog Data
	//{{AFX_DATA(CPage3TR)
	enum { IDD = IDD_PAGE3TR };
	CButton	m_test;
	int		m_slot;
	int		m_subaddress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3TR)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3TR)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
