#include "stdafx.h"
#include "MCP for NT.h"

#include "errors.h"
#include "stdio.h"
#include <stdlib.h>

#include "malloc.h"
#include "time.h"
#include "math.h"
#include "isorpc.h"
#include "string.h"
#include "dbcsvunx.h"
#include "errors.c"
typedef unsigned char BYTE;
typedef unsigned short WORD;

/*#define ERR_FATAL    -2*/
#define FEC_DIRECT_ACCESS   5
#define FEC_DELAYED_ACCESS  8
#define FEC_DELAY 30

#define CHECK_CHAR 13
#define tcp 2
#define REQUEST_PORT 8003
#define WATCH_DELAY  2000

#define TRUE 1
#define FALSE 0

#include "sys/types.h"
#include "fcntl.h"

#define NOD_BUF_SIZE      1024
#define RPC_RETRY_COUNT   3
#define RPC_TIMEOUT       5
#define RPC_SOCKET          0x4545
#define RPC_RECEIVE_ECBS    3

#define BUFFER_SIZE             100
#define FEC_NAME_SIZE           20
#define ELEM_NAME_SIZE          15
#define PROPERTY_SIZE           10
#define CYCLE_SIZE              20
#define EQNAME_SIZE     6

typedef struct
{
	short TotalSize;
	char UserName[16];
	short Protocol;
	short EqpSize;
}HeaderStruct;

typedef struct
{
	char ElemName[16];
	char EqpModName[8];
	short EqpModNo;
	char Property[12];
	char Cycle[22];
	short DataFormat;
	short DataSize;
}EqpInfoStruct;

typedef struct ELEMDATA
{
	char ElemName[ELEM_NAME_SIZE+1];
	char Property[PROPERTY_SIZE+1];
	char *Values;
	short DataFormat;
	short CompCode;
	char ErrStr[BUFFER_SIZE+1];
	short FecIndex;
	char EqName[EQNAME_SIZE+1];
	short Size;
	short EqNo;
	char Cycle[CYCLE_SIZE+1];
}ElemDataStruct;

typedef struct
{
	char FecName[FEC_NAME_SIZE+1];
	char FullName[ELEM_NAME_SIZE+1];
	char EqName[7];
	short EqNumber;
} RPCDataStruct;

typedef struct
{
	char Name[FEC_NAME_SIZE+1];
	BYTE Node[6];
	BYTE Network[4];
	char IP[17];
	short Prot;
}RPCFecStruct;

typedef struct
{
	char EqpName[7];
	char StatName[20];
	char GlobalCom[20];
	short Weight;
}RPCModStruct;

RPCModStruct *RPCMod;
RPCDataStruct *RPCData;
RPCFecStruct  *RPCFec;

short RPCNumRecsRead=0,RPCNumFecsRead=0,RPCNumEqpsRead=0;
short NetProt;


DataBaseElemsStruct RPCDataElems[]={
	"",sizeof(RPCDataStruct),NULL,
	"FECNAME",STR,NULL,
	"NAME",STR,NULL,
	"EQPMODULE",STR,NULL,
	"EQPNUMBER",INT,NULL,
	""};

DataBaseElemsStruct RPCModElems[]={
	"",sizeof(RPCModStruct),NULL,
	"EQPMODULE",STR,NULL,
	"STATUS",STR,NULL,
	"COMMAND",STR,NULL,
	"WEIGHT",INT,NULL,
	""};

DataBaseElemsStruct RPCFecElems[]={
	"",sizeof(RPCFecStruct),NULL,
	"FECNAME",STR,NULL,
	"FECNODE",HEX,NULL,
	"FECNETWORK",HEX,NULL,
	"IPADDR",STR,NULL,
	"PROTOCOL",INT,NULL,
	""};

char *sendBuffer;
short lasterror;

