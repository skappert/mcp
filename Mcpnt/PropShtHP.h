// PropShtHP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropShtHP
#include "Page1HP.h"
#include "Page2HP.h"
#include "Page3HP.h"

class CPropShtHP : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropShtHP)

// Construction
public:
	CPage1HP m_page1;
	CPage2HP m_page2;
	CPage3HP m_page3;

	
	CPropShtHP(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropShtHP(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropShtHP)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropShtHP();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropShtHP)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
