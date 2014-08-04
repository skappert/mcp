// Calculator.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "MCP for NTDoc.h"
#include "ActionObjects.h"
#include "Calculator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalculator dialog


CCalculator::CCalculator(CWnd* pParent /*=NULL*/)
	: CDialog(CCalculator::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalculator)
	//}}AFX_DATA_INIT
}


void CCalculator::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalculator)
	DDX_Control(pDX, IDC_TEMPLVIEW, m_templview);
	DDX_Control(pDX, IDC_ADDLIST, m_addlist);
	DDX_Control(pDX, IDC_FILELIST, m_listbox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalculator, CDialog)
	//{{AFX_MSG_MAP(CCalculator)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_SUB, OnSub)
	ON_BN_CLICKED(IDC_SAVEADDED, OnSaveadded)
	ON_BN_CLICKED(IDC_ADDBUTSUBSTRACT, OnAddbutsubstract)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalculator message handlers

BOOL CCalculator::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_listbox.Dir(0,"*.*");
	SumDoc				= NULL;
	CMCPforNTApp		*pApp;
	const				int LISTICONCOUNT = 3;
	long				lStyleOld;
	
	LV_COLUMN			column;


	lStyleOld = GetWindowLong(m_templview.m_hWnd, GWL_STYLE);
	lStyleOld &= ~(LVS_TYPEMASK);  // turn off all the style (view mode) bits
	lStyleOld |= LVS_REPORT|LVS_SINGLESEL|LVS_ALIGNLEFT;// Set the new Style for the control
	SetWindowLong(m_templview.m_hWnd, GWL_STYLE, lStyleOld);  // done!
	
	lStyleOld = GetWindowLong(m_addlist.m_hWnd, GWL_STYLE);
	lStyleOld &= ~(LVS_TYPEMASK);  // turn off all the style (view mode) bits
	lStyleOld |= LVS_SHOWSELALWAYS|LVS_LIST|LVS_ALIGNLEFT;// Set the new Style for the control
	SetWindowLong(m_addlist.m_hWnd, GWL_STYLE, lStyleOld);  // done!
	

	pApp = (CMCPforNTApp *)AfxGetApp();		
	m_xpimagelistSmall = new CImageList();
	ASSERT(m_xpimagelistSmall != NULL);    // no serious allocation failure checking
	m_xpimagelistSmall->Create(16, 16, TRUE/*bMask*/, LISTICONCOUNT, 4);
	m_xpimagelistSmall->Add(pApp->LoadIcon(IDI_ICON1));
	m_xpimagelistSmall->Add(pApp->LoadIcon(IDI_ICON3));
	m_xpimagelistSmall->Add(pApp->LoadIcon(IDI_ICON5));

	m_templview.SetImageList(m_xpimagelistSmall,LVSIL_SMALL);


	m_xpimagelist2Small = new CImageList();
	ASSERT(m_xpimagelist2Small != NULL);    // no serious allocation failure checking
	m_xpimagelist2Small->Create(16, 16, TRUE/*bMask*/, LISTICONCOUNT, 4);
	m_xpimagelist2Small->Add(pApp->LoadIcon(IDI_ICON14));
	m_xpimagelist2Small->Add(pApp->LoadIcon(IDI_ICON15));
	m_xpimagelist2Small->Add(pApp->LoadIcon(IDI_ICON14));

	m_addlist.SetImageList(m_xpimagelist2Small,LVSIL_SMALL);

	column.mask			= LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;       // see below
	column.fmt			= LVCFMT_LEFT;         // see below
	column.cx			= 120;          // width of the column, in pixels
	column.pszText		= "Object";   // see below
	column.cchTextMax	= 20;  // see below
	column.iSubItem		= 0;    // index of subitem associated with column

	m_templview.InsertColumn(0, &column );

	column.mask			= LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;       // see below
	column.fmt			= LVCFMT_LEFT;         // see below
	column.cx			= 300;          // width of the column, in pixels
	column.pszText		= "Data";   // see below
	column.cchTextMax	= 20;  // see below
	column.iSubItem		= 1;    // index of subitem associated with column

	m_templview.InsertColumn(1, &column );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCalculator::OnAdd() 
{
	// TODO: Add your control notification handler code here
	int Selected[20];
	int i;
	CString FileName;
	LV_FINDINFO findinfo;
	

	CMCPforNTDoc* pNewDoc;

	m_listbox.GetSelItems(20,(int *)&Selected);
	for (i=0;i<m_listbox.GetSelCount();i++)
	{
		m_listbox.GetText(Selected[i],FileName);
		findinfo.flags	= LVFI_STRING; 
		findinfo.psz	= FileName;  
		findinfo.lParam	= 0;
		if(m_addlist.FindItem(&findinfo,-1)==-1)
		{
			pNewDoc = new CMCPforNTDoc();
			pNewDoc->SetPathName(FileName,FALSE);
			if(pNewDoc->OnOpenDocument(FileName))
			{
				
				if(pDoc.GetCount()==0)
				{
					SumDoc = new CMCPforNTDoc();
					m_addlist.InsertItem(LVIF_TEXT|LVIF_IMAGE,pDoc.GetCount(),FileName,0,0,0,(LPARAM)0);
					pDoc.AddHead(pNewDoc);
					SumDoc->OnOpenDocument(FileName);
					UpdateTree(SumDoc);
					pNewDoc->AddMe = 0;
				}
				else
				{
					if(CompareDocs(pNewDoc,(CMCPforNTDoc*)pDoc.GetHead()))
					{
						m_addlist.InsertItem(LVIF_TEXT|LVIF_IMAGE,pDoc.GetCount(),FileName,0,0,0,(LPARAM)0);
						pDoc.AddTail(pNewDoc);
						pNewDoc->AddMe = 2;
					}
					else
					{
						AfxMessageBox("Different MCP-File format !",MB_OK,0);
						delete pNewDoc;
					}
				}
			}
			else
			{
				delete pNewDoc;
			}
		}
		else AfxMessageBox(FileName+" already added !",MB_OK,0);
	}
	m_listbox.SetSel(-1,FALSE);
	ReCalcSum();
	UpdateTree(SumDoc);
}

