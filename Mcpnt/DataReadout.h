// DataReadout.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataReadout dialog

class CDataReadout : public CDialog
{
// Construction
public:
	CDataReadout(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDataReadout)
	enum { IDD = IDD_DATAREADOUT };
	CButton	m_centerinsweep;
	CString	m_texterr;
	CString	m_textx;
	CString	m_texty;
	CString	m_numerr;
	CString	m_numx;
	CString	m_numy;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataReadout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataReadout)
	afx_msg void OnCenterinsweep();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
