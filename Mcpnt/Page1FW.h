// Page1FW.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1FW dialog

class CPage1FW : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1FW)

// Construction
public:
	CPage1FW();
	~CPage1FW();

// Dialog Data
	//{{AFX_DATA(CPage1FW)
	enum { IDD = IDD_PAGE1FW };
	double m_field;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1FW)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1FW)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