void CCalculator::OnSub() 
{
	// TODO: Add your control notification handler code here
	POSITION		pos;
	BOOL			FOUND;
	CMCPforNTDoc*	pDocument;
	CString			DeleteThis;
	int				index;

	index = GetMarkedItem();
	if(index!=-1)
	{
		DeleteThis = m_addlist.GetItemText(index,0); 

		if(!pDoc.IsEmpty())
		{
			pos = pDoc.GetHeadPosition();
			FOUND = FALSE;
			do
			{
				pDocument = (CMCPforNTDoc*)pDoc.GetNext(pos);
				if(DeleteThis == pDocument->GetTitle())
				{
					FOUND = TRUE;
				}
			}
			while (!FOUND);
			if(FOUND==TRUE)
			{
				pos = pDoc.Find(pDocument,NULL);
				pDoc.RemoveAt(pos);
				delete pDocument;
				m_addlist.DeleteItem(index);
			}
		}
	}
	if(pDoc.GetCount()==0)
	{
		m_templview.DeleteAllItems();
		delete SumDoc;
		SumDoc = NULL;
	}
	else 
	{
		ReCalcSum();
		UpdateTree(SumDoc);
	}
}

BOOL CCalculator::CompareDocs(CMCPforNTDoc* pDoc1,CMCPforNTDoc* pDoc2)
{
	BOOL EQUAL = TRUE;
	CString TheObject1[100];
	CString TheObject2[100];
	USHORT	Channels1[100];
	USHORT	Channels2[100];
	USHORT i,j;

	POSITION pos;


	if(!pDoc1->ActionObjList.IsEmpty())
	{
		i=0;
		ActionObject* pActionObj = (ActionObject*)pDoc1->ActionObjList.GetHead();
		pos = pDoc1->ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)pDoc1->ActionObjList.GetNext(pos);
			if (pActionObj!=NULL)
			{
				TheObject1[i]	= pActionObj->GetName();
				Channels1[i]	= pActionObj->pTrack->Channels;
				i++;
			}
		}
		while (pos!=NULL);
	}


	if(!pDoc2->ActionObjList.IsEmpty())
	{
		i=0;
		ActionObject* pActionObj = (ActionObject*)pDoc2->ActionObjList.GetHead();
		pos = pDoc2->ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)pDoc2->ActionObjList.GetNext(pos);
			if (pActionObj!=NULL)
			{
				TheObject2[i]	= pActionObj->GetName();
				Channels2[i]	= pActionObj->pTrack->Channels;
				i++;
			}
		}
		while (pos!=NULL);
	}

	for(j=0;j<i;j++)
	{
		if((TheObject2[j]!=TheObject1[j])||
			(Channels2[j]!=Channels1[j]))
			EQUAL = FALSE;
	}
	return EQUAL;
}


