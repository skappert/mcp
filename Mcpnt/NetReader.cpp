// NetReader.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "NetReader.h"
#include "Dip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetReader dialog


CNetReader::CNetReader(CWnd* pParent /*=NULL*/)
	: CDialog(CNetReader::IDD, pParent)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	
	//{{AFX_DATA_INIT(CNetReader)
	m_name = pApp->GetProfileString("DIPReader", "name");
	m_netanswer = _T("");
	//}}AFX_DATA_INIT

	if(m_name.IsEmpty()) m_name = "dip/acc/ISO/HT1.HTCTL/AQN1";

	dip = Dip::create("MCP_ISOLDE_COLLAPS_NETREADER");
	handler = new GeneralDataListener(this);
	
	dip->setDNSNode("dipnsgpn1,dipnsgpn2");
}


void CNetReader::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetReader)
	DDX_Text(pDX, IDC_NAME, m_name);
	DDV_MaxChars(pDX, m_name, 200);
	DDX_Text(pDX, IDC_NETANSWER, m_netanswer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetReader, CDialog)
	//{{AFX_MSG_MAP(CNetReader)
	ON_BN_CLICKED(IDC_READNET, OnReadnet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetReader message handlers

void CNetReader::HandleNetMessage( CString topic, double value )
{
	if(m_name == topic)
	{
		m_netanswer.Format("%g", value);
		UpdateData(FALSE);
	}
	else
	{
		TRACE1("Received unexpected topic %s\n", topic);
	}
}

void CNetReader::OnReadnet() 
{
	UpdateData(TRUE);

    sub = new DipSubscription*[1];
	sub[0] = dip->createDipSubscription(m_name, handler);

	m_netanswer = "";

	UpdateData(FALSE);
}

void CNetReader::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}
