// DataView.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "MinuitChild.h"
#include "minuit.h"
#include "MCP for NTDoc.h"
#include "ActionObjects.h"
#include "DataChild.h"
#include "DataView.h"
#include "MonitorChild.h"
#include "MonitorView.h"
#include "TheDefines.h"
#include "DataReadout.h"
#include "Configure.h"

#include "PropShtAS.h"
#include "PropShtPM.h"
#include "MinuitDlg.h"
#include "HfsFitDlg.h"

#include "eval.h"

#include <math.h>
#define WM_MINUITKILLED	WM_USER + 302
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const double SelectSize		= 6;
const SIZE  DragRectSize	= {1,1};
const SIZE  SelectRectSize	= {2,2};

const float OverScale    = (float)1.1;

const float minres       = (float)1e-5;
const float epsilon      = (float)minres/10;
const float epsilondata  = (float)5e-4;

const float epsilonShow  = (float)1e-7;
/*
const float AxisTextSize = (float)0.032;
const float NumberSize   = (float)0.028;

const float AxisLine     = (float)0.004;
const float TickLine     = (float)0.003;
const float DataLine     = (float)0.002;
const float FitFunc		 = (float)0.002;

const float MainTitleX   = (float)0.5;
const float MainTitleY   = (float)0.1;

const float XTitleX      = (float)0.5;
const float XTitleY      = (float)0.97;

const float YTitleX      = (float)0.03;
const float YTitleY      = (float)0.5;

const float TickLabelsX  = (float)0.905;
const float TickLabelsY  = (float)0.095;

const float PointSize    = (float)0.0045;
const float ErrorBarSize = (float)0.005;

const float DataxMin     = (float)0.1;
const float DatayMin     = (float)0.02;
const float DataxMax     = (float)0.98;
const float DatayMax     = (float)0.9;
const float TickLenX     = (float)0.015;
const float HalfTickLenX = (float)0.0075;
const float TickLenY     = (float)0.01;
const float HalfTickLenY = (float)0.005;
*/
/////////////////////////////////////////////////////////////////////////////
// DataView

IMPLEMENT_DYNCREATE(DataView, CView)

//DataView* DataView::pDataView;
DataView::DataView()
{
	pApp = (CMCPforNTApp*)AfxGetApp();
	DontDestroy = TRUE;
	DontRedrawAll = FALSE;
	
	data_type			= 2;
	OldPos				= -1;
	SelectOfs			= 0;
	curs = AfxGetApp()->LoadStandardCursor(IDC_CROSS);
	SCALER				= FALSE;
	ARITHSCALER			= FALSE;
	IntOnly				= FALSE;
	pActionObject		= NULL;
	pMinuit				= NULL;
	pMinuitPar			= NULL;
	FIT					= FALSE;
	STARTATZERO			= FALSE;
	LOGVIEW				= FALSE;

	SelectMax			= FALSE;
	SelectHfs			= FALSE;
	SelectNum			= 0;

	CRect rect;
	ActualIndex         = (int)0;
	LastShownDataPoint  = (float)0;
	minX                = (float)0;
	maxX                = (float)0;
	
	minY                = (float)0;
	maxY                = (float)0;

	fminX				= minX;
	fminY				= minY;
	fmaxX				= maxX;
	fmaxY				= maxY;

	MainTitle           = "No file loaded";
	XAxisTitle          = "x";
	XAxisUnit			= "";
	YAxisTitle          = "y";
	YAxisUnit			= "";

	LastShownDataPoint  = (float)1e10;
	DrawSingleDataPoint = FALSE;
	IsActivated         = FALSE;
	UserIsSelecting		= FALSE;
	IsFirstMove			= FALSE;
	DataSelect			= FALSE;

	rect.top            = 10;
	rect.left           = 300;
	rect.right          = 900;
	rect.bottom         = 400;

	pDataChild = new DataChild(this);
	pDataChild->CMDIChildWndEx::Create(NULL,
						"Scaler", 
						WS_CHILD|WS_VISIBLE|WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
						CFrameWnd::rectDefault, 
						NULL, 
						NULL );

	this->Create(NULL,NULL,WS_CHILD|WS_VISIBLE|
	WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rect,pDataChild,AFX_IDW_PANE_FIRST+1,NULL);
	IsActivated=TRUE;
	pDataChild->GetClientRect(rect);
	ResizeWin(rect.right,rect.bottom);

	// CG: This line was added by the Clipboard Assistant component
	m_nClipboardFormat = ::RegisterClipboardFormat(_T("CF_METAFILEPICT"));
	CFrameWnd* pFrame = (CFrameWnd*)GetParent();
	pFrame->SetActiveView(this,TRUE);

}

DataView::DataView(CMinuit* pMinuitParent)
{
	pApp = (CMCPforNTApp*)AfxGetApp();
	DontDestroy = TRUE;
	DontRedrawAll = FALSE;
	
	data_type = 2;
	OldPos				= -1;
	SelectOfs		= 0;
	curs = AfxGetApp()->LoadStandardCursor(IDC_CROSS);
	SCALER				= FALSE;
	ARITHSCALER			= FALSE;
	IntOnly				= FALSE;
	pActionObject		= NULL;
	pMinuit				= NULL;
	pMinuitPar			= pMinuitParent;
	FIT					= FALSE;
	STARTATZERO			= FALSE;
	LOGVIEW				= FALSE;

	SelectMax			= FALSE;
	SelectHfs			= FALSE;
	SelectNum			= 0;

	CRect rect;
	ActualIndex         = (int)0;
	LastShownDataPoint  = (float)0;
	minX                = (float)1e10;
	maxX                = (float)-1e10;
	
	minY                = (float)1e10;
	maxY                = (float)-1e10;

	fminX				= minX;
	fminY				= minY;
	fmaxX				= maxX;
	fmaxY				= maxY;

	MainTitle           = "";
	XAxisTitle          = "";
	XAxisUnit			= "";
	YAxisTitle          = "";
	YAxisUnit			= "";

	LastShownDataPoint  = (float)1e10;
	DrawSingleDataPoint = FALSE;
	IsActivated         = FALSE;
	UserIsSelecting		= FALSE;
	IsFirstMove			= FALSE;
	DataSelect			= FALSE;

	rect.top            = 10;
	rect.left           = 300;
	rect.right          = 900;
	rect.bottom         = 400;

	pDataChild = new DataChild(this);
	pDataChild->CMDIChildWndEx::Create(NULL,
						"Scaler", 
						WS_CHILD|WS_VISIBLE|WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
						CFrameWnd::rectDefault, 
						NULL, 
						NULL );

	this->Create(NULL,NULL,WS_CHILD|WS_VISIBLE|
	WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rect,pDataChild,AFX_IDW_PANE_FIRST+1,NULL);
	IsActivated=TRUE;
	pDataChild->GetClientRect(rect);
	ResizeWin(rect.right,rect.bottom);

	// CG: This line was added by the Clipboard Assistant component
	m_nClipboardFormat = ::RegisterClipboardFormat(_T("CF_METAFILEPICT"));
	CFrameWnd* pFrame = (CFrameWnd*)GetParent();
	pFrame->SetActiveView(this,TRUE);

}

