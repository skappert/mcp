// PropShtSR.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "PropShtSR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropShtSR

IMPLEMENT_DYNAMIC(CPropShtSR, CPropertySheet)

CPropShtSR::CPropShtSR(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropShtSR::CPropShtSR(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);
	AddPage(&m_page3);
}

CPropShtSR::~CPropShtSR()
{
}


BEGIN_MESSAGE_MAP(CPropShtSR, CPropertySheet)
	//{{AFX_MSG_MAP(CPropShtSR)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropShtSR message handlers
