// MCP for NTView.cpp : implementation of the CMCPforNTView class
//

#include "stdafx.h"

#include "MCP for NT.h"
#include "MeasThread.h"
#include "MinuitChild.h"

#include "minuit.h"

#include "camac.h"
#include "hardware.h"
#include "MCP for NTDoc.h"
#include "ActionObjects.h"

#include "DataChild.h"
#include "DataView.h"
#include "MonitorChild.h"
#include "MonitorView.h"
#include "LSIChild.h"
#include "LSIView.h"

#include "PropShtRS.h"
#include "PropShtHV.h"
#include "PropShtHP.h"
#include "PropShtFS.h"
#include "PropShtAM.h"

#include "MCP for NTView.h"
#include "ChildFrm.h"

#include "resource.h"
#include "mmsystem.h"
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_LSIKILLED  WM_USER+300
#define WM_SCALERKILLED  WM_USER+301
#define WM_MEASUREMENTKILLED WM_USER + 304
#define WM_MEASUREMENTHALTED WM_USER + 404
#define WM_DATAARRIVED WM_USER + 305

#define TimerID 3333

#define WM_SAVEIT WM_USER +12
#define WM_SETPANE WM_USER +13

#define WM_DISABLECLOSE WM_USER +602
#define WM_ENABLECLOSE WM_USER +603

/////////////////////////////////////////////////////////////////////////////
// CMCPforNTView




IMPLEMENT_DYNCREATE(CMCPforNTView, CListView)

BEGIN_MESSAGE_MAP(CMCPforNTView, CListView)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopyCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCopyCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	//ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CMCPforNTView)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
	ON_WM_RBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_POPUP_NEW_ARITHSCALER, OnPopupNewArithscaler)
	ON_COMMAND(ID_POPUP_NEW_FDAC, OnPopupNewFdac)
	ON_COMMAND(ID_POPUP_NEW_FLUKESWITCH, OnPopupNewFlukeswitch)
	ON_COMMAND(ID_POPUP_NEW_HPDAC, OnPopupNewHpdac)
	ON_COMMAND(ID_POPUP_NEW_HPDEKADE, OnPopupNewHpdekade)
	ON_COMMAND(ID_POPUP_NEW_ISOLDEMASS, OnPopupNewIsoldemass)
	ON_COMMAND(ID_POPUP_NEW_RSDEKADE, OnPopupNewRsdekade)
	ON_COMMAND(ID_POPUP_NEW_SCALER, OnPopupNewScaler)
	ON_COMMAND(ID_POPUP_NEW_TRACK, OnPopupNewTrack)
	ON_COMMAND(ID_GO, OnGo)
	ON_COMMAND(ID_ERGO, OnErgo)
	ON_COMMAND(ID_KILL, OnKill)
	ON_COMMAND(ID_LSIBOX, OnLsibox)
	ON_COMMAND(ID_POPUP_NEW_TRIGGER, OnPopupNewTrigger)
	ON_COMMAND(ID_POPUP_NEW_ALEXMOTOR, OnPopupNewAlexmotor)
	ON_COMMAND(ID_POPUP_NEW_KEPKOVOLTAGE, OnPopupNewKepkovoltage)
	ON_COMMAND(ID_POPUP_NEW_ISOLDEVOLTAGE, OnPopupNewIsoldevoltage)
	ON_COMMAND(ID_POPUP_NEW_PREMAVOLTAGE, OnPopupNewPremavoltage)
	ON_COMMAND(ID_CONFIGURE, OnConfigure)
	ON_COMMAND(ID_GPIBREADER, OnGpibreader)
	ON_COMMAND(ID_OUTBIT, OnOutbit)
	ON_COMMAND(ID_FIELDREADER, OnFieldreader)
	ON_COMMAND(ID_TEMPREADER, OnTempreader)
	ON_COMMAND(ID_POPUP_FIELDWRITER, OnPopupFieldwriter)
	ON_COMMAND(ID_POPUP_GPIBWRITER, OnPopupGpibwriter)
	ON_COMMAND(ID_TEMPWRITER, OnTempwriter)
	ON_COMMAND(ID_POPUP_NEW_HVDAC, OnPopupNewHvdac)
	ON_COMMAND(ID_AUTOGO, OnAutogo)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_UPDATE_COMMAND_UI(ID_ERGO,IsRunningErgo)
	ON_UPDATE_COMMAND_UI(ID_GO,IsRunningGo)
	ON_UPDATE_COMMAND_UI(ID_AUTOGO,IsRunningAutoGo)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS,IsRunningSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE,IsRunningSave)
	ON_EN_SETFOCUS(IDC_SAVEEDIT, OnGetFocus)
	ON_EN_KILLFOCUS(IDC_SAVEEDIT, OnKillFocus)
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
	ON_MESSAGE(WM_SAVEIT,CMCPforNTView::OnSave)
	ON_MESSAGE(WM_TIMER,CMCPforNTView::OnTimer)
	ON_MESSAGE(WM_MEASUREMENTKILLED,CMCPforNTView::OnMeasurementKilled)
	ON_MESSAGE(WM_MEASUREMENTHALTED,CMCPforNTView::OnMeasurementHalted)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMCPforNTView construction/destruction



CMCPforNTView::CMCPforNTView()
{
	// TODO: add construction code here
	SaveWindow			= FALSE;
	pCLSIView			= NULL;
	pDView				= NULL;
	m_xpimagelistSmall	= NULL;
	MeasDlg				= NULL;
	m_bDragging			= FALSE;
	m_pimagelist		= NULL;
	HALTNOW				= FALSE;

	// CG: This line was added by the Clipboard Assistant component
	m_nClipboardFormat = ::RegisterClipboardFormat(_T("ActionObject"));

	NewFont1 = new CFont();
	
}


CMCPforNTView::~CMCPforNTView()
{
	if(pCLSIView!=NULL)pCLSIView->GetParent()->DestroyWindow();
	delete m_xpimagelistSmall;
	delete NewFont1;
}

/////////////////////////////////////////////////////////////////////////////
// CMCPforNTView drawing

void CMCPforNTView::OnDraw(CDC* pDC)
{
	CMCPforNTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

void CMCPforNTView::OnInitialUpdate()
{
	LOGFONT logFont1;
    logFont1.lfHeight = 14;
    logFont1.lfWidth = 0;
    logFont1.lfEscapement = 0;
    logFont1.lfOrientation = 0;
    logFont1.lfWeight = FW_NORMAL;
    logFont1.lfItalic = 0;
    logFont1.lfUnderline = 0;
    logFont1.lfStrikeOut = 0;
    logFont1.lfCharSet = ANSI_CHARSET;
    logFont1.lfOutPrecision = OUT_DEFAULT_PRECIS;
    logFont1.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    logFont1.lfQuality = PROOF_QUALITY;
    logFont1.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	strcpy(logFont1.lfFaceName, "Courier New");
	NewFont1->CreateFontIndirect(&logFont1);
	SetFont(NewFont1,FALSE);
	CListView::OnInitialUpdate();

	CMCPforNTApp		*pApp;
	const				LISTICONCOUNT = 3;
	long				lStyleOld;

	LV_COLUMN			column;


	lStyleOld = GetWindowLong(GetListCtrl().m_hWnd, GWL_STYLE);
	lStyleOld &= ~(LVS_TYPEMASK);  // turn off all the style (view mode) bits
	lStyleOld |= LVS_REPORT|LVS_SINGLESEL|LVS_ALIGNLEFT;// Set the new Style for the control
	SetWindowLong(GetListCtrl().m_hWnd, GWL_STYLE, lStyleOld);  // done!
	
	pApp = (CMCPforNTApp *)AfxGetApp();		
	m_xpimagelistSmall = new CImageList();
	ASSERT(m_xpimagelistSmall != NULL);    // no serious allocation failure checking
	m_xpimagelistSmall->Create(16, 16, TRUE/*bMask*/, LISTICONCOUNT, 4);
	m_xpimagelistSmall->Add(pApp->LoadIcon(IDI_ICON1));
	m_xpimagelistSmall->Add(pApp->LoadIcon(IDI_ICON3));
	m_xpimagelistSmall->Add(pApp->LoadIcon(IDI_ICON5));

	GetListCtrl().SetImageList(m_xpimagelistSmall,LVSIL_SMALL);


	column.mask			= LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;       // see below
	column.fmt			= LVCFMT_LEFT;         // see below
	column.cx			= 120;          // width of the column, in pixels
	column.pszText		= "Object";   // see below
	column.cchTextMax	= 20;  // see below
	column.iSubItem		= 0;    // index of subitem associated with column

	GetListCtrl().InsertColumn(0, &column );

	column.mask			= LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;       // see below
	column.fmt			= LVCFMT_LEFT;         // see below
	column.cx			= 300;          // width of the column, in pixels
	column.pszText		= "Data";   // see below
	column.cchTextMax	= 20;  // see below
	column.iSubItem		= 1;    // index of subitem associated with column

	GetListCtrl().InsertColumn(1, &column );
	GetDocument()->UpdateAllViews(NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CMCPforNTView printing

BOOL CMCPforNTView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}


/////////////////////////////////////////////////////////////////////////////
// CMCPforNTView diagnostics

#ifdef _DEBUG
void CMCPforNTView::AssertValid() const
{
	CListView::AssertValid();
}

void CMCPforNTView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CMCPforNTDoc* CMCPforNTView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMCPforNTDoc)));
	return (CMCPforNTDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMCPforNTView message handlers


void CMCPforNTView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int					indexitem;
	UINT				flags = LVHT_ONITEM;

	if (m_bDragging)
	{
		ASSERT(m_pimagelist != NULL);
		m_pimagelist->DragMove(point);
		if ((indexitem = GetListCtrl().HitTest(point, &flags)) != -1)
		{
			m_pimagelist->DragLeave(this);
			indexdrop = indexitem;
			m_pimagelist->DragEnter(this, point);
		}
	}
	CListView::OnMouseMove(nFlags, point);
}

