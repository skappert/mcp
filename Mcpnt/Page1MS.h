// Page1MS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1MS dialog

class CPage1MS : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1MS)

// Construction
public:
	CPage1MS();
	~CPage1MS();
	afx_msg void OnSetAmu();
	// Dialog Data
	//{{AFX_DATA(CPage1MS)
	enum { IDD = IDD_PAGE1MS };
	CString	m_setamu;
	CString	m_setamu2;
	//}}AFX_DATA
	double setamu;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1MS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1MS)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
