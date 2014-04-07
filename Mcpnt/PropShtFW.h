// PropShtFW.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropShtFW
#include "Page1FW.h"
#include "Page3FW.h"
class CPropShtFW : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropShtFW)

// Construction
public:
	CPage1FW m_page1;
	CPage3FW m_page3;

	CPropShtFW(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropShtFW(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropShtFW)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropShtFW();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropShtFW)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
