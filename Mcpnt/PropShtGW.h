// PropShtGW.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropShtGW
#include "Page1GW.h"
#include "Page3GW.h"
class CPropShtGW : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropShtGW)

// Construction
public:
	CPage1GW m_page1;
	CPage3GW m_page3;

	CPropShtGW(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropShtGW(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropShtGW)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropShtGW();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropShtGW)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
