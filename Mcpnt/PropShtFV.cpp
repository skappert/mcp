// PropShtFV.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "PropShtFV.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropShtFV

IMPLEMENT_DYNAMIC(CPropShtFV, CPropertySheet)

CPropShtFV::CPropShtFV(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropShtFV::CPropShtFV(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);
	AddPage(&m_page2);
	AddPage(&m_page3);
}

CPropShtFV::~CPropShtFV()
{
}


BEGIN_MESSAGE_MAP(CPropShtFV, CPropertySheet)
	//{{AFX_MSG_MAP(CPropShtFV)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropShtFV message handlers
