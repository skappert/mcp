// PropShtPR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropShtPR
#include "Page1PR.h"
#include "Page3PR.h"

class CPropShtPR : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropShtPR)

// Construction
public:
	CPage1PR m_page1;

	CPage3PR m_page3;

	CPropShtPR(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropShtPR(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropShtPR)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropShtPR();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropShtPR)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
