// Page1SR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1SR dialog

class CPage1SR : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1SR)

// Construction
public:
	CPage1SR();
	~CPage1SR();

// Dialog Data
	//{{AFX_DATA(CPage1SR)
	enum { IDD = IDD_PAGE1SR };
	CString	m_sendstring;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1SR)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1SR)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
