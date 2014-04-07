// Page1fs.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1fs dialog

class CPage1fs : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1fs)

// Construction
public:
	CPage1fs();
	~CPage1fs();

// Dialog Data
	//{{AFX_DATA(CPage1fs)
	enum { IDD = IDD_PAGE1FS };
	BOOL	m_fluke1;
	BOOL	m_fluke2;
	BOOL	m_fluke3;
	BOOL	m_nofluke;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1fs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1fs)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
