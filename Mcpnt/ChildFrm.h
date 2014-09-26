// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:

// Operations
public:
	LRESULT OnToolbarReset(WPARAM wp,LPARAM);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	CMFCStatusBar  m_wndStatusBar;
	CMenu		m_ShortMenu;

	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	BOOL		CloseEnabled;
	CMFCToolBar	m_ToolBar;
	CMFCToolBar m_SaveDialog;
	CEdit*		pSaveEdit;
	CFont		m_font;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg void OnStatus(CCmdUI *pCmdUI);
	afx_msg LRESULT OnSetPane(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnDisableClose(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnEnableClose(WPARAM wparam,LPARAM lparam);
	//{{AFX_MSG(CChildFrame)
	afx_msg void OnSavesettings();
	afx_msg void OnClose();
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
