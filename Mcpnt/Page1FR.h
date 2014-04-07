// Page1FR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1FR dialog

class CPage1FR : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1FR)

// Construction
public:
	CPage1FR();
	~CPage1FR();

// Dialog Data
	//{{AFX_DATA(CPage1FR)
	enum { IDD = IDD_PAGE1FR };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1FR)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1FR)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
