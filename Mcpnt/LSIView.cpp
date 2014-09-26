// LSIView.cpp : implementation file
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
#include "LSIChild.h"
#include "LSIView.h"
#include "MCP for NTView.h"
#include "camac.h"
#include "HardwareConfig.h"

#include "hardware.h"
#include "dos.h"
#include <direct.h>

#define FontDim 15



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define WM_LSIKILLED	WM_USER + 300
#define MaxCommands 50
#define Morning 2
#define Night   18

#include <stdlib.h>
#include <math.h>
#include <setjmp.h>

KOMMANDO Commands[] =
	{
	{"exit"				,0,0,_exit				,"usage : exit"},
	{"help"				,0,0,_help				,"usage : help"},
	{"mem"				,0,0,_mem				,"usage : mem"},
	{"uptime"			,0,0,_uptime			,"usage : uptime"},
	{"global"			,0,0,_global			,"usage : global"},
	{"ver"				,0,0,_ver				,"usage : ver"},
	{"about"			,0,0,_about				,"usage : about"},
	{"who"				,0,0,_who				,"usage : who"},
	{"er"				,0,0,_er				,"usage : er"},
	{"go"				,0,0,_go				,"usage : go"},
	{"ergo"				,0,0,_ergo				,"usage : ergo"},
	{"autogo"			,0,0,_autogo			,"usage : autogo"},
	{"append"			,1,1,_append			,"usage : append filename"},
	{"ddprog"			,0,0,_ddprog			,"usage : "},
	{"clock"			,1,1,_clock				,"usage : clock TN"},
	{"click"			,0,0,_click				,"usage : click"},
	{"isotope"			,1,2,_isotope			,"usage : isotope TN string"},
	{"element"			,0,1,_element			,"usage : element string"},
	{"wavenumber"		,0,1,_wavenumber		,"usage : wavenumber no"},
	{"cmdlog"			,2,2,_cmdlog			,"usage : cmdlog TN no"},
	{"debug"			,0,0,_debug				,"usage : "},
	{"monitor"			,2,2,_monitor			,"usage : monitor TN no"},
	{"setmonitor"		,2,2,_setmonitor		,"usage : setmonitor TN counter"},
	{"domonitor"		,1,1,_domonitor			,"usage : domonitor ON/OFF"},
	{"setclicks"		,2,2,_setclicks			,"usage : setclicks TN clicks"},
	{"callcrosslist"	,2,2,_callcrosslist		,"usage : callcrosslist TN 0/1"},
	{"sweeptime"		,1,1,_sweeptime			,"usage : sweeptime TN"},
	{"cls"				,0,0,_cls				,"usage : cls"},
	{"disp"				,1,1,_disp				,"usage : disp TN"},
	{"d"				,2,2,_disp				,"usage : d(isplay) TN counter"},
	{"dispnorm"			,1,1,_dispnorm			,"usage : dispnorm TN"},
	{"dispsum"			,1,1,_dispsum			,"usage : dispsum TN"},
	{"dispsumnorm"		,1,1,_dispsumnorm		,"usage : dispsumnorm TN"},
	{"dispprotons"		,1,1,_dispprotons		,"usage : dispprotons TN"},
	{"counteroffset"	,3,3,_counteroffset		,"usage : counteroffset TN counter offset"},
	{"setpmpred"		,3,3,_setpmpred			,"usage : setpmpred TN counter counter"},
	{"display"			,2,2,_display			,"usage : display TN counter"},
	{"ousers232"		,1,1,_ousers232			,"usage : ousers232 ON/OFF"},
	{"odisp"			,2,2,_odisp				,"usage : odisp TN counter"},
	{"ores"				,2,2,_ores				,"usage : ores TN no"},
	{"ochannel"			,3,3,_ochannel			,"usage : ochannel TN no counter"},
	{"oread"			,2,2,_oread				,"usage : oread TN counter"},
	{"ostatus"			,0,0,_ostatus			,"usage : ostatus"},
	{"otrigger"			,0,0,_otrigger			,"usage : otrigger"},
	{"oexttrigger"		,0,0,_oexttrigger		,"usage : oexttrigger"},
	{"omeasdc"			,0,0,_omeasdc			,"usage : omeasdc"},
	{"ofasttrigger"		,0,0,_ofasttrigger		,"usage : ofasttrigger"},
	{"osavespec"		,3,3,_osavespec			,"usage : osavespec FN TN channel [from to]"},
	{"osavetimespec"	,3,3,_osavetimespec		,"usage : osavetimespec FN TN channel [from to]"},
	{"osavespecerr"		,3,3,_osavespecerr		,"usage : osavespecerr FN TN channel [from to]"},
	{"osavetimespecerr"	,3,3,_osavetimespecerr	,"usage : osavetimespecerr FN TN channel [from to]"},
	{"odelay"			,3,3,_odelay			,"usage : odelay TN counter delay"},
	{"omode"			,3,3,_omode				,"usage : omode TN counter mode"},
	{"savespec"			,3,3,_savespec			,"usage : savespec FN TN counter [from to]"},
	{"savespecerr"		,3,3,_savespecerr		,"usage : savespecerr FN TN counter [from to]"},
	{"saveasymspec"		,3,3,_saveasymspec		,"usage : saveasymspec FN TN counter [from to]"},
	{"sum"				,4,4,_sum				,"usage : sum TN a+b+.. new-spec-no"},
	{"norm"				,6,6,_norm				,"usage : morm TN a+b+../c+d+.. new-spec-no"},
	{"asym"				,4,4,_asym				,"usage : asym TN a b new-spec-no"},
	{"vdisp"			,1,1,_vdisp				,"usage : vdisp TN"},
	{"list"				,0,0,_list				,"usage : list"},
	{"save"				,1,1,_save				,"usage : save filename"},
	{"asave"			,1,1,_asave				,"usage : asave"},
	{"setautosave"		,2,2,_setautosave		,"usage : setautosave filename no"},
	{"autosave"			,0,0,_autosave			,"usage : autosave"},
	{"cy"				,1,1,_cy				,"usage : cy no"},
	{"scaler"			,2,2,_scaler			,"usage : scaler TN spectrum"},
	{"sc"				,0,0,_sc				,"usage : "},
	{"lswitch"			,3,3,_lswitch			,"usage : lswitch TN on-value off-value"},
	{"lnoswitch"		,1,1,_lnoswitch			,"usage : lnoswitch TN"},
	{"lno"				,2,2,_lno				,"usage : lno TN no"},
	{"l0"				,0,0,_l0				,"usage : l0"},
	{"l1"				,0,0,_l1				,"usage : l1"},
	{"v0"				,0,1,_v0				,"usage : v0"},
	{"lv0"				,0,1,_lv0				,"usage : lv0"},
	{"f0hp8"			,1,1,_f0hp8				,"usage : f0hp8 value[MHz]"},
	{"f0hp3"			,1,1,_f0hp3				,"usage : f0hp3 value[MHz]"},
	{"f0hp32"			,1,1,_f0hp32			,"usage : f0hp32 value[MHz]"},
	{"f0rs"				,1,1,_f0rs				,"usage : f0rs value[MHz]"},
	{"hpoff"			,0,0,_hpoff				,"usage : hpoff"},
	{"hpfm"				,0,0,_hpfm				,"usage : hpfm"},
	{"hpam"				,0,0,_hpam				,"usage : hpam"},
	{"rsfm"				,2,2,_rsfm				,"usage : rsfm TN value[kHz]"},
	{"rsmodoff"			,1,1,_rsmodoff			,"usage : rsmodoff TN"},
	{"rsmodfreq"		,2,2,_rsmodfreq			,"usage : rsmodfreq TN value[kHz]"},
	{"rs0modfreq"		,1,1,_rs0modfreq		,"usage : rs0modfreq value[kHz]"},
	{"rs0fm"			,1,1,_rs0fm				,"usage : rs0fm value[kHz]"},
	{"rs0modoff"		,0,0,_rs0modoff			,"usage : rs0modoff"},
	{"getmass"			,0,0,_getmass			,"usage : getmass"},
	{"getstatus"		,0,0,_getstatus			,"usage : getstatus"},
	{"getmassfactor"	,0,0,_getmassfactor		,"usage : getmassfactor"},
	{"getfield"			,0,0,_getfield			,"usage : getfield"},
	{"getht"			,0,0,_getht				,"usage : getht"},
	{"setnetwrite"		,0,0,_setnetwrite		,"usage : setnetwrite"},
	{"unsetnetwrite"	,0,0,_unsetnetwrite		,"usage : unsetnetwrite"},
	{"voltages"			,0,0,_voltages			,"usage : voltages"},
	{"out"				,3,3,_out				,"usage : out slot-no subaddress value"},
	{"in"				,2,2,_in				,"usage : in slot-no subaddress"},
	{"iec"				,0,0,_iec				,"usage : iec"},
	{"iecrec"			,0,0,_iecrec			,"usage : iecrec"},
	{"iecsend"			,2,5,_iecsend			,"usage : iecsend address command"},
	{"iecsendrec"		,2,5,_iecsendrec		,"usage : iecsendrec address command"},
	{"comsend"			,1,1,_comsend			,"usage : comsend command"},
	{"comsendrec"		,1,1,_comsendrec		,"usage : comsendrec command"},
	{"v"				,3,3,_v					,"usage : v TN start stop"},
	{"lv"				,3,3,_lv				,"usage : lv TN start stop"},
	{"c"				,2,2,_c					,"usage : c TN no"},
	{"n"				,2,2,_n					,"usage : n TN no"},
	{"lramp"			,3,3,_lramp				,"usage : lramp TN start stop"},
	{"fhp8"				,3,3,_fhp8				,"usage : fhp8 TN start stop"},
	{"fhp3"				,3,3,_fhp3				,"usage : fhp3 TN start stop"},
	{"fhp32"			,3,3,_fhp32				,"usage : fhp32 TN start stop"},
	{"frs"				,3,3,_frs				,"usage : frs TN start stop"},
	{"amp8"				,1,2,_amp8				,"usage : amp8 [TN] amp"},
	{"amp3"				,1,2,_amp3				,"usage : amp3 [TN] amp"},
	{"amp32"			,1,2,_amp32				,"usage : amp32 [TN] amp"},
	{"amprs"			,1,2,_amprs				,"usage : amprs [TN] amp"},
	{"repeatsteps"		,2,2,_repeatsteps		,"usage : repeatsteps TN no"},
	{"tomass"			,2,2,_tomass			,"usage : tomass TN mass"},
	{"tofluke"			,2,2,_tofluke			,"usage : tofluke TN fluke"},
	{"setfluke"			,0,1,_setfluke			,"usage : setfluke fluke"},
	{"setmass"			,1,1,_setmass			,"usage : setmass mass"},
	{"show"				,0,0,_show				,"usage : show [TN]"},
	{"info"				,0,0,_info				,"usage : info"},
	{"hardware"			,0,0,_hardware			,"usage : hardware"},
	{"camacclear"		,0,0,_camacclear		,"usage : camacclear"},
	{"clrinhibit"		,0,0,_clrinhibit		,"usage : clrinhibit"},
	{"setinhibit"		,0,0,_setinhibit		,"usage : setinhibit"},
	{"delete"			,2,2,_delete			,"usage : delete TN no"},
	{"newtrack"			,0,0,_newtrack			,"usage : newtrack1"},
	{"eichungtrack"		,0,0,_eichungtrack		,"usage : eichungtrack"},
	{"deftrack"			,0,0,_deftrack			,"usage : deftrack"},
	{"isoltrack"		,0,0,_isoltrack			,"usage : isoltrack"},
	{"slowisoltrack"	,0,0,_slowisoltrack		,"usage : slowisoltrack"},
	{"asymtrack"		,0,0,_asymtrack			,"usage : asymtrack"},
	{"superisoltrack"	,0,0,_superisoltrack  	,"usage : superisoltrack"},
	{"deltrack"			,1,1,_deltrack			,"usage : deltrack TN"},
	{"copytrack"		,1,1,_copytrack			,"usage : copytrack TN"},
	{"newaction"		,0,3,_newaction			,"usage : newaction TN action"},
	{"moveaction"		,3,3,_moveaction		,"usage : moveaction TN from to"},
	{"configaction"		,2,2,_configaction		,"usage : configacion TN no"},
	{"confighwaction"	,2,2,_confighwaction	,"usage : configwacion TN no"},
	{"configtrack"		,1,1,_configtrack		,"usage : configtrack TN"},
	{"updownsweep"		,1,1,_updownsweep		,"usage : updownsweep TN"},
	{"noupdownsweep"	,1,1,_noupdownsweep		,"usage : noupdownsweep TN"},
	{"configdata"		,0,0,_configdata		,"usage : configdata"},
	{"movetrack"		,2,2,_movetrack			,"usage : movetrack from to"},
	{"print"			,1,1,_print				,"usage : print destination"},
	{"channels"			,1,1,_channels			,"usage : channels ON/OFF"},
	{"beamgate"			,1,1,_beamgate			,"usage : beamgate ON/OFF"},
	{"refbit"			,1,1,_refbit			,"usage : refbit ON/OFF"},
	{"warnprotons"		,1,1,_warnprotons		,"usage : warnprotons ON/OFF"},
	{"readprotons"		,1,1,_readprotons		,"usage : readprotons ON/OFF"},
	{"badpulses"		,1,1,_badpulses			,"usage : badpulses no"},
	{"redostep"			,1,1,_redostep			,"usage : redostep ON/OFF"},
	{"newredostepmode"	,1,1,_newredostepmode	,"usage : newredostepmode ON/OFF"},
	{"slowreset"		,1,1,_slowreset			,"usage : slowreset ON/OFF"},
	{"rampreset"		,1,1,_rampreset			,"usage : rampreset ON/OFF"},
	{"configreset"		,1,1,_configreset		,"usage : configreset no"},
	{"uselp"			,1,1,_uselp				,"usage : uselp ON/OFF"},
	{"usedd"			,1,1,_usedd				,"usage : usedd ON/OFF"},
	{"useirqdd"			,1,1,_useirqdd			,"usage : useirqdd ON/OFF"},
	{"whatdd"			,0,0,_whatdd			,"usage : whatdd"},
	{"probedd"			,0,0,_probedd			,"usage : probedd"},
	{"irqdd"			,0,0,_irqdd				,"usage : irqdd"},
	{"irqokdd"			,0,0,_irqokdd			,"usage : irqokdd"},
	{"resetdd"			,0,0,_resetdd			,"usage : resetdd"},
	{"nextirqdd"		,0,0,_nextirqdd			,"usage : nextirqdd"},
	{"protonlimit"		,1,1,_protonlimit		,"usage : protonlimit value"},
	{"ignoreprotons"	,1,1,_ignoreprotons		,"usage : ignoreprotons TN"},
	{"lookforprotons"	,1,1,_lookforprotons	,"usage : lookforprotons TN"},
	{"closebeamgate"	,2,2,_closebeamgate		,"usage : closebeamgate TN time"},
	{"opengate"			,2,2,_opengate			,"usage : opengate TN channel"},
	{"closegate"		,2,2,_closegate			,"usage : closegate TN channel"},
	{"radioactive"		,1,1,_radioactive		,"usage : radioactive TN"},
	{"reference"		,1,1,_reference			,"usage : reference TN"},
	{"motor"			,1,1,_motor				,"usage : motor steps"},
	{"motorcm"			,1,1,_motorcm			,"usage : motorcm cm"},
	{"motornew"			,1,1,_motornew			,"usage : motornew steps"},
	{"setmotorcount"	,1,1,_setmotorcount		,"usage : setmotorcount value"},
	{"getmotorcount"	,0,0,_getmotorcount		,"usage : getmotorcount"},
	{"fastmotor"		,1,1,_fastmotor			,"usage : fastmotor TN"},
	{"normalmotor"		,1,1,_normalmotor		,"usage : normalmotor TN"},
	{"motordelay"		,1,1,_motordelay		,"usage : motordelay value"},
	{"stepdelay"		,1,1,_stepdelay			,"usage : stepdelay value"},
	{"accelerate"		,1,1,_accelerate		,"usage : accelerate ON/OFF"},
	{"accsteps"			,1,1,_accsteps			,"usage : accsteps value"},
	{"steps"			,2,2,_steps				,"usage : steps TN value"},
	{"distance"			,2,2,_distance			,"usage : distance TN value"},
	{"stepmode"			,1,1,_stepmode			,"usage : stepmode TN"},
	{"distmode"			,1,1,_distmode			,"usage : distmode TN"},
	{"direction"		,1,1,_direction			,"usage : direction f[orward]/b[ackward]"},
	{"minmotorcount"	,1,1,_minmotorcount		,"usage : minmotorcount value"},
	{"maxmotorcount"	,1,1,_maxmotorcount		,"usage : maxmotorcount value"},
	{"settaperad"		,1,1,_settaperad		,"usage : settaperad value"},
	{"settapeth"		,1,1,_settapeth			,"usage : settapeth value"},
	{"break"			,1,1,_break				,"usage : break ON/OFF"},
	{"bothmotors"		,1,1,_bothmotors		,"usage : bothmotors ON/OFF"},
	{"motorclicktime"	,2,2,_motorclicktime	,"usage : motorclicktime TN value"},
	{"motorwaitclicks"	,2,2,_motorwaitclicks	,"usage : motorwaitclicks TN value"},
	{"motorwaitfactor"	,2,2,_motorwaitfactor	,"usage : motorwaitfactor TN value"},
	{"kepcofactor"		,0,1,_kepcofactor		,"usage : kepcofactor [value]"},
	{"asymfactor"		,0,0,_asymfactor		,"usage : asymfactor [value]"},
	{"warndelay"		,1,1,_warndelay			,"usage : warndelay value"},
	{"regression"		,0,0,_regression		,"usage : regression"},
	{"dispkepco"		,0,0,_dispkepco			,"usage : dispkepco"},
	{"dispdelta"		,0,0,_dispdelta			,"usage : dispdelta"},
	{"dispisolde"		,0,0,_dispisolde		,"usage : dispisolde"},
	{"protondelay"		,2,2,_protondelay		,"usage : protondelay TN ms"},
	{"parttrigger"		,2,2,_parttrigger		,"usage : parttrigger TN steps"},
	{"triggerdelay"		,1,1,_triggerdelay		,"usage : triggerdelay value"},
	{"range"			,1,1,_range				,"usage : range value"},
	{"measperstep"		,2,2,_measperstep		,"usage : measperstep TN no"},
	{"measatscan"		,2,2,_measatscan		,"usage : measatscan TN no"},
	{"premafactor"		,0,0,_premafactor		,"usage : premafactor [value]"},
	{"flukedelay"		,2,2,_flukedelay		,"usage : flukedelay TN value"},
	{"isoldedelay"		,2,2,_isoldedelay		,"usage : isoldedelay TN value"},
	{"computerdelay"	,2,2,_computerdelay		,"usage : computerdelay TN value"},
	{"setmultino"		,3,3,_setmultino		,"usage : setmultino TN mfNo no"},
	{"changescbit"		,2,2,_changescbit		,"usage : changescbit TN clicks"},
	{"clearscbit"		,2,2,_clearscbit		,"usage : clearscbit TN clicks"},
	{"halt"				,0,0,_halt				,"usage : halt"},
	{"kill"				,0,0,_kill				,"usage : kill"},
	{"running"			,0,0,_running			,"usage : running"},
	{"closemonitor"		,0,0,_closemonitor		,"usage : closemonitor"},
	{"closestatus"		,0,0,_closestatus		,"usage : closestatus"},
	{"mot"				,0,0,_mot				,"usage : mot"},
	{"grabframe"		,2,2,_grabframe			,"usage : grabframe TN no"},
	{"dispframe"		,1,1,_dispframe			,"usage : dispframe TN"},
	{"setframename"		,1,1,_setframename		,"usage : setframename FN"},
	{"saveframe"		,2,2,_saveframe			,"usage : saveframe TN filename"},
	{"loadframe"		,2,2,_loadframe			,"usage : loadframe TN filename"},
	{"system"			,0,0,_system			,"usage : system"},
	{"setccdmode"		,2,2,_setccdmode		,"usage : setccdmode TN no"},
	{"setccdarea"		,5,5,_setccdarea		,"usage : setccdarea TN x0 y0 x1 y1"},
	{"cdisp"			,1,1,_cdisp				,"usage : cdisp TN"},
	{"camacfout"		,3,3,_camacfout			,"usage : camacfout station subaddress fcn [dataout]"},
	{"camacfin"			,3,3,_camacfin			,"usage : camacfin station subaddress fcn"},
	{"usestrobe"		,1,1,_usestrobe			,"usage : usestrobe ON/OFF"},
	{"load"				,1,1,_load				,"usage : load filename"},
	{"domath"			,2,2,_domath			,"usage : domath operator(+ or -) FN"},
	{"f"				,0,0,_f					,"usage : f"},
	{"b"				,0,0,_b					,"usage : b"},
	{0,-1,-1,0,0}
	};


	TrackObject* GetTrack(CMCPforNTView* pView,char* TheTrack);
	ActionObject* GetActionObj(CMCPforNTView* pView,TrackObject* TheTrack,CString TheObject);
	ActionObject* GetActionObj(CMCPforNTView* pView,CString TheObject);
	ActionObject* GetActionObjByIndex(CMCPforNTView* pView,TrackObject* TheTrack,USHORT TheIndex);
	POSITION GetEndOfTrack(CMCPforNTView* pView,TrackObject* TheTrack);
	void CalculateRealStartStop(double* start,double* stop,double* step,USHORT channels,double resolution);


