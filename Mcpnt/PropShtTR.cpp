// PropShtTR.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "PropShtTR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropShtTR

IMPLEMENT_DYNAMIC(CPropShtTR, CPropertySheet)

CPropShtTR::CPropShtTR(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropShtTR::CPropShtTR(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);
	AddPage(&m_page3);

}

CPropShtTR::~CPropShtTR()
{
}


BEGIN_MESSAGE_MAP(CPropShtTR, CPropertySheet)
	//{{AFX_MSG_MAP(CPropShtTR)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropShtTR message handlers
