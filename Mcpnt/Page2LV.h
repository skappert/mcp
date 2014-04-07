// Page2LV.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage2LV dialog

class CPage2LV : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage2LV)

// Construction
public:
	CPage2LV();
	~CPage2LV();
	afx_msg void	OnSetVolt();
// Dialog Data
	//{{AFX_DATA(CPage2LV)
	enum { IDD = IDD_PAGE2LV };
	double	m_noscanvolt;
	double	resolution;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage2LV)
	public:
	virtual BOOL OnApply();
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage2LV)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