DataView::DataView(ActionObject* pAction)
{
	pApp = (CMCPforNTApp*)AfxGetApp();
	DontDestroy = FALSE;
	DontRedrawAll = FALSE;
	
	data_type = 2;
	OldPos				= -1;
	SelectOfs		= 0;
	curs = AfxGetApp()->LoadStandardCursor(IDC_CROSS);
	SCALER				= TRUE;
	ARITHSCALER			= pAction->ARITHSCALER;
	IntOnly				= FALSE;
	SelectMax			= FALSE;
	SelectHfs			= FALSE;
	SelectNum			= 0;
	pMinuit				= NULL;
	pMinuitPar			= NULL;
	FIT					= FALSE;
	STARTATZERO			= FALSE;
	LOGVIEW				= FALSE;

	pActionObject		= pAction;
	CRect rect;
	ActualIndex         = (int)0;
	LastShownDataPoint  = (float)0;
	minX                = (float)1e10;
	maxX                = (float)-1e10;
	
	minY                = (float)1e10;
	maxY                = (float)-1e10;

	fminX				= minX;
	fminY				= minY;
	fmaxX				= maxX;
	fmaxY				= maxY;

	MainTitle           = "";
	XAxisTitle          = "";
	XAxisUnit			= "";
	YAxisTitle          = "";
	YAxisUnit			= "";

	LastShownDataPoint  = (float)1e10;
	DrawSingleDataPoint = FALSE;
	IsActivated         = FALSE;
	UserIsSelecting		= FALSE;
	IsFirstMove			= FALSE;
	DataSelect			= FALSE;

	rect.top            = 10;
	rect.left           = 300;
	rect.right          = 900;
	rect.bottom         = 400;

	pDataChild = new DataChild(this);
	pDataChild->CMDIChildWndEx::Create(NULL,
						"Scaler", 
						WS_CHILD|WS_VISIBLE|WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
						rect, 
						NULL, 
						NULL );

	this->Create(NULL,NULL,WS_CHILD|WS_VISIBLE|
	WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rect,pDataChild,AFX_IDW_PANE_FIRST+1,NULL);
	IsActivated=TRUE;
	pDataChild->GetClientRect(rect);
	ResizeWin(rect.right,rect.bottom);

	m_nClipboardFormat = ::RegisterClipboardFormat(_T("CF_METAFILEPICT"));

	CFrameWnd* pFrame = (CFrameWnd*)GetParent();
	pFrame->SetActiveView(this,TRUE);
}


DataView::~DataView()
{
	if(pMinuitPar!=NULL)pMinuitPar->pResView = NULL;
	IsActivated=FALSE;
}


BEGIN_MESSAGE_MAP(DataView, CView)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopyCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCopyCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY_TB, OnUpdateEditCopyCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT_TB, OnUpdateEditCopyCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_TB, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_COPY_TB, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT_TB, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE_TB, OnEditPaste)
	//{{AFX_MSG_MAP(DataView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_POPUP_RESIZETOFULLSCALE, OnPopupResizetofullscale)
	ON_COMMAND(ID_POPUP_SHOWPROPERTIES, OnPopupShowproperties)
	ON_COMMAND(ID_POPUP_SAVESPEC, OnPopupSavespec)
	ON_COMMAND(ID_POPUP_MINUITFITSESSION, OnPopupMinuitfitsession)
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_POPUP_LOADSPEC, OnPopupLoadspec)
	ON_COMMAND(ID_POPUP_SAVEWMF, OnPopupSavewmf)
	//}}AFX_MSG_MAP
	//ON_UPDATE_COMMAND_UI(ID_POPUP_SHOWPROPERTIES,IsScaler)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_MESSAGE(WM_MINUITKILLED,DataView::OnMinuitKilled)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DataView drawing

afx_msg void DataView::OnFilePrintPreview()
{
	/*
    // In derived classes, implement special window handling here
    // Be sure to Unhook Frame Window close if hooked.

    // must not create this on the frame. Must outlive this function
    CPrintPreviewState* pState = new CPrintPreviewState;

    if (!DoPrintPreview(AFX_IDD_PREVIEW_TOOLBAR, this,
                RUNTIME_CLASS(CPreviewView), pState))
    {
        // In derived classes, reverse special window handling
        // here for Preview failure case

        TRACE0("Error: DoPrintPreview failed");
        AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
        delete pState;      // preview failed to initialize, 
                    // delete State now
    }
	*/
}


inline float DataView::GetStep(float Min,float Max)
{
	float MinMax;
	float Step;
	MinMax=fabs(Max-Min);
	Step = minres;
	while (MinMax >= Step)
	{
		Step=Step*10;
	}
	if (Step < (MinMax*2)) Step=Step/10;
	else if (Step < (MinMax*5)) Step=Step/20;
	else if (Step < (MinMax*10)) Step=Step/50;
	if((Max-Min)<=0)Step = -Step;
	return Step;
}

inline void DataView::DrawBoundingRect(CDC* pDC,CRect rect)
{
	int dim=rect.right;
	if (dim>rect.bottom)dim=rect.bottom;
	dim = (int)(dim * cor);
	CPen* pPen = new CPen();
	pPen->CreatePen(PS_SOLID,(int)(pApp->AxisLine*dim),RGB(pApp->AxisRed,
													pApp->AxisGreen,
													pApp->AxisBlue));
	CPen* OldPen = pDC->SelectObject(pPen);

	pDC->MoveTo((int)(rect.right*pApp->DataxMin),(int)(rect.bottom*pApp->DatayMin));
	pDC->LineTo((int)(rect.right*pApp->DataxMin),(int)(rect.bottom*pApp->DatayMax));
	pDC->LineTo((int)(rect.right*pApp->DataxMax),(int)(rect.bottom*pApp->DatayMax));
	pDC->LineTo((int)(rect.right*pApp->DataxMax),(int)(rect.bottom*pApp->DatayMin));
	pDC->LineTo((int)(rect.right*pApp->DataxMin),(int)(rect.bottom*pApp->DatayMin));
	
	pDC->SelectObject(OldPen);
	delete pPen;
}

inline void DataView::DrawXORLine(CDC* pDC,int pos, int height)
{
	if(OldPos != -1) pDC->BitBlt(OldPos,0,1,height, NULL, 0, 0, DSTINVERT);
	pDC->BitBlt(pos,0,1,height, NULL, 0, 0, DSTINVERT);
	OldPos = pos;
}

inline void DataView::DrawTitles(CDC* pDC,CRect rect,CString Title,CString XTitle,CString YTitle)
{
	CString MyYTitle;
	int dim=rect.right;
	if (dim>rect.bottom)dim=rect.bottom;
	dim = (int)(dim * cor);
	LOGFONT logFont1;
    logFont1.lfHeight = (long)(pApp->AxisTextSize*dim);
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
	strcpy(logFont1.lfFaceName, "Arial");

	CFont* NewFont = new CFont();
	NewFont->CreateFontIndirect(&logFont1);
	CFont* OldFont = pDC->SelectObject(NewFont);
	pDC->SetTextAlign(TA_CENTER|TA_BASELINE); 
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(pApp->TextRed,
							pApp->TextGreen,
							pApp->TextBlue));
	pDC->TextOut((int)(pApp->MainTitleX*rect.right),(int)(pApp->MainTitleY*rect.bottom),Title);
	pDC->TextOut((int)(pApp->XTitleX*rect.right),(int)(pApp->XTitleY*rect.bottom),XTitle);

	pDC->SelectObject(OldFont);
	delete NewFont;

	LOGFONT logFont2;
    logFont2.lfHeight = (long)(pApp->AxisTextSize*dim);
	logFont2.lfWidth = 0;
    logFont2.lfEscapement = 900;
    logFont2.lfOrientation = 0;
    logFont2.lfWeight = FW_NORMAL;
    logFont2.lfItalic = 0;
    logFont2.lfUnderline = 0;
    logFont2.lfStrikeOut = 0;
    logFont2.lfCharSet = ANSI_CHARSET;
    logFont2.lfOutPrecision = OUT_DEFAULT_PRECIS;
    logFont2.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    logFont2.lfQuality = PROOF_QUALITY;
    logFont2.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	strcpy(logFont2.lfFaceName, "Arial");

	CFont* NewFont2 = new CFont();
	NewFont2->CreateFontIndirect(&logFont2);
	CFont* OldFont2 = pDC->SelectObject(NewFont2);
	pDC->SetTextAlign(TA_CENTER|TA_BASELINE); 
	if(LOGVIEW)MyYTitle = "log of "+YTitle;
	else MyYTitle = YTitle;
	pDC->TextOut((int)(pApp->YTitleX*rect.right),(int)(pApp->YTitleY*rect.bottom),MyYTitle);
	pDC->SelectObject(OldFont2);
	delete NewFont2;
}

inline int DataView::ConvertX(float Xin,CRect rect,float XMin,float XMax)
{
	int x;
	x=(int)(((Xin-XMin)*(pApp->DataxMax-pApp->DataxMin)*rect.right)/(XMax-XMin)+pApp->DataxMin*rect.right);
	return x;
}

