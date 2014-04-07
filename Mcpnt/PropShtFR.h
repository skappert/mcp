// PropShtFR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropShtFR
//#include "Page1FR.h"
#include "Page3FR.h"
class CPropShtFR : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropShtFR)

// Construction
public:
	//CPage1FR m_page1;
	CPage3FR m_page3;

	CPropShtFR(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropShtFR(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropShtFR)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropShtFR();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropShtFR)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
