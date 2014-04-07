// PropShtHP.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "PropShtHP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropShtHP

IMPLEMENT_DYNAMIC(CPropShtHP, CPropertySheet)

CPropShtHP::CPropShtHP(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropShtHP::CPropShtHP(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);
	AddPage(&m_page2);
	AddPage(&m_page3);
}

CPropShtHP::~CPropShtHP()
{
}


BEGIN_MESSAGE_MAP(CPropShtHP, CPropertySheet)
	//{{AFX_MSG_MAP(CPropShtHP)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropShtHP message handlers
