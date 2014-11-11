// MonitorView.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "MCP for NTDoc.h"
#include "ActionObjects.h"
#include "MonitorChild.h"
#include "MonitorView.h"
#include "TheDefines.h"
#include "DataReadout.h"
#include "Configure.h"

#include "PropShtAS.h"
#include "PropShtPM.h"

#include <math.h>
#define WM_MINUITKILLED	WM_USER + 302
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DimAll 300

const int   AcceptRange  = 4;


const float OverScale    = (float)1.1;

const float minres       = (float)1e-5;
const float epsilon      = (float)minres/10;
const float epsilondata  = (float)5e-4;

const float epsilonShow  = (float)1e-7;

const float AxisTextSize = (float)0.050*DimAll;
const float NumberSize   = (float)0.050*DimAll;

const float AxisLine     = (float)0.004*DimAll;
const float TickLine     = (float)0.003*DimAll;
const float DataLine     = (float)0.002*DimAll;

const float MainTitleX   = (float)0.5;
const float MainTitleY   = (float)0.1;

const float XTitleX      = (float)0.5;
const float XTitleY      = (float)0.98;

const float YTitleX      = (float)0.025;
const float YTitleY      = (float)0.5;

const float TickLabelsX  = (float)0.905;
const float TickLabelsY  = (float)0.125;

const float PointSize    = (float)0.008*DimAll;
const float ErrorBarSize = (float)0.005*DimAll;

const float DataxMin     = (float)0.13;
const float DatayMin     = (float)0.01;
const float DataxMax     = (float)0.995;
const float DatayMax     = (float)0.9;
const float TickLenX     = (float)0.015;
const float HalfTickLenX = (float)0.0075;
const float TickLenY     = (float)0.01;
const float HalfTickLenY = (float)0.005;

const UCHAR AxisRed		= (UCHAR)0;
const UCHAR AxisGreen	= (UCHAR)0;
const UCHAR AxisBlue	= (UCHAR)0;

const UCHAR SelectRed	= (UCHAR)200;
const UCHAR SelectGreen	= (UCHAR)0;
const UCHAR SelectBlue	= (UCHAR)0;

const UCHAR DataRed		= (UCHAR)0;
const UCHAR DataGreen	= (UCHAR)0;
const UCHAR DataBlue	= (UCHAR)200;
const UCHAR OldDataRed		= (UCHAR)190;
const UCHAR OldDataGreen	= (UCHAR)190;
const UCHAR OldDataBlue		= (UCHAR)190;

const UCHAR TextRed		= (UCHAR)0;
const UCHAR TextGreen	= (UCHAR)0;
const UCHAR TextBlue	= (UCHAR)0;

const int delta			= (int)5;
/////////////////////////////////////////////////////////////////////////////
// MonitorView

IMPLEMENT_DYNCREATE(MonitorView, CView)

//MonitorView* MonitorView::pDataView;

MonitorView::MonitorView()
{
	NumOfViews = 0;
	actual_monitor = 0;
	int i;
	pApp = (CMCPforNTApp*)AfxGetApp();
	
	data_type = 2;

	curs = AfxGetApp()->LoadStandardCursor(IDC_CROSS);

	CRect rect;
	for(i=0;i<MaxMonitors;i++)ActualIndex[i] = (int)0;

	DrawOnlyDataPoints	= FALSE;
	IsActivated         = FALSE;
	UserIsSelecting		= FALSE;
	IntOnly				= FALSE;

	rect.top            = 10;
	rect.left           = 400;
	rect.right          = 1000;
	rect.bottom         = 410;

	pDataChild = new MonitorChild(this);
	pDataChild->CMDIChildWnd::Create(NULL,
						"Scaler", 
						WS_CHILD|WS_VISIBLE|WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
						rect, 
						NULL, 
						NULL );

	this->Create(NULL,NULL,WS_CHILD|WS_VISIBLE|
	WS_CLIPCHILDREN|WS_CLIPSIBLINGS,rect,pDataChild,AFX_IDW_PANE_FIRST+1,NULL);
	IsActivated=TRUE;
	GetParentFrame()->MoveWindow(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);

	m_nClipboardFormat = ::RegisterClipboardFormat(_T("CF_METAFILEPICT"));

	CFrameWnd* pFrame = (CFrameWnd*)GetParent();
	pFrame->SetActiveView(this,TRUE);
}


MonitorView::~MonitorView()
{
	IsActivated=FALSE;
}


BEGIN_MESSAGE_MAP(MonitorView, CView)
	//{{AFX_MSG_MAP(MonitorView)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_POPUP_SHOWPROPERTIES, OnPopupShowproperties)
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	//ON_UPDATE_COMMAND_UI(ID_POPUP_SHOWPROPERTIES,IsScaler)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MonitorView drawing
BOOL MonitorView::MonitorFree()
{
	if (NumOfViews<MaxMonitors) return TRUE;
	else return FALSE;
}

