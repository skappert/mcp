// RPCAPI32.cpp : Defines the initialization routines for the DLL.

#include <StdAfx.h>		
#include "rpcapi32.h"
#include <afxsock.h>
#include "afxwin.h"
#include "errors.h"
#include "errsrv32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "errors.c"
#include "dbcsvapi.cpp"
//#include "dbcsv32.h"

typedef struct
{
	char Name[20];
	BYTE Node[6];
	BYTE Network[4];
	short AlarmCheck;
	char IP[17];
	short Prot;
	BOOL MHard;
	BOOL MDataTypes;
}RPCFecStruct;

typedef struct
{
	char EqpName[16];
	char Property[32];
	char StatName[20];
	char GlobalCom[20];
	unsigned long Weight;
}RPCModStruct;

typedef struct
{
	long TotalSize;
	long MessageVersion;
	long MessageType;
	char UserName[20];

	char ElemName[20];
	char EqpModName[16];
	long EqpModNo;
	char Property[32];
	char Cycle[20];
	long ArraySize;
	long DataSize;
	long DataFormat;
	long SubScrID;
	char RefreshEvent[20];
	long hWnd;
	long UpdatePort;
}RPCInfoStruct;

typedef struct
{
	long hWnd;
	char EqpModName[16];
	char Property[32];
	long ArraySize;
	long SubScrID;
	long CompCode;
	long DataFormat;
	long DataSize;
	long CycleNo;
	char User[20];
}EqpUpdateStruct;

struct IDListStruct
{
	HWND hWnd;
	char ElemName[20];
	char Property[32];
	char Cycle[20];
	long RefreshPeriod;
	long ArraySize;
	long SubScrID;
	long DataType;
	short FecIndex;
	time_t LastUpdate;
	struct IDListStruct *Next;
};

#define MESSAGE_VERSION		 19990305
#define DEFAULT_PACKET_SIZE  5000
#define FEC_PACKET_SIZE		 1024

#define APPLICATIONNAME "RPCAPI"
#define RPC_TIMEOUT       4

#define ERR_FATAL    -2
#define FEC_DIRECT_ACCESS 	30
//#define FEC_DELAYED_ACCESS	16
#define FEC_DELAY	60

#define UPDATE_MESSAGE WM_USER+1
#define CHECK_CHAR 13
#define tcp 2
#define REQUEST_PORT 		8100
#define INFO_PORT 			8101
#define UPDATE_PORT_OFFSET  8102
#define MAJOR_VERSION   1
#define MINOR_VERSION   1 
#define WSA_MAKEWORD(x,y)       ((y) * 256 + (x)) 

short RPCConnect(RPCInfoStruct *RPCInfo,WORD *connectionID,short *fecindex);
short NetSend(short connectionID,short size);
short RPCSend(RPCInfoStruct *RPCInfo,WORD connectionID,void *Data,short DataFormat,short DataType,short fecindex);
short RPCReceive(RPCInfoStruct *RPCInfo,WORD connectionID,void *Data,long DataSize,short DataFormat,short fecindex);
int InitRPCAPI();             
int InitDB();
void CodeFail(char *str);
short ConvertData(short cc,char *EqpModName,char *Property,void *sdata,short sformat,void *ddata,short dformat,long ddatasize,short Size,char *ErrStr,short ErrStrSize);
short GetFormatSize(short format);
short CreateListener();
HWND CreateLocalWnd();
void SendWatch(u_long addr);
short KnobCommandFunc(HWND hwnd,const char *name,char *ffunc);
LRESULT CALLBACK LocalWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL InitDone,DoRecreate;
char Path[200],ErrSrvCommand[200];
BOOL CaseSens;
char *FecHotlinkActive,*FecAccessCount;
time_t *FecLastAccess;
char *sendBuffer;
short NetStat;
void ListenUDP();
SOCKET LSck,SSck;
WORD VersionReqd;
LPWSADATA lpmyWSAData;
BOOL TCPFunc(void);
struct sockaddr_in req_sck_name;
u_short UpdatePort;
char ErrStr[BUFFER_SIZE+1],UserName[BUFFER_SIZE+1],ErrName[100],ErrProp[100];
BOOL ListenActive,DoListen;
HINSTANCE hInst;
HWND hRPCWnd;
char *UpdateMsgBuf;
u_long LocalAddr;
long hActualWnd;
short PacketSize;
BOOL HotlinkCheckBusy=FALSE;
long IDListCheckCount=0;

struct IDListStruct *IDHead;

RPCModStruct *RPCMod;
RPCDataStruct *RPCData;
RPCFecStruct  *RPCFec;

short RPCNumRecsRead=0,RPCNumFecsRead=0,RPCNumEqpsRead=0;

DataBaseElemsStruct RPCDataElems[]={
	"",sizeof(RPCDataStruct),0L,
	"FECNAME",STR,0L,
	"NAME",STR,0L,
	"EQPMODULE",STR,0L,
	"EQPNUMBER",INT,0L,
	"TYPE",STR,0L,
	"UNIT",STR,0L,
	"INCREMENT",STR,0L,
	"CONTROL32",STR,0L,
	""};

DataBaseElemsStruct RPCModElems[]={
	"",sizeof(RPCModStruct),0L,
	"EQPMODULE",STR,0L,
	"PROPERTY",STR,0L,
	"STATUS",STR,0L,
	"COMMAND",STR,0L,
	"WEIGHT",INT,0L,
	""};

DataBaseElemsStruct RPCFecElems[]={
	"",sizeof(RPCFecStruct),0L,
	"FECNAME",STR,0L,
	"FECNODE",HEX,0L,
	"FECNETWORK",HEX,0L,
	"ALARMCHECK",INT,0L,
	"IPADDR",STR,0L,
	"PROTOCOL",INT,0L,
	"MHARD",INT,0L,
	"MDATATYPES",INT,0L,
	""};

