// PropShtHV.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropShtHV
#include "Page1HV.h"
#include "Page2HV.h"
#include "Page3HV.h"

class CPropShtHV : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropShtHV)

// Construction
public:
	CPage1HV m_page1;
	CPage2HV m_page2;
	CPage3HV m_page3;


	CPropShtHV(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropShtHV(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropShtHV)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropShtHV();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropShtHV)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
