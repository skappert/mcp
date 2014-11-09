// ActionObjects.cpp : implementation of the ActionObject classes
//

#include "stdafx.h"
#include "MCP for NT.h"
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

#include "PropShtam.h"
#include "PropShtAS.h"
#include "PropShtfs.h"
#include "PropShtHP.h"
#include "PropShtHV.h"
#include "PropShtLV.h"
#include "PropShtFV.h"
#include "PropShtMF.h"
#include "PropShtPM.h"
#include "PropShtRS.h"
#include "PropShtTI.h"
#include "PropShtTR.h"
#include "PropShtMS.h"
#include "PropShtPR.h"
#include "PropShtBO.h"
#include "PropShtFR.h"
#include "PropShtFW.h"
#include "PropShtGR.h"
#include "PropShtGW.h"
#include "PropShtKR.h"
#include "PropShtKW.h"
#include "PropShtSR.h"

#include "Configure.h"

#include "StepTimer.h"
#include "TimerDelay.h"
#include "TriggerPartSweep.h"
#include "FastTimer.h"
#include "MeasThread.h"
#include "ComputerDelay.h"

#include "HardwareConfig.h"

#include "MCP for NTView.h"

#include "FileIO.h"

#include "eval.h"

#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


double PowerOf10(int Exp)
{
	double Value=1.0;

	if (Exp>0)
	{ 
		for (int i=1; i<=Exp; i++) 
		{ 
			Value = Value*10.0; 
		}; 
	}
	else
	{ 
		for (int i=-1; i>=Exp; i--) 
		{ 
			Value = Value*0.1; 
		}; 
	};
    return Value;
}

double PowerOf2(int Exp)
{
	double Value=1.0;

	if (Exp>0)
	{ 
		for (int i=1; i<=Exp; i++) 
		{ 
			Value = Value*2; 
		}; 
	}
	else
	{ 
		for (int i=-1; i>=Exp; i--) 
		{ 
			Value = Value*0.5; 
		}; 
	};
	return Value;
}

double BCDDigit(unsigned int Code)
// Bit no: 0 1 2 3
// Value : 1 2 4 8 -> Sum=Digit
// returns -1.0 if Code is not a correct BCD-Digit
{
	double Digit=0.0;

	for (int i=0; i<4; i++)
	{ 
		if (Code & (1 << i)) 
		{ 
			Digit = Digit+PowerOf2(i); 
		}; 
	};
    if ((int)Digit>9) 
	{ 
		return -1.0; 
	};
    return Digit;
}

double CalcStop(double start, double stop, USHORT channels, double resolution)
{
	double step;
	if(start == stop)return stop;
	step		= (stop-start)/(channels-1);
	step		= (step/fabs(step))*resolution*((int)(0.49+fabs(step/resolution)));
	if (step == 0) step = resolution;
	stop		= start+(channels-1)*step;
	return stop;
}

double CalcStep(double start, double stop, USHORT channels, double resolution)
{
	double step;
	if(start == stop)return 0;
	step		= (stop-start)/(channels-1);
	step = (step/fabs(step))*resolution*((int)(0.49+fabs(step/resolution)));
	return step;
}

double CalcValue(double start, double stop, USHORT channels, USHORT pos, double resolution)
{
	double step;
	double value;
	if(start == stop)return start;
	step		= (stop-start)/(channels-1);
	value = (double)(pos-1.0)*step+start+resolution/2;
	return value;
}

void CalcStartStop(double* start, double* stop, USHORT channels, double resolution, double newmin, double newmax)
{
	double stepval;
	double startval = (*stop-*start)*(double)(newmin-1.0)/(double)(channels-1)+*start;
	double stopval	= (*stop-*start)*(double)(newmax-1.0)/(double)(channels-1)+*start;

	startval	= resolution*((long)(startval/resolution));
	stopval		= resolution*((long)(stopval/resolution));

	if(startval!=stopval)
	{
		stepval		= (stopval-startval)/(channels-1);
		stepval		= (stepval/fabs(stepval))*resolution*((int)(0.49+fabs(stepval/resolution)));
		if (stepval == 0) stepval = resolution;
		stopval		= startval+(channels-1)*stepval;
	}
	*start = startval;
	*stop  = stopval;

}

void CalcCenter(double* start, double* stop, USHORT channels, double resolution,USHORT step)
{
	CString data;
	double stepval		= (*stop-*start)/(double)(channels-1);
	double centerval	= (double)(step)*(*stop-*start)/(double)(channels-1)+*start;


	data.Format("center = %g, step = %g",centerval,stepval);
	AfxMessageBox(data,MB_OK,0);

	if(stepval!=0)
	{
		stepval		= (stepval/fabs(stepval))*resolution*((int)(0.49+fabs(stepval/resolution)));
		if(stepval == 0) stepval = resolution;

		if(2*((int)channels/2)!=channels)
		{
			*start	= centerval-((channels-1)/2)*stepval;
			*stop	= centerval+((channels-1)/2)*stepval;
		}
		else
		{
			*start	= centerval-((channels-1)/2)*stepval;
			*stop	= centerval+(1+((channels-1)/2))*stepval;
		}
	}
}

/****************   Methods for Class ActionObject  ***********************/

void ActionObject::DoDoubleClickAction(void)
{};
void ActionObject::DoSummingAction(ActionObject* pDestination)
{};
void ActionObject::UpdateDisplay(void)
{};
void ActionObject::Load(CArchive& ar)
{};
void ActionObject::Save(CArchive& ar)
{};
void ActionObject::MeasurementBeginAction(BOOL RUNMODE)
{};
void ActionObject::MeasurementEndAction(void)
{};
void ActionObject::TrackBeginAction(USHORT track)
{};
void ActionObject::TrackEndAction(USHORT track,USHORT scansdone)
{};
void ActionObject::TrackStepAction(USHORT step, USHORT track, USHORT scan)
{};
void ActionObject::DoConfigureAction(void)
{
	DoDoubleClickAction();
};
void ActionObject::CopyObject(ActionObject* pSource)
{};
void ActionObject::UpdateData(void)
{};
void ActionObject::TakeData(BOOL RUNMODE, USHORT scan, USHORT channel, ULONG TheData)
{};
double ActionObject::GetY(USHORT channel)
{
	return (double)0;
}
double ActionObject::GetYErr(USHORT channel)
{
	return (double)0;
}
double ActionObject::EstimateY(USHORT channel, USHORT scan)
{
	return (double)0;
}

double ActionObject::EstimateYErr(USHORT channel, USHORT scan)
{
	return (double)0;
}

void ActionObject::SetCenter(USHORT step)
{
}

void ActionObject::SetMinMax(double minstep,double maxstep)
{
}

void ActionObject::UpdateNoScan()
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	if(pApp->CamacOn)MeasurementEndAction();
}

ActionObject::~ActionObject()
{
	if(pDataView!=NULL)pDataView->GetParent()->DestroyWindow();
	Name.Empty();
	ShortName.Empty();
	DoString.Empty();
	Isotope.Empty();
	scan_data_op.Empty();
	scan_error_op.Empty();
}

/****************   Methods for Class TrackObject  ***********************/

void TrackObject::DoSummingAction(ActionObject* pDestination)
{
	TrackObject* pTrack = (TrackObject*)pDestination;
	switch(pDocument->AddMe)
	{
	case 0:
		pTrack->RealScans	= RealScans;
		break;
	case 1:
		pTrack->RealScans	= RealScans;
		break;
	case 2:
		pTrack->RealScans	= pTrack->RealScans+RealScans;
		break;
	case 3:
		pTrack->RealScans	= pTrack->RealScans-RealScans;
		break;
	}
	if(pTrack->RealScans<=1)pTrack->RealScans = 1;
}


void TrackObject::DoDoubleClickAction(void)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",MyPosition);
	CPropShtTR propSheet(ShortName+" ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);

	propSheet.m_page1.m_isotope = Isotope;
	propSheet.m_page1.m_numscans = NoOfScans;
	propSheet.m_page1.m_realscans = RealScans;
	propSheet.m_page1.m_numsteps = Channels;
	propSheet.m_page1.m_protonlimit = ProtonLimit;

	propSheet.m_page1.m_element = pDocument->TheElement;
	propSheet.m_page1.m_wavenumber = pDocument->WaveNumber;
	propSheet.m_page1.m_cycles = pDocument->NoOfCycles;

	propSheet.m_page1.m_noredo = FALSE;
	propSheet.m_page1.m_redostop = FALSE;
	propSheet.m_page1.m_redowait = FALSE;
	propSheet.m_page1.m_redowaitredo = FALSE;
	propSheet.m_page1.m_kepcofactor = pApp->GlobalKepcoFactor;

	if(ProtonAction	== 0)propSheet.m_page1.m_noredo = TRUE;
	if(ProtonAction	== 1)propSheet.m_page1.m_redostop = TRUE;
	if(ProtonAction	== 2)propSheet.m_page1.m_redowait = TRUE;
	if(ProtonAction	== 3)propSheet.m_page1.m_redowaitredo = TRUE;

	propSheet.m_page3.m_slot= Slot;
	propSheet.m_page3.m_subaddress = SubAddress;

	if(IDOK == propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			Isotope		= propSheet.m_page1.m_isotope;
			NoOfScans	= propSheet.m_page1.m_numscans;
			Channels	= propSheet.m_page1.m_numsteps;
			ProtonLimit	= propSheet.m_page1.m_protonlimit;
			if(propSheet.m_page1.m_noredo)ProtonAction		= 0;
			if(propSheet.m_page1.m_redostop)ProtonAction	= 1;
			if(propSheet.m_page1.m_redowait)ProtonAction	= 2;
			if(propSheet.m_page1.m_redowaitredo)ProtonAction= 3;

			pDocument->TheElement = propSheet.m_page1.m_element;
			pDocument->WaveNumber = propSheet.m_page1.m_wavenumber;
			pDocument->NoOfCycles = propSheet.m_page1.m_cycles;

			pApp->GlobalKepcoFactor = propSheet.m_page1.m_kepcofactor;
			Slot = propSheet.m_page3.m_slot;
			SubAddress = propSheet.m_page3.m_subaddress;
			pDocument->UpdateAllData(pTrack->MyPosition);
			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
		}
		else Beep(1000,100);
	}
}

void TrackObject::CopyObject(ActionObject* pSource)
{
	TrackObject* pActionObject = (TrackObject*) pSource;
	
	pDocument		= pActionObject->pDocument;
	pTrack			= NULL;	
	HardwareReady	= pActionObject->HardwareReady;

	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;
	
	MyPosition		= pActionObject->MyPosition;
	NoOfScans		= pActionObject->NoOfScans;
	RealScans		= 0;
	Channels		= pActionObject->Channels;
	RepeatSteps		= pActionObject->RepeatSteps;	
}

void TrackObject::Load(CArchive& ar)
{
}

CString TrackObject::GetInfo(void)
{
	CString answer;
	answer.Format("No. %u: %s; %u of %u Scans done; %u Ch",
					MyPosition,Isotope,RealScans,NoOfScans,Channels);
	return answer;
}

void TrackObject::Save(CArchive& ar)
{
	if(MyPosition > 1)
	{
		WriteStructEnd(ar);
		WriteLine(ar);
		WriteStructEnd(ar);
		WriteSeparator(ar);
	}
	WriteStructBegin(ar);
	WriteString(ar,Isotope);
	WriteSeparator(ar);
	WriteFloat(ar,NoOfScans);
	WriteSeparator(ar);
	WriteFloat(ar,RealScans);
	WriteSeparator(ar);
	WriteFloat(ar,Channels);
	WriteSeparator(ar);
	WriteFloat(ar,Resolution2);
	WriteSeparator(ar);
	WriteFloat(ar,RepeatSteps);
	WriteSeparator(ar);
	WriteFloat(ar,UpDownSweep);
	WriteSeparator(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
}

void TrackObject::TrackStepAction(USHORT step, USHORT track, USHORT scan)
{
}

void TrackObject::MeasurementBeginAction(BOOL RUNMODE)
{
	if(RUNMODE==ERGO)RealScans = 0;
}

void TrackObject::MeasurementEndAction(void)
{
}

void TrackObject::TrackBeginAction(USHORT track)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	if(pApp->pMonitorView!=NULL)pApp->pMonitorView->DeleteAllViews();
}

void TrackObject::TrackEndAction(USHORT track,USHORT scansdone)
{
	RealScans = RealScans+scansdone;
}

/****************   Methods for Class HP_VoltageSweepObj  ***********************/

void HP_VoltageSweepObj::UpdateData(void)
{
	Sweep_Stop = CalcStop(Sweep_Start, Sweep_Stop, pTrack->Channels, Resolution);
}

void HP_VoltageSweepObj::SetMinMax(double minstep,double maxstep)
{
	CalcStartStop(&Sweep_Start, &Sweep_Stop, pTrack->Channels, Resolution, minstep, maxstep);
}

void HP_VoltageSweepObj::SetCenter(USHORT step)
{
	CalcCenter(&Sweep_Start, &Sweep_Stop, pTrack->Channels, Resolution, step);
}

void HP_VoltageSweepObj::DoDoubleClickAction(void)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);

	CPropShtHV propSheet(ShortName+" in Track ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);
	
	propSheet.m_page1.channels		= pTrack->Channels;
	propSheet.m_page1.resolution	= Resolution;

	propSheet.m_page1.startvolt		= Sweep_Start;
	propSheet.m_page1.stopvolt		= Sweep_Stop;

	propSheet.m_page1.m_slowreset	= SlowReset;
	propSheet.m_page1.m_slowresetch = SlowResetCh;

	NoSweep_Value					= pApp->V0;
	propSheet.m_page2.m_noscanvolt	= NoSweep_Value;

	propSheet.m_page3.m_slot		= Slot;
	propSheet.m_page3.m_subaddress	= SubAddress;

	
	if(IDOK == propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			Sweep_Start = propSheet.m_page1.startvolt;
			Sweep_Stop	= propSheet.m_page1.stopvolt;

			SlowReset	= propSheet.m_page1.m_slowreset;
			SlowResetCh = propSheet.m_page1.m_slowresetch;

			NoSweep_Value = propSheet.m_page2.m_noscanvolt;
			pApp->V0	  = NoSweep_Value;
			Slot		= propSheet.m_page3.m_slot;
			SubAddress	= propSheet.m_page3.m_subaddress;

			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
			UpdateNoScan();
		}
		else Beep(1000,100);
	}
	UpdateNoScan();

}

void HP_VoltageSweepObj::CopyObject(ActionObject* pSource)
{
	HP_VoltageSweepObj* pActionObject = (HP_VoltageSweepObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;


	Sweep_Start			= pActionObject->Sweep_Start;
	Sweep_Step			= pActionObject->Sweep_Step;
	Sweep_Stop			= pActionObject->Sweep_Stop;
	Sweep_Resolution	= pActionObject->Sweep_Resolution;
	Sweep_Amp			= pActionObject->Sweep_Amp;
	Sweep_Mode			= pActionObject->Sweep_Mode;
	Sweep_ModFreq		= pActionObject->Sweep_ModFreq;
	Sweep_ModAmpFM		= pActionObject->Sweep_ModAmpFM;
	Sweep_ModAmpAM		= pActionObject->Sweep_ModAmpAM;
		
	NoSweep_Value		= pActionObject->NoSweep_Value;
	NoSweep_Amp			= pActionObject->NoSweep_Amp;
	NoSweep_Mode		= pActionObject->NoSweep_Mode;
	NoSweep_ModFreq		= pActionObject->NoSweep_ModFreq;
	NoSweep_ModAmpFM	= pActionObject->NoSweep_ModAmpFM;
	NoSweep_ModAmpAM	= pActionObject->NoSweep_ModAmpAM;

	Sweep_ModFreq		= pActionObject->Sweep_ModFreq;
	Sweep_ModAmpFM		= pActionObject->Sweep_ModAmpFM;
	SlowReset			= pActionObject->SlowReset;
	SlowResetCh			= pActionObject->SlowResetCh;
}

CString HP_VoltageSweepObj::GetInfo(void)
{
	CString answer;
	double Step = CalcStep(Sweep_Start,Sweep_Stop,pTrack->Channels,Resolution);
	answer.Format("%gV to %gV step %gV",Sweep_Start,Sweep_Stop,Step);
	return answer;
}

void HP_VoltageSweepObj::Load(CArchive& ar)
{
	Resolution			= HPVoltageResolution;
	Name				= ReadString(ar);
	SlowReset			= FALSE;
	ShortName           = __HP_VoltageSweepObj; 
	ReadSeparator(ar);
	Sweep_Start			= ReadFloat(ar);
	NoSweep_Value		= 0;
	Sweep_Stop			= ReadFloat(ar);
	Sweep_Resolution	= ReadFloat(ar);
	if (pDocument->Version > 6)
	{
		Sweep_Mode		= (int)ReadFloat(ar);
		NoSweep_Mode = Sweep_Mode;
		Sweep_ModFreq		= ReadFloat(ar);
		if (pDocument->Version > 19)
		{
			Sweep_ModAmpFM	= ReadFloat(ar);
		}
		Sweep_Amp	= ReadFloat(ar);
	}
	ReadStructEnd(ar);

}

void HP_VoltageSweepObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);

	WriteFloat(ar,Sweep_Start);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Stop);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Resolution);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Mode);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_ModFreq);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_ModAmpFM);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Amp);

	WriteStructEnd(ar);
	WriteLine(ar);

}

void HP_VoltageSweepObj::MeasurementEndAction(void)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	ULONG Value	= (ULONG)(1000.0*fabs(pApp->V0));
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
     
	if(pApp->V0<0)SendDNAFCamac((ULONG)(~0-1),Slot,1,16);
	else SendDNAFCamac((ULONG)~0,Slot,1,16);
	SendDNAFCamac((ULONG)~Buffer,Slot,0,16);
}

void HP_VoltageSweepObj::ListVoltage(USHORT step,double Voltage)
{
	double Voltage2;

	Voltage2 = Voltage; 
	if(Voltage2<0.0)Voltage2 = -Voltage2;
	ULONG Value	= (ULONG)(1000.0*Voltage2);
	ULONG i,NoDigits,Digit;
    ULONG Buffer;

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
	
	if(step!=1)
	{
		if((Voltage < 0)&&(VoltageOld>=0)) 
		{
			ListDNAFCamac(SENDDNAF,(ULONG)(~0-1),Slot,1,16);
		}
		if((Voltage >= 0)&&(VoltageOld<0)) 
		{
			ListDNAFCamac(SENDDNAF,(ULONG)(~0),Slot,1,16);
		}
	}
	else
	{
		if(Sweep_Start<0)
		{
			ListDNAFCamac(SENDDNAF,(ULONG)(~0-1),Slot,1,16);
		}
		else 
		{
			ListDNAFCamac(SENDDNAF,(ULONG)(~0),Slot,1,16);
		}
	}

	VoltageOld = Voltage;
	ListDNAFCamac(SENDDNAF,(ULONG)(~(Buffer)),Slot,0,16);
}

void HP_VoltageSweepObj::TrackBeginAction(USHORT track)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	ULONG Value	= (ULONG)(1000.0*fabs(pApp->V0));
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
     
	if(pApp->V0<0)SendDNAFCamac((ULONG)(~0-1),Slot,1,16);
	else SendDNAFCamac((ULONG)~0,Slot,1,16);
	SendDNAFCamac((ULONG)~Buffer,Slot,0,16);
}

void HP_VoltageSweepObj::TrackStepAction(USHORT step, USHORT track, USHORT scan)
{
	const int ResetCh = 100;

	double Voltage  = CalcValue(Sweep_Start,Sweep_Stop,pTrack->Channels,step,Resolution);

	ListVoltage(step,Voltage);

	if(SlowReset&&step==pTrack->Channels)
	{
		USHORT i;
		CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
		
		for(i=ResetCh;i>0;i--)
		{
			double Voltage2  = (Sweep_Stop-Sweep_Start)*(double)(i-1.0)/(double)(ResetCh-1)+Sweep_Start;
			ListDelayCamac(pApp->PresetSlot,(USHORT)(1000*SlowResetCh/ResetCh));
			ListVoltage(i,Voltage2);
		}
	}
}

/****************   Methods for Class HP8660B_FrequencySweepObj  ***********************/

void HP8660B_FrequencySweepObj::UpdateData(void)
{
	Sweep_Stop = CalcStop(Sweep_Start, Sweep_Stop, pTrack->Channels, HP8660Resolution);
}

void HP8660B_FrequencySweepObj::SetMinMax(double minstep,double maxstep)
{
	CalcStartStop(&Sweep_Start, &Sweep_Stop, pTrack->Channels, Resolution, minstep, maxstep);
}

void HP8660B_FrequencySweepObj::SetCenter(USHORT step)
{
	CalcCenter(&Sweep_Start, &Sweep_Stop, pTrack->Channels, Resolution, step);
}

void HP8660B_FrequencySweepObj::DoDoubleClickAction(void)
{
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);
	CPropShtHP propSheet(ShortName+" in Track ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);

	propSheet.m_page1.channels		= pTrack->Channels;
	propSheet.m_page1.resolution	= Resolution;

	propSheet.m_page1.startfreq		= Sweep_Start;
	propSheet.m_page1.stopfreq		= Sweep_Stop;
	propSheet.m_page1.ampdb			= Sweep_Amp;

	propSheet.m_page2.ampdb			= NoSweep_Amp;
	propSheet.m_page2.m_noscanfreq	= NoSweep_Value;
	propSheet.m_page3.m_gpib		= Gpib;
	
	if(IDOK == propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			Sweep_Start = propSheet.m_page1.startfreq;
			Sweep_Stop	= propSheet.m_page1.stopfreq;
			Sweep_Amp	=  propSheet.m_page1.ampdb;

			NoSweep_Amp = propSheet.m_page2.ampdb;
			NoSweep_Value = propSheet.m_page2.m_noscanfreq;

			Gpib		= propSheet.m_page3.m_gpib;

			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
			UpdateNoScan();
		}
		else Beep(1000,100);
	}

}

void HP8660B_FrequencySweepObj::CopyObject(ActionObject* pSource)
{
	HP8660B_FrequencySweepObj* pActionObject = (HP8660B_FrequencySweepObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;

	Sweep_Start			= pActionObject->Sweep_Start;
	Sweep_Step			= pActionObject->Sweep_Step;
	Sweep_Stop			= pActionObject->Sweep_Stop;
	Sweep_Resolution	= pActionObject->Sweep_Resolution;
	Sweep_Amp			= pActionObject->Sweep_Amp;
	Sweep_Mode			= pActionObject->Sweep_Mode;
	Sweep_ModFreq		= pActionObject->Sweep_ModFreq;
	Sweep_ModAmpFM		= pActionObject->Sweep_ModAmpFM;
	Sweep_ModAmpAM		= pActionObject->Sweep_ModAmpAM;
		
	NoSweep_Value		= pActionObject->NoSweep_Value;
	NoSweep_Amp			= pActionObject->NoSweep_Amp;
	NoSweep_Mode		= pActionObject->NoSweep_Mode;
	NoSweep_ModFreq		= pActionObject->NoSweep_ModFreq;
	NoSweep_ModAmpFM	= pActionObject->NoSweep_ModAmpFM;
	NoSweep_ModAmpAM	= pActionObject->NoSweep_ModAmpAM;
}

CString HP8660B_FrequencySweepObj::GetInfo(void)
{
	CString answer;
	answer.Format("%gMHz TO %gMHz step %gMHz",Sweep_Start,Sweep_Stop,
		CalcStep(Sweep_Start,Sweep_Stop,pTrack->Channels,Resolution));
	return answer;
}

void HP8660B_FrequencySweepObj::Load(CArchive& ar)
{
	Resolution			= HP8660Resolution;
	Name=ReadString(ar);
	ShortName=__HP8660B_FrequencySweepObj;
	ReadSeparator(ar);
	Sweep_Start      = ReadFloat(ar);
	NoSweep_Value		= Sweep_Start;
	Sweep_Stop       = ReadFloat(ar);
	Sweep_Resolution = ReadFloat(ar);
	if (pDocument->Version > 6)
	{
		Sweep_Mode    = (int)ReadFloat(ar);
		NoSweep_Mode	= Sweep_Mode;
		Sweep_ModFreq = ReadFloat(ar);
		if (pDocument->Version > 19)
		{
			Sweep_ModAmpFM = ReadFloat(ar);
		}
		Sweep_Amp = ReadFloat(ar);
		Sweep_Amp		= Sweep_Amp;
		NoSweep_Amp		= (double)-137;
	}
	ReadStructEnd(ar);

}

void HP8660B_FrequencySweepObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);
	
	WriteFloat(ar,Sweep_Start);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Stop);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Resolution);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Mode);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_ModFreq);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_ModAmpFM);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Amp);

	WriteStructEnd(ar);
	WriteLine(ar);

}

/****************   Methods for Class HP3325B_FrequencySweepObj  ***********************/

void HP3325B_FrequencySweepObj::UpdateData(void)
{
	Sweep_Stop = CalcStop(Sweep_Start, Sweep_Stop, pTrack->Channels, HP3325Resolution);
}

void HP3325B_FrequencySweepObj::SetMinMax(double minstep,double maxstep)
{
	CalcStartStop(&Sweep_Start, &Sweep_Stop, pTrack->Channels, Resolution, minstep, maxstep);
}

void HP3325B_FrequencySweepObj::SetCenter(USHORT step)
{
	CalcCenter(&Sweep_Start, &Sweep_Stop, pTrack->Channels, Resolution, step);
}

void HP3325B_FrequencySweepObj::DoDoubleClickAction(void)
{
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);
	CPropShtHP propSheet(ShortName+" in Track ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);
		
	propSheet.m_page1.channels		= pTrack->Channels;
	propSheet.m_page1.resolution	= Resolution;

	propSheet.m_page1.startfreq		= Sweep_Start;
	propSheet.m_page1.stopfreq		= Sweep_Stop;
	propSheet.m_page1.ampdb			= Sweep_Amp;

	propSheet.m_page2.ampdb			= NoSweep_Amp;
	propSheet.m_page2.m_noscanfreq	= NoSweep_Value;

	propSheet.m_page3.m_gpib		= Gpib;

	
	if(IDOK == propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			Sweep_Start = propSheet.m_page1.startfreq;
			Sweep_Stop	= propSheet.m_page1.stopfreq;
			Sweep_Amp	=  propSheet.m_page1.ampdb;

			NoSweep_Amp = propSheet.m_page2.ampdb;
			NoSweep_Value = propSheet.m_page2.m_noscanfreq;

			Gpib		= propSheet.m_page3.m_gpib;

			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
			UpdateNoScan();
		}
		else Beep(1000,100);
	}

}

