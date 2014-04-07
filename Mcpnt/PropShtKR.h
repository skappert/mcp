// PropShtKR.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropShtKR
#include "Page1KR.h"
#include "Page3KR.h"
class CPropShtKR : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropShtKR)

// Construction
public:
	//CPage1KR m_page1;
	CPage3KR m_page3;

	CPropShtKR(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropShtKR(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropShtKR)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropShtKR();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropShtKR)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
