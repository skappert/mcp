// StepTimer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStepTimer dialog

class CStepTimer : public CDialog
{
// Construction
public:
	CStepTimer(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStepTimer)
	enum { IDD = IDD_STEPTIMER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStepTimer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStepTimer)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