void HP3325B_FrequencySweepObj::CopyObject(ActionObject* pSource)
{
	HP3325B_FrequencySweepObj* pActionObject = (HP3325B_FrequencySweepObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;

	Sweep_Start			= pActionObject->Sweep_Start;
	Sweep_Step			= pActionObject->Sweep_Step;
	Sweep_Stop			= pActionObject->Sweep_Stop;
	Sweep_Resolution	= pActionObject->Sweep_Resolution;
	Sweep_Amp			= pActionObject->Sweep_Amp;
	Sweep_Mode			= pActionObject->Sweep_Mode;
	Sweep_ModFreq		= pActionObject->Sweep_ModFreq;
	Sweep_ModAmpFM		= pActionObject->Sweep_ModAmpFM;
	Sweep_ModAmpAM		= pActionObject->Sweep_ModAmpAM;
		
	NoSweep_Value		= pActionObject->NoSweep_Value;
	NoSweep_Amp			= pActionObject->NoSweep_Amp;
	NoSweep_Mode		= pActionObject->NoSweep_Mode;
	NoSweep_ModFreq		= pActionObject->NoSweep_ModFreq;
	NoSweep_ModAmpFM	= pActionObject->NoSweep_ModAmpFM;
	NoSweep_ModAmpAM	= pActionObject->NoSweep_ModAmpAM;
}

CString HP3325B_FrequencySweepObj::GetInfo(void)
{
	CString answer;
	answer.Format("%gMHz TO %gMHz step %gMHz",Sweep_Start,Sweep_Stop,
		CalcStep(Sweep_Start,Sweep_Stop,pTrack->Channels,Resolution));
	return answer;
}

void HP3325B_FrequencySweepObj::Load(CArchive& ar)
{
	Resolution		= HP3325Resolution;
	Name             = ReadString(ar);
	ShortName        = __HP3325B_FrequencySweepObj;
	ReadSeparator(ar);
	Sweep_Start      = ReadFloat(ar);
	NoSweep_Value		= Sweep_Start;
	Sweep_Stop       = ReadFloat(ar);
	Sweep_Resolution = ReadFloat(ar);
	if (pDocument->Version > 6)
	{
		Sweep_Mode    = (int)ReadFloat(ar);
		NoSweep_Mode	= Sweep_Mode;
		Sweep_ModFreq = ReadFloat(ar);
		if (pDocument->Version > 19)
		{
			Sweep_ModAmpFM = ReadFloat(ar);
		}
		Sweep_Amp = ReadFloat(ar);
		Sweep_Amp		= Sweep_Amp;
		NoSweep_Amp		= (double)-137;
	}
	ReadStructEnd(ar);

}

void HP3325B_FrequencySweepObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);

	WriteFloat(ar,Sweep_Start);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Stop);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Resolution);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Mode);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_ModFreq);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_ModAmpFM);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Amp);

	WriteStructEnd(ar);
	WriteLine(ar);

}

/****************   Methods for Class HP3325B2_FrequencySweepObj  ***********************/

void HP3325B2_FrequencySweepObj::UpdateData(void)
{
	Sweep_Stop = CalcStop(Sweep_Start, Sweep_Stop, pTrack->Channels, HP3325Resolution);
}

void HP3325B2_FrequencySweepObj::SetMinMax(double minstep,double maxstep)
{
	CalcStartStop(&Sweep_Start, &Sweep_Stop, pTrack->Channels, Resolution, minstep, maxstep);
}

void HP3325B2_FrequencySweepObj::SetCenter(USHORT step)
{
	CalcCenter(&Sweep_Start, &Sweep_Stop, pTrack->Channels, Resolution, step);
}

void HP3325B2_FrequencySweepObj::DoDoubleClickAction(void)
{
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);
	CPropShtHP propSheet(ShortName+" in Track ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);
		
	propSheet.m_page1.channels		= pTrack->Channels;
	propSheet.m_page1.resolution	= Resolution;

	propSheet.m_page1.startfreq		= Sweep_Start;
	propSheet.m_page1.stopfreq		= Sweep_Stop;
	propSheet.m_page1.ampdb			= Sweep_Amp;

	propSheet.m_page2.ampdb			= NoSweep_Amp;
	propSheet.m_page2.m_noscanfreq	= NoSweep_Value;
	
	propSheet.m_page3.m_gpib		= Gpib;
	
	if(IDOK == propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			Sweep_Start = propSheet.m_page1.startfreq;
			Sweep_Stop	= propSheet.m_page1.stopfreq;
			Sweep_Amp	=  propSheet.m_page1.ampdb;

			NoSweep_Amp = propSheet.m_page2.ampdb;
			NoSweep_Value = propSheet.m_page2.m_noscanfreq;

			Gpib		= propSheet.m_page3.m_gpib;

			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
			UpdateNoScan();
		}
		else Beep(1000,100);
	}

}

void HP3325B2_FrequencySweepObj::CopyObject(ActionObject* pSource)
{
	HP3325B2_FrequencySweepObj* pActionObject = (HP3325B2_FrequencySweepObj*) pSource;

	pDocument		= NULL;
	pTrack			= NULL;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;

	Sweep_Start			= pActionObject->Sweep_Start;
	Sweep_Step			= pActionObject->Sweep_Step;
	Sweep_Stop			= pActionObject->Sweep_Stop;
	Sweep_Resolution	= pActionObject->Sweep_Resolution;
	Sweep_Amp			= pActionObject->Sweep_Amp;
	Sweep_Mode			= pActionObject->Sweep_Mode;
	Sweep_ModFreq		= pActionObject->Sweep_ModFreq;
	Sweep_ModAmpFM		= pActionObject->Sweep_ModAmpFM;
	Sweep_ModAmpAM		= pActionObject->Sweep_ModAmpAM;
		
	NoSweep_Value		= pActionObject->NoSweep_Value;
	NoSweep_Amp			= pActionObject->NoSweep_Amp;
	NoSweep_Mode		= pActionObject->NoSweep_Mode;
	NoSweep_ModFreq		= pActionObject->NoSweep_ModFreq;
	NoSweep_ModAmpFM	= pActionObject->NoSweep_ModAmpFM;
	NoSweep_ModAmpAM	= pActionObject->NoSweep_ModAmpAM;
}

CString HP3325B2_FrequencySweepObj::GetInfo(void)
{
	CString answer;
	answer.Format("%gMHz TO %gMHz step %gMHz",Sweep_Start,Sweep_Stop,
		CalcStep(Sweep_Start,Sweep_Stop,pTrack->Channels,Resolution));
	return answer;
}

void HP3325B2_FrequencySweepObj::Load(CArchive& ar)
{
	Resolution			= HP3325Resolution;
	Name=ReadString(ar);
	ShortName=__HP3325B2_FrequencySweepObj;
	ReadSeparator(ar);
	Sweep_Start      = ReadFloat(ar);
	NoSweep_Value		= Sweep_Start;
	Sweep_Stop       = ReadFloat(ar);
	Sweep_Resolution = ReadFloat(ar);
	if (pDocument->Version > 6)
	{
		Sweep_Mode			= (int)ReadFloat(ar);
		NoSweep_Mode	= Sweep_Mode;
		Sweep_ModFreq		= ReadFloat(ar);
		if (pDocument->Version > 19)
		{
			Sweep_ModAmpFM	= ReadFloat(ar);
		}
		Sweep_Amp	= ReadFloat(ar);
		Sweep_Amp		= Sweep_Amp;
		NoSweep_Amp		= (double)-137;
	}
	ReadStructEnd(ar);

}

void HP3325B2_FrequencySweepObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);

	WriteFloat(ar,Sweep_Start);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Stop);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Resolution);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Mode);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_ModFreq);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_ModAmpFM);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Amp);

	WriteStructEnd(ar);
	WriteLine(ar);

}

/****************   Methods for Class PM_SpectrumObj  ***********************/

double PM_SpectrumObj::EstimateY(USHORT channel, USHORT scan)
{
	if(!ARITHSCALER) return (double)(y[channel]*(scan)/(scan-1));
	else
	{
		double TheData;
		double y;
		int err;
		char evalStr[255];
		strcpy(evalStr,scan_data_op);
	
		EvaluateNext(pDocument,&evalStr[0],&y,&err,channel,scan);
		TheData=y;

		return y;
	}
}

void PM_SpectrumObj::DoSummingAction(ActionObject* pDestination)
{
	USHORT i;
	PM_SpectrumObj* pScaler = (PM_SpectrumObj*)pDestination;
	switch(pDocument->AddMe)
	{
	case 0:
		for(i=0;i<MAXPOINTS;i++)pScaler->y[i]	= y[i];
		break;
	case 1:
		for(i=0;i<MAXPOINTS;i++)pScaler->y[i]	= -y[i];
		break;
	case 2:
		for(i=0;i<MAXPOINTS;i++)pScaler->y[i]	= pScaler->y[i]+y[i];
		break;
	case 3:
		for(i=0;i<MAXPOINTS;i++)pScaler->y[i]	= pScaler->y[i]-y[i];
		break;
	}
}

double PM_SpectrumObj::EstimateYErr(USHORT channel, USHORT scan)
{
	if(!ARITHSCALER) return (double)sqrt(fabs(y[channel]*(scan)/(scan-1)));
	else
	{
		double y = 0;
		double TheData;
		int err;
		char errorStr[255];
		strcpy(errorStr,scan_error_op);
		if(scan_error_op.IsEmpty())
		{
			TheData=sqrt(fabs(y));
		}
		else
		{
			EvaluateNext(pDocument,&errorStr[0],&y,&err,channel,scan);
			TheData=y;
		}
		return TheData;
	}
}

void PM_SpectrumObj::TakeData(BOOL RUNMODE, USHORT scan, USHORT channel, ULONG TheData)
{
	if(scan==1&&RUNMODE==ERGO)y[channel] = (float)TheData;
	else y[channel] = TheData+y[channel];		
}

void PM_SpectrumObj::DoDoubleClickAction(void)
{
	BOOL ENDE = FALSE;
	USHORT track;
	int i;
	double Start,Stop;
	CString DoNew = "";
	ActionObject* pActionObject;
	TriggerObj* pTriggerObject = NULL;
	CString TheObject="";
	CString TrackNo;
	CString ScalNo;
	CString XName,XUnit;
	BOOL Arith = FALSE;
	TrackNo.Format("%u",pTrack->MyPosition);
	ScalNo.Format("%u",SubRegNo);

	if (DoString!="normal") 
	{
		Arith = TRUE;
	}
	
	XName = "Channel";
	XUnit = "";
	Start = (double)1;
	Stop  = (double)pTrack->Channels;

	if(pDataView==NULL)pDataView = new DataView(this);
	else pDataView->Reset();
	pDataView->SetWindowTitle(ShortName+" "+GetInfo()+" in Track ["+TrackNo+"]  of "+pDocument->GetTitle());


	if(!Arith)pDataView->SetYTitle("Counts in Scaler "+ScalNo,"");
	else pDataView->SetYTitle("f(x)","");
	POSITION pos=pDocument->ActionObjList.GetHeadPosition();
	int Channels = pTrack->Channels;
	
	track = pTrack->MyPosition;

	pActionObject = (ActionObject*)pDocument->ActionObjList.GetHead();
	pos = pDocument->ActionObjList.GetHeadPosition();
	do
	{
		pActionObject = (ActionObject*)pDocument->ActionObjList.GetNext(pos);
		if (pActionObject!=NULL&&pActionObject->pTrack->MyPosition == track)
		{
			TheObject     = pActionObject->GetName();
			if(TheObject==__TriggerObj) 
			{
				pTriggerObject = (TriggerObj*)pActionObject;
				ENDE = TRUE;
			}
		}
	}
	while (pos!=NULL&&ENDE==FALSE);

	ENDE = FALSE;

	pActionObject = (ActionObject*)pDocument->ActionObjList.GetHead();
	pos = pDocument->ActionObjList.GetHeadPosition();
	do
	{
		pActionObject = (ActionObject*)pDocument->ActionObjList.GetNext(pos);
		if (pActionObject!=NULL&&pActionObject->pTrack->MyPosition == track)
		{
			TheObject     = pActionObject->GetName();
			if((TheObject==__HP_VoltageSweepObj)||
			(TheObject==__Line_VoltageSweepObj)||
			(TheObject==__HP8660B_FrequencySweepObj)||
			(TheObject==__HP3325B_FrequencySweepObj)||
			(TheObject==__HP3325B2_FrequencySweepObj)||
			(TheObject==__RS_FrequencySweepObj))ENDE = TRUE;
		}
	}
	while (pos!=NULL&&ENDE==FALSE);

	if(pos!=NULL)
	{
		if((TheObject==__HP_VoltageSweepObj)||(TheObject==__Line_VoltageSweepObj))
		{
			if(TheObject==__HP_VoltageSweepObj)
			{
				CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
				HP_VoltageSweepObj* pAction1 = (HP_VoltageSweepObj*)pActionObject;
				Start	= pAction1->Sweep_Start;
				Stop	= pAction1->Sweep_Stop;
				XName	= "HPVoltage";
				if(ShowSumVoltage)
				{
					Start	= Start*pApp->GlobalKepcoFactor;
					Stop	= Stop*pApp->GlobalKepcoFactor;
				}
			}
			if(TheObject==__Line_VoltageSweepObj)
			{
				CMCPforNTApp* pApp2 = (CMCPforNTApp*)AfxGetApp();
				Line_VoltageSweepObj* pAction2 = (Line_VoltageSweepObj*)pActionObject;
				Start	= pAction2->Sweep_Start;
				Stop	= pAction2->Sweep_Stop;
				XName	= "LineVoltage";
				if(ShowSumVoltage)
				{
					Start	= Start*pApp2->GlobalKepcoFactor;
					Stop	= Stop*pApp2->GlobalKepcoFactor;
				}
			}
			if (Start==Stop)
			{
				if(pTriggerObject != NULL)
				{
					if(pTriggerObject->TriggerType != 1)
					{
						XName = "Time";
						XUnit = "s";
						Start = (double)0;
						Stop  = (double)pTrack->Channels*pTriggerObject->CloseScaler/1000.0;
					}
					else
					{	
						XName = "Channel";
						XUnit = "";
						Start = (double)1;
						Stop  = (double)pTrack->Channels;
					}
				}
			}
			else
			{
				CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
				double V_FLUKE	= 0;
				double V_PREMA	= 0;
				double V_ISOLDE	= 0;
				BOOL Found1 = FALSE;
				BOOL Found2 = FALSE;
				BOOL Found3 = FALSE;
				POSITION pos;
				ActionObject* pActionObj;
				TrackObject* TheTrack = pTrack;
				TrackObject* pThisTrack;
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
								pThisTrack=pActionObj->pTrack;
								if(pActionObj->Name==_IsoldeNetVoltageObj&&pThisTrack==TheTrack)
								{
									Found1 = TRUE;				
								}
							}
						}
						while (pos!=NULL&&!Found1);
						if(Found1) 
						{
							IsoldeNetVoltageObj* pIsolde = (IsoldeNetVoltageObj*)pActionObj;
							V_ISOLDE = pIsolde->Voltage[pIsolde->NumOfSamples-1];
						}
					}
				}

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
								pThisTrack=pActionObj->pTrack;
								if(pActionObj->Name==_FlukeVoltageObj&&pThisTrack==TheTrack)
								{
									Found2 = TRUE;				
								}
							}
						}
						while (pos!=NULL&&!Found2);
						if(Found2) 
						{
							FlukeVoltageObj* pFluke = (FlukeVoltageObj*)pActionObj;
							V_FLUKE = pFluke->Voltage[pFluke->NumOfSamples-1];
						}
					}
				}

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
								pThisTrack=pActionObj->pTrack;
								if(pActionObj->Name==_PremaVoltageObj&&pThisTrack==TheTrack)
								{
									Found3 = TRUE;				
								}
							}
						}
						while (pos!=NULL&&!Found3);
						if(Found3) 
						{
							PremaVoltageObj* pPrema = (PremaVoltageObj*)pActionObj;
							V_PREMA = pPrema->Voltage[pPrema->NumOfSamples-1];
						}
					}
				}

				/***************   find Isolde+Prema-voltage end ***************/

				if(Found1&&Found3) V_PREMA = -V_PREMA;
				if((Found1||Found3)&&(ShowSumVoltage))
				{
					XName = "Acceleration-Voltage";
					Start = V_ISOLDE+1000*V_PREMA-V_FLUKE-Start;
					Stop  = V_ISOLDE+1000*V_PREMA-V_FLUKE-Stop;
				}
				XUnit = "V";
			}
		}
		if(TheObject==__HP8660B_FrequencySweepObj)
		{
			HP8660B_FrequencySweepObj* pAction = (HP8660B_FrequencySweepObj*)pActionObject;
			Start = pAction->Sweep_Start;
			Stop  = pAction->Sweep_Stop;
			XName = "HP8-Frequency";
			XUnit = "MHz";
		}
		if(TheObject==__HP3325B_FrequencySweepObj)
		{
			HP3325B_FrequencySweepObj* pAction = (HP3325B_FrequencySweepObj*)pActionObject;
			Start = pAction->Sweep_Start;
			Stop  = pAction->Sweep_Stop;
			XName = "HP3-Frequency";
			XUnit = "MHz";
		}
		if(TheObject==__HP3325B2_FrequencySweepObj)
		{
			HP3325B2_FrequencySweepObj* pAction = (HP3325B2_FrequencySweepObj*)pActionObject;
			Start = pAction->Sweep_Start;
			Stop  = pAction->Sweep_Stop;
			XName = "HP3-Frequency";
			XUnit = "MHz";
		}
		if(TheObject==__RS_FrequencySweepObj)
		{
			RS_FrequencySweepObj* pAction = (RS_FrequencySweepObj*)pActionObject;
			Start = pAction->Sweep_Start;
			Stop  = pAction->Sweep_Stop;
			XName = "RS-Frequency";
			XUnit = "MHz";
		}
	}
	else
	{
		if(pTriggerObject != NULL)
		{
			if(pTriggerObject->TriggerType != 1)
			{
				XName = "Time";
				XUnit = "s";
				Start = (double)0;
				Stop  = (double)pTrack->Channels*pTriggerObject->CloseScaler/1000.0;
			}
		}
	}
	pDataView->SetXTitle(XName,XUnit);
	pDataView->SetTitle(pTrack->Isotope);
	for(i=0;i<pTrack->Channels;i++)
	{
		double error = sqrt(fabs(y[i]));
		if(error==0) error=1;
		pDataView->AddDataPoint((float)(i*((Stop-Start)/(Channels-1))+Start),
								(float)y[i],
								(float)error);
	}
	pDataView->data_type = data_type;
	pDataView->RedrawNow();
}

void PM_SpectrumObj::UpdateDisplay(void)
{
	BOOL ENDE = FALSE;
	USHORT track;
	int i;
	double Start,Stop;
	CString DoNew = "";
	ActionObject* pActionObject;
	CString TheObject="";
	CString TrackNo;
	CString ScalNo;
	CString XName,XUnit;
	BOOL Arith = FALSE;
	TrackNo.Format("%u",pTrack->MyPosition);
	ScalNo.Format("%u",SubRegNo);

	if (DoString!="normal") 
	{
		Arith = TRUE;
	}
	
	XName = "Channel";
	XUnit = "";
	Start = (double)1;
	Stop  = (double)pTrack->Channels;

	if(pDataView!=NULL)
	{
		pDataView->Reset();
		pDataView->SetWindowTitle(ShortName+" "+GetInfo()+" in Track ["+TrackNo+"]  of "+pDocument->GetTitle());


		if(!Arith)pDataView->SetYTitle("Counts in Scaler "+ScalNo,"");
		else pDataView->SetYTitle("f(x)","");
		POSITION pos=pDocument->ActionObjList.GetHeadPosition();
		int Channels = pTrack->Channels;
		
		track = pTrack->MyPosition;

		pActionObject = (ActionObject*)pDocument->ActionObjList.GetHead();
		pos = pDocument->ActionObjList.GetHeadPosition();
		do
		{
			pActionObject = (ActionObject*)pDocument->ActionObjList.GetNext(pos);
			if (pActionObject!=NULL&&pActionObject->pTrack->MyPosition == track)
			{
				TheObject     = pActionObject->GetName();
				if((TheObject==__HP_VoltageSweepObj)||
				(TheObject==__Line_VoltageSweepObj)||
				(TheObject==__HP8660B_FrequencySweepObj)||
				(TheObject==__HP3325B_FrequencySweepObj)||
				(TheObject==__HP3325B2_FrequencySweepObj)||
				(TheObject==__RS_FrequencySweepObj))ENDE = TRUE;
			}
		}
		while (pos!=NULL&&ENDE==FALSE);

		if(pos!=NULL)
		{
			if((TheObject==__HP_VoltageSweepObj)||(TheObject==__Line_VoltageSweepObj))
			{
				if(TheObject==__HP_VoltageSweepObj)
				{
					CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
					HP_VoltageSweepObj* pAction1 = (HP_VoltageSweepObj*)pActionObject;
					Start	= pAction1->Sweep_Start;
					Stop	= pAction1->Sweep_Stop;
					XName	= "HPVoltage";
					if(ShowSumVoltage)
					{
						Start	= Start*pApp->GlobalKepcoFactor;
						Stop	= Stop*pApp->GlobalKepcoFactor;
					}
				}
				if(TheObject==__Line_VoltageSweepObj)
				{
					CMCPforNTApp* pApp2 = (CMCPforNTApp*)AfxGetApp();
					Line_VoltageSweepObj* pAction2 = (Line_VoltageSweepObj*)pActionObject;
					Start	= pAction2->Sweep_Start;
					Stop	= pAction2->Sweep_Stop;
					XName	= "LineVoltage";
					if(ShowSumVoltage)
					{
						Start	= Start*pApp2->GlobalKepcoFactor;
						Stop	= Stop*pApp2->GlobalKepcoFactor;
					}
				}
				if (Start==Stop)
				{
					XName = "Channel";
					XUnit = "";
					Start = (double)1;
					Stop  = (double)pTrack->Channels;
				}
				else
				{
					CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
					double V_FLUKE	= 0;
					double V_PREMA	= 0;
					double V_ISOLDE	= 0;
					BOOL Found1 = FALSE;
					BOOL Found2 = FALSE;
					BOOL Found3 = FALSE;
					POSITION pos;
					ActionObject* pActionObj;
					TrackObject* TheTrack = pTrack;
					TrackObject* pThisTrack;
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
									pThisTrack=pActionObj->pTrack;
									if(pActionObj->Name==_IsoldeNetVoltageObj&&pThisTrack==TheTrack)
									{
										Found1 = TRUE;				
									}
								}
							}
							while (pos!=NULL&&!Found1);
							if(Found1) 
							{
								IsoldeNetVoltageObj* pIsolde = (IsoldeNetVoltageObj*)pActionObj;
								V_ISOLDE = pIsolde->Voltage[pIsolde->NumOfSamples-1];
							}
						}
					}

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
									pThisTrack=pActionObj->pTrack;
									if(pActionObj->Name==_FlukeVoltageObj&&pThisTrack==TheTrack)
									{
										Found2 = TRUE;				
									}
								}
							}
							while (pos!=NULL&&!Found2);
							if(Found2) 
							{
								FlukeVoltageObj* pFluke = (FlukeVoltageObj*)pActionObj;
								V_FLUKE = pFluke->Voltage[pFluke->NumOfSamples-1];
							}
						}
					}

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
									pThisTrack=pActionObj->pTrack;
									if(pActionObj->Name==_PremaVoltageObj&&pThisTrack==TheTrack)
									{
										Found3 = TRUE;				
									}
								}
							}
							while (pos!=NULL&&!Found3);
							if(Found3) 
							{
								PremaVoltageObj* pPrema = (PremaVoltageObj*)pActionObj;
								V_PREMA = pPrema->Voltage[pPrema->NumOfSamples-1];
							}
						}
					}

					/***************   find Isolde+Prema-voltage end ***************/

					if(Found1&&Found3) V_PREMA = -V_PREMA;
					if((Found1||Found3)&&(ShowSumVoltage))
					{
						XName = "Acceleration-Voltage";
						Start = V_ISOLDE+1000*V_PREMA-V_FLUKE-Start;
						Stop  = V_ISOLDE+1000*V_PREMA-V_FLUKE-Stop;
					}
					XUnit = "V";
				}
			}
			if(TheObject==__HP8660B_FrequencySweepObj)
			{
				HP8660B_FrequencySweepObj* pAction = (HP8660B_FrequencySweepObj*)pActionObject;
				Start = pAction->Sweep_Start;
				Stop  = pAction->Sweep_Stop;
				XName = "HP8-Frequency";
				XUnit = "MHz";
			}
			if(TheObject==__HP3325B_FrequencySweepObj)
			{
				HP3325B_FrequencySweepObj* pAction = (HP3325B_FrequencySweepObj*)pActionObject;
				Start = pAction->Sweep_Start;
				Stop  = pAction->Sweep_Stop;
				XName = "HP3-Frequency";
				XUnit = "MHz";
			}
			if(TheObject==__HP3325B2_FrequencySweepObj)
			{
				HP3325B2_FrequencySweepObj* pAction = (HP3325B2_FrequencySweepObj*)pActionObject;
				Start = pAction->Sweep_Start;
				Stop  = pAction->Sweep_Stop;
				XName = "HP3-Frequency";
				XUnit = "MHz";
			}
			if(TheObject==__RS_FrequencySweepObj)
			{
				RS_FrequencySweepObj* pAction = (RS_FrequencySweepObj*)pActionObject;
				Start = pAction->Sweep_Start;
				Stop  = pAction->Sweep_Stop;
				XName = "RS-Frequency";
				XUnit = "MHz";
			}
		}
		pDataView->SetXTitle(XName,XUnit);
		pDataView->SetTitle(pTrack->Isotope);
		for(i=0;i<pTrack->Channels;i++)
		{
			double error = sqrt(fabs(y[i]));
			if(error==0) error=1;
			pDataView->AddDataPoint((float)(i*((Stop-Start)/(Channels-1))+Start),
									(float)y[i],
									(float)error);
		}
		pDataView->RedrawNow();
	}
}

void PM_SpectrumObj::CopyObject(ActionObject* pSource)
{
	PM_SpectrumObj* pActionObject = (PM_SpectrumObj*) pSource;

	pDocument		= NULL;
	pTrack			= NULL;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;

	SubRegNo				= pActionObject->SubRegNo;
	Sweep_Resolution		= pActionObject->Sweep_Resolution;
	ChannelOffset			= pActionObject->ChannelOffset;
	DispMonitorMode			= pActionObject->DispMonitorMode;
	AnalysisMode			= pActionObject->AnalysisMode;
	PredecessorSubRegNo		= pActionObject->PredecessorSubRegNo;
	CountScansFlag			= pActionObject->CountScansFlag;
		
	scan_data_op			= pActionObject->scan_data_op;
	scan_error_op			= pActionObject->scan_error_op;
	SCALER					= pActionObject->SCALER;
	ARITHSCALER				= pActionObject->ARITHSCALER;

	Name.FreeExtra();
	ShortName.FreeExtra();
	DoString.FreeExtra();
	Isotope.FreeExtra();
	scan_data_op.FreeExtra();
	scan_error_op.FreeExtra();
}

double PM_SpectrumObj::GetY(USHORT channel)
{
	if(DoString	!= "normal")
	{
		double ly;
		int err;
		char evalStr[255];
		strcpy(evalStr,scan_data_op);
		Evaluate(pDocument,&evalStr[0],&ly,&err,channel);
		y[channel]=ly;
		return (double)ly;	
	}
	else
	{
		return (double)y[channel];
	}
}

double PM_SpectrumObj::GetYErr(USHORT channel)
{
	if(DoString	!= "normal")
	{
		double ly;
		int err;
		char evalStr[255];
		char errorStr[255];
		strcpy(errorStr,scan_error_op);
		if(scan_error_op.IsEmpty())
		{
			strcpy(evalStr,scan_data_op);
			Evaluate(pDocument,&evalStr[0],&ly,&err,channel);
			return (double)sqrt(fabs(ly));
		}
		else
		{
			Evaluate(pDocument,&errorStr[0],&ly,&err,channel);
			return (double)ly;
		}
	}
	else
	{
		return (double)sqrt(fabs(y[channel]));
	}
}

CString PM_SpectrumObj::GetInfo(void)
{
	CString SubReg;
	SubReg.Format("%u",SubRegNo);
	CString mode = "["+SubReg+"] ";
	if(DoString!="normal") mode = mode+"y = "+scan_data_op;
	return mode;
}

