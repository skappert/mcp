// PropShtBo.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "PropShtBo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropShtBo

IMPLEMENT_DYNAMIC(CPropShtBo, CPropertySheet)

CPropShtBo::CPropShtBo(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropShtBo::CPropShtBo(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);
	AddPage(&m_page3);
}

CPropShtBo::~CPropShtBo()
{
}


BEGIN_MESSAGE_MAP(CPropShtBo, CPropertySheet)
	//{{AFX_MSG_MAP(CPropShtBo)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropShtBo message handlers