void CCalculator::UpdateTree(CMCPforNTDoc* pDocument)
{
	int Image;
	int	Position = 0;
	CString TheObject;
	int Track=0;
	char TheString[50];
	POSITION pos;
	m_templview.DeleteAllItems();

	if(!pDocument->ActionObjList.IsEmpty())
	{
		ActionObject* pActionObj = (ActionObject*)pDocument->ActionObjList.GetHead();
		pos = pDocument->ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)pDocument->ActionObjList.GetNext(pos);
			if (pActionObj!=NULL)
			{
				if (pActionObj->GetName().Find(_TrackObj)!=-1)
				{	
					++Track;
					m_templview.InsertItem(LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM,Position,pActionObj->GetName(),0,0,2,(LPARAM)pActionObj);
					m_templview.SetItem(Position,1,LVIF_TEXT, pActionObj->GetInfo(),0,0,0,NULL);
				}
				else
				{
					if (pActionObj->TestHardware()) Image=0;
						else Image = 1;
	
					strcpy(TheString,(LPCSTR)TheObject);
					m_templview.InsertItem(LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM,Position,pActionObj->GetName(),0,0,Image,(LPARAM)pActionObj);
					m_templview.SetItem(Position,1,LVIF_TEXT, pActionObj->GetInfo(),0,0,0,NULL);
				}
				
			}
			Position++;
		}
		while (pos!=NULL);
	}
}

void CCalculator::PostNcDestroy() 
{
	if(SumDoc!=NULL)delete SumDoc;
	SumDoc = NULL;

	POSITION pos;
	if(!pDoc.IsEmpty())
	{
		CMCPforNTDoc* pDocument;
		pos = pDoc.GetHeadPosition();
		do
		{
			pDocument = (CMCPforNTDoc*)pDoc.GetNext(pos);
			if (pDocument!=NULL)
			{
				delete pDocument;
			}
		}
		while (pos!=NULL);
		pDoc.RemoveAll();
	}
	delete m_xpimagelistSmall;
	delete m_xpimagelist2Small;
	CDialog::PostNcDestroy();
	delete this;
}

void CCalculator::OnSaveadded() 
{
	// TODO: Add your command handler code here
	CString FileName;
	static char BASED_CODE szFilter[] = "MCP Files (*.mcp)|*.mcp|All Files (*.*)|*.*||";
	CFileDialog* pSaveFile = new CFileDialog(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,&szFilter[0],NULL);
	if(IDOK==pSaveFile->DoModal())
	{
		FileName = pSaveFile->GetPathName();
		delete pSaveFile;
		SumDoc->SetPathName(FileName,FALSE);
		SumDoc->OnSaveDocument(FileName);
	}
}


