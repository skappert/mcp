// EditorChild.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditorChild frame

class CEditorChild : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CEditorChild)
public:
	CEditorChild();           // protected constructor used by dynamic creation
	CEditorChild(CView* dView);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditorChild)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CView* pEditorView;
	virtual ~CEditorChild();

	// Generated message map functions
	//{{AFX_MSG(CEditorChild)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