BOOL CMCPforNTView::TransferItem(int indexDrag, int indexDrop,BOOL IsFirst)
{
	POSITION			posFrom,posTo;
	ActionObject* pActionObjFrom = (ActionObject*)GetListCtrl().GetItemData(indexDrag);
	if (pActionObjFrom!= NULL)
	{
		posFrom = GetDocument()->ActionObjList.Find(pActionObjFrom,NULL);
		if (posFrom != NULL) GetDocument()->ActionObjList.RemoveAt(posFrom);
	}
	ActionObject* pActionObjTo = (ActionObject*)GetListCtrl().GetItemData(indexDrop);
	if (pActionObjTo!= NULL)
	{
		posTo = GetDocument()->ActionObjList.Find(pActionObjTo,NULL);
		if (posTo != NULL) 
		{
			if (!IsFirst)
			{
				pActionObjFrom->pTrack = pActionObjTo->pTrack;
				GetDocument()->ActionObjList.InsertAfter(posTo,pActionObjFrom);
			}
			else 
			{
				GetDocument()->ActionObjList.InsertBefore(posTo,pActionObjFrom);
			}
		}
	}
	return TRUE;

}

void CMCPforNTView::OnButtonUp()
{
	if (m_bDragging)
	{
		ASSERT(m_pimagelist != NULL);
		m_pimagelist->DragLeave(this);
		m_pimagelist->EndDrag();
		delete m_pimagelist;
		m_pimagelist = NULL;
		int Target;
		BOOL IsFirst;
		if (indexdrop != indexdrag)
		{
			IsFirst=FALSE;
			Target=indexdrop;
			TransferItem(indexdrag, Target,IsFirst);
			GetDocument()->SetModifiedFlag(TRUE);
			GetDocument()->UpdateAllViews(NULL);
		}
		else MessageBeep(0);
		ReleaseCapture();
		m_bDragging = FALSE;
	}
}


void CMCPforNTView::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CPoint		ptAction;
	UINT		nFlags = LVHT_ONITEM;

	GetCursorPos(&ptAction);
	ScreenToClient(&ptAction);
	ASSERT(!m_bDragging);
	indexdrag = GetListCtrl().HitTest(ptAction, &nFlags);
	ActionObject* pActionObj = (ActionObject*)GetListCtrl().GetItemData(indexdrag);
	if(pActionObj->Name!=_TrackObj)
	{
		indexdrop = 0;
		m_bDragging = TRUE;

		ASSERT(m_pimagelist == NULL);
		m_pimagelist = GetListCtrl().CreateDragImage(indexdrag,&ptAction);  // get the image list for dragging
		m_pimagelist->DragShowNolock(TRUE);
		m_pimagelist->SetDragCursorImage(0, CPoint(0, 0));
		m_pimagelist->BeginDrag(0, CPoint(0,0));
		m_pimagelist->DragMove(ptAction);
		m_pimagelist->DragEnter(this, ptAction);
		SetCapture();
	
		*pResult = 0;
	}
}

void CMCPforNTView::OnContextMenu(CWnd*, CPoint point)
{
	// CG: This function was added by the Pop-up Menu component

	CMenu menu;
	VERIFY(menu.LoadMenu(CG_IDR_POPUP_MCPFOR_NTVIEW));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);
}

BOOL CMCPforNTView::PreTranslateMessage(MSG* pMsg)
{
	// CG: This block was added by the Pop-up Menu component
	{
		// Shift+F10: show pop-up menu.
		if ((((pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN) && // If we hit a key and
			(pMsg->wParam == VK_F10) && (GetKeyState(VK_SHIFT) & ~1)) != 0) ||	// it's Shift+F10 OR
			(pMsg->message == WM_CONTEXTMENU))									// Natural keyboard key
		{
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			CPoint point = rect.TopLeft();
			point.Offset(5, 5);
			OnContextMenu(NULL, point);

			return TRUE;
		}
	}

	return CListView::PreTranslateMessage(pMsg);
}

void CMCPforNTView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OnButtonUp();
	CListView::OnLButtonUp(nFlags, point);
}


void CMCPforNTView::OnUpdateEditCopyCut(CCmdUI* pCmdUI)
{
	// CG: This line was added by the Clipboard Assistant component
	pCmdUI->Enable(GetListCtrl().GetSelectedCount()!=0);
}

void CMCPforNTView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	// CG: This line was added by the Clipboard Assistant component
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	pCmdUI->Enable((!pApp->CopyActionList.IsEmpty())&&(!GetDocument()->MeasurementRunning));
}

void CMCPforNTView::OnEditCopy()
{
	// CG: This block was added by the Clipboard Assistant component
	POSITION pos;
	ActionObject* pActionObj;
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	pApp->EmptyActionList();
	if (!GetDocument()->ActionObjList.IsEmpty())
		{	
			pos = GetSelectedPosition();
			pActionObj = GetSelectedAction();

			if (pActionObj!= NULL)
			{
				if(pActionObj->GetName()!=_TrackObj)
				{	
					pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetNext(pos);
					pApp->CopyActionList.AddTail(CopyActionObject(pActionObj));	
				}
				else
				{
					do
					{
						pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetNext(pos);
						pApp->CopyActionList.AddTail(CopyActionObject(pActionObj));	
						if(pos!=NULL)pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetAt(pos);
					}while (pActionObj->GetName()!=_TrackObj&&pos!=NULL);
				}
			}
		}
}

POSITION CMCPforNTView::GetSelectedPosition(void)
{
	POSITION pos;
	int hItem;
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();

	if (!GetDocument()->ActionObjList.IsEmpty())
		{
			hItem = GetSelectedItem();
			if(hItem!=-1)
			{
				ActionObject* pActionObj = (ActionObject*)GetListCtrl().GetItemData(hItem);
				if (pActionObj!= NULL)
				{
					pos = GetDocument()->ActionObjList.Find(pActionObj,NULL);
					if (pos != NULL) 
					{
						return pos;
					}
				}
			}
		return (GetDocument()->ActionObjList.GetTailPosition());
		}
	return NULL;
}

ActionObject* CMCPforNTView::GetSelectedAction(void)
{
	POSITION pos;
	int hItem;
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();

	if (!GetDocument()->ActionObjList.IsEmpty())
		{
			hItem = GetSelectedItem();
			if(hItem!=-1)
			{
				ActionObject* pActionObj = (ActionObject*)GetListCtrl().GetItemData(hItem);
				if (pActionObj!= NULL)
				{
					pos = GetDocument()->ActionObjList.Find(pActionObj,NULL);
					if (pos != NULL) 
					{
						return (ActionObject*)GetDocument()->ActionObjList.GetAt(pos);
					}
				}
			}
		return (ActionObject*)GetDocument()->ActionObjList.GetAt(GetDocument()->ActionObjList.GetTailPosition());
		}
	return NULL;
}


TrackObject* CMCPforNTView::GetSelectedTrack(void)
{
	POSITION pos;
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();

	if (!GetDocument()->ActionObjList.IsEmpty())
		{
			int hItem = GetSelectedItem();
			if(hItem!=-1)
			{
				ActionObject* pActionObj = (ActionObject*)GetListCtrl().GetItemData(hItem);
				if (pActionObj!= NULL)
				{
					pos = GetDocument()->ActionObjList.Find(pActionObj,NULL);
					if (pos != NULL) 
					{
						while (pActionObj->GetName()!=_TrackObj&&pos!=NULL)
						{
							pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetPrev(pos);
						};
						return (TrackObject*)pActionObj;
					}
				}
			}
		}
	return NULL;
}
void CMCPforNTView::Remove(ActionObject* pActionObject)
{
	POSITION pos;
	pos = GetDocument()->ActionObjList.Find(pActionObject,NULL);
	if(pos!=NULL)
	{
		GetDocument()->ActionObjList.RemoveAt(pos);
	}
	delete pActionObject;
}

void CMCPforNTView::OnEditCut()
{
	// CG: This block was added by the Clipboard Assistant component
	POSITION pos;
	ActionObject* pActionObj;
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	if(!GetDocument()->MeasurementRunning)
	{
		pApp->EmptyActionList();
	
		if (!GetDocument()->ActionObjList.IsEmpty())
		{	
			pos = GetSelectedPosition();
			pActionObj = GetSelectedAction();

			if (pActionObj!= NULL)
			{
				if(pActionObj->GetName()!=_TrackObj)
				{	
					pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetNext(pos);
					pApp->CopyActionList.AddTail(CopyActionObject(pActionObj));
					Remove(pActionObj);
				}
				else
				{
					ActionPointerList.RemoveAll();
					do
					{
						
						pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetNext(pos);
						ActionPointerList.AddTail(pActionObj);
						pApp->CopyActionList.AddTail(CopyActionObject(pActionObj));
						if(pos!=NULL)pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetAt(pos);
					}
					while (pActionObj->GetName()!=_TrackObj&&pos!=NULL);
					pos = ActionPointerList.GetHeadPosition();
					for( pos = ActionPointerList.GetHeadPosition(); pos != NULL; )
					{
						Remove((ActionObject*)ActionPointerList.GetNext( pos ));
					}
					RearrangeTrackNum();
				}
			}
		}
		GetDocument()->SetModifiedFlag(TRUE);
		GetDocument()->UpdateAllViews(NULL);
	}

}

