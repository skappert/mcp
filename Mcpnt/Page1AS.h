// Page1AS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1AS dialog

class CPage1AS : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1AS)

// Construction
public:
	CPage1AS();
	~CPage1AS();

// Dialog Data
	//{{AFX_DATA(CPage1AS)
	enum { IDD = IDD_PAGE1AS };
	BOOL	m_autoopen;
	BOOL	m_autoresize;
	CString	m_dataop;
	CString	m_errorop;
	BOOL	m_lines;
	BOOL	m_points;
	BOOL	m_stairs;
	BOOL	m_sumvoltage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1AS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1AS)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