inline int DataView::ConvertY(float Yin,CRect rect,float YMin,float YMax)
{
	int y;
	if(!LOGVIEW)y=(int)(pApp->DatayMax*rect.bottom-((Yin-YMin)*(pApp->DatayMax-pApp->DatayMin)*rect.bottom)/(YMax-YMin));
	else y=(int)(pApp->DatayMax*rect.bottom-((log10(Yin)-YMin)*(pApp->DatayMax-pApp->DatayMin)*rect.bottom)/(YMax-YMin));
	return y;
}

inline int DataView::TextConvertY(float Yin,CRect rect,float YMin,float YMax)
{
	int y;
	y=(int)(pApp->DatayMax*rect.bottom-((Yin-YMin)*(pApp->DatayMax-pApp->DatayMin)*rect.bottom)/(YMax-YMin));
	return y;
}

inline float DataView::InvConvertX(int x,CRect rect,float XMin,float XMax)
{
	float Xin;
	Xin=(x-pApp->DataxMin*rect.right)*(XMax-XMin)/((pApp->DataxMax-pApp->DataxMin)*rect.right)+XMin;
	return Xin;
}

inline float DataView::InvConvertY(int y,CRect rect,float YMin,float YMax)
{
	float Yin;
	if(!LOGVIEW)Yin=YMin-(y-pApp->DatayMax*rect.bottom)*(YMax-YMin)/((pApp->DatayMax-pApp->DatayMin)*rect.bottom);
	else Yin=exp((float)2.3016*(YMin-(y-pApp->DatayMax*rect.bottom)*(YMax-YMin)/((pApp->DatayMax-pApp->DatayMin)*rect.bottom)));
	return Yin;
}

inline int DataView::ConvertErr(float Yin,CRect rect,float YMin,float YMax)
{
	int y;
	y=(int)((Yin*(pApp->DatayMax-pApp->DatayMin)*rect.bottom)/(YMax-YMin));
	return y;
}

inline void DataView::DrawDataPoint(CDC* pDC,CRect rect,USHORT index,int actual_type)
{
	int dim=rect.right;
	if (dim>rect.bottom)dim=rect.bottom;
	dim = (int)(dim * cor);
	int eSizeP,eSizeM;

	int xi,yi,xi_1,yi_1;

	int pSize = (int)(pApp->PointSize*dim);
	int dSize = (int)(pApp->ErrorBarSize*dim);


	xi = ConvertX(DataPoint[0][index],rect,minX,maxX);
	yi = ConvertY(DataPoint[1][index],rect,minY,maxY);

	if ((xi >= (rect.right*pApp->DataxMin ))&&(xi <= (rect.right*pApp->DataxMax )))
	{
		if(actual_type == 0)
		{
			eSizeM = ConvertY(DataPoint[1][index]+DataPoint[2][index],rect,minY,maxY);
			eSizeP = ConvertY(DataPoint[1][index]-DataPoint[2][index],rect,minY,maxY);
	
			if (eSizeM > (int)(rect.bottom*pApp->DatayMax)) eSizeM=(int)(rect.bottom*pApp->DatayMax);
			if (eSizeP < (int)(rect.bottom*pApp->DatayMin)) eSizeP=(int)(rect.bottom*pApp->DatayMin);
	
			pDC->MoveTo(xi,eSizeP);
			pDC->LineTo(xi,eSizeM);
	
			pDC->MoveTo(xi-dSize,eSizeP);
			pDC->LineTo(xi+dSize+1,eSizeP);
	
			pDC->MoveTo(xi-dSize,eSizeM);
			pDC->LineTo(xi+dSize+1,eSizeM);
	
			pDC->Rectangle(xi-pSize,yi-pSize,xi+pSize+1,yi+pSize+1);
		}
		
		if(actual_type == 1)
		{
			if(index == 0)index = 1;
		
			xi_1 = ConvertX(DataPoint[0][index-1],rect,minX,maxX);
			yi_1 = ConvertY(DataPoint[1][index-1],rect,minY,maxY);
		
			pDC->MoveTo(xi_1,yi_1);
			pDC->LineTo(xi,yi);
		}
		
		if(actual_type == 2)
		{
			int xmin	= ConvertX(DataPoint[0][0],rect,minX,maxX);
			int xmax	= ConvertX(DataPoint[0][ActualIndex-1],rect,minX,maxX);
			int dx		= (xmax-xmin)/ActualIndex;
			if(index == 0)
			{
				pDC->MoveTo(xi-dx/2,yi);
				pDC->LineTo(xi+dx/2,yi);
			}
			else
			{
				xi_1 = ConvertX(DataPoint[0][index-1],rect,minX,maxX);
				yi_1 = ConvertY(DataPoint[1][index-1],rect,minY,maxY);
			
				pDC->MoveTo(xi_1+dx/2,yi_1);
				pDC->LineTo(xi_1+dx/2,yi);
				pDC->LineTo(xi+dx/2,yi);
			}
		}
	}
}

inline void DataView::DrawSelectedDataPoint(CDC* pDC,CRect rect,float x,float y,float err,float XMin,float XMax,float YMin,float YMax)
{
	int dim=rect.right;
	if (dim>rect.bottom)dim=rect.bottom;
	dim = (int)(dim * cor);
	int eSizeP,eSizeM;
	CPen* pPen = new CPen();
	pPen->CreatePen(PS_SOLID,(int)(pApp->DataLine*dim),RGB(pApp->SelectRed,
													pApp->SelectGreen,
													pApp->SelectBlue));
	CPen* OldPen = pDC->SelectObject(pPen);
	int pSize = (int)(pApp->PointSize*dim);
	int dSize = (int)(pApp->ErrorBarSize*dim);

	int xi = ConvertX(x,rect,XMin,XMax);
	int yi = ConvertY(y,rect,YMin,YMax);
	if ((xi >= (rect.right*pApp->DataxMin ))&&(xi <= (rect.right*pApp->DataxMax ))&&
	    (yi >= (rect.bottom*pApp->DatayMin))&&(yi <= (rect.bottom*pApp->DatayMax)))
	{
	int eSize = ConvertErr(err,rect,YMin,YMax);
	eSizeM=yi+eSize;
	eSizeP=yi-eSize;
	
	if (eSizeM > (int)(rect.bottom*pApp->DatayMax)) eSizeM=(int)(rect.bottom*pApp->DatayMax);
	if (eSizeP < (int)(rect.bottom*pApp->DatayMin)) eSizeP=(int)(rect.bottom*pApp->DatayMin);
	
	pDC->MoveTo(xi,eSizeP);
	pDC->LineTo(xi,eSizeM);
	
	pDC->MoveTo(xi-dSize,eSizeP);
	pDC->LineTo(xi+dSize+1,eSizeP);
	
	pDC->MoveTo(xi-dSize,eSizeM);
	pDC->LineTo(xi+dSize+1,eSizeM);
	
	pDC->Rectangle(xi-pSize,yi-pSize,xi+pSize+1,yi+pSize+1);
	}

	pDC->SelectObject(OldPen);

	delete pPen;
}


void DataView::DrawTheData(CDC* pDC,CRect rect,float Xmin,float Xmax,float Ymin,float Ymax)
{
	int dim=rect.right;
	if (dim>rect.bottom)dim=rect.bottom;
	dim = (int)(dim * cor);
	CPen* pPen = new CPen();
	CBrush* pBrush = new CBrush();
	pPen->CreatePen(PS_SOLID,(int)(pApp->DataLine*dim),RGB(pApp->DataRed,
													pApp->DataGreen,
													pApp->DataBlue));
	pBrush->CreateSolidBrush(RGB(255,255,255));
	CPen* OldPen = pDC->SelectObject(pPen);
	CBrush* OldBrush = pDC->SelectObject(pBrush);

	int i;
	int ActIndex;
	ActIndex=ActualIndex;
	if (ActIndex==MAXPOINTS)ActIndex=MAXPOINTS-1;

	int actual_type = data_type;

	for (i=0;i<ActIndex;i++)
			DrawDataPoint(pDC,rect,i,actual_type);

	pDC->SelectObject(OldPen);
	pDC->SelectObject(OldBrush);
	delete pPen;
	delete pBrush;
}

