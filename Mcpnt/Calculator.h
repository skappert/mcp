// Calculator.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCalculator dialog

class CMCPforNTDoc;

class CCalculator : public CDialog
{
// Construction
public:
	CCalculator(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalculator)
	enum { IDD = IDD_TEMPLATECALCUL };
	CListCtrl	m_templview;
	CListCtrl	m_addlist;
	CListBox	m_listbox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalculator)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation

private: 
	CDialog m_calculator; 
	CPtrList pDoc;
	CMCPforNTDoc* SumDoc;
	void	UpdateTree(CMCPforNTDoc* pDocument);
	BOOL	CompareDocs(CMCPforNTDoc* pDoc1,CMCPforNTDoc* pDoc2);	
	int		GetSelectedItem(void);
	int		GetMarkedItem(void); 
	void	ReCalcSum(void); 
	CImageList	*m_xpimagelistSmall;
	CImageList	*m_xpimagelist2Small;
   	CImageList	*m_pimagelist;

protected:

	// Generated message map functions
	//{{AFX_MSG(CCalculator)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnSub();
	afx_msg void OnSaveadded();
	afx_msg void OnAddbutsubstract();
	afx_msg void OnClose();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
