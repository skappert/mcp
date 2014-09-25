// DataChild.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DataChild frame

class DataChild : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(DataChild)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DataChild();


	DataChild(CView* dDataView);
private:
	CView* pDataView;
// protected constructor used by dynamic creation
	static DataChild* pDataChild;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DataChild)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CMDIFrameWndEx* pParentWnd = NULL, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
private:
	virtual ~DataChild();
	// Generated message map functions
	//{{AFX_MSG(DataChild)
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