void PM_SpectrumObj::Load(CArchive& ar)
{
	SCALER		= TRUE;
	TakesData	= TRUE;
	long  i,j=0;
	long  Channels;
	CString TrackNo;
	CString ScalNo;

	TrackNo.Format("%u",pTrack->MyPosition);
	pTrack->ArithCounter++;
	DoString = "normal";
	Name     = ReadString(ar);
	ReadSeparator(ar);
	SubRegNo          = (int)ReadFloat(ar);
	ReadSeparator(ar);	
	ScalNo.Format("%u",SubRegNo);
	ShortName     = __PM_SpectrumObj;
	Sweep_Resolution  = ReadFloat(ar);
	
	if (pDocument->Version > 1)
	{
		ReadSeparator(ar);
		ChannelOffset = (int)ReadFloat(ar);
	}
	if (pDocument->Version > 4)
	{
		ReadSeparator(ar);
		DispMonitorMode = (int)ReadFloat(ar);
	}
	if (pDocument->Version > 5)
	{
		
		ReadSeparator(ar);
		DoString     = ReadString(ar);
		if (pDocument->Version >= 21)
		{
			ReadSeparator(ar);
			scan_data_op	= ReadString(ar);
			ReadSeparator(ar);
			scan_error_op	= ReadString(ar);
		}
		ReadSeparator(ar);
		AnalysisMode = (int)ReadFloat(ar);
	}
	if (pDocument->Version > 9)
	{
		ReadSeparator(ar);
		PredecessorSubRegNo = (int)ReadFloat(ar);
	}
	if (pDocument->Version > 15)
	{
		ReadSeparator(ar);
		CountScansFlag = (int)ReadFloat(ar);
	}
	ReadControlBegin(ar);
	Channels			 = (long)ReadFloat(ar);
	ReadControlEnd(ar);
	ReadStructBegin(ar);

	for (i=0;i<Channels;i++)
	{
		y[i] = ReadFloat(ar);
	}
	ReadStructEnd(ar);
	ReadStructEnd(ar);

	if (DoString!="normal") 
	{
			ShortName = __PM_ArithObj;
			ARITHSCALER = TRUE;
			SCALER		= TRUE;
	}
	if (pDocument->Version < 21)
	{
		if (DoString!="normal") 
		{
			ARITHSCALER = TRUE;
			SCALER		= TRUE;
			scan_data_op = DoString;
			CString DoNew = "";
			CString TheChar;
			if(isdigit(DoString.GetAt(0))&&
				(DoString.GetAt(1)=='-')&&
				isdigit(DoString.GetAt(2)))
			{
				scan_data_op = (CString)("asym(")+DoString.GetAt(0)+(CString)(",")+DoString.GetAt(2)+(CString)(",")+TrackNo+(CString)(")");
				scan_error_op = (CString)("asymerr(")+DoString.GetAt(0)+(CString)(",")+DoString.GetAt(2)+(CString)(",")+TrackNo+(CString)(")");
			}
			else
			{
				for(i=0;i < DoString.GetLength();i++)
				{
					TheChar = DoString.GetAt(i);
					if (isdigit(TheChar.GetAt(0)))
					{
						DoNew = DoNew + "c("+TheChar+","+TrackNo+")";
					}
					else
					{
						DoNew = DoNew + TheChar;
					}
				}
				scan_data_op = DoNew;
				scan_error_op = "";
			}
		}
	}
}

void PM_SpectrumObj::Save(CArchive& ar)
{
	USHORT i,j=0;

	USHORT Channels = pTrack->Channels;
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);

	WriteFloat(ar,SubRegNo);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Resolution);
	WriteSeparator(ar);
	WriteFloat(ar,ChannelOffset);
	WriteSeparator(ar);
	WriteFloat(ar,DispMonitorMode);
	WriteSeparator(ar);
	WriteString(ar,DoString);
	WriteSeparator(ar);
	WriteString(ar,scan_data_op);
	WriteSeparator(ar);
	WriteString(ar,scan_error_op);
	WriteSeparator(ar);
	WriteFloat(ar,AnalysisMode);
	WriteSeparator(ar);
	WriteFloat(ar,PredecessorSubRegNo);
	WriteSeparator(ar);
	WriteFloat(ar,CountScansFlag);

	WriteControlBegin(ar);
	WriteFloat(ar,Channels);
	WriteControlEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);

	for (i=0;i<Channels;i++)
	{
		WriteFloat(ar,y[i]);
		if(i!=(Channels-1))WriteSeparator(ar);
		j++;
		if(j==5)
		{
			j = 0;
			WriteLine(ar);
		}
	}
	WriteStructEnd(ar);
	WriteStructEnd(ar);
	WriteLine(ar);
}

void PM_SpectrumObj::MeasurementBeginAction(BOOL RUNMODE)
{
	USHORT i;
	pDocument->DataQueueIndex = 0;
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	if(DispMonitorMode!=0)if(pApp->pMonitorView!=NULL)pApp->pMonitorView->DeleteAllViews();
	if(RUNMODE==ERGO)for(i=0;i<MAXPOINTS;i++)y[i]=0;
}

void PM_SpectrumObj::MeasurementEndAction(void)
{
}

void PM_SpectrumObj::TrackBeginAction(USHORT track)
{
	int i;
	BOOL NewData = FALSE;
	BOOL ENDE = FALSE;
	CString XName,XUnit;
	CString YTitle,YUnit;
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	
	if(DispMonitorMode == 1)
	{
		if(pApp->pMonitorView==NULL)pApp->pMonitorView = new MonitorView();

		double Start,Stop;
		CString DoNew = "";
		ActionObject* pActionObject;
		CString TheObject="";
		CString TrackNo;
		CString ScalNo;
		
		TrackNo.Format("%u",pTrack->MyPosition);
		ScalNo.Format("%u",SubRegNo);
		
		XName = "Channel";
		XUnit = "";
		Start = (double)1;
		Stop  = (double)pTrack->Channels;
		 
		pApp->pMonitorView->SetWindowTitle("Monitor for Track ["+TrackNo+"]  of "+pDocument->GetTitle());

		for(i=0;i<4;i++)pApp->pMonitorView->ActualIndex[i] = 0;

		POSITION pos=pDocument->ActionObjList.GetHeadPosition();
		int Channels = pTrack->Channels;
		
		pActionObject = (ActionObject*)pDocument->ActionObjList.GetHead();
		pos = pDocument->ActionObjList.GetHeadPosition();
		do
		{
			pActionObject = (ActionObject*)pDocument->ActionObjList.GetNext(pos);
			if (pActionObject!=NULL&&pActionObject->pTrack->MyPosition == track)
			{
				TheObject     = pActionObject->GetName();
				if((TheObject==__HP_VoltageSweepObj)||	
				(TheObject==__Line_VoltageSweepObj)||
				(TheObject==__HP8660B_FrequencySweepObj)||
				(TheObject==__HP3325B_FrequencySweepObj)||
				(TheObject==__HP3325B2_FrequencySweepObj)||
				(TheObject==__RS_FrequencySweepObj))ENDE = TRUE;
			}
		}
		while (pos!=NULL&&ENDE==FALSE);

		if(pos!=NULL)
		{
			if((TheObject==__HP_VoltageSweepObj)||(TheObject==__Line_VoltageSweepObj))
			{
				if(TheObject==__HP_VoltageSweepObj)
				{
					CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
					HP_VoltageSweepObj* pAction1 = (HP_VoltageSweepObj*)pActionObject;
					Start	= pAction1->Sweep_Start;
					Stop	= pAction1->Sweep_Stop;
					XName	= "HPVoltage";
					XUnit = "V";
					if(ShowSumVoltage)
					{
						Start	= Start*pApp->GlobalKepcoFactor;
						Stop	= Stop*pApp->GlobalKepcoFactor;
					}
				}
				if(TheObject==__Line_VoltageSweepObj)
				{
					Line_VoltageSweepObj* pAction2 = (Line_VoltageSweepObj*)pActionObject;
					Start	= pAction2->Sweep_Start;
					Stop	= pAction2->Sweep_Stop;
					XName	= "LineVoltage";
					XUnit = "V";
				}
				if (Start==Stop)
				{
					XName = "Channel";
					XUnit = "";
					Start = (double)1;
					Stop  = (double)pTrack->Channels;
				}
			}
			if(TheObject==__HP8660B_FrequencySweepObj)
			{
				HP8660B_FrequencySweepObj* pAction = (HP8660B_FrequencySweepObj*)pActionObject;
				Start = pAction->Sweep_Start;
				Stop  = pAction->Sweep_Stop;
				XName = "HP8-Frequency";
				XUnit = "MHz";
			}
			if(TheObject==__HP3325B_FrequencySweepObj)
			{
				HP3325B_FrequencySweepObj* pAction = (HP3325B_FrequencySweepObj*)pActionObject;
				Start = pAction->Sweep_Start;
				Stop  = pAction->Sweep_Stop;
				XName = "HP3-Frequency";
				XUnit = "MHz";
			}
			if(TheObject==__HP3325B2_FrequencySweepObj)
			{
				HP3325B2_FrequencySweepObj* pAction = (HP3325B2_FrequencySweepObj*)pActionObject;
				Start = pAction->Sweep_Start;
				Stop  = pAction->Sweep_Stop;
				XName = "HP3-Frequency";
				XUnit = "MHz";
			}
			if(TheObject==__RS_FrequencySweepObj)
			{
				RS_FrequencySweepObj* pAction = (RS_FrequencySweepObj*)pActionObject;
				Start = pAction->Sweep_Start;
				Stop  = pAction->Sweep_Stop;
				XName = "RS-Frequency";
				XUnit = "MHz";
			}
		}
		if(DoString == "normal")YTitle = "scaler ["+ScalNo+"]";
		else YTitle = "arithscaler ["+ScalNo+"]";
		YUnit="";
		{
			if(pApp->pMonitorView->MonitorFree())
				pApp->pMonitorView->AddView(Channels,XName,XUnit,YTitle,YUnit,(float)Start,(float)Stop,this,pTrack->RealScans,ShowExpected);
		
			else 
			{
				DispMonitorMode = 0;
				AfxMessageBox("Too much monitors defined,\n\r disabled last",MB_OK,0);
			}
		}
	}
}

void PM_SpectrumObj::TrackStepAction(USHORT step, USHORT track, USHORT scan)
{
	ListDNAFCamac(SENDNAF,0,Slot,SubRegNo,0);
	ListDNAFCamac(READD,0,0,0,0);
	pDocument->DataQueue[pDocument->DataQueueIndex].scan = scan;
	pDocument->DataQueue[pDocument->DataQueueIndex].step = step;
	pDocument->DataQueue[pDocument->DataQueueIndex].pAction = this;
	pDocument->DataQueueIndex++;
	//ListDNAFCamac(SENDNAF,0,Slot,SubRegNo,9);	
}

void PM_SpectrumObj::DoConfigureAction(void)
{
	CString Track;
	Track.Format("%u",pTrack->MyPosition);
	Title=(ShortName+" "+GetInfo()+" in Track ["+Track+"]  of "+pDocument->GetTitle());
	if (!ARITHSCALER)
	{
		CPropShtPM  propSheet(Title,AfxGetMainWnd(),0);
		propSheet.m_page3.m_subaddress	= SubRegNo;
		propSheet.m_page3.m_slot		= Slot;
		propSheet.m_page1.m_points = FALSE;
		propSheet.m_page1.m_lines  = FALSE;
		propSheet.m_page1.m_stairs = FALSE;
		propSheet.m_page1.m_sumvoltage = ShowSumVoltage;
		if(data_type == 0) propSheet.m_page1.m_points = TRUE;
		if(data_type == 1) propSheet.m_page1.m_lines  = TRUE;
		if(data_type == 2) propSheet.m_page1.m_stairs = TRUE;
		if(DispMonitorMode == 1) propSheet.m_page1.m_autoopen = TRUE;
		propSheet.m_page1.m_autoresize = ShowExpected;
		if(IDOK == propSheet.DoModal())
		{
			if(!pDocument->MeasurementRunning)
			{
				if(propSheet.m_page1.m_points)data_type = 0;
				if(propSheet.m_page1.m_lines) data_type = 1;
				if(propSheet.m_page1.m_stairs)data_type = 2;
				if(propSheet.m_page1.m_autoopen)DispMonitorMode = 1;
				if(!propSheet.m_page1.m_autoopen)DispMonitorMode = 0;
				SubRegNo	= propSheet.m_page3.m_subaddress;
				Slot		= propSheet.m_page3.m_slot;
				ShowExpected = propSheet.m_page1.m_autoresize;
				ShowSumVoltage = propSheet.m_page1.m_sumvoltage;
				pDocument->SetModifiedFlag(TRUE);
				pDocument->UpdateAllViews(NULL,0,NULL);
				
			}
			else Beep(1000,100);
		}
	}
	else // is ArithScaler
	{
		CPropShtAS propSheet(Title,AfxGetMainWnd(),0);
		propSheet.m_page3.m_subaddress	= SubRegNo;
		propSheet.m_page1.m_dataop = scan_data_op;
		propSheet.m_page1.m_errorop = scan_error_op;
		propSheet.m_page1.m_points = FALSE;
		propSheet.m_page1.m_lines  = FALSE;
		propSheet.m_page1.m_stairs = FALSE;
		propSheet.m_page1.m_sumvoltage = ShowSumVoltage;
		if(data_type == 0) propSheet.m_page1.m_points = TRUE;
		if(data_type == 1) propSheet.m_page1.m_lines  = TRUE;
		if(data_type == 2) propSheet.m_page1.m_stairs = TRUE;
		if(DispMonitorMode == 1) propSheet.m_page1.m_autoopen = TRUE;
		propSheet.m_page1.m_autoresize = ShowExpected;
		if(IDOK == propSheet.DoModal())
		{
			if(!pDocument->MeasurementRunning)
			{
				if(propSheet.m_page1.m_points)data_type = 0;
				if(propSheet.m_page1.m_lines) data_type = 1;
				if(propSheet.m_page1.m_stairs)data_type = 2;
				if(propSheet.m_page1.m_autoopen)DispMonitorMode = 1;
				if(!propSheet.m_page1.m_autoopen)DispMonitorMode = 0;
				ShowExpected = propSheet.m_page1.m_autoresize;
				scan_data_op = propSheet.m_page1.m_dataop;	
				scan_error_op = propSheet.m_page1.m_errorop; 
				SubRegNo	= propSheet.m_page3.m_subaddress;
				ShowSumVoltage = propSheet.m_page1.m_sumvoltage;
				pDocument->SetModifiedFlag(TRUE);
				pDocument->UpdateAllViews(NULL,0,NULL);
			}
			else Beep(1000,100);
		}
	}
}

/****************   Methods for Class TriggerObj  ***********************/

void TriggerObj::TakeData(BOOL RUNMODE, USHORT scan, USHORT channel, ULONG TheData)
{
	if(scan==1&&RUNMODE==ERGO)y[channel] = (float)TheData;
	else y[channel] = TheData+y[channel];
	y[0]=y[1];
}

void TriggerObj::DoDoubleClickAction(void)
{
	INT_PTR result;
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);
	CPropShtTI propSheet(ShortName+" in Track ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);

	propSheet.m_page1.openbeamgate	= (long)OpenBeamgate;
	propSheet.m_page1.openscaler	= (long)OpenScaler;
	propSheet.m_page1.closebeamgate	= (long)CloseBeamgate;
	propSheet.m_page1.closescaler	= (long)CloseScaler;
	propSheet.m_page1.startbeamgate	= StartBeamgate;
	propSheet.m_page1.stopbeamgate	= StopBeamgate;
	
	propSheet.m_page1.m_notrigger = FALSE;
	propSheet.m_page1.m_steptrigger = FALSE;
	propSheet.m_page1.m_sweeptrigger= FALSE;

	if(TriggerType	== 0)propSheet.m_page1.m_notrigger = TRUE;
	if(TriggerType	== 1)propSheet.m_page1.m_steptrigger = TRUE;
	if(TriggerType	== 2)propSheet.m_page1.m_sweeptrigger = TRUE;

	propSheet.m_page3.m_slot= Slot;

	result = propSheet.DoModal();
	if(IDOK == result)
	{
		if(!pDocument->MeasurementRunning)
		{
			OpenBeamgate	= propSheet.m_page1.openbeamgate;
			OpenScaler		= propSheet.m_page1.openscaler;
			CloseBeamgate	= propSheet.m_page1.closebeamgate;
			CloseScaler		= propSheet.m_page1.closescaler;
			StartBeamgate	= propSheet.m_page1.startbeamgate;
			StopBeamgate	= propSheet.m_page1.stopbeamgate;

			if(propSheet.m_page1.m_notrigger)TriggerType		= 0;
			if(propSheet.m_page1.m_steptrigger)TriggerType		= 1;
			if(propSheet.m_page1.m_sweeptrigger)TriggerType		= 2;

			Slot = propSheet.m_page3.m_slot;
			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
			
		}
		else Beep(1000,100);
	}

	if(IDC_SHOWPROTONS == result)
	{
		BOOL ENDE = FALSE;
		USHORT track;
		int i;
		double Start,Stop;
		CString DoNew = "";
		CString TheObject="";
		CString TrackNo;
		CString ScalNo;
		CString XName,XUnit;
		BOOL Arith = FALSE;
		TrackNo.Format("%u",pTrack->MyPosition);
		
		XName = "Channel";
		XUnit = "";
		Start = (double)1;
		Stop  = (double)pTrack->Channels;

		if(pDataView==NULL)pDataView = new DataView(this);
		else pDataView->Reset();
		pDataView->SetWindowTitle(ShortName+" "+GetInfo()+" in Track ["+TrackNo+"]  of "+pDocument->GetTitle());


		pDataView->SetYTitle("Proton Counts","");
		POSITION pos=pDocument->ActionObjList.GetHeadPosition();
		int Channels = pTrack->Channels;
		
		track = pTrack->MyPosition;

		pDataView->SetXTitle(XName,XUnit);
		pDataView->SetTitle(pTrack->Isotope);
		for(i=0;i<pTrack->Channels;i++)
		{
			pDataView->AddDataPoint((float)(i*((Stop-Start)/(Channels-1))+Start),
									(float)y[i],
									(float)sqrt(fabs(y[i])));
		}
		pDataView->data_type = data_type;
		pDataView->RedrawNow();
	}
}

void TriggerObj::CopyObject(ActionObject* pSource)
{
	TriggerObj* pActionObject = (TriggerObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;

	OpenBeamgate	= pActionObject->OpenBeamgate;
	OpenScaler		= pActionObject->OpenScaler;
	CloseBeamgate	= pActionObject->CloseBeamgate;
	CloseScaler		= pActionObject->CloseScaler;
	TriggerType		= pActionObject->TriggerType;
}

CString TriggerObj::GetInfo(void)
{
	CString answer;
	CString in1;
	CString in2;
	CString Trigger;
	switch(TriggerType)
	{
	case 0:
		Trigger = "No Trigger";
		break;
	case 1:
		Trigger = "Step";
		break;
	case 2:
		Trigger = "Sweep";
		break;
	}
	in2.Format("delay %gs",OpenScaler/1000);
	if(OpenScaler==0)in2="";
	in1.Format("Meas %gs %s %s",(CloseScaler-OpenScaler)/1000,in2,Trigger);
	return in1;
}

void TriggerObj::Load(CArchive& ar)
{
	Resolution			= TriggerResolution;
	int i;
	Name = ReadString(ar);
	ShortName = __TriggerObj;
	ReadSeparator(ar);
		
	ReadStructBegin(ar);
	for (i=0;i<pTrack->Channels;i++)
	{
		y[i]   = ReadFloat(ar);
	}
	ReadStructEnd(ar);
	ReadSeparator(ar);
	OpenBeamgate	= ReadFloat(ar);
	ReadSeparator(ar);
	OpenScaler		= ReadFloat(ar);
	ReadSeparator(ar);
	CloseBeamgate	= ReadFloat(ar);
	ReadSeparator(ar);
	CloseScaler		= ReadFloat(ar);
	ReadSeparator(ar);
	StartBeamgate	= (USHORT)ReadFloat(ar);
	ReadSeparator(ar);
	StopBeamgate	= (USHORT)ReadFloat(ar);
	ReadSeparator(ar);
	TriggerType		= (int)ReadFloat(ar);

	ReadStructEnd(ar);
}

void TriggerObj::Save(CArchive& ar)
{
	USHORT i,j=0;
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);

	WriteStructBegin(ar);
	for (i=0;i<pTrack->Channels;i++)
	{
		WriteFloat(ar,y[i]);
		if(i!=(pTrack->Channels-1))WriteSeparator(ar);
		j++;
		if(j==5)
		{
			j = 0;
			WriteLine(ar);
		}
	}
	WriteStructEnd(ar);

	WriteSeparator(ar);
	WriteFloat(ar,OpenBeamgate);
	WriteSeparator(ar);
	WriteFloat(ar,OpenScaler);
	WriteSeparator(ar);
	WriteFloat(ar,CloseBeamgate);
	WriteSeparator(ar);
	WriteFloat(ar,CloseScaler);
	WriteStructEnd(ar);
	WriteFloat(ar,StartBeamgate);
	WriteSeparator(ar);
	WriteFloat(ar,StopBeamgate);
	WriteStructEnd(ar);
	WriteFloat(ar,TriggerType);
	WriteStructEnd(ar);

	WriteLine(ar);
}

void TriggerObj::MeasurementBeginAction(BOOL RUNMODE)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	USHORT i;
	if(RUNMODE==ERGO)for(i=0;i<MAXPOINTS;i++)y[i]=0;
	
	ResetLAMTimer(Slot);
	EnableLAMTimer(Slot);
	ResetLAMTrigger(pApp->Gelbebox2Slot);
	EnableLAMTrigger(pApp->Gelbebox2Slot);

	if(TriggerType != 0)SendFCamac(64+pApp->Gelbebox2Slot);
	SendFCamac(64+Slot);
	ReadCSRCamac();
	ReadENCLCamac();
}

void TriggerObj::MeasurementEndAction(void)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	SendFCamac(128+Slot);
	SendFCamac(128+pApp->Gelbebox2Slot);
	
	DisableLAMTimer(Slot);
	ResetLAMTimer(Slot);
	
	DisableLAMTrigger(pApp->Gelbebox2Slot);
	ResetLAMTrigger(pApp->Gelbebox2Slot);

	ReadENCLCamac();
}

void TriggerObj::TrackBeginAction(USHORT track)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	ResetLAMTimer(Slot);
	ReadCSRCamac();
	ReadENCLCamac();
	SendNAFCamac(pApp->ScalerSlot,0,9);
	ReadCSRCamac();
	ReadENCLCamac();
	SendNAFCamac(Slot,2,24);
}

void TriggerObj::TrackStepAction(USHORT step, USHORT track, USHORT scan)
{

	/********* for the protoncounter ******************/
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();

	ListDNAFCamac(SENDNAF,0,(UCHAR)ProtonSlot,(UCHAR)ProtonSubadd,(UCHAR)0);
	ListDNAFCamac(READD,0,0,0,0);
	pDocument->DataQueue[pDocument->DataQueueIndex].scan = scan;
	pDocument->DataQueue[pDocument->DataQueueIndex].step = step;
	pDocument->DataQueue[pDocument->DataQueueIndex].pAction = this;
	pDocument->DataQueueIndex++;
	ListDNAFCamac(SENDNAF,0,(UCHAR)ProtonSlot,(UCHAR)ProtonSubadd,(UCHAR)9);	
	/********* for the protoncounter ******************/

	if((TriggerType == 1)||((TriggerType == 2)&&(step==1)))
	{
		ListArmTrigger(pApp->Gelbebox2Slot);
	}

	if(OpenBeamgate!=0)
	{
		ListLoadTimer(Slot,OpenBeamgate);
		ListStartTimer(Slot);
	}
	
	if((OpenScaler!=0)&&(OpenScaler!=OpenBeamgate))
	{
		if((step>=StartBeamgate)&&(step<=StopBeamgate))ListOnBit(0,14);
		ListLoadTimer(Slot,OpenScaler-OpenBeamgate);
		ListStartTimer(Slot);
	}	
	
	if((step>=StartBeamgate)&&(step<=StopBeamgate))ListOnBit(0,14);
	ListDNAFCamac(SENDNAF,0,pApp->ScalerSlot,(UCHAR)0,(UCHAR)9);
	ListLoadTimer(Slot,CloseScaler-OpenScaler);
	ListStartTimer(Slot);

	ListResetLAMTimer(Slot);
	ListDNAFCamac(READENCL,0,0,0,0);
	ListOffBit(0,14);
}

/****************   Methods for Class PremaVoltageObj  ***********************/

void PremaVoltageObj::DoConfigureAction(void)
{
	CString Track;
	Track.Format("%u",pTrack->MyPosition);
	Title=(ShortName+" in Track ["+Track+"]  of "+pDocument->GetTitle());
	CPropShtPR  propSheet(Title,AfxGetMainWnd(),0);
	propSheet.m_page3.m_subaddress	= SubAddress;
	propSheet.m_page3.m_slot		= Slot;
	propSheet.m_page1.m_points		= FALSE;
	propSheet.m_page1.m_lines		= FALSE;
	propSheet.m_page1.m_stairs		= FALSE;
	propSheet.m_page3.m_delay		= (long)DelayBeforeMeas;
	if(data_type == 0) propSheet.m_page1.m_points = TRUE;
	if(data_type == 1) propSheet.m_page1.m_lines  = TRUE;
	if(data_type == 2) propSheet.m_page1.m_stairs = TRUE;
	if(DispMonitorMode == 1) propSheet.m_page1.m_autoopen = TRUE;
	propSheet.m_page1.m_autoresize = ShowExpected;
	if(IDOK == propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			if(propSheet.m_page1.m_points)data_type = 0;
			if(propSheet.m_page1.m_lines) data_type = 1;
			if(propSheet.m_page1.m_stairs)data_type = 2;
			if(propSheet.m_page1.m_autoopen)DispMonitorMode = 1;
			if(!propSheet.m_page1.m_autoopen)DispMonitorMode = 0;
			SubAddress	= propSheet.m_page3.m_subaddress;
			Slot		= propSheet.m_page3.m_slot;
			ShowExpected = propSheet.m_page1.m_autoresize;
			DelayBeforeMeas = propSheet.m_page3.m_delay;
			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
		}
		else Beep(1000,100);
	}
}

void PremaVoltageObj::DoDoubleClickAction(void)
{
	if(NumOfSamples>0)
	{
		int i;
		CString TrackNo;
		TrackNo.Format("%u",pTrack->MyPosition);
		if(pDataView==NULL)pDataView = new DataView(this);
		else pDataView->Reset();
		pDataView->SetWindowTitle(ShortName+" in Track ["+TrackNo+"]  of "+pDocument->GetTitle());
		pDataView->SetDrawType((int)0,TRUE);

		pDataView->SetYTitle("Voltage measured in "+ShortName,"V");
		pDataView->SetXTitle("Sample no.","");
		pDataView->SetTitle(pTrack->Isotope);
		for(i=0;i<NumOfSamples;i++)
		{
			pDataView->AddDataPoint((float)i+1,
									(float)Voltage[i],
									(float)(1e-6));
		}
		pDataView->RedrawNow();
	}
}