void CMCPforNTView::OnEditPaste()
{
	// CG: This block was added by the Clipboard Assistant component
	POSITION pos;
	POSITION Insertpos;
	BOOL CopyBeforeTrack = FALSE;
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	ActionObject* pActionObject;
	ActionObject* pInsertAction;
	TrackObject* pInsertTrack;
	if (!pApp->CopyActionList.IsEmpty())
	{	
		pos = pApp->CopyActionList.GetHeadPosition();
		if (!GetDocument()->ActionObjList.IsEmpty())
		{
			Insertpos = GetSelectedPosition();
			pActionObject = (ActionObject*)GetDocument()->ActionObjList.GetAt(Insertpos);
			if(pActionObject->Name==_TrackObj) CopyBeforeTrack = TRUE;
			pInsertTrack = GetSelectedTrack();
			do
			{
				pActionObject = (ActionObject*)pApp->CopyActionList.GetAt(pos);
					pInsertAction = (ActionObject*)CopyActionObject(pActionObject);
				
				if(pInsertAction->GetName()==_TrackObj)
				{
					TrackObject* pTrack = (TrackObject*)pInsertAction;
					pInsertTrack = pTrack;
				}
				else
				{
					if(GetSelectedAction()==NULL)
					{
						OnPopupNewTrack();
						pInsertTrack = GetSelectedTrack();
						break;
					}
				}
				pInsertAction->pDocument = GetDocument();
				pInsertAction->pTrack = pInsertTrack;
				if(CopyBeforeTrack)
				{
					Insertpos = GetDocument()->ActionObjList.InsertBefore(Insertpos,pInsertAction);
					CopyBeforeTrack = FALSE;
				}
				else Insertpos = GetDocument()->ActionObjList.InsertAfter(Insertpos,pInsertAction);
				
				ActionObject* pActionObj = (ActionObject*)pApp->CopyActionList.GetNext(pos);
			}while (pos!=NULL);
			RearrangeTrackNum();
			GetDocument()->SetModifiedFlag(TRUE);
			GetDocument()->UpdateAllViews(NULL);
		}
		else
		{
			pActionObject = (ActionObject*)pApp->CopyActionList.GetAt(pos);
			if(pActionObject->GetName()==_TrackObj)
			{
				do
				{
					pActionObject = (ActionObject*)pApp->CopyActionList.GetAt(pos);
					pInsertAction = (ActionObject*)CopyActionObject(pActionObject);
					
					if(pInsertAction->GetName()==_TrackObj)
					{
						TrackObject* pTrack = (TrackObject*)pInsertAction;
						pInsertTrack = pTrack;
					}
					pInsertAction->pDocument = GetDocument();
					pInsertAction->pTrack = pInsertTrack;
					Insertpos = GetDocument()->ActionObjList.AddTail(pInsertAction);		
					ActionObject* pActionObj = (ActionObject*)pApp->CopyActionList.GetNext(pos);
				}while (pos!=NULL);
				RearrangeTrackNum();
				GetDocument()->SetModifiedFlag(TRUE);
				GetDocument()->UpdateAllViews(NULL);
			}
		}	
	}
}


void CMCPforNTView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	int Track=0;
	int i=1;
	CString TheString;
	CString TheObject;
	POSITION pos;
	const Height=100;
	LOGFONT logFont1;
	logFont1.lfHeight = 50;
	logFont1.lfWidth = 0;
	logFont1.lfEscapement = 0;
	logFont1.lfOrientation = 0;
	logFont1.lfWeight = FW_NORMAL;
	logFont1.lfItalic = 0;
	logFont1.lfUnderline = 0;
	logFont1.lfStrikeOut = 0;
	logFont1.lfCharSet = ANSI_CHARSET;
	logFont1.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logFont1.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logFont1.lfQuality = PROOF_QUALITY;
	logFont1.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	strcpy(logFont1.lfFaceName, "Courier New");

	CFont* NewFont = new CFont();
	NewFont->CreateFontIndirect(&logFont1);
	CFont* OldFont = pDC->SelectObject(NewFont);
	pDC->SetTextAlign(TA_LEFT|TA_BASELINE); 
	pDC->SetBkMode(TRANSPARENT);
	
	if(!GetDocument()->ActionObjList.IsEmpty())
	{
		TheString=("MCP Template-File "+GetDocument()->GetPathName());
		pDC->TextOut(10,Height*i,TheString);
		++i;
		++i;

		ActionObject* pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetHead();
		pos = GetDocument()->ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetNext(pos);
			if (pActionObj!=NULL)
			{
				TheObject=pActionObj->GetName()+" ";
				while(TheObject.GetLength()<17)TheObject=TheObject+".";
				TheObject = TheObject+" "+pActionObj->GetInfo();

				if (pActionObj->GetName().Find(_TrackObj)!=-1)
				{					
					++i;
					++Track;
					TheObject =	pActionObj->GetName()+pActionObj->GetInfo();
					pDC->TextOut(10,Height*i,TheObject);
					++i;
				}
				else
				{
					pDC->TextOut(10,Height*i,TheObject);
					++i;
				}
			}
		}
		while (pos!=NULL);

	}
	pDC->SelectObject(OldFont);
	delete NewFont;
	CListView::OnPrint(pDC, pInfo);
}

void CMCPforNTView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//CListView::OnRButtonDown(nFlags, point);
	UINT flag = LVHT_ONITEM;
	int TheItem=GetListCtrl().HitTest(point,&flag);
	if (TheItem!=-1)GetListCtrl().SetItemState(TheItem,LVNI_FOCUSED|LVIS_SELECTED,LVNI_FOCUSED|LVIS_SELECTED);
	ClientToScreen(&point);
	OnContextMenu(this,point);
}

void CMCPforNTView::OnDestroy() 
{
	if(pCLSIView!=NULL)pCLSIView->GetParent()->DestroyWindow();
	
	CListView::OnDestroy();	
}

void CMCPforNTView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!GetDocument()->ActionObjList.IsEmpty())
	{
		int hItem = GetSelectedItem();
		if(hItem!=-1)
		{
			ActionObject* pActionObj = 
			(ActionObject*)GetListCtrl().GetItemData(hItem);
			if (pActionObj!= NULL)
			{
				pActionObj->DoDoubleClickAction();
			}
		}
	}
	//CListView::OnLButtonDblClk(nFlags, point);
}

void CMCPforNTView::OnPopupNewArithscaler() 
{
	// TODO: Add your command handler code here
	if(GetSelectedTrack()!=NULL)
	{

		PM_SpectrumObj* pAction = new PM_SpectrumObj;
		pAction->DoString		= "0";
		pAction->pTrack			= GetSelectedTrack();
		pAction->pDocument		= GetDocument();
		pAction->SubRegNo		= pAction->pTrack->ArithCounter++;
		pAction->ShortName		= __PM_ArithObj;
		pAction->SCALER			= TRUE;
		pAction->ARITHSCALER	= TRUE;

		GetDocument()->ActionObjList.InsertAfter(GetSelectedPosition(),pAction);
		GetDocument()->SetModifiedFlag(TRUE);
		GetDocument()->UpdateAllViews(NULL);
	}
}

void CMCPforNTView::OnPopupNewFdac() 
{
	// TODO: Add your command handler code here
	if(GetSelectedTrack()!=NULL)
	{

		FDAC_VoltageSweepObj* pAction = new FDAC_VoltageSweepObj;
		pAction->DoString		= "normal";
		pAction->pTrack			= GetSelectedTrack();
		pAction->pDocument		= GetDocument();

		GetDocument()->ActionObjList.InsertAfter(GetSelectedPosition(),pAction);
		GetDocument()->SetModifiedFlag(TRUE);
		GetDocument()->UpdateAllViews(NULL);
	}
}

void CMCPforNTView::OnPopupNewKepkovoltage() 
{
	// TODO: Add your command handler code here
	
	if(GetSelectedTrack()!=NULL)
	{
		KepcoEichungVoltageObj* pAction = new KepcoEichungVoltageObj;
		pAction->DoString		= "normal";
		pAction->pTrack			= GetSelectedTrack();

		pAction->pDocument		= GetDocument();

		GetDocument()->ActionObjList.InsertAfter(GetSelectedPosition(),pAction);
		GetDocument()->SetModifiedFlag(TRUE);
		GetDocument()->UpdateAllViews(NULL);	
	}
}


void CMCPforNTView::OnPopupNewIsoldevoltage() 
{
	// TODO: Add your command handler code here
	if(GetSelectedTrack()!=NULL)
	{

		IsoldeNetVoltageObj* pAction = new IsoldeNetVoltageObj;
		pAction->DoString		= "normal";
		pAction->pTrack			= GetSelectedTrack();
		pAction->pDocument		= GetDocument();

		GetDocument()->ActionObjList.InsertAfter(GetSelectedPosition(),pAction);
		GetDocument()->SetModifiedFlag(TRUE);
		GetDocument()->UpdateAllViews(NULL);
	}
}

