// PropShtFW.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "PropShtFW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropShtFW

IMPLEMENT_DYNAMIC(CPropShtFW, CPropertySheet)

CPropShtFW::CPropShtFW(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropShtFW::CPropShtFW(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);
	AddPage(&m_page3);
}

CPropShtFW::~CPropShtFW()
{
}


BEGIN_MESSAGE_MAP(CPropShtFW, CPropertySheet)
	//{{AFX_MSG_MAP(CPropShtFW)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropShtFW message handlers
