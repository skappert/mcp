// Page2am.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage2am dialog

class CPage2am : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage2am)

// Construction
public:
	CPage2am();
	~CPage2am();

// Dialog Data
	//{{AFX_DATA(CPage2am)
	enum { IDD = IDD_PAGE2AM };
	float	m_movecm;
	BOOL	m_usebrake;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage2am)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage2am)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
