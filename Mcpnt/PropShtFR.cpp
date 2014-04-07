// PropShtFR.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "PropShtFR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropShtFR

IMPLEMENT_DYNAMIC(CPropShtFR, CPropertySheet)

CPropShtFR::CPropShtFR(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropShtFR::CPropShtFR(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	//AddPage(&m_page1);
	AddPage(&m_page3);
}

CPropShtFR::~CPropShtFR()
{

}


BEGIN_MESSAGE_MAP(CPropShtFR, CPropertySheet)
	//{{AFX_MSG_MAP(CPropShtFR)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropShtFR message handlers
