// PropShtMS.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "PropShtMS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropShtMS

IMPLEMENT_DYNAMIC(CPropShtMS, CPropertySheet)

CPropShtMS::CPropShtMS(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropShtMS::CPropShtMS(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);
	AddPage(&m_page3);
}

CPropShtMS::~CPropShtMS()
{
}


BEGIN_MESSAGE_MAP(CPropShtMS, CPropertySheet)
	//{{AFX_MSG_MAP(CPropShtMS)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropShtMS message handlers
