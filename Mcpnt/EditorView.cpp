// EditorView.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "EditorChild.h"
#include "EditorView.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FontDim 15

/////////////////////////////////////////////////////////////////////////////
// CEditorView

IMPLEMENT_DYNCREATE(CEditorView, CEditView)

CEditorView::CEditorView()
{
		// TODO: add construction code here
	IsActivated = FALSE;
	CRect rect;
	pEditorChild = new CEditorChild(this);
	rect.top            = 100;
	rect.left           = 300;
	rect.right          = 850;
	rect.bottom         = 600;
	pEditorChild->CMDIChildWnd::Create(NULL,
						"Script-Window", 
						WS_CHILD|WS_VISIBLE|WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
						rect, 
						NULL, 
						NULL );

	this->Create(NULL,NULL,dwStyleDefault,rect,pEditorChild,AFX_IDW_PANE_FIRST+1,NULL);
	IsActivated=TRUE;
	pEditorChild->GetClientRect(rect);
	ResizeWin(rect.right,rect.bottom);
	LOGFONT logFont1;
    logFont1.lfHeight = FontDim;
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
	NewFont = new CFont();
	NewFont->CreateFontIndirect(&logFont1);
	SetFont(NewFont,FALSE);
	CFrameWnd* pFrame = (CFrameWnd*)GetParent();
	pFrame->SetActiveView(this,TRUE);

}

CEditorView::~CEditorView()
{
}


BEGIN_MESSAGE_MAP(CEditorView, CEditView)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CEditorView)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	ON_COMMAND(ID_POPUP_LOAD, OnPopupLoad)
	ON_COMMAND(ID_POPUP_SAVE, OnPopupSave)
	ON_COMMAND(ID_POPUP_CLEARWINDOW, OnPopupClearwindow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorView drawing

void CEditorView::ResizeWin(ULONG x,ULONG y)
{
	if (IsActivated)MoveWindow(0,0,x,y,TRUE);
}

void CEditorView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CEditorView diagnostics

#ifdef _DEBUG
void CEditorView::AssertValid() const
{
	CEditView::AssertValid();
}

void CEditorView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEditorView message handlers

void CEditorView::OnChange() 
{
	// TODO: Add your control notification handler code here
	
}

void CEditorView::OnUpdate() 
{
	// TODO: Add your control notification handler code here
	
}

void CEditorView::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete NewFont;
	CEditView::PostNcDestroy();
}

void CEditorView::OnContextMenu(CWnd*, CPoint point)
{
	// CG: This function was added by the Pop-up Menu component

	CMenu menu;
	VERIFY(menu.LoadMenu(CG_IDR_POPUP_EDITOR_VIEW));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);
}

BOOL CEditorView::PreTranslateMessage(MSG* pMsg)
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

	return CEditView::PreTranslateMessage(pMsg);
}

void CEditorView::OnPopupLoad() 
{
	// TODO: Add your command handler code here
	BOOL endmarker;
	CString FileName;
	CString WindowText;
	CString WindowLine;
	static char BASED_CODE szFilter[] = "Text Files (*.txt)|*.txt|Data Files (*.dat)|*.dat|All Files (*.*)|*.*||";
	CFileDialog* pLoadFile = new CFileDialog(TRUE,NULL,NULL,0,&szFilter[0],NULL);
	if (IDOK==pLoadFile->DoModal())
	{
		WindowText = "";
		FileName = pLoadFile->GetPathName();
		delete pLoadFile;
		CStdioFile* pFile = new CStdioFile(FileName,CFile::modeRead);
		do
		{
			endmarker = pFile->ReadString(WindowLine);
			WindowText = WindowText+WindowLine+"\r\n";
		}
		while (endmarker);

		GetEditCtrl().SetSel(-1,0,FALSE); 
		GetEditCtrl().ReplaceSel(WindowText);
		pFile->Close();
		delete pFile;
	}	
}

void CEditorView::OnPopupSave() 
{
	// TODO: Add your command handler code here
	CString FileName;
	CString WindowText;
	CString DataString;
	static char BASED_CODE szFilter[] = "Text Files (*.txt)|*.txt|Data Files (*.dat)|*.dat|All Files (*.*)|*.*||";
	CFileDialog* pSaveFile = new CFileDialog(FALSE,"dat",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,&szFilter[0],NULL);
	if(IDOK==pSaveFile->DoModal())
	{
		FileName = pSaveFile->GetPathName();
		delete pSaveFile;
		CStdioFile* pFile = new CStdioFile(FileName,CFile::modeWrite | CFile::modeCreate);
		GetWindowText(WindowText);
		pFile->WriteString(WindowText);
		pFile->Close();
		delete pFile;
	}	
}

void CEditorView::OnPopupClearwindow() 
{
	// TODO: Add your command handler code here
	SetWindowText("");
}
