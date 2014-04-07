// MCP for NT.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "camac.h"
#include "MinuitChild.h"
#include "minuit.h"
#include "HardwareDefaults.h"
#include "Hardware.h"
#include "LSIChild.h"
#include "LSIView.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "MCP for NTDoc.h"
#include "ActionObjects.h"

#include "DataChild.h"
#include "DataView.h"
#include "MonitorChild.h"
#include "MonitorView.h"
#include "MCP for NTView.h"



#include "dos.h"
#include <direct.h>
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMCPforNTApp


BEGIN_MESSAGE_MAP(CMCPforNTApp, CWinApp)
	//{{AFX_MSG_MAP(CMCPforNTApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_VIEW_HARDWARESETUP, OnViewHardwaresetup)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMCPforNTApp construction

CMCPforNTApp::CMCPforNTApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMCPforNTApp object

CMCPforNTApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMCPforNTApp initialization

BOOL CMCPforNTApp::InitInstance()
{
	DllMain(0,DLL_PROCESS_ATTACH,NULL);
	StartTime=CTime::GetCurrentTime();

	// CG: The following block was added by the Splash Screen component
	AlexMotorSlot		= 17;
	GlobalActPosition	= 0;
	OutRegister1		= 0;
	OutRegister2		= 0;
	GPIBWrite	= "";
	GPIBRead	= "";
	WriteGpib	= 1;
	ReadGpib	= 1;
	NetWrite	= FALSE;
	{
		CCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);

		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
	}
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	V0				= 0;
	LV0				= 0;
	LastX			= 0;
	LastY			= 0;
	GlobalMeasurementRunning = FALSE;
	pMonitorView	= NULL;

	AxisLine		= (float)GetProfileInt("Display","axislines",		4)/1000;
	AxisTextSize	= (float)GetProfileInt("Display","axistext",		32)/1000;
	DataLine		= (float)GetProfileInt("Display","datalines",		2)/1000;
	DataxMax		= (float)GetProfileInt("Display","dataxmax",		980)/1000;
	DataxMin		= (float)GetProfileInt("Display","dataxmin",		100)/1000;
	DatayMax		= (float)GetProfileInt("Display","dataymax",		900)/1000;
	DatayMin		= (float)GetProfileInt("Display","dataymin",		20)/1000;
	ErrorBarSize	= (float)GetProfileInt("Display","errorbars",		5)/1000;
	FitFunc			= (float)GetProfileInt("Display","fitlines",		2)/1000;
	HalfTickLenX	= (float)GetProfileInt("Display","halfticklenx",	8)/1000;
	NumberSize		= (float)GetProfileInt("Display","numbers",			28)/1000;
	PointSize		= (float)GetProfileInt("Display","points",			5)/1000;
	HalfTickLenY	= (float)GetProfileInt("Display","halftickleny",	5)/1000;
	TickLabelsX		= (float)GetProfileInt("Display","ticklabelsx",		905)/1000;
	TickLabelsY		= (float)GetProfileInt("Display","ticklabelsy",		95)/1000;
	TickLenX		= (float)GetProfileInt("Display","ticklenx",		15)/1000;
	TickLenY		= (float)GetProfileInt("Display","tickleny",		10)/1000;
	TickLine		= (float)GetProfileInt("Display","ticklines",		3)/1000;
	MainTitleX		= (float)GetProfileInt("Display","titleposx",		500)/1000;
	MainTitleY		= (float)GetProfileInt("Display","titleposy",		100)/1000;
	XTitleX			= (float)GetProfileInt("Display","xaxistitlex",		500)/1000;
	XTitleY			= (float)GetProfileInt("Display","xaxistitley",		970)/1000;
	YTitleX			= (float)GetProfileInt("Display","yaxistitlex",		30)/1000;
	YTitleY			= (float)GetProfileInt("Display","yaxistitley",		500)/1000;

	AxisBlue		= GetProfileInt("Display","axisblue",				0);
	AxisGreen		= GetProfileInt("Display","axisgreen",				0);
	AxisRed			= GetProfileInt("Display","axisred",				0);

	DataBlue		= GetProfileInt("Display","datablue",				0);
	DataRed			= GetProfileInt("Display","datared",				0);
	DataGreen		= GetProfileInt("Display","datagreen",				0);

	FitBlue			= GetProfileInt("Display","fitblue",				250);
	FitGreen		= GetProfileInt("Display","fitgreen",				0);
	FitRed			= GetProfileInt("Display","fitred",					0);
	FitFunRes		= GetProfileInt("Display","fitfunres",				250);

	SelectBlue		= GetProfileInt("Display","selectblue",				0);
	SelectGreen		= GetProfileInt("Display","selectgreen",			0);

	SelectRed		= GetProfileInt("Display","selectred",				250);
	TextBlue		= GetProfileInt("Display","textblue",				0);
	TextGreen		= GetProfileInt("Display","textgreen",				0);
	TextRed			= GetProfileInt("Display","textred",				0);


	AutoLsi			= GetProfileInt("Display","autolsi",		TRUE);
	AutoPlot		= GetProfileInt("Display","autoplot",		TRUE);
	AutoMonitor		= GetProfileInt("Display","automonitor",	FALSE);
	AutoScript		= GetProfileInt("Display","autoscript",		TRUE);
	AutoTemplate	= GetProfileInt("Display","autotemplate",	TRUE);
	
	BackBlue		= GetProfileInt("Display","backblue",		255);
	BackGreen		= GetProfileInt("Display","backgreen",		255);
	BackRed			= GetProfileInt("Display","backred",		255);
	
	NumberBlue		= GetProfileInt("Display","numberblue",		0);
	NumberGreen		= GetProfileInt("Display","numbergreen",	0);
	NumberRed		= GetProfileInt("Display","numberred",		0);
																
	LsiDx			= GetProfileInt("Display","lsidx",			500);
	LsiDy			= GetProfileInt("Display","lsidy",			300);
	LsiX			= GetProfileInt("Display","lsix",			100);
	LsiY			= GetProfileInt("Display","lsiy",			100);

	PlotDx			= GetProfileInt("Display","plotdx",			600);
	PlotDy			= GetProfileInt("Display","plotdy",			400);
	PlotX			= GetProfileInt("Display","plotx",			300);
	PlotY			= GetProfileInt("Display","ploty",			10);

	MonitorDx		= GetProfileInt("Display","monitordx",		300);
	MonitorDy		= GetProfileInt("Display","monitordy",		200);
	MonitorX		= GetProfileInt("Display","monitorx",		600);
	MonitorY		= GetProfileInt("Display","monitory",		0);

	ScriptDx		= GetProfileInt("Display","scriptdx",		550);
	ScriptDy		= GetProfileInt("Display","scriptdy",		500);
	ScriptX			= GetProfileInt("Display","scriptx",		300);
	ScriptY			= GetProfileInt("Display","scripty",		100);

	TemplateDx		= GetProfileInt("Display","templatedx",		335);
	TemplateDy		= GetProfileInt("Display","templatedy",		350);
	TemplateX		= GetProfileInt("Display","templatex",		30);
	TemplateY		= GetProfileInt("Display","templatey",		30);

	FerddichSubadd	= GetProfileInt("Hardware","ferddichsubadd",1);
	FlukeGpib		= GetProfileInt("Hardware","flukegpib",		8);
	TempGpib		= GetProfileInt("Hardware","tempgpib",		9);
	FieldGpib		= GetProfileInt("Hardware","fieldgpib",		10);
	GelbeboxSlot	= GetProfileInt("Hardware","gelbeboxslot",	6);
	Gelbebox2Slot	= GetProfileInt("Hardware","gelbebox2slot",	7);
	GpibSlot		= GetProfileInt("Hardware","gpibslot",		10);
	Hp1Gpib			= GetProfileInt("Hardware","hp1gpib",		16);
	Hp2Gpib			= GetProfileInt("Hardware","hp2gpib",		17);
	ListSlot		= GetProfileInt("Hardware","listslot",		24);
	PremaSlot		= GetProfileInt("Hardware","premaslot",		8);
	PresetSlot		= GetProfileInt("Hardware","presetslot",	15);
	ProtonSlot		= GetProfileInt("Hardware","protonslot",	1);
	ProtonSubadd	= GetProfileInt("Hardware","protonsubadd",	7);
	Rs1Gpib			= GetProfileInt("Hardware","rs1gpib",		18);
	Rs2Gpib			= GetProfileInt("Hardware","rs2gpib",		19);
	ScalerSlot		= GetProfileInt("Hardware","scalerslot",	2);
	TriggerSubadd	= GetProfileInt("Hardware","triggersubadd",	0);
	HpSlot			= GetProfileInt("Hardware","hpvoltslot",	7);
	HvSlot			= GetProfileInt("Hardware","hvvoltslot",	9);
	FSlot			= GetProfileInt("Hardware","fvoltslot",		4);
	PremaDelay		= GetProfileInt("Hardware","premadelay",	1000);
	FlukeDelay		= GetProfileInt("Hardware","flukedelay",	100);
	MassDelay		= GetProfileInt("Hardware","massdelay",		5000);
	GPIBDelay		= GetProfileInt("Hardware","gpibdelay",		20);

	GlobalKepcoFactor = (double)GetProfileInt("Hardware","globalkepcofactor",5000000)/100000;

	CamacReset();

	CamacOn = DriverIsExisting();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	LoadStdProfileSettings(10);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_MCPTYPE,
		RUNTIME_CLASS(CMCPforNTDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CMCPforNTView));
	AddDocTemplate(pDocTemplate);
	
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	MassesAvailable = LoadMasses("AtomicMasses.txt");
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL OnInitDialog();
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CMCPforNTApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMCPforNTApp commands

