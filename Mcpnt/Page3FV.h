// Page3FV.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3FV dialog

class CPage3FV : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3FV)

// Construction
public:
	CPage3FV();
	~CPage3FV();

// Dialog Data
	//{{AFX_DATA(CPage3FV)
	enum { IDD = IDD_PAGE3FV };
	UINT	m_slot;
	UINT	m_subaddress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3FV)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3FV)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