void CMCPforNTView::OnPopupNewPremavoltage() 
{
	// TODO: Add your command handler code here
	if(GetSelectedTrack()!=NULL)
	{

		PremaVoltageObj* pAction = new PremaVoltageObj;
		pAction->DoString		= "normal";
		pAction->pTrack			= GetSelectedTrack();
		pAction->pDocument		= GetDocument();

		GetDocument()->ActionObjList.InsertAfter(GetSelectedPosition(),pAction);
		GetDocument()->SetModifiedFlag(TRUE);
		GetDocument()->UpdateAllViews(NULL);
	}
}

void CMCPforNTView::OnPopupNewFlukeswitch() 
{
	// TODO: Add your command handler code here
	if(GetSelectedTrack()!=NULL)
	{

		FlukeSwitchObj* pAction = new FlukeSwitchObj;
		pAction->DoString		= "normal";
		pAction->pTrack			= GetSelectedTrack();
		pAction->pDocument		= GetDocument();

		GetDocument()->ActionObjList.InsertAfter(GetSelectedPosition(),pAction);
		GetDocument()->SetModifiedFlag(TRUE);
		GetDocument()->UpdateAllViews(NULL);
	}
}

void CMCPforNTView::OnPopupNewHpdac() 
{
	// TODO: Add your command handler code here
	if(GetSelectedTrack()!=NULL)
	{

		HP_VoltageSweepObj* pAction = new HP_VoltageSweepObj;
		pAction->DoString		= "normal";
		pAction->pTrack			= GetSelectedTrack();
		pAction->pDocument		= GetDocument();

		GetDocument()->ActionObjList.InsertAfter(GetSelectedPosition(),pAction);
		GetDocument()->SetModifiedFlag(TRUE);
		GetDocument()->UpdateAllViews(NULL);
	}
}

void CMCPforNTView::OnPopupNewHvdac() 
{
	// TODO: Add your command handler code here
	if(GetSelectedTrack()!=NULL)
	{

		Line_VoltageSweepObj* pAction = new Line_VoltageSweepObj;
		pAction->DoString		= "normal";
		pAction->pTrack			= GetSelectedTrack();
		pAction->pDocument		= GetDocument();

		GetDocument()->ActionObjList.InsertAfter(GetSelectedPosition(),pAction);
		GetDocument()->SetModifiedFlag(TRUE);
		GetDocument()->UpdateAllViews(NULL);
	}
}

void CMCPforNTView::OnPopupNewHpdekade() 
{
	// TODO: Add your command handler code here
	if(GetSelectedTrack()!=NULL)
	{

		HP3325B_FrequencySweepObj* pAction = new HP3325B_FrequencySweepObj;
		pAction->DoString		= "normal";
		pAction->pTrack			= GetSelectedTrack();
		pAction->pDocument		= GetDocument();

		GetDocument()->ActionObjList.InsertAfter(GetSelectedPosition(),pAction);
		GetDocument()->SetModifiedFlag(TRUE);
		GetDocument()->UpdateAllViews(NULL);
	}
}

void CMCPforNTView::OnPopupNewIsoldemass() 
{
	// TODO: Add your command handler code here
	if(GetSelectedTrack()!=NULL)
	{

		MassSwitchObj* pAction = new MassSwitchObj;
		pAction->DoString		= "normal";
		pAction->pTrack			= GetSelectedTrack();
		pAction->pDocument		= GetDocument();

		GetDocument()->ActionObjList.InsertAfter(GetSelectedPosition(),pAction);
		GetDocument()->SetModifiedFlag(TRUE);
		GetDocument()->UpdateAllViews(NULL);
	}
}

void CMCPforNTView::OnPopupNewRsdekade() 
{
	// TODO: Add your command handler code here
	if(GetSelectedTrack()!=NULL)
	{

		CMCPforNTApp* pApp	= (CMCPforNTApp*)AfxGetApp();
		RS_FrequencySweepObj* pAction = new RS_FrequencySweepObj;
		
		pAction->DoString		= "normal";
		pAction->pTrack			= GetSelectedTrack();
		pAction->pDocument		= GetDocument();
		
		pAction->pTrack->RS_FrequencyNum++;
		switch(pAction->pTrack->RS_FrequencyNum)
		{
		case 1:
			pAction->Gpib				= pApp->Rs1Gpib;
			break;
		case 2:
			pAction->Gpib				= pApp->Rs2Gpib;
			break;
		default:
			pAction->Gpib				= pApp->Rs1Gpib;
			break;
		}
		

		GetDocument()->ActionObjList.InsertAfter(GetSelectedPosition(),pAction);
		GetDocument()->SetModifiedFlag(TRUE);
		GetDocument()->UpdateAllViews(NULL);
	}
}

void CMCPforNTView::OnPopupNewScaler() 
{
	// TODO: Add your command handler code here
	if(GetSelectedTrack()!=NULL)
	{

		PM_SpectrumObj* pAction = new PM_SpectrumObj;
		pAction->DoString		= "normal";
		pAction->pTrack			= GetSelectedTrack();
		pAction->SubRegNo		= pAction->pTrack->ScalerCounter++;
		pAction->pDocument		= GetDocument();
		pAction->SCALER			= TRUE;
		pAction->ARITHSCALER	= FALSE;

		GetDocument()->ActionObjList.InsertAfter(GetSelectedPosition(),pAction);
		GetDocument()->SetModifiedFlag(TRUE);
		GetDocument()->UpdateAllViews(NULL);
	}
}

void CMCPforNTView::OnPopupNewTrack() 
{
	// TODO: Add your command handler code here
	USHORT MyPos = 1;
	TrackObject* pAction		= new TrackObject;
	pAction->DoString			= "normal";
	pAction->pTrack				= pAction;
	pAction->pDocument			= GetDocument();
	
	
	GetDocument()->ActionObjList.AddTail(pAction);
	RearrangeTrackNum();

	GetDocument()->UpdateAllViews(NULL,0,NULL);
}

void CMCPforNTView::OnPopupNewTrigger() 
{
	// TODO: Add your command handler code here
	if(GetSelectedTrack()!=NULL)
	{

		TriggerObj* pAction = new TriggerObj;
		pAction->DoString		= "normal";
		pAction->pTrack			= GetSelectedTrack();
		pAction->pDocument		= GetDocument();

		GetDocument()->ActionObjList.InsertAfter(GetSelectedPosition(),pAction);
		GetDocument()->SetModifiedFlag(TRUE);
		GetDocument()->UpdateAllViews(NULL);
	}
}

void CMCPforNTView::OnPopupNewAlexmotor() 
{
	// TODO: Add your command handler code here
	if(GetSelectedTrack()!=NULL)
	{

		AlexMotorObj* pAction = new AlexMotorObj;
		pAction->DoString		= "normal";
		pAction->pTrack			= GetSelectedTrack();
		pAction->pDocument		= GetDocument();

		GetDocument()->ActionObjList.InsertAfter(GetSelectedPosition(),pAction);
		GetDocument()->SetModifiedFlag(TRUE);
		GetDocument()->UpdateAllViews(NULL);
	}
}
void CMCPforNTView::OnGpibreader() 
{
	// TODO: Add your command handler code here
	if(GetSelectedTrack()!=NULL)
	{

		GPIBReaderObj* pAction = new GPIBReaderObj;
		pAction->DoString		= "normal";
		pAction->pTrack			= GetSelectedTrack();
		pAction->pDocument		= GetDocument();

		GetDocument()->ActionObjList.InsertAfter(GetSelectedPosition(),pAction);
		GetDocument()->SetModifiedFlag(TRUE);
		GetDocument()->UpdateAllViews(NULL);
	}	
}

void CMCPforNTView::OnOutbit() 
{
	// TODO: Add your command handler code here
	if(GetSelectedTrack()!=NULL)
	{

		OutBitObj* pAction = new OutBitObj;
		pAction->DoString		= "normal";
		pAction->pTrack			= GetSelectedTrack();
		pAction->pDocument		= GetDocument();

		GetDocument()->ActionObjList.InsertAfter(GetSelectedPosition(),pAction);
		GetDocument()->SetModifiedFlag(TRUE);
		GetDocument()->UpdateAllViews(NULL);
	}	
}

void CMCPforNTView::OnFieldreader() 
{
	// TODO: Add your command handler code here
	if(GetSelectedTrack()!=NULL)
	{

		FieldReaderObj* pAction = new FieldReaderObj;
		pAction->DoString		= "normal";
		pAction->pTrack			= GetSelectedTrack();
		pAction->pDocument		= GetDocument();

		GetDocument()->ActionObjList.InsertAfter(GetSelectedPosition(),pAction);
		GetDocument()->SetModifiedFlag(TRUE);
		GetDocument()->UpdateAllViews(NULL);
	}	
}

void CMCPforNTView::OnTempreader() 
{
	// TODO: Add your command handler code here
	if(GetSelectedTrack()!=NULL)
	{

		TempReaderObj* pAction = new TempReaderObj;
		pAction->DoString		= "normal";
		pAction->pTrack			= GetSelectedTrack();
		pAction->pDocument		= GetDocument();

		GetDocument()->ActionObjList.InsertAfter(GetSelectedPosition(),pAction);
		GetDocument()->SetModifiedFlag(TRUE);
		GetDocument()->UpdateAllViews(NULL);
	}	
}

