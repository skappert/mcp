// Page2PM.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage2PM dialog

class CPage2PM : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage2PM)

// Construction
public:
	CPage2PM();
	~CPage2PM();

// Dialog Data
	//{{AFX_DATA(CPage2PM)
	enum { IDD = IDD_PAGE2PM };
	BOOL	m_autoopen;
	BOOL	m_autoresize;
	BOOL	m_lines;
	BOOL	m_points;
	BOOL	m_stairs;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage2PM)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage2PM)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
