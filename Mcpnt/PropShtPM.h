// PropShtPM.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropShtPM
#include "Page1PM.h"
#include "Page3PM.h"

class CPropShtPM : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropShtPM)

// Construction
public:
	CPage1PM m_page1;

	CPage3PM m_page3;
	

	CPropShtPM(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropShtPM(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropShtPM)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropShtPM();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropShtPM)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
