// Page3am.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3am dialog

class CPage3am : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3am)

// Construction
public:
	CPage3am();
	~CPage3am();

// Dialog Data
	//{{AFX_DATA(CPage3am)
	enum { IDD = IDD_PAGE3AM };
	CButton	m_test;
	CButton	m_rewind;
	double	m_actpos;
	double	m_donepos;
	double	m_leftpos;
	double	m_moveto;
	int		m_slot;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3am)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3am)
	afx_msg void OnRewind();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
