// FastTimer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFastTimer dialog

class CFastTimer : public CDialog
{
// Construction
public:
	CFastTimer(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFastTimer)
	enum { IDD = IDD_FASTTIMER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFastTimer)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFastTimer)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
