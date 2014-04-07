// PropShtLV.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropShtLV
#include "Page1LV.h"
#include "Page2LV.h"
#include "Page3LV.h"

class CPropShtLV : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropShtLV)

// Construction
public:
	CPage1LV m_page1;
	CPage2LV m_page2;
	CPage3LV m_page3;

	CPropShtLV(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropShtLV(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropShtLV)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropShtLV();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPropShtLV)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