void MonitorView::AddView(USHORT NoOfChannels,CString XTitle, CString XUnit,CString YTitle, CString YUnit,float Start,float Stop, ActionObject* pSpectrumObj,USHORT scans,BOOL showexpected)
{
	{
		pAction[NumOfViews] = pSpectrumObj;
		ShowExpected[NumOfViews] = showexpected;
		USHORT i;
		NumOfChannels = NoOfChannels;
		float Xmin;
		float Xmax;
		Xmin	= Start-(Stop-Start)*(float)0.1;
		Xmax	= Stop+(Stop-Start)*(float)0.1;
		XAxisTitle				= XTitle;		
		XAxisUnit				= XUnit;	
		YAxisTitle[NumOfViews]	= YTitle;
		YAxisUnit[NumOfViews]	= YUnit;

		minY[NumOfViews] = (float)0;
		maxY[NumOfViews] = (float)0;
		minX	= Xmin;
		maxX	= Xmax;
		StartX	= Start;
		StopX	= Stop;
		if(scans!=0)
		{
			for(i=0;i<NoOfChannels;i++)
			{
				EstimatedDataPoint[NumOfViews][0][i] = (float)Start+((float)Stop-(float)Start)*i/(NumOfChannels-1);
				EstimatedDataPoint[NumOfViews][1][i] = (float)pSpectrumObj->GetY(i); 
				EstimatedDataPoint[NumOfViews][2][i] = (float)pSpectrumObj->GetYErr(i);
				DataPoint[NumOfViews][1][i] = EstimatedDataPoint[NumOfViews][1][i]; 
				DataPoint[NumOfViews][2][i] = EstimatedDataPoint[NumOfViews][2][i];
			}
		}
		else
		{
			for(i=0;i<NoOfChannels;i++)
			{
				EstimatedDataPoint[NumOfViews][0][i] = (float)Start+((float)Stop-(float)Start)*i/(NumOfChannels-1);
				EstimatedDataPoint[NumOfViews][1][i] = (float)0; 
				EstimatedDataPoint[NumOfViews][2][i] = (float)0;
			}
		}
		NumOfViews++;
	}
}

void MonitorView::DeleteAllViews(void)
{
	USHORT i;
	NumOfViews		= 0;
	actual_monitor	= 0;
	NumScans		= 0;
	for(i=0;i<MaxMonitors;i++)ActualIndex[i]=0;
}

inline float MonitorView::GetStep(float Min,float Max)
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

inline void MonitorView::DrawBoundingRect(CDC* pDC,CRect rect)
{
	int i;
	float Scale;

	Scale = 1/(float)NumOfViews;
	CPen* pPen = new CPen();
	pPen->CreatePen(PS_SOLID,(int)(AxisLine),RGB(AxisRed,
													AxisGreen,
													AxisBlue));
	CPen* OldPen = pDC->SelectObject(pPen);

	pDC->MoveTo((int)(rect.right*DataxMin),(int)(rect.bottom*DatayMin));
	pDC->LineTo((int)(rect.right*DataxMin),(int)(rect.bottom*DatayMax));
	pDC->LineTo((int)(rect.right*DataxMax),(int)(rect.bottom*DatayMax));
	pDC->LineTo((int)(rect.right*DataxMax),(int)(rect.bottom*DatayMin));
	pDC->LineTo((int)(rect.right*DataxMin),(int)(rect.bottom*DatayMin));
	
	for(i=1;i<NumOfViews;i++)
	{
		int	ActualViewBottom = (int)(rect.bottom*(DatayMin+i*(DatayMax-DatayMin)/NumOfViews));
		pDC->MoveTo((int)(rect.right*DataxMin),ActualViewBottom);
		pDC->LineTo((int)(rect.right*DataxMax),ActualViewBottom);	
	}
	pDC->SelectObject(OldPen);
	delete pPen;
}

