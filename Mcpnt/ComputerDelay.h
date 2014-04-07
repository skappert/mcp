// ComputerDelay.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComputerDelay dialog

class CComputerDelay : public CDialog
{
// Construction
public:
	CComputerDelay(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CComputerDelay)
	enum { IDD = IDD_COMPUTER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComputerDelay)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CComputerDelay)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
