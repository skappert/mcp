// TemplateAdder.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTemplateAdder dialog

class CTemplateAdder : public CDialog
{
// Construction
public:
	CTemplateAdder(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTemplateAdder)
	enum { IDD = IDD_TEMPLATEADDER };
	CListCtrl	m_addlist;
	CString	m_filelist;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTemplateAdder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTemplateAdder)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
