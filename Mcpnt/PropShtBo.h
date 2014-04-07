// PropShtBo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropShtBo
#include "Page1Bo.h"
#include "Page3Bo.h"
class CPropShtBo : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropShtBo)

// Construction
public:
	CPage1Bo m_page1;
	CPage3Bo m_page3;

	CPropShtBo(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropShtBo(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropShtBo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropShtBo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropShtBo)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