void CMCPforNTView::OnTempwriter() 
{
	// TODO: Add your command handler code here
	if(GetSelectedTrack()!=NULL)
	{

		TempWriterObj* pAction = new TempWriterObj;
		pAction->DoString		= "normal";
		pAction->pTrack			= GetSelectedTrack();
		pAction->pDocument		= GetDocument();

		GetDocument()->ActionObjList.InsertAfter(GetSelectedPosition(),pAction);
		GetDocument()->SetModifiedFlag(TRUE);
		GetDocument()->UpdateAllViews(NULL);
	}	
}

void CMCPforNTView::OnPopupFieldwriter() 
{
	// TODO: Add your command handler code here
	if(GetSelectedTrack()!=NULL)
	{

		FieldWriterObj* pAction = new FieldWriterObj;
		pAction->DoString		= "normal";
		pAction->pTrack			= GetSelectedTrack();
		pAction->pDocument		= GetDocument();

		GetDocument()->ActionObjList.InsertAfter(GetSelectedPosition(),pAction);
		GetDocument()->SetModifiedFlag(TRUE);
		GetDocument()->UpdateAllViews(NULL);
	}
	
}

void CMCPforNTView::OnPopupGpibwriter() 
{
	// TODO: Add your command handler code here
	if(GetSelectedTrack()!=NULL)
	{

		GPIBWriterObj* pAction = new GPIBWriterObj;
		pAction->DoString		= "normal";
		pAction->pTrack			= GetSelectedTrack();
		pAction->pDocument		= GetDocument();

		GetDocument()->ActionObjList.InsertAfter(GetSelectedPosition(),pAction);
		GetDocument()->SetModifiedFlag(TRUE);
		GetDocument()->UpdateAllViews(NULL);
	}
	
}

void CMCPforNTView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class

	int	Image;

	char PaneText[255];

	CString TheObject;
	int Track		= 0;
	int Position;
	char TheString[80];
	POSITION pos;
	BOOL	NoChanges = FALSE;

		
	if(GetListCtrl().GetItemCount()==GetDocument()->ActionObjList.GetCount())
	{
		Position = 0;
		NoChanges = TRUE;
		if(!GetDocument()->ActionObjList.IsEmpty())
		{
			ActionObject* pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetHead();
			pos = GetDocument()->ActionObjList.GetHeadPosition();
			do
			{
				pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetNext(pos);
				if (pActionObj!=NULL)
				{
					if(pActionObj!=(ActionObject*)GetListCtrl().GetItemData(Position)) NoChanges = FALSE;	
				}
				Position++;
			}
			while (pos!=NULL);
		}
	}

	Position = 0;
	if(!NoChanges)GetListCtrl().DeleteAllItems();

	if(!GetDocument()->ActionObjList.IsEmpty())
	{
		ActionObject* pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetHead();
		pos = GetDocument()->ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetNext(pos);
			if (pActionObj!=NULL)
			{
				if (pActionObj->GetName().Find(_TrackObj)!=-1)
				{	
					++Track;
					if(!NoChanges)GetListCtrl().InsertItem(LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM,Position,pActionObj->GetName(),0,0,2,(LPARAM)pActionObj);
					GetListCtrl().SetItem(Position,1,LVIF_TEXT, pActionObj->GetInfo(),0,0,0,NULL);
				}
				else
				{
					if (pActionObj->TestHardware()) Image=0;
						else Image = 1;
	
					strcpy(TheString,(LPCSTR)TheObject);
					if(!NoChanges)GetListCtrl().InsertItem(LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM,Position,pActionObj->GetName(),0,0,Image,(LPARAM)pActionObj);
					GetListCtrl().SetItem(Position,1,LVIF_TEXT, pActionObj->GetInfo(),0,0,0,NULL);
				}
				
			}
			Position++;
		}
		while (pos!=NULL);
	}
	CString Info = GetDocument()->GetInfo();
	strcpy(PaneText,Info);
	GetParent()->SendMessage(WM_SETPANE,0,(long) &PaneText); 
}

void CMCPforNTView::OnGo() 
{
	CString		Data;
	CTimeSpan	Step;
	CTimeSpan	Start;
	RUNMODE		= GO;
	DataCount	= 0;
	DataSum		= 0;
	ChannelSum	= 0;
	maxtrack	= 0;
	NumScaler	= 0;
	track		= 0;
	Time		= 0; 
	scaler		= 0;
	channel		= 0;
	cycle		= 1;
	maxcycle	= GetDocument()->NoOfCycles;
	CString		stepStr;
	CString		scanStr;
	CString		trackStr;
	CString		timeStr;

	POSITION pos;
	CString TheObject;
	TrackObject* pTrackObj;

	
	/**  Get DataCount ***/

	if(!GetDocument()->ActionObjList.IsEmpty())
	{
		ActionObject* pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetHead();
		pos = GetDocument()->ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetNext(pos);
			if (pActionObj!=NULL)
			{
				TheObject=pActionObj->GetName();
				if (TheObject.Find(_TrackObj)!=-1)
				{	
					maxtrack++;
					pTrackObj = (TrackObject*)pActionObj;
					if(pTrackObj->MyPosition == 1)
					{
						maxstep = pTrackObj->Channels;
						maxscan = pTrackObj->NoOfScans;
					}
					ChannelSum = ChannelSum + pTrackObj->Channels*pTrackObj->NoOfScans;
				}
				if(pActionObj->TakesData)
				{
					DataCount = DataCount + (pActionObj->pTrack->Channels*pActionObj->pTrack->NoOfScans)*pActionObj->DataCount;
				}
				if (TheObject.Find(__TriggerObj)!=-1)
				{
					TriggerObj* pTriggerObj = (TriggerObj*)pActionObj; 
					Time = Time + pTriggerObj->CloseScaler*pActionObj->pTrack->Channels*pActionObj->pTrack->NoOfScans/1000;
				}
			}
		}
		while (pos!=NULL);
	}
	
	
	DataArrived = 0;
	scan		= 0;
	track		= 0;
	DataCount	= DataCount*maxcycle;

	mytimer = SetTimer(TimerID,50,NULL);
	
	GetDocument()->MeasurementRunning = TRUE;
	CMCPforNTApp* pMyApp = (CMCPforNTApp*)AfxGetApp();
	pMyApp->GlobalMeasurementRunning = TRUE;
}

void CMCPforNTView::OnAutogo() 
{
	if(GetDocument()->MeasurementRunning == FALSE)
	{
		CString		Data;
		CTimeSpan	Step;
		CTimeSpan	Start;
		RUNMODE		= AUTOGO;
		DataCount	= 0;
		DataSum		= 0;
		ChannelSum	= 0;
		maxtrack	= 0;
		NumScaler	= 0;
		track		= 0;
		Time		= 0; 
		scaler		= 0;
		channel		= 0;
		cycle		= 1;
		maxcycle	= GetDocument()->NoOfMaximumCycles;
		CString		stepStr;
		CString		scanStr;
		CString		trackStr;
		CString		timeStr;

		POSITION pos;
		CString TheObject;
		TrackObject* pTrackObj;

		
		/**  Get DataCount ***/

		if(!GetDocument()->ActionObjList.IsEmpty())
		{
			ActionObject* pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetHead();
			pos = GetDocument()->ActionObjList.GetHeadPosition();
			do
			{
				pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetNext(pos);
				if (pActionObj!=NULL)
				{
					TheObject=pActionObj->GetName();
					if (TheObject.Find(_TrackObj)!=-1)
					{	
						maxtrack++;
						pTrackObj = (TrackObject*)pActionObj;
						if(pTrackObj->MyPosition == 1)
						{
							maxstep = pTrackObj->Channels;
							maxscan = pTrackObj->NoOfScans;
						}
						ChannelSum = ChannelSum + pTrackObj->Channels*pTrackObj->NoOfScans;
					}
					if(pActionObj->TakesData)
					{
						DataCount = DataCount + (pActionObj->pTrack->Channels*pActionObj->pTrack->NoOfScans)*pActionObj->DataCount;
					}
					if (TheObject.Find(__TriggerObj)!=-1)
					{
						TriggerObj* pTriggerObj = (TriggerObj*)pActionObj; 
						Time = Time + pTriggerObj->CloseScaler*pActionObj->pTrack->Channels*pActionObj->pTrack->NoOfScans/1000;
					}
				}
			}
			while (pos!=NULL);
		}
		
		
		DataArrived = 0;
		scan		= 0;
		track		= 0;
		DataCount	= DataCount*maxcycle;

		mytimer = SetTimer(TimerID,50,NULL);
		
		GetDocument()->MeasurementRunning = TRUE;
		CMCPforNTApp* pMyApp = (CMCPforNTApp*)AfxGetApp();
		pMyApp->GlobalMeasurementRunning = TRUE;
	}
	else maxcycle = cycle;
}


