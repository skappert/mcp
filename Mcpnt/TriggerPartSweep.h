// TriggerPartSweep.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTriggerPartSweep dialog

class CTriggerPartSweep : public CDialog
{
// Construction
public:
	CTriggerPartSweep(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTriggerPartSweep)
	enum { IDD = IDD_TRIGGERPART };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
protected:
	// ClassWizard generated virtual function ov
	//{{AFX_VIRTUAL(CTriggerPartSweep)

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTriggerPartSweep)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