inline void DataView::DrawXTicks(CDC *pDC,CRect rect,float XMin,float XMax)
{
	int dim=rect.right;
	if (dim>rect.bottom)dim=rect.bottom;
	dim = (int)(dim * cor);
	int i;
	float StepSize=GetStep(XMin,XMax);
	if(IntOnly)
	{
		StepSize = (float)(int)StepSize;
		if(StepSize == 0)StepSize = (float) 1;
	}
	float Size=XMax-XMin;
	float Step=(float)0.1;
	float Offs;
	float PosX;

	Offs=StepSize*(float)(long)(XMin/StepSize);
	PosX=Offs-StepSize;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(pApp->TextRed,
							pApp->TextGreen,
							pApp->TextBlue));
	CPen* pPen = new CPen();
	pPen->CreatePen(PS_SOLID,(int)(pApp->TickLine*dim),RGB(pApp->AxisRed,
													pApp->AxisGreen,
													pApp->AxisBlue));
	CPen* OldPen = pDC->SelectObject(pPen);

	LOGFONT logFont2;
    logFont2.lfHeight = (long)(pApp->NumberSize*dim);
	logFont2.lfWidth = 0;
    logFont2.lfEscapement = 0;
    logFont2.lfOrientation = 0;
    logFont2.lfWeight = FW_NORMAL;
    logFont2.lfItalic = 0;
    logFont2.lfUnderline = 0;
    logFont2.lfStrikeOut = 0;
    logFont2.lfCharSet = ANSI_CHARSET;
    logFont2.lfOutPrecision = OUT_DEFAULT_PRECIS;
    logFont2.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    logFont2.lfQuality = PROOF_QUALITY;
    logFont2.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	strcpy(logFont2.lfFaceName, "Arial");

	CFont* NewFont = new CFont();
	NewFont->CreateFontIndirect(&logFont2);
	CFont* OldFont = pDC->SelectObject(NewFont);
	pDC->SetTextAlign(TA_CENTER|TA_TOP); 

	for (i=0;i<(Size/StepSize)+2;i++)

	{
		int x=ConvertX(PosX,rect,XMin,XMax);
		if ((x>=pApp->DataxMin*rect.right)&&(x<=pApp->DataxMax*rect.right))
			{
				pDC->MoveTo(x,(int)(pApp->DatayMin*rect.bottom));
				pDC->LineTo(x,(int)((pApp->DatayMin+pApp->TickLenX)*rect.bottom));
				pDC->MoveTo(x,(int)(pApp->DatayMax*rect.bottom));
				pDC->LineTo(x,(int)((pApp->DatayMax-pApp->TickLenX)*rect.bottom));

				CString TickLabels;
				float PosXText = PosX+epsilonShow*PosX;
				if (abs(PosXText) < (XMax-XMin)*epsilondata)PosXText = (float)0;

				TickLabels.Format("%g",PosXText);
				pDC->TextOut(x,(int)(pApp->TickLabelsX*rect.bottom),TickLabels);
			}

		PosX=PosX+StepSize/2;
		if (fabs(PosX) < epsilon)PosX=(float)0;
		x=ConvertX(PosX,rect,XMin,XMax);

		if ((x>=pApp->DataxMin*rect.right)&&(x<=pApp->DataxMax*rect.right))
			{
				pDC->MoveTo(x,(int)(pApp->DatayMin*rect.bottom));
				pDC->LineTo(x,(int)((pApp->DatayMin+pApp->HalfTickLenX)*rect.bottom));
				pDC->MoveTo(x,(int)(pApp->DatayMax*rect.bottom));
				pDC->LineTo(x,(int)((pApp->DatayMax-pApp->HalfTickLenX)*rect.bottom));
			}
		PosX=PosX+StepSize/2;
		if (fabs(PosX) < epsilon)PosX=(float)0;
	}
	pDC->SelectObject(OldFont);
	pDC->SelectObject(OldPen);
	delete pPen;
	delete NewFont;
}

inline void DataView::DrawYTicks(CDC *pDC,CRect rect,float YMin,float YMax)
{
	int dim=rect.right;
	if (dim>rect.bottom)dim=rect.bottom;
	dim = (int)(dim * cor);
	int i;
	float StepSize=GetStep(YMin,YMax);
	float Size=YMax-YMin;
	float Step=(float)0.1;
	float Offs;
	float PosY;

	Offs=StepSize*(float)(long)(YMin/StepSize);
	PosY=Offs-StepSize;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(pApp->TextRed,
							pApp->TextGreen,
							pApp->TextBlue));
	CPen* pPen = new CPen();
	pPen->CreatePen(PS_SOLID,(int)(pApp->TickLine*dim),RGB(pApp->AxisRed,
													pApp->AxisGreen,
													pApp->AxisBlue));
	CPen* OldPen = pDC->SelectObject(pPen);

	LOGFONT logFont2;
    logFont2.lfHeight = (long)(pApp->NumberSize*dim);
	logFont2.lfWidth = 0;
    logFont2.lfEscapement = 0;
    logFont2.lfOrientation = 0;
    logFont2.lfWeight = FW_NORMAL;
    logFont2.lfItalic = 0;
    logFont2.lfUnderline = 0;
    logFont2.lfStrikeOut = 0;
    logFont2.lfCharSet = ANSI_CHARSET;
    logFont2.lfOutPrecision = OUT_DEFAULT_PRECIS;
    logFont2.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    logFont2.lfQuality = PROOF_QUALITY;
    logFont2.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	strcpy(logFont2.lfFaceName, "Arial");

	CFont* NewFont = new CFont();
	NewFont->CreateFontIndirect(&logFont2);
	CFont* OldFont = pDC->SelectObject(NewFont);
	pDC->SetTextAlign(TA_RIGHT|TA_BASELINE); 

	for (i=0;i<(Size/StepSize)+2;i++)
	{
		int y=TextConvertY(PosY,rect,YMin,YMax);
		if (fabs(PosY) < epsilon)PosY=(float)0;
		if ((y>=(int)(pApp->DatayMin*rect.bottom))&&(y<=(int)(pApp->DatayMax*rect.bottom)))
			{
				pDC->MoveTo((int)(pApp->DataxMin*rect.right),y);
				pDC->LineTo((int)((pApp->DataxMin+pApp->TickLenY)*rect.right),y);
				pDC->MoveTo((int)(pApp->DataxMax*rect.right),y);
				pDC->LineTo((int)((pApp->DataxMax-pApp->TickLenY)*rect.right),y);

				CString TickLabels;
				float PosYText = PosY+epsilonShow*PosY;
				if (abs(PosYText) < (YMax-YMin)*epsilondata)PosYText = (float)0;
				TickLabels.Format("%g",PosYText);
				pDC->TextOut((int)(pApp->TickLabelsY*rect.right),y,TickLabels);


			}
		PosY=PosY+StepSize/2;
		y=TextConvertY(PosY,rect,YMin,YMax);
		if ((y>=pApp->DatayMin*rect.bottom)&&(y<=pApp->DatayMax*rect.bottom))
			{
				pDC->MoveTo((int)(pApp->DataxMin*rect.right),y);
				pDC->LineTo((int)((pApp->DataxMin+pApp->HalfTickLenY)*rect.right),y);
				pDC->MoveTo((int)(pApp->DataxMax*rect.right),y);
				pDC->LineTo((int)((pApp->DataxMax-pApp->HalfTickLenY)*rect.right),y);
			}
		PosY=PosY+StepSize/2;
		if (fabs(PosY) < epsilon)PosY=(float)0;
	}
	pDC->SelectObject(OldFont);
	pDC->SelectObject(OldPen);
	delete pPen;
	delete NewFont;
}

