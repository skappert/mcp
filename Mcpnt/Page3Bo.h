// Page3Bo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3Bo dialog

class CPage3Bo : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3Bo)

// Construction
public:
	CPage3Bo();
	~CPage3Bo();

// Dialog Data
	//{{AFX_DATA(CPage3Bo)
	enum { IDD = IDD_PAGE3BO };
	UINT	m_slot;
	UINT	m_subaddress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3Bo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3Bo)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
