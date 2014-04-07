// Page1am.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1am dialog

class CPage1am : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1am)

// Construction
public:
	CPage1am();
	~CPage1am();

// Dialog Data
	//{{AFX_DATA(CPage1am)
	enum { IDD = IDD_PAGE1AM };
	double	m_movecm;
	BOOL	m_usebrake;
	BOOL	m_backwards;
	BOOL	m_forward;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1am)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1am)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