void PremaVoltageObj::CopyObject(ActionObject* pSource)
{
	USHORT i;
	PremaVoltageObj* pActionObject = (PremaVoltageObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;

	Factor				= pActionObject->Factor;
	DelayBeforeMeas		= pActionObject->DelayBeforeMeas;
	for(i=0;i<100;i++)Voltage[i]= pActionObject->Voltage[i];
	NumOfSamples		= pActionObject->NumOfSamples;
}

CString PremaVoltageObj::GetInfo(void)
{
	CString answer;
	if (pTrack->RealScans>0&&NumOfSamples>0)
	{
		answer.Format("LAST: %g",Voltage[NumOfSamples-1]);
	}
	else answer = "NOT MEASURED";
	return answer;
}
void PremaVoltageObj::Load(CArchive& ar)
{
	int i=0;
	unsigned char TheChar;
	Name = ReadString(ar);
	ShortName = __PremaVoltageObj;
	ReadSeparator(ar);
	if (pDocument->Version > 7)
	{
		Factor = ReadFloat(ar);
		DelayBeforeMeas = ReadFloat(ar);
	}
	if (pTrack->RealScans>0)
	{
		do
		{
		Voltage[i]=ReadFloat(ar);
		++i;
		TheChar = ReadChar(ar);
		} while (TheChar ==',');
		NumOfSamples=i;
	}
	else
	{
		ReadStructBegin(ar);
		ReadStructEnd(ar);
	}
	ReadSeparator(ar);
	
}

void PremaVoltageObj::Save(CArchive& ar)
{
	USHORT i,j=0;
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);

	WriteFloat(ar,Factor);
	WriteSeparator(ar);
	WriteFloat(ar,DelayBeforeMeas);
	WriteSeparator(ar);
	WriteStructBegin(ar);
	if (pTrack->RealScans>0)
	{
		for(i=0;i<NumOfSamples;i++)
		{
			WriteFloat(ar,Voltage[i]);
			if(i!=(NumOfSamples-1))WriteSeparator(ar);
			j++;
			if(j==5)
			{
				j = 0;
				WriteLine(ar);
			}
		} 
	}	
	WriteStructEnd(ar);
	WriteLine(ar);
	WriteStructEnd(ar);
	WriteLine(ar);	
}

void PremaVoltageObj::TrackBeginAction(USHORT track)
{
	LastChannel = (USHORT)-1;

	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();

	ListDNAFCamac(SENDDNAF,(ULONG)(~(0)),pApp->HpSlot,0,16);
	//ListDNAFCamac(SENDNAF,0,pApp->HvSlot,(UCHAR)1,(UCHAR)10);
	//ListDNAFCamac(SENDDNAF,8388608,pApp->HvSlot,(UCHAR)0,(UCHAR)16);
	ListDNAFCamac(SENDDNAF,0,pApp->HvSlot,(UCHAR)0,(UCHAR)16);
	ListDelayCamac(pApp->PresetSlot,(USHORT)DelayBeforeMeas);

	ListDNAFCamac(SENDNAF,0,0,0,37);

	ListDNAFCamac(SENDNAF,0,Slot,1,28);
	ListDNAFCamac(SENDNAF,0,Slot,0,28);

	ListDNAFCamac(SENDNAF,0,Slot,1,2);
	ListDNAFCamac(READD,0,0,0,0);
	
	ListDNAFCamac(SENDNAF,0,Slot,0,2);
	ListDNAFCamac(READD,0,0,0,0);

	ListDNAFCamac(SENDNAF,0,0,0,32);
	
	pDocument->DataQueue[pDocument->DataQueueIndex].scan = 1;
	pDocument->DataQueue[pDocument->DataQueueIndex].step = 1;
	pDocument->DataQueue[pDocument->DataQueueIndex].pAction = this;
	pDocument->DataQueueIndex++;
	
	pDocument->DataQueue[pDocument->DataQueueIndex].scan = 1;
	pDocument->DataQueue[pDocument->DataQueueIndex].step = 1;
	pDocument->DataQueue[pDocument->DataQueueIndex].pAction = this;
	pDocument->DataQueueIndex++;
}

double PremaVoltageObj::GetY(USHORT channel)
{
	return (double) Voltage[channel];
}

double PremaVoltageObj::GetYErr(USHORT channel)
{
	return (double)1e-3;
}

void PremaVoltageObj::TakeData(BOOL RUNMODE, USHORT scan, USHORT channel, ULONG TheData)
{	
	// BCD-Codierung des Prema-InputRegisters (experimentell bestimmt):
	//   Subregister 1 : 1111 1111 1111 1111
	//                    X?   6.   5.   4.
	//   Subregister 0 : 1111 1111 1111 1111
	//                    3.   2.   1.   +/-
	// dabei bedeuten die Ziffern die jeweilige Stelle (10er Potenz):
	// Ziffernfolge fuer die Spannung: 123456 !
	// Frage was passiert mit den ersten 4 Bits in w1 (X?) ?
	// (sind immer 1 oder 5 ?) --> eventuell MSB == 1/2 Stelle ?

	USHORT   Mask;
	int    Digit[8];
	long   l=0;
	int    i;
	double	Volt;
	double Expo;


	if(channel == LastChannel)
	{
		LastChannel = channel;
		w0	= TheData;

		w0 = ~w0;                    // invert the two words (input reg. has negativ logic)
		w1 = ~w1;
		Mask = 1+2+4+8;              // mask for getting a BCD-digit out of 16-bit word
		Digit[0] = (w1 & 1+2+4);
		Mask = Mask << 4;
		for (i=1; i<4; i++)
		{
			Digit[i] = (int) ((w1&Mask) >> (4*i)); 
			Mask = Mask << 4;        // next digit
		}
		Mask = 1+2+4+8;
		for (i=0; i<4; i++)
		{
			Digit[4+i] = ((w0&Mask) >> (4*i)); 
			Mask = Mask << 4;        // next digit
		}

		Volt =	Digit[1]+
				Digit[2]*10+
				Digit[3]*100+
				Digit[4]*1000+
				Digit[5]*10000+
				Digit[6]*100000+
				(Digit[7]&1)*1000000;

		Expo = PowerOf10(Digit[0]);
		if((Digit[7]&8)) Expo = 1/Expo;
		Volt = Volt * Expo/1e6;

		if((Digit[7]&2)) Volt = -Volt;
		
		Voltage[NumOfSamples]=Volt;
		NumOfSamples++;
	}
	else
	{
		w1			= TheData;
		LastChannel = channel;
	}
}

void PremaVoltageObj::MeasurementBeginAction(BOOL RUNMODE)
{
	LastChannel = (USHORT)-1;
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();

	if(RUNMODE==ERGO) NumOfSamples = 0;
	
	SendFCamac(64+pApp->PresetSlot);
	SendNAFCamac(pApp->PresetSlot,0,26);
}

void PremaVoltageObj::MeasurementEndAction(void)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	SendFCamac(128+pApp->PresetSlot);
	SendNAFCamac(pApp->PresetSlot,0,24);
}

/****************   Methods for Class FlukeVoltageObj  ***********************/

void FlukeVoltageObj::DoConfigureAction(void)
{
	CString Track;
	Track.Format("%u",pTrack->MyPosition);
	Title=(ShortName+" "+GetInfo()+" in Track ["+Track+"]  of "+pDocument->GetTitle());
	CPropShtPR  propSheet(Title,AfxGetMainWnd(),0);
	propSheet.m_page3.m_subaddress	= SubAddress;
	propSheet.m_page3.m_slot		= Slot;
	propSheet.m_page1.m_points		= FALSE;
	propSheet.m_page1.m_lines		= FALSE;
	propSheet.m_page1.m_stairs		= FALSE;
	propSheet.m_page3.m_delay		= (long)DelayBeforeMeas;
	if(data_type == 0) propSheet.m_page1.m_points = TRUE;
	if(data_type == 1) propSheet.m_page1.m_lines  = TRUE;
	if(data_type == 2) propSheet.m_page1.m_stairs = TRUE;
	if(DispMonitorMode == 1) propSheet.m_page1.m_autoopen = TRUE;
	propSheet.m_page1.m_autoresize = ShowExpected;
	if(IDOK == propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			if(propSheet.m_page1.m_points)data_type = 0;
			if(propSheet.m_page1.m_lines) data_type = 1;
			if(propSheet.m_page1.m_stairs)data_type = 2;
			if(propSheet.m_page1.m_autoopen)DispMonitorMode = 1;
			if(!propSheet.m_page1.m_autoopen)DispMonitorMode = 0;
			SubAddress	= propSheet.m_page3.m_subaddress;
			Slot		= propSheet.m_page3.m_slot;
			ShowExpected = propSheet.m_page1.m_autoresize;
			DelayBeforeMeas = propSheet.m_page3.m_delay;
			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
		}
		else Beep(1000,100);
	}
}

void FlukeVoltageObj::DoDoubleClickAction(void)
{
	int i;
	CString TrackNo;
	TrackNo.Format("%u",pTrack->MyPosition);
	if(pDataView==NULL)pDataView = new DataView(this);
	else pDataView->Reset();
	pDataView->SetWindowTitle(ShortName+" in Track ["+TrackNo+"]  of "+pDocument->GetTitle());
	pDataView->SetDrawType((int)0,TRUE);

	pDataView->SetYTitle("Voltage measured in "+ShortName,"V");
	pDataView->SetXTitle("Sample no.","");
	pDataView->SetTitle(pTrack->Isotope);
	for(i=0;i<NumOfSamples;i++)
	{
		pDataView->AddDataPoint((float)i+1,
								(float)Voltage[i],
								(float)0);
	}
	pDataView->RedrawNow();
}

void FlukeVoltageObj::CopyObject(ActionObject* pSource)
{
	USHORT i;
	FlukeVoltageObj* pActionObject = (FlukeVoltageObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;

	Factor				= pActionObject->Factor;
	DelayBeforeMeas		= pActionObject->DelayBeforeMeas;
	for(i=0;i<100;i++)Voltage[i]= pActionObject->Voltage[i];
	NumOfSamples		= pActionObject->NumOfSamples;
}

CString FlukeVoltageObj::GetInfo(void)
{
	CString answer;
	if (pTrack->RealScans>0)
	{
		answer.Format("LAST: %g",Voltage[NumOfSamples-1]);
	}
	else answer = "NOT MEASURED";
	return answer;
}

void FlukeVoltageObj::Load(CArchive& ar)
{
	int i=0;
	unsigned char TheChar;
	Name = ReadString(ar);
	ShortName = __FlukeVoltageObj;
	ReadSeparator(ar);
	if (pDocument->Version > 7)
	{
		Factor = ReadFloat(ar);
		DelayBeforeMeas = ReadFloat(ar);
	}
	if (pTrack->RealScans>0)
	{
		do
		{
		Voltage[i]=ReadFloat(ar);
		++i;
		TheChar = ReadChar(ar);
		} while (TheChar ==',');
		NumOfSamples=i;
	}
	else
	{
		ReadStructBegin(ar);
		ReadStructEnd(ar);
	}
	ReadSeparator(ar);	
}

void FlukeVoltageObj::Save(CArchive& ar)
{
	USHORT i,j=0;
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);

	WriteFloat(ar,Factor);
	WriteSeparator(ar);
	WriteFloat(ar,DelayBeforeMeas);
	WriteSeparator(ar);
	WriteStructBegin(ar);
	if (pTrack->RealScans>0)
	{
		for(i=0;i<NumOfSamples;i++)
		{
			WriteFloat(ar,Voltage[i]);
			if(i!=(NumOfSamples-1))WriteSeparator(ar);
			j++;
			if(j==5)
			{
				j = 0;
				WriteLine(ar);
			}
		} 
	}
	WriteStructEnd(ar);
	WriteLine(ar);
	WriteStructEnd(ar);
	WriteLine(ar);
}

/****************   Methods for Class IsoldeNetVoltageObj  ***********************/

void IsoldeNetVoltageObj::DoConfigureAction(void)
{
}

void IsoldeNetVoltageObj::DoDoubleClickAction(void)
{
	if(NumOfSamples>0)
	{
		int i;
		CString TrackNo;
		TrackNo.Format("%u",pTrack->MyPosition);
		if(pDataView==NULL)pDataView = new DataView(this);
		else pDataView->Reset();
		pDataView->SetWindowTitle(ShortName+" in Track ["+TrackNo+"]  of "+pDocument->GetTitle());
		pDataView->SetDrawType((int)0,TRUE);

		pDataView->SetYTitle("Voltage measured in "+ShortName,"V");
		pDataView->SetXTitle("Sample no.","");
		pDataView->SetTitle(pTrack->Isotope);
		for(i=0;i<NumOfSamples;i++)
		{
			pDataView->AddDataPoint((float)i+1,
									(float)Voltage[i],
									(float)0);
		}
		pDataView->RedrawNow();
	}
}

void IsoldeNetVoltageObj::CopyObject(ActionObject* pSource)
{
	USHORT i;
	IsoldeNetVoltageObj* pActionObject = (IsoldeNetVoltageObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;

	Factor				= pActionObject->Factor;
	DelayBeforeMeas		= pActionObject->DelayBeforeMeas;
	for(i=0;i<100;i++)Voltage[i]= pActionObject->Voltage[i];
	NumOfSamples		= pActionObject->NumOfSamples;
}

CString IsoldeNetVoltageObj::GetInfo(void)
{
	CString answer;
	if (pTrack->RealScans>0)
	{
		answer.Format("LAST: %.3f",Voltage[NumOfSamples-1]);
	}
	else answer = "NOT MEASURED";
	return answer;
}

void IsoldeNetVoltageObj::Load(CArchive& ar)
{
	BOOL ERR = FALSE;
	int i=0;
	unsigned char TheChar;
	CString TheString;
	Name = ReadString(ar);
	ShortName = __IsoldeNetVoltageObj;
	ReadSeparator(ar);
	if (pDocument->Version > 7)
	{
		Factor = ReadFloat(ar);
		DelayBeforeMeas = ReadFloat(ar);
	}
	if (pTrack->RealScans>0)
	{
		do
		{
			Voltage[i]=ReadFloat(ar);
			if(Voltage[i]==-999999)
			{
				ERR = TRUE;
				i = 0;
			}
			++i;
			TheChar = ReadChar(ar);
		} while (TheChar ==','&&!ERR);
		NumOfSamples = i;
	}
	else
	{
		ReadStructBegin(ar);
		ReadStructEnd(ar);
	}
	ReadSeparator(ar);	
}

void IsoldeNetVoltageObj::Save(CArchive& ar)
{
	USHORT i,j=0;
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);

	WriteFloat(ar,Factor);
	WriteSeparator(ar);
	WriteFloat(ar,DelayBeforeMeas);
	WriteSeparator(ar);
	WriteStructBegin(ar);
	if (pTrack->RealScans>0)
	{
		for(i=0;i<NumOfSamples;i++)
		{
			WriteFloat(ar,Voltage[i]);
			if(i!=(NumOfSamples-1))WriteSeparator(ar);
			j++;
			if(j==5)
			{
				j = 0;
				WriteLine(ar);
			}
		} 
	}
	WriteStructEnd(ar);
	WriteLine(ar);
	WriteStructEnd(ar);
	WriteLine(ar);
}

void IsoldeNetVoltageObj::MeasurementBeginAction(BOOL RUNMODE)
{
	if(RUNMODE==ERGO)NumOfSamples = 0;
}

void IsoldeNetVoltageObj::TrackBeginAction(USHORT track)
{
	double value;
	CMCPforNTApp* pMyApp = (CMCPforNTApp*)AfxGetApp();

	//
	// Get Protons value
	//
	value = pMyApp->GetIsoProtons();

	Voltage[NumOfSamples] = value*Factor; 
	NumOfSamples++;
}

/****************   Methods for Class SiclReaderObj  ***********************/

void SiclReaderObj::DoConfigureAction(void)
{
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);
	CPropShtSR propSheet(ShortName+" in Track ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);

	propSheet.m_page1.m_sendstring = SICLQuestion;
	propSheet.m_page3.m_gpib = SICLAddress;
	if(propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			SICLQuestion = propSheet.m_page1.m_sendstring;
			SICLAddress = propSheet.m_page3.m_gpib;
			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
		}
		else Beep(1000,100);
	}
}

void SiclReaderObj::DoDoubleClickAction(void)
{
	if(NumOfSamples>0)
	{
		int i;
		CString TrackNo;
		TrackNo.Format("%u",pTrack->MyPosition);
		if(pDataView==NULL)pDataView = new DataView(this);
		else pDataView->Reset();
		pDataView->SetWindowTitle(ShortName+" in Track ["+TrackNo+"]  of "+pDocument->GetTitle());
		pDataView->SetDrawType((int)0,TRUE);

		pDataView->SetYTitle("Data measured in "+ShortName,"");
		pDataView->SetXTitle("Sample no.","");
		pDataView->SetTitle(pTrack->Isotope);
		for(i=0;i<NumOfSamples;i++)
		{
			pDataView->AddDataPoint((float)i+1,
									(float)Data[i],
									(float)0);
		}
		pDataView->RedrawNow();
	}
}

void SiclReaderObj::CopyObject(ActionObject* pSource)
{
	SiclReaderObj* pActionObject = (SiclReaderObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;
	Gpib			= pActionObject->Gpib;
	SICLAddress		= pActionObject->SICLAddress;
	SICLQuestion	= pActionObject->SICLQuestion;
	for(int i=0;i<100;i++)Data[i]= pActionObject->Data[i];
	NumOfSamples		= pActionObject->NumOfSamples;
}

CString SiclReaderObj::GetInfo(void)
{
	CString answer;
	if (pTrack->RealScans>0 && NumOfSamples>0)
	{
		answer.Format("LAST: %.3f",Data[NumOfSamples-1]);
	}
	else answer = "NOT MEASURED";
	return answer;
}

void SiclReaderObj::Load(CArchive& ar)
{
	BOOL ERR = FALSE;
	int i=0;
	unsigned char TheChar;
	CString TheString;
	Name = ReadString(ar);
	ShortName = __SiclReaderObj;
	ReadSeparator(ar);
	SICLAddress = ReadString(ar);
	SICLQuestion = ReadString(ar);
	if (pTrack->RealScans>0)
	{
		do
		{
			Data[i]=ReadFloat(ar);
			if(Data[i]==-999999)
			{
				ERR = TRUE;
				i = 0;
			}
			++i;
			TheChar = ReadChar(ar);
		} while (TheChar ==','&&!ERR);
		NumOfSamples = i;
	}
	else
	{
		ReadStructBegin(ar);
		ReadStructEnd(ar);
	}
	ReadSeparator(ar);	
}

void SiclReaderObj::Save(CArchive& ar)
{
	USHORT i,j=0;
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);

	WriteString(ar,SICLAddress);
	WriteSeparator(ar);
	WriteString(ar,SICLQuestion);
	WriteSeparator(ar);
	WriteStructBegin(ar);
	if (pTrack->RealScans>0)
	{
		for(i=0;i<NumOfSamples;i++)
		{
			WriteFloat(ar,Data[i]);
			if(i!=(NumOfSamples-1))WriteSeparator(ar);
			j++;
			if(j==5)
			{
				j = 0;
				WriteLine(ar);
			}
		} 
	}
	WriteStructEnd(ar);
	WriteLine(ar);
	WriteStructEnd(ar);
	WriteLine(ar);
}

void SiclReaderObj::MeasurementBeginAction(BOOL RUNMODE)
{
	if(RUNMODE==ERGO) NumOfSamples = 0;

	/* disable trigger bit */
	OffBit(0,SubAddress);

	CT2A address(SICLAddress);

	if(SiclHandle <= 0 && !SICLAddress.IsEmpty())
	{
		SiclHandle = iopen (address);
		itimeout (SiclHandle, 10000);
	}
}

void SiclReaderObj::MeasurementEndAction(void)
{
	if(SiclHandle > 0)
	{
		/* Close SICL handle */
		iclose (SiclHandle);

		SiclHandle = 0;
	}
}

void SiclReaderObj::TrackBeginAction(USHORT track)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	
	/* disable trigger bit */
	ListOffBit(0,SubAddress);

	if(SiclHandle > 0)
	{
		CT2A question(SICLQuestion);

		/* trailing line feed */ 
		strcat(question,"\n");

		/* Take measurement */
		iprintf (SiclHandle,question);
	}

	if(DelayBeforeMeas > 0)ListDelayCamac(pApp->PresetSlot,(USHORT)DelayBeforeMeas);

	/* pulse trigger bit */
	ListOnBit(0,SubAddress);
	ListDelayCamac(pApp->PresetSlot,1);
	ListOffBit(0,SubAddress);

	if(DelayAfterMeas > 0)ListDelayCamac(pApp->PresetSlot,(USHORT)DelayAfterMeas);
}

void SiclReaderObj::TrackEndAction(USHORT track,USHORT scansdone)
{
	int result;
	int header;

	if(SiclHandle > 0)
	{
		/* read all available data */
		CT2A question("R?\n");

		/* fetch data */
		iprintf (SiclHandle,question);
	
		/* Convert and store the results */
		result = iscanf (SiclHandle,"#%d%lf", &header, &Data[NumOfSamples++]);

		TRACE3("SiclReaderObj::TrackEndAction iscanf = %d header = %d NumOfSamples = %d\n", result, header, NumOfSamples );
	}
}

/****************   Methods for Class SiclStepObj  ***********************/

void SiclStepObj::DoConfigureAction(void)
{
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);
	CPropShtSR propSheet(ShortName+" in Track ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);

	propSheet.m_page1.m_sendstring = SICLQuestion;
	propSheet.m_page3.m_gpib = SICLAddress;
	if(propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			SICLQuestion = propSheet.m_page1.m_sendstring;
			SICLAddress = propSheet.m_page3.m_gpib;
			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
		}
		else Beep(1000,100);
	}
}

double SiclStepObj::GetY(USHORT channel)
{
	return (double) Data[channel];
}

double SiclStepObj::GetYErr(USHORT channel)
{
	return (double)1e-5;
}

void SiclStepObj::DoDoubleClickAction(void)
{
	int i;
	double Start,Stop;
	ActionObject* pActionObject;
	CString TheObject="";
	CString ScalNo;
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);
	if(pDataView==NULL)pDataView = new DataView(this);
	else pDataView->Reset();
	pDataView->SetWindowTitle(ShortName+" in Track ["+TrackNo+"]  of "+pDocument->GetTitle());
	pDataView->SetDrawType((int)0,FALSE);

	pDataView->SetYTitle("Voltage","V");
	POSITION pos=pDocument->ActionObjList.GetHeadPosition();
	int Channels = pTrack->Channels;
	do
	{
		pActionObject = (ActionObject*)pDocument->ActionObjList.GetNext(pos);
		TheObject     = pActionObject->GetName();
	}
	while ((pActionObject->pTrack->MyPosition != this->pTrack->MyPosition)||((pos!=NULL)&&(TheObject!=__HP_VoltageSweepObj)&&(TheObject!=__Line_VoltageSweepObj)));
	if((pos!=NULL)&&(TheObject==__HP_VoltageSweepObj))
	{
		HP_VoltageSweepObj* pAction = (HP_VoltageSweepObj*)pActionObject;
		Start = pAction->Sweep_Start;
		Stop  = pAction->Sweep_Stop;
		XName = "HP-Voltage";
		XUnit = "V";
		pDataView->SetXTitle(XName,XUnit);
	}

	if((pos!=NULL)&&(TheObject==__Line_VoltageSweepObj))
	{
		Line_VoltageSweepObj* pAction = (Line_VoltageSweepObj*)pActionObject;
		Start = pAction->Sweep_Start;
		Stop  = pAction->Sweep_Stop;
		XName = "Line-Voltage";
		XUnit = "V";
		pDataView->SetXTitle(XName,XUnit);
	}

	pDataView->SetTitle(pTrack->Isotope);
	for(i=0;i<Channels;i++)
	{
		pDataView->AddDataPoint((float)(i*((Stop-Start)/(Channels-1))+Start),
								(float)Data[i],
								(float)(1e-5));
	}
	pDataView->RedrawNow();
}

void SiclStepObj::CopyObject(ActionObject* pSource)
{
	SiclStepObj* pActionObject = (SiclStepObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;
	Gpib			= pActionObject->Gpib;
	SICLAddress		= pActionObject->SICLAddress;
	SICLQuestion	= pActionObject->SICLQuestion;
	for(int i=0;i<100;i++)Data[i]= pActionObject->Data[i];
	NumOfSamples		= pActionObject->NumOfSamples;
}

CString SiclStepObj::GetInfo(void)
{
	CString answer;
	CString in1;
	in1.Format("%g",DelayBeforeMeas);
	answer = "PreDelay: "+in1+" ms";
	return answer;
}

void SiclStepObj::Load(CArchive& ar)
{
	BOOL ERR = FALSE;
	int i=0;
	unsigned char TheChar;
	CString TheString;
	Name = ReadString(ar);
	ShortName = __SiclStepObj;
	ReadSeparator(ar);
	SICLAddress = ReadString(ar);
	SICLQuestion = ReadString(ar);
	if (pTrack->RealScans>0)
	{
		do
		{
			Data[i]=ReadFloat(ar);
			if(Data[i]==-999999)
			{
				ERR = TRUE;
				i = 0;
			}
			++i;
			TheChar = ReadChar(ar);
		} while (TheChar ==','&&!ERR);
		NumOfSamples = i;
	}
	else
	{
		ReadStructBegin(ar);
		ReadStructEnd(ar);
	}
	ReadSeparator(ar);	
}

void SiclStepObj::Save(CArchive& ar)
{
	USHORT i,j=0;
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);

	WriteString(ar,SICLAddress);
	WriteSeparator(ar);
	WriteString(ar,SICLQuestion);
	WriteSeparator(ar);
	WriteStructBegin(ar);
	if (pTrack->RealScans>0)
	{
		for(i=0;i<NumOfSamples;i++)
		{
			WriteFloat(ar,Data[i]);
			if(i!=(NumOfSamples-1))WriteSeparator(ar);
			j++;
			if(j==5)
			{
				j = 0;
				WriteLine(ar);
			}
		} 
	}
	WriteStructEnd(ar);
	WriteLine(ar);
	WriteStructEnd(ar);
	WriteLine(ar);
}

void SiclStepObj::MeasurementBeginAction(BOOL RUNMODE)
{
	if(RUNMODE==ERGO) NumOfSamples = 0;

	/* disable trigger bit */
	OffBit(0,SubAddress);

	CT2A address(SICLAddress);

	if(SiclHandle <= 0 && !SICLAddress.IsEmpty())
	{
		SiclHandle = iopen (address);
		itimeout (SiclHandle, 10000);
	}
}

void SiclStepObj::MeasurementEndAction(void)
{
	if(SiclHandle > 0)
	{
		/* Close SICL handle */
		iclose (SiclHandle);

		SiclHandle = 0;
	}
}

