// PropShtGR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropShtGR
#include "Page1GR.h"
#include "Page3GR.h"
class CPropShtGR : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropShtGR)

// Construction
public:
	CPage1GR m_page1;
	CPage3GR m_page3;

	CPropShtGR(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropShtGR(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropShtGR)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropShtGR();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropShtGR)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
