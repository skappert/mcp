// PropShtSR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropShtSR
#include "Page1SR.h"
#include "Page3SR.h"
class CPropShtSR : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropShtSR)

// Construction
public:
	CPage1SR m_page1;
	CPage3SR m_page3;

	CPropShtSR(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropShtSR(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropShtSR)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropShtSR();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropShtSR)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