void CMCPforNTView::OnErgo() 
{
	CString		Data;
	CTimeSpan	Step;
	CTimeSpan	Start;
	RUNMODE		= ERGO;
	DataCount	= 0;
	DataSum		= 0;
	ChannelSum	= 0;
	maxtrack	= 0;
	NumScaler	= 0;
	track		= 0;
	Time		= 0; 
	scaler		= 0;
	channel		= 0;
    cycle		= 1;
	maxcycle	= GetDocument()->NoOfCycles;
	CString		stepStr;
	CString		scanStr;
	CString		trackStr;
	CString		timeStr;

	POSITION pos;
	CString TheObject;
	TrackObject* pTrackObj;

	
	/**  Get DataCount ***/

	if(!GetDocument()->ActionObjList.IsEmpty())
	{
		ActionObject* pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetHead();
		pos = GetDocument()->ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetNext(pos);
			if (pActionObj!=NULL)
			{
				TheObject=pActionObj->GetName();
				if (TheObject.Find(_TrackObj)!=-1)
				{	
					maxtrack++;
					pTrackObj = (TrackObject*)pActionObj;
					if(pTrackObj->MyPosition == 1)
					{
						maxstep = pTrackObj->Channels;
						maxscan = pTrackObj->NoOfScans;
					}
					ChannelSum = ChannelSum + pTrackObj->Channels*pTrackObj->NoOfScans;
				}
				if(pActionObj->TakesData)
				{
					DataCount = DataCount + pActionObj->pTrack->Channels*pActionObj->pTrack->NoOfScans*pActionObj->DataCount;
				}
				if (TheObject.Find(__TriggerObj)!=-1)
				{
					TriggerObj* pTriggerObj = (TriggerObj*)pActionObj; 
					Time = Time + pTriggerObj->CloseScaler*pActionObj->pTrack->Channels*pActionObj->pTrack->NoOfScans/1000;
				}
			}
		}
		while (pos!=NULL);
	}
	
	DataArrived = 0;
	scan		= 0;
	track		= 0;
	DataCount	= DataCount*maxcycle;

	mytimer = SetTimer(TimerID,50,NULL);
	
	GetDocument()->MeasurementRunning = TRUE;
	CMCPforNTApp* pMyApp = (CMCPforNTApp*)AfxGetApp();
	pMyApp->GlobalMeasurementRunning = TRUE;
}

void CMCPforNTView::OnKill() 
{	
	POSITION pos;
	KillTimer(mytimer);
	
	if(MeasDlg!=NULL)delete MeasDlg;
	MeasDlg	= NULL;

	GetDocument()->TrackRunning			= FALSE;
	GetDocument()->MeasurementRunning	= FALSE;
	GetDocument()->DataValid			= TRUE; //Nur ausnahmsweise
	GetDocument()->SetModifiedFlag(TRUE);
	CMCPforNTApp* pMyApp = (CMCPforNTApp*)AfxGetApp();
	pMyApp->GlobalMeasurementRunning	= FALSE;
	GetDocument()->DataQueueIndex = 0;
	DataQueuePos = 0;
	DataReset();
	ListReset();
	CamacReset();
	/********** Initialisierung:  MeasurementEndAction  ***************/
		if(!GetDocument()->ActionObjList.IsEmpty())
		{
			ActionObject* pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetHead();
			pos = GetDocument()->ActionObjList.GetHeadPosition();
			do
			{
				pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetNext(pos);
				if (pActionObj!=NULL)
				{
					pActionObj->MeasurementEndAction();
				}
			}
			while (pos!=NULL);
		}
							
	GetParent()->SendMessage(WM_ENABLECLOSE,0,0);
}

void CMCPforNTView::OnLsibox() 
{
	// TODO: Add your command handler code here
	if(pCLSIView==NULL) 
	{
		pCLSIView = new CLSIView(this);
		pCLSIView->SetFocus();
	}
	else 
	{
		pCLSIView->GetParent()->BringWindowToTop();
		pCLSIView->SetFocus();
	}
}

LRESULT CMCPforNTView::OnMeasurementKilled(WPARAM wparam,LPARAM lparam)
{
	CMCPforNTView::OnKill();
	return 0;
};

LRESULT CMCPforNTView::OnMeasurementHalted(WPARAM wparam,LPARAM lparam)
{
	HALTNOW = TRUE;
	return 0;
};

LRESULT CMCPforNTView::OnTimer(WPARAM wparam,LPARAM lparam)
{
	CString		AnswerString;
	BOOL		GotData = FALSE;
	ULONG		ScalerData;
	POSITION	pos; 
	CString		stepStr;
	CString		scanStr;
	CString		trackStr;
	CString		cycleStr;
	CString		timeStr;
	BOOL		RUNMODE2;

	ActionObject* pTakeData;
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();

	if (GetDocument()->MeasurementRunning)
	{
		if (track == 0)
		{
			/********** Initialisierung:  MeasurementBeginAction  ***************/ 
			GetParent()->SendMessage(WM_DISABLECLOSE,0,0);
			DataReset();
			ListReset();

			MeasDlg = new CMeasThread(this);
	
			MeasDlg->Create(IDD_MEASTHREAD,NULL);
			MeasDlg->m_progstep.SetRange(0,100);
	
			stepStr.Format("%i",maxstep);
			MeasDlg->SetDlgItemText(IDC_STEP,stepStr);

			scanStr.Format("%i",maxscan);
			MeasDlg->SetDlgItemText(IDC_SCAN,scanStr);

			trackStr.Format("%i",maxtrack);
			MeasDlg->SetDlgItemText(IDC_TRACK,trackStr);

			cycleStr.Format("%i",maxcycle);
			MeasDlg->SetDlgItemText(IDC_CYCLE,cycleStr);

			if(!GetDocument()->ActionObjList.IsEmpty())
			{
				ActionObject* pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetHead();
				pos = GetDocument()->ActionObjList.GetHeadPosition();
				do
				{
					pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetNext(pos);
					if (pActionObj!=NULL)
					{
						pActionObj->MeasurementBeginAction(RUNMODE);
					}
				}
				while (pos!=NULL);
			}
			scan	= 0;
			track	= 1;
			maxstep		= GetSteps(track);
			maxscan		= GetScans(track);
			NumScaler	= GetNumScaler(track);
		}

		
		if (GetDocument()->TrackRunning)
		{
			RUNMODE2 = RUNMODE;
			if(cycle!=1)RUNMODE2 = GO;
			{
				while (ListReadd(&ScalerData))
				{
					pTakeData = (ActionObject*)GetDocument()->DataQueue[DataQueuePos].pAction;
					if(pTakeData == NULL)
					{
						GetDocument()->TrackRunning = FALSE;
						KillTimer(mytimer);
					}
					else
					{
						channel = GetDocument()->DataQueue[DataQueuePos].step-1;
						pTakeData->TakeData(RUNMODE2,
									GetDocument()->DataQueue[DataQueuePos].scan,
									channel,
									ScalerData);

						
						if(pApp->pMonitorView!=NULL)
						{
							if(pTakeData->DispMonitorMode == 1)
							{
								pApp->pMonitorView->AddDataPoint(channel,(float)pTakeData->GetY(channel),(float)pTakeData->GetYErr(channel));
							}
						}

						DataQueuePos++;

						GotData = TRUE;
						DataArrived++;
						DataSum++;
					}
				}
				if(GotData)
				{
					if(pApp->pMonitorView!=NULL)pApp->pMonitorView->RedrawNow();
					CTimeSpan lefttime((long)(Time-((double)DataSum/(double)DataCount)*Time));

					MeasDlg->m_progstep.SetPos((ULONG)100*DataSum/DataCount);

					stepStr.Format("%i",channel+1);
					MeasDlg->SetDlgItemText(IDC_ACTSTEP,stepStr);
						
					scanStr.Format("%i",scan);
					MeasDlg->SetDlgItemText(IDC_ACTSCAN,scanStr);

					trackStr.Format("%i",track);
					MeasDlg->SetDlgItemText(IDC_ACTTRACK,trackStr);
					
					cycleStr.Format("%i",cycle);
					MeasDlg->SetDlgItemText(IDC_ACTCYCLE,cycleStr);

					timeStr=lefttime.Format( "%H:%M:%S" );		
					MeasDlg->SetDlgItemText(IDC_TIMELEFT,timeStr);
				}
			}
		}



		if (!GetDocument()->TrackRunning)
		{
			if (HALTNOW)
			{
				HALTNOW = FALSE;
				track_end(track);
				/********** Deinitialisierung:  MeasurementEndAction  ***************/ 
				if(!GetDocument()->ActionObjList.IsEmpty())
				{
					ActionObject* pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetHead();
					pos = GetDocument()->ActionObjList.GetHeadPosition();
					do
					{
						pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetNext(pos);
						if (pActionObj!=NULL)
						{
							pActionObj->MeasurementEndAction();
						}
					}
					while (pos!=NULL);
				}
				CMCPforNTApp* pMyApp = (CMCPforNTApp*)AfxGetApp();
				pMyApp->GlobalMeasurementRunning = FALSE;
				KillTimer(mytimer);
				GetDocument()->MeasurementRunning = FALSE;
				GetDocument()->DataValid = TRUE;
				delete MeasDlg;
				MeasDlg = NULL;
				GetDocument()->SetModifiedFlag(TRUE);
				GetDocument()->UpdateAllViews(NULL);
				GetParent()->SendMessage(WM_ENABLECLOSE,0,0);
				PlaySound("Done.wav",NULL,SND_ASYNC|SND_FILENAME);
			}
			else
			{
				scan++;
				if(scan>maxscan)
				{
					track_end(track);
					track++;
					maxstep		= GetSteps(track);
					maxscan		= GetScans(track);
					NumScaler	= GetNumScaler(track);
					scan = 1;
					if(track>maxtrack)
					{
						if(RUNMODE==AUTOGO)SaveAuto();
						cycle++;
						scan = 1;
						track = 1;
						maxstep		= GetSteps(track);
						maxscan		= GetScans(track);
						NumScaler	= GetNumScaler(track);
						if(cycle>maxcycle)
						{
							CMCPforNTApp* pMyApp = (CMCPforNTApp*)AfxGetApp();
							pMyApp->GlobalMeasurementRunning = FALSE;
							GetDocument()->TrackRunning = FALSE;
							GetDocument()->MeasurementRunning = FALSE;
							/********** Initialisierung:  MeasurementEndAction  ***************/
							if(!GetDocument()->ActionObjList.IsEmpty())
							{
								ActionObject* pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetHead();
								pos = GetDocument()->ActionObjList.GetHeadPosition();
								do
								{
									pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetNext(pos);
									if (pActionObj!=NULL)
									{
										pActionObj->MeasurementEndAction();
									}
								}
								while (pos!=NULL);
							}
							
							GetDocument()->DataValid = TRUE;
							if(MeasDlg!=NULL)delete MeasDlg;
							MeasDlg	= NULL;
							SaveBackup();
							GetDocument()->SetModifiedFlag(TRUE);
							GetDocument()->UpdateAllViews(NULL);
							GetParent()->SendMessage(WM_ENABLECLOSE,0,0);
							PlaySound("Done.wav",NULL,SND_ASYNC|SND_FILENAME);
						}
					}
				}

				if (track <= maxtrack&&cycle<=maxcycle)
				{
					GetDocument()->DataQueueIndex = 0;
					ListReset();
					DataReset();
					if(scan==1)track_start(track);
					program_scan(track,maxstep);
					ListDNAFCamac(READD,0,0,0,0);
					GetDocument()->DataQueue[GetDocument()->DataQueueIndex].scan = 0;
					GetDocument()->DataQueue[GetDocument()->DataQueueIndex].step = 0;
					GetDocument()->DataQueue[GetDocument()->DataQueueIndex].pAction = NULL;
					GetDocument()->DataQueueIndex++;
				
					DataQueuePos = 0;
					DataReset();
					
					stepStr.Format("%i",maxstep);
					MeasDlg->SetDlgItemText(IDC_STEP,stepStr);
					scanStr.Format("%i",maxscan);
					MeasDlg->SetDlgItemText(IDC_SCAN,scanStr);
					trackStr.Format("%i",maxtrack);
					MeasDlg->SetDlgItemText(IDC_TRACK,trackStr);

					if(pApp->pMonitorView!=NULL)
					{
						pApp->pMonitorView->CalcNextScan(GetRealScans(track)+scan);
						pApp->pMonitorView->ForceRedraw();
					}
					start_scan();
					lastscan = scan;
				}
			}
		}
	}
	return 0;
}

