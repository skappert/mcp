// MinuitThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CMinuitThread thread

class CMinuitThread : public CWinThread
{
	DECLARE_DYNCREATE(CMinuitThread)
protected:
	CMinuitThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMinuitThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMinuitThread();

	// Generated message map functions
	//{{AFX_MSG(CMinuitThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