short NetSend(short connectionID,short size);
short RPCConnect(WORD wParam,ElemDataStruct *Elem,WORD *connectionID);
short RPCSend(WORD wParam,ElemDataStruct *Elem,WORD connectionID);
void RPCReceive(WORD wParam,ElemDataStruct *Elem,WORD connectionID);
short GetFormatSize(short format);
void ConvertFormat(void *sdata,short sformat,void *ddata,short dformat,short Size);
short ValueToStr(char *Values,short DataFormat,char *ValDesc,short Datasize,short RowNum,short ColNum,char *EqName,char *Property,BOOL Formatting);
short StrToValue(double *Value,char *Command,char *EqName,char *Property);
WORD IntSwap(WORD w);

struct sockaddr_in req_sck_name;

char *FecAccessCount;
time_t *FecLastAccess;

short InitRPC(char *PathEntry)
{
	lasterror = 0;
	USHORT i;
	short j,size;
	char str[100],buf[100];
	char *ptr;
	char Path[50];
	struct hostent *host;
	unsigned long l;

	WORD wVersionRequested;  
	WSADATA wsaData; 
	
	if (!strlen(PathEntry)) 
	{
		//printf("No database path specified\n");
		return database_not_loaded;
	}
	
	wVersionRequested = MAKEWORD(2,0); 
 
	if(WSAStartup(wVersionRequested, &wsaData)!=0) return 1; 
 
    /* Tell the user that we couldn't find a useable */ 
    /* winsock.dll.     */ 

	strcpy(Path,PathEntry);strcat(Path,"/");

	//printf("Initializing RPC-linksoftware\n");

	sendBuffer=(char *)malloc(NOD_BUF_SIZE);
	if (sendBuffer==NULL) return out_of_memory;

	sprintf(RPCModElems[0].ColName,"%seqpmod.csv",Path);
	size=GetDataBaseSize(RPCModElems[0].ColName);
	if (size>=0) sprintf(str,"Reading EQPMOD.CSV : %d lines",size);
	else sprintf(str,"Reading EQPMOD.CSV : %s",erlst[abs(size)]);
	//printf("%s\n",str);
	RPCMod=NULL;
	if (size>0)
	{
		RPCMod=(RPCModStruct *)malloc(sizeof(RPCModStruct)*size);
		if (RPCMod==NULL) return out_of_memory;
		ptr=(char *)RPCMod;
		for (i=0;i<sizeof(RPCModStruct)*size;i++) ptr[i]=0;
		RPCModElems[1].Address=RPCMod[0].EqpName;
		RPCModElems[2].Address=RPCMod[0].StatName;
		RPCModElems[3].Address=RPCMod[0].GlobalCom;
		RPCModElems[4].Address=&RPCMod[0].Weight;
		RPCNumEqpsRead=ReadDataBase(RPCModElems);
		if (RPCNumEqpsRead<0) return database_not_loaded;
	}
	else RPCNumEqpsRead=0;

	sprintf(RPCFecElems[0].ColName,"%sfecaddr.csv",Path);
	size=GetDataBaseSize(RPCFecElems[0].ColName);
	if (size>=0) sprintf(str,"Reading FECADDR.CSV : %d lines",size);
	else sprintf(str,"Reading FECADDR.CSV : %s",erlst[abs(size)]);
	//printf("%s\n",str);
	if (size<0) return abs(size);
	RPCFec=(RPCFecStruct *)malloc(sizeof(RPCFecStruct)*size);
	if (RPCFec==NULL) return out_of_memory;
	ptr=(char *)RPCFec;
	for (i=0;i<sizeof(RPCFecStruct)*size;i++) ptr[i]=0;
	RPCFecElems[1].Address=RPCFec[0].Name;
	RPCFecElems[2].Address=RPCFec[0].Node;
	RPCFecElems[3].Address=RPCFec[0].Network;
	RPCFecElems[4].Address=RPCFec[0].IP;
	RPCFecElems[5].Address=&RPCFec[0].Prot;
	RPCNumFecsRead=ReadDataBase(RPCFecElems);
	if (RPCNumFecsRead<0) return database_not_loaded;

	FecAccessCount=(char *)malloc(size+1);
	if (FecAccessCount==NULL) return out_of_memory;
	FecLastAccess=(time_t *)malloc((size+1)*sizeof(time_t));
	if (FecLastAccess==NULL) return out_of_memory;
	for (i=0;i<RPCNumFecsRead;i++)
	{
			FecAccessCount[i]=0;
			FecLastAccess[i]=0;
	}

	for (i=0;i<RPCNumFecsRead;i++)
	{
		if (strlen(RPCFec[i].IP))
		{
			strcpy((char *)str,RPCFec[i].IP);
			l=inet_addr(str);
			if (l==-1)
			{
			   //printf("Trying by name\n");
			   host=gethostbyname(str);
			   if (host!=NULL) memcpy(RPCFec[i].IP,host->h_addr,sizeof(unsigned long));
			   else
			   {
				sprintf(buf,"Cannot find %s IP-address",str);
				//printf("%s\n",buf);
			   }
			}
			else *(unsigned long *)RPCFec[i].IP=l;
			/*printf("%X\n",*(unsigned long *)RPCFec[i].IP);*/
		}
		else for (j=0;j<sizeof(long);j++) RPCFec[i].IP[j]=0;
	}

	sprintf(RPCDataElems[0].ColName,"%seqpnames.csv",Path);
	size=GetDataBaseSize(RPCDataElems[0].ColName);
	if (size>=0) sprintf(str,"Reading EQPNAMES.CSV : %d lines",size);
	else sprintf(str,"Reading EQPNAMES.CSV : %s",erlst[abs(size)]);
	//printf("%s\n",str);
	if (size<0) return abs(size);
	RPCData=(RPCDataStruct *)malloc(sizeof(RPCDataStruct)*size);
	if (RPCData==NULL) return out_of_memory;
	ptr=(char *)RPCData;
	for (i=0;i<sizeof(RPCDataStruct)*size;i++) ptr[i]=0;
	RPCDataElems[1].Address=RPCData[0].FecName;
	RPCDataElems[2].Address=RPCData[0].FullName;
	RPCDataElems[3].Address=RPCData[0].EqName;
	RPCDataElems[4].Address=&RPCData[0].EqNumber;
	RPCNumRecsRead=ReadDataBase(RPCDataElems);
	if (RPCNumRecsRead<0) return database_not_loaded;

	//printf("Initialization terminated.\n");

	return 0;
}