inline void MonitorView::DrawTitles(CDC* pDC,CRect rect)
{
	int Data=0;
	CString Title;

	LOGFONT logFont1;
    logFont1.lfHeight = (long)(AxisTextSize);
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
	pDC->SetTextColor(RGB(TextRed,
							TextGreen,
							TextBlue));
	pDC->TextOut((int)(XTitleX*rect.right),(int)(XTitleY*rect.bottom),(CString)XAxisTitle+"["+(CString)XAxisUnit+"]");

	pDC->SelectObject(OldFont);
	delete NewFont;

	LOGFONT logFont2;
    logFont2.lfHeight = (long)(AxisTextSize);
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
	
	for(Data=0;Data<NumOfViews;Data++)
	{
		int	ActualViewTop = (int)(rect.bottom*(DatayMin+Data*(DatayMax-DatayMin)/NumOfViews));
		int	ActualViewBottom = (int)(rect.bottom*(DatayMin+(Data+1)*(DatayMax-DatayMin)/NumOfViews));
		pDC->TextOut((int)(YTitleX*rect.right),(int)((ActualViewBottom+ActualViewTop)/2),YAxisTitle[Data]);
	}
	pDC->SelectObject(OldFont2);
	delete NewFont2;
}

inline int MonitorView::ConvertX(float Xin,CRect rect,float XMin,float XMax)
{
	int x;
	x=(int)(((Xin-XMin)*(DataxMax-DataxMin)*rect.right)/(XMax-XMin)+DataxMin*rect.right);
	return x;
}

inline int MonitorView::ConvertY(USHORT ActualView,float Yin,CRect rect,float YMin,float YMax)
{
	int y;
	float		DataYMax = (DatayMin+(ActualView+1)*(DatayMax-DatayMin)/NumOfViews);
	float		DataYMin = (DatayMin+(ActualView  )*(DatayMax-DatayMin)/NumOfViews);
	y=(int)(DataYMax*rect.bottom-((Yin-YMin)*(DataYMax-DataYMin)*rect.bottom)/(YMax-YMin));
	return y;
}

inline float MonitorView::InvConvertX(int x,CRect rect,float XMin,float XMax)
{
	float Xin;
	Xin=(x-DataxMin*rect.right)*(XMax-XMin)/((DataxMax-DataxMin)*rect.right)+XMin;
	return Xin;
}

inline float MonitorView::InvConvertY(USHORT ActualView,int y,CRect rect,float YMin,float YMax)
{
	float Yin;
	float		DataYMax = (DatayMin+(ActualView+1)*(DatayMax-DatayMin)/NumOfViews);
	float		DataYMin = (DatayMin+(ActualView  )*(DatayMax-DatayMin)/NumOfViews);
	Yin=YMin-(y-DataYMax*rect.bottom)*(YMax-YMin)/((DataYMax-DataYMin)*rect.bottom);
	return Yin;
}

inline int MonitorView::ConvertErr(USHORT ActualView,float Yin,CRect rect,float YMin,float YMax)
{
	int y;
	float		DataYMax = (DatayMin+(ActualView+1)*(DatayMax-DatayMin)/NumOfViews);
	float		DataYMin = (DatayMin+(ActualView  )*(DatayMax-DatayMin)/NumOfViews);
	y=(int)((Yin*(DataYMax-DataYMin)*rect.bottom)/(YMax-YMin));
	return y;
}

inline void MonitorView::DrawEstimatedDataPoint(CDC* pDC,CRect rect,USHORT index,int actual_type,USHORT Data)
{
	int i=0;

	int eSizeP,eSizeM;

	float		DataYMax = (DatayMin+(Data+1)*(DatayMax-DatayMin)/NumOfViews);
	float		DataYMin = (DatayMin+(Data)*(DatayMax-DatayMin)/NumOfViews);

	int pSize = (int)(PointSize);
	int dSize = (int)(ErrorBarSize);

	int xi = ConvertX(EstimatedDataPoint[Data][0][index],rect,minX,maxX);
	int yi = ConvertY(Data,EstimatedDataPoint[Data][1][index],rect,minY[Data],maxY[Data]);
	
	if ((xi >= (rect.right*DataxMin ))&&(xi <= (rect.right*DataxMax ))&&
	    (yi >= (rect.bottom*DatayMin))&&(yi <= (rect.bottom*DatayMax)))
	{
		if(actual_type == 0)
		{
			int eSize = ConvertErr(Data,EstimatedDataPoint[Data][2][index],rect,minY[Data],maxY[Data]);
			eSizeM=yi+eSize;
			eSizeP=yi-eSize;
	
			if (eSizeM > (int)(rect.bottom*DatayMax)) eSizeM=(int)(rect.bottom*DatayMax);
			if (eSizeP < (int)(rect.bottom*DatayMin)) eSizeP=(int)(rect.bottom*DatayMin);
	
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
			int xi_1 = ConvertX(EstimatedDataPoint[Data][0][index-1],rect,minX,maxX);
			int yi_1 = ConvertY(Data,EstimatedDataPoint[Data][1][index-1],rect,minY[Data],maxY[Data]);
			pDC->MoveTo(xi_1,yi_1);
			pDC->LineTo(xi,yi);
		}
		
		if(actual_type == 2)
		{
			int xmin	= ConvertX((float)StartX,rect,minX,maxX);
			int xmax	= ConvertX((float)StopX,rect,minX,maxX);
			int dx		= (xmax-xmin)/NumOfChannels;
			if(index == 0)
			{
				pDC->MoveTo(xi-dx/2,yi);
				pDC->LineTo(xi+dx/2,yi);
			}
			else
			{
				int xi_1 = ConvertX(EstimatedDataPoint[Data][0][index-1],rect,minX,maxX);
				int yi_1 = ConvertY(Data,EstimatedDataPoint[Data][1][index-1],rect,minY[Data],maxY[Data]);
				pDC->MoveTo(xi_1+dx/2,yi_1);
				pDC->LineTo(xi_1+dx/2,yi);
				pDC->LineTo(xi+dx/2,yi);
			}
		}
	}
}