void SiclStepObj::TrackBeginAction(USHORT track)
{
	int i;
	BOOL ENDE = FALSE;
	CString XName,XUnit;
	CString YTitle,YUnit;
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	
	if(DispMonitorMode == 1)
	{
		if(pApp->pMonitorView==NULL)pApp->pMonitorView = new MonitorView();

		double Start,Stop;
		CString DoNew = "";
		ActionObject* pActionObject;
		CString TheObject="";
		CString TrackNo;
		
		TrackNo.Format("%u",pTrack->MyPosition);
		
		XName = "Channel";
		XUnit = "";
		Start = (double)1;
		Stop  = (double)pTrack->Channels;
		 
		pApp->pMonitorView->SetWindowTitle("Monitor for Track ["+TrackNo+"]  of "+pDocument->GetTitle());

		for(i=0;i<4;i++)pApp->pMonitorView->ActualIndex[i] = 0;

		POSITION pos=pDocument->ActionObjList.GetHeadPosition();
		int Channels = pTrack->Channels;
		
		pActionObject = (ActionObject*)pDocument->ActionObjList.GetHead();
		pos = pDocument->ActionObjList.GetHeadPosition();
		do
		{
			pActionObject = (ActionObject*)pDocument->ActionObjList.GetNext(pos);
			if (pActionObject!=NULL&&pActionObject->pTrack->MyPosition == track)
			{
				TheObject     = pActionObject->GetName();
				if((TheObject==__HP_VoltageSweepObj)||	
				(TheObject==__Line_VoltageSweepObj)||
				(TheObject==__HP8660B_FrequencySweepObj)||
				(TheObject==__HP3325B_FrequencySweepObj)||
				(TheObject==__HP3325B2_FrequencySweepObj)||
				(TheObject==__RS_FrequencySweepObj))ENDE = TRUE;
			}
		}
		while (pos!=NULL&&ENDE==FALSE);

		if(pos!=NULL)
		{
			if((TheObject==__HP_VoltageSweepObj)||(TheObject==__Line_VoltageSweepObj))
			{
				if(TheObject==__HP_VoltageSweepObj)
				{
					CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
					HP_VoltageSweepObj* pAction1 = (HP_VoltageSweepObj*)pActionObject;
					Start	= pAction1->Sweep_Start;
					Stop	= pAction1->Sweep_Stop;
					XName	= "HPVoltage";
					XUnit	= "V";
				}
				if(TheObject==__Line_VoltageSweepObj)
				{
					Line_VoltageSweepObj* pAction2 = (Line_VoltageSweepObj*)pActionObject;
					Start	= pAction2->Sweep_Start;
					Stop	= pAction2->Sweep_Stop;
					XName	= "LineVoltage";
					XUnit	= "V";
				}
				if (Start==Stop)
				{
					XName = "Channel";
					XUnit = "";
					Start = (double)1;
					Stop  = (double)pTrack->Channels;
				}
			}
		}
		YTitle = "Voltage [V]";
		YUnit="V";
		{
			if(pApp->pMonitorView->MonitorFree())
				pApp->pMonitorView->AddView(Channels,XName,XUnit,YTitle,YUnit,(float)Start,(float)Stop,(ActionObject*)this,pTrack->RealScans,ShowExpected);
		
			else 
			{
				DispMonitorMode = 0;
				AfxMessageBox("Too much monitors defined,\n\r disabled last",MB_OK,0);
			}
		}
	}

	if(SiclHandle > 0)
	{
		CT2A question(SICLQuestion);

		/* trailing line feed */ 
		strcat(question,"\n");

		/* Take measurement */
		iprintf (SiclHandle,question);
	}
}

void SiclStepObj::TrackStepAction(USHORT step, USHORT track, USHORT scan)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();

	if(DelayBeforeMeas > 0)ListDelayCamac(pApp->PresetSlot,(USHORT)DelayBeforeMeas);

	/* pulse trigger bit */
	ListOnBit(0,SubAddress);
	ListDelayCamac(pApp->PresetSlot,1);
	ListOffBit(0,SubAddress);

	if(DelayAfterMeas > 0)ListDelayCamac(pApp->PresetSlot,(USHORT)DelayAfterMeas);

	NumOfSamples = step + 1;
}

void SiclStepObj::TrackEndAction(USHORT track,USHORT scansdone)
{
	int result;
	int header;

	if(SiclHandle > 0)
	{
		/* read all available data */
		CT2A question("R?\n");

		/* fetch data */
		iprintf (SiclHandle,question);
	
		/* Convert and store the results */
		result = iscanf (SiclHandle,"#%d%,4000lf", &header, &Data[0]);

		TRACE3("SiclStepObj::TrackEndAction iscanf = %d header = %d NumOfSamples = %d\n", result, header, NumOfSamples );
	}
}

/****************   Methods for Class KepcoEichungVoltageObj  ***********************/

void KepcoEichungVoltageObj::DoConfigureAction(void)
{
	CString Track;
	Track.Format("%u",pTrack->MyPosition);
	Title=(ShortName+" "+GetInfo()+" in Track ["+Track+"]  of "+pDocument->GetTitle());
	CPropShtPR  propSheet(Title,AfxGetMainWnd(),0);
	propSheet.m_page3.m_subaddress	= SubAddress;
	propSheet.m_page3.m_slot		= Slot;
	propSheet.m_page1.m_points		= FALSE;
	propSheet.m_page1.m_lines		= FALSE;
	propSheet.m_page1.m_stairs		= FALSE;
	propSheet.m_page3.m_delay		= (long)DelayBeforeMeas;
	if(data_type == 0) propSheet.m_page1.m_points = TRUE;
	if(data_type == 1) propSheet.m_page1.m_lines  = TRUE;
	if(data_type == 2) propSheet.m_page1.m_stairs = TRUE;
	if(DispMonitorMode == 1) propSheet.m_page1.m_autoopen = TRUE;
	propSheet.m_page1.m_autoresize = ShowExpected;
	if(IDOK == propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			if(propSheet.m_page1.m_points)data_type = 0;
			if(propSheet.m_page1.m_lines) data_type = 1;
			if(propSheet.m_page1.m_stairs)data_type = 2;
			if(propSheet.m_page1.m_autoopen)DispMonitorMode = 1;
			if(!propSheet.m_page1.m_autoopen)DispMonitorMode = 0;
			SubAddress	= propSheet.m_page3.m_subaddress;
			Slot		= propSheet.m_page3.m_slot;
			ShowExpected = propSheet.m_page1.m_autoresize;
			DelayBeforeMeas = propSheet.m_page3.m_delay;
			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
		}
		else Beep(1000,100);
	}
}

double KepcoEichungVoltageObj::GetY(USHORT channel)
{
	return (double) Voltage[channel];
}

double KepcoEichungVoltageObj::GetYErr(USHORT channel)
{
	return (double)1e-5;
}

void KepcoEichungVoltageObj::TakeData(BOOL RUNMODE, USHORT scan, USHORT channel, ULONG TheData)
{	
	// BCD-Codierung des Prema-InputRegisters (experimentell bestimmt):
	//   Subregister 1 : 1111 1111 1111 1111
	//                    X?   6.   5.   4.
	//   Subregister 0 : 1111 1111 1111 1111
	//                    3.   2.   1.   +/-
	// dabei bedeuten die Ziffern die jeweilige Stelle (10er Potenz):
	// Ziffernfolge fuer die Spannung: 123456 !
	// Frage was passiert mit den ersten 4 Bits in w1 (X?) ?
	// (sind immer 1 oder 5 ?) --> eventuell MSB == 1/2 Stelle ?

	USHORT   Mask,i;
	int    Digit[8];
	long   l=0;
	double	Volt;
	double  Expo;

	if(channel == LastChannel)
	{
		w0	= TheData;

		w0 = ~w0;                    // invert the two words (input reg. has negativ logic)
		w1 = ~w1;
		Mask = 1+2+4+8;              // mask for getting a BCD-digit out of 16-bit word
		Digit[0] = (w1 & 1+2+4+8);
		Mask = Mask << 4;
		for (i=1; i<4; i++)
		{
			Digit[i] = (int) ((w1&Mask) >> (4*i)); 
			Mask = Mask << 4;        // next digit
		}
		Mask = 1+2+4+8;
		for (i=0; i<4; i++)
		{
			Digit[4+i] = ((w0&Mask) >> (4*i)); 
			Mask = Mask << 4;        // next digit
		}

		Volt =	Digit[1]+
				Digit[2]*10+
				Digit[3]*100+
				Digit[4]*1000+
				Digit[5]*10000+
				Digit[6]*100000+
				(Digit[7]&1)*1000000;
	
		/*
		if(Digit[0]==0) Expo = 1e0;
		if(Digit[0]==1) Expo = 1e1; 
		if(Digit[0]==2) Expo = 1e2;	
		if(Digit[0]==3) Expo = 1e3;	
		if(Digit[0]==4) Expo = 1e4;
		if(Digit[0]==5) Expo = 1e5;
		if(Digit[0]==6) Expo = 1e6;
		if(Digit[0]==7) Expo = 1e7;
		if(Digit[0]>7) Expo = 1e19;
		*/

		Expo = PowerOf10(Digit[0]);
		if((Digit[7]&8)) Expo = 1/Expo;
		Volt = Volt * Expo/1e6;

		if((Digit[7]&2)) Volt = -Volt;
		Voltage[channel]=Volt;
		NumOfSamples = channel+1;
	}
	else
	{
		w1			= TheData;
		LastChannel = channel;
	}
}

void KepcoEichungVoltageObj::DoDoubleClickAction(void)
{
	int i;
	double Start,Stop;
	ActionObject* pActionObject;
	CString TheObject="";
	CString ScalNo;
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);
	if(pDataView==NULL)pDataView = new DataView(this);
	else pDataView->Reset();
	pDataView->SetWindowTitle(ShortName+" in Track ["+TrackNo+"]  of "+pDocument->GetTitle());
	pDataView->SetDrawType((int)0,FALSE);

	pDataView->SetYTitle("Kepco-Voltage","V");
	POSITION pos=pDocument->ActionObjList.GetHeadPosition();
	int Channels = pTrack->Channels;
	do
	{
		pActionObject = (ActionObject*)pDocument->ActionObjList.GetNext(pos);
		TheObject     = pActionObject->GetName();
	}
	while ((pActionObject->pTrack->MyPosition != this->pTrack->MyPosition)||((pos!=NULL)&&(TheObject!=__HP_VoltageSweepObj)&&(TheObject!=__Line_VoltageSweepObj)));
	if((pos!=NULL)&&(TheObject==__HP_VoltageSweepObj))
	{
		HP_VoltageSweepObj* pAction = (HP_VoltageSweepObj*)pActionObject;
		Start = pAction->Sweep_Start;
		Stop  = pAction->Sweep_Stop;
		XName = "HP-Voltage";
		XUnit = "V";
		pDataView->SetXTitle(XName,XUnit);
	}

	if((pos!=NULL)&&(TheObject==__Line_VoltageSweepObj))
	{
		Line_VoltageSweepObj* pAction = (Line_VoltageSweepObj*)pActionObject;
		Start = pAction->Sweep_Start;
		Stop  = pAction->Sweep_Stop;
		XName = "Line-Voltage";
		XUnit = "V";
		pDataView->SetXTitle(XName,XUnit);
	}

	pDataView->SetTitle(pTrack->Isotope);
	for(i=0;i<Channels;i++)
	{
		pDataView->AddDataPoint((float)(i*((Stop-Start)/(Channels-1))+Start),
								(float)Voltage[i],
								(float)(1e-5));
	}
	pDataView->RedrawNow();
}

void KepcoEichungVoltageObj::CopyObject(ActionObject* pSource)
{
	USHORT i;
	KepcoEichungVoltageObj* pActionObject = (KepcoEichungVoltageObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;

	Factor				= pActionObject->Factor;
	DelayBeforeMeas		= pActionObject->DelayBeforeMeas;
	for(i=0;i<MAXPOINTS;i++)Voltage[i]=0;
	NumOfSamples		= 0;
}

CString KepcoEichungVoltageObj::GetInfo(void)
{
	CString answer;
	CString in1;
	in1.Format("%g",DelayBeforeMeas);
	answer = "PreDelay: "+in1+" ms";
	return answer;
}

void KepcoEichungVoltageObj::Load(CArchive& ar)
{
	TakesData = TRUE;
	int i=0;
	unsigned char TheChar;
	Name = ReadString(ar);
	ShortName = __KepcoEichungVoltageObj;
	ReadSeparator(ar);
	if (pDocument->Version > 7)
	{
		Factor = ReadFloat(ar);
		DelayBeforeMeas = ReadFloat(ar);
	}
	if (pTrack->RealScans>0)
	{
		do
		{
		Voltage[i]=ReadFloat(ar);
		++i;
		TheChar = ReadChar(ar);
		} while (TheChar ==',');
		NumOfSamples=i;
	}

	else
	{
		ReadStructBegin(ar);
		ReadStructEnd(ar);
	}
	ReadSeparator(ar);
	
}

void KepcoEichungVoltageObj::Save(CArchive& ar)
{
	USHORT i,j=0;
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);

	WriteFloat(ar,Factor);
	WriteSeparator(ar);
	WriteFloat(ar,DelayBeforeMeas);
	WriteSeparator(ar);
	WriteStructBegin(ar);
	if (pTrack->RealScans>0)
	{
		for(i=0;i<NumOfSamples;i++)
		{
			WriteFloat(ar,Voltage[i]);
			if(i!=(NumOfSamples-1))WriteSeparator(ar);
			j++;
			if(j==5)
			{
				j = 0;
				WriteLine(ar);
			}
		} 
	}	
	WriteStructEnd(ar);
	WriteLine(ar);
	WriteStructEnd(ar);
	WriteLine(ar);
}

void KepcoEichungVoltageObj::MeasurementBeginAction(BOOL RUNMODE)
{
	LastChannel = (USHORT)-1;
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();

	if(RUNMODE==ERGO) NumOfSamples = 0;
	
	SendFCamac(64+pApp->PresetSlot);
	SendNAFCamac(pApp->PresetSlot,0,26);
}

void KepcoEichungVoltageObj::MeasurementEndAction(void)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	SendFCamac(128+pApp->PresetSlot);
	SendNAFCamac(pApp->PresetSlot,0,24);
}

void KepcoEichungVoltageObj::TrackStepAction(USHORT step, USHORT track, USHORT scan)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	if(DelayBeforeMeas > 0)ListDelayCamac(pApp->PresetSlot,(USHORT)DelayBeforeMeas);

	ListDNAFCamac(SENDNAF,0,0,0,37);

	//ListDNAFCamac(SENDNAF,0,Slot,1,9);
	//ListDNAFCamac(SENDNAF,0,Slot,0,9);

	ListDNAFCamac(SENDNAF,0,Slot,1,28);
	ListDNAFCamac(SENDNAF,0,Slot,0,28);

	ListDNAFCamac(SENDNAF,0,Slot,1,2);
	ListDNAFCamac(READD,0,0,0,0);
	
	ListDNAFCamac(SENDNAF,0,Slot,0,2);
	ListDNAFCamac(READD,0,0,0,0);

	

	ListDNAFCamac(SENDNAF,0,0,0,32);
	
	pDocument->DataQueue[pDocument->DataQueueIndex].scan = scan;
	pDocument->DataQueue[pDocument->DataQueueIndex].step = step;
	pDocument->DataQueue[pDocument->DataQueueIndex].pAction = this;
	pDocument->DataQueueIndex++;
	
	pDocument->DataQueue[pDocument->DataQueueIndex].scan = scan;
	pDocument->DataQueue[pDocument->DataQueueIndex].step = step;
	pDocument->DataQueue[pDocument->DataQueueIndex].pAction = this;
	pDocument->DataQueueIndex++;
}

void KepcoEichungVoltageObj::TrackBeginAction(USHORT track)
{

	int i;
	BOOL ENDE = FALSE;
	CString XName,XUnit;
	CString YTitle,YUnit;
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	
	if(DispMonitorMode == 1)
	{
		if(pApp->pMonitorView==NULL)pApp->pMonitorView = new MonitorView();

		double Start,Stop;
		CString DoNew = "";
		ActionObject* pActionObject;
		CString TheObject="";
		CString TrackNo;
		
		TrackNo.Format("%u",pTrack->MyPosition);
		
		XName = "Channel";
		XUnit = "";
		Start = (double)1;
		Stop  = (double)pTrack->Channels;
		 
		pApp->pMonitorView->SetWindowTitle("Monitor for Track ["+TrackNo+"]  of "+pDocument->GetTitle());

		for(i=0;i<4;i++)pApp->pMonitorView->ActualIndex[i] = 0;

		POSITION pos=pDocument->ActionObjList.GetHeadPosition();
		int Channels = pTrack->Channels;
		
		pActionObject = (ActionObject*)pDocument->ActionObjList.GetHead();
		pos = pDocument->ActionObjList.GetHeadPosition();
		do
		{
			pActionObject = (ActionObject*)pDocument->ActionObjList.GetNext(pos);
			if (pActionObject!=NULL&&pActionObject->pTrack->MyPosition == track)
			{
				TheObject     = pActionObject->GetName();
				if((TheObject==__HP_VoltageSweepObj)||	
				(TheObject==__Line_VoltageSweepObj)||
				(TheObject==__HP8660B_FrequencySweepObj)||
				(TheObject==__HP3325B_FrequencySweepObj)||
				(TheObject==__HP3325B2_FrequencySweepObj)||
				(TheObject==__RS_FrequencySweepObj))ENDE = TRUE;
			}
		}
		while (pos!=NULL&&ENDE==FALSE);

		if(pos!=NULL)
		{
			if((TheObject==__HP_VoltageSweepObj)||(TheObject==__Line_VoltageSweepObj))
			{
				if(TheObject==__HP_VoltageSweepObj)
				{
					CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
					HP_VoltageSweepObj* pAction1 = (HP_VoltageSweepObj*)pActionObject;
					Start	= pAction1->Sweep_Start;
					Stop	= pAction1->Sweep_Stop;
					XName	= "HPVoltage";
					XUnit	= "V";
				}
				if(TheObject==__Line_VoltageSweepObj)
				{
					Line_VoltageSweepObj* pAction2 = (Line_VoltageSweepObj*)pActionObject;
					Start	= pAction2->Sweep_Start;
					Stop	= pAction2->Sweep_Stop;
					XName	= "LineVoltage";
					XUnit	= "V";
				}
				if (Start==Stop)
				{
					XName = "Channel";
					XUnit = "";
					Start = (double)1;
					Stop  = (double)pTrack->Channels;
				}
			}
		}
		YTitle = "KepcoVoltage [V]";
		YUnit="V";
		{
			if(pApp->pMonitorView->MonitorFree())
				pApp->pMonitorView->AddView(Channels,XName,XUnit,YTitle,YUnit,(float)Start,(float)Stop,(ActionObject*)this,pTrack->RealScans,ShowExpected);
		
			else 
			{
				DispMonitorMode = 0;
				AfxMessageBox("Too much monitors defined,\n\r disabled last",MB_OK,0);
			}
		}
	}
}

/****************   Methods for Class ComputerDelayObj  ***********************/

void ComputerDelayObj::DoDoubleClickAction(void)
{
	CComputerDelay dlg;
	dlg.DoModal();
}

void ComputerDelayObj::CopyObject(ActionObject* pSource)
{
	ComputerDelayObj* pActionObject = (ComputerDelayObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;

	TimerClicksPerStep			= pActionObject->TimerClicksPerStep;
	TimerClickLength			= pActionObject->TimerClickLength;
	CloseGateAfterClicks		= pActionObject->CloseGateAfterClicks;
	IgnoreProtons				= pActionObject->IgnoreProtons;
	ShowChannels				= pActionObject->ShowChannels;
	OpenBeamgateAfterChannel	= pActionObject->OpenBeamgateAfterChannel;
	ScalerOpenBitChange			= pActionObject->ScalerOpenBitChange;
	DoCallCrossList				= pActionObject->DoCallCrossList;
	CloseBeamgateAfterChannel	= pActionObject->CloseBeamgateAfterChannel;
	DelayTime					= pActionObject->DelayTime;
}

CString ComputerDelayObj::GetInfo(void)
{
	CString answer;
	answer.Format("DELAY: %g",DelayTime);
	return answer;
}

void ComputerDelayObj::Load(CArchive& ar)
{
	Name      = ReadString(ar);
	ShortName = 		__ComputerDelayObj;
	ReadSeparator(ar);
	DelayTime = ReadFloat(ar);

}
void ComputerDelayObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);
	WriteFloat(ar,DelayTime);
	WriteStructEnd(ar);
	WriteLine(ar);
}

/****************   Methods for Class TimerDelayObj  ***********************/

void TimerDelayObj::DoDoubleClickAction(void)
{
	CTimerDelay dlg;
	dlg.DoModal();
}

void TimerDelayObj::CopyObject(ActionObject* pSource)
{
	TimerDelayObj* pActionObject = (TimerDelayObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;

	TimerClicksPerStep			= pActionObject->TimerClicksPerStep;
	TimerClickLength			= pActionObject->TimerClickLength;
	CloseGateAfterClicks		= pActionObject->CloseGateAfterClicks;
	IgnoreProtons				= pActionObject->IgnoreProtons;
	ShowChannels				= pActionObject->ShowChannels;
	OpenBeamgateAfterChannel	= pActionObject->OpenBeamgateAfterChannel;
	ScalerOpenBitChange			= pActionObject->ScalerOpenBitChange;
	DoCallCrossList				= pActionObject->DoCallCrossList;
	CloseBeamgateAfterChannel	= pActionObject->CloseBeamgateAfterChannel;
}

CString TimerDelayObj::GetInfo(void)
{
	CString answer;
	answer.Format("DELAY: %g",TimerClickLength*TimerClicksPerStep);
	return answer;
}

void TimerDelayObj::Load(CArchive& ar)
{
	Name=ReadString(ar);
	ShortName = __TimerDelayObj;
	ReadSeparator(ar);
	if (pDocument->Version > 1)
	{
		TimerClicksPerStep   = (int)ReadFloat(ar);
		TimerClickLength     = ReadFloat(ar);
		CloseGateAfterClicks = (int)ReadFloat(ar);
		IgnoreProtons        = (int)ReadFloat(ar);
	}
	if (pDocument->Version > 7)
	{
		ShowChannels = (int)ReadFloat(ar);
		OpenBeamgateAfterChannel = (int)ReadFloat(ar);
		ScalerOpenBitChange = (int)ReadFloat(ar);
	}
	if (pDocument->Version > 11)
	{
		DoCallCrossList = (int)ReadFloat(ar);
	}
	if (pDocument->Version > 12)
	{
		CloseBeamgateAfterChannel = (int)ReadFloat(ar);
	}

}

void TimerDelayObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);
	WriteFloat(ar,TimerClicksPerStep);
	WriteSeparator(ar);
	WriteFloat(ar,TimerClickLength);
	WriteSeparator(ar);
	WriteFloat(ar,CloseGateAfterClicks);
	WriteSeparator(ar);
	WriteFloat(ar,IgnoreProtons);
	WriteSeparator(ar);
	WriteFloat(ar,ShowChannels);
	WriteSeparator(ar);
	WriteFloat(ar,OpenBeamgateAfterChannel);
	WriteSeparator(ar);
	WriteFloat(ar,ScalerOpenBitChange);
	WriteSeparator(ar);
	WriteFloat(ar,DoCallCrossList);
	WriteSeparator(ar);
	WriteFloat(ar,CloseBeamgateAfterChannel);
	WriteStructEnd(ar);
	WriteLine(ar);
}

/****************   Methods for Class StepTimerDelayObj  ***********************/

void StepTimerDelayObj::DoDoubleClickAction(void)
{
	CStepTimer dlg;
	dlg.DoModal();
}

void StepTimerDelayObj::CopyObject(ActionObject* pSource)
{
	StepTimerDelayObj* pActionObject = (StepTimerDelayObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;

	TimerClicksPerStep			= pActionObject->TimerClicksPerStep;
	TimerClickLength			= pActionObject->TimerClickLength;
	CloseGateAfterClicks		= pActionObject->CloseGateAfterClicks;
	IgnoreProtons				= pActionObject->IgnoreProtons;
	ShowChannels				= pActionObject->ShowChannels;
	OpenBeamgateAfterChannel	= pActionObject->OpenBeamgateAfterChannel;
	ScalerOpenBitChange			= pActionObject->ScalerOpenBitChange;
	DoCallCrossList				= pActionObject->DoCallCrossList;
	CloseBeamgateAfterChannel	= pActionObject->CloseBeamgateAfterChannel;
}

CString StepTimerDelayObj::GetInfo(void)
{
	CString answer;
	answer.Format("DELAY: %g",TimerClickLength*TimerClicksPerStep);
	return answer;
}

void StepTimerDelayObj::Load(CArchive& ar)
{
	Name            = ReadString(ar);
	ShortName = 			__StepTimerDelayObj;
	if (pDocument->Version > 1)              // 1 war erste gltige MCP-Datenfile Version
	{
	  ReadSeparator(ar);
	  TimerClicksPerStep = (int)ReadFloat(ar);
	  ReadSeparator(ar);
	  TimerClickLength = ReadFloat(ar);
	  ReadSeparator(ar);
	  CloseGateAfterClicks = (int)ReadFloat(ar);
	  ReadSeparator(ar);
	  IgnoreProtons = (BOOL)ReadFloat(ar);
	};
      if (pDocument->Version > 7)
      {
			ReadSeparator(ar);
			ShowChannels = (BOOL)ReadFloat(ar);
			ReadSeparator(ar);
			OpenBeamgateAfterChannel = (int)ReadFloat(ar);
			ReadSeparator(ar);
			ReadControlBegin(ar);
			ScalerOpenBitChange = (int)ReadFloat(ar);
      }
      if (pDocument->Version > 11)
        {
		  ReadStructBegin(ar);
          DoCallCrossList = (int)ReadFloat(ar);
        }
      if (pDocument->Version > 12)
		{
			ReadSeparator(ar);
			CloseBeamgateAfterChannel = (int)ReadFloat(ar);
			ReadSeparator(ar);
			ReadFloat(ar);	
		}
	  ReadStructEnd(ar);
	  
}

void StepTimerDelayObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);
	WriteFloat(ar,TimerClicksPerStep);
	WriteSeparator(ar);
	WriteFloat(ar,TimerClickLength);
	WriteSeparator(ar);
	WriteFloat(ar,CloseGateAfterClicks);
	WriteSeparator(ar);
	WriteFloat(ar,IgnoreProtons);
	WriteSeparator(ar);
	WriteFloat(ar,ShowChannels);
	WriteSeparator(ar);
	WriteFloat(ar,OpenBeamgateAfterChannel);
	WriteSeparator(ar);
	WriteControlBegin(ar);
	WriteFloat(ar,ScalerOpenBitChange);
	WriteStructBegin(ar);
    WriteFloat(ar,DoCallCrossList);
	WriteSeparator(ar);
	WriteFloat(ar,CloseBeamgateAfterChannel);
	WriteSeparator(ar);
	WriteFloat(ar,0);	

	WriteStructEnd(ar);
	WriteLine(ar);	  
}

/****************   Methods for Class MassAndFlukeSwitchObj  ***********************/

void MassAndFlukeSwitchObj::DoDoubleClickAction(void)
{
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);
	CPropShtMF propSheet(ShortName+" in Track ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);
	
	propSheet.m_page1.m_fluke1	= FALSE;
	propSheet.m_page1.m_fluke2	= FALSE;
	propSheet.m_page1.m_fluke3	= FALSE;
	propSheet.m_page1.m_nofluke = FALSE;
	propSheet.m_page1.m_nomass	= TRUE;

	if(ToFlukeNo ==1)propSheet.m_page1.m_fluke1 = TRUE;
	if(ToFlukeNo ==2)propSheet.m_page1.m_fluke2 = TRUE;
	if(ToFlukeNo ==3)propSheet.m_page1.m_fluke3 = TRUE;
	if(ToFlukeNo ==0)propSheet.m_page1.m_nofluke = TRUE;
	if(!DontSetMass)propSheet.m_page1.m_nomass = FALSE;

	propSheet.m_page1.m_setamu = (float)ToMassNo;

	propSheet.m_page3.m_gps = TRUE;
	propSheet.m_page3.m_hrs = FALSE;

	if(UseHrs)
	{
		propSheet.m_page3.m_gps = FALSE;
		propSheet.m_page3.m_hrs = TRUE;
	}
	propSheet.m_page3.m_slot= Slot;
	
	if(IDOK == propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			if(propSheet.m_page1.m_nofluke) ToFlukeNo = 0;
			if(propSheet.m_page1.m_fluke1) ToFlukeNo = 1;
			if(propSheet.m_page1.m_fluke2) ToFlukeNo = 2;
			if(propSheet.m_page1.m_fluke3) ToFlukeNo = 3;
			
			ToMassNo = propSheet.m_page1.m_setamu;
			DontSetMass = propSheet.m_page1.m_nomass;

			Slot		= propSheet.m_page3.m_slot;

			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
		}
		else Beep(1000,100);
	}
}