/////////////////////////////////////////////////////////////////////////////
// CLSIView

IMPLEMENT_DYNCREATE(CLSIView, CRichEditView)

CLSIView::CLSIView()
{
}

CLSIView::CLSIView(CView* pParent)
{
	

	CTime time=CTime::GetCurrentTime();

	CharCount			= 0;
	TOGGLE				= FALSE;
	OLDCOMMAND			= FALSE;
	lsi_exit			= FALSE;
	pMCPView            = pParent;
	IsActivated         = FALSE;
	CRect rect;
	rect.top            = 100;
	rect.left           = 100;
	rect.right          = 600;
	rect.bottom         = 400;

	CString Title;
	Title="LSI-Box for "+pMCPView->GetDocument()->GetTitle();
	pCLSIChild = new CLSIChild(this);
	pCLSIChild->CMDIChildWndEx::Create(NULL,
						Title, 
						WS_CHILD|WS_VISIBLE|WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
						rect, 
						NULL, 
						NULL );

	rect.top            = 0;
	rect.left           = 0;
	rect.right          = 600;
	rect.bottom         = 400;

	this->Create(NULL,NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL |
	ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL,rect,pCLSIChild,AFX_IDW_PANE_FIRST+1,NULL);
	IsActivated=TRUE;
	pCLSIChild->GetClientRect(rect);
	ResizeWin(rect.right,rect.bottom);
	LOGFONT logFont1;
    logFont1.lfHeight = FontDim;
    logFont1.lfWidth = 0;
    logFont1.lfEscapement = 0;
    logFont1.lfOrientation = 0;
    logFont1.lfWeight = 400;
    logFont1.lfItalic = 0;
    logFont1.lfUnderline = 0;
    logFont1.lfStrikeOut = 0;
    logFont1.lfCharSet = ANSI_CHARSET;
    logFont1.lfOutPrecision = OUT_DEFAULT_PRECIS;
    logFont1.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    logFont1.lfQuality = PROOF_QUALITY;
    logFont1.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;
	strcpy(logFont1.lfFaceName, "Courier New");

	NewFont1 = new CFont();
	NewFont1->CreateFontIndirect(&logFont1);
	SetFont(NewFont1,FALSE);
	printfh("\r\n");
	printfh(LsiWelcome+pMCPView->GetDocument()->GetTitle()+"\r\n");
	printfh("\r\n");
	if ((time.GetHour()>Morning) && (time.GetHour()<Night))
		TheMessage = DayMessage1;		
	else TheMessage = NightMessage1;
	printfh(TheMessage);
}

CLSIView::~CLSIView()
{
	CMCPforNTView* pMCPV = (CMCPforNTView*)pMCPView;
	pMCPV->pCLSIView = NULL;
	IsActivated = FALSE;
}