void DataView::DrawFitFunction(CDC* pDC,CRect rect)
{

	int i;
	int dim=rect.right;
	if (dim>rect.bottom)dim=rect.bottom;
	dim = (int)(dim * cor);
	CPen* pPen = new CPen();
	pPen->CreatePen(PS_SOLID,(int)(pApp->FitFunc*dim),RGB(pApp->FitRed,
													pApp->FitGreen,
													pApp->FitBlue));
	CPen* OldPen = pDC->SelectObject(pPen);


	pDC->MoveTo(ConvertX(pMinuit->FitFunc[0][0],rect,minX,maxX),
					ConvertY(pMinuit->FitFunc[1][0],rect,minY,maxY));

	for (i=1;i<pApp->FitFunRes;i++)
	{
			
		int xi = ConvertX(pMinuit->FitFunc[0][i],rect,minX,maxX);
		int yi = ConvertY(pMinuit->FitFunc[1][i],rect,minY,maxY);
		pDC->LineTo(xi,yi);
	}


	pDC->SelectObject(OldPen);
	delete pPen;
};

void DataView::OnDraw(CDC* pDC)
{

	CDocument* pDoc = GetDocument();
	CRect rect;
	GetClientRect(rect);
	// TODO: add draw code here
	if (pDC->IsPrinting())
	{
		cor = 0.5;
		rect.right=(long)(pDC->GetDeviceCaps(HORZRES)*0.9);
		rect.bottom=(long)(pDC->GetDeviceCaps(VERTRES)*0.9);
		if (rect.bottom > rect.right)rect.bottom=(long)(rect.right*0.75);
		CTime theTime;
		theTime=CTime::GetCurrentTime();
		CString s = theTime.Format( "%A, %B %d, %Y, %X");
		pDC->SetTextAlign(TA_LEFT|TA_BOTTOM); 
		pDC->TextOut((int)(rect.right*0.05),(int)(rect.bottom*1.05),WindowTitle);
		pDC->TextOut((int)(rect.right*0.05),(int)(rect.bottom*1.1),s);
	}
	else cor = 1;

	if (DrawSingleDataPoint)
	{
		DrawSingleDataPoint=FALSE;
		DontRedrawAll = FALSE;
		int actual_type = data_type;
		if (SCALER)
		{
			if (pActionObject->pDocument->MeasurementRunning)actual_type = data_type;
		}
		if (DataPoint[1][ActualIndex-1]<LastShownDataPoint)
			DrawDataPoint(pDC,rect,ActualIndex-1,actual_type);
		
	}
	else
	{
		if (!DontRedrawAll)
		{
			DrawBoundingRect(pDC,rect);
			DrawTitles(pDC,rect,MainTitle,XAxisTitle+" ["+XAxisUnit+"]",YAxisTitle+" ["+YAxisUnit+"]");
			DrawXTicks(pDC,rect,minX,maxX);
			DrawYTicks(pDC,rect,minY,maxY);
		}
		DontRedrawAll = FALSE;
		DrawTheData(pDC,rect,minX,maxX,minY,maxY);
	}
	if (FIT)
	{
		DrawFitFunction(pDC,rect);
	}
}

/////////////////////////////////////////////////////////////////////////////
// DataView diagnostics

#ifdef _DEBUG
void DataView::AssertValid() const
{
	CView::AssertValid();
}

void DataView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// DataView message handlers

BOOL DataView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return DoPreparePrinting(pInfo);
}

void DataView::ResizeWin(ULONG x,ULONG y)
{
	if (IsActivated)MoveWindow(0,0,x,y,TRUE);
}

void DataView::AddDataPoint(float x,float y,float err)
{
 	DataPoint[0][ActualIndex] = x;
	DataPoint[1][ActualIndex] = y;
	DataPoint[2][ActualIndex] = err;

	if (x < minX)
	{
		minX  = x;
		fminX = minX;
	}
	if (x > maxX)
	{
		maxX  = x;
		fmaxX = maxX;
	}
	if (y-err < minY)
	{
		minY  = y-err;
		fminY = minY;
	}
	if (y+err > maxY)
	{
		maxY  = y+err;
		fmaxY = maxY;
	}
	ActualIndex++;
}

void DataView::SetXAxis(float Xmin,float Xmax)
{
	minX=Xmin;
	maxX=Xmax;
	fminX=Xmin;
	fmaxX=Xmax;
	Invalidate(TRUE);
}

void DataView::SetYAxis(float Ymin,float Ymax)
{
	minY=Ymin;
	maxY=Ymax;
	fminY=Ymin;
	fmaxY=Ymax;
	Invalidate(TRUE);
}

void DataView::SetTitle(CString Title)
{
	MainTitle=Title;
	Invalidate(TRUE);

}

void DataView::SetWindowTitle(CString Title)
{
	GetParent()->SetWindowText(Title);
	WindowTitle=Title;
}

void DataView::SetXTitle(CString XTitle, CString XUnit)
{
	XAxisTitle=XTitle;
	XAxisUnit=XUnit;
	Invalidate(TRUE);
}

void DataView::SetYTitle(CString YTitle, CString YUnit)
{
	YAxisTitle=YTitle;
	YAxisUnit=YUnit;
	Invalidate(TRUE);
}

void DataView::RedrawNow(void)
{
	if (ActualIndex > 0)
	{
		if(ARITHSCALER)
		{
			CMCPforNTDoc* pDoc;
			pDoc = pActionObject->pDocument;
			double y;
			int err,i;
			char evalStr[255];
			char errorStr[255];
			strcpy(evalStr,pActionObject->scan_data_op);
			strcpy(errorStr,pActionObject->scan_error_op);
			for (i=0;i<ActualIndex;i++) 
			{
				Evaluate(pDoc,&evalStr[0],&y,&err,i);
				DataPoint[1][i]=(float)y;
				if(pActionObject->scan_error_op.IsEmpty())
				{
					DataPoint[2][i]=(float)sqrt(fabs(y));
				}
				else
				{
					Evaluate(pDoc,&errorStr[0],&y,&err,i);
					DataPoint[2][i]=(float)y;
				}
			}
		}
	int i;
	float xl,xu,yl,yu;
	
	xl=(float)1e10;
	yl=(float)1e10;
	
	xu=(float)-1e10;
	yu=(float)-1e10;

	if(!LOGVIEW)
	{
		for (i=0;i<ActualIndex;i++)
		{
			if(DataPoint[0][i]<xl)xl=DataPoint[0][i];
			if(DataPoint[0][i]>xu)xu=DataPoint[0][i];
			if((DataPoint[1][i]-DataPoint[2][i])<yl)yl=DataPoint[1][i]-DataPoint[2][i];
			if((DataPoint[1][i]+DataPoint[2][i])>yu)yu=DataPoint[1][i]+DataPoint[2][i];
		}
	}
	else
	{
		for (i=0;i<ActualIndex;i++)
		{
			if((DataPoint[1][i])!=0)
			{
				if(DataPoint[0][i]<xl)xl=DataPoint[0][i];
				if(DataPoint[0][i]>xu)xu=DataPoint[0][i];
				if(log10(DataPoint[1][i])<yl)yl=log10(DataPoint[1][i]);
				if(log10(DataPoint[1][i])>yu)yu=log10(DataPoint[1][i]);
			}
		}
	}
	if((yl>0)&&(STARTATZERO == TRUE))yl = (float)0;

	if((yu<0)&&(STARTATZERO == TRUE))yu = (float)0;

	minX  = xl-(xu-xl)*(float)0.1;
	maxX  = xu+(xu-xl)*(float)0.1;
	minY  = yl-(yu-yl)*(float)0.1;
	maxY  = yu+(yu-yl)*(float)0.1;
	}
	if (minY == maxY) 
	{
		maxY=minY+minres*(fabs(maxY)+1);
		minY=minY-minres*(fabs(maxY)+1);
	}
	if (minX == maxX) 
	{
		maxX=minX+minres;
		minX=minX-minres;
	}
	fminX = minX;
	fminY = minY;
	fmaxX = maxX;
	fmaxY = maxY;
	if(DataPoint[0][0]>DataPoint[0][ActualIndex-1])
		{
			float c;
			c		= minX;
			minX	= maxX;
			maxX	= c;

			fminX	= minX;
			fmaxX	= maxX;
		}
	Invalidate(TRUE);
}

