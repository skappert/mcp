// PropShtRS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropShtRS
#include "Page1RS.h"
#include "Page2RS.h"
#include "Page3RS.h"

class CPropShtRS : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropShtRS)

// Construction
public:
	CPage1RS m_page1;
	CPage2RS m_page2;
	CPage3RS m_page3;

	CPropShtRS(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropShtRS(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropShtRS)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropShtRS();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropShtRS)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
