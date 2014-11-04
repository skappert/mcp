// MCP for NTDoc.cpp : implementation of the CMCPforNTDoc class
//

#include "stdafx.h"
#include "MCP for NT.h"

#include "MCP for NTDoc.h"
#include "ActionObjects.h"
#include "FileIO.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMCPforNTDoc

IMPLEMENT_DYNCREATE(CMCPforNTDoc, CDocument)

BEGIN_MESSAGE_MAP(CMCPforNTDoc, CDocument)
	//{{AFX_MSG_MAP(CMCPforNTDoc)
	ON_COMMAND(ID_FILE_SAVEOLD, OnFileSaveold)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_SEND_MAIL, OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, OnUpdateFileSendMail)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMCPforNTDoc construction/destruction

CMCPforNTDoc::CMCPforNTDoc()
{
	// TODO: add one-time construction code here
	MeasurementRunning  = FALSE;
	TrackRunning		= FALSE;
	DataValid			= TRUE;
	SaveActualVersion	= TRUE;
	NoOfMaximumCycles	= 1000;
}

CMCPforNTDoc::~CMCPforNTDoc()
{
	POSITION pos;
	CView* pView;
	if (!pChildWin.IsEmpty())
	{
		pos=pChildWin.GetHeadPosition();
		do
		{
			pView = (CView*)pChildWin.GetNext(pos);
			pView->GetParent()->DestroyWindow();
		}while (pos!=NULL);
	}
	

	if(!ActionObjList.IsEmpty())
	{
		ActionObject* pActionObj = (ActionObject*)ActionObjList.GetHead();
		pos = ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)ActionObjList.GetNext(pos);
			if (pActionObj!=NULL)
			{
				delete pActionObj;
			}
		}
		while (pos!=NULL);
		ActionObjList.RemoveAll();
	}
}

BOOL CMCPforNTDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	TheElement = "???";
	WaveNumber = 0;
	NoOfCycles = 1;
	Version = 21;
	SaveTime = 0;

	
	TrackObject* pAction1			= new TrackObject;
	pAction1->DoString				= "normal";
	pAction1->pTrack				= pAction1;
	pAction1->MyPosition			= 1;
	pAction1->pDocument				= this;
	ActionObjList.AddTail(pAction1);

	TriggerObj* pAction2			= new TriggerObj;
	pAction2->DoString				= "normal";
	pAction2->pTrack				= pAction1;
	pAction2->pDocument				= this;
	ActionObjList.AddTail(pAction2);
	

	PM_SpectrumObj* pAction3		= new PM_SpectrumObj;
	pAction3->DoString				= "normal";
	pAction3->pTrack				= pAction1;
	pAction3->SubRegNo				= pAction3->pTrack->ScalerCounter++;
	pAction3->pDocument				= this;
	pAction3->SCALER				= TRUE;
	pAction3->ARITHSCALER			= FALSE;
	ActionObjList.AddTail(pAction3);

	UpdateAllViews(NULL);

	return TRUE;
}

bool CMCPforNTDoc::OnSave(CString SaveString)
{
	if(!IsModified() && GetTitle() == SaveString)
	{
		Beep(1000,100);
		return TRUE;
	}

	CFile File;
	if(File.Open(SaveString,CFile::modeRead))
	{
		AfxMessageBox("File exists, not saving !",MB_OK,0);
		File.Close();
		return TRUE;
	}
	else
	{ 
		if(!OnSaveDocument(SaveString))
		{
			AfxMessageBox("Document not saved !",MB_OK,0);
		}
		else 
		{
			SetTitle(SaveString);
			SetPathName(SaveString);
		}
	}

	return FALSE;
}

CString	CMCPforNTDoc::GetInfo(void)
{
	
	CString		answer;
	CString		LastSave;

	if(SaveTime!=0) LastSave = SaveTime.Format( "saved %x, %H:%M");
	else LastSave = "Not yet saved";
	if(IsModified())LastSave = LastSave + "*modified";

	answer.Format("%s ; %u  cy ; wave = %g ; ",TheElement,NoOfCycles,WaveNumber);
	answer = answer + LastSave; 
	return answer;
}

/////////////////////////////////////////////////////////////////////////////
// CMCPforNTDoc serialization

