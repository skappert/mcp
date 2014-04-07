// Page1GW.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1GW dialog

class CPage1GW : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1GW)

// Construction
public:
	CPage1GW();
	~CPage1GW();

// Dialog Data
	//{{AFX_DATA(CPage1GW)
	enum { IDD = IDD_PAGE1GW };
	CString m_sendstring;
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1GW)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1GW)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
