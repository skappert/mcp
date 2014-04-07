// MinuitThread.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "MinuitThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMinuitThread

IMPLEMENT_DYNCREATE(CMinuitThread, CWinThread)

CMinuitThread::CMinuitThread()
{
	AfxMessageBox("Thread created",MB_OK,0);
}

CMinuitThread::~CMinuitThread()
{
	AfxMessageBox("Thread killed",MB_OK,0);
}

BOOL CMinuitThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CMinuitThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMinuitThread, CWinThread)
	//{{AFX_MSG_MAP(CMinuitThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMinuitThread message handlers
