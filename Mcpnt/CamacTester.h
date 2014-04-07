// CamacTester.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCamacTester dialog

class CCamacTester : public CDialog
{
// Construction
public:
	CCamacTester(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCamacTester)
	enum { IDD = IDD_CAMACSEND };
	int		m_a;
	long	m_data;
	int		m_f;
	int		m_n;
	CString	m_answer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCamacTester)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCamacTester)
	afx_msg void OnSendnaf();
	afx_msg void OnRandom();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
