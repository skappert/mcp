// PropShtTI.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropShtTI
#include "Page1TI.h"
#include "Page3TI.h"

class CPropShtTI : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropShtTI)

// Construction
public:
	CPage1TI m_page1;
	CPage3TI m_page3;

	CPropShtTI(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropShtTI(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropShtTI)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropShtTI();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropShtTI)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
