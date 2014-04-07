// PropShtMS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropShtMS
#include "Page1MS.h"
#include "Page3MS.h"
class CPropShtMS : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropShtMS)

// Construction
public:
	CPage1MS m_page1;

	CPage3MS m_page3;

	CPropShtMS(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropShtMS(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropShtMS)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropShtMS();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropShtMS)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
