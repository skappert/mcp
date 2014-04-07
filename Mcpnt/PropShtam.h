// PropShtam.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropShtam
#include "Page1AM.h"
#include "Page3AM.h"
class CPropShtam : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropShtam)

// Construction
public:
	CPage1am m_page1;

	CPage3am m_page3;
	
	CPropShtam(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropShtam(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropShtam)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropShtam();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropShtam)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
