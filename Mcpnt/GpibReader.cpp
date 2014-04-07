// GpibReader.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "GpibReader.h"
#include "camac.h"
#include "Hardware.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGpibReader dialog


CGpibReader::CGpibReader(CWnd* pParent /*=NULL*/)
	: CDialog(CGpibReader::IDD, pParent)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	//{{AFX_DATA_INIT(CGpibReader)
	m_address = pApp->ReadGpib;
	m_gpibstring = pApp->GPIBRead;
	m_gpibanswer = _T("");
	//}}AFX_DATA_INIT
}


void CGpibReader::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGpibReader)
	DDX_Text(pDX, IDC_ADDRESS, m_address);
	DDV_MinMaxUInt(pDX, m_address, 1, 31);
	DDX_Text(pDX, IDC_GPIBANSWER, m_gpibanswer);
	DDV_MaxChars(pDX, m_gpibanswer, 100);
	DDX_Text(pDX, IDC_GPIBSTRING, m_gpibstring);
	DDV_MaxChars(pDX, m_gpibstring, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGpibReader, CDialog)
	//{{AFX_MSG_MAP(CGpibReader)
	ON_BN_CLICKED(IDC_WRITEGPIB, OnWritegpib)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGpibReader message handlers

void CGpibReader::OnWritegpib() 
{
	// TODO: Add your control notification handler code here
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	if(!pApp->GlobalMeasurementRunning&&pApp->CamacOn)
	{
		UpdateData(TRUE);
		pApp->ReadGpib = m_address;
		pApp->GPIBRead = m_gpibstring;
		m_gpibanswer	= "";
		UpdateData(FALSE);
		m_gpibanswer = SendReadGPIB(m_gpibstring,m_address,pApp->GpibSlot); 
	}
	UpdateData(FALSE);
}


void CGpibReader::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}
