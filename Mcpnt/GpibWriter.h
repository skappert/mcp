// GpibWriter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGpibWriter dialog

class CGpibWriter : public CDialog
{
// Construction
public:
	CGpibWriter(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGpibWriter)
	enum { IDD = IDD_GPIBWRITE };
	UINT	m_address;
	CString	m_gpibstring;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGpibWriter)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGpibWriter)
	afx_msg void OnWritegpib();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
