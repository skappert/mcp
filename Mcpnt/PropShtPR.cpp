// PropShtPR.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "PropShtPR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropShtPR

IMPLEMENT_DYNAMIC(CPropShtPR, CPropertySheet)

CPropShtPR::CPropShtPR(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropShtPR::CPropShtPR(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);
	AddPage(&m_page3);
}

CPropShtPR::~CPropShtPR()
{
}


BEGIN_MESSAGE_MAP(CPropShtPR, CPropertySheet)
	//{{AFX_MSG_MAP(CPropShtPR)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropShtPR message handlers
