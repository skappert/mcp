// MinuitChild.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMinuitChild frame

class CMinuitChild : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CMinuitChild)
public:
	CMinuitChild();           // protected constructor used by dynamic creation
	CMinuitChild(CView* dMinuitView);
// Attributes
public:

// Operations
private:
	CView* pMinuitView;
	static CMinuitChild* pMinuitChild;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMinuitChild)
	//}}AFX_VIRTUAL

// Implementation
protected:
	CSplitterWnd m_wndSplitter;
	virtual ~CMinuitChild();
	CWnd* m_pParent;

	// Generated message map functions
	//{{AFX_MSG(CMinuitChild)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
