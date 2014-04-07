// Page1PR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1PR dialog

class CPage1PR : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1PR)

// Construction
public:
	CPage1PR();
	~CPage1PR();

// Dialog Data
	//{{AFX_DATA(CPage1PR)
	enum { IDD = IDD_PAGE1PR };
	BOOL	m_autoopen;
	BOOL	m_autoresize;
	BOOL	m_lines;
	BOOL	m_points;
	BOOL	m_stairs;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1PR)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1PR)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
