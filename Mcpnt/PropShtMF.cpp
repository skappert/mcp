// PropShtMF.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "PropShtMF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropShtMF

IMPLEMENT_DYNAMIC(CPropShtMF, CPropertySheet)

CPropShtMF::CPropShtMF(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropShtMF::CPropShtMF(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);
	AddPage(&m_page3);

}

CPropShtMF::~CPropShtMF()
{
}


BEGIN_MESSAGE_MAP(CPropShtMF, CPropertySheet)
	//{{AFX_MSG_MAP(CPropShtMF)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropShtMF message handlers
