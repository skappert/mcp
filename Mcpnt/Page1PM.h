// Page1PM.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1PM dialog

class CPage1PM : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1PM)

// Construction
public:
	CPage1PM();
	~CPage1PM();

// Dialog Data
	//{{AFX_DATA(CPage1PM)
	enum { IDD = IDD_PAGE1PM };
	BOOL	m_autoopen;
	BOOL	m_autoresize;
	BOOL	m_lines;
	BOOL	m_points;
	BOOL	m_stairs;
	BOOL	m_sumvoltage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1PM)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1PM)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
