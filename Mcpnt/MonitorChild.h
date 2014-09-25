// MonitorChild.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MonitorChild frame

class MonitorChild : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(MonitorChild)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	MonitorChild();


	MonitorChild(CView* dDataView);
private:
	CView* pDataView;
// protected constructor used by dynamic creation
	static MonitorChild* pDataChild;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MonitorChild)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CMDIFrameWndEx* pParentWnd = NULL, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
private:
	virtual ~MonitorChild();
	// Generated message map functions
	//{{AFX_MSG(MonitorChild)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	CPaneDialog m_wndErrorbar;
	CPaneDialog m_wndDataStyle;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CWnd* m_pParent;
	afx_msg void OnContextMenu(CWnd*, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////