void FreeRPC()
{
	free((void *)sendBuffer);

	free((void *)RPCData);
	if (RPCMod!=NULL) free((void *)RPCMod);
	free((void *)RPCFec);

	free((void *)FecAccessCount);
	free((void *)FecLastAccess);
	WSACleanup();
}

void GetRPCLastError(char* buffer, short dummy)
{
	if(lasterror>0 && lasterror < 109)strcpy(buffer,erlst[lasterror]);
	else sprintf(buffer,"unknown error %d",lasterror);
}

short SyncRPC(char *Name,char *Property,char *Cycle,short Size,void *Data,short Datasize,short DataFormat,short dummy)
{
	ElemDataStruct Elem;
	USHORT i;
	short j,cc,dataindex,flag;
	char Buffer[BUFFER_SIZE+1],eqname[EQNAME_SIZE+1];
	char *values;
	double value;
	char *ptr;
	WORD connectionID;

	flag=abs(Size)/Size;
	if (strlen(Name)>ELEM_NAME_SIZE) return non_existent_elem;
	for (i=0;i<strlen(Name);i++) Elem.ElemName[i]=toupper(Name[i]);
	Elem.ElemName[i]=0;

	dataindex = SearchDataBase(RPCDataElems,Elem.ElemName,"NAME");

	CString data;
	data.Format("%d",dataindex);
	AfxMessageBox(data,MB_OK,0);

	if (dataindex < 0) 
	{
		strcpy(Buffer,"DEFAULT");
		/************************/
		cc = non_existent_fec;
		goto out2;
	}
	else 
	strcpy((char *)Buffer,RPCData[dataindex].FecName);

	Elem.Values = NULL;
	Elem.FecIndex = SearchDataBase(RPCFecElems,Buffer,"FECNAME");
	if (Elem.FecIndex < 0)
	{
		if (dataindex < 0) cc = non_existent_elem;
		else cc = non_existent_fec;
		goto out;
	}

	if (strlen(Property)>PROPERTY_SIZE) return illegal_property;
	strcpy(Elem.Property,Property);
	for (i=strlen(Elem.Property);i<PROPERTY_SIZE+1;i++) Elem.Property[i]=0;

	if (dataindex<0)
	{
		eqname[0]=0;Elem.EqNo=0;
	}
	else
	{
		strcpy((char *)eqname,RPCData[dataindex].EqName);
		Elem.EqNo=RPCData[dataindex].EqNumber;
	}
	strcpy(Elem.EqName,eqname);
	for (i=strlen(Elem.EqName);i<EQNAME_SIZE+1;i++) Elem.EqName[i]=0;
	Elem.DataFormat=DataFormat;
	if (Cycle==NULL) Elem.Cycle[0]=0;
	else strcpy(Elem.Cycle,Cycle);
	Elem.Size=abs(Size);

	if (Size<0)
	{
		switch (Elem.DataFormat)
		{
		case CF_TEXT:
			ptr=(char *)Data;

			for (i=0;i<abs(Size) && *ptr;i++)
			{
				j=0;
				while (*ptr && *ptr!='\t' && *ptr!='\r' && *ptr!='\n') Buffer[j++]=*ptr++;
				Buffer[j]=0;
				while (*ptr=='\t' || *ptr=='\r' || *ptr=='\n') ptr++;
				cc=StrToValue((double *)&value,Buffer,(char *)eqname,(char *)Property);
				if (!cc)
				{
					if (!i)
					{
						if ((Elem.Values = (char *) malloc(GetFormatSize(CF_DOUBLE)*abs(Size)))==NULL)
							{cc=out_of_memory;goto out;}
					}
					values=(char *)&value;
					for (j=0;j<GetFormatSize(CF_DOUBLE);j++)
						Elem.Values[i*GetFormatSize(CF_DOUBLE)+j]=values[j];
					Elem.DataFormat=CF_DOUBLE;
				}
				else
				{
					if (strlen((char *)Data)>=(USHORT)GetFormatSize(CF_TEXT)) {cc=dimension_error;goto out;}
					if ((Elem.Values = (char *) malloc(GetFormatSize(CF_TEXT)*abs(Size)))==NULL)
						{cc=out_of_memory;goto out;}
					strcpy(Elem.Values,(char *)Data);
					break;
				}
			}
			break;
		case CF_DOUBLE:
		case CF_FLOAT:
		case CF_SHORT:
		case CF_LONG:
		case CF_BYTE:
			if ((Elem.Values = (char *) malloc(GetFormatSize(CF_DOUBLE)*abs(Size)))==NULL)
				{cc=out_of_memory;goto out;}

			ConvertFormat(Data,DataFormat,(void *)Elem.Values,CF_DOUBLE,abs(Size));
			Elem.DataFormat=CF_DOUBLE;
			break;
		default:
			cc=illegal_format;goto out;
		}
	}

	cc=RPCConnect(0,&Elem,&connectionID);
	if (cc) goto out;

	cc=RPCSend(flag,&Elem,connectionID);
	if (cc) goto out;

	RPCReceive(flag,&Elem,connectionID);

	cc=Elem.CompCode;

	if (flag>0 && !cc && Data!=NULL)
	{
		ptr=(char *)Data;
		if (DataFormat==CF_TEXT)
		{
			if (Elem.DataFormat==CF_TEXT)
			{
				for (i=0;i<Datasize && Elem.Values[i];i++) ptr[i]=Elem.Values[i];
				ptr[i]=0;
			}
			else ValueToStr(Elem.Values,Elem.DataFormat,ptr,Datasize,
				1,abs(Size),Elem.EqName,Elem.Property,TRUE);
		}
		else ConvertFormat((void *)Elem.Values,Elem.DataFormat,Data,DataFormat,abs(Size));
	}
	if (cc<0 && Data!=NULL && DataFormat==CF_TEXT && Datasize>(short)strlen(Elem.Values))
	{
		strcpy((char *)Data,Elem.Values);
	}
out:
	if (Elem.Values!=NULL) free((void *)Elem.Values);
out2:
	lasterror = cc;
	return cc;
}

