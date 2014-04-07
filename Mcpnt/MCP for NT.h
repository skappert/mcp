// MCP for NT.h : main header file for the MCP FOR NT application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMCPforNTApp:
// See MCP for NT.cpp for the implementation of this class
//

class ActionObject;
class MonitorView;

class CMCPforNTApp : public CWinApp
{
public:
	double	GlobalActPosition;
	BOOL	MassesAvailable;
	CMapStringToString MassesMap;
	USHORT	OutRegister1;
	USHORT	OutRegister2;
	CString GPIBWrite;
	CString GPIBRead;
	int		WriteGpib;
	int		ReadGpib;

	BOOL	NetActive;
	BOOL	NetWrite;
	BOOL	CamacOn;
	MonitorView*	pMonitorView;
	int		FerddichSubadd;
	
	int		GelbeboxSlot;
	int		Gelbebox2Slot;
	int		AlexMotorSlot;
	int		GpibSlot;
	int		ListSlot;
	int		PremaSlot;
	int		PresetSlot;
	int		ProtonSlot;
	int		ProtonSubadd;
	int		Hp1Gpib;
	int		Hp2Gpib;
	int		Rs1Gpib;
	int		Rs2Gpib;
	int		FlukeGpib;
	int		TempGpib;
	int		FieldGpib;
	int		ScalerSlot;
	int		TriggerSubadd;
	int		HpSlot;
	int		HvSlot;
	int		FSlot;
	USHORT	PremaDelay;
	USHORT	FlukeDelay;
	USHORT	MassDelay;
	USHORT	GPIBDelay;

	CPtrList CopyActionList;
	
	double V0;
	double LV0;
	CTime StartTime;
	float AxisTextSize;
	float NumberSize;

	float AxisLine;
	float TickLine;
	float DataLine;
	float FitFunc;

	float MainTitleX;
	float MainTitleY;

	float XTitleX;
	float XTitleY;

	float YTitleX;
	float YTitleY;

	float TickLabelsX;
	float TickLabelsY;

	float PointSize;
	float ErrorBarSize;

	float DataxMin;
	float DatayMin;
	float DataxMax;
	float DatayMax;
	float TickLenX;
	float HalfTickLenX;
	float TickLenY;
	float HalfTickLenY;

	UCHAR AxisRed;
	UCHAR AxisGreen;
	UCHAR AxisBlue;

	UCHAR SelectRed;
	UCHAR SelectGreen;
	UCHAR SelectBlue;

	UCHAR DataRed;
	UCHAR DataGreen;
	UCHAR DataBlue;

	UCHAR FitRed;
	UCHAR FitGreen;
	UCHAR FitBlue;

	UCHAR TextRed;
	UCHAR TextGreen;
	UCHAR TextBlue;

	USHORT FitFunRes;

	BOOL AutoLsi;
	BOOL AutoPlot;
	BOOL AutoMonitor;
	BOOL AutoScript;
	BOOL AutoTemplate;
	
	UCHAR BackBlue;
	UCHAR BackGreen;
	UCHAR BackRed;
	
	UCHAR NumberBlue;
	UCHAR NumberGreen;
	UCHAR NumberRed;

	USHORT LsiDx;	
	USHORT LsiDy;
	USHORT LsiX;
	USHORT LsiY;

	USHORT PlotDx;
	USHORT PlotDy;
	USHORT PlotX;
	USHORT PlotY;

	USHORT MonitorDx;
	USHORT MonitorDy;
	USHORT MonitorX;
	USHORT MonitorY;

	USHORT ScriptDx;
	USHORT ScriptDy;
	USHORT ScriptX;	
	USHORT ScriptY;	

	USHORT TemplateDx;
	USHORT TemplateDy;
	USHORT TemplateX;	
	USHORT TemplateY;

	USHORT LastX;	
	USHORT LastY;

	double GlobalKepcoFactor;

	void EmptyActionList(void);
	BOOL LoadMasses(CString MassFile);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CMCPforNTApp();
	BOOL GlobalMeasurementRunning;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMCPforNTApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMCPforNTApp)
	afx_msg void OnAppAbout();
	afx_msg void OnViewHardwaresetup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