short FAR PASCAL GetRPCLastError(char *ErrMsg,short ErrStrLen)
{
	short l;
	char *str;

	l=lstrlen(ErrStr)+lstrlen(ErrName)+lstrlen(ErrProp)+4;
	str=(char*)malloc(l+2);
	sprintf(str,"%s\\%s : %s",ErrName,ErrProp,ErrStr);

	if (l>=ErrStrLen) l=ErrStrLen-1;
	memcpy(ErrMsg,str,l);
	ErrMsg[l]=0;

	free(str);
	return l;
}
short FAR PASCAL SyncRPC(const char *Name,const char *Property,const char *Cycle,short ArraySize,void *Data,long DataSize,short DataFormat,short DataType)
{   
	RPCInfoStruct RPCInfo;
	short i,cc,retry,fecindex;
	WORD connectionID;
	char buf[200];

	ErrStr[0]=0;ErrName[0]=0;ErrProp[0]=0;

	if (lstrlen(Name)>sizeof(RPCInfo.ElemName)) {cc=non_existent_elem;goto out;}
	if (lstrlen(Property)>sizeof(RPCInfo.Property)) {cc=illegal_property;goto out;}

	if (DataType && DataType!=CF_TEXT && DataType!=CF_DOUBLE && DataType!=CF_FLOAT && DataType!=CF_BYTE && DataType!=CF_SHORT && DataType!=CF_LONG && DataType!=CF_BI && DataType!=CF_AI && DataType!=CF_BR && DataType!=CF_AR) {cc=illegal_datatype;goto out;}
	
	memset(&RPCInfo,0,sizeof(RPCInfoStruct));

	strcpy(RPCInfo.ElemName,Name);
	strcpy(RPCInfo.Property,Property);
	if (Cycle==NULL) RPCInfo.Cycle[0]=0;
	else strcpy(RPCInfo.Cycle,(char *)Cycle);
	RPCInfo.ArraySize=ArraySize;

	RPCInfo.MessageType=1;
	
	fecindex=-1;
	cc=RPCConnect(&RPCInfo,&connectionID,&fecindex);
	if (cc) goto out;

	retry=2;
	do
	{
		cc=RPCSend(&RPCInfo,connectionID,Data,DataFormat,DataType,fecindex);
		if (cc) goto out;

		cc=RPCReceive(&RPCInfo,connectionID,Data,DataSize,DataFormat,fecindex);
		retry--;
	}while (cc==link_not_open && retry);

out:
	if (NetStat) 
	{
		closesocket(NetStat);
		NetStat=0;
	}
	if (cc>0)
	{
		i=lstrlen(erlst[cc]);
		if (i>=BUFFER_SIZE) i=BUFFER_SIZE-1;
	 	memcpy(ErrStr,erlst[cc],i);
		ErrStr[i]=0;
	}
	if (lstrlen(ErrStr)) 
	{
		wsprintf(buf,"SyncRPC %s\\%s\\%s\\S%d\\F%d -> %s",Name,Property,RPCInfo.Cycle,ArraySize,DataFormat,ErrStr);
		RegisterError(APPLICATIONNAME,-1,buf,FALSE);
		strcpy(ErrName,Name);strcpy(ErrProp,Property);
	}
	return cc;
}     
short FAR PASCAL CreateRPCHotlink(HWND hWnd,const char *Name,const char *Property,const char *Cycle,long RefreshPeriod,short ArraySize,short SubScrID,short DataType)
{    
	RPCInfoStruct RPCInfo;
	short i,cc,retry,fecindex;
	WORD connectionID;
	char buf[200];
	struct IDListStruct *IDList;

	ErrStr[0]=0;ErrName[0]=0;ErrProp[0]=0;

	if (!hWnd) {cc=invalid_data;goto out;}
	if (lstrlen(Name)>sizeof(RPCInfo.ElemName)) {cc=non_existent_elem;goto out;}
	if (lstrlen(Property)>sizeof(RPCInfo.Property)) {cc=illegal_property;goto out;}
	if (ArraySize<=0) {cc=illegal_read_write;goto out;}

	if (DataType && DataType!=CF_TEXT && DataType!=CF_DOUBLE && DataType!=CF_FLOAT && DataType!=CF_BYTE && DataType!=CF_SHORT && DataType!=CF_LONG && DataType!=CF_BI && DataType!=CF_AI && DataType!=CF_BR && DataType!=CF_AR) {cc=illegal_datatype;goto out;}

	if (!DoRecreate)
	{
		IDList=IDHead;
		while (IDList)
		{
			if (IDList->hWnd==hWnd && IDList->SubScrID==SubScrID) {cc=invalid_data;goto out;}
			if (IDList) IDList=IDList->Next;
		}
	}
	
	memset(&RPCInfo,0,sizeof(RPCInfoStruct));

	strcpy(RPCInfo.ElemName,Name);
	strcpy(RPCInfo.Property,Property);
	if (Cycle==NULL) RPCInfo.Cycle[0]=0;
	else strcpy(RPCInfo.Cycle,(char *)Cycle);
	RPCInfo.ArraySize=ArraySize;

	RPCInfo.hWnd=(long)hWnd;
	sprintf(RPCInfo.RefreshEvent,"%d",RefreshPeriod);
	RPCInfo.SubScrID=SubScrID;

	RPCInfo.MessageType=2;
	
	fecindex=-1;
	cc=RPCConnect(&RPCInfo,&connectionID,&fecindex);
	if (cc) goto out;

	retry=2;
	do
	{
		cc=RPCSend(&RPCInfo,connectionID,NULL,0,DataType,fecindex);
		if (cc) goto out;

		cc=RPCReceive(&RPCInfo,connectionID,NULL,0,0,fecindex);
		retry--;
	}while (cc==link_not_open && retry);
	
out:
	if (NetStat) 
	{
		closesocket(NetStat);
		NetStat=0;
	}
	if (!DoRecreate && !cc)
	{
		IDList=(IDListStruct *)malloc(sizeof(struct IDListStruct));
		if (IDList==NULL) cc=out_of_memory;
		else
		{
			IDList->hWnd=hWnd;
			strcpy(IDList->ElemName,Name);
			strcpy(IDList->Property,Property);
			strcpy(IDList->Cycle,RPCInfo.Cycle);
			IDList->RefreshPeriod=RefreshPeriod;
			IDList->ArraySize=ArraySize;
			IDList->SubScrID=SubScrID;
			IDList->FecIndex=fecindex;
			IDList->Next=IDHead;
			IDList->LastUpdate=time(NULL);
			IDList->DataType=DataType;
			IDHead=IDList;
		}
	}
	if (cc>0)
	{
		i=lstrlen(erlst[cc]);
		if (i>=BUFFER_SIZE) i=BUFFER_SIZE-1;
	 	memcpy(ErrStr,erlst[cc],i);
		ErrStr[i]=0;
	}
	if (lstrlen(ErrStr)) 
	{
		wsprintf(buf,"CreateRPCHotlink %s\\%s\\%s\\S%d\\ID%d -> %s",Name,Property,RPCInfo.Cycle,ArraySize,SubScrID,ErrStr);
		RegisterError(APPLICATIONNAME,-1,buf,FALSE);
		strcpy(ErrName,Name);strcpy(ErrProp,Property);
	}
	
	return cc;
}            
short FAR PASCAL RemoveRPCHotlink(HWND hWnd,short SubScrID)
{   
	RPCInfoStruct RPCInfo;
	short i,cc=0,retry,fecindex;
	WORD connectionID;
	char buf[200];
	struct IDListStruct *IDList,*IDPreced;

	ErrStr[0]=0;ErrName[0]=0;ErrProp[0]=0;

	memset(&RPCInfo,0,sizeof(RPCInfoStruct));

	RPCInfo.hWnd=(long)hWnd;
	RPCInfo.SubScrID=SubScrID;

	RPCInfo.MessageType=3;
	
	fecindex=-1;

 	IDList=IDHead;
	while (IDList)
	{
	 	if ((IDList->hWnd==hWnd || hWnd==(HWND)-1) && (IDList->SubScrID==SubScrID || SubScrID==-1)) 
	 	{
	 		fecindex=IDList->FecIndex;
			break;
		}
		if (IDList) IDList=IDList->Next;
	}
	if (fecindex==-1) goto out;

	cc=RPCConnect(&RPCInfo,&connectionID,&fecindex);
	if (cc) goto out;

	retry=2;
	do
	{
		cc=RPCSend(&RPCInfo,connectionID,NULL,0,0,fecindex);
		if (cc) goto out;

		cc=RPCReceive(&RPCInfo,connectionID,NULL,0,0,fecindex);
		retry--;
	}while (cc==link_not_open && retry);
	
out:
	if (NetStat) 
	{
		closesocket(NetStat);
		NetStat=0;
	}
	if (cc>0)
	{
		i=lstrlen(erlst[cc]);
		if (i>=BUFFER_SIZE) i=BUFFER_SIZE-1;
	 	memcpy(ErrStr,erlst[cc],i);
		ErrStr[i]=0;
	}
	if (lstrlen(ErrStr)) 
	{
		wsprintf(buf,"RemoveRPCHotlink %ld\\%d -> %s",(u_long)hWnd,SubScrID,ErrStr);
		RegisterError(APPLICATIONNAME,-1,buf,FALSE);
		sprintf(ErrName,"ID=%d",SubScrID);
	}
loop:
	if (!DoRecreate)
	{
		IDList=IDHead;IDPreced=NULL;
		while (IDList)
		{
	 		if (IDList->hWnd && (IDList->hWnd==hWnd || hWnd==(HWND)-1) && (IDList->SubScrID==SubScrID || SubScrID==-1)) 
	 		{
				if (IDPreced!=NULL) IDPreced->Next=IDList->Next;
				else IDHead=IDList->Next;
				free(IDList);
				goto loop;
			}
			if (IDList) 
			{
				IDPreced=IDList;
				IDList=IDList->Next;
			}
		}
	}
	return cc;
}
short FAR PASCAL GetRPCHotlinkIDs(short *SubScrIDs)
{     
	EqpUpdateStruct *EqpUpdate;
	long eqpsize,dataoffset,eqp,count;
	
	if (!ListenActive || !UpdateMsgBuf) 
	{
		strcpy(ErrStr,"No data available");
		return 0;
	}

	ErrStr[0]=0;ErrName[0]=0;ErrProp[0]=0;
	eqpsize=*(long *)UpdateMsgBuf;
	count=0;

	for (dataoffset=4,eqp=0;eqp<eqpsize;eqp++)
	{
		EqpUpdate=(EqpUpdateStruct *)&UpdateMsgBuf[dataoffset];

		if (EqpUpdate->hWnd==hActualWnd)
		{
			SubScrIDs[count++]=(short)EqpUpdate->SubScrID;
		}		
		dataoffset=dataoffset+sizeof(EqpUpdateStruct)+EqpUpdate->DataSize;
	}
	return (short)count;
}
short FAR PASCAL GetRPCHotlinkData(short SubScrID,void *Data,short DataSize,short DataFormat,short *CycleNo,char *User)
{
	EqpUpdateStruct *EqpUpdate;
	char buf[200];
	long eqpsize,dataoffset,eqp;
	
	ErrStr[0]=0;ErrName[0]=0;ErrProp[0]=0;
	if (!ListenActive || !UpdateMsgBuf) 
	{
		strcpy(ErrStr,"No data available");
		return -1;
	}
	
	eqpsize=*(long *)UpdateMsgBuf;

	for (dataoffset=4,eqp=0;eqp<eqpsize;eqp++)
	{
		EqpUpdate=(EqpUpdateStruct *)&UpdateMsgBuf[dataoffset];
		if (EqpUpdate->SubScrID==SubScrID && EqpUpdate->hWnd==hActualWnd)
		{
			ConvertData((short)EqpUpdate->CompCode,EqpUpdate->EqpModName,EqpUpdate->Property,&UpdateMsgBuf[dataoffset+sizeof(EqpUpdateStruct)],(short)EqpUpdate->DataFormat,
						Data,DataFormat,DataSize,(short)EqpUpdate->ArraySize,ErrStr,BUFFER_SIZE);

			if (lstrlen(ErrStr)) 
			{
				wsprintf(buf,"RPCHotlinkData %s\\%s -> %s",EqpUpdate->EqpModName,EqpUpdate->Property,ErrStr);
				RegisterError(APPLICATIONNAME,-1,buf,FALSE);
			}
			if (CycleNo!=NULL) *CycleNo=(short)EqpUpdate->CycleNo;
			if (User!=NULL) strcpy(User,EqpUpdate->User);
			return (short)EqpUpdate->CompCode;
		}
		dataoffset=dataoffset+sizeof(EqpUpdateStruct)+EqpUpdate->DataSize;
	}	

	strcpy(ErrStr,"SubScrID not registered");
	return -1;	
}
short FAR PASCAL KnobAttach(HWND hwnd,const char *name)
{
	return (KnobCommandFunc(hwnd,name,"ATTACH"));
}
short FAR PASCAL KnobRemove(HWND hwnd,const char *name)
{
	return (KnobCommandFunc(hwnd,name,"REMOVE"));
}
short KnobCommandFunc(HWND hwnd,const char *name,char *func)
{
	short cc,dataindex;
	char upname[50],buf[200];
	WORD i;
	SOCKET sck;
	char command[MAX_MESSAGE_SIZE];
	struct sockaddr_in sck_name;
	short s,count;
	u_long arg;
	HWND hclientWnd;
	
	cc=0;ErrStr[0]=0;ErrName[0]=0;ErrProp[0]=0;

	cc=InitRPCAPI();
	if (cc) goto out;
	
	for (i=0;i<strlen(name);i++) 
	{
		upname[i]=toupper(name[i]);
	}
	upname[i]=0;

	count=sizeof(RPCDataStruct);
	if (func && strlen(func) && strcmp(func,"REMOVE"))
	{
		dataindex=SearchDataBase(RPCDataElems,upname,"NAME");
		if (dataindex<0) {cc=non_existent;goto out;}
		memcpy(command,&RPCData[dataindex],count);
	}
	else memset(command,0,count);

	hclientWnd=hwnd;
	memcpy(&command[count],&hclientWnd,sizeof(HWND));count=count+sizeof(HWND);
	strcpy(&command[count],func);count=count+strlen(func);

	sck=socket(AF_INET,SOCK_DGRAM,0);
	if (sck==INVALID_SOCKET) return no_link;

	arg=1L;
	if (ioctlsocket(sck,FIONBIO,(u_long *)&arg)) 
	{
		closesocket(sck);
		return no_link;
	}
		                            
	memset(&sck_name,0,sizeof(sck_name));
	sck_name.sin_addr.s_addr=LocalAddr;
	sck_name.sin_family=AF_INET;
	sck_name.sin_port=htons(KNOBSRV_PORT);

	s=sendto(sck,(char *)command,count,0,(struct sockaddr *)&sck_name,sizeof(sck_name));
    if (s<0) 
	{
		closesocket(sck);
		return net_write_error;
	}
	closesocket(sck);	
out:	
	if (cc>0) strcpy(ErrStr,erlst[cc]);

	if (lstrlen(ErrStr)) 
	{
		wsprintf(buf,"Knob %s\\%s -> %s",name,func,ErrStr);
		RegisterError(APPLICATIONNAME,-1,buf,FALSE);
		strcpy(ErrName,name);strcpy(ErrProp,func);
	}
	return cc;
}
int InitDB()
{
	short cc,i,size;
	char serverName[100],*ptr;
	DWORD s,vType;
	char text[100];
	CDaoDatabase eqpDB;
	CDaoRecordset *recSet;
	COleVariant olevar;
	VARIANT va;
	HKEY hKey=0;

	strcpy(ErrSrvCommand,"Z:\\P32\\CONTROL\\ERRSRV32");
	CaseSens=0;DoRecreate=FALSE;
	strcpy(Path,"Z:\\P32\\CONTROL\\DEFLTDB");

	RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Control System\\General",0,KEY_READ,&hKey);
	if (hKey)
	{
		s=sizeof(ErrSrvCommand);
		RegQueryValueEx(hKey,"ErrorServerPath",NULL,&vType,(LPBYTE)ErrSrvCommand,&s);

		s=sizeof(text);
		RegQueryValueEx(hKey,"AutoUppercase",NULL,&vType,(LPBYTE)text,&s);
		CaseSens=atoi(text);

		s=sizeof(Path);
		RegQueryValueEx(hKey,"EquipmentDatabasePath",NULL,&vType,(LPBYTE)Path,&s);

		RegCloseKey(hKey);
	}
	strcat(Path,"\\DBRT\\ControlDB.mdb");

	ptr=strstr(Path,"\\\\");
	if (ptr==Path)
	{
		strcpy(serverName,&Path[2]);
		ptr=strchr(serverName,'\\');
		if (ptr) *ptr=0;
		cc=(short)WNetAddConnection(serverName,NULL,NULL);
	}

	RPCMod=NULL;
	RPCFec=NULL;
	RPCData=NULL;

	RPCNumEqpsRead=0;
	RPCNumFecsRead=0;
	RPCNumRecsRead=0;
	VariantInit(&va);

	try
	{
		eqpDB.Open((LPCTSTR)Path,TRUE,TRUE);
	}
	catch (CDaoException* e)
	{
		return database_not_loaded;
	}

	if (!eqpDB.IsOpen()) return database_not_loaded;

	recSet=new CDaoRecordset(&eqpDB);

	recSet->Open(dbOpenSnapshot,(LPCTSTR)"SELECT BaseClassPropSpec.ClassName, BaseClassPropSpec.PropertyName, StatusCodes.Weight, StatusCodes.Status, StatusCodes.Command FROM BaseClassPropSpec INNER JOIN StatusCodes ON BaseClassPropSpec.BaseProID = StatusCodes.BaseProID WHERE (((BaseClassPropSpec.Used)=1)) ORDER BY BaseClassPropSpec.ClassName, BaseClassPropSpec.PropertyName, StatusCodes.Weight");

	i=0;
	if (!recSet->IsEOF())
	{
		recSet->MoveLast();
		size=(short)recSet->GetRecordCount();
		RPCMod=(RPCModStruct *)GlobalAlloc(GPTR,sizeof(RPCModStruct)*size);
		if (RPCMod==NULL) return out_of_memory;
		RPCModElems[1].Address=(long)RPCMod[0].EqpName;
		RPCModElems[2].Address=(long)RPCMod[0].Property;
		RPCModElems[3].Address=(long)RPCMod[0].StatName;
		RPCModElems[4].Address=(long)RPCMod[0].GlobalCom;
		RPCModElems[5].Address=(long)&RPCMod[0].Weight;
			
		recSet->MoveFirst();
		while (!recSet->IsEOF())
		{
			recSet->GetFieldValue(0,olevar);
			if (olevar.vt!=VT_NULL) 
			{
				strcpy(RPCMod[i].EqpName,(char*)olevar.pbVal);

				recSet->GetFieldValue(1,olevar);
				if (olevar.vt!=VT_NULL) strcpy(RPCMod[i].Property,(char*)olevar.pbVal);

				recSet->GetFieldValue(2,olevar);
				if (olevar.vt!=VT_NULL) 
				{
					if (VariantChangeType(&va,&olevar,0,VT_I4)) return database_not_loaded;
					RPCMod[i].Weight=va.lVal;
				}

				recSet->GetFieldValue(3,olevar);
				if (olevar.vt!=VT_NULL) strcpy(RPCMod[i].StatName,(char*)olevar.pbVal);
				
				recSet->GetFieldValue(4,olevar);
				if (olevar.vt!=VT_NULL) strcpy(RPCMod[i].GlobalCom,(char*)olevar.pbVal);

				i++;
			}
			recSet->MoveNext();
		}
	}
	recSet->Close();
	delete recSet;
	RPCNumEqpsRead=i;
	RPCModElems[0].Type=sizeof(RPCModStruct)+65536*(unsigned long)i;
	
	recSet=new CDaoRecordset(&eqpDB);

	recSet->Open(dbOpenSnapshot,(LPCTSTR)"SELECT Fecs.FecName, ComputerData.IPAddr, Fecs.AlarmCheck, Fecs.MDataTypes FROM ComputerData INNER JOIN Fecs ON ComputerData.HostName = Fecs.HostName WHERE (((Fecs.Used)=1))");
	
	i=0;
	if (!recSet->IsEOF())
	{
		recSet->MoveLast();
		size=(short)recSet->GetRecordCount();
		RPCFec=(RPCFecStruct *)GlobalAlloc(GPTR,sizeof(RPCFecStruct)*size);
		if (RPCFec==NULL) return out_of_memory;
		RPCFecElems[1].Address=(long)RPCFec[0].Name;
		RPCFecElems[2].Address=(long)RPCFec[0].Node;
		RPCFecElems[3].Address=(long)RPCFec[0].Network;
		RPCFecElems[4].Address=(long)&RPCFec[0].AlarmCheck;
		RPCFecElems[5].Address=(long)RPCFec[0].IP;
		RPCFecElems[6].Address=(long)&RPCFec[0].Prot;
		RPCFecElems[7].Address=(long)&RPCFec[0].MHard;
		RPCFecElems[8].Address=(long)&RPCFec[0].MDataTypes;
			
		recSet->MoveFirst();
		while (!recSet->IsEOF())
		{
			recSet->GetFieldValue(0,olevar);
			if (olevar.vt!=VT_NULL) 
			{
				strcpy(RPCFec[i].Name,(char*)olevar.pbVal);

				recSet->GetFieldValue(1,olevar);
				if (olevar.vt!=VT_NULL) strcpy(RPCFec[i].IP,(char*)olevar.pbVal);

				recSet->GetFieldValue(2,olevar);
				if (olevar.vt!=VT_NULL) 
				{
					if (VariantChangeType(&va,&olevar,0,VT_I2)) return database_not_loaded;
					RPCFec[i].AlarmCheck=va.iVal;
				}

				recSet->GetFieldValue(3,olevar);
				if (olevar.vt!=VT_NULL) 
				{
					if (VariantChangeType(&va,&olevar,0,VT_I2)) return database_not_loaded;
					RPCFec[i].MDataTypes=va.iVal;
				}
				RPCFec[i].Prot=2;
				RPCFec[i].MHard=1;
			
				i++;
			}
			recSet->MoveNext();
		}
	}
	recSet->Close();
	delete recSet;
	RPCNumFecsRead=i;
	RPCFecElems[0].Type=sizeof(RPCFecStruct)+65536*(unsigned long)i;

	recSet=new CDaoRecordset(&eqpDB);

	recSet->Open(dbOpenSnapshot,(LPCTSTR)"SELECT Equipment.EqpName, Equipment.FecName, Equipment.ClassName, Equipment.ClassMbNo, Equipment.TypeName, Equipment.Unit, Equipment.Increment, Panels.Control FROM Panels RIGHT JOIN Equipment ON Panels.PanelName = Equipment.PanelName WHERE (((Equipment.Used)=1))");

	i=0;
	if (!recSet->IsEOF())
	{
		recSet->MoveLast();
		size=(short)recSet->GetRecordCount();
		RPCData=(RPCDataStruct *)GlobalAlloc(GPTR,sizeof(RPCDataStruct)*size);
		if (RPCData==NULL) return out_of_memory;
		RPCDataElems[1].Address=(long)RPCData[0].FecName;
		RPCDataElems[2].Address=(long)RPCData[0].FullName;
		RPCDataElems[3].Address=(long)RPCData[0].EqName;
		RPCDataElems[4].Address=(long)&RPCData[0].EqNumber;
		RPCDataElems[5].Address=(long)RPCData[0].Type;
		RPCDataElems[6].Address=(long)RPCData[0].Unit;
		RPCDataElems[7].Address=(long)RPCData[0].Increment;
		RPCDataElems[8].Address=(long)RPCData[0].Control;
		
		recSet->MoveFirst();
		while (!recSet->IsEOF())
		{
			recSet->GetFieldValue(0,olevar);
			if (olevar.vt!=VT_NULL) 
			{
					strcpy(RPCData[i].FullName,(char*)olevar.pbVal);

					recSet->GetFieldValue(1,olevar);
					if (olevar.vt!=VT_NULL) strcpy(RPCData[i].FecName,(char*)olevar.pbVal);

					recSet->GetFieldValue(2,olevar);
					if (olevar.vt!=VT_NULL) strcpy(RPCData[i].EqName,(char*)olevar.pbVal);

					recSet->GetFieldValue(3,olevar);
					if (olevar.vt!=VT_NULL) 
					{
						if (!VariantChangeType(&va,&olevar,0,VT_I2)) RPCData[i].EqNumber=va.iVal;
					}

					recSet->GetFieldValue(4,olevar);
					if (olevar.vt!=VT_NULL) strcpy(RPCData[i].Type,(char*)olevar.pbVal);

					recSet->GetFieldValue(5,olevar);
					if (olevar.vt!=VT_NULL) strcpy(RPCData[i].Unit,(char*)olevar.pbVal);

					recSet->GetFieldValue(6,olevar);
					if (olevar.vt!=VT_NULL) 
					{
						if (!VariantChangeType(&va,&olevar,0,VT_R8)) sprintf(RPCData[i].Increment,"%g",va.dblVal);
					}

					recSet->GetFieldValue(7,olevar);
					if (olevar.vt!=VT_NULL) strcpy(RPCData[i].Control,(char*)olevar.pbVal);

					i++;
			}
			recSet->MoveNext();
		}
	}
	recSet->Close();
	delete recSet;
	RPCNumRecsRead=i;
	RPCDataElems[0].Type=sizeof(RPCDataStruct)+65536*(unsigned long)i;

	eqpDB.Close();
	return 0;
}
int InitRPCAPI()
{
	short cc,i,j;
	char str[100],buf[100];
	struct hostent *host;
	WORD wMajorVersion, wMinorVersion;
	DWORD s;
	unsigned long addr;

	if (InitDone) return 0;

	AfxDaoInit();
	cc=InitDB();
	AfxDaoTerm();

	if (cc)
	{
		if (cc>0) strcpy(ErrStr,erlst[cc]);

		if (lstrlen(ErrStr)) 
		{
			wsprintf(buf,"Initialize RPCAPI32 -> %s",ErrStr);
			RegisterError(APPLICATIONNAME,-1,buf,FALSE);
		}
		return cc;
	}
	
	if (GetModuleHandle("ERRSRV32.EXE")==NULL) // When DDL is called by ErrSrv do not restart
	{
		strcat(ErrSrvCommand,"\\ERRSRV32.EXE");
		// Do not start Error-Server
		//cc=WinExec((LPSTR)ErrSrvCommand,SW_SHOW);
	}

	s=16;
	cc=GetUserName(UserName,(LPDWORD)&s);
	if (!cc) RegisterError(APPLICATIONNAME,-1,"Cannot get Novell Username",TRUE);

	wMajorVersion = MAJOR_VERSION;
	wMinorVersion = MINOR_VERSION;
	VersionReqd=WSA_MAKEWORD(wMajorVersion, wMinorVersion);
  
	lpmyWSAData = (LPWSADATA)calloc(1,sizeof(WSADATA));
	if (WSAStartup(VersionReqd, lpmyWSAData)) return no_link;
	
	cc=gethostname(str,99);
	if (cc) return no_link;

	host=gethostbyname((char *)str);
	if (host==NULL) return no_link;
	memcpy((void *)&LocalAddr,(void *)host->h_addr,sizeof(u_long));

	sendBuffer=(char *)malloc(DEFAULT_PACKET_SIZE);
	if (sendBuffer==NULL) return out_of_memory;

	FecHotlinkActive=(char *)GlobalAlloc(GPTR,RPCNumFecsRead+1);
	if (FecHotlinkActive==NULL) return out_of_memory;
	FecAccessCount=(char *)GlobalAlloc(GPTR,RPCNumFecsRead+1);
	if (FecAccessCount==NULL) return out_of_memory;
	FecLastAccess=(time_t *)GlobalAlloc(GPTR,(RPCNumFecsRead+1)*sizeof(time_t));
	if (FecLastAccess==NULL) return out_of_memory;

	if ((long)WSASetBlockingHook((FARPROC)TCPFunc)==0L) return no_link;
	
	for (i=0;i<RPCNumFecsRead;i++)
	{
		if (lstrlen(RPCFec[i].IP))
		{
			lstrcpy((char *)str,RPCFec[i].IP);  
			addr=inet_addr(str);
			if (addr!=INADDR_NONE)
			{
				memcpy((void *)RPCFec[i].IP,(void *)&addr,sizeof(u_long));
				continue;
			}
			host=gethostbyname((char *)str);
			if (host!=NULL) memcpy((void *)RPCFec[i].IP,(void *)host->h_addr,sizeof(u_long));
			else
			{
				wsprintf(buf,"Cannot resolve hostname %s",str);
				RegisterError(APPLICATIONNAME,-1,buf,TRUE);
			}
		}
		else for (j=0;j<sizeof(u_long);j++) RPCFec[i].IP[j]=0;
	}
	cc=CreateListener();
	if (cc) return cc;

	IDHead=NULL;
	SetTimer(hRPCWnd,1,5000,NULL);

	InitDone=TRUE;  
	
	return 0;   
}
BOOL TCPFunc(void)
{                                              
	MSG msg;
	BOOL ret;
	
	ret=PeekMessage(&msg,NULL,0,0,PM_REMOVE);
	if (ret)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);	
	}
 	return ret;
}
void FAR PASCAL FreeRPCAPI()
{
	RemoveRPCHotlink((HWND)-1,-1);

	if (hRPCWnd)
	{
		KillTimer(hRPCWnd,1);
		DestroyWindow(hRPCWnd);

		UnregisterClass("LocalWindow",hInst);
	}

	GlobalFree(GlobalHandle(RPCData));
	if (RPCMod!=NULL) GlobalFree(GlobalHandle(RPCMod));
	GlobalFree(GlobalHandle(RPCFec));
	GlobalFree(FecHotlinkActive);
	GlobalFree(FecAccessCount);
	GlobalFree(FecLastAccess);

	closesocket(LSck);closesocket(SSck);
	WSACleanup();
}
short RPCConnect(RPCInfoStruct *RPCInfo,WORD *connectionID,short *fecindex)
{
	short dataindex,cc,i;
	char Buffer[BUFFER_SIZE+1];
	SOCKET sck;
	u_long arg;
	char upname[50];

	cc=InitRPCAPI();
	if (cc) return cc;

	RPCInfo->MessageVersion=MESSAGE_VERSION;

	if ((*fecindex)==-1)
	{	
		if ((RPCInfo->DataFormat==CF_DOUBLE || RPCInfo->DataFormat==CF_BR || RPCInfo->DataFormat==CF_AR) && RPCInfo->DataSize<(short)(sizeof(double)*abs(RPCInfo->ArraySize))) return dimension_error;
		if ((RPCInfo->DataFormat==CF_SHORT || RPCInfo->DataFormat==CF_BI || RPCInfo->DataFormat==CF_AI) && RPCInfo->DataSize<(short)(sizeof(short)*abs(RPCInfo->ArraySize))) return dimension_error;
		if (RPCInfo->DataFormat==CF_BYTE && RPCInfo->DataSize<(short)(sizeof(BYTE)*abs(RPCInfo->ArraySize))) return dimension_error;
		if (RPCInfo->DataFormat==CF_TEXT && RPCInfo->DataSize<(short)(30*abs(RPCInfo->ArraySize))) return dimension_error;
		if (RPCInfo->DataFormat==CF_LONG && RPCInfo->DataSize<(short)(sizeof(long)*abs(RPCInfo->ArraySize))) return dimension_error;

		for (i=0;i<lstrlen(RPCInfo->ElemName);i++) 
		{
			if (!CaseSens) RPCInfo->ElemName[i]=toupper(RPCInfo->ElemName[i]);
			upname[i]=toupper(RPCInfo->ElemName[i]);
		}
		RPCInfo->ElemName[i]=0;upname[i]=0;

		dataindex=SearchDataBase(RPCDataElems,upname,"NAME");

		if (dataindex<0) strcpy(Buffer,"DEFAULT");
		else lstrcpy((char *)Buffer,RPCData[dataindex].FecName);

		*fecindex=SearchDataBase(RPCFecElems,Buffer,"FECNAME");
		if (*fecindex<0)
		{
			if (dataindex<0) cc=non_existent_elem;else cc=non_existent_fec;
			return cc;
		}
		if (!RPCFec[*fecindex].MDataTypes) PacketSize=FEC_PACKET_SIZE;else PacketSize=DEFAULT_PACKET_SIZE;

		if (dataindex>=0)
		{
			lstrcpy((char *)RPCInfo->EqpModName,RPCData[dataindex].EqName);
			RPCInfo->EqpModNo=RPCData[dataindex].EqNumber;
		}
	}
	if (RPCFec[*fecindex].Prot!=tcp) return illegal_protocol;
	
	if (FecAccessCount[*fecindex]>=FEC_DIRECT_ACCESS)
	{
	 	//if (FecAccessCount[*fecindex]>=FEC_DELAYED_ACCESS) return link_disabled;
	    if (time(NULL)<FecLastAccess[*fecindex]+FEC_DELAY) return link_suspended;
	}
	FecLastAccess[*fecindex]=time(NULL);
	
	if (NetStat) return link_busy;
				
	sck=socket(AF_INET,SOCK_DGRAM,0);
	if (sck==INVALID_SOCKET) return no_link;

	arg=1L;
	if (ioctlsocket(sck,FIONBIO,(u_long *)&arg)) return no_link;
		                            
	memset(&req_sck_name,0,sizeof(req_sck_name));
	memcpy((void *)&req_sck_name.sin_addr,(void *)RPCFec[*fecindex].IP,sizeof(u_long));
	req_sck_name.sin_family=AF_INET;
	req_sck_name.sin_port=htons(REQUEST_PORT);
		
	*connectionID=sck;
	NetStat=*connectionID;
		
	return 0;
}
short NetSend(short connectionID,short size)
{
	short s,count,retry;
	time_t interim;
	char str[80];

	time(&interim);retry=0;    
	for (count=0;count<size;)
	{
        s=sendto(connectionID,(char *)&sendBuffer[count],size-count,0,(struct sockaddr *)&req_sck_name,sizeof(req_sck_name));
        if (s<0) return net_write_error;
		if (s>0) count=count+s;
		if (time((time_t *)0)>(interim+RPC_TIMEOUT))
		{
			wsprintf(str,"%s %d/%d",erlst[net_write_error],count,size);
			CodeFail(str);
			return net_write_error;
		}
	}
	return 0;
}
short RPCSend(RPCInfoStruct *RPCInfo,WORD connectionID,void *Data,short DataFormat,short DataType,short fecindex)
{
	short cc=0,i,count,rwflag,datasize,fecformat;
	char *ptrd;
	BOOL remove=FALSE,found=FALSE;
	char *ValBuf;
	
	req_sck_name.sin_port=htons(REQUEST_PORT);

	RPCInfo->TotalSize=sizeof(RPCInfoStruct);
	strcpy(RPCInfo->UserName,UserName);

	RPCInfo->UpdatePort=UpdatePort;

	datasize=0;ValBuf=NULL;
	if (!RPCFec[fecindex].MDataTypes) 
	{
		fecformat=CF_DOUBLE;
	}
	else
	{
		if (DataType) fecformat=DataType;else fecformat=CF_DOUBLE; // default format for Multidatatype FECs
	}
	RPCInfo->DataFormat=fecformat;
	
	if (RPCInfo->ArraySize)
	{
		rwflag=(short)abs(RPCInfo->ArraySize)/(short)RPCInfo->ArraySize;
		if (rwflag<0)
		{
			datasize=GetFormatSize(fecformat)*abs(RPCInfo->ArraySize);
			ValBuf=(char*)malloc(datasize);
			if (!ValBuf)
			{
				cc=out_of_memory;
				goto error;
			}
			cc=ConvertData(0,RPCInfo->EqpModName,RPCInfo->Property,Data,DataFormat,ValBuf,fecformat,datasize,(short)abs(RPCInfo->ArraySize),NULL,0);
		}
		RPCInfo->TotalSize=RPCInfo->TotalSize+datasize;
	}

//	if (RPCInfo->RefreshPeriod<=0) RPCInfo->RefreshPeriod=-1;

	ptrd=(char *)sendBuffer;
	count=sizeof(RPCInfoStruct);
	memcpy(ptrd,RPCInfo,count);

	for (i=0;i<datasize;i++)
	{
		if (count==PacketSize)
		{
			if (cc=NetSend(connectionID,count),cc) goto error;
			count=0;
		}
		ptrd[count++]=ValBuf[i];	 	
	}
	if (count) cc=NetSend(connectionID,count);
error:
	if (ValBuf) free(ValBuf);
	return cc;
}
short RPCReceive(RPCInfoStruct *RPCInfo,WORD connectionID,void *Data,long DataSize,short DataFormat,short fecindex)
{
	long i,done,actseq,seqnr,cc,el,ind,count,s,msgsize,arraysize;
	long dataformat;
	time_t interim;
	int l;
	char str[DEFAULT_PACKET_SIZE+1],*msgbuf;
	
	msgbuf=NULL;

	el=0;ind=0;cc=0;

	done=0;actseq=0;time(&interim);seqnr=-1;cc=0;

	time(&interim);
		
	while (1)
	{                 
		Sleep(0);
		ListenUDP();

		l=sizeof(req_sck_name);
		s=recvfrom(connectionID,(char *)str,PacketSize,0,(struct sockaddr *)&req_sck_name,(int *)&l);
		if (s>0) break;

		if (time((time_t *)0)>(interim+RPC_TIMEOUT))
		{                                           
			FecAccessCount[fecindex]++;
			cc=link_not_open;goto out;
		}
	}   
	msgsize=*(long *)str;                 

	if (msgsize>s)
	{
		sendBuffer[0]=CHECK_CHAR;NetSend(connectionID,1);
	}
			
	msgbuf=(char *)malloc(msgsize);
				
	if (msgbuf==NULL) {cc=out_of_memory;goto out;}    
	if (msgsize<=PacketSize) count=msgsize;else count=PacketSize;

	for (i=4;i<count;i++) msgbuf[i-4]=str[i];
		
	time(&interim);
	while (count<msgsize)
	{                    
		Sleep(0);
		l=sizeof(req_sck_name);        
		s=recvfrom(connectionID,(char *)&msgbuf[count-4],msgsize-count,0,(struct sockaddr *)&req_sck_name,(int *)&l);
		if (s>0)
		{       
			count=count+s;
			if (msgsize>count)
			{
				sendBuffer[0]=CHECK_CHAR;NetSend(connectionID,1);
			}
			time(&interim);
		}
		if (time((time_t *)0)>(interim+RPC_TIMEOUT))
		{                              
			FecAccessCount[fecindex]++;
			cc=link_not_open;goto out;
		}                                
	}
			
	count=0;msgsize=msgsize-4;
	arraysize=0;
	
	cc=*(long *)&msgbuf[count];
	count=count+sizeof(long);

	if (cc<=0 && count<msgsize)
	{
		dataformat=*(long *)&msgbuf[count];
		count=count+sizeof(long);
		arraysize=*(long *)&msgbuf[count];
		count=count+sizeof(long);
	}  
	ConvertData((short)cc,RPCInfo->EqpModName,RPCInfo->Property,&msgbuf[count],(short)dataformat,Data,DataFormat,DataSize,(short)arraysize,ErrStr,BUFFER_SIZE);
	FecAccessCount[fecindex]=0;
out:
	if (msgbuf) free(msgbuf);
	return (short)cc;
}
void CodeFail(char *str)
{
	char buf[200];

	wsprintf(buf,"Code failure : %s  Please note !",str);
	MessageBox(GetFocus(),(LPSTR)buf,APPLICATIONNAME,MB_SYSTEMMODAL | MB_OK | MB_ICONHAND);
}
short ConvertData(short cc,char *EqpModName,char *Prop,void *sdata,short sformat,void *ddata,short dformat,long ddatasize,short Size,char *Err,short ErrStrSize)
{
	double *dptrs,*dptrd;
	short *iptrs,*iptrd;
	char *dptr,*sptr;
	long *lptrs,*lptrd;
	float *fptrs,*fptrd;
	BYTE *bptrs,*bptrd;
	long i,x;
	char buf[100];
	double dvalue;
	BOOL Formatting=TRUE;
	char Property[100];

	strcpy(Property,Prop);
	for (i=0;i<(long)strlen(Property);i++) Property[i]=toupper(Property[i]);

	if (Err && ErrStrSize)
	{
		Err[0]=0;
		if (cc>0)
		{
			i=lstrlen(erlst[cc]);
			if (i>=ErrStrSize) i=ErrStrSize-1;
	 		memcpy(Err,erlst[cc],i);
			Err[i]=0;
			return 0;
		}
		if (cc<0)
		{
			if (sformat!=CF_TEXT) 
			{
				strcpy(Err,"Cannot return error message");
				return 0;
			}
	 		i=lstrlen((char *)sdata);
			if (i>=ErrStrSize) i=ErrStrSize-1;
	 		memcpy(Err,sdata,i);
			Err[i]=0;
			return 0;
		}
	}
	if (!Size) return 0;

	dptr=(char *)ddata;dptr[0]=0;

	if (dformat!=CF_TEXT && sformat==CF_TEXT && !atof((char*)sdata))
	{
		i=0;
		while (i<RPCNumEqpsRead && lstrcmpi(RPCMod[i].EqpName,EqpModName)) i++;
		if (i<RPCNumEqpsRead)
		{
			x=0;
			while ((strcmpi(RPCMod[i+x].Property,Property) || strcmpi(RPCMod[i+x].EqpName,EqpModName) || lstrcmpi(RPCMod[i+x].GlobalCom,(char*)sdata)) && (i+x)<RPCNumEqpsRead) x++;
			if ((i+x)<RPCNumEqpsRead)
			{
				dvalue=(double)RPCMod[i+x].Weight;
				goto out;
			}
		}
		sptr=(char *)sdata;
		for (i=-1,x=0;sptr[x];x++)
		{
			if (sptr[x]>='0' && sptr[x]<='9')
			{
				i=x;
				break;
			}
		}
		if (i!=-1) 
		{
			dvalue=atof(&sptr[i]);
			goto out;
		}
		else return illegal_command;
	}

	switch (dformat)
	{
	case CF_TEXT:
		if (Size==1)
		{
			i=0;
			while (i<RPCNumEqpsRead && lstrcmpi(RPCMod[i].EqpName,EqpModName)) i++;
			if (i<RPCNumEqpsRead)
			{
				switch (sformat)
				{
				case CF_DOUBLE:
				case CF_AR:
				case CF_BR:
					dvalue=*(double *)sdata;
					break;
				case CF_FLOAT:
					dvalue=*(float *)sdata;
					break;
				case CF_LONG:
					dvalue=*(long *)sdata;
					break;
				case CF_SHORT:
				case CF_BI:
				case CF_AI:
					dvalue=*(short *)sdata;
					break;
				case CF_BYTE:
					dvalue=*(BYTE *)sdata;
					break;
				case CF_TEXT:
					dvalue=(double)atof((char *)sdata);
					break;
				}
		
				while (i<RPCNumEqpsRead && (strcmpi(RPCMod[i].EqpName,EqpModName) || strcmpi(RPCMod[i].Property,Property))) i++;
				if (i<RPCNumEqpsRead)
				{
					while (i<RPCNumEqpsRead && !strcmpi(RPCMod[i].EqpName,EqpModName) && !strcmpi(RPCMod[i].Property,Property)) 
					{
						if ((RPCMod[i].Weight & ((unsigned long)dvalue)) || (!RPCMod[i].Weight && !(unsigned long)dvalue))
						{
							if (lstrlen(dptr)) strcat(dptr,",");

							if ((lstrlen(dptr)+lstrlen(RPCMod[i].StatName))>=ddatasize) return 0;
							strcat(dptr,RPCMod[i].StatName);
						}
						i++;
					}
					return 0;
				}
			}
		}

		if (sformat==CF_TEXT)
		{
			i=lstrlen((char *)sdata);
			if (i>=ddatasize) i=ddatasize-1;
	 		memcpy(dptr,sdata,i);
			dptr[i]=0;
			return 0;
		}
		else
		{
			for (i=0;i<Size;i++)
	  		{
				if (i) strcat(dptr,",");
				switch (sformat)
				{
				case CF_DOUBLE:
				case CF_AR:
				case CF_BR:
					dptrs=(double *)sdata;	
					if (Formatting) sprintf(buf,"%0.10lg",dptrs[i]);else sprintf(buf,"%lf",dptrs[i]);	
					break;
				case CF_FLOAT:
					fptrs=(float *)sdata;
					if (Formatting) sprintf(buf,"%0.10g",fptrs[i]);else sprintf(buf,"%f",fptrs[i]);	
					break;
				case CF_LONG:
					lptrs=(long *)sdata;
					wsprintf(buf,"%ld",lptrs[i]);	
					break;
				case CF_SHORT:
				case CF_AI:
				case CF_BI:
					iptrs=(short *)sdata;
					wsprintf(buf,"%d",iptrs[i]);	
					break;
				case CF_BYTE:
					bptrs=(BYTE *)sdata;
					wsprintf(buf,"%d",bptrs[i]);	
					break;
				}
				if ((lstrlen(dptr)+lstrlen(buf))>=ddatasize) return 0;
				strcat(dptr,buf);
			}
		}
		break;
	case CF_DOUBLE:
	case CF_AR:
	case CF_BR:
		dptrd=(double *)ddata;

		switch (sformat)
		{
		case CF_DOUBLE:
		case CF_AR:
		case CF_BR:
			dptrs=(double *)sdata;
			for (i=0;i<Size;i++) dptrd[i]=dptrs[i];
			break;
		case CF_FLOAT:
			fptrs=(float *)sdata;
			for (i=0;i<Size;i++) dptrd[i]=(double)fptrs[i];
			break;
		case CF_LONG:
			lptrs=(long *)sdata;
			for (i=0;i<Size;i++) dptrd[i]=(double)lptrs[i];
			break;
		case CF_SHORT:
		case CF_AI:
		case CF_BI:
			iptrs=(short *)sdata;
			for (i=0;i<Size;i++) dptrd[i]=(double)iptrs[i];
			break;
		case CF_BYTE:
			bptrs=(BYTE *)sdata;
			for (i=0;i<Size;i++) dptrd[i]=(double)bptrs[i];
			break;
		case CF_TEXT:
			dptrd[0]=(double)atof((char *)sdata);
			break;
		}
		break;
	case CF_FLOAT:
		fptrd=(float *)ddata;

		switch (sformat)
		{
		case CF_DOUBLE:
		case CF_AR:
		case CF_BR:
			dptrs=(double *)sdata;
			for (i=0;i<Size;i++) fptrd[i]=(float)dptrs[i];
			break;
		case CF_FLOAT:
			fptrs=(float *)sdata;
			for (i=0;i<Size;i++) fptrd[i]=fptrs[i];
			break;
		case CF_LONG:
			lptrs=(long *)sdata;
			for (i=0;i<Size;i++) fptrd[i]=(float)lptrs[i];
			break;
		case CF_SHORT:
		case CF_AI:
		case CF_BI:
			iptrs=(short *)sdata;
			for (i=0;i<Size;i++) fptrd[i]=(float)iptrs[i];
			break;
		case CF_BYTE:
			bptrs=(BYTE *)sdata;
			for (i=0;i<Size;i++) fptrd[i]=(float)bptrs[i];
			break;
		case CF_TEXT:
			fptrd[0]=(float)atof((char *)sdata);
			break;
		}
		break;
	case CF_LONG:
		lptrd=(long *)ddata;

		switch (sformat)
		{
		case CF_DOUBLE:
		case CF_AR:
		case CF_BR:
			dptrs=(double *)sdata;
			for (i=0;i<Size;i++) lptrd[i]=(long)dptrs[i];
			break;
		case CF_FLOAT:
			fptrs=(float *)sdata;
			for (i=0;i<Size;i++) lptrd[i]=(long)fptrs[i];
			break;
		case CF_LONG:
			lptrs=(long *)sdata;
			for (i=0;i<Size;i++) lptrd[i]=lptrs[i];
			break;
		case CF_SHORT:
		case CF_AI:
		case CF_BI:
			iptrs=(short *)sdata;
			for (i=0;i<Size;i++) lptrd[i]=(long)iptrs[i];
			break;
		case CF_BYTE:
			bptrs=(BYTE *)sdata;
			for (i=0;i<Size;i++) lptrd[i]=(long)bptrs[i];
			break;
		case CF_TEXT:
			lptrd[0]=(long)atol((char *)sdata);
			break;
		}
		break;
	case CF_SHORT:
	case CF_AI:
	case CF_BI:
		iptrd=(short *)ddata;

		switch (sformat)
		{
		case CF_DOUBLE:
		case CF_AR:
		case CF_BR:
			dptrs=(double *)sdata;
			for (i=0;i<Size;i++) iptrd[i]=(short)dptrs[i];
			break;
		case CF_FLOAT:
			fptrs=(float *)sdata;
			for (i=0;i<Size;i++) iptrd[i]=(short)fptrs[i];
			break;
		case CF_LONG:
			lptrs=(long *)sdata;
			for (i=0;i<Size;i++) iptrd[i]=(short)lptrs[i];
			break;
		case CF_SHORT:
		case CF_AI:
		case CF_BI:
			iptrs=(short *)sdata;
			for (i=0;i<Size;i++) iptrd[i]=iptrs[i];
			break;
		case CF_BYTE:
			bptrs=(BYTE *)sdata;
			for (i=0;i<Size;i++) iptrd[i]=(short)bptrs[i];
			break;
		case CF_TEXT:
			iptrd[0]=(short)atoi((char *)sdata);
			break;
		}
		break;
	case CF_BYTE:
		bptrd=(BYTE *)ddata;

		switch (sformat)
		{
		case CF_DOUBLE:
		case CF_AR:
		case CF_BR:
			dptrs=(double *)sdata;
			for (i=0;i<Size;i++) bptrd[i]=(BYTE)dptrs[i];
			break;
		case CF_FLOAT:
			fptrs=(float *)sdata;
			for (i=0;i<Size;i++) bptrd[i]=(BYTE)fptrs[i];
			break;
		case CF_LONG:
			lptrs=(long *)sdata;
			for (i=0;i<Size;i++) bptrd[i]=(BYTE)lptrs[i];
			break;
		case CF_SHORT:
		case CF_AI:
		case CF_BI:
			iptrs=(short *)sdata;
			for (i=0;i<Size;i++) bptrd[i]=(BYTE)iptrs[i];
			break;
		case CF_BYTE:
			bptrs=(BYTE *)sdata;
			for (i=0;i<Size;i++) bptrd[i]=bptrs[i];
			break;
		case CF_TEXT:
			iptrd[0]=(BYTE)atoi((char *)sdata);
			break;
		}
		break;
	}
	return 0;
out:
	switch (dformat)
	{
	case CF_DOUBLE:
	case CF_AR:
	case CF_BR:
		*(double *)ddata=(double)dvalue;
		break;
	case CF_FLOAT:
		*(float *)ddata=(float)dvalue;
		break;
	case CF_LONG:
		*(long *)ddata=(long)dvalue;
		break;
	case CF_SHORT:
	case CF_AI:
	case CF_BI:
		*(short *)ddata=(short)dvalue;
		break;
	case CF_BYTE:
		*(BYTE *)ddata=(BYTE)dvalue;
		break;
	}	
	return 0;
}
short GetFormatSize(short format)
{
	switch (format)
	{
	case CF_DOUBLE:
	case CF_AR:
	case CF_BR:
		return sizeof(double);
	case CF_SHORT:
	case CF_AI:
	case CF_BI:
		return sizeof(short);
	case CF_LONG:return sizeof(long);
	case CF_FLOAT:return sizeof(float);
	case CF_TEXT:return 500*sizeof(char);
	}
	return 0;
}
short CreateListener()
{
	u_long arg;
	SOCKADDR_IN sck_name;
	short cc;
	HWND hWnd;

	UpdateMsgBuf=NULL;
	ListenActive=FALSE;
	DoListen=FALSE;
		
	hWnd=CreateLocalWnd();
	if (!hWnd) 
	{
		RegisterError(APPLICATIONNAME,-1,"Unable to Create Window",TRUE);
		return no_link;
	}
	hRPCWnd=hWnd;
	
	LSck=socket(AF_INET,SOCK_DGRAM,0);
	if (LSck==INVALID_SOCKET)
	{
		RegisterError(APPLICATIONNAME,-1,"Unable to Create Listener Socket.",TRUE);
		return no_link;
	}
	arg=1L;
	if (ioctlsocket(LSck,FIONBIO,(u_long *)&arg))
	{
		RegisterError(APPLICATIONNAME,-1,"Unable to set Listener Socket non-blocking.",TRUE);
		return no_link;
	} 

	UpdatePort=UPDATE_PORT_OFFSET;
	do
	{
		memset(&sck_name,0,sizeof(sck_name));
		sck_name.sin_addr.s_addr=0;
		sck_name.sin_family=AF_INET;
		sck_name.sin_port=htons(UpdatePort);

		if (cc=bind(LSck,(LPSOCKADDR)&sck_name,sizeof(sck_name)))
		{
			if (GetLastError()!=WSAEADDRINUSE)
			{
				RegisterError(APPLICATIONNAME,-1,"Unable to bind the Listener Socket.",TRUE);
				return no_link;
			}
			else UpdatePort++;
		}                  
	}while (cc);

	if (WSAAsyncSelect(LSck,hWnd,UPDATE_MESSAGE,FD_READ))
	{
		RegisterError(APPLICATIONNAME,-1,"Unable select socket.",TRUE);
		return no_link;
	}

	SSck=socket(AF_INET,SOCK_DGRAM,0);
	if (SSck==INVALID_SOCKET)
	{
		RegisterError(APPLICATIONNAME,-1,"Unable to Create Watchdog Socket.",TRUE);
		return no_link;
	}
	arg=1L;
	if (ioctlsocket(SSck,FIONBIO,(u_long *)&arg))
	{
		RegisterError(APPLICATIONNAME,-1,"Unable to set Watchdog Socket non-blocking.",TRUE);
		return no_link;
	}                               
	return 0;
}
void ListenUDP()
{
	EqpUpdateStruct *EqpUpdate;
	time_t interim;
	long i,s,dataoffset,eqp,eqpsize;
	int l;
	struct sockaddr_in sck_name;
	long msgsize,seqnr,actseq,count;
	char *msgbuf,ch,buf[DEFAULT_PACKET_SIZE];
	struct IDListStruct *IDList;
	long *hwndlist;

	msgbuf=NULL;hwndlist=NULL;
				  
	memset(&sck_name,0,sizeof(sck_name));

	l=sizeof(sck_name);
	s=recvfrom(LSck,(char *)buf,DEFAULT_PACKET_SIZE,0,(struct sockaddr *)&sck_name,(int *)&l);
	if (s<=0) goto out;
	         
	actseq=0;
	seqnr=*(long *)buf;
	if (seqnr!=actseq) goto out;
	msgsize=*(long *)&buf[4];

	msgbuf=(char *)malloc(msgsize);
	if (msgbuf==NULL) goto out;
	if (s<DEFAULT_PACKET_SIZE && s<FEC_PACKET_SIZE && s!=msgsize) goto out;

	for (count=0;count<(s-8) && count<(msgsize-8);count++) msgbuf[count]=buf[count+8];
	if (count<msgsize-8) 
	{
		ch=CHECK_CHAR;        
		sendto(LSck,(char *)&ch,1,0,(struct sockaddr *)&sck_name,sizeof(sck_name));	                 	
	}
	time(&interim);

	while (count<msgsize-8)
	{
		l=sizeof(sck_name);
		s=recvfrom(LSck,(char *)buf,DEFAULT_PACKET_SIZE,0,(struct sockaddr *)&sck_name,(int *)&l);
		if (s>0)
		{
			actseq++;      
			
			seqnr=*(short *)buf;
			if (seqnr!=actseq) goto out;
			for (i=4;i<s && count<msgsize-8;i++,count++) msgbuf[count]=buf[i];
			if (count<msgsize-8)
			{
				ch=CHECK_CHAR;
				sendto(LSck,(char *)&ch,1,0,(struct sockaddr *)&sck_name,sizeof(sck_name));	                 	
			}
			time(&interim);
		}
		if (time((time_t *)0)>(interim+RPC_TIMEOUT))
		{                              
			goto out;
		}                                
	}
	
	if (NetStat || ListenActive || DoListen) goto out;
	
	DoListen=TRUE;
	UpdateMsgBuf=msgbuf;

	eqpsize=*(long *)UpdateMsgBuf;
	hwndlist=(long *)malloc(sizeof(long)*eqpsize);
	if (hwndlist==NULL) goto out;

	for (dataoffset=4,eqp=0;eqp<eqpsize;eqp++)
	{
		EqpUpdate=(EqpUpdateStruct *)&UpdateMsgBuf[dataoffset];
	
		IDList=IDHead;
		while (IDList)
		{
	 		if (IDList->SubScrID==EqpUpdate->SubScrID && IDList->hWnd==(HWND)EqpUpdate->hWnd)
			{
				IDList->LastUpdate=interim;
				break;
			}
			if (IDList) IDList=IDList->Next;
		}
	
		hActualWnd=EqpUpdate->hWnd;
		hwndlist[eqp]=hActualWnd;

		for (i=0;i<eqp;i++)
		{
			if (hwndlist[i]==hActualWnd) goto neqp;
		}
		
		if (!IsWindow((HWND)hActualWnd))
		{
			RemoveRPCHotlink((HWND)hActualWnd,-1);
			goto out;
		}
		
		ListenActive=TRUE;

		SendMessage((HWND)hActualWnd,WM_SIZE,(WPARAM)0,(LPARAM)0);

		ListenActive=FALSE;	
neqp:
		dataoffset=dataoffset+sizeof(EqpUpdateStruct)+EqpUpdate->DataSize;
	}
UpdateMsgBuf=NULL;
out:
	DoListen=FALSE;
	if (msgbuf!=NULL) free(msgbuf);
	if (hwndlist!=NULL) free(hwndlist);
}
void SendWatch(u_long addr)
{
	struct sockaddr_in sck_name;
	short s;
	
	memset(&sck_name,0,sizeof(sck_name));

	sck_name.sin_addr.s_addr=addr;
	sck_name.sin_family=AF_INET;
	sck_name.sin_port=htons(INFO_PORT);
	
	s=sendto(SSck,(char *)&UpdatePort,sizeof(short),0,(struct sockaddr *)&sck_name,sizeof(sck_name));
 	return;
}
HWND CreateLocalWnd()
{
	HWND hWnd;
	WNDCLASS wndclass;

	wndclass.lpfnWndProc=(WNDPROC)LocalWndProc;
	wndclass.lpszClassName="LocalWindow";
	wndclass.style         = 0;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = hInst;
	wndclass.hIcon         = NULL;
	wndclass.hCursor       = NULL;
	wndclass.hbrBackground = NULL;
	wndclass.lpszMenuName  = NULL;

	hRPCWnd=0;

	if (!RegisterClass (&wndclass)) return 0;

	hWnd = CreateWindow ((LPSTR)"LocalWindow",(LPSTR)"Local Window",
						 0,0,0,0,0,
						 NULL,NULL,
						 hInst,
						 (LPSTR)NULL);
	return hWnd;
}
LRESULT CALLBACK LocalWndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	struct IDListStruct *IDList;
	time_t interim;
	u_long ipaddr;
	short findex[200],i,cc;
	char buf[300];

	switch (message)
	{
	case UPDATE_MESSAGE:
		ListenUDP();
		break;
	case WM_TIMER:
		if (HotlinkCheckBusy) break;
		HotlinkCheckBusy=TRUE;
		IDListCheckCount++;

		if (IDListCheckCount>=12)  //12
		{
			time(&interim);DoRecreate=TRUE;

			IDList=IDHead;
			while (IDList)
			{
	 			if ((IDList->LastUpdate+300)<interim)  //300
	 			{
					RemoveRPCHotlink(IDList->hWnd,(short)IDList->SubScrID);
					cc=CreateRPCHotlink(IDList->hWnd,IDList->ElemName,IDList->Property,IDList->Cycle,IDList->RefreshPeriod,(short)IDList->ArraySize,(short)IDList->SubScrID,(short)IDList->DataType);
					if (cc)
					{
						wsprintf(buf,"Recreate Hotlink %s\\%s\\%s\\S%d\\ID%d -> %s",IDList->ElemName,IDList->Property,IDList->Cycle,IDList->ArraySize,IDList->SubScrID,ErrStr);
						RegisterError(APPLICATIONNAME,-1,buf,FALSE);
					}
					IDList->LastUpdate=interim;
				}
				if (IDList) IDList=IDList->Next;
			}
			IDListCheckCount=0;
			DoRecreate=FALSE;
		}
		else
		{
			for (i=0;i<200;i++) findex[i]=-1;
			IDList=IDHead;
			while (IDList)
			{
				for (i=0;i<200;i++)
				{
					if (findex[i]==IDList->FecIndex) break;
					if (findex[i]==-1)
					{
						findex[i]=IDList->FecIndex;
						memcpy((void *)&ipaddr,(void *)RPCFec[IDList->FecIndex].IP,sizeof(u_long));
						SendWatch(ipaddr);
						break;
					}
				}
				if (IDList) IDList=IDList->Next;
			}
			
		}
		HotlinkCheckBusy=FALSE;

		break;
	default:
		return DefWindowProc (hWnd, message, wParam, lParam);
	}
	return 0;
}
short FAR PASCAL RegisterError(const char *fapplicationname,short errorvalue,const char *fdesc,BOOL activate)
{
	char command[MAX_MESSAGE_SIZE],str[200],c;
	SOCKET sck;
	struct sockaddr_in sck_name;
	short s;
	u_long arg;

	if (activate) c='T';else c='F';
	if (fdesc==NULL || !fdesc[0]) str[0]=0;else lstrcpy((LPSTR)str,fdesc);

	wsprintf((LPSTR)command,(LPSTR)"%s_%d_%s_%c",fapplicationname,errorvalue,(LPSTR)str,c);
	
	sck=socket(AF_INET,SOCK_DGRAM,0);
	if (sck==INVALID_SOCKET) return no_link;

	arg=1L;
	if (ioctlsocket(sck,FIONBIO,(u_long *)&arg)) 
	{
		closesocket(sck);
		return no_link;
	}
		                            
	memset(&sck_name,0,sizeof(sck_name));
	sck_name.sin_addr.s_addr=LocalAddr;
	sck_name.sin_family=AF_INET;
	sck_name.sin_port=htons(ERRORSRV_PORT);

	s=sendto(sck,(char *)command,strlen(command),0,(struct sockaddr *)&sck_name,sizeof(sck_name));
    if (s<0) 
	{
		closesocket(sck);
		return net_write_error;
	}
	closesocket(sck);	
	return 0;
}
