// MeasThread.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "MeasThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define WM_MEASUREMENTKILLED WM_USER + 304
#define WM_MEASUREMENTHALTED WM_USER + 404

/////////////////////////////////////////////////////////////////////////////
// CMeasThread dialog


CMeasThread::CMeasThread(CWnd* pParent /*=NULL*/)
	: CDialog(CMeasThread::IDD, pParent)
{
	pParentWnd = pParent;
	//{{AFX_DATA_INIT(CMeasThread)
	m_timeleft = _T("");
	//}}AFX_DATA_INIT
}


void CMeasThread::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMeasThread)
	DDX_Control(pDX, IDC_PROGSTEP, m_progstep);
	DDX_Text(pDX, IDC_TIMELEFT, m_timeleft);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMeasThread, CDialog)
	//{{AFX_MSG_MAP(CMeasThread)
	ON_BN_CLICKED(IDKILL, OnKill)
	ON_BN_CLICKED(ID_HALTMEAS, OnHaltmeas)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeasThread message handlers


void CMeasThread::OnCancel() 
{
	// TODO: Add extra cleanup here
	DestroyWindow();
}

void CMeasThread::OnKill() 
{
	// TODO: Add your control notification handler code here
	pParentWnd->PostMessage(WM_MEASUREMENTKILLED,0,0);
}

void CMeasThread::OnHaltmeas() 
{
	// TODO: Add your control notification handler code here
	pParentWnd->PostMessage(WM_MEASUREMENTHALTED,0,0);
}
