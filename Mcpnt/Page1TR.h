// Page1TR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage1TR dialog

class CPage1TR : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage1TR)

// Construction
public:
	CPage1TR();
	~CPage1TR();

// Dialog Data
	//{{AFX_DATA(CPage1TR)
	enum { IDD = IDD_PAGE1TR };
	int		m_numscans;
	int		m_numsteps;
	long	m_protonlimit;
	int		m_realscans;
	BOOL	m_noredo;
	BOOL	m_redostop;
	BOOL	m_redowait;
	BOOL	m_redowaitredo;
	CString	m_isotope;
	UINT	m_cycles;
	CString	m_element;
	double	m_wavenumber;
	double	m_kepcofactor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage1TR)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage1TR)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
