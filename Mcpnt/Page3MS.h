// Page3MS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3MS dialog

class CPage3MS : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3MS)

// Construction
public:
	CPage3MS();
	~CPage3MS();

// Dialog Data
	//{{AFX_DATA(CPage3MS)
	enum { IDD = IDD_PAGE3MS };
	CButton	m_test;
	BOOL	m_gps;
	BOOL	m_hrs;
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3MS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3MS)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
