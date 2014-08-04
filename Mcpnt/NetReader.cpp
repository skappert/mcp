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
	//{{AFX_DATA_INIT(CNetReader)
	m_name = _T("");
	m_netanswer = _T("");
	m_property = _T("AQN");
	//}}AFX_DATA_INIT

	dip = Dip::create("sample-ISOLDE-client");
	handler = new GeneralDataListener(this);
	//Creating an array of DipSubscriptions.
	sub = new DipSubscription*[1];
	dip->setDNSNode("dipnsgpn1,dipnsgpn2");
	sub[0] = dip->createDipSubscription("dip/acc/ISO/HT1.HTCTL/AQN1", handler);
}


void CNetReader::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetReader)
	DDX_Text(pDX, IDC_NAME, m_name);
	DDV_MaxChars(pDX, m_name, 40);
	DDX_Text(pDX, IDC_NETANSWER, m_netanswer);
	DDX_Text(pDX, IDC_PROPERTY, m_property);
	DDV_MaxChars(pDX, m_property, 40);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetReader, CDialog)
	//{{AFX_MSG_MAP(CNetReader)
	ON_BN_CLICKED(IDC_READNET, OnReadnet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetReader message handlers

void CNetReader::OnReadnet() 

{
	double Data;
	short cc;
	char buffer[255];
	char name[255];
	char property[255];

	UpdateData(TRUE);

    strcpy(name,m_name);
	strcpy(property,m_property);
	{
		//cc=SyncRPC(&name[0],&property[0],"",1,(void *)&Data,sizeof(double),CF_DOUBLE,0);
		//if (cc) GetRPCLastError(&buffer[0],255);
		//else sprintf(buffer,"\n%s / %s = %lf\n\n",name,property,Data);
	}

	m_netanswer = buffer;

	UpdateData(FALSE);
}

void CNetReader::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}
