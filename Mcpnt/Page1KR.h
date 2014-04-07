// Page1KR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1KR dialog

class CPage1KR : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1KR)

// Construction
public:
	CPage1KR();
	~CPage1KR();

// Dialog Data
	//{{AFX_DATA(CPage1KR)
	enum { IDD = IDD_PAGE1KR };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1KR)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1KR)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
