// Page2fs.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage2fs dialog

class CPage2fs : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage2fs)

// Construction
public:
	CPage2fs();
	~CPage2fs();

// Dialog Data
	//{{AFX_DATA(CPage2fs)
	enum { IDD = IDD_PAGE2FS };
	BOOL	m_fluke1;
	BOOL	m_fluke2;
	BOOL	m_fluke3;
	BOOL	m_nofluke;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage2fs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage2fs)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
