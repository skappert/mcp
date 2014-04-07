// Page1KW.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1KW dialog

class CPage1KW : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1KW)

// Construction
public:
	CPage1KW();
	~CPage1KW();

// Dialog Data
	//{{AFX_DATA(CPage1KW)
	enum { IDD = IDD_PAGE1KW };
	double m_temp;
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1KW)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1KW)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