inline void MonitorView::DrawDataPoint(CDC* pDC,CRect rect,USHORT index,int actual_type,USHORT Data)
{
	int i=0;

	int eSizeP,eSizeM;

	float		DataYMax = (DatayMin+(Data+1)*(DatayMax-DatayMin)/NumOfViews);
	float		DataYMin = (DatayMin+(Data)*(DatayMax-DatayMin)/NumOfViews);

	int pSize = (int)(PointSize);
	int dSize = (int)(ErrorBarSize);

	int xi = ConvertX(DataPoint[Data][0][index],rect,minX,maxX);
	int yi = ConvertY(Data,DataPoint[Data][1][index],rect,minY[Data],maxY[Data]);
	
	if ((xi >= (rect.right*DataxMin ))&&(xi <= (rect.right*DataxMax ))&&
	    (yi >= (rect.bottom*DatayMin))&&(yi <= (rect.bottom*DatayMax)))
	{
		if(actual_type == 0)
		{
			int eSize = ConvertErr(Data,DataPoint[Data][2][index],rect,minY[Data],maxY[Data]);
			eSizeM=yi+eSize;
			eSizeP=yi-eSize;
	
			if (eSizeM > (int)(rect.bottom*DatayMax)) eSizeM=(int)(rect.bottom*DatayMax);
			if (eSizeP < (int)(rect.bottom*DatayMin)) eSizeP=(int)(rect.bottom*DatayMin);
	
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
			int xi_1 = ConvertX(DataPoint[Data][0][index-1],rect,minX,maxX);
			int yi_1 = ConvertY(Data,DataPoint[Data][1][index-1],rect,minY[Data],maxY[Data]);
			pDC->MoveTo(xi_1,yi_1);
			pDC->LineTo(xi,yi);
		}
		
		if(actual_type == 2)
		{
			int xmin	= ConvertX((float)StartX,rect,minX,maxX);
			int xmax	= ConvertX((float)StopX,rect,minX,maxX);
			int dx		= (xmax-xmin)/NumOfChannels;
			if(index == 0)
			{
				pDC->MoveTo(xi-dx/2,yi);
				pDC->LineTo(xi+dx/2,yi);
			}
			else
			{
				int xi_1 = ConvertX(DataPoint[Data][0][index-1],rect,minX,maxX);
				int yi_1 = ConvertY(Data,DataPoint[Data][1][index-1],rect,minY[Data],maxY[Data]);
				pDC->MoveTo(xi_1+dx/2,yi_1);
				pDC->LineTo(xi_1+dx/2,yi);
				pDC->LineTo(xi+dx/2,yi);
			}
		}
	}
}

