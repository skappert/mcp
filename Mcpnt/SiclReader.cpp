// SiclReader.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "SiclReader.h"
#include "sicl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSiclReader dialog

CSiclReader::CSiclReader(CWnd* pParent /*=NULL*/)
	: CDialog(CSiclReader::IDD, pParent)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();

	//{{AFX_DATA_INIT(CSiclReader)
	m_address = pApp->GetProfileString("SICL", "address");
	m_siclstring = pApp->GetProfileString("SICL", "command");
	m_siclanswer = _T("");
	//}}AFX_DATA_INIT

	if(m_address.IsEmpty()) m_address = "lan[A-34461A-06386]:inst0";
	if(m_siclstring.IsEmpty()) m_siclstring = "*IDN?";
}


void CSiclReader::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSiclReader)
	DDX_Text(pDX, IDC_ADDRESS, m_address);
	DDV_MaxChars(pDX, m_address, 100);
	DDX_Text(pDX, IDC_SICLANSWER, m_siclanswer);
	DDV_MaxChars(pDX, m_siclanswer, 100);
	DDX_Text(pDX, IDC_SICLSTRING, m_siclstring);
	DDV_MaxChars(pDX, m_siclstring, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSiclReader, CDialog)
	//{{AFX_MSG_MAP(CSiclReader)
	ON_BN_CLICKED(IDC_WRITESICL, OnWritesicl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSiclReader message handlers

void CSiclReader::OnWritesicl() 
{
	INST siclhandle;
	char buf[256];

	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	
	UpdateData(TRUE);

	pApp->WriteProfileString("SICL", "address", m_address);
	pApp->WriteProfileString("SICL", "command", m_siclstring);

	if(!pApp->GlobalMeasurementRunning)
	{
		m_siclanswer	= "";
		UpdateData(FALSE);

		CT2A address(m_address);

		siclhandle = iopen (address);

		if( siclhandle > 0 )
		{
			itimeout (siclhandle, 1000);

			CT2A question(m_siclstring);

			/* trailing line feed */ 
			strcat(question,"\n");

			/* Take measurement */
			ipromptf(siclhandle, question, "%t", buf);

			m_siclanswer = buf;

			iclose (siclhandle);
		}
	}
	UpdateData(FALSE);
}


void CSiclReader::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}
