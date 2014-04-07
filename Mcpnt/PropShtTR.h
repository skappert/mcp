// PropShtTR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropShtTR
#include "Page1TR.h"
#include "Page3TR.h"

class CPropShtTR : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropShtTR)

// Construction
public:
	CPage1TR m_page1;
	CPage3TR m_page3;

	CPropShtTR(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropShtTR(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropShtTR)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropShtTR();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropShtTR)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