void DataView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	IsFirstMove = FALSE;
	BOOL found  = FALSE;
	int i;
	CDC* pDC=GetDC();
	CRect rect;
	CRect Selectrect;
	GetClientRect(rect);
	i=0;

	if (SelectMax)
	{
		if (SelectBase)
		{
			SelectBase = FALSE;
			pDC->MoveTo(0,point.y);
			pDC->LineTo(1024,point.y);
			if(fitfunc==0||fitfunc==1||fitfunc==2||fitfunc==12||fitfunc==13||fitfunc==14)
			{
				if(!equalspaced)
				{
					pDC->TextOut(10,rect.bottom-25,"Doubleclick to all maxima");
				}
				else
				{
					pDC->TextOut(10,rect.bottom-25,"Doubleclick to leftmost and rightmost maximum");
				}
			}
			if(fitfunc==3)
			{
				pDC->TextOut(10,rect.bottom-25,"Doubleclick to start of decay");
			}
			if(fitfunc==5||fitfunc==6||fitfunc==7||fitfunc==8||fitfunc==9||fitfunc==10)
				pDC->TextOut(10,rect.bottom-25,"Doubleclick to leftmost transition");
			if (!SelectHfs)
			{
				FitPar[SelectNum].x = InvConvertX(point.x,rect,minX,maxX);
				FitPar[SelectNum].y = InvConvertY(point.y,rect,minY,maxY);
			}
			else
			{
				FitPar[14].x = InvConvertX(point.x,rect,minX,maxX);
				FitPar[14].y = InvConvertY(point.y,rect,minY,maxY);
			}
		}
		else
		{
			pDC->MoveTo(point.x,0);
			pDC->LineTo(point.x,1024);
			SelectNum--;
			if (!SelectHfs)
			{
				FitPar[SelectNum].x = InvConvertX(point.x,rect,minX,maxX);
				FitPar[SelectNum].y = InvConvertY(point.y,rect,minY,maxY);
			}
			else
			{
				FitPar[15].x = InvConvertX(point.x,rect,minX,maxX);
				FitPar[15].y = InvConvertY(point.y,rect,minY,maxY);
				SelectHfs = FALSE;
			}
			if(SelectNum==0||fitfunc == 3)
			{
				SelectMax = FALSE;
				if(COMMONWIDTH&&(fitfunc==0||fitfunc==1||fitfunc==2||fitfunc==12||fitfunc==13||fitfunc==14))fitfunc = fitfunc + 20;
				pMinuit = new CMinuit(this,
									WindowTitle,
									fitfunc,
									sign,
									em_start,
									eb_start,
									equalspaced,
									numofpeaks,
									FitPar);
				pMinuit->SetFocus();
			}
		}
	}
	else
	{
		if(!DataSelect)
		{
			DataSelect		= TRUE;
			SelectOfs		= 0;
			SelectPoint		= point;
			IsFirstMove     = TRUE;
			OldPos			= -1;
			DataSelecting(SelectPoint);
		}
		else 
		{
			DataSelect		= FALSE;
			Invalidate(TRUE);
		}
	}
	CView::OnLButtonDblClk(nFlags, point);
}

void DataView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	if(!DataSelect)
	{
		CRect rect;
		CDC* pDC=GetDC();

		rect.left = point.x;
		rect.top  = point.y;
		rect.left = point.x+2;
		rect.top  = point.y+2;

		MouseMoveStart  = rect; 
		UserIsSelecting = TRUE;
		IsFirstMove     = TRUE;
	}
	CView::OnLButtonDown(nFlags, point);
}

void DataView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    
	if (UserIsSelecting)
	{
		CDC* pDC=GetDC();
		CRect rect;
		rect.left   = MouseMoveStart.left;
		rect.top    = MouseMoveStart.top;
		
		rect.right  = rect.left+2;
		rect.bottom = rect.top+2;

		if (point.x>rect.left) rect.right  = point.x;
		if (point.y>rect.top)  rect.bottom = point.y;
		if (IsFirstMove)pDC->DrawDragRect(&rect,DragRectSize,NULL,DragRectSize,NULL,NULL );
		if (!IsFirstMove)
		{
			pDC->DrawDragRect(&rect,DragRectSize,&MouseMoveStart,DragRectSize,NULL,NULL );
		}
		IsFirstMove=FALSE;
		MouseMoveStart = rect;
	}
	if(DataSelect)
	{
		SelectOfs		= 0;
		SelectPoint		= point;
		DataSelecting(SelectPoint);
	}
	CView::OnMouseMove(nFlags, point);
}

void DataView::DataSelecting(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    
		int i,i_off;
		double AcceptRange,x,y;
		CDC* pDC=GetDC();
		CRect rect;
		CRect Selectrect;
		GetClientRect(rect);
		i=0;
		BOOL found=FALSE;
		while((i<=ActualIndex)&!(found))
		{
			x=InvConvertX(point.x,rect,minX,maxX);
			AcceptRange = fabs((DataPoint[0][1]-DataPoint[0][0])/2);
			if ((fabs(DataPoint[0][i] - x) < AcceptRange))
				{
					if((i + SelectOfs) < (int)0)
					{
						SelectOfs = -i;
					}
					if((i + SelectOfs) >= ActualIndex)
					{
						SelectOfs = ActualIndex-1-i;
					}

					i_off = i + SelectOfs;

					x = ConvertX(DataPoint[0][i_off],rect,minX,maxX);
					y = ConvertY(DataPoint[1][i_off],rect,minY,maxY);
					found=TRUE;
					CString TheDataX;
					TheDataX.Format("x = %.7g         ",DataPoint[0][i_off]);
					CString TheDataY;
					TheDataY.Format("y = %.4g  +/-  %.4g     ",DataPoint[1][i_off],DataPoint[2][i_off]);
					pDC->TextOut(100,rect.top+25,TheDataX);
					pDC->TextOut(100,rect.top+45,TheDataY);
					Selectrect.left   = (long)(x-SelectSize);
					Selectrect.top    = (long)(y-SelectSize);
					
					Selectrect.right  = (long)(x+SelectSize);
					Selectrect.bottom = (long)(y+SelectSize);
					DrawXORLine(pDC,(int)x, 800);
	
					SelectOldrect = Selectrect;
				}
			i++;
		}
}

void DataView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	float minXt,minYt,maxXt,maxYt;
	CRect rect;
	if ((UserIsSelecting)&!(IsFirstMove)&&
		(point.x>MouseMoveStart.left)&&
		(point.y>MouseMoveStart.top))
	{
	GetClientRect(rect);
	minXt = InvConvertX(MouseMoveStart.left,rect,minX,maxX);
	maxYt = InvConvertY(MouseMoveStart.top,rect,minY,maxY);
	maxXt = InvConvertX(MouseMoveStart.right,rect,minX,maxX);
	minYt = InvConvertY(MouseMoveStart.bottom,rect,minY,maxY);
	
	if(!LOGVIEW)
	{
		minX=minXt;
		minY=minYt;
		maxX=maxXt;
		maxY=maxYt;
	}
	else
	{
		minX=minXt;
		minY=log10(minYt);
		maxX=maxXt;
		maxY=log10(maxYt);
	}
	Invalidate(TRUE);
	}

	UserIsSelecting = FALSE;
	IsFirstMove     = TRUE;
	
	CView::OnLButtonUp(nFlags, point);
}

void DataView::OnPopupResizetofullscale() 
{
	// TODO: Add your command handler code here
	minX=fminX;
	maxX=fmaxX;
	minY=fminY;
	maxY=fmaxY;
	Invalidate(TRUE);
}

void DataView::OnPopupShowproperties() 
{
	if(pActionObject!=NULL)
	{
		pActionObject->DoConfigureAction();
		pActionObject->DoDoubleClickAction();
	}
}

void DataView::Reset()
{
	ActualIndex         = (int)0;
	LastShownDataPoint  = (float)0;
	minX                = (float)1e10;
	maxX                = (float)-1e10;
	
	minY                = (float)1e10;
	maxY                = (float)-1e10;

	fminX				= minX;
	fminY				= minY;
	fmaxX				= maxX;
	fmaxY				= maxY;

	LastShownDataPoint  = (float)1e10;
	DrawSingleDataPoint = FALSE;
	IsActivated         = TRUE;
	UserIsSelecting		= FALSE;
	IsFirstMove			= FALSE;
	GetParent()->BringWindowToTop();
	SetFocus();
}