BOOL CAboutDlg::OnInitDialog()
{
	// CG: Folowing code is added by System Info Component
	{
	CDialog::OnInitDialog();
#ifndef _MAC
	CString strFreeDiskSpace;
	CString strFreeMemory;
	CString strFmt;

	// Fill available memory
	MEMORYSTATUS MemStat;
	MemStat.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&MemStat);
	strFmt.LoadString(CG_IDS_PHYSICAL_MEM);
	strFreeMemory.Format(strFmt, MemStat.dwAvailPhys / 1024L);

	//TODO: Add a static control to your About Box to receive the memory
	//      information.  Initialize the control with code like this:
	SetDlgItemText(IDC_PHYSICAL_MEM, strFreeMemory);

	// Fill disk free information
	struct _diskfree_t diskfree;
	int nDrive = _getdrive(); // use current default drive
	if (_getdiskfree(nDrive, &diskfree) == 0)
	{
		strFmt.LoadString(CG_IDS_DISK_SPACE);
		strFreeDiskSpace.Format(strFmt,
			(DWORD)diskfree.avail_clusters *
			(DWORD)diskfree.sectors_per_cluster *
			(DWORD)diskfree.bytes_per_sector / (DWORD)1024L,
			nDrive-1 + _T('A'));
	}
 	else
 		strFreeDiskSpace.LoadString(CG_IDS_DISK_SPACE_UNAVAIL);

	//TODO: Add a static control to your About Box to receive the memory
	//      information.  Initialize the control with code like this:
	SetDlgItemText(IDC_DISK_SPACE, strFreeDiskSpace);

#endif //_MAC
	}

	return TRUE;
}

