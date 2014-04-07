// PropShtHV.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "PropShtHV.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropShtHV

IMPLEMENT_DYNAMIC(CPropShtHV, CPropertySheet)

CPropShtHV::CPropShtHV(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropShtHV::CPropShtHV(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);
	AddPage(&m_page2);
	AddPage(&m_page3);
}

CPropShtHV::~CPropShtHV()
{
}


BEGIN_MESSAGE_MAP(CPropShtHV, CPropertySheet)
	//{{AFX_MSG_MAP(CPropShtHV)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropShtHV message handlers
