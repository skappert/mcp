// LSIChild.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLSIChild frame

class CLSIChild : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CLSIChild)
public:
	CLSIChild();           // protected constructor used by dynamic creation
	CLSIChild(CView* dLSIView);
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLSIChild)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
private:
	CView* pCLSIView;
protected:
	virtual ~CLSIChild();

	// Generated message map functions
	//{{AFX_MSG(CLSIChild)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
