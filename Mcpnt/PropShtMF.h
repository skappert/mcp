// PropShtMF.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropShtMF
#include "Page1MF.h"
#include "Page3MF.h"

class CPropShtMF : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropShtMF)

// Construction
public:
	CPage1MF m_page1;

	CPage3MF m_page3;

	CPropShtMF(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropShtMF(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropShtMF)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropShtMF();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropShtMF)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
