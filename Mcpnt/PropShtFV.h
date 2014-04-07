// PropShtFV.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropShtFV
#include "Page1FV.h"
#include "Page2FV.h"
#include "Page3FV.h"

class CPropShtFV : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropShtFV)

// Construction
public:
	CPage1FV m_page1;
	CPage2FV m_page2;
	CPage3FV m_page3;


	CPropShtFV(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropShtFV(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropShtFV)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropShtFV();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropShtFV)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