short RPCConnect(WORD wParam,ElemDataStruct *ElemPointer,WORD *connectionID)
{
	short fecindex;
	BOOL ProtOK;
	SOCKET sck;

	if (wParam<0 && abs(wParam)!=1) return illegal_format;

	fecindex=ElemPointer->FecIndex;

	NetProt=RPCFec[fecindex].Prot;

	if (FecAccessCount[fecindex]>=FEC_DIRECT_ACCESS)
	{
		if (FecAccessCount[fecindex]>=FEC_DELAYED_ACCESS) return link_disabled;
			if (time(NULL)<FecLastAccess[fecindex]+FEC_DELAY) return link_suspended;
	}
	FecLastAccess[fecindex]=time(NULL);

	ProtOK=FALSE;

	sck=socket(AF_INET,SOCK_DGRAM,0);
	if (sck>WSABASEERR)
	{
		return no_link;
	}
	int timeout = WATCH_DELAY;
	int err;
	err = setsockopt( 
			sck, 
			SOL_SOCKET, 
			SO_SNDTIMEO, 
			(char *)&timeout, 
			sizeof(timeout));

	memset(&req_sck_name,0,sizeof(req_sck_name));
	memcpy(&req_sck_name.sin_addr,(unsigned long *)RPCFec[fecindex].IP,sizeof(unsigned long));
	req_sck_name.sin_family=AF_INET;
	req_sck_name.sin_port=htons(REQUEST_PORT);

	*connectionID=sck;
	FecAccessCount[fecindex]=0;
	return 0;
}
short RPCSend(WORD Param,ElemDataStruct *lParam,WORD connectionID)
{
	char user[255];
	short cc=0,x,eqsize,count,rwflag;
	ElemDataStruct *ElemPointer;
	char *ptrs,*ptrd;

	HeaderStruct RPCHeader;
	EqpInfoStruct EqpInfo;
	BOOL remove=FALSE;
	WORD wParam;

	wParam=Param;
	if (!wParam)
	{
		wParam = 1;
		remove = TRUE;
	}

	ElemPointer=lParam;
	eqsize = abs((short)wParam);
	rwflag = eqsize/(short)wParam;

	RPCHeader.TotalSize=sizeof(HeaderStruct)+sizeof(EqpInfoStruct)*eqsize;
	if (getenv("USER")!=NULL) strcpy(user,getenv("USER"));
	else strcpy(user,"COLLAPS");
	strcpy(RPCHeader.UserName,user);
	RPCHeader.EqpSize=(short)wParam;

	RPCHeader.Protocol=1;

	if (rwflag<0)
	{
		RPCHeader.TotalSize=RPCHeader.TotalSize+
			GetFormatSize(ElemPointer->DataFormat)*(ElemPointer->Size);
	}

	ptrd=(char *) sendBuffer;
	ptrs=(char *) &RPCHeader;

	for (count=0;count<sizeof(HeaderStruct);count++) ptrd[count]=ptrs[count];

	for (x=0;x<sizeof(ElemPointer->ElemName);x++) EqpInfo.ElemName[x]=ElemPointer->ElemName[x];
	for (x=0;x<sizeof(ElemPointer->EqName);x++) EqpInfo.EqpModName[x]=ElemPointer->EqName[x];
	EqpInfo.EqpModNo=ElemPointer->EqNo;
	for (x=0;x<sizeof(ElemPointer->Property);x++) EqpInfo.Property[x]=ElemPointer->Property[x];
	strcpy(EqpInfo.Cycle,ElemPointer->Cycle);
	EqpInfo.DataFormat=ElemPointer->DataFormat;
	EqpInfo.DataSize=ElemPointer->Size;

	ptrs=(char *)&EqpInfo;
	for (x=0;x<sizeof(EqpInfoStruct);x++)
	{
		if (count==NOD_BUF_SIZE)
		{
			if (cc=NetSend(connectionID,count),cc) goto error;
			count=0;
		}
		ptrd[count++]=ptrs[x];
	}

	if (rwflag<0)
	{
		ptrs=(char *)ElemPointer->Values;
		if (ptrs==NULL) return out_of_memory;

		for (x=0;x<(ElemPointer->Size*GetFormatSize(ElemPointer->DataFormat));x++)
		{
				ptrd[count++]=ptrs[x];
				if (count==NOD_BUF_SIZE)
				{
					if (cc=NetSend(connectionID,count),cc) goto error;
					count=0;
				}
		}
	}
	if (count)
	{
		if (cc=NetSend(connectionID,count),cc) goto error;
	}
error:
	if (cc)
	{
		if (NetProt==tcp) closesocket(connectionID);
	}
	return cc;
}
short NetSend(short connectionID,short size)
{
	short s,count;
	time_t interim;

	time(&interim);
	for (count=0;count<size;)
	{
		s=sendto(connectionID,(char *)&sendBuffer[count],size-count,0,(struct sockaddr *)&req_sck_name,sizeof(req_sck_name));
		if (s<0) return link_not_open;
		if (s>0) count=count+s;
		if (time((time_t *)0)>(interim+RPC_TIMEOUT)) return net_write_error;
	}

	return 0;
}
short GetFormatSize(short format)
{
	switch (format)
	{
	case CF_DOUBLE:return sizeof(double);
	case CF_SHORT:return sizeof(short);
	case CF_BYTE:return sizeof(BYTE);
	case CF_LONG:return sizeof(long);
	case CF_FLOAT:return sizeof(float);
	case CF_TEXT:return 500*sizeof(char);
	}
	return 0;
}
void RPCReceive(WORD wParam,ElemDataStruct *lParam,WORD connectionID)
{
	short i,datasize,done,actseq,seqnr,cc,el,ind,flag,eqsize,count,s,msgsize;
	short fecindex;
	time_t interim;
	ElemDataStruct *ElemPointer;
	int l;
	char str[NOD_BUF_SIZE+1],*msgbuf;

	msgbuf=NULL;
	eqsize=abs((short)wParam);
	flag=(short)wParam;
	ElemPointer=lParam;
	fecindex=ElemPointer->FecIndex;

	el=0;ind=0;cc=0;

	done=0;actseq=0;time(&interim);seqnr=-1;cc=0;

	time(&interim);

	while (1)
	{
			l=sizeof(req_sck_name);
			s=recvfrom(connectionID,(char *)str,NOD_BUF_SIZE,0,(struct sockaddr *)&req_sck_name,&l);
			if (s>0) break;

			if (time((time_t *)0)>(interim+RPC_TIMEOUT))
			{
				FecAccessCount[fecindex]++;
				cc=link_not_open;goto outtcp;
			}
	}
	sendBuffer[0]=CHECK_CHAR;NetSend(connectionID,1);
	msgsize=*(short *)str;
	msgbuf=(char *)malloc(msgsize);

	if (msgbuf==NULL) {cc=out_of_memory;goto outtcp;}
	if (msgsize<=NOD_BUF_SIZE) count=msgsize;else count=NOD_BUF_SIZE;
	for (i=2;i<count;i++) msgbuf[i-2]=str[i];

	time(&interim);
	while (count<msgsize)
	{
			l=sizeof(req_sck_name);
			s=recvfrom(connectionID,(char *)&msgbuf[count-2],msgsize-count,0,(struct sockaddr *)&req_sck_name,/*(short *)*/&l);
			if (s>0)
			{
				sendBuffer[0]=CHECK_CHAR;
				NetSend(connectionID,1);
				count=count+s;
				time(&interim);
			}
			if (time((time_t *)0)>(interim+RPC_TIMEOUT))
			{
				FecAccessCount[fecindex]++;
				cc=link_not_open;goto outtcp;
			}
	}
outtcp:
	closesocket(connectionID);

	count=0;msgsize=msgsize-2;

		if (cc) ElemPointer->CompCode=cc;
		else
		{
		ElemPointer->CompCode=*(short *)&msgbuf[count];
		count=count+sizeof(short);

		if (ElemPointer->CompCode<=0 && (count+2*sizeof(short))<=(USHORT)msgsize)
		{
			ElemPointer->DataFormat=*(short *)&msgbuf[count];
			count=count+sizeof(short);
			datasize=*(short *)&msgbuf[count];
			count=count+sizeof(short);

			if (ElemPointer->Values!=NULL) free(ElemPointer->Values);
			ElemPointer->Values=(char *)malloc(datasize);
			if (ElemPointer->Values==NULL)
			{
				ElemPointer->CompCode=out_of_memory;
				count=count+datasize;
			}
			else
			{
				for (i=0;i<datasize;i++) ElemPointer->Values[i]=msgbuf[count++];
			}
		}
		}

	if (msgbuf!=NULL) free((void *)msgbuf);
}

