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

class CModelessAfx
{
public:	
	CModelessAfx( CString str ) 
	{
		char *buffer=new char[256]; 
		lstrcpy(buffer,str);		 				
 		AfxBeginThread(showMessage, (LPVOID *) buffer,   THREAD_PRIORITY_NORMAL) ;
	}	
	
	static UINT showMessage(LPVOID lParam)  
	{  
		if(lParam == NULL)
			AfxEndThread(NULL);
		
		char *pStr = (char *) lParam;
		AfxMessageBox(pStr);  		
		delete pStr; // don't forget
		
		return TRUE;
	}  
};

class CMCPforNTApp : public CWinAppEx
{
private:
	// hold reference to subscription objects
	DipSubscription **sub;

	// DIP object
	DipFactory *dip;

	/**
	* handler for connect/disconnect/data reception events
	* Nested class
	* */
	class GeneralDataListener:public DipSubscriptionListener
	{
	private:
		// allow us to access subscription objects
		CMCPforNTApp* client;

	public:
		GeneralDataListener(CMCPforNTApp *c):client(c){};

		/**
		* handle changes to subscribed to publications
		* Simply prints the contents of the received data.
		* @param subscription - the subsciption to the publications thats changed.
		* @param message - object containing publication data
		* */
		void handleMessage(DipSubscription *subscription, DipData &message)
		{
			// Isolde general
			if(strcmp(subscription->getTopicName(),client->DipHT)==0)
			{
				client->SetIsoHighvolt( message.extractDouble(client->DipHTValue) );
			}
			if(strcmp(subscription->getTopicName(),client->DipPC)==0)
			{
				client->SetIsoProtons( message.extractDouble(client->DipPCValue) );
			}
			// GPS new style
			if(strcmp(subscription->getTopicName(),"dip/acc/ISO/GPS.MAG70/UserSettings")==0)
			{
				client->m_iso_gps_highvolt = message.extractDouble("highVoltage");
				client->m_iso_gps_mfactor = message.extractDouble("massFactor");
				client->m_iso_gps_aqn = message.extractDouble("fieldAqn");
			}
			// GPS old style
			if(strcmp(subscription->getTopicName(),"dip/acc/ISO/GPS.MAG70/HIGHVOLT")==0)
			{
				client->m_iso_gps_highvolt = message.extractDouble("value");
			}
			if(strcmp(subscription->getTopicName(),"dip/acc/ISO/GPS.MAG70/MFACTOR")==0)
			{
				client->m_iso_gps_mfactor = message.extractDouble("value");
			}
			if(strcmp(subscription->getTopicName(),"dip/acc/ISO/GPS.MAG70/AQN")==0)
			{
				client->m_iso_gps_aqn = message.extractDouble("value");
			}
			//---<<< HRS.MAG90 >>>---//
			if(strcmp(subscription->getTopicName(),"dip/acc/ISO/HRS.MAG90/UserSettings")==0)
			{
				client->m_iso_hrs_mag90_highvolt = message.extractDouble("highVoltage");
				client->m_iso_hrs_mag90_mfactor = message.extractDouble("massFactor");
				client->m_iso_hrs_mag90_aqn = message.extractDouble("fieldAqn");
			}
			//---<<< HRS.MAG60 >>>---//
			if(strcmp(subscription->getTopicName(),"dip/acc/ISO/HRS.MAG60/UserSettings")==0)
			{
				client->m_iso_hrs_mag60_highvolt = message.extractDouble("highVoltage");
				client->m_iso_hrs_mag60_mfactor = message.extractDouble("massFactor");
				client->m_iso_hrs_mag60_aqn = message.extractDouble("fieldAqn");
			}
		}


		/**
		* called when a publication subscribed to is available.
		* @param arg0 - the subsctiption who's publication is available.
		* */
		void connected(DipSubscription *arg0)
		{
			TRACE1("Publication source %s available\n", arg0->getTopicName());
		}

		/**
		* called when a publication subscribed to is unavailable.
		* @param arg0 - the subsctiption who's publication is unavailable.
		* @param arg1 - string providing more information about why the publication is unavailable.
		* */
		void disconnected(DipSubscription *arg0, char *arg1)
		{
			TRACE1("Publication source %s unavailable\n", arg0->getTopicName());
		}

		void handleException(DipSubscription* subscription, DipException& ex)
		{
			TRACE2("Subs %s has error %s\n", subscription->getTopicName(), ex.what());
		}

	};

	//A handle on the DIP Data recipient.
	GeneralDataListener *handler;

public:
	CString compile_time;
	CString compile_date;

	double m_iso_highvolt;
	double m_iso_protons;

	double m_iso_gps_highvolt;
	double m_iso_gps_mfactor;
	double m_iso_gps_aqn;

	double m_iso_hrs_mag90_highvolt;
	double m_iso_hrs_mag90_mfactor;
	double m_iso_hrs_mag90_aqn;
	double m_iso_hrs_mag60_highvolt;
	double m_iso_hrs_mag60_mfactor;
	double m_iso_hrs_mag60_aqn;

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
	int		SICLReaderBit;
	int		SICLStepBit;
	USHORT	PremaDelay;
	USHORT	PremaPostDelay;
	USHORT	FlukeDelay;
	USHORT	MassDelay;
	USHORT	GPIBDelay;
	CString DipHT, DipHTValue;
	CString DipPC, DipPCValue;

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
	void SetIsoHighvolt( double value );
	void SetIsoProtons( double value );
	double GetIsoHighvolt();
	double GetIsoProtons();
	double GetMassFactor();
	double GetField();

	bool GetProductAndVersion(CString & strProductName, CString & strProductVersion);
	void GetCompileTimeAndDate(CString & strCompileTime, CString & strCompileDate);
	double GetMass(bool useGps = true);
	int SetMass(double ToMassNo, bool useGps = true);
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