void DataView::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (SCALER)
	{
		pActionObject->pDataView = NULL;
		//POSITION pos;
		//pos = pActionObject->pDocument->pChildWin.Find(this);
		//if (pos!=NULL)pActionObject->pDocument->pChildWin.RemoveAt(pos);
	}
	CView::PostNcDestroy();
}

void DataView::OnPopupLoadspec() 
{
	// TODO: Add your command handler code here
	BOOL endmarker;
	CString FileName;
	CString File;
	CString FileLine;
	int narg;
	int i;
	static char BASED_CODE szFilter[] = "Data Files (*.dat)|*.dat|All Files (*.*)|*.*||";
	CFileDialog* pLoadFile = new CFileDialog(TRUE,NULL,NULL,0,&szFilter[0],NULL);
	if (IDOK==pLoadFile->DoModal())
	{
		FileName = pLoadFile->GetPathName();
		File = pLoadFile->GetFileName();
		delete pLoadFile;
		CStdioFile* pFile = new CStdioFile(FileName,CFile::modeRead);
		i=0;
		do
		{
			endmarker = pFile->ReadString(FileLine);
			if (endmarker)
			{
				narg = sscanf((LPCSTR)FileLine,"%g %g %g",&DataPoint[0][i],&DataPoint[1][i],&DataPoint[2][i]);
				if (DataPoint[2][i]<=0)DataPoint[2][i]=(float)1;
				if (narg==3) ActualIndex = i+1;
			}
			++i;
		}
		while (endmarker);
		pFile->Close();
		delete pFile;
		SetTitle(File);
		SetWindowTitle(FileName);
   		SetXTitle("x","");
		SetYTitle("y","");
		if(SCALER)
		{
			pActionObject->pDataView = NULL;
			SCALER = FALSE;
		}
		RedrawNow();
	}
}

void DataView::OnPopupSavespec() 
{
	// TODO: Add your command handler code here
	CString FileName;
	CString DataString;
	static char BASED_CODE szFilter[] = "Data Files (*.dat)|*.dat|All Files (*.*)|*.*||";
	CFileDialog* pSaveFile = new CFileDialog(FALSE,"dat",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,&szFilter[0],NULL);
	if(IDOK==pSaveFile->DoModal())
	{
		FileName = pSaveFile->GetPathName();
		delete pSaveFile;
		CStdioFile* pFile = new CStdioFile(FileName,CFile::modeWrite | CFile::modeCreate);
		long i;
		for (i=0;i<ActualIndex;i++)
		{
			DataString.Format("%.8g %.8g %.8g\n",DataPoint[0][i],DataPoint[1][i],DataPoint[2][i]);
			pFile->WriteString(DataString);
		}
		pFile->Close();
		delete pFile;
	}
}

void DataView::OnPopupMinuitfitsession() 
{
	// TODO: Add your command handler code here

	if (!FIT)
	{

		CMinuitDlg* pdlg = new CMinuitDlg;
		pdlg->DoModal();
		numofpeaks		= pdlg->m_numofpeaks;
		if(pdlg->m_negativepeaks) sign = -1;
			else sign = +1;
		equalspaced		= pdlg->m_equalspaced;
		if(pdlg->m_baseoffset) 
		{
			eb_start=1;
		}
		else 
		{
			eb_start=0;
		}
		if(pdlg->m_linoffset) 
		{
			em_start=1;
		}
		else 
		{
			em_start=0;
		}
	
		if (pdlg->m_gauss)		fitfunc = 0;
		if (pdlg->m_lorentz)	fitfunc = 1;
		if (pdlg->m_voigt)		fitfunc = 2;
		if (pdlg->m_decay)		fitfunc = 3;
		if (pdlg->m_linear)		fitfunc = 4;
		if (pdlg->m_user)		fitfunc = 11;
		if (pdlg->m_gauss_exp)	fitfunc = 12;
		if (pdlg->m_lorentz_exp)fitfunc = 13;
		if (pdlg->m_voigt_exp)	fitfunc = 14;
		if (pdlg->m_cos_exp)	fitfunc = 100;
		if (pdlg->m_mx_bloch)	fitfunc = 101;
		if (pdlg->m_my_bloch)	fitfunc = 102;
		if (pdlg->m_mz_bloch)	fitfunc = 103;
		if (pdlg->m_mxyz_bloch)	fitfunc = 104;
		if (pdlg->m_commonwidth)COMMONWIDTH = TRUE;
			else				COMMONWIDTH = FALSE;
		if (pdlg->m_hfsfit)	
		{
			double V_FLUKE	= 0;
			double V_ISOLDE	= 0;
			BOOL Found1 = FALSE;
			BOOL Found2 = FALSE;

			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	
			CHfsFitDlg* phfsdlg = new CHfsFitDlg;
			
			CString TheData;

			TheData = pApp->GetProfileString("HFS-Fit","m_a_g");
			phfsdlg->m_a_g			= atof(TheData);
			TheData = pApp->GetProfileString("HFS-Fit","m_b_g");
			phfsdlg->m_b_g			= atof(TheData); 
			TheData = pApp->GetProfileString("HFS-Fit","m_a_a");
			phfsdlg->m_a_a			= atof(TheData);
			TheData = pApp->GetProfileString("HFS-Fit","m_b_a");
			phfsdlg->m_b_a			= atof(TheData); 
			TheData = pApp->GetProfileString("HFS-Fit","m_wavelength");
			phfsdlg->m_wavelength	= atof(TheData);
			TheData = pApp->GetProfileString("HFS-Fit","m_i");
			phfsdlg->m_i			= atof(TheData); 
			TheData = pApp->GetProfileString("HFS-Fit","m_mass");
			phfsdlg->m_mass			= atof(TheData);
			TheData = pApp->GetProfileString("HFS-Fit","m_j_g");
			phfsdlg->m_j_g			= atof(TheData); 
			TheData = pApp->GetProfileString("HFS-Fit","m_j_a");
			phfsdlg->m_j_a			= atof(TheData); 
			TheData = pApp->GetProfileString("HFS-Fit","m_isv");
			phfsdlg->m_isv			= atof(TheData); 
	
			TheData = pApp->GetProfileString("HFS-Fit","m_accelvoltage");
			phfsdlg->m_accelvoltage = atof(TheData); 

			phfsdlg->DoModal();

			if(phfsdlg->m_gauss)		fitfunc = 5;
			if(phfsdlg->m_lorentz)		fitfunc = 6;
			if(phfsdlg->m_voigt)		fitfunc = 7;
			if(phfsdlg->m_gaussexp)		fitfunc = 8;
			if(phfsdlg->m_lorentzexp)	fitfunc = 9;
			if(phfsdlg->m_voigtexp)		fitfunc = 10;

			FitPar[0].y		= phfsdlg->m_a_g;
			FitPar[1].y		= phfsdlg->m_b_g;
			FitPar[2].y		= phfsdlg->m_a_a;
			FitPar[3].y		= phfsdlg->m_b_a;

			FitPar[4].y		= phfsdlg->m_wavelength;
			FitPar[5].y		= phfsdlg->m_i;
			FitPar[6].y		= phfsdlg->m_mass;
			FitPar[7].y		= phfsdlg->m_j_g;
			FitPar[8].y		= phfsdlg->m_j_a;
			FitPar[9].y		= phfsdlg->m_isv;
			FitPar[10].y	= phfsdlg->m_accelvoltage;
			FitPar[11].y	= phfsdlg->m_mass;
			FitPar[12].y	= 1;
			FitPar[13].y	= phfsdlg->m_kepco;
			
			TheData.Format("%.10g",phfsdlg->m_a_g);
			pApp->WriteProfileString("HFS-Fit","m_a_g",TheData);

			TheData.Format("%.10g",phfsdlg->m_b_g);
			pApp->WriteProfileString("HFS-Fit","m_b_g",TheData);

			TheData.Format("%.10g",phfsdlg->m_a_a);
			pApp->WriteProfileString("HFS-Fit","m_a_a",TheData);

			TheData.Format("%.10g",phfsdlg->m_b_a);
			pApp->WriteProfileString("HFS-Fit","m_b_a",TheData);

			TheData.Format("%.10g",phfsdlg->m_wavelength);
			pApp->WriteProfileString("HFS-Fit","m_wavelength",TheData);

			TheData.Format("%.10g",phfsdlg->m_i);
			pApp->WriteProfileString("HFS-Fit","m_i",TheData);

			TheData.Format("%.10g",phfsdlg->m_mass);
			pApp->WriteProfileString("HFS-Fit","m_mass",TheData);

			TheData.Format("%.10g",phfsdlg->m_j_g);
			pApp->WriteProfileString("HFS-Fit","m_j_g",TheData);

			TheData.Format("%.10g",phfsdlg->m_j_a);
			pApp->WriteProfileString("HFS-Fit","m_j_a",TheData);

			TheData.Format("%.10g",phfsdlg->m_isv);
			pApp->WriteProfileString("HFS-Fit","m_isv",TheData);

			TheData.Format("%.10g",phfsdlg->m_accelvoltage);
			pApp->WriteProfileString("HFS-Fit","m_accelvoltage",TheData);

			SelectHfs	= TRUE;
			numofpeaks	= 1;
			delete phfsdlg;
		}
		delete pdlg;
		if (!equalspaced)
		{
			if (fitfunc == 4||
				fitfunc == 11||
				fitfunc >= 100)
			{
				if(COMMONWIDTH&&(fitfunc==0||fitfunc==1||fitfunc==2||fitfunc==12||fitfunc==13||fitfunc==14))fitfunc = fitfunc + 20;
				pMinuit = new CMinuit(this,
									WindowTitle,
									fitfunc,
									sign,
									em_start,
									eb_start,
									equalspaced,
									numofpeaks,
									FitPar);
				pMinuit->SetFocus();
			}
			else
			{
				CDC* pDC = GetDC();
				CRect rect;
				GetClientRect(rect);
				pDC->TextOut(10,rect.bottom-25,"Doubleclick to baseline");
				SelectMax  = TRUE;
				SelectBase = TRUE;
				SelectNum = numofpeaks;
			}
		}
		else //equalspaced
		{
			CDC* pDC = GetDC();
			CRect rect;
			GetClientRect(rect);
			pDC->TextOut(10,rect.bottom-25,"Doubleclick to baseline");
			SelectMax  = TRUE;
			SelectBase = TRUE;
			SelectNum = 2;
		}
	}
	
	else
	{
		pMinuit->GetParent()->BringWindowToTop();
		pMinuit->SetFocus();
	}
}

