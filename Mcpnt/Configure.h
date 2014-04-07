// Configure.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigure dialog

class CConfigure : public CDialog
{
// Construction
public:
	CConfigure(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigure)
	enum { IDD = IDD_CONFIGURE };
	BOOL	m_lines;
	BOOL	m_points;
	BOOL	m_stairs;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigure)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigure)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
