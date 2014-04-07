// PropShtLV.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "PropShtLV.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropShtLV

IMPLEMENT_DYNAMIC(CPropShtLV, CPropertySheet)

CPropShtLV::CPropShtLV(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropShtLV::CPropShtLV(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);
	AddPage(&m_page2);
	AddPage(&m_page3);
}

CPropShtLV::~CPropShtLV()
{
}


BEGIN_MESSAGE_MAP(CPropShtLV, CPropertySheet)
	//{{AFX_MSG_MAP(CPropShtLV)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropShtLV message handlers