inline void MonitorView::DrawSelectedDataPoint(CDC* pDC,CRect rect,float x,float y,float err,float XMin,float XMax,float YMin,float YMax,USHORT Data)
{


	int eSizeP,eSizeM;
	CPen* pPen = new CPen();
	pPen->CreatePen(PS_SOLID,(int)(DataLine),RGB(SelectRed,
													SelectGreen,
													SelectBlue));
	CPen* OldPen = pDC->SelectObject(pPen);
	int pSize = (int)(PointSize);
	int dSize = (int)(ErrorBarSize);

	int xi = ConvertX(x,rect,XMin,XMax);
	int yi = ConvertY(Data,y,rect,YMin,YMax);
	if ((xi >= (rect.right*DataxMin ))&&(xi <= (rect.right*DataxMax ))&&
	    (yi >= (rect.bottom*DatayMin))&&(yi <= (rect.bottom*DatayMax)))
	{
		int eSize = ConvertErr(Data,err,rect,YMin,YMax);
		eSizeM=yi+eSize;
		eSizeP=yi-eSize;
		
		if (eSizeM > (int)(rect.bottom*DatayMax)) eSizeM=(int)(rect.bottom*DatayMax);
		if (eSizeP < (int)(rect.bottom*DatayMin)) eSizeP=(int)(rect.bottom*DatayMin);
		
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


void MonitorView::DrawTheData(USHORT Data,CDC* pDC,CRect rect,float Xmin,float Xmax,float Ymin,float Ymax)
{
	CPen* pPen = new CPen();
	CBrush* pBrush = new CBrush();
	pPen->CreatePen(PS_SOLID,(int)(DataLine),RGB(DataRed,
													DataGreen,
													DataBlue));
	pBrush->CreateSolidBrush(RGB(255,255,255));
	CPen* OldPen = pDC->SelectObject(pPen);
	CBrush* OldBrush = pDC->SelectObject(pBrush);

	int i;
	int ActIndex;
	ActIndex=ActualIndex[Data];
	if (ActIndex==MAXPOINTS)ActIndex=MAXPOINTS-1;

	for (i=0;i<ActIndex;i++)
	{
		DrawDataPoint(pDC,rect,i,data_type,Data);
	}
	pDC->SelectObject(OldPen);
	pDC->SelectObject(OldBrush);
	delete pPen;
	delete pBrush;
}


void MonitorView::DrawTheEstimatedData(USHORT Data,CDC* pDC,CRect rect,float Xmin,float Xmax,float Ymin,float Ymax)
{
	USHORT i;
	if(ShowExpected[Data])
	{
		CPen* pPen2 = new CPen();
		CBrush* pBrush2 = new CBrush();
		pPen2->CreatePen(PS_SOLID,(int)(DataLine),RGB(OldDataRed,
														OldDataGreen,
														OldDataBlue));
		pBrush2->CreateSolidBrush(RGB(255,255,255));
		CPen* OldPen2 = pDC->SelectObject(pPen2);
		CBrush* OldBrush2 = pDC->SelectObject(pBrush2);
		for (i=0;i<NumOfChannels;i++)
		{
			DrawEstimatedDataPoint(pDC,rect,i,data_type,Data);
		}
		pDC->SelectObject(OldPen2);
		pDC->SelectObject(OldBrush2);
		delete pPen2;
		delete pBrush2;
	}
}


inline void MonitorView::DrawXTicks(CDC *pDC,CRect rect,float XMin,float XMax)
{
	int dim=rect.right;
	if (dim>rect.bottom)dim=rect.bottom;

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
	pDC->SetTextColor(RGB(TextRed,
							TextGreen,
							TextBlue));
	CPen* pPen = new CPen();
	pPen->CreatePen(PS_SOLID,(int)(TickLine),RGB(AxisRed,
													AxisGreen,
													AxisBlue));
	CPen* OldPen = pDC->SelectObject(pPen);

	LOGFONT logFont2;
    logFont2.lfHeight = (long)(NumberSize);
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
		if ((x>=DataxMin*rect.right)&&(x<=DataxMax*rect.right))
			{
				pDC->MoveTo(x,(int)(DatayMin*rect.bottom));
				pDC->LineTo(x,(int)((DatayMin+TickLenX)*rect.bottom));
				pDC->MoveTo(x,(int)(DatayMax*rect.bottom));
				pDC->LineTo(x,(int)((DatayMax-TickLenX)*rect.bottom));

				CString TickLabels;
				float PosXText = PosX+epsilonShow*PosX;
				if (abs(PosXText) < (XMax-XMin)*epsilondata)PosXText = (float)0;

				TickLabels.Format("%g",PosXText);
				pDC->TextOut(x,(int)(TickLabelsX*rect.bottom),TickLabels);
			}

		PosX=PosX+StepSize/2;
		if (fabs(PosX) < epsilon)PosX=(float)0;
		x=ConvertX(PosX,rect,XMin,XMax);

		if ((x>=DataxMin*rect.right)&&(x<=DataxMax*rect.right))
			{
				pDC->MoveTo(x,(int)(DatayMin*rect.bottom));
				pDC->LineTo(x,(int)((DatayMin+HalfTickLenX)*rect.bottom));
				pDC->MoveTo(x,(int)(DatayMax*rect.bottom));
				pDC->LineTo(x,(int)((DatayMax-HalfTickLenX)*rect.bottom));
			}
		PosX=PosX+StepSize/2;
		if (fabs(PosX) < epsilon)PosX=(float)0;
	}
	pDC->SelectObject(OldFont);
	pDC->SelectObject(OldPen);
	delete pPen;
	delete NewFont;
}

inline void MonitorView::DrawYTicks(USHORT ActualView,CDC *pDC,CRect rect,float YMin,float YMax)
{
	int i;
	float StepSize=GetStep(YMin,YMax);
	float Size=YMax-YMin;
	float Step=(float)0.1;
	float Offs;
	float PosY;
	CString TheChar;

	Offs=StepSize*(float)(long)(YMin/StepSize);
	PosY=Offs-StepSize;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(TextRed,
							TextGreen,
							TextBlue));
	CPen* pPen = new CPen();
	pPen->CreatePen(PS_SOLID,(int)(TickLine),RGB(AxisRed,
													AxisGreen,
													AxisBlue));
	CPen* OldPen = pDC->SelectObject(pPen);

	LOGFONT logFont2;
    logFont2.lfHeight = (long)(NumberSize);
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
	pDC->SetTextAlign(TA_RIGHT|TA_BOTTOM); 

	for (i=0;i<(Size/StepSize)+2;i++)
	{
		int y=ConvertY(ActualView,PosY,rect,YMin,YMax);
		if (fabs(PosY) < epsilon)PosY=(float)0;
		
		float DataYMax = (DatayMin+(ActualView+1)*(DatayMax-DatayMin)/NumOfViews);
		float DataYMin = (DatayMin+(ActualView  )*(DatayMax-DatayMin)/NumOfViews);
		
		if ((y>=(int)(DataYMin*rect.bottom+delta))&&(y<=(int)(DataYMax*rect.bottom-delta)))
			{
				pDC->MoveTo((int)(DataxMin*rect.right),y);
				pDC->LineTo((int)((DataxMin+TickLenY)*rect.right),y);
				pDC->MoveTo((int)(DataxMax*rect.right),y);
				pDC->LineTo((int)((DataxMax-TickLenY)*rect.right),y);

				CString TickLabels;
				float PosYText = PosY+epsilonShow*PosY;

				if(fabs(PosYText) < (YMax-YMin)*epsilondata)
				{
					PosYText = (float)0;
					TickLabels = "0";
				}
				else
				{
					int exponent=0;
					if(fabs(PosYText)<1)
					{
						if(fabs(PosYText)>0.001)
						{
							TickLabels.Format("%.6g",PosYText);
						}
						else
						{
							while(fabs(PosYText)<=0.99995)
							{
								PosYText=PosYText*10;
								exponent--;
							}
							TickLabels.Format("%.4ge%i",PosYText,exponent);
						}
					}
					else
					{
						if(fabs(PosYText)<9.9995e5)
						{
							TickLabels.Format("%.6g",PosYText);
						}
						else
						{
							while(fabs(PosYText)>=9.9995)
							{
								PosYText=PosYText/10;
								exponent++;
							}
							TickLabels.Format("%.4ge%i",PosYText,exponent);
						}
					}
				}
				pDC->TextOut((int)(TickLabelsY*rect.right),y+(int)(NumberSize/2),TickLabels);
			}
		PosY=PosY+StepSize/2;
		y=ConvertY(ActualView,PosY,rect,YMin,YMax);
		if ((y>=DataYMin*rect.bottom+delta)&&(y<=DataYMax*rect.bottom-delta))
			{
				pDC->MoveTo((int)(DataxMin*rect.right),y);
				pDC->LineTo((int)((DataxMin+HalfTickLenY)*rect.right),y);
				pDC->MoveTo((int)(DataxMax*rect.right),y);
				pDC->LineTo((int)((DataxMax-HalfTickLenY)*rect.right),y);
			}
		PosY=PosY+StepSize/2;
		if (fabs(PosY) < epsilon)PosY=(float)0;
	}
	pDC->SelectObject(OldFont);
	pDC->SelectObject(OldPen);
	delete pPen;
	delete NewFont;
}