void CMCPforNTDoc::SaveActionObj(CArchive& ar)
{
	POSITION pos;
	POSITION pos2;
	if(!ActionObjList.IsEmpty())
	{
		ActionObject* pActionObj = (ActionObject*)ActionObjList.GetHead();
		pos = ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)ActionObjList.GetNext(pos);
			if (pActionObj!=NULL)
			{
				pActionObj->Save(ar);
				if(pos!=NULL&&pActionObj->Name.Find(_TrackObj)==-1)
				{
					pos2 = pos;
					ActionObject* pActionObj2 = (ActionObject*)ActionObjList.GetNext(pos2);
					if(pActionObj2->Name.Find(_TrackObj)==-1) WriteSeparator(ar);
				}
			}
		}
		while (pos!=NULL);
	}
}

void CMCPforNTDoc::DispatchActionObj(CArchive& ar)
{
	CString TheObject;
	BOOL    CouldDispatch;

	CMCPforNTApp* pApp	= (CMCPforNTApp*)AfxGetApp();

	do
	{
	TrackObject* pActionObj = new TrackObject;
	pActionObj->pDocument   = this;
	pActionObj->Name        = _TrackObj;
	pActionObj->ShortName   = _TrackObj;
	++TrackNum;
	pActionObj->MyPosition	= TrackNum;
	pActualTrack			= pActionObj;
	Control					= ReadStructBegin(ar);
	pActionObj->Isotope     = ReadString(ar);
	Control					= ReadSeparator(ar);
	pActionObj->NoOfScans   = (int)ReadFloat(ar);
	Control					= ReadSeparator(ar);
	pActionObj->RealScans   = (int)ReadFloat(ar);
	Control					= ReadSeparator(ar);
	pActionObj->Channels	= (int)ReadFloat(ar);
	Control					= ReadSeparator(ar);
	pActionObj->Resolution2 = (int)ReadFloat(ar);
	Control					= ReadSeparator(ar);
	if (Version > 1)
	{
		pActionObj->RepeatSteps = (int)ReadFloat(ar);
		Control					= ReadSeparator(ar);
	}
	if (Version > 15)
	{
		pActionObj->UpDownSweep = (int)ReadFloat(ar);
		Control					= ReadSeparator(ar);
	}

	Control		  = ReadStructBegin(ar);

	ActionObjList.AddTail(pActionObj);
	do
	{
	
	Control		  = ReadNameBegin(ar);
	TheObject     = ReadString(ar);
	Control		  = ReadNameEnd(ar);
	CouldDispatch = FALSE;
	//AfxMessageBox("Next dispatching "+TheObject,MB_OK,0);

	if(TheObject==_HP_VoltageSweepObj)
	{
		ActionObject* pActionObj = new HP_VoltageSweepObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;
	}
	if(TheObject==_Line_VoltageSweepObj)
	{
		ActionObject* pActionObj = new Line_VoltageSweepObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;
	}
	if(TheObject==_HP8660B_FrequencySweepObj)
	{
		ActionObject* pActionObj = new HP8660B_FrequencySweepObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;
	}
	if(TheObject==_HP3325B_FrequencySweepObj)
	{
		ActionObject* pActionObj = new HP3325B_FrequencySweepObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;
	}
	if(TheObject==_HP3325B2_FrequencySweepObj)
	{
		ActionObject* pActionObj = new HP3325B2_FrequencySweepObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_RS_FrequencySweepObj||TheObject==_RS2_FrequencySweepObj)
	{
		ActionObject* pActionObj	= new RS_FrequencySweepObj;
		pActionObj->pDocument		= this;
		pActionObj->pTrack			= pActualTrack;
		pActionObj->pTrack->RS_FrequencyNum++;
		switch(pActionObj->pTrack->RS_FrequencyNum)
		{
		case 1:
			pActionObj->Gpib				= pApp->Rs1Gpib;
			break;
		case 2:
			pActionObj->Gpib				= pApp->Rs2Gpib;
			break;
		default:
			pActionObj->Gpib				= pApp->Rs1Gpib;
			break;
		}
		
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;
	}
	if(TheObject==_PM_SpectrumObj)
	{
		ActionObject* pActionObj = new PM_SpectrumObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_PremaVoltageObj)
	{
		ActionObject* pActionObj = new PremaVoltageObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_FlukeVoltageObj)
	{
		ActionObject* pActionObj = new FlukeVoltageObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_IsoldeNetVoltageObj)
	{
		ActionObject* pActionObj = new IsoldeNetVoltageObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_KepcoEichungVoltageObj)
	{
		ActionObject* pActionObj = new KepcoEichungVoltageObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_ComputerDelayObj)
	{
		ActionObject* pActionObj = new ComputerDelayObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_TimerDelayObj)
	{
		ActionObject* pActionObj = new TimerDelayObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_StepTimerDelayObj)
	{
		ActionObject* pActionObj = new StepTimerDelayObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_FastTimerDelayObj)
	{
		ActionObject* pActionObj = new FastTimerDelayObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_Keithley_VoltageMeasObj)
	{
		ActionObject* pActionObj = new Keithley_VoltageMeasObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_FlukeOsziObj)
	{
		ActionObject* pActionObj = new FlukeOsziObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_MassAndFlukeSwitchObj)
	{
		ActionObject* pActionObj = new MassAndFlukeSwitchObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_MassSwitchObj)
	{
		ActionObject* pActionObj = new MassSwitchObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}

	if(TheObject==_FlukeSwitchObj)
	{
		ActionObject* pActionObj = new FlukeSwitchObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}

	if(TheObject==_TriggerSweepObj)
	{
		ActionObject* pActionObj = new TriggerSweepObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_StepTriggerObj)
	{
		ActionObject* pActionObj = new StepTriggerObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_TriggerObj)
	{
		ActionObject* pActionObj = new TriggerObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_TriggerPartSweepObj)
	{
		ActionObject* pActionObj = new TriggerPartSweepObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_AlexMotorObj)
	{
		ActionObject* pActionObj = new AlexMotorObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_GPIBReaderObj)
	{
		ActionObject* pActionObj = new GPIBReaderObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_SiclReaderObj)
	{
		ActionObject* pActionObj = new SiclReaderObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_SiclStepObj)
	{
		ActionObject* pActionObj = new SiclStepObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_TempReaderObj)
	{
		ActionObject* pActionObj = new TempReaderObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_TempWriterObj)
	{
		ActionObject* pActionObj = new TempWriterObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_FieldReaderObj)
	{
		ActionObject* pActionObj = new FieldReaderObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_OutBitObj)
	{
		ActionObject* pActionObj = new OutBitObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_FieldWriterObj)
	{
		ActionObject* pActionObj = new FieldWriterObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_GPIBWriterObj)
	{
		ActionObject* pActionObj = new GPIBWriterObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_MultiFunctionObj)
	{
		ActionObject* pActionObj = new MultiFunctionObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_FDAC_VoltageSweepObj)
	{
		ActionObject* pActionObj = new FDAC_VoltageSweepObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_CamacLPObj)
	{
		ActionObject* pActionObj = new CamacLPObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_CCDCameraObj)
	{
		ActionObject* pActionObj = new CCDCameraObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}
	if(TheObject==_DummyObj)
	{
		ActionObject* pActionObj = new DummyObj;
		pActionObj->pDocument=this;
		pActionObj->pTrack = pActualTrack;
		pActionObj->Load(ar);
		ActionObjList.AddTail(pActionObj);
		CouldDispatch=TRUE;

	}

	if (!CouldDispatch)AfxMessageBox("Unknown Object "+TheObject,MB_OK,0);
	}
	while (ReadChar(ar)==',');
	ReadStructEnd(ar);
	}
	while (ReadChar(ar)==',');
}


