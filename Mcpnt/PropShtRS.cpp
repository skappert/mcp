// PropShtRS.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "PropShtRS.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropShtRS

IMPLEMENT_DYNAMIC(CPropShtRS, CPropertySheet)

CPropShtRS::CPropShtRS(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropShtRS::CPropShtRS(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);
	AddPage(&m_page2);
	AddPage(&m_page3);
}

CPropShtRS::~CPropShtRS()
{
}


BEGIN_MESSAGE_MAP(CPropShtRS, CPropertySheet)
	//{{AFX_MSG_MAP(CPropShtRS)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropShtRS message handlers
