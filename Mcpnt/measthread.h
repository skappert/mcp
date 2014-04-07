// MeasThread.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMeasThread dialog

class CMeasThread : public CDialog
{
// Construction
public:
	CMeasThread(CWnd* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMeasThread)
	enum { IDD = IDD_MEASTHREAD };
	CProgressCtrl	m_progstep;
	CString	m_timeleft;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMeasThread)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	CWnd* pParentWnd;

protected:

	// Generated message map functions
	//{{AFX_MSG(CMeasThread)
	virtual void OnCancel();
	afx_msg void OnKill();
	afx_msg void OnHaltmeas();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