void ConvertFormat(void *sdata,short sformat,void *ddata,short dformat,short Size)
{
		double    *dptrs,*dptrd;
		short       *iptrs,*iptrd;
		char      *bptrs,*bptrd;
		long      *lptrs,*lptrd;
		float     *fptrs,*fptrd;
		short i;

		switch (dformat)
		{
		case CF_DOUBLE:
			dptrd=(double *)ddata;

			switch (sformat)
			{
			case CF_DOUBLE:
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
				iptrs=(short *)sdata;
				for (i=0;i<Size;i++) dptrd[i]=(double)iptrs[i];
				break;
			case CF_BYTE:
				bptrs=(char *)sdata;
				for (i=0;i<Size;i++) dptrd[i]=(double)bptrs[i];
				break;
			}
			break;
		case CF_FLOAT:
			fptrd=(float *)ddata;

			switch (sformat)
			{
			case CF_DOUBLE:
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
				iptrs=(short *)sdata;
				for (i=0;i<Size;i++) fptrd[i]=(float)iptrs[i];
				break;
			case CF_BYTE:
				bptrs=(char *)sdata;
				for (i=0;i<Size;i++) fptrd[i]=(float)bptrs[i];
				break;
			}
			break;
		case CF_LONG:
			lptrd=(long *)ddata;

			switch (sformat)
			{
			case CF_DOUBLE:
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
				iptrs=(short *)sdata;
				for (i=0;i<Size;i++) lptrd[i]=(long)iptrs[i];
				break;
			case CF_BYTE:
				bptrs=(char *)sdata;
				for (i=0;i<Size;i++) lptrd[i]=(long)bptrs[i];
				break;
			}
			break;
		case CF_SHORT:
			iptrd=(short *)ddata;

			switch (sformat)
			{
			case CF_DOUBLE:
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
				iptrs=(short *)sdata;
				for (i=0;i<Size;i++) iptrd[i]=iptrs[i];
				break;
			case CF_BYTE:
				bptrs=(char *)sdata;
				for (i=0;i<Size;i++) iptrd[i]=(short)bptrs[i];
				break;
			}
			break;
		case CF_BYTE:
			bptrd=(char *)ddata;

			switch (sformat)
			{
			case CF_DOUBLE:
				dptrs=(double *)sdata;
				for (i=0;i<Size;i++) bptrd[i]=(char)dptrs[i];
				break;
			case CF_FLOAT:
				fptrs=(float *)sdata;
				for (i=0;i<Size;i++) bptrd[i]=(char)fptrs[i];
				break;
			case CF_LONG:
				lptrs=(long *)sdata;
				for (i=0;i<Size;i++) bptrd[i]=(char)lptrs[i];
				break;
			case CF_SHORT:
				iptrs=(short *)sdata;
				for (i=0;i<Size;i++) bptrd[i]=(char)iptrs[i];
				break;
			case CF_BYTE:
				bptrs=(char *)sdata;
				for (i=0;i<Size;i++) bptrd[i]=bptrs[i];
				break;
			}
			break;
		}
}
short ValueToStr(char *Values,short DataFormat,char *ValDesc,short Datasize,short RowNum,short ColNum,char *EqName,char *Property,BOOL Formatting)
{
	short ind,i,stroff;
	char Buffer[BUFFER_SIZE+1];
	double value;
	short col,row,cl=-1;

	stroff=0;ValDesc[0]=0;
	if (strcmp(Property,(char *)"STAQ") && strcmp(Property,(char *)"STCC"))
	{
	  if (DataFormat==CF_TEXT)
	  {
		for (i=0;Values[i] && i<Datasize-1;i++) ValDesc[i]=Values[i];
		ValDesc[i]=0;
		return cl;
	  }
	  for (row=0;row<RowNum;row++)
	  {
		for (col=0;col<ColNum;col++)
		{
			ind=col+row*ColNum;
			ConvertFormat((void *)&Values[ind*GetFormatSize(DataFormat)],DataFormat,
					(void *)&value,CF_DOUBLE,1);
			if (!strcmp(Property,(char *)"STAQ") || !strcmp(Property,(char *)"STCC"))
				sprintf(Buffer,"%d",(short)value);
			else
			{
				if (Formatting) sprintf(Buffer,"%lg",value);else sprintf(Buffer,"%lf",value);
			}
			if ((stroff+strlen(Buffer))>=(USHORT)Datasize) return cl;
			strcpy(&ValDesc[stroff],(char *)Buffer);
			stroff=strlen(ValDesc);
			if (stroff)
			{
				if (col==ColNum-1) ValDesc[stroff++]='\n';
				else ValDesc[stroff++]='\t';
				ValDesc[stroff]=0;
			}
		}
	  }
	}
	else
	{
		i=0;
		while (i<RPCNumEqpsRead && strcmp(RPCMod[i].EqpName,EqName)) i++;
		value=*(double *)Values;
		if (i>=RPCNumEqpsRead)
		{
			sprintf(Buffer,"%lg",value);
			strcpy(ValDesc,(char *)Buffer);
			return cl;
		}
		while (i<RPCNumEqpsRead && !strcmp(RPCMod[i].EqpName,EqName))
		{
			if ((RPCMod[i].Weight & ((short)value)) ||
				(!RPCMod[i].Weight && !(short)value))
			{
				if ((strlen(ValDesc)+strlen(RPCMod[i].StatName)+3)>=(USHORT)Datasize)
				{
					if ((strlen(ValDesc)+4)<=(USHORT)Datasize) stroff=strlen(ValDesc);
					else stroff=strlen(ValDesc)-4;
					strcpy(&ValDesc[stroff],(char *)" ->");
					return cl;
				}
				if (strlen((char *)&ValDesc[stroff])) strcat(ValDesc,(char *)" + ");
				else cl=i;
				strcat(ValDesc,RPCMod[i].StatName);
			}
			i++;
		}
		stroff=strlen(ValDesc);
		if (stroff) ValDesc[stroff++]=9;ValDesc[stroff]=0;
	}
	if (stroff && (ValDesc[stroff-1]=='\t' || ValDesc[stroff-1]=='\n')) ValDesc[stroff-1]=0;
	return cl;
}
short StrToValue(double *Value,char *Command,char *EqName,char *Property)
{
	short i,x;
	double value;

	value=atof(Command);
	if (value)
	{
		*Value=value;
		return 0;
	}

	if (!strcmp(Property,(char *)"STAQ") || !strcmp(Property,(char *)"STCC"))
	{
		i=0;
		while (i<RPCNumEqpsRead && strcmp(RPCMod[i].EqpName,EqName)) i++;
		if (i<RPCNumEqpsRead)
		{
			x=0;
			while ((strcmp(RPCMod[i+x].EqpName,EqName)
				|| strcmp(RPCMod[i+x].GlobalCom,(char *)Command)) &&
					(i+x)<RPCNumEqpsRead) x++;
			if ((i+x)<RPCNumEqpsRead)
			{
				*Value=(double)RPCMod[i+x].Weight;
				return 0;
			}
		}
	}
	for (i=-1,x=0;Command[x];x++)
	{
		if (Command[x]>='0' && Command[x]<='9')
		{
			i=x;
			break;
		}
	}
	if (i!=-1) value=atof(&Command[i]);else return illegal_command;

	*Value=value;
	return 0;
}
WORD IntSwap(WORD w)
{
	char *ptrs,*ptrd;
	WORD r;

	ptrs=(char *)&w;
	ptrd=(char *)&r;
	ptrd[0]=ptrs[1];
	ptrd[1]=ptrs[0];

	return r;
}




























