// NetworkWriter.cpp : implementation file
//

#include "stdafx.h"
#include "mcp for nt.h"
#include "NetworkWriter.h"
#include "rpcapi32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkWriter dialog


CNetworkWriter::CNetworkWriter(CWnd* pParent /*=NULL*/)
	: CDialog(CNetworkWriter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetworkWriter)
	m_name = _T("");
	m_property = _T("CCV");
	m_value = _T("");
	//}}AFX_DATA_INIT
}


void CNetworkWriter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetworkWriter)
	DDX_Text(pDX, IDC_NAME, m_name);
	DDX_Text(pDX, IDC_PROPERTY, m_property);
	DDX_Text(pDX, IDC_VALUE, m_value);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetworkWriter, CDialog)
	//{{AFX_MSG_MAP(CNetworkWriter)
	ON_BN_CLICKED(IDC_WRITENET, OnWritenet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkWriter message handlers

void CNetworkWriter::OnWritenet() 
{
	// TODO: Add your control notification handler code here
	double Data;
	short cc;
	char buffer[255];
	char name[255];
	char property[255];

	UpdateData(TRUE);

    strcpy(name,m_name);
	strcpy(property,m_property);
	Data = atof(m_value);
	{
		cc=SyncRPC(&name[0],&property[0],"",-1,&Data,sizeof(Data),CF_DOUBLE,CF_TEXT);
		if (cc) GetRPCLastError(&buffer[0],255);
		else sprintf(buffer,"\n%s / %s = %lf\n\n",name,property,Data);
	}

	//m_netanswer = buffer;

	UpdateData(FALSE);
}

void CNetworkWriter::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	CDialog::PostNcDestroy();
	delete this;
}
