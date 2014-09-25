// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	int m_nDatePaneNo;
	int m_nTimePaneNo;
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
private:
	BOOL IconToggle;
	BOOL HTBACK;
	BOOL IconToggle2;
	BOOL PROTONSBACK;
	BOOL InitStatusBar(UINT *pIndicators, int nSize, int nSeconds);
	BOOL LoadFocus;
	BOOL SaveFocus;
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	afx_msg void OnUpdateDate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTime(CCmdUI* pCmdUI);
	void OnUpdateViewProtons(CCmdUI* pCmdUI);
	void OnUpdateViewHT(CCmdUI* pCmdUI);
	void OnMeasHT();
	void OnMeasProtons();
	void OnReturn();

	CMFCMenuBar	m_wndMenuBar;
	CMFCStatusBar m_wndStatusBar;
	CMFCToolBar	m_wndGPS_HTMEAS;
	bool m_ShowGPS_HTMEAS;
	CMFCToolBar	m_wndLoadFileBar;
	bool m_ShowLoadFileBar;
	CMFCToolBar	m_wndISOLDE_PROTONS;
	bool m_ShowISOLDE_PROTONS;
	CMFCToolBar	m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSavesettings();
	afx_msg void OnViewTemplatecalculator();
	afx_msg void OnViewScriptwindow();
	afx_msg void OnViewEmptyplot();
	afx_msg void OnViewDisplaysetup();
	afx_msg void OnViewCamactester();
	afx_msg void OnViewNetreader();
	afx_msg void OnViewSiclReader();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGpibwriter();
	afx_msg void OnGpibreader();
	afx_msg void OnSiclreader();
	afx_msg void OnReset();
	afx_msg void OnViewNetwriter();
	//}}AFX_MSG
	afx_msg void OnSelChange();
	afx_msg void OnDropDown();
	afx_msg void OnGetFocus();
	afx_msg void OnKillFocus();
	afx_msg void OnLoadCombo(UINT nID);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
