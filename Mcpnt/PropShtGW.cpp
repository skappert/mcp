// PropShtGW.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "PropShtGW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropShtGW

IMPLEMENT_DYNAMIC(CPropShtGW, CPropertySheet)

CPropShtGW::CPropShtGW(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropShtGW::CPropShtGW(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);
	AddPage(&m_page3);
}

CPropShtGW::~CPropShtGW()
{
}


BEGIN_MESSAGE_MAP(CPropShtGW, CPropertySheet)
	//{{AFX_MSG_MAP(CPropShtGW)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropShtGW message handlers
