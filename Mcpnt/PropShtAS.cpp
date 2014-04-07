// PropShtAS.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "PropShtAS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropShtAS

IMPLEMENT_DYNAMIC(CPropShtAS, CPropertySheet)

CPropShtAS::CPropShtAS(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropShtAS::CPropShtAS(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);

	AddPage(&m_page3);
}

CPropShtAS::~CPropShtAS()
{
}


BEGIN_MESSAGE_MAP(CPropShtAS, CPropertySheet)
	//{{AFX_MSG_MAP(CPropShtAS)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropShtAS message handlers
