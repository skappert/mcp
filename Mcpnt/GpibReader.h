// GpibReader.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGpibReader dialog

class CGpibReader : public CDialog
{
// Construction
public:
	CGpibReader(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGpibReader)
	enum { IDD = IDD_GPIBREAD };
	UINT	m_address;
	CString	m_gpibanswer;
	CString	m_gpibstring;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGpibReader)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGpibReader)
	afx_msg void OnWritegpib();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