USHORT CMCPforNTView::GetNumScaler(USHORT track)
{
	POSITION	pos;
	CString		TheObject;
	USHORT		Scaler = 0;
	
	if(!GetDocument()->ActionObjList.IsEmpty())
	{
		ActionObject* pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetHead();
		pos = GetDocument()->ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetNext(pos);
			if ((pActionObj!=NULL)&&(pActionObj->pTrack->MyPosition == track))
			{
				if(pActionObj->TakesData)
				{
					Scaler = Scaler + 1; 
				}
			}
		}
		while (pos!=NULL);
	}	
	return Scaler;
}

USHORT CMCPforNTView::GetSteps(USHORT track)
{
	POSITION pos;
	CString TheObject;
	TrackObject* pTrackObj;

	if(!GetDocument()->ActionObjList.IsEmpty())
	{
		ActionObject* pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetHead();
		pos = GetDocument()->ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetNext(pos);
			if (pActionObj!=NULL)
			{
				TheObject=pActionObj->GetName();
				if (TheObject.Find(_TrackObj)!=-1)
				{	
					pTrackObj = (TrackObject*)pActionObj;
					if(pTrackObj->MyPosition == track)
					{
						return pTrackObj->Channels;
					}
				}
			}
		}
		while (pos!=NULL);
	}
	return 0;
}

USHORT CMCPforNTView::GetScans(USHORT track)
{
	POSITION pos;
	CString TheObject;
	TrackObject* pTrackObj;

	if(track==0) return 0; 
	if(!GetDocument()->ActionObjList.IsEmpty())
	{
		ActionObject* pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetHead();
		pos = GetDocument()->ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetNext(pos);
			if (pActionObj!=NULL)
			{
				TheObject=pActionObj->GetName();
				if (TheObject.Find(_TrackObj)!=-1)
				{	
					pTrackObj = (TrackObject*)pActionObj;
					if(pTrackObj->MyPosition == track)
					{
						return pTrackObj->NoOfScans;
					}
				}
			}
		}
		while (pos!=NULL);
	}
	return 0;
}

USHORT CMCPforNTView::GetRealScans(USHORT track)
{
	POSITION pos;
	CString TheObject;
	TrackObject* pTrackObj;

	if(!GetDocument()->ActionObjList.IsEmpty())
	{
		ActionObject* pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetHead();
		pos = GetDocument()->ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetNext(pos);
			if (pActionObj!=NULL)
			{
				TheObject=pActionObj->GetName();
				if (TheObject.Find(_TrackObj)!=-1)
				{	
					pTrackObj = (TrackObject*)pActionObj;
					if(pTrackObj->MyPosition == track)
					{
						return pTrackObj->RealScans;
					}
				}
			}
		}
		while (pos!=NULL);
	}
	return 0;
}


void CMCPforNTView::track_start(USHORT track)
{
	POSITION	pos;
	CString		TheObject;
	if(!GetDocument()->ActionObjList.IsEmpty())
	{
		ActionObject* pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetHead();
		pos = GetDocument()->ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetNext(pos);
			if (pActionObj!=NULL&&pActionObj->pTrack->MyPosition == track)
			{
				pActionObj->TrackBeginAction(track);
			}
		}
		while (pos!=NULL);
	}
}


void CMCPforNTView::track_end(USHORT track)
{
	POSITION	pos;
	CString		TheObject;

	if(!GetDocument()->ActionObjList.IsEmpty())
	{
		ActionObject* pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetHead();
		pos = GetDocument()->ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetNext(pos);
			if (pActionObj!=NULL&&pActionObj->pTrack->MyPosition == track)
			{
				pActionObj->TrackEndAction(track,lastscan);
			}
		}
		while (pos!=NULL);
	}
}


void CMCPforNTView::program_scan(USHORT track,USHORT steps)
{
	POSITION	pos;
	CString		TheObject;
	USHORT		step;
	
	if(!GetDocument()->ActionObjList.IsEmpty())
	{
		for (step = 1; step <= steps; step++)
		{
			ActionObject* pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetHead();
			pos = GetDocument()->ActionObjList.GetHeadPosition();
			do
			{
				pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetNext(pos);
				if (pActionObj!=NULL&&pActionObj->pTrack->MyPosition == track)
				{
					pActionObj->TrackStepAction(step,track,scan);
				}
			}
			while (pos!=NULL);
		}
	}	
}

void CMCPforNTView::start_scan(void)
{
	DataArrived = 0;
	ListStart();
	GetDocument()->TrackRunning = TRUE;
	mytimer = SetTimer(TimerID,50,NULL);
}

void CMCPforNTView::IsRunningErgo(CCmdUI* pCmdUI)
{
	CMCPforNTApp* pMyApp = (CMCPforNTApp*)AfxGetApp();
	pCmdUI->Enable(!GetDocument()->MeasurementRunning&&!pMyApp->GlobalMeasurementRunning&&pMyApp->CamacOn);
}

void CMCPforNTView::IsRunningGo(CCmdUI* pCmdUI)
{
	CMCPforNTApp* pMyApp = (CMCPforNTApp*)AfxGetApp();
	pCmdUI->Enable(!GetDocument()->MeasurementRunning&&!pMyApp->GlobalMeasurementRunning&&pMyApp->CamacOn);
}

void CMCPforNTView::IsRunningAutoGo(CCmdUI* pCmdUI)
{
	CString AutoGoText;
	AutoGoText = "StopMe";
	BOOL SETAUTOGO = FALSE;
	CMCPforNTApp* pMyApp = (CMCPforNTApp*)AfxGetApp();
	if(pMyApp->CamacOn)
	{
		if((GetDocument()->MeasurementRunning&&pMyApp->GlobalMeasurementRunning&&RUNMODE==AUTOGO)||
			(!GetDocument()->MeasurementRunning&&!pMyApp->GlobalMeasurementRunning))
				SETAUTOGO = TRUE;
	}
	pCmdUI->Enable(SETAUTOGO);
	if((GetDocument()->MeasurementRunning&&pMyApp->GlobalMeasurementRunning&&RUNMODE==AUTOGO))
	{
		if(maxcycle > cycle)pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(2);
	}
	else pCmdUI->SetCheck(0);
}

void CMCPforNTView::IsRunningSave(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!GetDocument()->MeasurementRunning&&
					GetDocument()->DataValid&&
					GetDocument()->IsModified());
}