BOOL CMCPforNTApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following lines were added by the Splash Screen component.
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}

int CMCPforNTApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	WriteProfileInt("Hardware","globalkepcofactor",(int)(GlobalKepcoFactor*100000));
	EmptyActionList();
	DllMain(0,DLL_PROCESS_DETACH,NULL);
	return CWinApp::ExitInstance();
}

void CMCPforNTApp::EmptyActionList(void)
{
	POSITION pos;

	ActionObject* pAction;
	if(!CopyActionList.IsEmpty())
	{
		pos = CopyActionList.GetHeadPosition();
		do
		{
			pAction =  (ActionObject*)CopyActionList.GetAt(pos);
			delete pAction;
			CopyActionList.GetNext(pos);
		}
		while (pos!=NULL);
		CopyActionList.RemoveAll();
	}
}

void CMCPforNTApp::OnViewHardwaresetup() 
{
	// TODO: Add your command handler code here
	CHardwareDefaults* pdlg = new CHardwareDefaults();
	pdlg->m_ferddichsubadd	= GetProfileInt("Hardware","ferddichsubadd",1);
	pdlg->m_flukegpib		= GetProfileInt("Hardware","flukegpib",		8);
	pdlg->m_tempgpib		= GetProfileInt("Hardware","tempgpib",		9);
	pdlg->m_fieldgpib		= GetProfileInt("Hardware","fieldgpib",		10);
	pdlg->m_gelbeboxslot	= GetProfileInt("Hardware","gelbeboxslot",	6);
	pdlg->m_gelbebox2slot	= GetProfileInt("Hardware","gelbebox2slot",	7);
	pdlg->m_gpibslot		= GetProfileInt("Hardware","gpibslot",		10);
	pdlg->m_hp1gpib			= GetProfileInt("Hardware","hp1gpib",		16);
	pdlg->m_hp2gpib			= GetProfileInt("Hardware","hp2gpib",		17);
	pdlg->m_listslot		= GetProfileInt("Hardware","listslot",		24);
	pdlg->m_premaslot		= GetProfileInt("Hardware","premaslot",		8);
	pdlg->m_presetslot		= GetProfileInt("Hardware","presetslot",	15);
	pdlg->m_protonslot		= GetProfileInt("Hardware","protonslot",	1);
	pdlg->m_protonsubadd	= GetProfileInt("Hardware","protonsubadd",	7);
	pdlg->m_rs1gpib			= GetProfileInt("Hardware","rs1gpib",		18);
	pdlg->m_rs2gpib			= GetProfileInt("Hardware","rs2gpib",		19);
	pdlg->m_scalerslot		= GetProfileInt("Hardware","scalerslot",	2);
	pdlg->m_triggersubadd	= GetProfileInt("Hardware","triggersubadd",	0);
	pdlg->m_hpvoltslot		= GetProfileInt("Hardware","hpvoltslot",	7);
	pdlg->m_hvvoltslot		= GetProfileInt("Hardware","hvvoltslot",	9);
	pdlg->m_fvoltslot		= GetProfileInt("Hardware","fvoltslot",		4);
	pdlg->m_premadelay		= GetProfileInt("Hardware","premadelay",	1000);		
	pdlg->m_flukedelay		= GetProfileInt("Hardware","flukedelay",	100);
	pdlg->m_massdelay		= GetProfileInt("Hardware","massdelay",		5000);
	pdlg->m_gpibdelay		= GetProfileInt("Hardware","gpibdelay",		20);

	if(IDOK==pdlg->DoModal())
	{
		WriteProfileInt("Hardware","premadelay",pdlg->m_premadelay);
		PremaDelay	= pdlg->m_premadelay;
		WriteProfileInt("Hardware","flukedelay",pdlg->m_flukedelay);
		FlukeDelay	= pdlg->m_flukedelay;
		WriteProfileInt("Hardware","massdelay",pdlg->m_massdelay);
		MassDelay	= pdlg->m_massdelay;
		WriteProfileInt("Hardware","gpibdelay",pdlg->m_gpibdelay);
		GPIBDelay	= pdlg->m_gpibdelay;
		WriteProfileInt("Hardware","ferddichsubadd",pdlg->m_ferddichsubadd);
		FerddichSubadd	= pdlg->m_ferddichsubadd;
		WriteProfileInt("Hardware","flukegpib",pdlg->m_flukegpib);
		FlukeGpib		=  pdlg->m_flukegpib;
		WriteProfileInt("Hardware","tempgpib",pdlg->m_tempgpib);
		TempGpib		=  pdlg->m_tempgpib;
		WriteProfileInt("Hardware","fieldgpib",pdlg->m_fieldgpib);
		FieldGpib		=  pdlg->m_fieldgpib;
		WriteProfileInt("Hardware","gelbeboxslot",pdlg->m_gelbeboxslot);
		GelbeboxSlot	= pdlg->m_gelbeboxslot;
		WriteProfileInt("Hardware","gelbebox2slot",pdlg->m_gelbebox2slot);
		Gelbebox2Slot	= pdlg->m_gelbebox2slot;
		WriteProfileInt("Hardware","gpibslot",pdlg->m_gpibslot);
		GpibSlot		= pdlg->m_gpibslot;
		WriteProfileInt("Hardware","hp1gpib",pdlg->m_hp1gpib);
		Hp1Gpib			= pdlg->m_hp1gpib;
		WriteProfileInt("Hardware","hp2gpib",pdlg->m_hp2gpib);
		Hp2Gpib			= pdlg->m_hp2gpib;
		WriteProfileInt("Hardware","listslot",pdlg->m_listslot);
		ListSlot		= pdlg->m_listslot;
		WriteProfileInt("Hardware","premaslot",pdlg->m_premaslot);
		PremaSlot		= pdlg->m_premaslot;
		WriteProfileInt("Hardware","presetslot",pdlg->m_presetslot);
		PresetSlot		= pdlg->m_presetslot;
		WriteProfileInt("Hardware","protonslot",pdlg->m_protonslot);
		ProtonSlot		= pdlg->m_protonslot;
		WriteProfileInt("Hardware","protonsubadd",pdlg->m_protonsubadd);
		ProtonSubadd	= pdlg->m_protonsubadd;
		WriteProfileInt("Hardware","rs1gpib",pdlg->m_rs1gpib);
		Rs1Gpib			= pdlg->m_rs1gpib;
		WriteProfileInt("Hardware","rs2gpib",pdlg->m_rs2gpib);
		Rs2Gpib			= pdlg->m_rs2gpib;
		WriteProfileInt("Hardware","scalerslot",pdlg->m_scalerslot);
		ScalerSlot		= pdlg->m_scalerslot;
		WriteProfileInt("Hardware","triggersubadd",pdlg->m_triggersubadd);
		TriggerSubadd	= pdlg->m_triggersubadd;
		WriteProfileInt("Hardware","hpvoltslot",pdlg->m_hpvoltslot);
		HpSlot			= pdlg->m_hpvoltslot;
		WriteProfileInt("Hardware","hvvoltslot",pdlg->m_hvvoltslot);
		HvSlot			= pdlg->m_hvvoltslot;
		WriteProfileInt("Hardware","fvoltslot",pdlg->m_fvoltslot);
		FSlot			= pdlg->m_fvoltslot;

	}
	delete pdlg;
}