void MonitorView::OnDraw(CDC* pDC)
{
	int i=0;

	CDocument* pDoc = GetDocument();
	CRect rect;
	GetClientRect(rect);
	USHORT Data = 0;

	// TODO: add draw code here
	if (pDC->IsPrinting())
	{
		rect.right=(long)(pDC->GetDeviceCaps(HORZRES)*0.95);
		rect.bottom=(long)(pDC->GetDeviceCaps(VERTRES)*0.95);
		if (rect.bottom > rect.right)rect.bottom=(long)(rect.right*0.75);
		CTime theTime;
		theTime=CTime::GetCurrentTime();
		CString s = theTime.Format( "%A, %B %d, %Y, %X");
		pDC->SetTextAlign(TA_RIGHT|TA_BOTTOM); 
		pDC->TextOut((int)(rect.right/0.95),(int)(rect.bottom/0.95),WindowTitle+"   "+s);
	}

	if (DrawOnlyDataPoints)
	{
		//DrawBoundingRect(pDC,rect);
		//DrawTitles(pDC,rect);
		//DrawXTicks(pDC,rect,minX,maxX);
		DrawOnlyDataPoints=FALSE;
		//for(Data=0;Data<NumOfViews;Data++)
		//	DrawYTicks(Data,pDC,rect,minY[Data],maxY[Data]);
		for(Data=0;Data<NumOfViews;Data++)
			DrawTheData(Data,pDC,rect,minX,maxX,minY[Data],maxY[Data]);
		
	}
	else
	{
		DrawBoundingRect(pDC,rect);
		DrawTitles(pDC,rect);
		DrawXTicks(pDC,rect,minX,maxX);
		for(Data=0;Data<NumOfViews;Data++)
			DrawYTicks(Data,pDC,rect,minY[Data],maxY[Data]);
		if(!FirstScan)for(Data=0;Data<NumOfViews;Data++)
			DrawTheEstimatedData(Data,pDC,rect,minX,maxX,minY[Data],maxY[Data]);
		for(Data=0;Data<NumOfViews;Data++)
			DrawTheData(Data,pDC,rect,minX,maxX,minY[Data],maxY[Data]);
	}
}