afx_msg LRESULT DataView::OnMinuitKilled(WPARAM wparam,LPARAM pWin)
{
	pMinuit=NULL;
	return 0;
}

void DataView::OnDestroy() 
{
	CView::OnDestroy();
	if(FIT)pMinuit->KillMe();
}

BOOL DataView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	SetCursor(curs);
	/*return CView::OnSetCursor(pWnd, nHitTest, message);*/
	return 0;
}


void DataView::OnUpdateEditCopyCut(CCmdUI* pCmdUI)
{
	// CG: This line was added by the Clipboard Assistant component
	pCmdUI->Enable(IsSelected());
}

void DataView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
	// CG: This line was added by the Clipboard Assistant component
	pCmdUI->Enable(IsClipboardFormatAvailable(m_nClipboardFormat));
}

void DataView::OnEditCopy()
{
	// CG: This block was added by the Clipboard Assistant component
	{
		CMetaFileDC* pMetaDC = new CMetaFileDC;
		pMetaDC->Create(NULL);
		OnDraw(pMetaDC);
		HMETAFILE MetaFile = pMetaDC->Close();

		if (OpenClipboard())
		{
			::SetClipboardData(m_nClipboardFormat, MetaFile);
			CloseClipboard();
		}
		else
			AfxMessageBox(CG_IDS_CANNOT_OPEN_CLIPBOARD);
	}
}

void DataView::OnEditCut()
{
	// CG: This block was added by the Clipboard Assistant component

}

void DataView::OnEditPaste()
{
	// CG: This block was added by the Clipboard Assistant component
	{
		if (OpenClipboard())
		{
			HANDLE hData = ::GetClipboardData(m_nClipboardFormat);
			if (hData != NULL)
			{
				CDC* pDC = GetDC();
				pDC->PlayMetaFile((HMETAFILE)hData);

			}
			else
				AfxMessageBox(CG_IDS_CANNOT_GET_CLIPBOARD_DATA);
			CloseClipboard();
		}
		else
			AfxMessageBox(CG_IDS_CANNOT_OPEN_CLIPBOARD);
	}

}

BOOL DataView::IsSelected()
{
	// CG: This function was added by the Clipboard Assistant component

	// TODO: Add/Replace code to selectively enable and disable the copy 
	//       and cut menu items. Return TRUE when one or more items are
	//       selected, and return FALSE when no items are selected.

	return TRUE;
}

void DataView::DoCutCopyPaste(CArchive &ar, BOOL bCut)
{
	// CG: This function was added by the Clipboard Assistant component

	// TODO: Add/Replace code to copy the currently selected data to or
	//       from the clipboard, via the CArchive object. Note: 
	//       ar.IsStoring() set to TRUE indicates a copy or cut operation.

	//GetDocument()->Serialize(ar);

	if (bCut)
	{
		ASSERT(ar.IsStoring()); // no cutting on paste operation

		// TODO: Add/Replace code to delete the currently selected items.

		//GetDocument()->DeleteContents();
	}

	// updated view and flag as modified if cut or paste operation
	if (!(ar.IsStoring() && !bCut))
	{
		pActionObject->pDocument->SetModifiedFlag(TRUE);
		pActionObject->pDocument->UpdateAllViews(NULL,0,NULL);
	}
}

BOOL DataView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{	
		switch (pMsg->wParam)
		{
			
			case CHAR_N:
			{
				data_type = 2;
				Invalidate(TRUE);
				return 1;
			}
			case CHAR_R:
			{
				RedrawNow();
				return 1;
			}
			case CHAR_S:
			{
				data_type = 0;
				Invalidate(TRUE);
				return 1;
			}
			case CHAR_P:
			{
				CView::OnFilePrint();
				return 1;
			}
			case CHAR_Z:
			{
				if(!STARTATZERO)STARTATZERO = TRUE;
				else STARTATZERO = FALSE;
	
				RedrawNow();
				return 1;
			}
			case CHAR_L:
			{
				if(!LOGVIEW)LOGVIEW = TRUE;
				else LOGVIEW = FALSE;

				RedrawNow();
				return 1;
			}
			case CHAR_E:
			{
				GetParent()->DestroyWindow();
				return 1;
			}
			case VK_RETURN:
			{
				DataSelect		= FALSE;
				OldPos			= -1;
				SelectOfs	= 0;
				Invalidate(TRUE);
				return 1;
			}
			case VK_LEFT:
			{
				SelectOfs--; 
				DataSelecting(SelectPoint);
				return 1;
			}
			case VK_RIGHT:
			{
				SelectOfs++; 
				DataSelecting(SelectPoint);
				return 1;
			}
			case VK_F5:
			{
				SelectOfs = SelectOfs - 10; 
				DataSelecting(SelectPoint);
				return 1;
			}
			case VK_F6:
			{
				SelectOfs--;  
				DataSelecting(SelectPoint);
				return 1;
			}
			case VK_F7:
			{
				SelectOfs++;  
				DataSelecting(SelectPoint);
				return 1;
			}
			case VK_F8:
			{
				SelectOfs = SelectOfs + 10;  
				DataSelecting(SelectPoint);
				return 1;
			}
			default: 
			{
				break;
			}
		}
	}
	return CView::PreTranslateMessage(pMsg);
}


void DataView::OnPopupSavewmf() 
{
	// TODO: Add your command handler code here
	CMetaFileDC* pMetaDC = new CMetaFileDC;
	pMetaDC->Create("plot.wmf");
	OnDraw(pMetaDC);
	HMETAFILE MetaFile = pMetaDC->Close();
	DeleteMetaFile(MetaFile);
}
