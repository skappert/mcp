// PropShtfs.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropShtfs

#include "Page1FS.h"
#include "Page3FS.h"

class CPropShtfs : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropShtfs)

// Construction
public:
	CPage1fs m_page1;

	CPage3fs m_page3;


	CPropShtfs(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropShtfs(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropShtfs)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropShtfs();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropShtfs)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