BOOL CCalculator::PreTranslateMessage(MSG* pMsg) 
{
	{
		if (pMsg->message == WM_LBUTTONDBLCLK)
		{
			if(pDoc.GetCount()!=0)
			{
				CMCPforNTDoc* pHeadDoc = (CMCPforNTDoc*)pDoc.GetHead();
				if (!pHeadDoc->ActionObjList.IsEmpty())
				{
					ActionObject* pActionObj = 
						(ActionObject*)m_templview.GetItemData(GetSelectedItem());
					if ((pActionObj!= NULL)&&(pActionObj->SCALER))
					{
						pActionObj->DoDoubleClickAction();
					}
				}
			}
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}



int CCalculator::GetSelectedItem() 
{
	// TODO: Add your command handler code here
	return m_templview.GetNextItem(-1,LVNI_FOCUSED);
}

int CCalculator::GetMarkedItem() 
{
	// TODO: Add your command handler code here
	return m_addlist.GetNextItem(-1,LVNI_FOCUSED);
}

void CCalculator::OnAddbutsubstract() 
{
	// TODO: Add your control notification handler code here
	int Selected[20];
	int i;
	CString FileName;
	LV_FINDINFO findinfo;
	

	CMCPforNTDoc* pNewDoc;

	m_listbox.GetSelItems(20,(int *)&Selected);
	for (i=0;i<m_listbox.GetSelCount();i++)
	{
		m_listbox.GetText(Selected[i],FileName);
		findinfo.flags	= LVFI_STRING; 
		findinfo.psz	= FileName;  
		findinfo.lParam	= 0;
		if(m_addlist.FindItem(&findinfo,-1)==-1)
		{
			pNewDoc = new CMCPforNTDoc();
			pNewDoc->SetPathName(FileName,FALSE);
			if(pNewDoc->OnOpenDocument(FileName))
			{
				
				if(pDoc.GetCount()==0)
				{
					SumDoc = new CMCPforNTDoc();
					m_addlist.InsertItem(LVIF_TEXT|LVIF_IMAGE,pDoc.GetCount(),FileName,0,0,1,(LPARAM)0);
					pDoc.AddHead(pNewDoc);
					SumDoc->OnOpenDocument(FileName);
					UpdateTree(SumDoc);
					pNewDoc->AddMe = 1;
				}
				else
				{
					if(CompareDocs(pNewDoc,(CMCPforNTDoc*)pDoc.GetHead()))
					{
						m_addlist.InsertItem(LVIF_TEXT|LVIF_IMAGE,pDoc.GetCount(),FileName,0,0,1,(LPARAM)0);
						pDoc.AddTail(pNewDoc);
						pNewDoc->AddMe = 3;
					}
					else
					{
						AfxMessageBox("Different MCP-File format !",MB_OK,0);
						delete pNewDoc;
					}
				}
			}
			else
			{
				delete pNewDoc;
			}
		}
		else AfxMessageBox(FileName+" already added !",MB_OK,0);
	}
	m_listbox.SetSel(-1,FALSE);
	ReCalcSum();
	UpdateTree(SumDoc);
}

void CCalculator::ReCalcSum()
{
	POSITION		pos1;
	POSITION		pos2;
	POSITION		pos3;
	CMCPforNTDoc*	pDocument;
	ActionObject* pActionObj;
	ActionObject* pSummingObj;

	if(!pDoc.IsEmpty())
	{
		pos1 = pDoc.GetHeadPosition();
		do
		{
			pDocument = (CMCPforNTDoc*)pDoc.GetNext(pos1);
			if(!pDocument->ActionObjList.IsEmpty())
			{
				pActionObj = (ActionObject*)pDocument->ActionObjList.GetHead();
				pos2 = pDocument->ActionObjList.GetHeadPosition();
				pos3 = SumDoc->ActionObjList.GetHeadPosition();
				do
				{
					pActionObj	= (ActionObject*)pDocument->ActionObjList.GetNext(pos2);
					pSummingObj = (ActionObject*)SumDoc->ActionObjList.GetNext(pos3);
					pActionObj->DoSummingAction(pSummingObj);	
				}
				while (pos2!=NULL&&pos3!=NULL);
			}
		}
		while (pos1!=NULL);
	}

	pos3 = SumDoc->ActionObjList.GetHeadPosition();
	do
	{
		pSummingObj = (ActionObject*)SumDoc->ActionObjList.GetNext(pos3);
		pSummingObj->UpdateDisplay();
	}
	while (pos3!=NULL);
}

void CCalculator::OnClose() 
{
	CDialog::OnClose();
	PostNcDestroy();
}

void CCalculator::OnOK() 
{
	CDialog::OnOK();
	PostNcDestroy();
}
