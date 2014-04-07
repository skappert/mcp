// ActionObjects.h : interface of the ActionObject class
//
/////////////////////////////////////////////////////////////////////////////
#include "HardwareConfig.h"

#define ERGO	0
#define GO		1
#define AUTOGO	2

#define MAXPOINTS 4000

class DataView;

double CalcStop(double start, double stop, USHORT channels, double resolution);
void CalcStartStop(double* start, double* stop, USHORT channels, double resolution, double newmin, double newmax);
void CalcCenter(double* start, double* stop, USHORT channels, double resolution, USHORT step);

/************************ ActionObject ***************************/


class ActionObject
	{
	public:
		ActionObject()
		{
			DataCount	= 1;
			Resolution	= 1e-31;
			Slot		= 0;
			SubAddress	= 0;
			Gpib		= 0;
			pDocument		= NULL;
			pTrack			= NULL;
			HardwareReady	= FALSE;
			SCALER			= FALSE;
			ARITHSCALER		= FALSE;
			TakesData		= FALSE;
			ShowExpected	= FALSE;
			data_type		= 2;
			DispMonitorMode	= 0;

			Name			= "";
			ShortName		= "";
			DoString		= "";
 			Isotope			= "";
			pDataView		= NULL;
		}
		
		CMCPforNTDoc*	pDocument;
		TrackObject*	pTrack;
		DataView*		pDataView;

		BOOL	HardwareReady;
		int		Slot;
		int		SubAddress;
		int		Gpib;

		double	Resolution;

		CString Name;
		CString ShortName;
		CString DoString;
 		CString	Isotope;

		CString scan_data_op;
		CString scan_error_op;


		CString Title;
		BOOL	SCALER;
		BOOL	ARITHSCALER;

		BOOL	TakesData;
		BOOL	ShowExpected;
		USHORT  DataCount;

		int		data_type;
		int		DispMonitorMode;

		virtual ~ActionObject();
		virtual CString GetName(void){return "HP_VoltageSweepObj";};
		virtual CString GetInfo(void){return " ";};
		virtual void CopyObject(ActionObject* pSource);

		virtual BOOL TestHardware(void){return TRUE;};
		virtual void DoDoubleClickAction(void);
		virtual void DoSummingAction(ActionObject* pDestination);
		virtual void UpdateDisplay(void);
		virtual void Load(CArchive& ar);
		virtual void Save(CArchive& ar);
		virtual void MeasurementBeginAction(BOOL RUNMODE);
		virtual void MeasurementEndAction(void);
		virtual void TrackBeginAction(USHORT track);
		virtual void TrackEndAction(USHORT track,USHORT scansdone);
		virtual void TrackStepAction(USHORT step, USHORT track, USHORT scan);
		virtual void DoConfigureAction(void);
		virtual void UpdateData(void);
		virtual void TakeData(BOOL RUNMODE, USHORT scan, USHORT channel, ULONG TheData);
		virtual double GetY(USHORT channel);
		virtual double GetYErr(USHORT channel);
		virtual double EstimateY(USHORT channel, USHORT scan);
		virtual double EstimateYErr(USHORT channel, USHORT scan);
		virtual void   SetCenter(USHORT step);
		virtual void   SetMinMax(double minstep,double maxstep);
		virtual void   UpdateNoScan();
	};

	/************************ TrackObject ***************************/

	class TrackObject				:public ActionObject
	{
	public:
		TrackObject()
		{
			Slot			= 0;
			SubAddress		= 0;
			Gpib			= 0;
			Name			= _TrackObj;
			ShortName		= _TrackObj;

			MyPosition		= 0;
			pTrack			= this;
			NoOfScans		= 1;
			RealScans		= 0;
			Channels		= 101;
			Resolution2		= 0;
			RepeatSteps		= 0;
			UpDownSweep		= 0;
			ProtonLimit		= 1000;
			ProtonAction	= 0;
			ArithCounter	= 42;
			ScalerCounter	= 1;
			RS_FrequencyNum	= 0;
			HP_FrequencyNum	= 0;
		}

		long ProtonLimit;
		USHORT ProtonAction;
		int MyPosition;
		int ArithCounter;
		int ScalerCounter;
		int	NoOfScans;
		int	RealScans;
		int	Channels;
		int	Resolution2;
		int	RepeatSteps;
		int	UpDownSweep;
		int	RS_FrequencyNum;
		int	HP_FrequencyNum;

		CString GetName(void){return ShortName;};
		CString GetInfo(void);
		void CopyObject(ActionObject* pSource);

		BOOL TestHardware(void){return TRUE;};
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		void DoSummingAction(ActionObject* pDestination);
		void MeasurementBeginAction(BOOL RUNMODE);
		void MeasurementEndAction(void);
		void TrackBeginAction(USHORT track);
		void TrackEndAction(USHORT track,USHORT scansdone);
		void TrackStepAction(USHORT step, USHORT track, USHORT scan);
	
	};

					

	/************************ HP_VoltageSweepObj ***************************/

	class HP_VoltageSweepObj		:public ActionObject
	{
	public:
		HP_VoltageSweepObj()
		{
			Resolution	= HPVoltageResolution;
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->HpSlot;
			SubAddress	= 0;
			Gpib		= 0;

			Name			= _HP_VoltageSweepObj;
			ShortName		= __HP_VoltageSweepObj;


			Sweep_Start			= 0;
			Sweep_Step			= 0;
			Sweep_Stop			= 0;
			Sweep_Resolution	= 0;
			Sweep_Amp			= 0;
			Sweep_Mode			= 0;
			Sweep_ModFreq		= 0;
			Sweep_ModAmpFM		= 0;
			Sweep_ModAmpAM		= 0;
		
			NoSweep_Value		= 0;
			NoSweep_Amp			= 0;
			NoSweep_Mode		= 0;
			NoSweep_ModFreq		= 0;
			NoSweep_ModAmpFM	= 0;
			NoSweep_ModAmpAM	= 0;

			SlowResetCh			= 6;
			NoSweep_Value		= 0;
			SlowReset			= FALSE;
		}

		double	VoltageOld;
		double	Sweep_Start;
		double	Sweep_Step;
		double	Sweep_Stop;
		double	Sweep_Resolution;
		double	Sweep_Amp;
		long	Sweep_Mode;
		double	Sweep_ModFreq;
		double	Sweep_ModAmpFM;
		double	Sweep_ModAmpAM;
		
		double	NoSweep_Value;
		double	NoSweep_Amp;
		long	NoSweep_Mode;
		double	NoSweep_ModFreq;
		double	NoSweep_ModAmpFM;
		double	NoSweep_ModAmpAM;
		BOOL	SlowReset;
		double	SlowResetCh;


		CString GetName(void){return ShortName;};
		void DoDoubleClickAction(void);
		void CopyObject(ActionObject* pSource);
		CString GetInfo(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		void MeasurementEndAction(void);
		void TrackStepAction(USHORT step, USHORT track, USHORT scan);
		void UpdateData(void);
		void ListVoltage(USHORT step,double Voltage);
		void SetMinMax(double minstep,double maxstep);
		void   SetCenter(USHORT step);
	};


	/************************ HP8660B_FrequencySweepObj ***************************/

	class HP8660B_FrequencySweepObj	: public ActionObject
	{
	public:
		HP8660B_FrequencySweepObj()
		{
			Resolution	= HP8660Resolution;
			Slot		= 1;
			SubAddress	= 0;
			Gpib		= 0;
			Name			= _HP8660B_FrequencySweepObj;
			ShortName		= __HP8660B_FrequencySweepObj;


			Sweep_Start			= 1;
			Sweep_Step			= 0;
			Sweep_Stop			= 1;
			Sweep_Resolution	= 0;
			Sweep_Amp			= -20;
			Sweep_Mode			= 0;
			Sweep_ModFreq		= 0;
			Sweep_ModAmpFM		= 0;
			Sweep_ModAmpAM		= 0;

			NoSweep_Value		= 0;
			NoSweep_Amp			= -137;
			NoSweep_Mode		= 0;
			NoSweep_ModFreq		= 0;
			NoSweep_ModAmpFM	= 0;
			NoSweep_ModAmpAM	= 0;
		}

		double Sweep_Start;
		double Sweep_Step;
		double Sweep_Stop;
		double Sweep_Resolution;
		double Sweep_Amp;
		long  Sweep_Mode;
		double Sweep_ModFreq;
		double Sweep_ModAmpFM;
		double	Sweep_ModAmpAM;
		double NoSweep_Value;
		double NoSweep_Amp;
		long  NoSweep_Mode;
		double NoSweep_ModFreq;
		double NoSweep_ModAmpAM;
		double NoSweep_ModAmpFM;

		CString GetName(void){return ShortName;};
		BOOL TestHardware(void){return FALSE;};
		void DoDoubleClickAction(void);
		void CopyObject(ActionObject* pSource);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		void UpdateData(void);
		CString GetInfo(void);
		void SetMinMax(double minstep,double maxstep);
		void SetCenter(USHORT step);
	};


	/************************ HP3325B_FrequencySweepObj ***************************/

	class HP3325B_FrequencySweepObj	: public ActionObject
	{
	public:
		HP3325B_FrequencySweepObj()
		{
			Resolution	= HP3325Resolution;
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->GpibSlot;
			Gpib		= pApp->Hp1Gpib;
			Name			= _HP3325B_FrequencySweepObj;
			ShortName		= __HP3325B_FrequencySweepObj;


			Sweep_Start			= 1;
			Sweep_Step			= 0;
			Sweep_Stop			= 1;
			Sweep_Resolution	= 0;
			Sweep_Amp			= -20;
			Sweep_Mode			= 0;
			Sweep_ModFreq		= 0;
			Sweep_ModAmpFM		= 0;
		
			NoSweep_Value		= 0;
			NoSweep_Amp			= -137;
			NoSweep_Mode		= 0;
			NoSweep_ModFreq		= 0;
			NoSweep_ModAmpFM	= 0;
			NoSweep_ModAmpAM	= 0;
		}


		double Sweep_Start;
		double Sweep_Step;
		double Sweep_Stop;
		double Sweep_Resolution;
		double Sweep_Amp;
		long  Sweep_Mode;
		double Sweep_ModFreq;
		double Sweep_ModAmpFM;
		double	Sweep_ModAmpAM;
		double NoSweep_Value;
		double NoSweep_Amp;
		long  NoSweep_Mode;
		double NoSweep_ModFreq;
		double NoSweep_ModAmpAM;
		double NoSweep_ModAmpFM;		


		CString GetName(void){return ShortName;};
		void DoDoubleClickAction(void);
		void CopyObject(ActionObject* pSource);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		void UpdateData(void);
		CString GetInfo(void);
		void SetMinMax(double minstep,double maxstep);
		void   SetCenter(USHORT step);
	};


	/************************ HP3325B2_FrequencySweepObj ***************************/

	class HP3325B2_FrequencySweepObj: public ActionObject
	{
	public:
		HP3325B2_FrequencySweepObj()
		{
			Resolution	= HP3325Resolution;
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->GpibSlot;
			Gpib		= pApp->Hp2Gpib;
			Name			= _HP3325B2_FrequencySweepObj;
			ShortName		= __HP3325B2_FrequencySweepObj;


			Sweep_Start			= 1;
			Sweep_Step			= 0;
			Sweep_Stop			= 1;
			Sweep_Resolution	= 0;
			Sweep_Amp			= -20;
			Sweep_Mode			= 0;
			Sweep_ModFreq		= 0;
			Sweep_ModAmpFM		= 0;
		
			NoSweep_Value		= 0;
			NoSweep_Amp			= -137;
			NoSweep_Mode		= 0;
			NoSweep_ModFreq		= 0;
			NoSweep_ModAmpFM	= 0;
			NoSweep_ModAmpAM	= 0;
		}


		double Sweep_Start;
		double Sweep_Step;
		double Sweep_Stop;
		double Sweep_Resolution;
		double Sweep_Amp;
		long  Sweep_Mode;
		double Sweep_ModFreq;
		double Sweep_ModAmpFM;
		double	Sweep_ModAmpAM;
		double NoSweep_Value;
		double NoSweep_Amp;
		long  NoSweep_Mode;
		double NoSweep_ModFreq;
		double NoSweep_ModAmpAM;
		double NoSweep_ModAmpFM;


		CString GetName(void){return ShortName;};
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		void CopyObject(ActionObject* pSource);
		void UpdateData(void);
		CString GetInfo(void);
		void SetMinMax(double minstep,double maxstep);
		void   SetCenter(USHORT step);
	};


	/************************ RS_FrequencySweepObj ***************************/

	class RS_FrequencySweepObj		: public ActionObject
	{
	public:
		RS_FrequencySweepObj()
		{
			Resolution			= RSResolution;
			CMCPforNTApp* pApp	= (CMCPforNTApp*)AfxGetApp();
			Slot				= pApp->GpibSlot;
			Gpib				= pApp->Rs1Gpib;
			
			Name				= _RS_FrequencySweepObj;
			ShortName			= __RS_FrequencySweepObj;


			Sweep_Start			= 1;
			Sweep_Step			= 0;
			Sweep_Stop			= 1;
			Sweep_Resolution	= 0;
			Sweep_Amp			= -20;
			Sweep_Mode			= 0;
			Sweep_ModFreq		= 0;
			Sweep_ModAmpFM		= 0;
			Sweep_ModAmpAM		= 0;
		
			NoSweep_Value		= 1;
			NoSweep_Amp			= -137;
			NoSweep_Mode		= 0;
			NoSweep_ModFreq		= 0;
			NoSweep_ModAmpFM	= 0;
			NoSweep_ModAmpAM	= 0;
		}

		double Sweep_Start;
		double Sweep_Step;
		double Sweep_Stop;
		double Sweep_Resolution;
		double Sweep_Amp;
		long  Sweep_Mode;
		double Sweep_ModFreq;
		double Sweep_ModAmpAM;
		double Sweep_ModAmpFM;
		
		double NoSweep_Value;
		double NoSweep_Amp;
		long  NoSweep_Mode;
		double NoSweep_ModFreq;
		double NoSweep_ModAmpAM;
		double NoSweep_ModAmpFM;


		CString GetName(void){return ShortName;};
		void DoDoubleClickAction(void);
		void CopyObject(ActionObject* pSource);
		void Load(CArchive& ar);
		void Save(CArchive& ar);

		void UpdateData(void);
		CString GetInfo(void);
		void SetMinMax(double minstep,double maxstep);
		void SetCenter(USHORT step);

		void MeasurementBeginAction(BOOL RUNMODE);
		void MeasurementEndAction(void);
		void TrackBeginAction(USHORT track);
		void TrackStepAction(USHORT step, USHORT track, USHORT scan);
	};


	/************************ PM_SpectrumObj ***************************/

	class PM_SpectrumObj			: public ActionObject
	{
	public:
		PM_SpectrumObj::PM_SpectrumObj()
		{
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->ScalerSlot;
			SubAddress	= 0;
			Gpib		= 0;

			Name			= _PM_SpectrumObj;
			ShortName		= __PM_SpectrumObj;
			DoString		= "normal";
			scan_data_op	= "";
			scan_error_op	= "";
			USHORT i;
			for(i=0;i<MAXPOINTS;i++)y[i]	= 0;
			SubRegNo				= 0;
			Sweep_Resolution		= 0;
			ChannelOffset			= 0;
			AnalysisMode			= 0;
			PredecessorSubRegNo		= 0;
			CountScansFlag			= 0;
   
			scan_data_op			= "0";
			scan_error_op.Empty();
			ShowExpected			= TRUE;
			TakesData				= TRUE;
			ShowSumVoltage			= FALSE;
		}  

		double   y[MAXPOINTS];
		int		SubRegNo;
		double	Sweep_Resolution;
		int		ChannelOffset;
		int		AnalysisMode;
		int		PredecessorSubRegNo;
		BOOL	CountScansFlag;
		BOOL	ShowSumVoltage;
		

		CString GetName(void){return ShortName;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void DoConfigureAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		void DoSummingAction(ActionObject* pDestination);
		void MeasurementBeginAction(BOOL RUNMODE);
		void MeasurementEndAction(void);
		void TrackBeginAction(USHORT track);
		void TrackStepAction(USHORT step, USHORT track, USHORT scan);
		void UpdateDisplay(void);
		double GetY(USHORT channel);
		double GetYErr(USHORT channel);
		void  TakeData(BOOL RUNMODE, USHORT scan, USHORT channel, ULONG TheData);
		double EstimateY(USHORT channel, USHORT scan);
		double EstimateYErr(USHORT channel, USHORT scan);
		CString GetInfo(void);
	};
	

	/************************ PremaVoltageObj ***************************/

	class PremaVoltageObj			: public ActionObject
	{
	public:
		PremaVoltageObj()
		{
			CMCPforNTApp* pApp	= (CMCPforNTApp*)AfxGetApp();
			Slot				= pApp->PremaSlot;
			SubAddress			= 0;
			Gpib				= 0;
			Name				= _PremaVoltageObj;
			ShortName			= __PremaVoltageObj;


			Factor				= 0;
			DelayBeforeMeas		= pApp->PremaDelay;
			USHORT i;
			for(i=0;i<100;i++)Voltage[i]= 0;
			NumOfSamples		= 0;

			TakesData			= FALSE;
			DataCount			= 0;
		}

		ULONG		w0;
		ULONG		w1;

		USHORT		LastChannel;

		double		Factor;
		double		DelayBeforeMeas;
		double		Voltage[100];
		int			NumOfSamples;
		
		CString GetName(void){return ShortName;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void DoConfigureAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		void MeasurementBeginAction(BOOL RUNMODE);
		void MeasurementEndAction(void);
		void TrackBeginAction(USHORT track);
		double GetY(USHORT channel);
		double GetYErr(USHORT channel);
		//void TrackStepAction(USHORT step, USHORT track, USHORT scan);
		void  TakeData(BOOL RUNMODE, USHORT scan, USHORT channel, ULONG TheData);
		CString GetInfo(void);
	};

	/************************ FlukeVoltageObj ***************************/

	class FlukeVoltageObj			: public ActionObject
	{
	public:
		FlukeVoltageObj()
		{
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->PremaSlot;
			SubAddress	= 0;
			Gpib		= 0;
			Name			= _FlukeVoltageObj;
			ShortName		= __FlukeVoltageObj;


			Factor			= 0;
			DelayBeforeMeas	= 0;
			USHORT i;
			for(i=0;i<100;i++)Voltage[i]= 0;
			NumOfSamples	= 0;
		}


		double		Factor;
		double		DelayBeforeMeas;
		double		Voltage[100];
		int			NumOfSamples;

		CString GetName(void){return ShortName;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void DoConfigureAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
		
	};

	/************************ IsoldeNetVoltageObj ***************************/

	class IsoldeNetVoltageObj		: public ActionObject
	{
	public:
		IsoldeNetVoltageObj()
		{
			Slot		= 0;
			SubAddress	= 0;
			Gpib		= 0;
			Name			= _IsoldeNetVoltageObj;
			ShortName		= __IsoldeNetVoltageObj;


			Factor			= 1000;
			DelayBeforeMeas	= 0;
			USHORT i;
			for(i=0;i<100;i++)Voltage[i]= 0;
			NumOfSamples	= 0;
		}


		double		Factor;
		double		DelayBeforeMeas;
		double		Voltage[100];
		int			NumOfSamples;

		CString GetName(void){return ShortName;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void DoConfigureAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
		void MeasurementBeginAction(BOOL RUNMODE);
		void TrackBeginAction(USHORT track);
	};
	
	/************************ KepcoEichungVoltageObj ***************************/

	class KepcoEichungVoltageObj	: public ActionObject
	{
	public:
		KepcoEichungVoltageObj()
		{
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->PremaSlot;
			SubAddress	= 0;
			Gpib		= 0;
			Name			= _KepcoEichungVoltageObj;
			ShortName		= __KepcoEichungVoltageObj;


			Factor			= 0;
			DelayBeforeMeas	= pApp->PremaDelay;
			USHORT i;
			for(i=0;i<MAXPOINTS;i++)Voltage[i]= 0;
			NumOfSamples	= 0;
			TakesData		= TRUE;
			DataCount		= 2;
			DispMonitorMode = 1;
		}


		double		Factor;
		double		DelayBeforeMeas;
		double		Voltage[MAXPOINTS];
		int			NumOfSamples;
		ULONG		w0;
		ULONG		w1;

		USHORT		LastChannel;

		void CopyObject(ActionObject* pSource);
		CString GetName(void){return ShortName;};
		CString GetInfo(void);
		void DoDoubleClickAction(void);
		void DoConfigureAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		void MeasurementBeginAction(BOOL RUNMODE);
		void MeasurementEndAction(void);
		void TrackBeginAction(USHORT track);
		void TrackStepAction(USHORT step, USHORT track, USHORT scan);
		//void TrackEndAction(USHORT track,USHORT scansdone);
		double GetY(USHORT channel);
		double GetYErr(USHORT channel);
		void  TakeData(BOOL RUNMODE, USHORT scan, USHORT channel, ULONG TheData);
	};
	
	/************************ ComputerDelayObj ***************************/

	class ComputerDelayObj			: public ActionObject
	{
	public:
		ComputerDelayObj()
		{
			Slot		= 0;
			SubAddress	= 0;
			Gpib		= 0;
			Name			= _ComputerDelayObj;
			ShortName		= __ComputerDelayObj;


			TimerClicksPerStep			= 0;
			TimerClickLength			= 0;
			CloseGateAfterClicks		= 0;
			IgnoreProtons				= 0;
			ShowChannels				= 0;
			OpenBeamgateAfterChannel	= 0;
			ScalerOpenBitChange			= 0;
			DoCallCrossList				= 0;
			CloseBeamgateAfterChannel	= 0;
			DelayTime					= 0;
		}

		int   TimerClicksPerStep;
		double TimerClickLength;
		int   CloseGateAfterClicks;
		BOOL  IgnoreProtons;
		BOOL  ShowChannels;
		int   OpenBeamgateAfterChannel;
		int   ScalerOpenBitChange;
		int   DoCallCrossList;
		int   CloseBeamgateAfterChannel;
		double DelayTime;
		
		CString GetName(void){return ShortName;};
		BOOL TestHardware(void){return FALSE;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
	};
	
	/************************ TimerDelayObj ***************************/

	class TimerDelayObj				: public ActionObject
	{
	public:
		TimerDelayObj()
		{
			Resolution	= 1e-3;
			Slot		= 0;
			SubAddress	= 0;
			Gpib		= 0;
			Name			= _TimerDelayObj;
			ShortName		= __TimerDelayObj;


			TimerClicksPerStep			= 0;
			TimerClickLength			= 0;
			CloseGateAfterClicks		= 0;
			IgnoreProtons				= 0;
			ShowChannels				= 0;
			OpenBeamgateAfterChannel	= 0;
			ScalerOpenBitChange			= 0;
			DoCallCrossList				= 0;
			CloseBeamgateAfterChannel	= 0;
		}

		int   TimerClicksPerStep;
		double TimerClickLength;
		int   CloseGateAfterClicks;
		BOOL  IgnoreProtons;
		BOOL  ShowChannels;
		int   OpenBeamgateAfterChannel;
		int   ScalerOpenBitChange;
		int   DoCallCrossList;
		int   CloseBeamgateAfterChannel;

		CString GetName(void){return ShortName;};
		BOOL TestHardware(void){return FALSE;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
	};
	
	/************************ StepTimerDelayObj ***************************/

	class StepTimerDelayObj			: public ActionObject
	{
	public:
		StepTimerDelayObj()
		{
			Slot		= 0;
			SubAddress	= 0;
			Gpib		= 0;
			Name			= _StepTimerDelayObj;
			ShortName		= __StepTimerDelayObj;


			TimerClicksPerStep			= 0;
			TimerClickLength			= 0;
			CloseGateAfterClicks		= 0;
			IgnoreProtons				= 0;
			ShowChannels				= 0;
			OpenBeamgateAfterChannel	= 0;
			ScalerOpenBitChange			= 0;
			DoCallCrossList				= 0;
			CloseBeamgateAfterChannel	= 0;
		}


		int   TimerClicksPerStep;
		double TimerClickLength;
		int   CloseGateAfterClicks;
		BOOL  IgnoreProtons;
		BOOL  ShowChannels;
		int   OpenBeamgateAfterChannel;
		int   ScalerOpenBitChange;
		int   DoCallCrossList;
		int   CloseBeamgateAfterChannel;

		CString GetName(void){return ShortName;};
		BOOL TestHardware(void){return FALSE;};
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
		void CopyObject(ActionObject* pSource);
	};
	
	/************************ FastTimerDelayObj ***************************/

	class FastTimerDelayObj			: public ActionObject
	{
	public:
		FastTimerDelayObj()
		{
			Slot		= 0;
			SubAddress	= 0;
			Gpib		= 0;
			Name			= _FastTimerDelayObj;
			ShortName		= __FastTimerDelayObj;


			TimerClicksPerStep			= 0;
			TimerClickLength			= 0;
			CloseGateAfterClicks		= 0;
			IgnoreProtons				= 0;
			ShowChannels				= 0;
			OpenBeamgateAfterChannel	= 0;
			ScalerOpenBitChange			= 0;
			DoCallCrossList				= 0;
			CloseBeamgateAfterChannel	= 0;
		}


		int   TimerClicksPerStep;
		double TimerClickLength;
		int   CloseGateAfterClicks;
		BOOL  IgnoreProtons;
		BOOL  ShowChannels;
		int   OpenBeamgateAfterChannel;
		int   ScalerOpenBitChange;
		int   DoCallCrossList;
		int   CloseBeamgateAfterChannel;

		CString GetName(void){return ShortName;};
		BOOL TestHardware(void){return FALSE;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
	};
	
	/************************ Keithley_VoltageMeasObj ***************************/

	class Keithley_VoltageMeasObj	: public ActionObject
	{
	public:
		Keithley_VoltageMeasObj()
		{
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->PremaSlot;
			SubAddress	= 0;
			Gpib		= 0;
			Name			= _Keithley_VoltageMeasObj;
			ShortName		= __Keithley_VoltageMeasObj;


			MeasAtScanNo		= 0;
			NoOfMeasPerStep		= 0;
			DelayAfterTrigger	= 0;
			TotalMeasTime		= 0;
			RangeNo				= 0;
			Reso				= 0;
			USHORT i;
			for(i=0;i<MAXPOINTS;i++)Voltage[i]= 0;
			NumOfSamples		= 0;
			TakesData			= TRUE;
		}

		double MeasAtScanNo;
		int   NoOfMeasPerStep;
		double DelayAfterTrigger;
		double TotalMeasTime;
		double RangeNo;
		double Reso;
		double Voltage[100];
		int	  NumOfSamples;

		CString GetName(void){return ShortName;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		double GetY(USHORT channel);
		double GetYErr(USHORT channel);
		void  TakeData(BOOL RUNMODE, USHORT scan, USHORT channel, ULONG TheData);

		CString GetInfo(void);
	};
	
	/************************ FlukeOsziObj ***************************/

	class FlukeOsziObj				: public ActionObject
	{
	public:
		FlukeOsziObj()
		{
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->GpibSlot;
			SubAddress	= 0;
			Gpib		= pApp->FlukeGpib;
			Name			= _FlukeOsziObj;
			ShortName		= __FlukeOsziObj;
		}
		CString GetName(void){return ShortName;};
		BOOL TestHardware(void){return FALSE;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
	};
	
	/************************ MassAndFlukeSwitchObj ***************************/

	class MassAndFlukeSwitchObj		:public ActionObject
	{
	public:
		MassAndFlukeSwitchObj()
		{
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->GelbeboxSlot;
			SubAddress	= 0;
			Gpib		= 0;
			Name			= _MassAndFlukeSwitchObj;
			ShortName		= __MassAndFlukeSwitchObj;


			ToFlukeNo	= 0;
			ToMassNo	= 0;
		}
		BOOL	UseHrs;
		int		ToFlukeNo;
		double	ToMassNo;
		BOOL	IsRadioactive;
		BOOL	DontSetMass;

		CString GetName(void){return ShortName;};
		void DoDoubleClickAction(void);
		void CopyObject(ActionObject* pSource);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
	};
	
	/************************ MassSwitchObj ***************************/

	class MassSwitchObj		:public ActionObject
	{
	public:
		MassSwitchObj()
		{
			Slot		= 0;
			SubAddress	= 0;
			Gpib		= 0;
			Name			= _MassSwitchObj;
			ShortName		= __MassSwitchObj;

			ToMassNo	= 0;
		}

		double	ToMassNo;
		BOOL	UseHrs;
		BOOL	IsRadioactive;
		BOOL	DontSetMass;

		CString GetName(void){return ShortName;};
		void DoDoubleClickAction(void);
		void CopyObject(ActionObject* pSource);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
		void TrackBeginAction(USHORT track);
	};
	
	/************************ FlukeSwitchObj ***************************/

	class FlukeSwitchObj		:public ActionObject
	{
	public:
		FlukeSwitchObj()
		{
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->GelbeboxSlot;
			SubAddress	= 0;
			Gpib		= 0;
			Name			= _FlukeSwitchObj;
			ShortName		= __FlukeSwitchObj;
			ToFlukeNo	= 0;
		}

		int   ToFlukeNo;
		CString GetName(void){return ShortName;};
		void DoDoubleClickAction(void);
		void CopyObject(ActionObject* pSource);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
		void MeasurementBeginAction(BOOL RUNMODE);
		void TrackBeginAction(USHORT track);
		void MeasurementEndAction(void);
	};
	
	/************************ TriggerSweepObj ***************************/

	class TriggerSweepObj			: public ActionObject
	{
	public:
		TriggerSweepObj()
		{
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->GelbeboxSlot;
			SubAddress	= 0;
			Gpib		= 0;
			Name			= _TriggerSweepObj;
			ShortName		= __TriggerSweepObj;


			USHORT i;
			for(i=0;i<MAXPOINTS;i++)y[i]	= 0;
			ProtonCounts			= 0;
			TriggerDelay			= 0;
			CheckProtons			= 0;
		}

		double y[MAXPOINTS];
		double ProtonCounts;
		double TriggerDelay;
		double CheckProtons;

		CString GetName(void){return ShortName;};
		BOOL TestHardware(void){return FALSE;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
	};
	
	/************************ StepTriggerObj ***************************/

	class StepTriggerObj			: public ActionObject
	{
	public:
		StepTriggerObj()
		{
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->GelbeboxSlot;
			SubAddress	= 0;
			Gpib		= 0;
			Name			= _StepTriggerObj;
			ShortName		= __StepTriggerObj;


			USHORT i;
			for(i=0;i<MAXPOINTS;i++)y[i]	= 0;
			ProtonCounts			= 0;
			TriggerDelay			= 0;
			CheckProtons			= 0;
			ReadProtons				= 0;
			RedoStepFlag			= 0;
		}

		double y[MAXPOINTS];
		double ProtonCounts;
		double TriggerDelay;
		double CheckProtons;
		double ReadProtons;
		BOOL  RedoStepFlag;

		CString GetName(void){return ShortName;};
		BOOL TestHardware(void){return FALSE;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
	};
	
	/************************ TriggerObj ***************************/

	class TriggerObj			: public ActionObject
	{
	public:
		TriggerObj()
		{
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->PresetSlot;
			SubAddress	= 0;
			ProtonSlot	= pApp->ProtonSlot;
			ProtonSubadd= pApp->ProtonSubadd;
			Gpib		= 0;
			Name			= _TriggerObj;
			ShortName		= __TriggerObj;


			USHORT i;
			for(i=0;i<MAXPOINTS;i++)y[i]	= 0;
			OpenBeamgate			= 0;
			OpenScaler				= 0;
			CloseBeamgate			= 1150;
			CloseScaler				= 1150;
			StartBeamgate			= 1;
			StopBeamgate			= 9999;
			TriggerType				= 0;
			TakesData				= TRUE;
		}
		double	y[MAXPOINTS];
		double	OpenBeamgate;
		double	OpenScaler;
		double	CloseBeamgate;
		double	CloseScaler;
		USHORT	StartBeamgate;
		USHORT	StopBeamgate;
		USHORT	ProtonSlot;
		USHORT	ProtonSubadd;
		int		TriggerType;

		CString GetName(void){return ShortName;};
		BOOL TestHardware(void){return TRUE;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		void MeasurementBeginAction(BOOL RUNMODE);
		void MeasurementEndAction(void);
		void TrackBeginAction(USHORT track);
		void TrackStepAction(USHORT step, USHORT track, USHORT scan);
		void TakeData(BOOL RUNMODE, USHORT scan, USHORT channel, ULONG TheData);
		
		CString GetInfo(void);
	};
	
	/************************ TriggerPartSweepObj ***************************/

	class TriggerPartSweepObj		: public ActionObject
	{
	public:
		TriggerPartSweepObj()
		{
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->GelbeboxSlot;
			SubAddress	= 0;
			Gpib		= 0;
			Name			= _TriggerPartSweepObj;
			ShortName		= __TriggerPartSweepObj;


			TriggerAfterNoOfSteps = 0;
		}

		int TriggerAfterNoOfSteps;
		CString GetName(void){return ShortName;};
		BOOL TestHardware(void){return FALSE;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
	};
	
	
	class AlexMotorObj				: public ActionObject
	{
	public:
		AlexMotorObj()
		{
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot			= pApp->AlexMotorSlot;
			SubAddress		= 0;
			Gpib			= 0;
			Name			= _AlexMotorObj;
			ShortName		= __AlexMotorObj;
			ActPosition		= 0;
			UseBrake		= FALSE;
			MoveCm			= 10;
			DonePosition	= 0;
			LeftPosition	= 0;
			MoveTo			= 0;
			FORWARD			= TRUE;
		}

		BOOL FORWARD;
		BOOL UseBrake;
		double ActPosition;
		double MoveCm;
		double DonePosition;
		double LeftPosition;
		double MoveTo;
		
		CString GetName(void){return ShortName;};
		BOOL TestHardware(void){return TRUE;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
		void TrackBeginAction(USHORT track);
		void TrackEndAction(USHORT track,USHORT scansdone);
		void TrackStepAction(USHORT step, USHORT track, USHORT scan);
		void MeasurementBeginAction(BOOL RUNMODE);
		void MeasurementEndAction(void);
	};
	
	/************************ MultiFunctionObj ***************************/

	class MultiFunctionObj			: public ActionObject
	{
	public:
		MultiFunctionObj()
		{
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->GelbeboxSlot;
			SubAddress	= 0;
			Gpib		= 0;
			Name			= _MultiFunctionObj;
			ShortName		= __MultiFunctionObj;


		}
		CString GetName(void){return ShortName;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
	};
	
	/************************ FDAC_VoltageSweepObj ***************************/

	class FDAC_VoltageSweepObj		: public ActionObject
	{
	public:
		FDAC_VoltageSweepObj()
		{
			Resolution	= FDACResolution;
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->FSlot;
			SubAddress	= 0;
			Gpib		= 0;
			Name			= _FDAC_VoltageSweepObj;
			ShortName		= __FDAC_VoltageSweepObj;


			Sweep_Start			= 0;
			Sweep_Step			= 0;
			Sweep_Stop			= 0;
			Sweep_Resolution	= 0;
			Sweep_SubAddress	= 0;
			NoSweep_Value		= 0;
			SwitchMode			= 0;
			ChangePos1			= 0;
			ChangePos2			= 0;
			ChangePos3			= 0;
			ChangePos4			= 0;
		}

		double Sweep_Start;
		double Sweep_Step;
		double Sweep_Stop;
		double Sweep_Resolution;
		double Sweep_SubAddress;
		double NoSweep_Value;
		int   SwitchMode;
		double ChangePos1;
		double ChangePos2;
		double ChangePos3;
		double ChangePos4;

		CString GetName(void){return ShortName;};
		void DoDoubleClickAction(void);
		void CopyObject(ActionObject* pSource);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		void UpdateData(void);
		void SetMinMax(double minstep,double maxstep);
		void   SetCenter(USHORT step);
		void TrackStepAction(USHORT step, USHORT track, USHORT scan);
		void MeasurementBeginAction(BOOL RUNMODE);
		void MeasurementEndAction(void);
		void TrackBeginAction(USHORT track);
		CString GetInfo(void);
	};

	/************************ CamacLPObj ***************************/
	
	class CamacLPObj				: public ActionObject
	{
	public:
		CamacLPObj()
		{
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->ListSlot;
			SubAddress	= 0;
			Gpib		= 0;
			Name			= _CamacLPObj;
			ShortName		= __CamacLPObj;


		}

		CString GetName(void){return ShortName;};
		BOOL TestHardware(void){return FALSE;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
	};
	
	/************************ CCDCameraObj ***************************/

	class CCDCameraObj				: public ActionObject
	{
	public:
		CCDCameraObj()
		{
			Slot		= 0;
			SubAddress	= 0;
			Gpib		= 0;
			Name			= _CCDCameraObj;
			ShortName		= __CCDCameraObj;


		}

		CString GetName(void){return ShortName;};
		BOOL TestHardware(void){return FALSE;};
		void DoDoubleClickAction(void);
		void CopyObject(ActionObject* pSource);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
	};
	
	/************************ DummyObj ***************************/
	
	class DummyObj					: public ActionObject
	{
	public:
		DummyObj()
		{
			Slot		= 0;
			SubAddress	= 0;
			Gpib		= 0;
			Name			= _DummyObj;
			ShortName		= __DummyObj;


		}

		CString GetName(void){return ShortName;};
		BOOL TestHardware(void){return FALSE;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
	};

	/************************ FieldReaderObj ***************************/
	
	class FieldReaderObj				: public ActionObject
	{
	public:
		FieldReaderObj()
		{
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->GpibSlot;
			SubAddress	= 0;
			Gpib		= pApp->FieldGpib;
			Name		= _FieldReaderObj;
			ShortName	= __FieldReaderObj;
			Field		= 0;
		}

		double Field;
		CString GetName(void){return ShortName;};
		BOOL TestHardware(void){return TRUE;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
		void TrackBeginAction(USHORT track);
		void TrackEndAction(USHORT track,USHORT scansdone);
	};

	/************************ TempReaderObj ***************************/

	class TempReaderObj				: public ActionObject
	{
	public:
		TempReaderObj()
		{
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->GpibSlot;
			SubAddress	= 0;
			Gpib		= pApp->TempGpib;
			Name		= _TempReaderObj;
			ShortName	= __TempReaderObj;
			Temp		= 0;
			T1			= 0;
			T2			= 0;
			T3			= 0;
		}

		double Temp;
		double T1;
		double T2;
		double T3;
		CString GetName(void){return ShortName;};
		BOOL TestHardware(void){return TRUE;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
		void TrackBeginAction(USHORT track);
		void TrackEndAction(USHORT track,USHORT scansdone);
		void MeasurementBeginAction(BOOL RUNMODE);
	};
		
	/************************ OutBitObj ***************************/

	class OutBitObj				: public ActionObject
	{
	public:
		OutBitObj()
		{
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->Gelbebox2Slot;
			SubAddress	= 0;
			Gpib		= 0;
			Name		= _OutBitObj;
			ShortName	= __OutBitObj;
			TogglePosition = 1;
		}

		USHORT TogglePosition;
		USHORT ToggleType;
		CString GetName(void){return ShortName;};
		BOOL TestHardware(void){return TRUE;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);

		void TrackStepAction(USHORT step, USHORT track, USHORT scan);
		void TrackBeginAction(USHORT track);
		void TrackEndAction(USHORT track,USHORT scansdone);
	};

	/************************ TempWriterObj ***************************/

	class TempWriterObj				: public ActionObject
	{
	public:
		TempWriterObj()
		{
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->GpibSlot;
			SubAddress	= 0;
			Gpib		= pApp->TempGpib;
			Name		= _TempWriterObj;
			ShortName	= __TempWriterObj;
			SetTemp		= 293;
		}

		double SetTemp;
		CString GetName(void){return ShortName;};
		BOOL TestHardware(void){return TRUE;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
		void TrackBeginAction(USHORT track);
		void MeasurementBeginAction(BOOL RUNMODE);
	};

	/************************ FieldWriterObj ***************************/
		
	class FieldWriterObj				: public ActionObject
	{
	public:
		FieldWriterObj()
		{
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->GpibSlot;
			SubAddress	= 0;
			Gpib		= pApp->FieldGpib;
			Name		= _FieldWriterObj;
			ShortName	= __FieldWriterObj;
			Field		= 0;
		}

		double Field;
		CString GetName(void){return ShortName;};
		BOOL TestHardware(void){return TRUE;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
		void TrackBeginAction(USHORT track);
	};

	/************************ GPIBReaderObj ***************************/

	class GPIBReaderObj				: public ActionObject
	{
	public:
		GPIBReaderObj()
		{
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->GpibSlot;
			SubAddress	= 0;
			Gpib		= 1;
			Name		= _GPIBReaderObj;
			ShortName	= __GPIBReaderObj;
			GPIBQuestion= "*IDN?";
			GPIBAnswer	= "";
		}

		CString GPIBQuestion;
		CString GPIBAnswer;
		CString GetName(void){return ShortName;};
		BOOL TestHardware(void){return TRUE;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
		void TrackBeginAction(USHORT track);
		void TrackEndAction(USHORT track,USHORT scansdone);
	};

	/************************ GPIBWriterObj ***************************/
	
	class GPIBWriterObj				: public ActionObject
	{
	public:
		GPIBWriterObj()
		{
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->GpibSlot;
			SubAddress	= 0;
			Gpib		= 1;
			Name		= _GPIBWriterObj;
			ShortName	= __GPIBWriterObj;
			SendString	= "";
		}

		CString SendString;
		CString GetName(void){return ShortName;};
		BOOL TestHardware(void){return TRUE;};
		void CopyObject(ActionObject* pSource);
		void DoDoubleClickAction(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		CString GetInfo(void);
		void TrackBeginAction(USHORT track);
	};
	
	/************************ Line_VoltageSweepObj ***************************/

	class Line_VoltageSweepObj		:public ActionObject
	{
	public:
		Line_VoltageSweepObj()
		{
			Resolution	= LineVoltageResolution;
			CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
			Slot		= pApp->HvSlot;
			SubAddress	= 0;
			Gpib		= 0;

			Name			= _Line_VoltageSweepObj;
			ShortName		= __Line_VoltageSweepObj;


			Sweep_Start			= 0;
			Sweep_Step			= 0;
			Sweep_Stop			= 0;
			Sweep_Resolution	= 0;
			Sweep_Amp			= 0;
			Sweep_Mode			= 0;
			Sweep_ModFreq		= 0;
			Sweep_ModAmpFM		= 0;
			Sweep_ModAmpAM		= 0;
		
			NoSweep_Value		= 0;
			NoSweep_Amp			= 0;
			NoSweep_Mode		= 0;
			NoSweep_ModFreq		= 0;
			NoSweep_ModAmpFM	= 0;
			NoSweep_ModAmpAM	= 0;

			SlowResetCh			= 6;
			NoSweep_Value		= 0;
			SlowReset			= FALSE;
		}

		double	VoltageOld;
		double	Sweep_Start;
		double	Sweep_Step;
		double	Sweep_Stop;
		double	Sweep_Resolution;
		double	Sweep_Amp;
		long	Sweep_Mode;
		double	Sweep_ModFreq;
		double	Sweep_ModAmpFM;
		double	Sweep_ModAmpAM;
		
		double	NoSweep_Value;
		double	NoSweep_Amp;
		long	NoSweep_Mode;
		double	NoSweep_ModFreq;
		double	NoSweep_ModAmpFM;
		double	NoSweep_ModAmpAM;
		BOOL	SlowReset;
		double	SlowResetCh;


		CString GetName(void){return ShortName;};
		void DoDoubleClickAction(void);
		void CopyObject(ActionObject* pSource);
		CString GetInfo(void);
		void Load(CArchive& ar);
		void Save(CArchive& ar);
		void MeasurementBeginAction(BOOL RUNMODE);
		void MeasurementEndAction(void);
		void TrackStepAction(USHORT step, USHORT track, USHORT scan);
		void UpdateData(void);
		void ListVoltage(USHORT step,double Voltage);
		void SetMinMax(double minstep,double maxstep);
		void   SetCenter(USHORT step);
	};

