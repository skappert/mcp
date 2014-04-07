// PropShtKW.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropShtKW
#include "Page1KW.h"
#include "Page3KW.h"
class CPropShtKW : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropShtKW)

// Construction
public:
	CPage1KW m_page1;
	CPage3KW m_page3;

	CPropShtKW(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropShtKW(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropShtKW)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropShtKW();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropShtKW)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
