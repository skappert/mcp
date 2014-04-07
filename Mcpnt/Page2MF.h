// Page2MF.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage2MF dialog

class CPage2MF : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage2MF)

// Construction
public:
	CPage2MF();
	~CPage2MF();

// Dialog Data
	//{{AFX_DATA(CPage2MF)
	enum { IDD = IDD_PAGE2MF };
	BOOL	m_fluke1;
	BOOL	m_fluke2;
	BOOL	m_fluke3;
	BOOL	m_nofluke;
	BOOL	m_nomass;
	float	m_setamu;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage2MF)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage2MF)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
