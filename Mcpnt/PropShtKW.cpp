// PropShtKW.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "PropShtKW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropShtKW

IMPLEMENT_DYNAMIC(CPropShtKW, CPropertySheet)

CPropShtKW::CPropShtKW(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropShtKW::CPropShtKW(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);
	AddPage(&m_page3);
}

CPropShtKW::~CPropShtKW()
{
}


BEGIN_MESSAGE_MAP(CPropShtKW, CPropertySheet)
	//{{AFX_MSG_MAP(CPropShtKW)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropShtKW message handlers
