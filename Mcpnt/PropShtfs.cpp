// PropShtfs.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "PropShtfs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropShtfs

IMPLEMENT_DYNAMIC(CPropShtfs, CPropertySheet)

CPropShtfs::CPropShtfs(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropShtfs::CPropShtfs(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);
	AddPage(&m_page3);
}

CPropShtfs::~CPropShtfs()
{
}


BEGIN_MESSAGE_MAP(CPropShtfs, CPropertySheet)
	//{{AFX_MSG_MAP(CPropShtfs)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropShtfs message handlers
