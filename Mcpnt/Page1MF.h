// Page1MF.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1MF dialog

class CPage1MF : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1MF)

// Construction
public:
	CPage1MF();
	~CPage1MF();

// Dialog Data
	//{{AFX_DATA(CPage1MF)
	enum { IDD = IDD_PAGE1MF };
	BOOL	m_fluke1;
	BOOL	m_fluke2;
	BOOL	m_fluke3;
	BOOL	m_nofluke;
	BOOL	m_nomass;
	float	m_setamu;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1MF)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1MF)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
