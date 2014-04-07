// Page3LV.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3LV dialog

class CPage3LV : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3LV)

// Construction
public:
	CPage3LV();
	~CPage3LV();

// Dialog Data
	//{{AFX_DATA(CPage3LV)
	enum { IDD = IDD_PAGE3LV };
	CButton	m_test;
	int		m_slot;
	int		m_subaddress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3LV)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3LV)
	afx_msg void OnTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