BOOL ReadLine(HANDLE pFile,char str[])
{
	char TheChar;
	unsigned long len = 124;
	ULONG read;
	ReadFile(pFile,(LPSTR)str,len,&read,NULL);
	if (read!=len)
	{
		return FALSE;
	};
	do
	{
		ReadFile(pFile,&TheChar,1,&read,NULL);
	}while ((TheChar!=10)&&(read==1));
	str[124]=0;
	if(read==1)return TRUE;
	else return FALSE;
}

void strip(char str[])
{
	char in[200];
	USHORT start_index;
	USHORT stop_index;
	USHORT i;
	strcpy(in,str);
	
	for(start_index=0;(!isdigit(in[start_index]))&&(!isalpha(in[start_index])&&(!ispunct(in[start_index])));start_index++);
	for(stop_index=start_index;(isdigit(in[stop_index]))||(isalpha(in[stop_index])||(ispunct(in[stop_index])));stop_index++);
	for(i=start_index;i<=stop_index;i++) in[i] = tolower(in[i]);
	strncpy(str,&in[start_index],stop_index-start_index);
	str[stop_index-start_index]=0;
}

BOOL CMCPforNTApp::LoadMasses(CString MassFile)
{
	HANDLE DataBaseFP;
	USHORT i;
	char str[200];
	char element[10];
	char mass[20];

	CString data;

	DataBaseFP=CreateFile(MassFile,
							GENERIC_READ,
							FILE_SHARE_READ,
							NULL,
							OPEN_EXISTING,
							(DWORD) 0,
							NULL);
	if (DataBaseFP==INVALID_HANDLE_VALUE) 
		return FALSE;
	// Header
	for(i=0;i<39;i++)ReadLine(DataBaseFP,str);
	
	while (ReadLine(DataBaseFP,str))
	{
		strncpy(&element[0],&str[16],3);
		strncpy(&element[3],&str[20],3);
		str[99]='.';
		strncpy(mass,&str[96],10);
		strip(element);
		strip(mass);
		MassesMap.SetAt(element,mass);
	}

	CloseHandle(DataBaseFP);
	return TRUE;
}