void MassAndFlukeSwitchObj::CopyObject(ActionObject* pSource)
{
	MassAndFlukeSwitchObj* pActionObject = (MassAndFlukeSwitchObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;

	ToFlukeNo		= pActionObject->ToFlukeNo;
	ToMassNo		= pActionObject->ToMassNo;
	IsRadioactive	= pActionObject->IsRadioactive;	
}

CString MassAndFlukeSwitchObj::GetInfo(void)
{
	CString answer;
	CString in1;
	CString in2;
	in1.Format("%u",ToFlukeNo);
	in2.Format("%g",ToMassNo);
	answer = "FLUKE "+in1+" MASS "+in2;
	return answer;
}

void MassAndFlukeSwitchObj::Load(CArchive& ar)
{
	Name = ReadString(ar);
	ShortName = __MassAndFlukeSwitchObj;
	ReadSeparator(ar);
	ToMassNo = ReadFloat(ar);
	ReadSeparator(ar);
	ToFlukeNo = (int)ReadFloat(ar);
	if (pDocument->Version > 1)
	{
		ReadSeparator(ar);
		IsRadioactive = (int)ReadFloat(ar);
	}
	ReadStructEnd(ar);
}

void MassAndFlukeSwitchObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);

	WriteFloat(ar,ToMassNo);
	WriteSeparator(ar);
	WriteFloat(ar,ToFlukeNo);
	WriteSeparator(ar);
	WriteFloat(ar,IsRadioactive);
	WriteStructEnd(ar);
	WriteLine(ar);
}

/****************   Methods for Class MassSwitchObj  ***********************/

void MassSwitchObj::DoDoubleClickAction(void)
{
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);
	CPropShtMS propSheet(ShortName+" in Track ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);
	
	propSheet.m_page1.setamu = ToMassNo;

	propSheet.m_page3.m_gps = TRUE;
	propSheet.m_page3.m_hrs = FALSE;

	if(UseHrs)
	{
		propSheet.m_page3.m_gps = FALSE;
		propSheet.m_page3.m_hrs = TRUE;
	}
	
	if(IDOK == propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			ToMassNo	= propSheet.m_page1.setamu;

			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
		}
		else Beep(1000,100);
	}


}

void MassSwitchObj::CopyObject(ActionObject* pSource)
{
	MassSwitchObj* pActionObject = (MassSwitchObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;

	ToMassNo		= pActionObject->ToMassNo;
	IsRadioactive	= pActionObject->IsRadioactive;	
}

CString MassSwitchObj::GetInfo(void)
{
	CString answer;
	CString in1;
	CString in2;
	if(IsRadioactive)in1 = " RADIOACTIVE";
	else in1 = "  NOT RADIOACTIVE";
	in2.Format("%g",ToMassNo);
	answer = "MASS "+in2+in1;
	return answer;
}

void MassSwitchObj::Load(CArchive& ar)
{
	Name = ReadString(ar);
	ShortName = __MassSwitchObj;
	ReadSeparator(ar);
	ToMassNo = ReadFloat(ar);
	if (pDocument->Version > 1)
	{
		ReadSeparator(ar);
		IsRadioactive = (int)ReadFloat(ar);
	}
	ReadStructEnd(ar);
}

void MassSwitchObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);

	WriteFloat(ar,ToMassNo);
	WriteSeparator(ar);
	WriteFloat(ar,IsRadioactive);
	WriteStructEnd(ar);
	WriteLine(ar);
}

void MassSwitchObj::TrackBeginAction(USHORT track)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();

	pApp->SetMass(ToMassNo);

	ListDelayCamac(pApp->PresetSlot,pApp->MassDelay);
}
/****************   Methods for Class FlukeSwitchObj  ***********************/

void FlukeSwitchObj::DoDoubleClickAction(void)
{
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);
	CPropShtfs propSheet(ShortName+" in Track ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);
	
	propSheet.m_page1.m_fluke1	= FALSE;
	propSheet.m_page1.m_fluke2	= FALSE;
	propSheet.m_page1.m_fluke3	= FALSE;
	propSheet.m_page1.m_nofluke = FALSE;

	if(ToFlukeNo ==1)propSheet.m_page1.m_fluke1		= TRUE;
	if(ToFlukeNo ==2)propSheet.m_page1.m_fluke2		= TRUE;
	if(ToFlukeNo ==3)propSheet.m_page1.m_fluke3		= TRUE;
	if(ToFlukeNo ==0)propSheet.m_page1.m_nofluke	= TRUE;

	propSheet.m_page3.m_slot= Slot;
	
	if(IDOK == propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			if(propSheet.m_page1.m_nofluke) ToFlukeNo	= 0;
			if(propSheet.m_page1.m_fluke1) ToFlukeNo	= 1;
			if(propSheet.m_page1.m_fluke2) ToFlukeNo	= 2;
			if(propSheet.m_page1.m_fluke3) ToFlukeNo	= 3;

			Slot		= propSheet.m_page3.m_slot;

			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
		}
		else Beep(1000,100);
	}

}

void FlukeSwitchObj::CopyObject(ActionObject* pSource)
{
	FlukeSwitchObj* pActionObject = (FlukeSwitchObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;

	ToFlukeNo		= pActionObject->ToFlukeNo;
}

CString FlukeSwitchObj::GetInfo(void)
{
	CString answer;
	CString in1;
	in1.Format("%u",ToFlukeNo);
	answer = "FLUKE "+in1;
	return answer;
}

void FlukeSwitchObj::Load(CArchive& ar)
{
	Name = ReadString(ar);
	ShortName = __FlukeSwitchObj;
	ReadSeparator(ar);
	ToFlukeNo = (int)ReadFloat(ar);
	ReadStructEnd(ar);
}

void FlukeSwitchObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);

	WriteFloat(ar,ToFlukeNo);
	WriteStructEnd(ar);
	WriteLine(ar);
}

void FlukeSwitchObj::MeasurementBeginAction(BOOL RUNMODE)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	
	SendFCamac(64+pApp->PresetSlot);
	SendNAFCamac(pApp->PresetSlot,0,26);
}

void FlukeSwitchObj::MeasurementEndAction(void)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	OffBit(1,0);
	OffBit(1,1);
	OffBit(1,2);
	SendFCamac(128+pApp->PresetSlot);
	SendNAFCamac(pApp->PresetSlot,0,24);
}

void FlukeSwitchObj::TrackBeginAction(USHORT track)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	
	switch(ToFlukeNo)
	{
	case 0:
		ListOffBit(1,0);
		ListOffBit(1,1);
		ListOffBit(1,2);
		ListDelayCamac(pApp->PresetSlot,500);
		break;
	case 1:
		ListOffBit(1,1);
		ListOffBit(1,2);
		ListDelayCamac(pApp->PresetSlot,500);
		ListOnBit(1,0);
		break;
	case 2:
		ListOffBit(1,0);
		ListOffBit(1,2);
		ListDelayCamac(pApp->PresetSlot,500);
		ListOnBit(1,1);
		break;
	case 3:
		ListOffBit(1,0);
		ListOffBit(1,1);
		ListDelayCamac(pApp->PresetSlot,500);
		ListOnBit(1,2);
		break;
	}
	ListDelayCamac(pApp->PresetSlot,pApp->FlukeDelay);
}

/****************   Methods for Class TriggerSweepObj  ***********************/

void TriggerSweepObj::DoDoubleClickAction(void)
{
}

void TriggerSweepObj::CopyObject(ActionObject* pSource)
{
	USHORT i;
	TriggerSweepObj* pActionObject = (TriggerSweepObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;

	for(i=0;i<MAXPOINTS;i++)y[i]=0;
	ProtonCounts		= pActionObject->ProtonCounts;
	TriggerDelay		= pActionObject->TriggerDelay;
	CheckProtons		= pActionObject->CheckProtons;
}

CString TriggerSweepObj::GetInfo(void)
{
	return " ";
}

void TriggerSweepObj::Load(CArchive& ar)
{
	int i;
	Name = ReadString(ar);
	ShortName = __TriggerSweepObj;
	if (pDocument->Version > 1)
	{
		ReadSeparator(ar);
		if (pDocument->Version < 4)
		{
			ReadStructBegin(ar);
			ReadStructEnd(ar);
		}
		else ProtonCounts = ReadFloat(ar);
	}
	if (pDocument->Version > 2)
	{
		TriggerDelay = ReadFloat(ar);
	}
	if (pDocument->Version > 7)
	{
		CheckProtons = ReadFloat(ar);
	}
	if (pDocument->Version > 11)
	{
		for (i=0;i<pTrack->Channels;i++)
		{
			y[i]   = ReadFloat(ar);
		}
	}

}

void TriggerSweepObj::Save(CArchive& ar)
{
	USHORT i,j=0;
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);
	
	WriteFloat(ar,ProtonCounts);
	WriteSeparator(ar);
	WriteFloat(ar,TriggerDelay);
	WriteSeparator(ar);
	WriteFloat(ar,CheckProtons);
	WriteSeparator(ar);
	for (i=0;i<pTrack->Channels;i++)
	{
		WriteFloat(ar,y[i]);
		if(i!=(pTrack->Channels-1))WriteSeparator(ar);
		j++;
		if(j==5)
		{
			j = 0;
			WriteLine(ar);
		}
	}
	WriteStructEnd(ar);
	WriteLine(ar);
}

/****************   Methods for Class StepTriggerObj  ***********************/

void StepTriggerObj::DoDoubleClickAction(void)
{
}

void StepTriggerObj::CopyObject(ActionObject* pSource)
{
	USHORT i;
	StepTriggerObj* pActionObject = (StepTriggerObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;

	for(i=0;i<MAXPOINTS;i++)y[i]=0;
	ProtonCounts		= pActionObject->ProtonCounts;
	TriggerDelay		= pActionObject->TriggerDelay;
	CheckProtons		= pActionObject->CheckProtons;
	ReadProtons			= pActionObject->ReadProtons;
	RedoStepFlag		= pActionObject->RedoStepFlag;
}

CString StepTriggerObj::GetInfo(void)
{
	return " ";
}

void StepTriggerObj::Load(CArchive& ar)
{
	int i;
	Name = ReadString(ar);
	ShortName = __StepTriggerObj;
	if (pDocument->Version > 1)
	{
		ReadSeparator(ar);
		if (pDocument->Version < 4)
		{
			ReadStructBegin(ar);
			ReadStructEnd(ar);
			for (i=0;i<pTrack->Channels;i++)
			{
				y[i]   = ReadFloat(ar);
			}
			ReadStructEnd(ar);
		}

		else 
		{
			ReadControlBegin(ar);
			ReadFloat(ar);
			ReadControlEnd(ar);
			ReadStructBegin(ar);
			for (i=0;i<pTrack->Channels;i++)
			{
				y[i]   = ReadFloat(ar);
			}
			ReadStructEnd(ar);
		}
	}

	if (pDocument->Version > 2)
	{
		ReadSeparator(ar);
		TriggerDelay = ReadFloat(ar);
	}
	if (pDocument->Version > 7)
	{
		ReadSeparator(ar);
		CheckProtons = ReadFloat(ar);
	}
	if (pDocument->Version > 11)
	{
		ReadSeparator(ar);
		ReadProtons = ReadFloat(ar);
	}
	if (pDocument->Version > 2)
	{
		ReadStructEnd(ar);
	}


	if (pDocument->Version > 7)
	{
		ReadSeparator(ar);
		ReadStructBegin(ar);
		RedoStepFlag = (int)ReadFloat(ar);
		ReadStructEnd(ar);
	}

}

void StepTriggerObj::Save(CArchive& ar)
{
	USHORT i,j=0;
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);

	WriteControlBegin(ar);
	WriteFloat(ar,0);
	WriteControlEnd(ar);
	WriteStructBegin(ar);
	for (i=0;i<pTrack->Channels;i++)
	{
		WriteFloat(ar,y[i]);
		if(i!=(pTrack->Channels-1))WriteSeparator(ar);
		j++;
		if(j==5)
		{
			j = 0;
			WriteLine(ar);
		}
	}
	WriteStructEnd(ar);

	WriteSeparator(ar);
	WriteFloat(ar,TriggerDelay);
	WriteSeparator(ar);
	WriteFloat(ar,CheckProtons);
	WriteSeparator(ar);
	WriteFloat(ar,ReadProtons);
	WriteStructEnd(ar);
	WriteSeparator(ar);
	WriteStructBegin(ar);
	WriteFloat(ar,RedoStepFlag);
	WriteStructEnd(ar);
	WriteLine(ar);
}

/****************   Methods for Class TriggerPartSweepObj  ***********************/

void TriggerPartSweepObj::DoDoubleClickAction(void)
{
	CTriggerPartSweep dlg;
	dlg.DoModal();
}

void TriggerPartSweepObj::CopyObject(ActionObject* pSource)
{
	TriggerPartSweepObj* pActionObject = (TriggerPartSweepObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;

	TriggerAfterNoOfSteps = pActionObject->TriggerAfterNoOfSteps;
}

CString TriggerPartSweepObj::GetInfo(void)
{
	return " ";
}

void TriggerPartSweepObj::Load(CArchive& ar)
{
	Name=ReadString(ar);
	ShortName = __TriggerPartSweepObj;
	ReadSeparator(ar);
	TriggerAfterNoOfSteps =	(int)ReadFloat(ar);
}

void TriggerPartSweepObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);

	WriteFloat(ar,TriggerAfterNoOfSteps);
	WriteStructEnd(ar);
	WriteLine(ar);
}

/****************   Methods for Class MultiFunctionObj  ***********************/

void MultiFunctionObj::DoDoubleClickAction(void)
{
}

void MultiFunctionObj::CopyObject(ActionObject* pSource)
{
	MultiFunctionObj* pActionObject = (MultiFunctionObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;
}

CString MultiFunctionObj::GetInfo(void)
{
	return " ";
}

void MultiFunctionObj::Load(CArchive& ar)
{
}

void MultiFunctionObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);
	WriteLine(ar);
}

/****************   Methods for Class FDAC_VoltageSweepObj  ***********************/

void FDAC_VoltageSweepObj::UpdateData(void)
{
	Sweep_Stop = CalcStop(Sweep_Start, Sweep_Stop, pTrack->Channels, FDACResolution);
}

void FDAC_VoltageSweepObj::SetMinMax(double minstep,double maxstep)
{
	CalcStartStop(&Sweep_Start, &Sweep_Stop, pTrack->Channels, Resolution, minstep, maxstep);
}

void FDAC_VoltageSweepObj::SetCenter(USHORT step)
{
	CalcCenter(&Sweep_Start, &Sweep_Stop, pTrack->Channels, Resolution, step);
}

void FDAC_VoltageSweepObj::DoDoubleClickAction(void)
{
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);

	CPropShtFV propSheet(ShortName+" in Track ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);
	
	propSheet.m_page1.channels		= pTrack->Channels;
	propSheet.m_page1.resolution	= Resolution;

	propSheet.m_page1.startvolt		= Sweep_Start;
	propSheet.m_page1.stopvolt		= Sweep_Stop;

	propSheet.m_page1.m_slowreset	= FALSE;
	propSheet.m_page1.m_slowresetch = 0;

	propSheet.m_page2.m_noscanvolt	= NoSweep_Value;

	propSheet.m_page3.m_slot		= Slot;
	propSheet.m_page3.m_subaddress	= SubAddress;

	if(IDOK == propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			Sweep_Start = propSheet.m_page1.startvolt;
			Sweep_Stop	= propSheet.m_page1.stopvolt;

			NoSweep_Value = propSheet.m_page2.m_noscanvolt;

			Slot		= propSheet.m_page3.m_slot;
			SubAddress	= propSheet.m_page3.m_subaddress;

			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
		}
		else Beep(1000,100);
	}
}

void FDAC_VoltageSweepObj::CopyObject(ActionObject* pSource)
{
	FDAC_VoltageSweepObj* pActionObject = (FDAC_VoltageSweepObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;

	Sweep_Start			= pActionObject->Sweep_Start;
	Sweep_Step			= pActionObject->Sweep_Step;
	Sweep_Stop			= pActionObject->Sweep_Stop;
	Sweep_Resolution	= pActionObject->Sweep_Resolution;
	Sweep_SubAddress	= pActionObject->Sweep_SubAddress;
	NoSweep_Value		= pActionObject->NoSweep_Value;
	SwitchMode			= pActionObject->SwitchMode;
	ChangePos1			= pActionObject->ChangePos1;
	ChangePos2			= pActionObject->ChangePos2;
	ChangePos3			= pActionObject->ChangePos3;
	ChangePos4			= pActionObject->ChangePos4;
}

CString FDAC_VoltageSweepObj::GetInfo(void)
{
	CString answer;
	answer.Format("%gV TO %gV step %gV",Sweep_Start,Sweep_Stop,
		CalcStep(Sweep_Start,Sweep_Stop,pTrack->Channels,Resolution));
	return answer;
}

void FDAC_VoltageSweepObj::Load(CArchive& ar)
{
	Resolution			= FDACResolution;
	Name=ReadString(ar);
	ShortName = __FDAC_VoltageSweepObj;
	ReadSeparator(ar);
	Sweep_Start      = ReadFloat(ar);
	NoSweep_Value		= 0;
	ReadSeparator(ar);
	Sweep_Stop       = ReadFloat(ar);
	ReadSeparator(ar);
	Sweep_Resolution = ReadFloat(ar);
	ReadSeparator(ar);
	Sweep_SubAddress = ReadFloat(ar);
	
	if (pDocument->Version > 14)
	{
		ReadSeparator(ar);
		SwitchMode = (int)ReadFloat(ar);
	}
	if (pDocument->Version > 18)
	{
		ReadSeparator(ar);
		ChangePos1 = ReadFloat(ar);
		ReadSeparator(ar);
		ChangePos2 = ReadFloat(ar);
		ReadSeparator(ar);
		ChangePos3 = ReadFloat(ar);
		ReadSeparator(ar);
		ChangePos4 = ReadFloat(ar);
	}
	ReadStructEnd(ar);
}

void FDAC_VoltageSweepObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);

	WriteFloat(ar,Sweep_Start);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Stop);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Resolution);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_SubAddress);
	WriteSeparator(ar);
	WriteFloat(ar,SwitchMode);
	WriteSeparator(ar);
	WriteFloat(ar,ChangePos1);
	WriteSeparator(ar);
	WriteFloat(ar,ChangePos2);
	WriteSeparator(ar);
	WriteFloat(ar,ChangePos3);
	WriteSeparator(ar);
	WriteFloat(ar,ChangePos4);
	WriteStructEnd(ar);
	WriteLine(ar);
}

void FDAC_VoltageSweepObj::TrackStepAction(USHORT step, USHORT track, USHORT scan)
{
	int Data;
	double Voltage  = CalcValue(Sweep_Start,Sweep_Stop,pTrack->Channels,step,Resolution);
	Data = (int)(Voltage*100);
	if(Data < 0)Data = (Data | 1024);
	ListDNAFCamac(SENDDNAF,Data,Slot,SubAddress,16);
}
	
void FDAC_VoltageSweepObj::TrackBeginAction(USHORT track)
{
	int Data;
	double Voltage  = Sweep_Start;
	Data = (int)(Voltage*100);
	if(Data < 0)Data = (Data | 1024);
	ListDNAFCamac(SENDDNAF,Data,Slot,SubAddress,16);
}

void FDAC_VoltageSweepObj::MeasurementBeginAction(BOOL RUNMODE)
{
	SendDNAFCamac((ULONG)0,Slot,SubAddress,16);
}

void FDAC_VoltageSweepObj::MeasurementEndAction(void)
{
	SendDNAFCamac((ULONG)0,Slot,SubAddress,16);
}

/****************   Methods for Class CamacLPObj  ***********************/

void CamacLPObj::DoDoubleClickAction(void)
{
}

void CamacLPObj::CopyObject(ActionObject* pSource)
{
	CamacLPObj* pActionObject = (CamacLPObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;
}

CString CamacLPObj::GetInfo(void)
{
	return " ";
}

void CamacLPObj::Load(CArchive& ar)
{
}

void CamacLPObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);
	WriteLine(ar);
}

/****************   Methods for Class CCDCameraObj  ***********************/

void CCDCameraObj::DoDoubleClickAction(void)
{
}

void CCDCameraObj::CopyObject(ActionObject* pSource)
{
	CCDCameraObj* pActionObject = (CCDCameraObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;
}

CString CCDCameraObj::GetInfo(void)
{
	return " ";
}

void CCDCameraObj::Load(CArchive& ar)
{
}

void CCDCameraObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);
	WriteLine(ar);
}

/****************   Methods for Class DummyObj  ***********************/

void DummyObj::DoDoubleClickAction(void)
{
}

void DummyObj::CopyObject(ActionObject* pSource)
{
	DummyObj* pActionObject = (DummyObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;
}

CString DummyObj::GetInfo(void)
{
	return " ";
}

void DummyObj::Load(CArchive& ar)
{
}

void DummyObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);
	WriteLine(ar);
}

/****************   Methods for Class FastTimerDelayObj  ***********************/

void FastTimerDelayObj::CopyObject(ActionObject* pSource)
{
	FastTimerDelayObj* pActionObject = (FastTimerDelayObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;

	TimerClicksPerStep			= pActionObject->TimerClicksPerStep;
	TimerClickLength			= pActionObject->TimerClickLength;
	CloseGateAfterClicks		= pActionObject->CloseGateAfterClicks;
	IgnoreProtons				= pActionObject->IgnoreProtons;
	ShowChannels				= pActionObject->ShowChannels;
	OpenBeamgateAfterChannel	= pActionObject->OpenBeamgateAfterChannel;
	ScalerOpenBitChange			= pActionObject->ScalerOpenBitChange;
	DoCallCrossList				= pActionObject->DoCallCrossList;
	CloseBeamgateAfterChannel	= pActionObject->CloseBeamgateAfterChannel;	
}

void FastTimerDelayObj::DoDoubleClickAction(void)
{
	CFastTimer dlg;
	dlg.DoModal();
}

CString FastTimerDelayObj::GetInfo(void)
{
	CString answer;
	answer.Format("DELAY: %g",TimerClickLength*TimerClicksPerStep);
	return answer;
}

void FastTimerDelayObj::Load(CArchive& ar)
{
	Name            = ReadString(ar);
	ShortName = 			__FastTimerDelayObj;
	if (pDocument->Version > 1)              // 1 war erste gltige MCP-Datenfile Version
	{
	  ReadSeparator(ar);
	  TimerClicksPerStep = (int)ReadFloat(ar);
	  ReadSeparator(ar);
	  TimerClickLength = ReadFloat(ar);
	  ReadSeparator(ar);
	  CloseGateAfterClicks = (int)ReadFloat(ar);
	  ReadSeparator(ar);
	  IgnoreProtons = (BOOL)ReadFloat(ar);
	};
      if (pDocument->Version > 7)
      {
			ReadSeparator(ar);
			ShowChannels = (BOOL)ReadFloat(ar);
			ReadSeparator(ar);
			OpenBeamgateAfterChannel = (int)ReadFloat(ar);
			ReadSeparator(ar);
			ReadControlBegin(ar);
			ScalerOpenBitChange = (int)ReadFloat(ar);
      }
      if (pDocument->Version > 11)
        {
		  ReadStructBegin(ar);
          DoCallCrossList = (int)ReadFloat(ar);
        }
      if (pDocument->Version > 12)
		{
			ReadSeparator(ar);
			CloseBeamgateAfterChannel = (int)ReadFloat(ar);
			ReadSeparator(ar);
			ReadFloat(ar);	
		}
	  ReadStructEnd(ar);	  
}

void FastTimerDelayObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);
	WriteFloat(ar,TimerClicksPerStep);
	WriteSeparator(ar);
	WriteFloat(ar,TimerClickLength);
	WriteSeparator(ar);
	WriteFloat(ar,CloseGateAfterClicks);
	WriteSeparator(ar);
	WriteFloat(ar,IgnoreProtons);
	WriteSeparator(ar);
	WriteFloat(ar,ShowChannels);
	WriteSeparator(ar);
	WriteFloat(ar,OpenBeamgateAfterChannel);
	WriteSeparator(ar);
	WriteControlBegin(ar);
	WriteFloat(ar,ScalerOpenBitChange);
	WriteControlEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
    WriteFloat(ar,DoCallCrossList);
	WriteStructEnd(ar);
	WriteSeparator(ar);
	WriteFloat(ar,CloseBeamgateAfterChannel);
	WriteSeparator(ar);
	WriteFloat(ar,0);	

	WriteStructEnd(ar);
	WriteLine(ar);
}

/****************   Methods for Class Keithley_VoltageMeasObj  ***********************/

void Keithley_VoltageMeasObj::DoDoubleClickAction(void)
{
}

double Keithley_VoltageMeasObj::GetY(USHORT channel)
{
	return (double) Voltage[channel];
}

double Keithley_VoltageMeasObj::GetYErr(USHORT channel)
{
	return (double)0;
}

void Keithley_VoltageMeasObj::TakeData(BOOL RUNMODE, USHORT scan, USHORT channel, ULONG TheData)
{
	if(scan==1&&RUNMODE==ERGO)Voltage[channel] = (float)TheData;
	else Voltage[channel] = TheData+Voltage[channel];		
}

void Keithley_VoltageMeasObj::CopyObject(ActionObject* pSource)
{
	Keithley_VoltageMeasObj* pActionObject = (Keithley_VoltageMeasObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;
}

CString Keithley_VoltageMeasObj::GetInfo(void)
{
	return " ";
}

void Keithley_VoltageMeasObj::Load(CArchive& ar)
{
	TakesData = TRUE;
	int i=0;
	double x;
	unsigned char TheChar;
	Name            = ReadString(ar);
	ShortName = 			  __Keithley_VoltageMeasObj;
	ReadSeparator(ar);
	Reso		    = ReadFloat(ar);
	ReadSeparator(ar);
	MeasAtScanNo    = ReadFloat(ar);
	ReadSeparator(ar);
	NoOfMeasPerStep = (int)ReadFloat(ar);
	ReadSeparator(ar);
	TotalMeasTime   = ReadFloat(ar);
	ReadSeparator(ar);

	if (pDocument->Version > 7)
	{
		DelayAfterTrigger = ReadFloat(ar);
		ReadSeparator(ar);
		RangeNo = ReadFloat(ar);
		ReadSeparator(ar);
	}
	if (pTrack->RealScans>0)
	{
		ReadStructBegin(ar);
		do
		{
		x=ReadFloat(ar);
		if (i<MAXPOINTS)Voltage[i]=x;
		++i;
		TheChar = ReadChar(ar);
		} while (TheChar ==',');
	}
	ReadStructEnd(ar);
	ReadStructEnd(ar);

}

void Keithley_VoltageMeasObj::Save(CArchive& ar)
{
	USHORT i,j=0;
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);

	WriteFloat(ar,Reso);
	WriteSeparator(ar);
	WriteFloat(ar,MeasAtScanNo);
	WriteSeparator(ar);
	WriteFloat(ar,NoOfMeasPerStep);
	WriteSeparator(ar);
	WriteFloat(ar,TotalMeasTime);
	WriteSeparator(ar);
	WriteFloat(ar,DelayAfterTrigger);
	WriteSeparator(ar);
	WriteFloat(ar,RangeNo);
	WriteSeparator(ar);

	WriteStructBegin(ar);
	if (pTrack->RealScans>0)
	{
		WriteStructBegin(ar);
		for(i=0;i<1;i++)
		{
			WriteFloat(ar,Voltage[i]);
			if(i!=(NumOfSamples-1))WriteSeparator(ar);
			j++;
			if(j==5)
			{
				j = 0;
				WriteLine(ar);
			}

		}
	}
	WriteStructEnd(ar);
	WriteStructEnd(ar);
	WriteLine(ar);
}

