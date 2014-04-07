// PropShtTI.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "PropShtTI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropShtTI

IMPLEMENT_DYNAMIC(CPropShtTI, CPropertySheet)

CPropShtTI::CPropShtTI(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropShtTI::CPropShtTI(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);
	AddPage(&m_page3);
}

CPropShtTI::~CPropShtTI()
{
}


BEGIN_MESSAGE_MAP(CPropShtTI, CPropertySheet)
	//{{AFX_MSG_MAP(CPropShtTI)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropShtTI message handlers
