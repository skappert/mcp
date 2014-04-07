// NetReader.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNetReader dialog

class CNetReader : public CDialog
{
// Construction
public:
	CNetReader(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNetReader)
	enum { IDD = IDD_NETREAD };
	CString	m_name;
	CString	m_netanswer;
	CString	m_property;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetReader)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNetReader)
	afx_msg void OnReadnet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
