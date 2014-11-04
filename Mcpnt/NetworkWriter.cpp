// NetworkWriter.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "NetworkWriter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkWriter dialog


CNetworkWriter::CNetworkWriter(CWnd* pParent /*=NULL*/)
	: CDialog(CNetworkWriter::IDD, pParent)
	, m_SetProperty(_T(""))
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	
	//{{AFX_DATA_INIT(CNetworkWriter)
	m_name = pApp->GetProfileString("DIPWriter", "name" );
	m_value = _T("");
	//}}AFX_DATA_INIT

	if(m_name.IsEmpty()) m_name = "dip/acc/ISO/COLLAPS/GPS.MAG70/CCV.Setter";

	m_SetProperty = pApp->GetProfileString("DIPWriter", "set_property" );
	if(m_SetProperty.IsEmpty()) m_SetProperty = "value";

	dip = Dip::create("MCP_ISOLDE_COLLAPS_NETWRITER");
	handler = new GeneralDataListener(this);
	
	dip->setDNSNode("dipnsgpn1,dipnsgpn2");
}


void CNetworkWriter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetworkWriter)
	DDX_Text(pDX, IDC_NAME, m_name);
	DDX_Text(pDX, IDC_VALUE, m_value);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_SET_PROPERTY, m_SetProperty);
}


BEGIN_MESSAGE_MAP(CNetworkWriter, CDialog)
	//{{AFX_MSG_MAP(CNetworkWriter)
	ON_BN_CLICKED(IDC_WRITENET, OnWritenet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkWriter message handlers

class WriterErrHandler:public DipPublicationErrorHandler
{
public:
	void handleException(DipPublication* publication, DipException& ex)
	{
		TRACE1("Error because %s", ex.what());
	}
};



void CNetworkWriter::HandleNetMessage( CString topic, double value )
{
	if(m_name == topic)
	{
		TRACE1("Received topic %s\n", topic);
	}
	else
	{
		TRACE1("Received unexpected topic %s\n", topic);
	}
}

void CNetworkWriter::OnWritenet() 
{
	double Data;
	
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	
	UpdateData(TRUE);

	pApp->WriteProfileString("DIPWriter", "name", m_name);
	pApp->WriteProfileString("DIPWriter", "set_property", m_SetProperty);

	Data = atof(m_value);

	DipPublication **pub = new DipPublication*[1]();
	DipData **pubData  = new DipData*[1];

	WriterErrHandler errorHandler;
	pub[0] = dip->createDipPublication(m_name,&errorHandler);
	pubData[0] = dip->createDipData();
	pubData[0]->insert(Data,m_SetProperty);
	DipTimestamp time;
	pub[0]->send(*pubData[0],time);

	dip->destroyDipPublication(pub[0]);
	delete pubData;

	UpdateData(FALSE);
}

void CNetworkWriter::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	CDialog::PostNcDestroy();
	delete this;
}