BEGIN_MESSAGE_MAP(CLSIView, CRichEditView)
	//{{AFX_MSG_MAP(CLSIView)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLSIView drawing

void CLSIView::ResizeWin(ULONG x,ULONG y)
{
	if (IsActivated)MoveWindow(0,0,x,y,TRUE);
}

void CLSIView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CLSIView diagnostics

#ifdef _DEBUG
void CLSIView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CLSIView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLSIView message handlers

void CLSIView::OnChange() 
{
	// TODO: Add your control notification handler code here
	
}

void CLSIView::OnUpdate() 
{
	// TODO: Add your control notification handler code here
	
}


void CLSIView::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	//POSITION pos;
	//CMCPforNTDoc* pDoc = (CMCPforNTDoc*)pMCPView->GetDocument();
	//pos = pDoc->pChildWin.Find(this);
	//if (pos!=NULL)pDoc->pChildWin.RemoveAt(pos);
	CMCPforNTView* pMCPV = (CMCPforNTView*)pMCPView;
	pMCPV->pCLSIView = NULL;
	delete NewFont1;
	CRichEditView::PostNcDestroy();
}

void CLSIView::Dispatch(CString TheCommand)
{
	char input[255];
	char command[255];
	CString answer;
	char arg1[80];
	char arg2[80];
	char arg3[80];
	char arg4[80];
	char arg5[80];
	int i,numargs;

	strcpy(input,TheCommand);
	numargs = sscanf(input,"%s %s %s %s %s %s",command,&arg1,&arg2,&arg3,&arg4,&arg5)-1;
	BOOL FOUND = FALSE;
	for( i = 0; Commands[i].NumArgsMin!=-1; i++ )
	{
		if(!_stricmp(command,Commands[i].comm))
		{
			FOUND = TRUE;
			if(numargs < Commands[i].NumArgsMin||numargs > Commands[i].NumArgsMax)
			{
				printf("\r\n");
				printf(Commands[i].Help);
			}
			else
			{
				answer = Commands[i].func((CMCPforNTView*)pMCPView,numargs,&arg1,&arg2,&arg3,&arg4,&arg5);
				printf("\r\n");
				printf(answer);
			}
		}
	}
	if(!FOUND)
	{
		printf("\r\n");
		printf(WrongCommand);
	}

}

void CLSIView::log_printf(const char *format, ...)
{
  va_list arg_ptr;
  char OutString[5000];
  va_start (arg_ptr, format);
  vsprintf(OutString,format, arg_ptr);
  PrintString=PrintString+OutString;	  
  va_end(arg_ptr);
}

void CLSIView::log_flush(void)
{
  CHARFORMAT textformat;
  textformat.cbSize = sizeof(CHARFORMAT);
  textformat.dwMask = CFM_PROTECTED | CFM_COLOR;
  textformat.crTextColor = RGB(0,0,0);
  textformat.dwEffects   = 0;
  GetRichEditCtrl().SetSel(GetRichEditCtrl().GetTextLength()-1,GetRichEditCtrl().GetTextLength()-1); 
  GetRichEditCtrl().SetSelectionCharFormat(textformat);
  GetRichEditCtrl().ReplaceSel(PrintString);
  PrintString="";
}

void CLSIView::printf(const char *format, ...)
{
  CHARFORMAT textformat;
  va_list arg_ptr;
  char OutString[5000];
  va_start (arg_ptr, format);
  vsprintf(OutString,format, arg_ptr);	  
  va_end(arg_ptr);
  
  textformat.cbSize = sizeof(CHARFORMAT);
  textformat.dwMask = CFM_PROTECTED | CFM_COLOR;
  textformat.crTextColor = RGB(0,0,0);
  textformat.dwEffects   = 0;
  
  GetRichEditCtrl().SetSel(GetRichEditCtrl().GetTextLength()-1,GetRichEditCtrl().GetTextLength()-1);
  GetRichEditCtrl().SetSelectionCharFormat(textformat);
  GetRichEditCtrl().ReplaceSel(OutString);
  GetRichEditCtrl().SetSel(GetRichEditCtrl().GetTextLength()-1,GetRichEditCtrl().GetTextLength()-1);
}

void CLSIView::printfh(const char *format, ...)
{
  CHARFORMAT textformat;
  va_list arg_ptr;
  char OutString[5000];
  va_start (arg_ptr, format);
  vsprintf(OutString,format, arg_ptr);	  
  va_end(arg_ptr);
  
  textformat.cbSize = sizeof(CHARFORMAT);
  textformat.dwMask = CFM_PROTECTED | CFM_COLOR;
  textformat.crTextColor = RGB(0,0,230);
  textformat.dwEffects   = CFE_PROTECTED;

  
  GetRichEditCtrl().SetSel(GetRichEditCtrl().GetTextLength()-1,GetRichEditCtrl().GetTextLength()-1);
  GetRichEditCtrl().SetSelectionCharFormat(textformat);
  GetRichEditCtrl().ReplaceSel(OutString);
  GetRichEditCtrl().SetSel(GetRichEditCtrl().GetTextLength()-1,GetRichEditCtrl().GetTextLength()-1);

  textformat.cbSize = sizeof(CHARFORMAT);
  textformat.dwMask = CFM_PROTECTED | CFM_COLOR;
  textformat.crTextColor = RGB(0,0,0);
  textformat.dwEffects   = 0;
  
  GetRichEditCtrl().SetSel(GetRichEditCtrl().GetTextLength()-1,GetRichEditCtrl().GetTextLength()-1);
  GetRichEditCtrl().SetSelectionCharFormat(textformat);
}

CString CLSIView::getline(void)
{
	int len;
	char string[81];
	CString InString;
	len=GetRichEditCtrl().GetLine(GetRichEditCtrl().LineFromChar(GetRichEditCtrl().GetTextLength()-1),&string[0],80);
	string[len]='\0';
	InString = string;
	InString = InString.Right(InString.GetLength()-InString.Find('>')-2);
	InString = InString.Left(InString.GetLength()-2);
	return InString;
}

void CLSIView::log_over(const char *format, ...)
{
  CHARFORMAT textformat;
  textformat.cbSize = sizeof(CHARFORMAT);
  textformat.dwMask = CFM_PROTECTED | CFM_COLOR;
  textformat.crTextColor = RGB(0,0,230);
  textformat.dwEffects   = CFE_PROTECTED;
  
  va_list arg_ptr;
  char OutString[2000];
  va_start (arg_ptr, format);
  vsprintf(OutString,format, arg_ptr);
  int Start=GetRichEditCtrl().LineIndex(-1);
  int Len=GetRichEditCtrl().LineLength(-1);  
  GetRichEditCtrl().SetSel(Start,Start+Len);
  GetRichEditCtrl().SetSelectionCharFormat(textformat);
  GetRichEditCtrl().ReplaceSel((CString)OutString);  
  va_end(arg_ptr);
}


void CLSIView::OnLButtonUp(UINT nFlags, CPoint point) 
{
}
void CLSIView::OnLButtonDown(UINT nFlags, CPoint point) 
{
}
void CLSIView::OnRButtonDown(UINT nFlags, CPoint point) 
{
}
void CLSIView::OnRButtonUp(UINT nFlags, CPoint point) 
{
}


BOOL CLSIView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{		
		switch (pMsg->wParam)
			{
			case VK_RETURN:
				{
					CString InputString;
					CTime time = CTime::GetCurrentTime();
					if ((time.GetHour()>Morning) && (time.GetHour()<Night))
					{
						if(TOGGLE)
						{
							TheMessage = DayMessage1;
							TOGGLE = FALSE;
						}
						else
						{
							TheMessage = DayMessage2;
							TOGGLE = TRUE;
						}
					}
					else
					{
						if(TOGGLE)
						{
							TheMessage = NightMessage1;
							TOGGLE = FALSE;
						}
						else
						{
							TheMessage = NightMessage2;
							TOGGLE = TRUE;
						}
					}

					InputString=getline();
					if(InputString!="")CommandList.AddTail(InputString);
					if (CommandList.GetCount()>MaxCommands) CommandList.RemoveHead();
					LastCommand = CommandList.GetTailPosition();
					OLDCOMMAND = FALSE;
					if(InputString!="")Dispatch(InputString);
					CharCount = 0;
					printfh("\r\n"+TheMessage);
					if (lsi_exit) GetParent()->DestroyWindow();
					return TRUE;
					break;
				}

				case VK_UP:
				{
					CString TheCommand;
					if(!CommandList.IsEmpty())
					{
						TheCommand = CommandList.GetPrev(LastCommand);
						log_over(TheMessage);
						printf(TheCommand);
						CharCount = TheCommand.GetLength();
						OLDCOMMAND=TRUE;
						if(LastCommand==NULL)
						{
							LastCommand = CommandList.GetHeadPosition();
						}
					}
					
					return TRUE;
					break;
				}

				case VK_DOWN:
				{
					CString TheCommand;
					if(!CommandList.IsEmpty())
					{
						TheCommand = CommandList.GetNext(LastCommand);
						log_over(TheMessage);
						printf(TheCommand);
						CharCount = TheCommand.GetLength();
						OLDCOMMAND=TRUE;
						if(LastCommand==NULL)
						{
							LastCommand = CommandList.GetTailPosition();
						}
					}
					return TRUE;
					break;
				}

				case VK_RIGHT:
				{
					CharCount++;
					break;
				}
				
				case VK_LEFT:
				{
					CharCount--;
					if(CharCount < 0) 
					{
						CharCount = 0;
						return TRUE;
					}
					break;
				}
				
				case VK_BACK:
				{
					CharCount--;
					if(CharCount < 0) 
					{
						CharCount = 0;
						return TRUE;
					}
					break;
				}

				case VK_DELETE:
				{
					return TRUE;
					break;
				}

				case VK_TAB:
				{
					CString InputString;
					InputString = getline();
					InputString = FindRest(InputString);
					log_over(TheMessage);
					printf(InputString);
					CharCount = InputString.GetLength();
					return TRUE;
					break;
				}

				default: 
				{
					if((pMsg->wParam>40)&&(pMsg->wParam<91)) CharCount++;
					break;
				}
			}
	}
	return CRichEditView::PreTranslateMessage(pMsg);
}

CString CLSIView::FindRest(CString InputString)
{
	CString OutputString;
	char input[255];
	char command[255];
	CString answer;
	int i,foundInd,foundNum = 0;

	strcpy(input,InputString);

	for( i = 0; Commands[i].NumArgsMin!=-1; i++ )
	{
		strcpy(command,Commands[i].comm);
		if(!strncmp(command,input,strlen(input)))
		{
			foundNum++;
			foundInd = i;
		}
	}
	if (foundNum <  1) Beep(1000,100);
	if (foundNum == 1) return Commands[foundInd].comm;
	if (foundNum >  1) 
	{
		log_printf("\r\n");
		for( i = 0; Commands[i].NumArgsMin!=-1; i++ )
		{
			strcpy(command,Commands[i].comm);
			if(!strncmp(command,input,strlen(input)))
			{
				log_printf(Commands[i].comm);
				log_printf("\r\n");
			}	
		}
		log_flush();
	}	return "";

	return InputString;
}


CString _exit(CMCPforNTView* pView,...)
{
	CString answer = "Closing LSI-Box";
	pView->pCLSIView->lsi_exit = TRUE;
	return answer;
}

CString _help(CMCPforNTView* pView,...)
{
	CString answer = "";
	pView->SendMessage(WM_HELP,0,0x0012fbe8);
	return answer;
}

CString _mem(CMCPforNTView* pView,...)
{
	CString answer;
	CString strFreeDiskSpace;
	CString strFreeMemory;
	CString strFmt;

	// Fill available memory
	MEMORYSTATUS MemStat;
	MemStat.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&MemStat);
	strFmt.LoadString(CG_IDS_PHYSICAL_MEM);
	strFreeMemory.Format(strFmt, MemStat.dwAvailPhys / 1024L);

	struct _diskfree_t diskfree;
	int nDrive = _getdrive(); 
	if (_getdiskfree(nDrive, &diskfree) == 0)
	{
		strFmt.LoadString(CG_IDS_DISK_SPACE);
		strFreeDiskSpace.Format(strFmt,
			(DWORD)diskfree.avail_clusters *
			(DWORD)diskfree.sectors_per_cluster *
			(DWORD)diskfree.bytes_per_sector / (DWORD)1024L,
			nDrive-1 + _T('A'));
	}
 	else strFreeDiskSpace.LoadString(CG_IDS_DISK_SPACE_UNAVAIL);

	answer = strFreeMemory+" Free mem\r\n"+strFreeDiskSpace;

	return answer;
}

