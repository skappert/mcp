// SiclReader.h : header file
//

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

/////////////////////////////////////////////////////////////////////////////
// CSiclReader dialog

class CSiclReader : public CDialog
{
// Construction
public:
	CSiclReader(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSiclReader)
	enum { IDD = IDD_SICLREAD };
	CString	m_address;
	CString	m_siclanswer;
	CString	m_siclstring;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSiclReader)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSiclReader)
	afx_msg void OnWritesicl();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
