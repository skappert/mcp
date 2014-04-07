// CamacTester.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "CamacTester.h"
#include "Camac.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCamacTester dialog


CCamacTester::CCamacTester(CWnd* pParent /*=NULL*/)
	: CDialog(CCamacTester::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCamacTester)
	m_a = 0;
	m_data = 0;
	m_f = 0;
	m_n = 1;
	m_answer = _T("");
	//}}AFX_DATA_INIT
}


void CCamacTester::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCamacTester)
	DDX_Text(pDX, IDC_A, m_a);
	DDX_Text(pDX, IDC_DATA, m_data);
	DDX_Text(pDX, IDC_F, m_f);
	DDX_Text(pDX, IDC_N, m_n);
	DDX_Text(pDX, IDC_CAMACANSWER, m_answer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCamacTester, CDialog)
	//{{AFX_MSG_MAP(CCamacTester)
	ON_BN_CLICKED(IDC_SENDNAF, OnSendnaf)
	ON_BN_CLICKED(IDC_RANDOM, OnRandom)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCamacTester message handlers

void CCamacTester::OnSendnaf() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int answer;
	CString Status;
	//do
	{
		SendDNAFCamac(m_data,m_n,m_a,m_f);
		answer = ReadCSRCamac();
		//if (answer < 128) m_a++;
		//if(m_a == 32)m_f++;
		//if(m_f == 47)m_n++;
		//if(m_n == 25) break;
		//m_a = m_a & 31;
		//m_f = m_f & 31;
	}//while(answer < 0);
	answer = answer & 129;
	if(answer == 129) Status = "X = 1 Q = 1";
	else Status = "X = 1 Q = 0";
	m_answer.Format("%s : Data = %u",Status,ReadDCamac());
	ReadCSRCamac();
	
	UpdateData(FALSE);
}

void CCamacTester::OnRandom() 
{
	// TODO: Add your control notification handler code here
	CString Status = "";
	USHORT i;
	UpdateData(TRUE);
	int answer;
	for(i=0;i<20;i++)
	{
		m_a=rand()&31;
		m_f=rand()&31;
		do
		{
			SendDNAFCamac(rand(),m_n,
								m_a,
								m_f);
			answer = ReadCSRCamac();
			if (answer<128) m_a++;
			if(m_a == 32)m_f++;
			if(m_f == 32) 
			{
				m_a = 0;
				m_f = 0;
			}break;
			m_a = m_a & 31;
			m_f = m_f & 31;
			answer = answer & 129;
		}while(answer<128);
	}
	if(answer == 129) Status = "X = 1 Q = 1";
	else Status = "X = 1 Q = 0";
	m_answer.Format("%s : Data = %u",Status,ReadDCamac());
	
	UpdateData(FALSE);	
}

void CCamacTester::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();
	delete this;
}