/****************   Methods for Class FlukeOsziObj  ***********************/

void FlukeOsziObj::DoDoubleClickAction(void)
{
}

void FlukeOsziObj::CopyObject(ActionObject* pSource)
{
	FlukeOsziObj* pActionObject = (FlukeOsziObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;
}

CString FlukeOsziObj::GetInfo(void)
{
	return " ";
}

void FlukeOsziObj::Load(CArchive& ar)
{
}

void FlukeOsziObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);
	WriteLine(ar);
}

/****************   Methods for Class AlexMotorObj  ***********************/

void AlexMotorObj::DoDoubleClickAction(void)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);
	CPropShtam propSheet(ShortName+" in Track ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);

	if(FORWARD)
	{
		propSheet.m_page1.m_forward = TRUE;
		propSheet.m_page1.m_backwards = FALSE;
	}
	else
	{
		propSheet.m_page1.m_forward = FALSE;
		propSheet.m_page1.m_backwards = TRUE;
	}
	propSheet.m_page1.m_movecm		= MoveCm;
	propSheet.m_page1.m_usebrake	= UseBrake;
	propSheet.m_page3.m_actpos		= pApp->GlobalActPosition;
	propSheet.m_page3.m_donepos		= pApp->GlobalActPosition;
	propSheet.m_page3.m_leftpos		= LeftPosition;
	propSheet.m_page3.m_moveto		= MoveTo;
	propSheet.m_page3.m_slot		= Slot;

	if(propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			MoveCm		= propSheet.m_page1.m_movecm;
			UseBrake	= propSheet.m_page1.m_usebrake;
			Slot		= propSheet.m_page3.m_slot;
			FORWARD		= propSheet.m_page1.m_forward;
			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
		}
		else Beep(1000,100);
	}
}

void AlexMotorObj::CopyObject(ActionObject* pSource)
{
	AlexMotorObj* pActionObject = (AlexMotorObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;
	MoveTo			= pActionObject->MoveTo;
	FORWARD			= pActionObject->FORWARD;
}

CString AlexMotorObj::GetInfo(void)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	CString answer;
	CString in1;
	CString in2;
	in1.Format("%g",pApp->GlobalActPosition);
	answer = "Position: "+in1+" m";
	return answer;
}

void AlexMotorObj::Load(CArchive& ar)
{
	Name=ReadString(ar);
	ShortName = __AlexMotorObj;
	ReadSeparator(ar);
	MoveCm = ReadFloat(ar);
	ReadSeparator(ar);
	Gpib = (int)ReadFloat(ar);
	ReadStructEnd(ar);
}

void AlexMotorObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);
	WriteFloat(ar,MoveCm);
	WriteSeparator(ar);
	WriteFloat(ar,Gpib);
	WriteStructEnd(ar);
	WriteLine(ar);
}

void AlexMotorObj::MeasurementBeginAction(BOOL RUNMODE)
{
	SendDNAFCamac(50*1+250*256+1*65536,Slot,0,17);
	SendNAFCamac(Slot,0,10);
	SendFCamac(Slot+64);
}

void AlexMotorObj::MeasurementEndAction(void)
{
	ULONG state;
	SendNAFCamac(Slot,12,1);
	SendFCamac(Slot+128);
	ReadCSRCamac();
	state = ReadDCamac();
	if((state&6) == 2)
	{
		FORWARD = TRUE;
		AfxMessageBox("CCW-Limit reached in last measurement!",MB_OK,0);
	}
	if((state&6) == 4)
	{
		FORWARD = FALSE;
		AfxMessageBox("CW-Limit reached in last measurement!",MB_OK,0);
	}
	if((state&6) == 6)
	{
		if(FORWARD)FORWARD = FALSE;
		else FORWARD = TRUE;
		AfxMessageBox("Both limits reached in last measurement!",MB_OK,0);
	}
}

void AlexMotorObj::TrackBeginAction(USHORT track)
{
	SendNAFCamac(Slot,0,24);
	if(FORWARD)SendNAFCamac(Slot,0,28);
	else SendNAFCamac(Slot,0,30);
}

void AlexMotorObj::TrackEndAction(USHORT track,USHORT scansdone)
{
}

void AlexMotorObj::TrackStepAction(USHORT step, USHORT track, USHORT scan)
{
	USHORT data = (USHORT)(20.75*MoveCm)&32767;
	if(!FORWARD) data = data|32768;
	ListDNAFCamac(SENDNAF,0,Slot,0,10);
	ListDNAFCamac(SENDNAF,0,Slot,0,26);
	ListDNAFCamac(SENDDNAF|WAITINT,data,Slot,0,16);
	ListDNAFCamac(READENCL,0,0,0,0);
	ListDNAFCamac(SENDNAF,0,Slot,0,10);
	ListDNAFCamac(SENDNAF,0,Slot,0,24);

	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	pApp->GlobalActPosition = pApp->GlobalActPosition+MoveCm/100;
}

/****************   Methods for Class OutBitObj  ***********************/

void OutBitObj::DoDoubleClickAction(void)
{
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);
	CPropShtBo propSheet(ShortName+" in Track ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);


	propSheet.m_page1.m_biton		= FALSE;
	propSheet.m_page1.m_bitoff		= FALSE;
	propSheet.m_page1.m_bittoggle	= FALSE;
	propSheet.m_page1.m_bitchannel	= TogglePosition;
	propSheet.m_page3.m_subaddress	= SubAddress;
	propSheet.m_page3.m_slot		= Slot;
	switch(ToggleType)
	{
		case 0:
			propSheet.m_page1.m_biton		= TRUE;
		break;
		case 1:
			propSheet.m_page1.m_bitoff		= TRUE;
		break;
		case 2:
			propSheet.m_page1.m_bittoggle	= TRUE;
		break;
	}
	if(propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			SubAddress		= propSheet.m_page3.m_subaddress;
			TogglePosition	= propSheet.m_page1.m_bitchannel;
			Slot			= propSheet.m_page3.m_slot;
			if(propSheet.m_page1.m_biton)		ToggleType = 0;
			if(propSheet.m_page1.m_bitoff)		ToggleType = 1;
			if(propSheet.m_page1.m_bittoggle)	ToggleType = 2;
			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
		}
		else Beep(1000,100);
	}
}

void OutBitObj::CopyObject(ActionObject* pSource)
{
	OutBitObj* pActionObject = (OutBitObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;
	TogglePosition	= pActionObject->TogglePosition;
	SubAddress		= pActionObject->SubAddress;
	Slot			= pActionObject->Slot;
	ToggleType		= pActionObject->ToggleType;
}

CString OutBitObj::GetInfo(void)
{
	CString answer;
	CString Toggle;
	switch(ToggleType)
	{
		case 0:
			Toggle = "ON"; 
		break;
		case 1:
			Toggle = "OFF"; 
		break;
		case 2:
			Toggle = "TOGGLE"; 
		break;
	}
	answer.Format("Bit %u %s in ch %u",SubAddress,Toggle,TogglePosition);
	return answer;
}

void OutBitObj::Load(CArchive& ar)
{
	Name=ReadString(ar);
	ShortName = __OutBitObj;
	ReadSeparator(ar);
	TogglePosition = (int)ReadFloat(ar);
	ReadSeparator(ar);
	ToggleType = (int)ReadFloat(ar);
	ReadSeparator(ar);
	SubAddress = (int)ReadFloat(ar);
	ReadStructEnd(ar);
}

void OutBitObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);
	WriteFloat(ar,TogglePosition);
	WriteSeparator(ar);
	WriteFloat(ar,ToggleType);
	WriteSeparator(ar);
	WriteFloat(ar,SubAddress);
	WriteStructEnd(ar);
	WriteLine(ar);
}

void OutBitObj::TrackBeginAction(USHORT track)
{
	if(TogglePosition == 0)
	{
		switch(ToggleType)
		{
		case 0:
			ListOnBit(0,SubAddress);
			break;
		case 1:
			ListOffBit(0,SubAddress);
			break;
		case 2:
			ListToggleBit(0,SubAddress);
			break;
		}
	}
}

void OutBitObj::TrackStepAction(USHORT step, USHORT track, USHORT scan)
{
	if(step==TogglePosition)
	{
		switch(ToggleType)
		{
		case 0:
			ListOnBit(0,SubAddress);
			break;
		case 1:
			ListOffBit(0,SubAddress);
			break;
		case 2:
			ListToggleBit(0,SubAddress);
			break;
		}
	}
}

void OutBitObj::TrackEndAction(USHORT track,USHORT scansdone)
{
	OffBit(0,SubAddress);
}

/****************   Methods for Class GPIBWriterObj  ***********************/

void GPIBWriterObj::DoDoubleClickAction(void)
{
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);
	CPropShtGW propSheet(ShortName+" in Track ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);

	propSheet.m_page1.m_sendstring = SendString;
	propSheet.m_page3.m_gpib = Gpib;
	if(propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			SendString = propSheet.m_page1.m_sendstring;
			Gpib = propSheet.m_page3.m_gpib;
			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
		}
		else Beep(1000,100);
	}
}

void GPIBWriterObj::CopyObject(ActionObject* pSource)
{
	GPIBWriterObj* pActionObject = (GPIBWriterObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;
	Gpib			= pActionObject->Gpib;
	SendString		= pActionObject->SendString;
}

CString GPIBWriterObj::GetInfo(void)
{
	CString answer;
	answer.Format("%s to %u",SendString,Gpib);
	return answer;
}

void GPIBWriterObj::Load(CArchive& ar)
{
	Name=ReadString(ar);
	ShortName = __GPIBWriterObj;
	ReadSeparator(ar);
	SendString = ReadString(ar);
	ReadSeparator(ar);
	Gpib = (int)ReadFloat(ar);
	ReadStructEnd(ar);
}

void GPIBWriterObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);
	WriteString(ar,SendString);
	WriteSeparator(ar);
	WriteFloat(ar,Gpib);
	WriteStructEnd(ar);
	WriteLine(ar);
}
void GPIBWriterObj::TrackBeginAction(USHORT track)
{
	ListSendGPIB(SendString,Gpib,Slot);
}

/****************   Methods for Class GPIBReaderObj  ***********************/

void GPIBReaderObj::DoDoubleClickAction(void)
{
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);
	CPropShtGR propSheet(ShortName+" in Track ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);

	propSheet.m_page1.m_sendstring = GPIBQuestion;
	propSheet.m_page3.m_gpib = Gpib;
	if(propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			GPIBQuestion = propSheet.m_page1.m_sendstring;
			Gpib = propSheet.m_page3.m_gpib;
			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
		}
		else Beep(1000,100);
	}
}

void GPIBReaderObj::CopyObject(ActionObject* pSource)
{
	GPIBReaderObj* pActionObject = (GPIBReaderObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;
	Gpib			= pActionObject->Gpib;
	GPIBQuestion	= pActionObject->GPIBQuestion;
	GPIBAnswer		= pActionObject->GPIBAnswer;
}

CString GPIBReaderObj::GetInfo(void)
{
	CString answer;
	answer.Format("%s to %u: %s",GPIBQuestion,Gpib,GPIBAnswer);
	return answer;
}

void GPIBReaderObj::Load(CArchive& ar)
{
	Name=ReadString(ar);
	ShortName = __GPIBReaderObj;
	ReadSeparator(ar);
	GPIBQuestion = ReadString(ar);
	ReadSeparator(ar);
	GPIBAnswer = ReadString(ar);
	ReadSeparator(ar);
	Gpib = (int)ReadFloat(ar);
	ReadStructEnd(ar);
}

void GPIBReaderObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);
	WriteString(ar,GPIBQuestion);
	WriteSeparator(ar);
	WriteString(ar,GPIBAnswer);
	WriteSeparator(ar);
	WriteFloat(ar,Gpib);
	WriteStructEnd(ar);
	WriteLine(ar);
}

void GPIBReaderObj::TrackBeginAction(USHORT track)
{
	ListSendReadGPIB(GPIBQuestion,Gpib,Slot);
}
void GPIBReaderObj::TrackEndAction(USHORT track,USHORT scansdone)
{
	char TheAnswer[255];
	USHORT length;
	ListReadS(&TheAnswer[0],&length);
	GPIBAnswer = TheAnswer;
}
/****************   Methods for Class FieldWriterObj  ***********************/

void FieldWriterObj::DoDoubleClickAction(void)
{
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);
	CPropShtFW propSheet(ShortName+" in Track ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);

	propSheet.m_page1.m_field = Field;
	propSheet.m_page3.m_gpib = Gpib;
	if(propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			Field = propSheet.m_page1.m_field;
			Gpib = propSheet.m_page3.m_gpib;
			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
		}
		else Beep(1000,100);
	}
}

void FieldWriterObj::CopyObject(ActionObject* pSource)
{
	FieldWriterObj* pActionObject = (FieldWriterObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;
	Gpib			= pActionObject->Gpib;
	Field			= pActionObject->Field;
}

CString FieldWriterObj::GetInfo(void)
{
	CString answer;
	CString in1;
	in1.Format("%g",Field);
	answer = "SendField: "+in1+" T";
	return answer;
}

void FieldWriterObj::Load(CArchive& ar)
{
	Name=ReadString(ar);
	ShortName = __FieldWriterObj;
	ReadSeparator(ar);
	Field = ReadFloat(ar);
	ReadSeparator(ar);
	Gpib = (int)ReadFloat(ar);
	ReadStructEnd(ar);
}

void FieldWriterObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);
	WriteFloat(ar,Field);
	WriteSeparator(ar);
	WriteFloat(ar,Gpib);
	WriteStructEnd(ar);
	WriteLine(ar);
}
void FieldWriterObj::TrackBeginAction(USHORT track)
{
	CString SendString;
	SendString.Format("CF%u",Field*10000);
	ListSendGPIB(SendString,Gpib,Slot);
}
/****************   Methods for Class FieldReaderObj  ***********************/

void FieldReaderObj::DoDoubleClickAction(void)
{
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);
	CPropShtFR propSheet(ShortName+" in Track ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);

	propSheet.m_page3.m_gpib = Gpib;
	if(propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			Gpib = propSheet.m_page3.m_gpib;
			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
		}
		else Beep(1000,100);
	}
}

void FieldReaderObj::CopyObject(ActionObject* pSource)
{
	FieldReaderObj* pActionObject = (FieldReaderObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;
	Gpib			= pActionObject->Gpib;
	Field			= pActionObject->Field;
}

CString FieldReaderObj::GetInfo(void)
{
	CString answer;
	CString in1;
	in1.Format("%g",Field);
	answer = "AqnField: "+in1+" T";
	return answer;
}

void FieldReaderObj::Load(CArchive& ar)
{
	Name=ReadString(ar);
	ShortName = __FieldReaderObj;
	ReadSeparator(ar);
	Field = ReadFloat(ar);
	ReadSeparator(ar);
	Gpib = (int)ReadFloat(ar);
	ReadStructEnd(ar);
}

void FieldReaderObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);
	WriteFloat(ar,Field);
	WriteSeparator(ar);
	WriteFloat(ar,Gpib);
	WriteStructEnd(ar);
	WriteLine(ar);
}
void FieldReaderObj::TrackBeginAction(USHORT track)
{
	ListSendReadGPIB("FC",Gpib,Slot);
}
void FieldReaderObj::TrackEndAction(USHORT track,USHORT scansdone)
{
	char TheAnswer[255];
	USHORT length;
	ListReadS(&TheAnswer[0],&length);
	Field = atof(&TheAnswer[2])/1.e4;
}
/****************   Methods for Class TempReaderObj  ***********************/

void TempReaderObj::DoDoubleClickAction(void)
{
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);
	CPropShtKR propSheet(ShortName+" in Track ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);

	propSheet.m_page3.m_gpib = Gpib;
	if(propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			Gpib = propSheet.m_page3.m_gpib;
			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
		}
		else Beep(1000,100);
	}
}

void TempReaderObj::CopyObject(ActionObject* pSource)
{
	TempReaderObj* pActionObject = (TempReaderObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;
	Gpib			= pActionObject->Gpib;
	Temp			= pActionObject->Temp;
}

CString TempReaderObj::GetInfo(void)
{
	CString answer;
	answer.Format("T1 %gK T2 %gK T3 %gK",T1,T2,T3);
	return answer;
}

void TempReaderObj::Load(CArchive& ar)
{
	Name=ReadString(ar);
	ShortName = __TempReaderObj;
	ReadSeparator(ar);
	Temp = ReadFloat(ar);
	ReadSeparator(ar);
	T1 = ReadFloat(ar);
	ReadSeparator(ar);
	T2 = ReadFloat(ar);
	ReadSeparator(ar);
	T3 = ReadFloat(ar);
	ReadSeparator(ar);
	Gpib = (int)ReadFloat(ar);
	ReadStructEnd(ar);
}

void TempReaderObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);
	WriteFloat(ar,Temp);
	WriteSeparator(ar);
	WriteFloat(ar,T1);
	WriteSeparator(ar);
	WriteFloat(ar,T2);
	WriteSeparator(ar);
	WriteFloat(ar,T3);
	WriteSeparator(ar);
	WriteFloat(ar,Gpib);
	WriteStructEnd(ar);
	WriteLine(ar);
}
void TempReaderObj::TrackBeginAction(USHORT track)
{
	ListSendReadGPIB("R1",Gpib,Slot);
	ListSendReadGPIB("R2",Gpib,Slot);
	ListSendReadGPIB("R3",Gpib,Slot);
}
void TempReaderObj::TrackEndAction(USHORT track,USHORT scansdone)
{
	char TheAnswer[255];
	USHORT length;
	ListReadS(&TheAnswer[0],&length);
	T1 = atof(&TheAnswer[1])/10.;
	ListReadS(&TheAnswer[0],&length);
	T2 = atof(&TheAnswer[1])/10.;
	ListReadS(&TheAnswer[0],&length);
	T3 = atof(&TheAnswer[1])/10.;
}

void TempReaderObj::MeasurementBeginAction(BOOL RUNMODE)
{
	SendGPIB("$Q2",Gpib,Slot);
}

/****************   Methods for Class TempWriterObj  ***********************/

void TempWriterObj::DoDoubleClickAction(void)
{
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);
	CPropShtKW propSheet(ShortName+" in Track ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);

	propSheet.m_page1.m_temp = SetTemp;
	propSheet.m_page3.m_gpib = Gpib;
	if(propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			SetTemp = propSheet.m_page1.m_temp;
			Gpib = propSheet.m_page3.m_gpib;
			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
		}
		else Beep(1000,100);
	}
}

void TempWriterObj::CopyObject(ActionObject* pSource)
{
	TempWriterObj* pActionObject = (TempWriterObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;
	Gpib			= pActionObject->Gpib;
	SetTemp			= pActionObject->SetTemp;
}

CString TempWriterObj::GetInfo(void)
{
	CString answer;
	CString in1;
	in1.Format("%g",SetTemp);
	answer = "Set Temperature: "+in1+" K";
	return answer;
}

void TempWriterObj::Load(CArchive& ar)
{
	Name=ReadString(ar);
	ShortName = __TempWriterObj;
	ReadSeparator(ar);
	SetTemp = ReadFloat(ar);
	ReadSeparator(ar);
	Gpib = (int)ReadFloat(ar);
	ReadStructEnd(ar);
}

void TempWriterObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);
	WriteFloat(ar,SetTemp);
	WriteSeparator(ar);
	WriteFloat(ar,Gpib);
	WriteStructEnd(ar);
	WriteLine(ar);
}

void TempWriterObj::TrackBeginAction(USHORT track)
{
	CString SendString;
	SendString.Format("$T%u",(USHORT)(SetTemp*10));
	ListSendGPIB(SendString,Gpib,Slot);
}

void TempWriterObj::MeasurementBeginAction(BOOL RUNMODE)
{
	SendGPIB("C3",Gpib,Slot);
	SendGPIB("Q2",Gpib,Slot);
}

/****************   Methods for Class RS_FrequencySweepObj  ***********************/

void RS_FrequencySweepObj::UpdateData(void)
{
	Sweep_Stop = CalcStop(Sweep_Start, Sweep_Stop, pTrack->Channels, RSResolution);
}

void RS_FrequencySweepObj::SetMinMax(double minstep,double maxstep)
{
	CalcStartStop(&Sweep_Start, &Sweep_Stop, pTrack->Channels, Resolution, minstep, maxstep);
}

void RS_FrequencySweepObj::SetCenter(USHORT step)
{
	CalcCenter(&Sweep_Start, &Sweep_Stop, pTrack->Channels, Resolution, step);
}

void RS_FrequencySweepObj::DoDoubleClickAction(void)
{
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);
	CPropShtRS propSheet(ShortName+" in Track ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);
		
	propSheet.m_page1.channels		= pTrack->Channels;
	propSheet.m_page1.resolution	= Resolution;

	propSheet.m_page1.startfreq		= Sweep_Start;
	propSheet.m_page1.stopfreq		= Sweep_Stop;
	propSheet.m_page1.ampdb			= Sweep_Amp;

	propSheet.m_page1.m_nomod		= FALSE;
	propSheet.m_page1.m_modam		= FALSE;
	propSheet.m_page1.m_modfm		= FALSE;
	propSheet.m_page1.m_modfreq		= Sweep_ModFreq;
	propSheet.m_page1.m_modampfm	= Sweep_ModAmpFM;
	propSheet.m_page1.m_modampam	= Sweep_ModAmpAM;

	propSheet.m_page2.m_noscannomod	= FALSE;
	propSheet.m_page2.m_noscanmodam	= FALSE;
	propSheet.m_page2.m_noscanmodfm	= FALSE;
	propSheet.m_page2.m_noscanmodfreq	= NoSweep_ModFreq;
	propSheet.m_page2.m_noscanmodampfm	= NoSweep_ModAmpFM;
	propSheet.m_page2.m_noscanmodampam	= NoSweep_ModAmpAM;

	switch(Sweep_Mode)
	{
		case 0:
		{
			propSheet.m_page1.m_nomod = TRUE;
			break;
		}
		
		case 1:
		{
			propSheet.m_page1.m_modfm = TRUE;
			break;
		}
		
		case 2:
		{
			propSheet.m_page1.m_modam = TRUE;
			break;
		}
	}

	switch(NoSweep_Mode)
	{
		case 0:
		{
			propSheet.m_page2.m_noscannomod = TRUE;
			break;
		}
		
		case 1:
		{
			propSheet.m_page2.m_noscanmodfm = TRUE;
			break;
		}
		
		case 2:
		{
			propSheet.m_page2.m_noscanmodam = TRUE;
			break;
		}
	}
	propSheet.m_page2.ampdb			= NoSweep_Amp;
	propSheet.m_page2.m_noscanfreq	= NoSweep_Value;
	
	propSheet.m_page3.m_gpib		= Gpib;
	
	if(IDOK == propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			Sweep_Start = propSheet.m_page1.startfreq;
			Sweep_Stop	= propSheet.m_page1.stopfreq;
			Sweep_Amp	=  propSheet.m_page1.ampdb;

			NoSweep_Amp = propSheet.m_page2.ampdb;
			NoSweep_Value = propSheet.m_page2.m_noscanfreq;

			if(propSheet.m_page1.m_nomod)
				Sweep_Mode = 0;
			if(propSheet.m_page1.m_modfm)
				Sweep_Mode = 1;
			if(propSheet.m_page1.m_modam)
				Sweep_Mode = 2;

			if(propSheet.m_page2.m_noscannomod)
				NoSweep_Mode = 0;
			if(propSheet.m_page2.m_noscanmodfm)
				NoSweep_Mode = 1;
			if(propSheet.m_page2.m_noscanmodam)
				NoSweep_Mode = 2;

			Sweep_ModFreq		= propSheet.m_page1.m_modfreq;
			Sweep_ModAmpFM		= propSheet.m_page1.m_modampfm;
			Sweep_ModAmpAM		= propSheet.m_page1.m_modampam;

			NoSweep_ModFreq		= propSheet.m_page2.m_noscanmodfreq;
			NoSweep_ModAmpFM	= propSheet.m_page2.m_noscanmodampfm;
			NoSweep_ModAmpAM	= propSheet.m_page2.m_noscanmodampam;

			Gpib		= propSheet.m_page3.m_gpib;

			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
			UpdateNoScan();
		}
		else Beep(1000,100);
	}
	UpdateNoScan();
};

void RS_FrequencySweepObj::CopyObject(ActionObject* pSource)
{
	RS_FrequencySweepObj* pActionObject = (RS_FrequencySweepObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;

	Sweep_Start			= pActionObject->Sweep_Start;
	Sweep_Step			= pActionObject->Sweep_Step;
	Sweep_Stop			= pActionObject->Sweep_Stop;
	Sweep_Resolution	= pActionObject->Sweep_Resolution;
	Sweep_Amp			= pActionObject->Sweep_Amp;
	Sweep_Mode			= pActionObject->Sweep_Mode;
	Sweep_ModFreq		= pActionObject->Sweep_ModFreq;
	Sweep_ModAmpFM		= pActionObject->Sweep_ModAmpFM;
	Sweep_ModAmpAM		= pActionObject->Sweep_ModAmpAM;
		
	NoSweep_Value		= pActionObject->NoSweep_Value;
	NoSweep_Amp			= pActionObject->NoSweep_Amp;
	NoSweep_Mode		= pActionObject->NoSweep_Mode;
	NoSweep_ModFreq		= pActionObject->NoSweep_ModFreq;
	NoSweep_ModAmpFM	= pActionObject->NoSweep_ModAmpFM;
	NoSweep_ModAmpAM	= pActionObject->NoSweep_ModAmpAM;
};

CString RS_FrequencySweepObj::GetInfo(void)
{
	CString answer;
	CString mod;
	double Step = CalcStep(Sweep_Start,Sweep_Stop,pTrack->Channels,Resolution);
	switch(Sweep_Mode)
	{
		case 0:
		{
			mod = "";
			break;
		}
		
		case 1:
		{
			mod.Format("FM %gkHz depth %gkHz",Sweep_ModFreq,Sweep_ModAmpFM);
			break;
		}
		
		case 2:
		{
			mod.Format("AM %gkHz depth %g%%",Sweep_ModFreq,Sweep_ModAmpAM);
			break;
		}
	}

	answer.Format("%gMHz to %gMHz step %gMHz ",Sweep_Start,Sweep_Stop,Step);
	answer = answer + mod;
	return answer;
};
void RS_FrequencySweepObj::Load(CArchive& ar)
{
	Resolution			= RSResolution;
	Name				= ReadString(ar);
	ShortName			= __RS_FrequencySweepObj;
	ReadSeparator(ar);
	Sweep_Start			= ReadFloat(ar);
	NoSweep_Value		= Sweep_Start;
	Sweep_Stop			= ReadFloat(ar);
	Sweep_Resolution	= ReadFloat(ar);
	if (pDocument->Version > 6)
	{
		Sweep_Mode		= (int)ReadFloat(ar);
		NoSweep_Mode	= Sweep_Mode;
		Sweep_ModFreq	= ReadFloat(ar);
		Sweep_ModFreq	= Sweep_ModFreq;
		NoSweep_ModFreq	= Sweep_ModFreq;
		if (pDocument->Version > 19)
		{
			Sweep_ModAmpFM		= ReadFloat(ar);
			NoSweep_ModAmpFM	= Sweep_ModAmpFM;
			Sweep_ModAmpAM		= Sweep_ModAmpFM;
			NoSweep_ModAmpAM	= Sweep_ModAmpFM;
		}
		Sweep_Amp		= ReadFloat(ar);
		NoSweep_Amp		= (double)-137;
	}
	ReadStructEnd(ar);

};
void RS_FrequencySweepObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);

	WriteFloat(ar,Sweep_Start);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Stop);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Resolution);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Mode);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_ModFreq);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_ModAmpFM);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Amp);

	WriteStructEnd(ar);
	WriteLine(ar);

}

