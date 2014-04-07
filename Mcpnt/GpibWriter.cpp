// GpibWriter.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "GpibWriter.h"
#include "camac.h"
#include "Hardware.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGpibWriter dialog


CGpibWriter::CGpibWriter(CWnd* pParent /*=NULL*/)
	: CDialog(CGpibWriter::IDD, pParent)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	//{{AFX_DATA_INIT(CGpibWriter)
	m_address = pApp->ReadGpib;
	m_gpibstring = pApp->GPIBWrite;
	//}}AFX_DATA_INIT
}


void CGpibWriter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGpibWriter)
	DDX_Text(pDX, IDC_ADDRESS, m_address);
	DDV_MinMaxUInt(pDX, m_address, 1, 31);
	DDX_Text(pDX, IDC_GPIBSTRING, m_gpibstring);
	DDV_MaxChars(pDX, m_gpibstring, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGpibWriter, CDialog)
	//{{AFX_MSG_MAP(CGpibWriter)
	ON_BN_CLICKED(IDC_WRITEGPIB, OnWritegpib)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGpibWriter message handlers

void CGpibWriter::OnWritegpib() 
{
	// TODO: Add your control notification handler code here
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	if(!pApp->GlobalMeasurementRunning&&pApp->CamacOn)
	{
		UpdateData(TRUE);
		SendGPIB(m_gpibstring,m_address,pApp->GpibSlot); 
	}
	pApp->ReadGpib = m_address;
	pApp->GPIBWrite = m_gpibstring;
}


void CGpibWriter::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}
