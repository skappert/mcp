// EditorView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditorView view

class CEditorView : public CEditView
{
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ResizeWin(ULONG x,ULONG y);
	CEditorView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEditorView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditorView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	CFont* NewFont;
	BOOL   IsActivated;
	CEditorChild* pEditorChild;
	virtual ~CEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditorView)
	afx_msg void OnChange();
	afx_msg void OnUpdate();
	afx_msg void OnPopupLoad();
	afx_msg void OnPopupSave();
	afx_msg void OnPopupClearwindow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
