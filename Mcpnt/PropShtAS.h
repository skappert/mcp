// PropShtAS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropShtAS
#include "Page1AS.h"
#include "Page3AS.h"

class CPropShtAS : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropShtAS)

// Construction
public:
	CPage1AS m_page1;

	CPage3AS m_page3;


	CPropShtAS(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropShtAS(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropShtAS)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropShtAS();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropShtAS)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
