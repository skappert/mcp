// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	CDialogBar  m_wndSaveDialog;
	CStatusBar  m_wndStatusBar;

	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	BOOL		CloseEnabled;
	CToolBar	m_ToolBar;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg void OnStatus(CCmdUI *pCmdUI);
	afx_msg LRESULT OnSetPane(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnDisableClose(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnEnableClose(WPARAM wparam,LPARAM lparam);
	//{{AFX_MSG(CChildFrame)
	afx_msg void OnSavesettings();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