void RS_FrequencySweepObj::MeasurementBeginAction(BOOL RUNMODE)
{
}

void RS_FrequencySweepObj::TrackBeginAction(USHORT track)
{
	CString SendString;
	switch(Sweep_Mode)
	{
	case 0:
		SendString = "FM:OFF;AM:OFF";
		break;
	case 1:
		SendString.Format("AM:OFF;AF %.8gkHz; FM:I %.8gkHz",Sweep_ModFreq,Sweep_ModAmpFM);
		break;
	case 2:
		SendString.Format("FM:OFF;AF %.8gkHz; AM:I %g",Sweep_ModFreq,Sweep_ModAmpAM);
		break;
	}
	ListSendGPIB(SendString,Gpib,Slot);
	SendString.Format("L %.8g",Sweep_Amp);
	ListSendGPIB(SendString,Gpib,Slot);
}

void RS_FrequencySweepObj::TrackStepAction(USHORT step, USHORT track, USHORT scan)
{
	CString StrFrequency;
	double Frequency  = CalcValue(Sweep_Start,Sweep_Stop,pTrack->Channels,step,Resolution);
	StrFrequency.Format("RF %.8g",Frequency*1e6);
	ListSendGPIB(StrFrequency,Gpib,Slot);
}

void RS_FrequencySweepObj::MeasurementEndAction(void)
{
	CString SendString;
	switch(NoSweep_Mode)
	{
	case 0:
		SendString = "FM:OFF;AM:OFF";
		break;
	case 1:
		SendString.Format("AM:OFF;AF %.8gkHz; FM:I %.8gkHz",NoSweep_ModFreq,NoSweep_ModAmpFM);
		break;
	case 2:
		SendString.Format("FM:OFF;AF %.8gkHz; AM %g",NoSweep_ModFreq,NoSweep_ModAmpAM);
		break;
	}
	SendGPIB(SendString,Gpib,Slot);
	SendString.Format("L %.8g",NoSweep_Amp);
	SendGPIB(SendString,Gpib,Slot);
	SendString.Format("RF %.8g",NoSweep_Value*1e6);
	SendGPIB(SendString,Gpib,Slot);
}

/****************   Methods for Class RS_FrequencySweepObj  ***********************/



/****************   Methods for Class Line_VoltageSweepObj  ***********************/

void Line_VoltageSweepObj::UpdateData(void)
{
	Sweep_Stop = CalcStop(Sweep_Start, Sweep_Stop, pTrack->Channels, Resolution);
}

void Line_VoltageSweepObj::SetMinMax(double minstep,double maxstep)
{
	CalcStartStop(&Sweep_Start, &Sweep_Stop, pTrack->Channels, Resolution, minstep, maxstep);
}

void Line_VoltageSweepObj::SetCenter(USHORT step)
{
	CalcCenter(&Sweep_Start, &Sweep_Stop, pTrack->Channels, Resolution, step);
}

void Line_VoltageSweepObj::DoDoubleClickAction(void)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);

	CPropShtLV propSheet(ShortName+" in Track ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);
	
	propSheet.m_page1.channels		= pTrack->Channels;
	propSheet.m_page1.resolution	= Resolution;
	propSheet.m_page2.resolution	= Resolution;

	propSheet.m_page1.startvolt		= Sweep_Start;
	propSheet.m_page1.stopvolt		= Sweep_Stop;

	propSheet.m_page1.m_slowreset	= SlowReset;
	propSheet.m_page1.m_slowresetch = SlowResetCh;

	NoSweep_Value					= pApp->LV0;
	propSheet.m_page2.m_noscanvolt	= NoSweep_Value;

	propSheet.m_page3.m_slot		= Slot;
	propSheet.m_page3.m_subaddress	= SubAddress;

	
	if(IDOK == propSheet.DoModal())
	{
		if(!pDocument->MeasurementRunning)
		{
			Sweep_Start = propSheet.m_page1.startvolt;
			Sweep_Stop	= propSheet.m_page1.stopvolt;

			SlowReset	= propSheet.m_page1.m_slowreset;
			SlowResetCh = propSheet.m_page1.m_slowresetch;

			NoSweep_Value = propSheet.m_page2.m_noscanvolt;
			pApp->LV0	  = NoSweep_Value;
			Slot		= propSheet.m_page3.m_slot;
			SubAddress	= propSheet.m_page3.m_subaddress;

			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
			UpdateNoScan();
		}
		else Beep(1000,100);
	}
	UpdateNoScan();

}

void Line_VoltageSweepObj::CopyObject(ActionObject* pSource)
{
	Line_VoltageSweepObj* pActionObject = (Line_VoltageSweepObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;


	Sweep_Start			= pActionObject->Sweep_Start;
	Sweep_Step			= pActionObject->Sweep_Step;
	Sweep_Stop			= pActionObject->Sweep_Stop;
	Sweep_Resolution	= pActionObject->Sweep_Resolution;
	Sweep_Amp			= pActionObject->Sweep_Amp;
	Sweep_Mode			= pActionObject->Sweep_Mode;
	Sweep_ModFreq		= pActionObject->Sweep_ModFreq;
	Sweep_ModAmpFM		= pActionObject->Sweep_ModAmpFM;
	Sweep_ModAmpAM		= pActionObject->Sweep_ModAmpAM;
		
	NoSweep_Value		= pActionObject->NoSweep_Value;
	NoSweep_Amp			= pActionObject->NoSweep_Amp;
	NoSweep_Mode		= pActionObject->NoSweep_Mode;
	NoSweep_ModFreq		= pActionObject->NoSweep_ModFreq;
	NoSweep_ModAmpFM	= pActionObject->NoSweep_ModAmpFM;
	NoSweep_ModAmpAM	= pActionObject->NoSweep_ModAmpAM;

	Sweep_ModFreq		= pActionObject->Sweep_ModFreq;
	Sweep_ModAmpFM		= pActionObject->Sweep_ModAmpFM;
	SlowReset			= pActionObject->SlowReset;
	SlowResetCh			= pActionObject->SlowResetCh;
}

CString Line_VoltageSweepObj::GetInfo(void)
{
	CString answer;
	double Step = CalcStep(Sweep_Start,Sweep_Stop,pTrack->Channels,Resolution);
	answer.Format("%.3fV to %.3fV step %.3fV",Sweep_Start,Sweep_Stop,Step);
	return answer;
}

void Line_VoltageSweepObj::Load(CArchive& ar)
{
	Resolution			= LineVoltageResolution;
	Name				= ReadString(ar);
	SlowReset			= FALSE;
	ShortName           = __Line_VoltageSweepObj; 
	ReadSeparator(ar);
	Sweep_Start			= ReadFloat(ar);
	NoSweep_Value		= 0;
	Sweep_Stop			= ReadFloat(ar);
	Sweep_Resolution	= ReadFloat(ar);
	if (pDocument->Version > 6)
	{
		Sweep_Mode		= (int)ReadFloat(ar);
		NoSweep_Mode = Sweep_Mode;
		Sweep_ModFreq		= ReadFloat(ar);
		if (pDocument->Version > 19)
		{
			Sweep_ModAmpFM	= ReadFloat(ar);
		}
		Sweep_Amp	= ReadFloat(ar);
	}
	ReadStructEnd(ar);

}

void Line_VoltageSweepObj::Save(CArchive& ar)
{
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);

	WriteFloat(ar,Sweep_Start);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Stop);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Resolution);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Mode);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_ModFreq);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_ModAmpFM);
	WriteSeparator(ar);
	WriteFloat(ar,Sweep_Amp);

	WriteStructEnd(ar);
	WriteLine(ar);

}

void Line_VoltageSweepObj::MeasurementBeginAction(BOOL RUNMODE)
{
}

void Line_VoltageSweepObj::MeasurementEndAction(void)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	long Data = (long)((-pApp->LV0/1e1)*8388608)+8388608;
	//SendNAFCamac(Slot,(UCHAR)1,(UCHAR)10);
	//SendDNAFCamac(Data,Slot,(UCHAR)0,(UCHAR)16);
	Data = (long)((pApp->LV0/1e1)*131072);
	if(pApp->LV0<0) Data = Data | 1<<17;
	SendDNAFCamac(Data,Slot,(UCHAR)0,(UCHAR)16);
}

void Line_VoltageSweepObj::ListVoltage(USHORT step,double Voltage)
{
	ULONG Data = (ULONG)((-Voltage/1e1)*8388608)+8388608;
	//ListDNAFCamac(SENDNAF,0,Slot,(UCHAR)1,(UCHAR)10);
	//ListDNAFCamac(SENDDNAF,Data,Slot,(UCHAR)0,(UCHAR)16);
	//ListDNAFCamac(READENCL,0,0,0,0);
	Data = (ULONG)((Voltage/1e1)*131072);
	if((Data&131071)==0)Data = 0;
	ListDNAFCamac(SENDDNAF,Data,Slot,(UCHAR)0,(UCHAR)16);
}

void Line_VoltageSweepObj::TrackBeginAction(USHORT track)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
	long Data = (long)((-pApp->LV0/1e1)*8388608)+8388608;
	//SendNAFCamac(Slot,(UCHAR)1,(UCHAR)10);
	//SendDNAFCamac(Data,Slot,(UCHAR)0,(UCHAR)16);
	Data = (long)((pApp->LV0/1e1)*131072);
	if(pApp->LV0<0) Data = Data | 1<<17;
	SendDNAFCamac(Data,Slot,(UCHAR)0,(UCHAR)16);
}

void Line_VoltageSweepObj::TrackStepAction(USHORT step, USHORT track, USHORT scan)
{
	const int ResetCh = 100;

	double Voltage  = CalcValue(Sweep_Start,Sweep_Stop,pTrack->Channels,step,Resolution);

	ListVoltage(step,Voltage);

	if(SlowReset&&step==pTrack->Channels)
	{
		USHORT i;
		CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
		
		for(i=ResetCh;i>0;i--)
		{
			double Voltage2  = (Sweep_Stop-Sweep_Start)*(double)(i-1.0)/(double)(ResetCh-1)+Sweep_Start;
			ListDelayCamac(pApp->PresetSlot,(USHORT)(1000*SlowResetCh/ResetCh));
			ListVoltage(i,Voltage2);
		}
	}
}

/****************   Methods for Class TriggerObj  ***********************/
/* Old CamacTimer

void TriggerObj::TakeData(BOOL RUNMODE, USHORT scan, USHORT channel, ULONG TheData)
{
	if(scan==1&&RUNMODE==ERGO)y[channel] = (float)TheData;
	else y[channel] = TheData+y[channel];
	y[0]=y[1];
}

void TriggerObj::DoDoubleClickAction(void)
{
	USHORT result;
	CString TrackNo;
	CString XName,XUnit;
	TrackNo.Format("%u",pTrack->MyPosition);
	CPropShtTI propSheet(ShortName+" in Track ["+TrackNo+"] of "+pDocument->GetTitle(),AfxGetMainWnd(),0);

	propSheet.m_page1.openbeamgate	= (long)OpenBeamgate;
	propSheet.m_page1.openscaler	= (long)OpenScaler;
	propSheet.m_page1.closescaler	= (long)CloseScaler;
	propSheet.m_page1.startbeamgate	= StartBeamgate;
	propSheet.m_page1.stopbeamgate	= StopBeamgate;
	
	propSheet.m_page1.m_notrigger = FALSE;
	propSheet.m_page1.m_steptrigger = FALSE;
	propSheet.m_page1.m_sweeptrigger= FALSE;

	if(TriggerType	== 0)propSheet.m_page1.m_notrigger = TRUE;
	if(TriggerType	== 1)propSheet.m_page1.m_steptrigger = TRUE;
	if(TriggerType	== 2)propSheet.m_page1.m_sweeptrigger = TRUE;

	propSheet.m_page3.m_slot= Slot;

	result = propSheet.DoModal();
	if(IDOK == result)
	{
		if(!pDocument->MeasurementRunning)
		{
			OpenBeamgate	= propSheet.m_page1.openbeamgate;
			OpenScaler		= propSheet.m_page1.openscaler;
			CloseScaler		= propSheet.m_page1.closescaler;
			StartBeamgate	= propSheet.m_page1.startbeamgate;
			StopBeamgate	= propSheet.m_page1.stopbeamgate;

			if(propSheet.m_page1.m_notrigger)TriggerType		= 0;
			if(propSheet.m_page1.m_steptrigger)TriggerType		= 1;
			if(propSheet.m_page1.m_sweeptrigger)TriggerType		= 2;

			Slot = propSheet.m_page3.m_slot;
			pDocument->SetModifiedFlag(TRUE);
			pDocument->UpdateAllViews(NULL,0,NULL);
			
		}
		else Beep(1000,100);
	}

	if(IDC_SHOWPROTONS == result)
	{
		BOOL ENDE = FALSE;
		USHORT track;
		int i;
		double Start,Stop;
		CString DoNew = "";
		CString TheObject="";
		CString TrackNo;
		CString ScalNo;
		CString XName,XUnit;
		BOOL Arith = FALSE;
		TrackNo.Format("%u",pTrack->MyPosition);
		
		XName = "Channel";
		XUnit = "";
		Start = (double)1;
		Stop  = (double)pTrack->Channels;

		if(pDataView==NULL)pDataView = new DataView(this);
		else pDataView->Reset();
		pDataView->SetWindowTitle(ShortName+" "+GetInfo()+" in Track ["+TrackNo+"]  of "+pDocument->GetTitle());


		pDataView->SetYTitle("Proton Counts","");
		POSITION pos=pDocument->ActionObjList.GetHeadPosition();
		int Channels = pTrack->Channels;
		
		track = pTrack->MyPosition;

		pDataView->SetXTitle(XName,XUnit);
		pDataView->SetTitle(pTrack->Isotope);
		for(i=0;i<pTrack->Channels;i++)
		{
			pDataView->AddDataPoint((float)(i*((Stop-Start)/(Channels-1))+Start),
									(float)y[i],
									(float)sqrt(fabs(y[i])));
		}
		pDataView->data_type = data_type;
		pDataView->RedrawNow();
	}
}

void TriggerObj::CopyObject(ActionObject* pSource)
{
	TriggerObj* pActionObject = (TriggerObj*) pSource;

	pDocument		= pActionObject->pDocument;
	pTrack			= pActionObject->pTrack;	
	HardwareReady	= pActionObject->HardwareReady;
	Name			= pActionObject->Name;
	ShortName		= pActionObject->ShortName;
	DoString		= pActionObject->DoString;
	Isotope			= pActionObject->Isotope;

	OpenBeamgate	= pActionObject->OpenBeamgate;
	OpenScaler		= pActionObject->OpenScaler;
	CloseBeamgate	= pActionObject->CloseBeamgate;
	CloseScaler		= pActionObject->CloseScaler;
	TriggerType		= pActionObject->TriggerType;
}

CString TriggerObj::GetInfo(void)
{
	CString answer;
	CString in1;
	CString in2;
	CString Trigger;
	switch(TriggerType)
	{
	case 0:
		Trigger = "No Trigger";
		break;
	case 1:
		Trigger = "Step";
		break;
	case 2:
		Trigger = "Sweep";
		break;
	}
	in2.Format("delay %gs",OpenScaler/1000);
	if(OpenScaler==0)in2="";
	in1.Format("Meas %gs %s %s",(CloseScaler-OpenScaler)/1000,in2,Trigger);
	return in1;
}

void TriggerObj::Load(CArchive& ar)
{
	Resolution			= TriggerResolution;
	int i;
	Name = ReadString(ar);
	ShortName = __TriggerObj;
	ReadSeparator(ar);
		
	ReadStructBegin(ar);
	for (i=0;i<pTrack->Channels;i++)
	{
		y[i]   = ReadFloat(ar);
	}
	ReadStructEnd(ar);
	ReadSeparator(ar);
	OpenBeamgate	= ReadFloat(ar);
	ReadSeparator(ar);
	OpenScaler		= ReadFloat(ar);
	ReadSeparator(ar);
	CloseBeamgate	= ReadFloat(ar);
	ReadSeparator(ar);
	CloseScaler		= ReadFloat(ar);
	ReadSeparator(ar);
	StartBeamgate	= (USHORT)ReadFloat(ar);
	ReadSeparator(ar);
	StopBeamgate	= (USHORT)ReadFloat(ar);
	ReadSeparator(ar);
	TriggerType		= (int)ReadFloat(ar);

	ReadStructEnd(ar);
}

void TriggerObj::Save(CArchive& ar)
{
	USHORT i,j=0;
	WriteNameBegin(ar);
	WriteString(ar,Name);
	WriteNameEnd(ar);
	WriteLine(ar);
	WriteStructBegin(ar);
	WriteString(ar,Name);
	WriteSeparator(ar);

	WriteStructBegin(ar);
	for (i=0;i<pTrack->Channels;i++)
	{
		WriteFloat(ar,y[i]);
		if(i!=(pTrack->Channels-1))WriteSeparator(ar);
		j++;
		if(j==5)
		{
			j = 0;
			WriteLine(ar);
		}
	}
	WriteStructEnd(ar);

	WriteSeparator(ar);
	WriteFloat(ar,OpenBeamgate);
	WriteSeparator(ar);
	WriteFloat(ar,OpenScaler);
	WriteSeparator(ar);
	WriteFloat(ar,CloseBeamgate);
	WriteSeparator(ar);
	WriteFloat(ar,CloseScaler);
	WriteStructEnd(ar);
	WriteFloat(ar,StartBeamgate);
	WriteSeparator(ar);
	WriteFloat(ar,StopBeamgate);
	WriteStructEnd(ar);
	WriteFloat(ar,TriggerType);
	WriteStructEnd(ar);

	WriteLine(ar);
}

void TriggerObj::MeasurementBeginAction(BOOL RUNMODE)
{
	USHORT i;
	if(RUNMODE==ERGO)for(i=0;i<MAXPOINTS;i++)y[i]=0;

	SendFCamac(64+Slot);
	SendNAFCamac(Slot,0,26);
}

void TriggerObj::MeasurementEndAction(void)
{
	SendFCamac(128+Slot);
	SendNAFCamac(Slot,0,24);
	SendNAFCamac(Slot,0,8);
}

void TriggerObj::TrackBeginAction(USHORT track)
{
	double ticks;

	ticks = 10e3*CloseScaler;
	ticks = ticks/65536;
	
	if(ticks>32768)
	{
		divisor = 262144;
		fac		= 64;
	}
	if(ticks<=32768 && ticks>4096)
	{
		divisor = 32768;
		fac		= 32;
	}
	if(ticks<=4096 && ticks>512)
	{
		divisor = 4096;
		fac		= 16;
	}
	if(ticks<=512 && ticks>64)
	{
		divisor = 512;
		fac		= 8;
	}
	if(ticks<=64 && ticks>8)
	{
		divisor = 64;
		fac		= 4;
	}
	if(ticks<=8 && ticks>1)
	{
		divisor = 8;
		fac		= 2;
	}
	if(ticks<=1)
	{
		divisor = 1;
		fac		= 1;
	}
	SendDNAFCamac(fac,Slot,1,16);
	SendNAFCamac(Slot,0,10);
	SendNAFCamac(Slot,0,8);
	ReadENCLCamac();
}

void TriggerObj::TrackStepAction(USHORT step, USHORT track, USHORT scan)
{

	ListDNAFCamac(SENDNAF,0,(UCHAR)ProtonSlot,(UCHAR)ProtonSubadd,(UCHAR)0);
	ListDNAFCamac(READD,0,0,0,0);
	pDocument->DataQueue[pDocument->DataQueueIndex].scan = scan;
	pDocument->DataQueue[pDocument->DataQueueIndex].step = step;
	pDocument->DataQueue[pDocument->DataQueueIndex].pAction = this;
	pDocument->DataQueueIndex++;
	ListDNAFCamac(SENDNAF,0,(UCHAR)ProtonSlot,(UCHAR)ProtonSubadd,(UCHAR)9);	
	

	ListDNAFCamac(SENDDNAF,fac,Slot,1,16);
	if(step==1)
	{
		ListDNAFCamac(SENDNAF,0,Slot,0,10);
		ListDNAFCamac(SENDNAF,0,Slot,0,8);
		ListDNAFCamac(READENCL,0,0,0,0);
	}
	//ListDNAFCamac(SENDDNAF,fac,Slot,1,16);
	if(OpenBeamgate!=0)
	{
		if(TriggerType == 0)
		{
			ListDNAFCamac(SENDDNAF,(USHORT)(OpenBeamgate*10e3/divisor),Slot,0,16);
			ListDNAFCamac(SENDNAF|WAITINT,0,Slot,0,28);
		}
		if(TriggerType == 1)
		{
			ListDNAFCamac(SENDNAF|WAITINT,0,Slot,0,27);
			ListDNAFCamac(SENDNAF,0,Slot,0,8);
			ListDNAFCamac(READENCL,0,0,0,0);
			ListDNAFCamac(SENDDNAF,(USHORT)(OpenBeamgate*10e3/divisor),Slot,0,16);
			ListDNAFCamac(SENDNAF|WAITINT,0,Slot,0,28);
		}
		if(TriggerType == 2)
		{
			if(step==1)
			{
				ListDNAFCamac(SENDNAF|WAITINT,0,Slot,0,27);
				ListDNAFCamac(SENDNAF,0,Slot,0,8);
				ListDNAFCamac(READENCL,0,0,0,0);
				ListDNAFCamac(SENDDNAF,(USHORT)(OpenBeamgate*10e3/divisor),Slot,0,16);
				ListDNAFCamac(SENDNAF|WAITINT,0,Slot,0,28);
			}
			else
			{
				ListDNAFCamac(SENDDNAF,(USHORT)(OpenBeamgate*10e3/divisor),Slot,0,16);
				ListDNAFCamac(SENDNAF|WAITINT,0,Slot,0,28);
			}
		}
		ListDNAFCamac(SENDNAF,0,Slot,0,10);
		ListDNAFCamac(READENCL,0,0,0,0);
	}
	
	if(OpenScaler!=0&&OpenScaler!=OpenBeamgate)
	{
		if(TriggerType == 0)
		{
			if((step>=StartBeamgate)&&(step<=StopBeamgate))ListOnBit(0,14);
			ListDNAFCamac(SENDDNAF,(USHORT)((OpenScaler-OpenBeamgate)*10e3/divisor),Slot,0,16);
			ListDNAFCamac(SENDNAF|WAITINT,0,Slot,0,28);
		}
		if(TriggerType == 1)
		{
			if(OpenBeamgate==0)
			{
				ListDNAFCamac(SENDNAF|WAITINT,0,Slot,0,27);
				ListDNAFCamac(SENDNAF,0,Slot,0,10);
				ListDNAFCamac(SENDNAF,0,Slot,0,8);
				ListDNAFCamac(READENCL,0,0,0,0);
			}
			if((step>=StartBeamgate)&&(step<=StopBeamgate))ListOnBit(0,14);
			ListDNAFCamac(SENDDNAF,(USHORT)((OpenScaler-OpenBeamgate)*10e3/divisor),Slot,0,16);
			ListDNAFCamac(SENDNAF|WAITINT,0,Slot,0,28);
		}
		if(TriggerType == 2)
		{
			if(step==1)
			{
				if(OpenBeamgate==0)
				{
					ListDNAFCamac(SENDNAF|WAITINT,0,Slot,0,27);
					ListDNAFCamac(SENDNAF,0,Slot,0,10);
					ListDNAFCamac(SENDNAF,0,Slot,0,8);
					ListDNAFCamac(READENCL,0,0,0,0);
				}
				if((step>=StartBeamgate)&&(step<=StopBeamgate))ListOnBit(0,14);
				ListDNAFCamac(SENDDNAF,(USHORT)((OpenScaler-OpenBeamgate)*10e3/divisor),Slot,0,16);
				ListDNAFCamac(SENDNAF|WAITINT,0,Slot,0,28);
			}
			else
			{
				if((step>=StartBeamgate)&&(step<=StopBeamgate))ListOnBit(0,14);
				ListDNAFCamac(SENDDNAF,(USHORT)((OpenScaler-OpenBeamgate)*10e3/divisor),Slot,0,16);
				ListDNAFCamac(SENDNAF|WAITINT,0,Slot,0,28);
			}
		}
		ListDNAFCamac(SENDNAF,0,Slot,0,10);
		ListDNAFCamac(READENCL,0,0,0,0);
	}

	
	if(TriggerType == 0)
	{
		if((step>=StartBeamgate)&&(step<=StopBeamgate))ListOnBit(0,14);
		ListDNAFCamac(SENDDNAF,(USHORT)((CloseScaler-OpenScaler)*10e3/divisor),Slot,0,16);
		ListDNAFCamac(SENDNAF|WAITINT,0,Slot,0,28);
	}
	if(TriggerType == 1)
	{
		if(OpenBeamgate==0&&OpenScaler==0)
		{
			ListDNAFCamac(SENDNAF|WAITINT,0,Slot,0,27);
			ListDNAFCamac(SENDNAF,0,Slot,0,10);
			ListDNAFCamac(SENDNAF,0,Slot,0,8);
			ListDNAFCamac(READENCL,0,0,0,0);
		}
		if((step>=StartBeamgate)&&(step<=StopBeamgate))ListOnBit(0,14);
		ListDNAFCamac(SENDDNAF,(USHORT)((CloseScaler-OpenScaler)*10e3/divisor),Slot,0,16);
		ListDNAFCamac(SENDNAF|WAITINT,0,Slot,0,28);
	}
	if(TriggerType == 2)
	{
		if(step==1)
		{
			if(OpenBeamgate==0&&OpenScaler==0)
			{
				ListDNAFCamac(SENDNAF|WAITINT,0,Slot,0,27);
				ListDNAFCamac(SENDNAF,0,Slot,0,10);
				ListDNAFCamac(SENDNAF,0,Slot,0,8);
				ListDNAFCamac(READENCL,0,0,0,0);
			}
			if((step>=StartBeamgate)&&(step<=StopBeamgate))ListOnBit(0,14);
			ListDNAFCamac(SENDDNAF,(USHORT)((CloseScaler-OpenScaler)*10e3/divisor),Slot,0,16);
			ListDNAFCamac(SENDNAF|WAITINT,0,Slot,0,28);
		}
		else
		{
			if((step>=StartBeamgate)&&(step<=StopBeamgate))ListOnBit(0,14);
			ListDNAFCamac(SENDDNAF,(USHORT)((CloseScaler-OpenScaler)*10e3/divisor),Slot,0,16);
			ListDNAFCamac(SENDNAF|WAITINT,0,Slot,0,28);
		}
	}

	ListDNAFCamac(SENDNAF,0,Slot,0,10);
	ListDNAFCamac(READENCL,0,0,0,0);
	ListOffBit(0,14);
}
*/