/////////////////////////////////////////////////////////////////////////////
// MonitorView diagnostics

#ifdef _DEBUG
void MonitorView::AssertValid() const
{
	CView::AssertValid();
}

void MonitorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// MonitorView message handlers

BOOL MonitorView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return DoPreparePrinting(pInfo);
}

void MonitorView::ResizeWin(ULONG x,ULONG y)
{
	if (IsActivated)MoveWindow(0,0,x,y,TRUE);
}

void MonitorView::SetWindowTitle(CString Title)
{
	GetParent()->SetWindowText(Title);
	WindowTitle=Title;
}

void MonitorView::AddDataPoint(USHORT channel,float y,float err)
{
	DataPoint[actual_monitor][0][channel]=(float)StartX+((float)StopX-(float)StartX)*channel/(NumOfChannels-1);
	DataPoint[actual_monitor][1][channel]=y;
	DataPoint[actual_monitor][2][channel]=err;
	ActualIndex[actual_monitor]=channel+1;
	actual_monitor++;
	if(actual_monitor>=NumOfViews)actual_monitor = 0;
}


void MonitorView::CalcNextScan(USHORT scan)
{
	USHORT i,j;
	for(i=0;i<MaxMonitors;i++)ActualIndex[i]=0;
	if(scan==1)
	{
		FirstScan = TRUE;
	}
	else
	{
		FirstScan = FALSE;
		for(j=0;j<NumOfViews;j++)
		{
			if(ShowExpected[j])
			{
				for(i=0;i<NumOfChannels;i++)
				{
					EstimatedDataPoint[j][1][i]=(float)pAction[j]->EstimateY(i,scan);
					EstimatedDataPoint[j][2][i]=(float)pAction[j]->EstimateYErr(i,scan);


					//EstimatedDataPoint[j][1][i]=DataPoint[j][1][i]*(scan)/(scan-1);
					//EstimatedDataPoint[j][2][i]=sqrt(fabs(EstimatedDataPoint[j][1][i]));
					
					DataPoint[j][1][i] = EstimatedDataPoint[j][1][i];
					DataPoint[j][2][i] = EstimatedDataPoint[j][2][i];
				}
			}
		}
	}
}

void MonitorView::RedrawNow(void)
{
	USHORT Data;
	int i;
	float yl,yu,ymin,ymax;
	BOOL keep = TRUE;

	for (Data=0;Data < NumOfViews;Data++)
	{
		if (ActualIndex[Data] >= 0)
		{
			yl=(float)1e30;
			yu=(float)-1e30;

			for (i=0;i<ActualIndex[Data];i++)
			{
				if((DataPoint[Data][1][i]-DataPoint[Data][2][i])<=yl)yl=DataPoint[Data][1][i]-DataPoint[Data][2][i];
				if((DataPoint[Data][1][i]+DataPoint[Data][2][i])>=yu)yu=DataPoint[Data][1][i]+DataPoint[Data][2][i];
			}
			ymin  = yl-(yu-yl)*(float)0.1;
			ymax  = yu+(yu-yl)*(float)0.1;
		}

		if (ymin == ymax) 
		{
			ymax=ymin+minres*(fabs(ymax)+1);
			ymin=ymin-minres*(fabs(ymax)+1);
		}

		if (!((ymax<=maxY[Data])&&ymin>=minY[Data]))
		{
			minY[Data]  = (float)(ymin-(ymax-ymin)*0.15);
			maxY[Data]  = (float)(ymax+(ymax-ymin)*0.15);
			keep = FALSE;
		}
	}

	if(keep==TRUE)
	{
		DrawOnlyDataPoints = TRUE;
		Invalidate(FALSE);
	}
	else
	{
		Invalidate(TRUE);
	}
}

