// GPSHtmeas.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGPSHtmeas dialog

class CGPSHtmeas : public CDialog
{
// Construction
public:
	CGPSHtmeas(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGPSHtmeas)
	enum { IDD = CG_IDD_GPS_HTMEAS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGPSHtmeas)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGPSHtmeas)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
