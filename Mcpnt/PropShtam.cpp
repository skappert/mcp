// PropShtam.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "PropShtam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropShtam

IMPLEMENT_DYNAMIC(CPropShtam, CPropertySheet)

CPropShtam::CPropShtam(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropShtam::CPropShtam(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);

	AddPage(&m_page3);
}

CPropShtam::~CPropShtam()
{
}


BEGIN_MESSAGE_MAP(CPropShtam, CPropertySheet)
	//{{AFX_MSG_MAP(CPropShtam)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropShtam message handlers