CString _uptime(CMCPforNTView* pView,...)
{
	CString answer;
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	CTimeSpan uptime;
	CTime ActualTime = CTime::GetCurrentTime();
	uptime = ActualTime - pApp->StartTime; 
	answer = uptime.Format( "d: %D, h: %H, m: %M, s: %S" );
	return answer;
}

CString _global(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _ver(CMCPforNTView* pView,...)
{
	CString answer = "MCP for NT Version 1.0";

	return answer;
}

CString _about(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _who(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _er(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _go(CMCPforNTView* pView,...)
{
	CString answer = "Device not ready...";
	CMCPforNTApp* pMyApp = (CMCPforNTApp*)AfxGetApp();
	if(!pView->GetDocument()->MeasurementRunning&&!pMyApp->GlobalMeasurementRunning&&pMyApp->CamacOn)
	{
		pView->OnGo();
		answer = "Measurement started";
	}
	pView->pCLSIView->GetParent()->BringWindowToTop();
	pView->pCLSIView->SetFocus();
	return answer;
}


CString _ergo(CMCPforNTView* pView,...)
{
	CString answer = "Device not ready...";
	CMCPforNTApp* pMyApp = (CMCPforNTApp*)AfxGetApp();
	if(!pView->GetDocument()->MeasurementRunning&&!pMyApp->GlobalMeasurementRunning&&pMyApp->CamacOn)
	{
		pView->OnErgo();
		answer = "New Measurement started";
	}
	pView->pCLSIView->GetParent()->BringWindowToTop();
	pView->pCLSIView->SetFocus();
	return answer;
}


CString _autogo(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _append(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _ddprog(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _clock(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();

	int	NumArgs;

	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs	= va_arg(arg_ptr,int);
	
	

	return answer;
}


CString _click(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();

	int	NumArgs;

	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs	= va_arg(arg_ptr,int);
	
	


	return answer;
}


CString _isotope(CMCPforNTView* pView,...)
{
	BOOL Found = FALSE;

	CString answer = "Track not found";


	int	NumArgs;
	char* StrTrack;
	char* Isotope;
	

	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs		= va_arg(arg_ptr,int);
	StrTrack	= va_arg(arg_ptr,char*);
	if(NumArgs==2)Isotope = va_arg(arg_ptr,char*);
	TrackObject* pTrack = GetTrack(pView,StrTrack);

	if(pTrack == NULL) return answer;
	
	if(NumArgs == 1)answer = pTrack->Isotope;
	if(NumArgs == 2)
	{
		pTrack->Isotope = Isotope;
		answer = "";
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);
		
	}
	return answer;
}


CString _element(CMCPforNTView* pView,...)
{
	CString answer = "";
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();

	int	NumArgs;
	char* Element;

	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs	= va_arg(arg_ptr,int);
	if(NumArgs == 1)Element = va_arg(arg_ptr,char*);
	
	if(NumArgs == 0) answer = pDocument->TheElement;
	if(NumArgs == 1) 
	{
		pDocument->TheElement = Element;
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);
	}
	return answer;
}


CString _wavenumber(CMCPforNTView* pView,...)
{
	CString answer = "";
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();

	int	NumArgs;
	char* StrWaveNumber;
	double WaveNumber;

	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs	= va_arg(arg_ptr,int);
	if(NumArgs == 1)
	{
		StrWaveNumber = va_arg(arg_ptr,char*);
		WaveNumber = atof(StrWaveNumber);
	}
	
	if(NumArgs == 0) answer.Format("%.8g",pDocument->WaveNumber);
	if(NumArgs == 1) 
	{
		pDocument->WaveNumber = WaveNumber;
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);
	}
	return answer;
}


CString _cmdlog(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();

	int	NumArgs;

	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs	= va_arg(arg_ptr,int);
	
	
	return answer;
}


CString _debug(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _monitor(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();

	int	NumArgs;

	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs	= va_arg(arg_ptr,int);
	
	
	return answer;
}


CString _setmonitor(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();

	int	NumArgs;

	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs	= va_arg(arg_ptr,int);
	
	
	return answer;
}


CString _domonitor(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();

	int	NumArgs;

	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs	= va_arg(arg_ptr,int);
	
	
	return answer;
}


CString _setclicks(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();

	int	NumArgs;

	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs	= va_arg(arg_ptr,int);
	
	
	return answer;
}


CString _callcrosslist(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();

	int	NumArgs;
	
	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs	= va_arg(arg_ptr,int);
	
	
	return answer;
}


CString _sweeptime(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();

	int	NumArgs;
	
	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs	= va_arg(arg_ptr,int);
	
	
	return answer;
}


CString _cls(CMCPforNTView* pView,...)
{
	CString answer = "";
	pView->pCLSIView->SetWindowText("");
	return answer;
}


CString _disp(CMCPforNTView* pView,...)
{
	CString answer = "Scaler doesn't exist";
	POSITION pos;
	CString TheObject;
	BOOL FOUND = FALSE;
	
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();

	int	NumArgs;
	int Track;
	int SubReg;
	
	char* StrTrack;
	char* StrSubReg;
	CString TrackStr;
	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs	= va_arg(arg_ptr,int);
	StrTrack	= va_arg(arg_ptr,char*);
	StrSubReg	= va_arg(arg_ptr,char*);
	
	
	Track	= atoi(StrTrack);
	SubReg	= atoi(StrSubReg);

	if(!pDocument->ActionObjList.IsEmpty())
	{
		ActionObject* pActionObj = (ActionObject*)pDocument->ActionObjList.GetHead();
		pos = pDocument->ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)pDocument->ActionObjList.GetNext(pos);
			if (pActionObj!=NULL)
			{
				TheObject = pActionObj->GetName();
				if (TheObject.Find(__PM_SpectrumObj)!=-1)
				{
					PM_SpectrumObj* pScalerObj = (PM_SpectrumObj*)pActionObj;
					if ((pScalerObj->SubRegNo==(int)SubReg)&&
						(pScalerObj->pTrack->MyPosition==(int)Track))
					{
						FOUND = TRUE;
						answer = "";
						pScalerObj->DoDoubleClickAction();
					}
				}
			}
		}
		while ((pos!=NULL)&&(!FOUND));
	}
	pView->pCLSIView->GetParent()->BringWindowToTop();
	pView->pCLSIView->SetFocus();
	return answer;
}


CString _dispnorm(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _dispsum(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _dispsumnorm(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _dispprotons(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _counteroffset(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _setpmpred(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _display(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _ousers232(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _odisp(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _ores(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _ochannel(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _oread(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _ostatus(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _otrigger(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _oexttrigger(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _omeasdc(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _ofasttrigger(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _osavespec(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _osavetimespec(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _osavespecerr(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _osavetimespecerr(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _odelay(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _omode(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _savespec(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _savespecerr(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _saveasymspec(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _sum(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _norm(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _asym(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _vdisp(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _list(CMCPforNTView* pView,...)
{
	CString answer = pView->GetDocument()->GetInfo()+"\r\n";
	POSITION pos;
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();
	USHORT i;
	CString TheObject;
	if(!pDocument->ActionObjList.IsEmpty())
	{
		ActionObject* pActionObj = (ActionObject*)pDocument->ActionObjList.GetHead();
		pos = pDocument->ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)pDocument->ActionObjList.GetNext(pos);
			if (pActionObj!=NULL)
			{
				if(pActionObj->GetName()==_TrackObj) 
				{
					answer = answer+ "\r\n";
					i=1;
					answer = answer+pActionObj->GetName()+pActionObj->GetInfo()+"\r\n";
				}
				else
				{
					TheObject.Format("%u",i);
					answer = answer+"["+TheObject+"] "+pActionObj->GetName()+"\r\n";
					i++;
				}
			}
		}
		while (pos!=NULL);
	}

	return answer;

}


CString _save(CMCPforNTView* pView,...)
{
	CString answer = "";

	int	NumArgs;

	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs	= va_arg(arg_ptr,int);
	CString SaveString = va_arg(arg_ptr,char*);

	CFile File;
	if(File.Open(SaveString,CFile::modeRead))
	{
		answer= "File exists, not saving !";
		File.Close();
		return answer;
	}
	else
	{ 
		if(!pView->GetDocument()->OnSaveDocument(SaveString))
		{
			answer = "Not saved, unknown reason";
		}
		else 
		{
			pView->GetDocument()->SetTitle(SaveString);
			pView->GetDocument()->SetPathName(SaveString);
			pView->pCLSIView->GetParent()->SetWindowText("LSI-Box for "+pView->GetDocument()->GetTitle());
			answer = "Saved as "+SaveString;
		}
	}	
	return answer;
}


CString _asave(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _setautosave(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _autosave(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _cy(CMCPforNTView* pView,...)
{
	CString answer = "";
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();

	int	NumArgs;
	char* StrCy;
	int Cy;

	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs	= va_arg(arg_ptr,int);
	if(NumArgs == 1)
	{
		StrCy = va_arg(arg_ptr,char*);
		Cy = atoi(StrCy);
	}
	
	if(NumArgs == 0) answer.Format("%u",pDocument->NoOfCycles);
	if(NumArgs == 1) 
	{
		pDocument->NoOfCycles = Cy;
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);
	}
	return answer;
}


CString _scaler(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _sc(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _lswitch(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _lnoswitch(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _lno(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _l0(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _l1(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _v0(CMCPforNTView* pView,...)
{
	CString answer;
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();

	int	NumArgs;
	char* StrVoltage;
	double Voltage;

	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs	= va_arg(arg_ptr,int);
	if(NumArgs == 1)
	{
		StrVoltage = va_arg(arg_ptr,char*);
		Voltage = atof(StrVoltage);
	}
	
	if(NumArgs == 0) 
	{
		answer = "HP set to 0V";
		SendDNAFCamac((ULONG)~0,pApp->HpSlot,0,16);
		SendDNAFCamac((ULONG)~0,pApp->HpSlot,1,16);
		pApp->V0 = 0;
		return answer;
	}

	ULONG Value	= (ULONG)(1000.0*fabs(Voltage));
	ULONG i,NoDigits,Digit;
    unsigned long Buffer;

    if (Value!=0)
    {
		Buffer = 10;
		for (NoDigits=1; Value>=Buffer; NoDigits++)           // Anzahl der Stellen bestimmen
		{ 
			Buffer = Buffer*10; 
		};
		Buffer = 0;                                           // reset Buffer
		for (i=1; i<=NoDigits; i++)
		{
			Digit = Value % 10;
			Value = Value / 10;
			Buffer = Buffer + ( Digit << ((i-1)*4) );
		}
	}
    else
	{
		Buffer = 0; 
	};
     
	if(Voltage<0)SendDNAFCamac((ULONG)(~0-1),pApp->HpSlot,1,16);
	else SendDNAFCamac((ULONG)~0,pApp->HpSlot,1,16);
	SendDNAFCamac((ULONG)~Buffer,pApp->HpSlot,0,16);
	answer.Format("HP set to %g V",Voltage);
	pApp->V0 = Voltage;
	return answer;
}


CString _lv0(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _lv(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _f0hp8(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _f0hp3(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _f0hp32(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _f0rs(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _hpoff(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _hpfm(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _hpam(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _rsfm(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _rsmodoff(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _rsmodfreq(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _rs0modfreq(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _rs0fm(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _rs0modoff(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _getmass(CMCPforNTView* pView,...)
{
	CString answer;
	double value;

	CMCPforNTApp* pMyApp = (CMCPforNTApp*)AfxGetApp();

	//
	// Get mass value
	//
	value = pMyApp->GetMass();

	answer.Format("mass = %.5f amu",value);

	return answer;
}


CString _getstatus(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _getmassfactor(CMCPforNTView* pView,...)
{
	CString answer;
	double value;

	CMCPforNTApp* pMyApp = (CMCPforNTApp*)AfxGetApp();

	//
	// Get massFactor value
	//
	value = pMyApp->GetMassFactor();

	answer.Format("massfactor = %.5f",value);

	return answer;
}


CString _getfield(CMCPforNTView* pView,...)
{
	CString answer;
	double value;

	CMCPforNTApp* pMyApp = (CMCPforNTApp*)AfxGetApp();

	//
	// Get field value
	//
	value = pMyApp->GetField();

	answer.Format("field = %.5f",value);

	return answer;
}


CString _getht(CMCPforNTView* pView,...)
{
	CString answer;
	double value;

	CMCPforNTApp* pMyApp = (CMCPforNTApp*)AfxGetApp();

	//
	// Get field value
	//
	value = pMyApp->GetIsoHighvolt();

	answer.Format("HT = %.5f V",value);

	return answer;
}


CString _setnetwrite(CMCPforNTView* pView,...)
{
	CString answer = "net write actions now enabled !";
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	pApp->NetWrite = TRUE;
	return answer;
}


CString _unsetnetwrite(CMCPforNTView* pView,...)
{
	CString answer = "net write actions now disabled !";
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	pApp->NetWrite = FALSE;
	return answer;
}


CString _voltages(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}


CString _out(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _in(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _iec(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _iecrec(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _iecsend(CMCPforNTView* pView,...)
{

	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	CString answer = "Illegal address";

	int	i,NumArgs;
	char*	StrAddress;
	USHORT 	address;
	char*	IECCommand="";
	char*	NewString;

	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs		= va_arg(arg_ptr,int);
	StrAddress	= va_arg(arg_ptr,char*);
	
	strcpy(IECCommand,"");
	for(i=0;i<NumArgs-1;i++)
	{
		NewString = va_arg(arg_ptr,char*);
		strcat(IECCommand,NewString);
	}

	address		= atoi(StrAddress);
	if(address<0||address>31) return answer;
	
	SendGPIB(IECCommand,address,pApp->GpibSlot);
	answer = "";


	return answer;
}

CString _iecsendrec(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _comsend(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _comsendrec(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _v(CMCPforNTView* pView,...)
{
	CString answer = "Track not found";


	int	NumArgs;
	double From;
	double To;
	double Step;
	char* StrTrack;
	char* StrFrom;
	char* StrTo;
	
	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs		= va_arg(arg_ptr,int);
	StrTrack	= va_arg(arg_ptr,char*);

	if(NumArgs==3)
	{
		StrFrom	= va_arg(arg_ptr,char*);
		StrTo	= va_arg(arg_ptr,char*);
		From    = atof(StrFrom);
		To      = atof(StrTo);
	}

	TrackObject* pTrack = GetTrack(pView,StrTrack);

	if(pTrack == NULL) return answer;
	HP_VoltageSweepObj* pActionObj = (HP_VoltageSweepObj*)GetActionObj(pView,pTrack,_HP_VoltageSweepObj);
	if(pActionObj == NULL) 
	{
		return "Object not found";
	}

	if(NumArgs == 1)answer.Format("%.8g to %.8g",pActionObj->Sweep_Start,pActionObj->Sweep_Stop);
	if(NumArgs == 3)
	{
		CalculateRealStartStop(&From,&To,&Step,pTrack->Channels,HPVoltageResolution);
		pActionObj->Sweep_Start = From;
		pActionObj->Sweep_Stop	= To;
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);
		answer.Format("Sweep set : %.8gV to %.8gV  step = %.8gV",pActionObj->Sweep_Start,pActionObj->Sweep_Stop,Step);
		return answer;
	}
	return answer;

}


CString _c(CMCPforNTView* pView,...)
{
	CString answer = "Track not found";


	int	NumArgs;
	int Channels;
	char* StrTrack;
	char* StrChannels;
	
	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs		= va_arg(arg_ptr,int);
	StrTrack	= va_arg(arg_ptr,char*);
	if(NumArgs==2)
	{
		StrChannels	= va_arg(arg_ptr,char*);
		Channels = atoi(StrChannels);
	}

	TrackObject* pTrack = GetTrack(pView,StrTrack);

	if(pTrack == NULL) return answer;
	
	if(NumArgs == 1)answer.Format("%u",pTrack->Channels);
	if(NumArgs == 2)
	{
		pTrack->Channels = Channels;
		answer = "";
		pView->GetDocument()->UpdateAllData(pTrack->MyPosition);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);
	}
	return answer;
}

CString _n(CMCPforNTView* pView,...)
{
	CString answer = "Track not found";


	int	NumArgs;
	int NoOfScans;
	char* StrTrack;
	char* StrNoOfScans;

	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs		= va_arg(arg_ptr,int);
	StrTrack	= va_arg(arg_ptr,char*);
	if(NumArgs==2)
	{
		StrNoOfScans	= va_arg(arg_ptr,char*);
		NoOfScans = atoi(StrNoOfScans);
	}

	TrackObject* pTrack = GetTrack(pView,StrTrack);

	if(pTrack == NULL) return answer;
	
	if(NumArgs == 1)answer.Format("%u",pTrack->NoOfScans);
	if(NumArgs == 2)
	{
		pTrack->NoOfScans = NoOfScans;
		answer = "";
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);
	}
	return answer;

}

CString _lramp(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _fhp8(CMCPforNTView* pView,...)
{
	CString answer = "Track not found";


	int	NumArgs;
	double From;
	double To;
	double Step;
	char* StrTrack;
	char* StrFrom;
	char* StrTo;
	
	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs		= va_arg(arg_ptr,int);
	StrTrack	= va_arg(arg_ptr,char*);

	if(NumArgs==3)
	{
		StrFrom	= va_arg(arg_ptr,char*);
		StrTo	= va_arg(arg_ptr,char*);
		From    = atof(StrFrom);
		To      = atof(StrTo);
	}

	TrackObject* pTrack = GetTrack(pView,StrTrack);

	if(pTrack == NULL) return answer;
	HP8660B_FrequencySweepObj* pActionObj = (HP8660B_FrequencySweepObj*)GetActionObj(pView,pTrack,_HP8660B_FrequencySweepObj);
	if(pActionObj == NULL) 
	{
		return "Object not found";
	}

	if(NumArgs == 1)answer.Format("%.8g to %.8g",pActionObj->Sweep_Start,pActionObj->Sweep_Stop);
	if(NumArgs == 3)
	{
		CalculateRealStartStop(&From,&To,&Step,pTrack->Channels,HP8660Resolution);
		pActionObj->Sweep_Start = From;
		pActionObj->Sweep_Stop	= To;
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);
		answer.Format("Sweep set : %.8gMHz to %.8gMHz  step = %.8gMHz",pActionObj->Sweep_Start,pActionObj->Sweep_Stop,Step);
		return answer;
	}
	return answer;

}


CString _fhp3(CMCPforNTView* pView,...)
{
	CString answer = "Track not found";


	int	NumArgs;
	double From;
	double To;
	double Step;
	char* StrTrack;
	char* StrFrom;
	char* StrTo;
	
	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs		= va_arg(arg_ptr,int);
	StrTrack	= va_arg(arg_ptr,char*);

	if(NumArgs==3)
	{
		StrFrom	= va_arg(arg_ptr,char*);
		StrTo	= va_arg(arg_ptr,char*);
		From    = atof(StrFrom);
		To      = atof(StrTo);
	}

	TrackObject* pTrack = GetTrack(pView,StrTrack);

	if(pTrack == NULL) return answer;
	HP3325B_FrequencySweepObj* pActionObj = (HP3325B_FrequencySweepObj*)GetActionObj(pView,pTrack,_HP3325B_FrequencySweepObj);
	if(pActionObj == NULL) 
	{
		return "Object not found";
	}

	if(NumArgs == 1)answer.Format("%.8g to %.8g",pActionObj->Sweep_Start,pActionObj->Sweep_Stop);
	if(NumArgs == 3)
	{
		CalculateRealStartStop(&From,&To,&Step,pTrack->Channels,HP3325Resolution);
		pActionObj->Sweep_Start = From;
		pActionObj->Sweep_Stop	= To;
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);
		answer.Format("Sweep set : %.8gMHz to %.8gMHz  step = %.8gMHz",pActionObj->Sweep_Start,pActionObj->Sweep_Stop,Step);
		return answer;
	}
	return answer;

}

CString _fhp32(CMCPforNTView* pView,...)
{
	CString answer = "Track not found";


	int	NumArgs;
	double From;
	double To;
	double Step;
	char* StrTrack;
	char* StrFrom;
	char* StrTo;
	
	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs		= va_arg(arg_ptr,int);
	StrTrack	= va_arg(arg_ptr,char*);

	if(NumArgs==3)
	{
		StrFrom	= va_arg(arg_ptr,char*);
		StrTo	= va_arg(arg_ptr,char*);
		From    = atof(StrFrom);
		To      = atof(StrTo);
	}

	TrackObject* pTrack = GetTrack(pView,StrTrack);

	if(pTrack == NULL) return answer;
	HP3325B_FrequencySweepObj* pActionObj = (HP3325B_FrequencySweepObj*)GetActionObj(pView,pTrack,_HP3325B_FrequencySweepObj);
	if(pActionObj == NULL) 
	{
		return "Object not found";
	}

	if(NumArgs == 1)answer.Format("%.8g to %.8g",pActionObj->Sweep_Start,pActionObj->Sweep_Stop);
	if(NumArgs == 3)
	{
		CalculateRealStartStop(&From,&To,&Step,pTrack->Channels,HP3325Resolution);
		pActionObj->Sweep_Start = From;
		pActionObj->Sweep_Stop	= To;
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);
		answer.Format("Sweep set : %.8gMHz to %.8gMHz  step = %.8gMHz",pActionObj->Sweep_Start,pActionObj->Sweep_Stop,Step);
		return answer;
	}
	return answer;

}

CString _amp8(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _amp3(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _amp32(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _amprs(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _frs(CMCPforNTView* pView,...)
{
	CString answer = "Track not found";


	int	NumArgs;
	double From;
	double To;
	double Step;
	char* StrTrack;
	char* StrFrom;
	char* StrTo;
	
	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs		= va_arg(arg_ptr,int);
	StrTrack	= va_arg(arg_ptr,char*);

	if(NumArgs==3)
	{
		StrFrom	= va_arg(arg_ptr,char*);
		StrTo	= va_arg(arg_ptr,char*);
		From    = atof(StrFrom);
		To      = atof(StrTo);
	}

	TrackObject* pTrack = GetTrack(pView,StrTrack);

	if(pTrack == NULL) return answer;
	RS_FrequencySweepObj* pActionObj = (RS_FrequencySweepObj*)GetActionObj(pView,pTrack,_RS_FrequencySweepObj);
	if(pActionObj == NULL) 
	{
		return "Object not found";
	}

	if(NumArgs == 1)answer.Format("%.8g to %.8g",pActionObj->Sweep_Start,pActionObj->Sweep_Stop);
	if(NumArgs == 3)
	{
		CalculateRealStartStop(&From,&To,&Step,pTrack->Channels,RSResolution);
		pActionObj->Sweep_Start = From;
		pActionObj->Sweep_Stop	= To;
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);
		answer.Format("Sweep set : %.8gMHz to %.8gMHz  step = %.8gMHz",pActionObj->Sweep_Start,pActionObj->Sweep_Stop,Step);
		return answer;
	}
	return answer;

}


CString _repeatsteps(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _tomass(CMCPforNTView* pView,...)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	CString answer = "Track not found";


	int	NumArgs;
	double Mass;
	char* StrTrack;
	char* StrMass;
	
	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs		= va_arg(arg_ptr,int);
	StrTrack	= va_arg(arg_ptr,char*);
	if(NumArgs==2)
	{
		StrMass	= va_arg(arg_ptr,char*);
		Mass = atof(StrMass);
		if(!pApp->MassesMap.IsEmpty())
		{
			CString low;
			CString Str2Mass;
			low = StrMass;
			if(pApp->MassesMap.Lookup(low,Str2Mass))
			{
				Mass	= atof(Str2Mass);
			}
		}
	}

	TrackObject* pTrack = GetTrack(pView,StrTrack);

	if(pTrack == NULL) return answer;
	MassSwitchObj* pActionObj = (MassSwitchObj*)GetActionObj(pView,pTrack,_MassSwitchObj);
	if(pActionObj == NULL) 
	{
		MassAndFlukeSwitchObj* pActionObj = (MassAndFlukeSwitchObj*)GetActionObj(pView,pTrack,_MassAndFlukeSwitchObj);
		if(pActionObj == NULL) return "Object not found";
		if(NumArgs == 1)answer.Format("%u",pActionObj->ToMassNo);
		if(NumArgs == 2)
		{
			pActionObj->ToMassNo = Mass;
			answer = "";
			pView->GetDocument()->SetModifiedFlag(TRUE);
			pView->GetDocument()->UpdateAllViews(NULL,0,NULL);
		}
		return answer;
	}

	if(NumArgs == 1)answer.Format("%u",pActionObj->ToMassNo);
	if(NumArgs == 2)
	{
		pActionObj->ToMassNo = Mass;
		answer.Format("Mass in track %s set to %.4f amu",StrTrack,Mass);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);
	}
	return answer;

}

CString _tofluke(CMCPforNTView* pView,...)
{
	CString answer = "Track not found";


	int	NumArgs;
	int Fluke;
	char* StrTrack;
	char* StrFluke;
	
	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs		= va_arg(arg_ptr,int);
	StrTrack	= va_arg(arg_ptr,char*);
	if(NumArgs==2)
	{
		StrFluke	= va_arg(arg_ptr,char*);
		Fluke = atoi(StrFluke);
	}

	TrackObject* pTrack = GetTrack(pView,StrTrack);

	if(pTrack == NULL) return answer;
	FlukeSwitchObj* pActionObj = (FlukeSwitchObj*)GetActionObj(pView,pTrack,_FlukeSwitchObj);
	if(pActionObj == NULL) 
	{
		MassAndFlukeSwitchObj* pActionObj = (MassAndFlukeSwitchObj*)GetActionObj(pView,pTrack,_MassAndFlukeSwitchObj);
		if(pActionObj == NULL) return "Object not found";
		if(NumArgs == 1)answer.Format("%u",pActionObj->ToFlukeNo);
		if(NumArgs == 2)
		{
			pActionObj->ToFlukeNo = Fluke;
			answer = "";
			pView->GetDocument()->SetModifiedFlag(TRUE);
			pView->GetDocument()->UpdateAllViews(NULL,0,NULL);
		}
		return answer;
	}

	if(NumArgs == 1)answer.Format("%u",pActionObj->ToFlukeNo);
	if(NumArgs == 2)
	{
		pActionObj->ToFlukeNo = Fluke;
		answer = "";
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);
	}
	return answer;

}

CString _setmass(CMCPforNTView* pView,...)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	CString answer = "";

	int	NumArgs;
	char* StrSetMass;
	double SetMass;
	double Massfactor = 0;
	double Mass = 0;
	
	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs	= va_arg(arg_ptr,int);
	if(NumArgs == 1)
	{
		StrSetMass = va_arg(arg_ptr,char*);
		SetMass = atof(StrSetMass);
		if(!pApp->MassesMap.IsEmpty())
		{
			CString low;
			CString Str2Mass;
			low = StrSetMass;
			if(pApp->MassesMap.Lookup(low,Str2Mass))
			{
				SetMass	= atof(Str2Mass);
			}
		}
	}
	
	if(NumArgs == 0) 
	{
		CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
		Mass = pApp->GetMass();
		answer.Format("%.8g",Mass);
	}
	if(NumArgs == 1) 
	{
		CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
		pApp->SetMass(SetMass);
		answer.Format("Mass set to %.4f amu",SetMass);
	}
	return answer;
}

CString _setfluke(CMCPforNTView* pView,...)
{
	CString answer = "";
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();

	int	NumArgs;
	char* StrFluke;
	int Fluke;

	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs	= va_arg(arg_ptr,int);
	if(NumArgs==0)
	{
		Fluke = 0;
	}
	else
	{
		StrFluke = va_arg(arg_ptr,char*);
		Fluke = atoi(StrFluke);
	}

	switch(Fluke)
	{
	case 0:
		OffBit(1,0);
		OffBit(1,1);
		OffBit(1,2);
		DelayCamac(500);
		break;
	case 1:
		OffBit(1,1);
		OffBit(1,2);
		DelayCamac(500);
		OnBit(1,0);
		break;
	case 2:
		OffBit(1,0);
		OffBit(1,2);
		DelayCamac(500);
		OnBit(1,1);
		break;
	case 3:
		OffBit(1,0);
		OffBit(1,1);
		DelayCamac(500);
		OnBit(1,2);
		break;
	}
	answer.Format("Fluke %d set",Fluke);
	return answer;
}

CString _show(CMCPforNTView* pView,...)
{
	CString answer = pView->GetDocument()->GetInfo()+"\r\n";
	CString answerx;
	POSITION pos;
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();
	USHORT i;
	CString TheObject;
	if(!pDocument->ActionObjList.IsEmpty())
	{
		ActionObject* pActionObj = (ActionObject*)pDocument->ActionObjList.GetHead();
		pos = pDocument->ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)pDocument->ActionObjList.GetNext(pos);
			if (pActionObj!=NULL)
			{
				if(pActionObj->GetName()==_TrackObj) 
				{
					answer = answer+ "\r\n";
					i=1;
					answer = answer+pActionObj->GetName()+pActionObj->GetInfo()+"\r\n";
				}
				else
				{
					TheObject.Format("%u",i);
					answerx="["+TheObject+"] "+pActionObj->GetName()+" ";
					while(answerx.GetLength()<21)answerx=answerx+".";
					answerx = answerx+" "+pActionObj->GetInfo()+"\r\n";
					answer=answer+answerx;
					i++;
				}
			}
		}
		while (pos!=NULL);
	}

	return answer;
}

CString _info(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _hardware(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _camacclear(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _clrinhibit(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _setinhibit(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _delete(CMCPforNTView* pView,...)
{
	CString answer = "Track not found";

	ActionObject* Action;


	int	NumArgs;
	int Index;

	char* StrTrack;
	char* StrIndex;
	
	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs		= va_arg(arg_ptr,int);
	StrTrack	= va_arg(arg_ptr,char*);
	StrIndex		= va_arg(arg_ptr,char*);
	
	Index	= atoi(StrIndex);

	TrackObject* pTrack = GetTrack(pView,StrTrack);

	if(pTrack == NULL) return answer;
	
	Action	= GetActionObjByIndex(pView,pTrack,Index);

	if(Action!=NULL)
	{
		pView->GetDocument()->ActionObjList.RemoveAt(pView->GetDocument()->ActionObjList.Find(Action));
		delete Action;
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);
		return "";
	}
	return "Position not found";
}


CString _newtrack(CMCPforNTView* pView,...)
{
	CString answer = "";
	
	USHORT MyPos = 1;
	TrackObject* pAction		= new TrackObject;
	pAction->DoString			= "normal";
	pAction->pTrack				= pAction;
	pAction->pDocument			= pView->GetDocument();
	
	
	pView->GetDocument()->ActionObjList.AddTail(pAction);
	pView->RearrangeTrackNum();
	pView->GetDocument()->UpdateAllViews(NULL,0,NULL);
	

	return answer;
}

CString _eichungtrack(CMCPforNTView* pView,...)
{
	CString TheTrack;
	CString answer = "";
	USHORT Track;
	pView->pCLSIView->Dispatch("newtrack");
	ActionObject* pAction = (ActionObject*)pView->GetDocument()->ActionObjList.GetTail();
	Track = pAction->pTrack->MyPosition;
	TheTrack.Format("%u",Track);
	pView->pCLSIView->Dispatch("newaction "+TheTrack+" "+__FlukeSwitchObj);
	pView->pCLSIView->Dispatch("newaction "+TheTrack+" "+__HP_VoltageSweepObj);
	pView->pCLSIView->Dispatch("v "+TheTrack+" -10 10");
	pView->pCLSIView->Dispatch("newaction "+TheTrack+" "+__KepcoEichungVoltageObj);

	return answer;
}

CString _deftrack(CMCPforNTView* pView,...)
{
	CString TheTrack;
	CString answer = "";
	USHORT Track;
	pView->pCLSIView->Dispatch("newtrack");
	ActionObject* pAction = (ActionObject*)pView->GetDocument()->ActionObjList.GetTail();
	Track = pAction->pTrack->MyPosition;
	TheTrack.Format("%u",Track);
	pView->pCLSIView->Dispatch("newaction "+TheTrack+" "+__FlukeSwitchObj);
	pView->pCLSIView->Dispatch("newaction "+TheTrack+" "+__TriggerObj);
	pView->pCLSIView->Dispatch("newaction "+TheTrack+" "+__PM_SpectrumObj);

	return answer;
}

CString _isoltrack(CMCPforNTView* pView,...)
{
	CString TheTrack;
	CString answer = "";
	USHORT Track;
	pView->pCLSIView->Dispatch("newtrack");
	ActionObject* pAction = (ActionObject*)pView->GetDocument()->ActionObjList.GetTail();
	Track = pAction->pTrack->MyPosition;
	TheTrack.Format("%u",Track);
	pView->pCLSIView->Dispatch("newaction "+TheTrack+" "+__FlukeSwitchObj);
	pView->pCLSIView->Dispatch("newaction "+TheTrack+" "+__TriggerObj);
	pView->pCLSIView->Dispatch("newaction "+TheTrack+" "+__PM_SpectrumObj);

	return answer;

}

CString _slowisoltrack(CMCPforNTView* pView,...)
{
	CString TheTrack;
	CString answer = "";
	USHORT Track;
	pView->pCLSIView->Dispatch("newtrack");
	ActionObject* pAction = (ActionObject*)pView->GetDocument()->ActionObjList.GetTail();
	Track = pAction->pTrack->MyPosition;
	TheTrack.Format("%u",Track);
	pView->pCLSIView->Dispatch("newaction "+TheTrack+" "+__FlukeSwitchObj);
	pView->pCLSIView->Dispatch("newaction "+TheTrack+" "+__TriggerObj);
	pView->pCLSIView->Dispatch("newaction "+TheTrack+" "+__PM_SpectrumObj);

	return answer;

}

CString _asymtrack(CMCPforNTView* pView,...)
{
	CString TheTrack;
	CString answer = "";
	USHORT Track;
	pView->pCLSIView->Dispatch("newtrack");
	ActionObject* pAction = (ActionObject*)pView->GetDocument()->ActionObjList.GetTail();
	Track = pAction->pTrack->MyPosition;
	TheTrack.Format("%u",Track);
	pView->pCLSIView->Dispatch("newaction "+TheTrack+" "+__FlukeSwitchObj);
	pView->pCLSIView->Dispatch("newaction "+TheTrack+" "+__TriggerObj);
	pView->pCLSIView->Dispatch("newaction "+TheTrack+" "+__PM_SpectrumObj);

	return answer;

}

CString _superisoltrack(CMCPforNTView* pView,...)
{
	CString TheTrack;
	CString answer = "";
	USHORT Track;
	pView->pCLSIView->Dispatch("newtrack");
	ActionObject* pAction = (ActionObject*)pView->GetDocument()->ActionObjList.GetTail();
	Track = pAction->pTrack->MyPosition;
	TheTrack.Format("%u",Track);
	pView->pCLSIView->Dispatch("newaction "+TheTrack+" "+__FlukeSwitchObj);
	pView->pCLSIView->Dispatch("newaction "+TheTrack+" "+__TriggerObj);
	pView->pCLSIView->Dispatch("newaction "+TheTrack+" "+__PM_SpectrumObj);

	return answer;
}
  
CString _deltrack(CMCPforNTView* pView,...)
{
	CString answer = "Track not found";

	int	NumArgs;
	char* StrTrack;

	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs		= va_arg(arg_ptr,int);
	StrTrack	= va_arg(arg_ptr,char*);

	TrackObject* pTrack = GetTrack(pView,StrTrack);

	if(pTrack == NULL) return answer;

	POSITION pos;
	ActionObject* pActionObj;
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	pApp->EmptyActionList();

	if (!pView->GetDocument()->ActionObjList.IsEmpty())
		{	
			pos = pView->GetDocument()->ActionObjList.Find(pTrack);
			pActionObj = (ActionObject*)pView->GetDocument()->ActionObjList.GetAt(pos);

			if (pActionObj!= NULL)
			{

				pView->ActionPointerList.RemoveAll();
				do
				{
					
					pActionObj = (ActionObject*)pView->GetDocument()->ActionObjList.GetNext(pos);
					pView->ActionPointerList.AddTail(pActionObj);
					pApp->CopyActionList.AddTail(pView->CopyActionObject(pActionObj));
					if(pos!=NULL)pActionObj = (ActionObject*)pView->GetDocument()->ActionObjList.GetAt(pos);
				}while (pActionObj->GetName()!=_TrackObj&&pos!=NULL);
				pos = pView->ActionPointerList.GetHeadPosition();
				for( pos = pView->ActionPointerList.GetHeadPosition(); pos != NULL; )
				{
					pView->Remove((ActionObject*)pView->ActionPointerList.GetNext( pos ));
				}
				pView->RearrangeTrackNum();
			}
		}

	pView->GetDocument()->SetModifiedFlag(TRUE);
	pView->GetDocument()->UpdateAllViews(NULL,0,NULL);
	return "";
}

CString _copytrack(CMCPforNTView* pView,...)
{
	CString answer = "Track not found";

	int	NumArgs;
	char* StrTrack;

	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs		= va_arg(arg_ptr,int);
	StrTrack	= va_arg(arg_ptr,char*);

	TrackObject* pTrack = GetTrack(pView,StrTrack);

	if(pTrack == NULL) return answer;

	POSITION pos;
	ActionObject* pActionObj;

	POSITION Insertpos;
	ActionObject* pActionObject;
	ActionObject* pInsertAction;
	TrackObject* pInsertTrack;
	TrackObject* pLastTrack;

	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	pApp->EmptyActionList();

	if (!pView->GetDocument()->ActionObjList.IsEmpty())
		{	
			pos = pView->GetDocument()->ActionObjList.Find(pTrack);
			pActionObj = (ActionObject*)pView->GetDocument()->ActionObjList.GetAt(pos);

			if (pActionObj!= NULL)
			{

				pView->ActionPointerList.RemoveAll();
				do
				{
					pActionObj = (ActionObject*)pView->GetDocument()->ActionObjList.GetNext(pos);
					pView->ActionPointerList.AddTail(pActionObj);
					pApp->CopyActionList.AddTail(pView->CopyActionObject(pActionObj));
					if(pos!=NULL)pActionObj = (ActionObject*)pView->GetDocument()->ActionObjList.GetAt(pos);

				}while (pActionObj->GetName()!=_TrackObj&&pos!=NULL);
			}
		}
	
	

	if (!pApp->CopyActionList.IsEmpty())
		{	
			pos = pApp->CopyActionList.GetHeadPosition();
			Insertpos = pView->GetDocument()->ActionObjList.GetTailPosition();
			pActionObject = (ActionObject*)pView->GetDocument()->ActionObjList.GetAt(Insertpos);
			pLastTrack = pActionObject->pTrack;
			do
			{
				pActionObject = (ActionObject*)pApp->CopyActionList.GetAt(pos);

				pInsertAction = (ActionObject*)pView->CopyActionObject(pActionObject);
				
				if(pInsertAction->GetName()==_TrackObj)
				{
					TrackObject* pThisTrack = (TrackObject*)pInsertAction;
					pInsertTrack = pThisTrack;
				}
				pInsertAction->pDocument = pView->GetDocument();
				pInsertAction->pTrack = pInsertTrack;

				Insertpos = pView->GetDocument()->ActionObjList.InsertAfter(Insertpos,pInsertAction);
				
				ActionObject* pActionObj = (ActionObject*)pApp->CopyActionList.GetNext(pos);
			}while (pos!=NULL);
		}

	pView->RearrangeTrackNum();
	pView->GetDocument()->SetModifiedFlag(TRUE);
	pView->GetDocument()->UpdateAllViews(NULL,0,NULL);




	return "";
}

CString _newaction(CMCPforNTView* pView,...)
{
	CString answer = "Track not found";
	CString UsageString = "usage : newaction TN \r\n"; 
			UsageString = UsageString + __TriggerObj				+"\r\n"; 
			UsageString = UsageString + __HP_VoltageSweepObj		+"\r\n";  
			UsageString = UsageString + __HP3325B_FrequencySweepObj	+"\r\n";
			UsageString = UsageString + __RS_FrequencySweepObj		+"\r\n";
			UsageString = UsageString + __PM_SpectrumObj			+"\r\n";
			UsageString = UsageString +	__PM_ArithObj	   			+"\r\n";
			UsageString = UsageString +	__PremaVoltageObj			+"\r\n";
			UsageString = UsageString +	__FlukeVoltageObj			+"\r\n";
			UsageString = UsageString +	__IsoldeNetVoltageObj		+"\r\n";
			UsageString = UsageString +	__KepcoEichungVoltageObj	+"\r\n";
			UsageString = UsageString +	__Keithley_VoltageMeasObj	+"\r\n";
			UsageString = UsageString +	__MassAndFlukeSwitchObj		+"\r\n";
			UsageString = UsageString +	__MassSwitchObj				+"\r\n";
			UsageString = UsageString +	__FlukeSwitchObj			+"\r\n";
			UsageString = UsageString +	__AlexMotorObj				+"\r\n";
			UsageString = UsageString +	__GPIBReaderObj				+"\r\n";
			UsageString = UsageString +	__FieldReaderObj			+"\r\n";
			UsageString = UsageString +	__TempReaderObj				+"\r\n";
			UsageString = UsageString +	__TempWriterObj				+"\r\n";
			UsageString = UsageString +	__OutBitObj					+"\r\n";
			UsageString = UsageString +	__FieldWriterObj			+"\r\n";
			UsageString = UsageString +	__GPIBWriterObj				+"\r\n";
			UsageString = UsageString +	__FDAC_VoltageSweepObj;		

	int	NumArgs;
	char* StrTrack;
	char* TheActionObj;
	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs		= va_arg(arg_ptr,int);
	StrTrack	= va_arg(arg_ptr,char*);
	if(NumArgs!=2)
	{
		return UsageString;
	}
	TheActionObj = va_arg(arg_ptr,char*);

	TrackObject* pTrack = GetTrack(pView,StrTrack);

	if(pTrack == NULL) return answer;

	if (!_stricmp(TheActionObj,__TriggerObj))
	{
		ActionObject* pAction = new TriggerObj();
		pAction->pTrack = pTrack;
		pAction->pDocument = pView->GetDocument();
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";
	}

	if (!_stricmp(TheActionObj,__HP_VoltageSweepObj)) 
	{
		ActionObject* pAction = new HP_VoltageSweepObj();
		pAction->pTrack = pTrack;
		pAction->pDocument = pView->GetDocument();
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";

	}

	if (!_stricmp(TheActionObj,__HP3325B_FrequencySweepObj))
	{
		ActionObject* pAction = new HP3325B_FrequencySweepObj();
		pAction->pTrack = pTrack;
		pAction->pDocument = pView->GetDocument();
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";

	}

	if (!_stricmp(TheActionObj,__RS_FrequencySweepObj))
	{
		ActionObject* pAction = new RS_FrequencySweepObj();
		pAction->pTrack = pTrack;
		pAction->pDocument = pView->GetDocument();
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";

	}

	if (!_stricmp(TheActionObj,__PM_SpectrumObj))
	{
		PM_SpectrumObj* pAction = new PM_SpectrumObj();
		pAction->pTrack = pTrack;
		pAction->SubRegNo = pAction->pTrack->ScalerCounter++;
		pAction->pDocument = pView->GetDocument();
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";

	}

	if (!_stricmp(TheActionObj,__PM_ArithObj))
	{
		PM_SpectrumObj* pAction = new PM_SpectrumObj();
		pAction->pTrack = pTrack;
		pAction->pDocument = pView->GetDocument();
		pAction->scan_data_op = "0";
		pAction->DoString = "0";
		pAction->SubRegNo = pAction->pTrack->ArithCounter++;
		pAction->ShortName = __PM_ArithObj;
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";

	}

	if (!_stricmp(TheActionObj,__PremaVoltageObj))
	{
		ActionObject* pAction = new PremaVoltageObj();
		pAction->pTrack = pTrack;
		pAction->pDocument = pView->GetDocument();
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";

	}

	if (!_stricmp(TheActionObj,__FlukeVoltageObj))
	{
		ActionObject* pAction = new FlukeVoltageObj();
		pAction->pTrack = pTrack;
		pAction->pDocument = pView->GetDocument();
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";

	}

	if (!_stricmp(TheActionObj,__IsoldeNetVoltageObj))
	{
		ActionObject* pAction = new IsoldeNetVoltageObj();
		pAction->pTrack = pTrack;
		pAction->pDocument = pView->GetDocument();
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";

	}

	if (!_stricmp(TheActionObj,__KepcoEichungVoltageObj))
	{
		ActionObject* pAction = new KepcoEichungVoltageObj();
		pAction->pTrack = pTrack;
		pAction->pDocument = pView->GetDocument();
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";

	}

	if (!_stricmp(TheActionObj,__Keithley_VoltageMeasObj))
	{
		ActionObject* pAction = new Keithley_VoltageMeasObj();
		pAction->pTrack = pTrack;
		pAction->pDocument = pView->GetDocument();
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";

	}

	if (!_stricmp(TheActionObj,__MassAndFlukeSwitchObj))
	{
		ActionObject* pAction = new MassAndFlukeSwitchObj();
		pAction->pTrack = pTrack;
		pAction->pDocument = pView->GetDocument();
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";

	}

	if (!_stricmp(TheActionObj,__MassSwitchObj))
	{
		ActionObject* pAction = new MassSwitchObj();
		pAction->pTrack = pTrack;
		pAction->pDocument = pView->GetDocument();
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";

	}

	if (!_stricmp(TheActionObj,__FlukeSwitchObj))
	{
		ActionObject* pAction = new FlukeSwitchObj();
		pAction->pTrack = pTrack;
		pAction->pDocument = pView->GetDocument();
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";

	}

	if (!_stricmp(TheActionObj,__AlexMotorObj))
	{
		ActionObject* pAction = new AlexMotorObj();
		pAction->pTrack = pTrack;
		pAction->pDocument = pView->GetDocument();
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";

	}

	if (!_stricmp(TheActionObj,__GPIBReaderObj))
	{
		ActionObject* pAction = new GPIBReaderObj();
		pAction->pTrack = pTrack;
		pAction->pDocument = pView->GetDocument();
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";

	}

	if (!_stricmp(TheActionObj,__FieldReaderObj))
	{
		ActionObject* pAction = new FieldReaderObj();
		pAction->pTrack = pTrack;
		pAction->pDocument = pView->GetDocument();
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";

	}

	if (!_stricmp(TheActionObj,__TempReaderObj))
	{
		ActionObject* pAction = new TempReaderObj();
		pAction->pTrack = pTrack;
		pAction->pDocument = pView->GetDocument();
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";

	}

	if (!_stricmp(TheActionObj,__TempWriterObj))
	{
		ActionObject* pAction = new TempWriterObj();
		pAction->pTrack = pTrack;
		pAction->pDocument = pView->GetDocument();
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";

	}

	if (!_stricmp(TheActionObj,__OutBitObj))
	{
		ActionObject* pAction = new OutBitObj();
		pAction->pTrack = pTrack;
		pAction->pDocument = pView->GetDocument();
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";

	}

	if (!_stricmp(TheActionObj,__FieldWriterObj))
	{
		ActionObject* pAction = new FieldWriterObj();
		pAction->pTrack = pTrack;
		pAction->pDocument = pView->GetDocument();
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";

	}

	if (!_stricmp(TheActionObj,__GPIBWriterObj))
	{
		ActionObject* pAction = new GPIBWriterObj();
		pAction->pTrack = pTrack;
		pAction->pDocument = pView->GetDocument();
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";

	}

	if (!_stricmp(TheActionObj,__FDAC_VoltageSweepObj))
	{
		ActionObject* pAction = new FDAC_VoltageSweepObj();
		pAction->pTrack = pTrack;
		pAction->pDocument = pView->GetDocument();
		pView->GetDocument()->ActionObjList.InsertAfter(GetEndOfTrack(pView,pTrack),pAction);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";

	}

	return "Unknown ActionObject";
}

CString _moveaction(CMCPforNTView* pView,...)
{
	CString answer = "Track not found";

	ActionObject* ActionFrom;
	ActionObject* ActionTo;

	int	NumArgs;
	int from;
	int to;

	char* StrTrack;
	char* Strfrom;
	char* Strto;
	
	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs		= va_arg(arg_ptr,int);
	StrTrack	= va_arg(arg_ptr,char*);
	Strfrom		= va_arg(arg_ptr,char*);
	Strto		= va_arg(arg_ptr,char*);
	
	from    = atoi(Strfrom);
	to      = atoi(Strto);

	if(from==to) return "Source = destination";

	TrackObject* pTrack = GetTrack(pView,StrTrack);

	if(pTrack == NULL) return answer;
	
	ActionFrom	= GetActionObjByIndex(pView,pTrack,from);
	ActionTo	= GetActionObjByIndex(pView,pTrack,to);

	if(ActionFrom!=NULL&&ActionTo!=NULL)
	{
		if(ActionFrom==ActionTo) return "Source = destination";
		pView->GetDocument()->ActionObjList.RemoveAt(pView->GetDocument()->ActionObjList.Find(ActionFrom));
		pView->GetDocument()->ActionObjList.InsertBefore(pView->GetDocument()->ActionObjList.Find(ActionTo),ActionFrom);
		pView->GetDocument()->SetModifiedFlag(TRUE);
		pView->GetDocument()->UpdateAllViews(NULL,0,NULL);

		return "";
	}
	return "Position not found";
}

CString _configaction(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _confighwaction(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _configtrack(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _updownsweep(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _noupdownsweep(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _configdata(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _movetrack(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _print(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _channels(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _beamgate(CMCPforNTView* pView,...)
{
	CString answer = "sorry ?";
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();

	int	NumArgs;
	char* StrBeamgate;

	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs	= va_arg(arg_ptr,int);

	StrBeamgate = va_arg(arg_ptr,char*);

	if(0==_stricmp(StrBeamgate,"off"))
	{
		OffBit(0,14);
		answer = "Beamgate now closed"; 
	}

	if(0==_stricmp(StrBeamgate,"on"))
	{
		OnBit(0,14);
		answer = "Beamgate now open"; 
	}

	return answer;
}

CString _refbit(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _warnprotons(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _readprotons(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _badpulses(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _redostep(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _newredostepmode(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _slowreset(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _rampreset(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _configreset(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _uselp(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _usedd(CMCPforNTView* pView,...)
{
	CString answer = "No more needed...";
	return answer;
}

CString _useirqdd(CMCPforNTView* pView,...)
{
	CString answer = "No more needed...";
	return answer;
}

CString _whatdd(CMCPforNTView* pView,...)
{
	CString answer = "No more needed...";
	return answer;
}

CString _probedd(CMCPforNTView* pView,...)
{
	CString answer = "No more needed...";
	return answer;
}

CString _irqdd(CMCPforNTView* pView,...)
{
	CString answer = "No more needed...";
	return answer;
}

CString _irqokdd(CMCPforNTView* pView,...)
{
	CString answer = "No more needed...";
	return answer;
}

CString _resetdd(CMCPforNTView* pView,...)
{
	CString answer = "No more needed...";
	return answer;
}

CString _nextirqdd(CMCPforNTView* pView,...)
{
	CString answer = "No more needed...";
	return answer;
}

CString _protonlimit(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _ignoreprotons(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _lookforprotons(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _closebeamgate(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _opengate(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _closegate(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _radioactive(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _reference(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _motor(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _motorcm(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _motornew(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _setmotorcount(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _getmotorcount(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _fastmotor(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _normalmotor(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _motordelay(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _stepdelay(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _accelerate(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _accsteps(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _steps(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _distance(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _stepmode(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _distmode(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _direction(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _minmotorcount(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _maxmotorcount(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _settaperad(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _settapeth(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _break(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _bothmotors(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _motorclicktime(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _motorwaitclicks(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _motorwaitfactor(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _kepcofactor(CMCPforNTView* pView,...)
{
	CString answer = "";
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();

	int	NumArgs;
	char* StrKepcoFactor;
	double KepcoFactor;

	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs	= va_arg(arg_ptr,int);
	if(NumArgs == 1)
	{
		StrKepcoFactor = va_arg(arg_ptr,char*);
		KepcoFactor = atof(StrKepcoFactor);
	}
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	if(NumArgs == 0) answer.Format("%.8g",pApp->GlobalKepcoFactor);
	if(NumArgs == 1) 
	{
		
		pApp->GlobalKepcoFactor = KepcoFactor;
		//pView->GetDocument()->UpdateAllViews(NULL,0,NULL);
		//pView->GetDocument()->SetModifiedFlag(TRUE);
	}
	return answer;
}

CString _asymfactor(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _warndelay(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _regression(CMCPforNTView* pView,...)
{
	CString answer = "Please use Minuit-Interface";
	return answer;
}

CString _dispkepco(CMCPforNTView* pView,...)
{
	CString answer = "Track not found";


	int	NumArgs;
	
	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs		= va_arg(arg_ptr,int);

	KepcoEichungVoltageObj* pActionObj = (KepcoEichungVoltageObj*)GetActionObj(pView,_KepcoEichungVoltageObj);
	if(pActionObj == NULL) 
	{
		return "Object not found";
	}

	pActionObj->DoDoubleClickAction();
	pView->pCLSIView->GetParent()->BringWindowToTop();
	pView->pCLSIView->SetFocus();
	return "";
}


CString _dispdelta(CMCPforNTView* pView,...)
{
	CString answer = "Please use Minuit-Interface";
	return answer;
}

CString _dispisolde(CMCPforNTView* pView,...)
{
	CString answer = "Track not found";


	int	NumArgs;
	
	va_list arg_ptr;
	va_start(arg_ptr,pView);
	NumArgs		= va_arg(arg_ptr,int);

	IsoldeNetVoltageObj* pActionObj = (IsoldeNetVoltageObj*)GetActionObj(pView,_IsoldeNetVoltageObj);
	if(pActionObj == NULL) 
	{
		return "Object not found";
	}

	pActionObj->DoDoubleClickAction();
	pView->pCLSIView->GetParent()->BringWindowToTop();
	pView->pCLSIView->SetFocus();
	return "";
}

CString _protondelay(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _parttrigger(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _triggerdelay(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _range(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _measperstep(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _measatscan(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _premafactor(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _flukedelay(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _isoldedelay(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _computerdelay(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _setmultino(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _changescbit(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _clearscbit(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _halt(CMCPforNTView* pView,...)
{
	CString answer;
	CMCPforNTApp* pMyApp = (CMCPforNTApp*)AfxGetApp();
	if(pView->GetDocument()->MeasurementRunning&&pMyApp->GlobalMeasurementRunning)
	{
		pView->HALTNOW = TRUE;
		answer = "Measurement will be halted after scan";
	}
	else
	{
		answer = "No measurement running for this LSI-Box";
	}
	return answer;
}

CString _kill(CMCPforNTView* pView,...)
{
	CString answer;
	CMCPforNTApp* pMyApp = (CMCPforNTApp*)AfxGetApp();
	if(pView->GetDocument()->MeasurementRunning&&pMyApp->GlobalMeasurementRunning)
	{
		pView->OnKill();
		answer = "Measurement killed";
	}
	else
	{
		answer = "No measurement running for this LSI-Box";
	}
	return answer;
}

CString _running(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _closemonitor(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _closestatus(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _mot(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _grabframe(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _dispframe(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _setframename(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _saveframe(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _loadframe(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _system(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _setccdmode(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _setccdarea(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _cdisp(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _camacfout(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _camacfin(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _usestrobe(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _load(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _domath(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _f(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

CString _b(CMCPforNTView* pView,...)
{
	CString answer = "Not yet implemented...";
	return answer;
}

TrackObject* GetTrack(CMCPforNTView* pView,char* TheTrack)
{
	TrackObject* pTrack;
	USHORT Track;
	BOOL Found = FALSE;
	POSITION pos;
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();
	CString TheObject;
	Track = atoi(TheTrack);
	if(!pDocument->ActionObjList.IsEmpty())
	{
		ActionObject* pActionObj = (ActionObject*)pDocument->ActionObjList.GetHead();
		pos = pDocument->ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)pDocument->ActionObjList.GetNext(pos);
			if (pActionObj!=NULL)
			{
				if(pActionObj->GetName()==_TrackObj)
				{
					pTrack = (TrackObject*) pActionObj;
					if (pTrack->MyPosition == Track)
					{
						Found = TRUE;
					}
								
				}
			}
		}
		while (pos!=NULL&&!Found);
	}
	if(Found) return pTrack;
	return NULL;
}

ActionObject* GetActionObj(CMCPforNTView* pView,CString TheObject)
{
	BOOL Found = FALSE;
	POSITION pos;
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();
	ActionObject* pActionObj;

	if(!pDocument->ActionObjList.IsEmpty())
	{
		pos = pDocument->ActionObjList.GetHeadPosition();
		if(pos!=NULL)
		{
			do
			{
				pActionObj = (ActionObject*)pDocument->ActionObjList.GetNext(pos);
				if (pActionObj!=NULL)
				{
					if(pActionObj->Name==TheObject)
					{
						Found = TRUE;				
					}
				}
			}
			while (pos!=NULL&&!Found);
			if(Found) return pActionObj;
		}
	}
	return NULL;
}


ActionObject* GetActionObj(CMCPforNTView* pView,TrackObject* TheTrack,CString TheObject)
{
	BOOL Found = FALSE;
	POSITION pos;
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();
	ActionObject* pActionObj;

	if(!pDocument->ActionObjList.IsEmpty())
	{
		pos = pDocument->ActionObjList.Find(TheTrack,NULL);
		if(pos!=NULL)
		{
			do
			{
				pActionObj = (ActionObject*)pDocument->ActionObjList.GetNext(pos);
				if (pActionObj!=NULL)
				{
					if(pActionObj->Name==TheObject)
					{
						Found = TRUE;				
					}
				}
			}
			while (pos!=NULL&&!Found);
			if(Found&&pActionObj->pTrack==TheTrack) return pActionObj;
		}
	}
	return NULL;
}


ActionObject* GetActionObjByIndex(CMCPforNTView* pView,TrackObject* TheTrack,USHORT TheIndex)
{
	ActionObject* pActionObj = NULL;
	POSITION pos;
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();
	USHORT i;
	if(!pDocument->ActionObjList.IsEmpty())
	{
		
		pos = pDocument->ActionObjList.Find(TheTrack);
		pActionObj = (ActionObject*)pDocument->ActionObjList.GetAt(pos);
		for(i=0;i<=TheIndex&&pos!=0;i++)
		{
			pActionObj = (ActionObject*)pDocument->ActionObjList.GetNext(pos);
			if(i!=TheIndex)pActionObj = NULL;
		}
	}
	return pActionObj;
}


POSITION GetEndOfTrack(CMCPforNTView* pView,TrackObject* TheTrack)
{
	ActionObject* pActionObj;
	POSITION pos;
	POSITION Insert;
	CMCPforNTDoc* pDocument = (CMCPforNTDoc*)pView->GetDocument();
	if(!pDocument->ActionObjList.IsEmpty())
	{
		pos = pDocument->ActionObjList.Find(TheTrack);
		do
		{
			Insert = pos;
			pActionObj = (ActionObject*)pDocument->ActionObjList.GetNext(pos);
			if(pActionObj->pTrack!=TheTrack)
			{
				return Insert;
			}

		}while(pos!=NULL);

		return pDocument->ActionObjList.GetTailPosition();
	}
	return NULL;
}

void CalculateRealStartStop(double* start,double* stop,double* step,USHORT channels,double resolution)
{
	*step		= (*stop-*start)/(channels-1);
	if(*step!=0)*step = (*step/fabs(*step))*resolution*((int)(0.49+fabs((*step)/resolution)));
	*stop		= *start+(channels-1)*(*step);
}

void CLSIView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRichEditView::OnLButtonDblClk(nFlags, point);
	printf(GetRichEditCtrl().GetSelText());
}