void* CMCPforNTView::CopyActionObject(ActionObject* pActionObj)
{
				CString TheObject = pActionObj->Name;

				if(TheObject==_HP_VoltageSweepObj)
				{
					HP_VoltageSweepObj* pNewActionObj = new HP_VoltageSweepObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_Line_VoltageSweepObj)
				{
					Line_VoltageSweepObj* pNewActionObj = new Line_VoltageSweepObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_HP8660B_FrequencySweepObj)
				{
					HP8660B_FrequencySweepObj* pNewActionObj = new HP8660B_FrequencySweepObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_HP3325B_FrequencySweepObj)
				{
					HP3325B_FrequencySweepObj* pNewActionObj = new HP3325B_FrequencySweepObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_HP3325B2_FrequencySweepObj)
				{
					HP3325B2_FrequencySweepObj* pNewActionObj = new HP3325B2_FrequencySweepObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_RS_FrequencySweepObj||TheObject==_RS2_FrequencySweepObj)
				{
					RS_FrequencySweepObj* pNewActionObj = new RS_FrequencySweepObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_PM_SpectrumObj)
				{
					PM_SpectrumObj* pNewActionObj = new PM_SpectrumObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_PremaVoltageObj)
				{
					PremaVoltageObj* pNewActionObj = new PremaVoltageObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_FlukeVoltageObj)
				{
					FlukeVoltageObj* pNewActionObj = new FlukeVoltageObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_IsoldeNetVoltageObj)
				{
					IsoldeNetVoltageObj* pNewActionObj = new IsoldeNetVoltageObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_KepcoEichungVoltageObj)
				{
					KepcoEichungVoltageObj* pNewActionObj = new KepcoEichungVoltageObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_ComputerDelayObj)
				{
					ComputerDelayObj* pNewActionObj = new ComputerDelayObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_TimerDelayObj)
				{
					TimerDelayObj* pNewActionObj = new TimerDelayObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_StepTimerDelayObj)
				{
					StepTimerDelayObj* pNewActionObj = new StepTimerDelayObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_FastTimerDelayObj)
				{
					FastTimerDelayObj* pNewActionObj = new FastTimerDelayObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_Keithley_VoltageMeasObj)
				{
					Keithley_VoltageMeasObj* pNewActionObj = new Keithley_VoltageMeasObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_FlukeOsziObj)
				{
					FlukeOsziObj* pNewActionObj = new FlukeOsziObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_MassAndFlukeSwitchObj)
				{
					MassAndFlukeSwitchObj* pNewActionObj = new MassAndFlukeSwitchObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_MassSwitchObj)
				{
					MassSwitchObj* pNewActionObj = new MassSwitchObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_FlukeSwitchObj)
				{
					FlukeSwitchObj* pNewActionObj = new FlukeSwitchObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_TriggerSweepObj)
				{
					TriggerSweepObj* pNewActionObj = new TriggerSweepObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_StepTriggerObj)
				{
					StepTriggerObj* pNewActionObj = new StepTriggerObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_TriggerObj)
				{
					TriggerObj* pNewActionObj = new TriggerObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_TriggerPartSweepObj)
				{
					TriggerPartSweepObj* pNewActionObj = new TriggerPartSweepObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_AlexMotorObj)
				{
					AlexMotorObj* pNewActionObj = new AlexMotorObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_GPIBReaderObj)
				{
					GPIBReaderObj* pNewActionObj = new GPIBReaderObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_FieldReaderObj)
				{
					FieldReaderObj* pNewActionObj = new FieldReaderObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_TempReaderObj)
				{
					TempReaderObj* pNewActionObj = new TempReaderObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
					if(TheObject==_TempWriterObj)
				{
					TempWriterObj* pNewActionObj = new TempWriterObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_OutBitObj)
				{
					OutBitObj* pNewActionObj = new OutBitObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_FieldWriterObj)
				{
					FieldWriterObj* pNewActionObj = new FieldWriterObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_GPIBWriterObj)
				{
					GPIBWriterObj* pNewActionObj = new GPIBWriterObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_MultiFunctionObj)
				{
					MultiFunctionObj* pNewActionObj = new MultiFunctionObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_FDAC_VoltageSweepObj)
				{
					FDAC_VoltageSweepObj* pNewActionObj = new FDAC_VoltageSweepObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_CamacLPObj)
				{
					CamacLPObj* pNewActionObj = new CamacLPObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_CCDCameraObj)
				{
					CCDCameraObj* pNewActionObj = new CCDCameraObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_DummyObj)
				{
					DummyObj* pNewActionObj = new DummyObj;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
				if(TheObject==_TrackObj)
				{
					TrackObject* pNewActionObj = new TrackObject;
					pNewActionObj->CopyObject(pActionObj);
					return pNewActionObj;
				}
	return NULL;
}

BOOL CMCPforNTView::SaveBackup(void)
{
	CString SaveString = "Backup.mcp";
	CFile File;
		
	if(!GetDocument()->OnSaveDocument(SaveString))
	{
		AfxMessageBox("Backup not saved !",MB_OK,0);
	}	
	return TRUE;
}

BOOL CMCPforNTView::SaveAuto(void)
{
	CString SaveFile,SaveFileLeft,SaveFileRight;
	CString TimeString;
	CFile File;
	int pos;

	CTime t = CTime::GetCurrentTime();
	TimeString = t.Format("@%H_%M_%S");
	SaveFile = GetDocument()->GetTitle();
	pos = SaveFile.Find('.');
	if(pos!=-1)
	{
		SaveFileLeft = SaveFile.Left(pos);
		SaveFileRight = SaveFile.Right(SaveFile.GetLength()-pos);
	}
	else
	{
		SaveFileLeft = SaveFile;
		SaveFileRight = "";
	}
	
	SaveFile = SaveFileLeft+TimeString+SaveFileRight; 

	if(!GetDocument()->OnSaveDocument(SaveFile))
	{
		AfxMessageBox("Document not saved !",MB_OK,0);
	}
	return TRUE;
}

LRESULT CMCPforNTView::OnSave(WPARAM wparam,LPARAM lparam)
{
	if(SaveWindow)
	{
		if(!GetDocument()->IsModified())
		{
			Beep(1000,100);
			return TRUE;
		}
		CString SaveString((LPCSTR)lparam);
		CFile File;
		if(File.Open(SaveString,CFile::modeRead))
		{
			AfxMessageBox("File exists, not saving !",MB_OK,0);
			File.Close;
			return TRUE;
		}
		else
		{ 
			if(!GetDocument()->OnSaveDocument(SaveString))
			{
				AfxMessageBox("Document not saved !",MB_OK,0);
			}
			else 
			{
				GetDocument()->SetTitle(SaveString);
				GetDocument()->SetPathName(SaveString);
				if(pCLSIView!=NULL)pCLSIView->GetParent()->SetWindowText("LSI-Box for "+GetDocument()->GetTitle());
			}
		}	
	}
	return TRUE;
}

void CMCPforNTView::OnGetFocus()
{
	SaveWindow = TRUE;
}

void CMCPforNTView::OnKillFocus()
{
	SaveWindow = FALSE;
}

void CMCPforNTView::RearrangeTrackNum(void)
{
	USHORT TrackNum = 1;
	TrackObject* pTrack;
	POSITION pos;
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)GetDocument();
	CString TheObject;
	ActionObject* pActionObj;

	if(!pDocument->ActionObjList.IsEmpty())
	{
		pos = pDocument->ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)pDocument->ActionObjList.GetNext(pos);
			if(pActionObj->Name==_TrackObj)
			{
				pTrack = (TrackObject*) pActionObj;
				pTrack->MyPosition = TrackNum++;
			}
		}
		while (pos!=NULL);
	}
}

PM_SpectrumObj* CMCPforNTView::FindScaler(USHORT Track,USHORT SubReg)
{
	POSITION pos;
	CString TheObject;
	if(!GetDocument()->ActionObjList.IsEmpty())
	{
		ActionObject* pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetHead();
		pos = GetDocument()->ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)GetDocument()->ActionObjList.GetNext(pos);
			if (pActionObj!=NULL)
			{
				TheObject = pActionObj->GetName();
				if (TheObject.Find(__PM_SpectrumObj)!=-1)
				{
					PM_SpectrumObj* pScalerObj = (PM_SpectrumObj*)pActionObj;
					if ((pScalerObj->SubRegNo==(int)SubReg)&&
						(pScalerObj->pTrack->MyPosition==(int)Track))
					{
						return pScalerObj;
					}
				}
			}
		}
		while (pos!=NULL);
	}
	return NULL;
}

void CMCPforNTView::OnConfigure() 
{
	// TODO: Add your command handler code here
	if (!GetDocument()->ActionObjList.IsEmpty())
	{
		int hItem = GetSelectedItem();
		if(hItem!=-1)
		{
			ActionObject* pActionObj = 
			(ActionObject*)GetListCtrl().GetItemData(hItem);
			if (pActionObj!= NULL)
			{
				pActionObj->DoConfigureAction();
			}
		}
	}	
}

int CMCPforNTView::GetSelectedItem() 
{
	// TODO: Add your command handler code here
	return GetListCtrl().GetNextItem(-1,LVNI_FOCUSED);
}