void CMCPforNTDoc::Serialize(CArchive& ar)
{
	CFileStatus Status;
	CString TheObject;
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		CTime theTime;
		theTime=CTime::GetCurrentTime();
		SaveTime = theTime;
		CString time = theTime.Format( "%a %b %d %X %Y");

		if(SaveActualVersion)WriteString(ar,NewHeader);
		else WriteString(ar,CompatibleHeader);
		WriteLine(ar);
		WriteControlBegin(ar);
		if(SaveActualVersion)WriteFloat(ar,ActualVersion);
		else WriteFloat(ar,CompatibleVersion);
		WriteControlEnd(ar);
		WriteLine(ar);
 		WriteDataFileBegin(ar);
		WriteStructBegin(ar);
		WriteString(ar,time);
		WriteLine(ar);
		WriteSeparator(ar);
		WriteString(ar,TheElement);
		WriteSeparator(ar);
		WriteLine(ar);
		WriteFloat(ar,WaveNumber);
		WriteSeparator(ar);
		WriteFloat(ar,NoOfCycles);
		WriteSeparator(ar);
		WriteFloat(ar,DoMonitor);
		WriteSeparator(ar);
		WriteLine(ar);
		WriteStructBegin(ar);

		SaveActionObj(ar);

		WriteStructEnd(ar);
		WriteLine(ar);
		WriteStructEnd(ar);
		WriteStructEnd(ar);
		WriteLine(ar);
		WriteStructEnd(ar);
		WriteDataFileEnd(ar);

		SetModifiedFlag(FALSE);
		UpdateAllViews(NULL);
	}
	else
	{
		ar.GetFile()->GetStatus(Status);
		SaveTime = Status.m_mtime;

		TrackNum	= 0;
		Header		= ReadString(ar);
		Control		= ReadControlBegin(ar);
		Version		= (int)ReadFloat(ar);
		Control		= ReadControlEnd(ar);
 		Control		= ReadDataFileBegin(ar);
		Control		= ReadStructBegin(ar);
		Time		= ReadString(ar);
		Control		= ReadSeparator(ar);
		TheElement	= ReadString(ar);
		Control		= ReadSeparator(ar);
		WaveNumber	= (float)ReadFloat(ar);
		Control		= ReadSeparator(ar);
		NoOfCycles  = (int)ReadFloat(ar);
		Control		= ReadSeparator(ar);
		if (Version > 10)
		{
			DoMonitor	=(BOOL)ReadFloat(ar);
			Control		= ReadSeparator(ar);
		}
		Control		= ReadStructBegin(ar);
		
		DispatchActionObj(ar);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMCPforNTDoc diagnostics

#ifdef _DEBUG
void CMCPforNTDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMCPforNTDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMCPforNTDoc commands

void CMCPforNTDoc::OnFileSaveold() 
{
	// TODO: Add your command handler code here
	SaveActualVersion = FALSE;
	OnFileSaveAs();
	SaveActualVersion = TRUE;
}

void	CMCPforNTDoc::UpdateAllData(int track)
{
	POSITION	pos;
	CString		TheObject;

	if(!ActionObjList.IsEmpty())
	{
		ActionObject* pActionObj = (ActionObject*)ActionObjList.GetHead();
		pos = ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)ActionObjList.GetNext(pos);
			if (pActionObj!=NULL&&pActionObj->pTrack->MyPosition == track)
			{
				pActionObj->UpdateData();
			}
		}
		while (pos!=NULL);
	}
}

void CMCPforNTDoc::SetNewCenter(USHORT track,USHORT step)
{
	POSITION	pos;
	CString		TheObject;

	if(!ActionObjList.IsEmpty())
	{
		ActionObject* pActionObj = (ActionObject*)ActionObjList.GetHead();
		pos = ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)ActionObjList.GetNext(pos);
			if (pActionObj!=NULL&&pActionObj->pTrack->MyPosition == track)
			{
				pActionObj->SetCenter(step);
			}
		}
		while (pos!=NULL);
		UpdateAllViews(NULL);
	}
}

void CMCPforNTDoc::SetNewMinMax(USHORT track,double minstep, double maxstep)
{
	POSITION	pos;
	CString		TheObject;

	CString minmax;

	if(!ActionObjList.IsEmpty())
	{
		ActionObject* pActionObj = (ActionObject*)ActionObjList.GetHead();
		pos = ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)ActionObjList.GetNext(pos);
			if (pActionObj!=NULL&&pActionObj->pTrack->MyPosition == track)
			{
				pActionObj->SetMinMax(minstep,maxstep);
			}
		}
		while (pos!=NULL);
		UpdateAllViews(NULL);
	}
}

	
