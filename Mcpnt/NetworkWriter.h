// NetworkWriter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNetworkWriter dialog

class CNetworkWriter : public CDialog
{
// Construction
public:
	CNetworkWriter(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNetworkWriter)
	enum { IDD = IDD_NETWRITE };
	CString	m_name;
	CString	m_property;
	CString	m_value;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkWriter)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNetworkWriter)
	afx_msg void OnWritenet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
