// PropShtKR.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "PropShtKR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropShtKR

IMPLEMENT_DYNAMIC(CPropShtKR, CPropertySheet)

CPropShtKR::CPropShtKR(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropShtKR::CPropShtKR(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	//AddPage(&m_page1);
	AddPage(&m_page3);
}

CPropShtKR::~CPropShtKR()
{
}


BEGIN_MESSAGE_MAP(CPropShtKR, CPropertySheet)
	//{{AFX_MSG_MAP(CPropShtKR)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropShtKR message handlers