void MonitorView::ForceRedraw(void)
{
	USHORT Data;
	int i;
	float yl,yu,ymin,ymax;
	if(!FirstScan)
	{
		for (Data=0;Data < NumOfViews;Data++)
		{
			if (ActualIndex[Data] >= 0)
			{
				yl=(float)1e30;
				yu=(float)-1e30;

				for (i=0;i<NumOfChannels;i++)
				{
					if((DataPoint[Data][1][i]-DataPoint[Data][2][i])<=yl)yl=DataPoint[Data][1][i]-DataPoint[Data][2][i];
					if((DataPoint[Data][1][i]+DataPoint[Data][2][i])>=yu)yu=DataPoint[Data][1][i]+DataPoint[Data][2][i];
				}
				ymin  = yl-(yu-yl)*(float)0.1;
				ymax  = yu+(yu-yl)*(float)0.1;
			}

			if (ymin == ymax) 
			{
				ymax=ymin+minres*(fabs(ymax)+1);
				ymin=ymin-minres*(fabs(ymax)+1);
			}

			if (!((ymax<=maxY[Data])&&ymin>=minY[Data]))
			{
				minY[Data]  = (float)(ymin-(ymax-ymin)*0.15);
				maxY[Data]  = (float)(ymax+(ymax-ymin)*0.15);
			}
		}
	}
	DrawOnlyDataPoints = FALSE;
	Invalidate(TRUE);
}


void MonitorView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	BOOL found  = FALSE;
	int i,x,y,err;
	USHORT Data = 0;
	CDC* pDC=GetDC();
	CRect rect;
	GetClientRect(rect);
	for(Data=0;Data<NumOfViews;Data++)
	{
		i=0;
		while((i<=ActualIndex[Data])&!(found))
		{
			x=ConvertX(DataPoint[Data][0][i],rect,minX,maxX);
			y=ConvertY(Data,DataPoint[Data][1][i],rect,minY[Data],maxY[Data]);
			err=ConvertErr(Data,DataPoint[Data][2][i],rect,minY[Data],maxY[Data]);
			if ((abs(point.x - x) < AcceptRange)&&
				(abs(point.y - y) < AcceptRange+err))
				{
					found=TRUE;
					DrawSelectedDataPoint(pDC,rect,
										DataPoint[Data][0][i],
										DataPoint[Data][1][i],
										DataPoint[Data][2][i],
										minX,
										maxX,
										minY[Data],
										maxY[Data],Data);
				
					CString TheDataX;
					TheDataX.Format("%g",DataPoint[Data][0][i]);
					CString TheDataY;
					TheDataY.Format("%g",DataPoint[Data][1][i]);
					CString TheDataErr;
					TheDataErr.Format("%g",DataPoint[Data][2][i]);
					CDataReadout* pdlg = new CDataReadout;
					pdlg->m_textx = XAxisTitle;
					pdlg->m_texty = YAxisTitle[Data];
					pdlg->m_texterr ="Error";
					pdlg->m_numx = TheDataX+" "+XAxisUnit;
					pdlg->m_numy = TheDataY;
					pdlg->m_numerr = TheDataErr;
					pdlg->DoModal();
					delete pdlg;
					int actual_type = data_type;
					DrawDataPoint(pDC,rect,i,actual_type,Data);
				}
			i++;
		}
	}
	if (!found)Beep(500,100); 
	CView::OnLButtonDblClk(nFlags, point);
}

void MonitorView::OnPopupShowproperties() 
{
	// TODO: Add your command handler code here
	USHORT Data = 0;
	
	CConfigure* pdlg = new CConfigure;
	pdlg->m_points = FALSE;
	pdlg->m_lines  = FALSE;
	pdlg->m_stairs = FALSE;
	if(data_type == 0) pdlg->m_points = TRUE;
	if(data_type == 1) pdlg->m_lines  = TRUE;
	if(data_type == 2) pdlg->m_stairs = TRUE;
		
	if(IDOK==pdlg->DoModal())
	{
		if(pdlg->m_points)data_type = 0;
		if(pdlg->m_lines) data_type = 1;
		if(pdlg->m_stairs)data_type = 2;
		Invalidate(TRUE);
	}
	delete pdlg;
}

void MonitorView::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class

	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	pApp->pMonitorView = NULL;
	CView::PostNcDestroy();
}

BOOL MonitorView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	SetCursor(curs);
	return 0;
}

BOOL MonitorView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{	
		switch (pMsg->wParam)
		{
			
			case CHAR_N:
			{
				data_type = 2;
				ForceRedraw();
				return 1;
			}
			case CHAR_S:
			{
				data_type = 0;
				ForceRedraw();
				return 1;
			}
			case CHAR_P:
			{
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
