// PropShtPM.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "PropShtPM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropShtPM

IMPLEMENT_DYNAMIC(CPropShtPM, CPropertySheet)

CPropShtPM::CPropShtPM(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CPropShtPM::CPropShtPM(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_page1);
	AddPage(&m_page3);

}

CPropShtPM::~CPropShtPM()
{
}


BEGIN_MESSAGE_MAP(CPropShtPM, CPropertySheet)
	//{{AFX_MSG_MAP(CPropShtPM)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropShtPM message handlers
