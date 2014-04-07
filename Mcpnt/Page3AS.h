// Page3AS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage3AS dialog

class CPage3AS : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3AS)

// Construction
public:
	CPage3AS();
	~CPage3AS();

// Dialog Data
	//{{AFX_DATA(CPage3AS)
	enum { IDD = IDD_PAGE3AS };
	int		m_slot;
	int		m_subaddress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3AS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3AS)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
