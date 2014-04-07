// Page1Bo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1Bo dialog

class CPage1Bo : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1Bo)

// Construction
public:
	CPage1Bo();
	~CPage1Bo();

// Dialog Data
	//{{AFX_DATA(CPage1Bo)
	enum { IDD = IDD_PAGE1BO };
	UINT	m_bitchannel;
	BOOL	m_biton;
	BOOL	m_bitoff;
	BOOL	m_bittoggle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1Bo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1Bo)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
/////////////////////////////////////////////////////////////////////////////
// CPage1BO dialog

class CPage1BO : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1BO)

// Construction
public:
	CPage1BO();
	~CPage1BO();

// Dialog Data
	//{{AFX_DATA(CPage1BO)
	enum { IDD = IDD_PAGE1BO };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1BO)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1BO)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
