// Page2AS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage2AS dialog

class CPage2AS : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage2AS)

// Construction
public:
	CPage2AS();
	~CPage2AS();

// Dialog Data
	//{{AFX_DATA(CPage2AS)
	enum { IDD = IDD_PAGE2AS };
	BOOL	m_autoopen;
	BOOL	m_autoresize;
	CString	m_dataop;
	CString	m_errorop;
	BOOL	m_lines;
	BOOL	m_points;
	BOOL	m_stairs;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage2AS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage2AS)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
