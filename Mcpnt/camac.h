#define SENDF		(UCHAR)0x01
#define SENDNAF		(UCHAR)0x02
#define SENDDNAF	(UCHAR)0x03
#define READD		(UCHAR)0x04
#define READCSR		(UCHAR)0x05
#define READENCL	(UCHAR)0x06
#define READGPIB	(UCHAR)0x07
#define WAITINT		(UCHAR)0x10

#define COMPAREEQUAL		(UCHAR)0x20
#define COMPAREATLEAST		(UCHAR)0x40
#define COMPARELESSTHAN		(UCHAR)0x60

// Direkter IO-Zugriff ; adr ist Offset zur 
// Basisaddresse, die in der Registry unter GenPort
// eingetragen ist (änderung der Basisaddresse 
// ebenfalls dort.

// Bei allen Send-Befehlen wird der Erfolg mit TRUE/FALSE angezeigt
 BOOL	__stdcall DllMain(HINSTANCE hinstDll,DWORD fwdReason,LPVOID lpvReserved);

 ULONG   ReadLongIO(ULONG adr);
 USHORT  SendLongIO(ULONG adr,ULONG data);
 USHORT  ReadWordIO(ULONG adr);
 USHORT  SendWordIO(ULONG adr,USHORT data);
 UCHAR   ReadByteIO(ULONG adr);
 USHORT  SendByteIO(ULONG adr,UCHAR data);

// Die folgenden Befehle gelten nur für HYTEC 1330,1331
// und kompatible

 USHORT  SendDNAFCamac(ULONG d,UCHAR n,UCHAR a, UCHAR f);
 USHORT  SendDCamac(ULONG d);
 USHORT  SendNAFCamac(UCHAR n,UCHAR a, UCHAR f);
 USHORT  SendFCamac(UCHAR f);
 USHORT  RepeatLastCamac(void);

 ULONG   ReadDCamac(void);
 UCHAR   ReadENCLCamac(void);
 UCHAR   ReadCSRCamac(void);

 USHORT  LAMEnableCamac(UCHAR a);
 USHORT  LAMDisableCamac(UCHAR a);
 UCHAR   GetLAMCamac(UCHAR a);

 ULONG	 ListDNAFCamac(ULONG type,ULONG d,UCHAR n,UCHAR a, UCHAR f);
 USHORT  ListStart(void);
 USHORT  ListReset(void);
 USHORT  DataReset(void);
 ULONG   ListReadd(ULONG* data);
 ULONG   ListReadS(char* data, USHORT* length);
 ULONG   ShowListPtr(void);
 ULONG   ShowActualListPtr(void);
 ULONG   ShowDataPtr(void);
 ULONG   ShowActualDataPtr(void);
 ULONG   ShowStringPtr(void);
 ULONG   ShowActualStringPtr(void);
 BOOL	 GPIBReady(void);
 BOOL    ListReady(void);

 ULONG   ListDelayCamac(USHORT slot, USHORT delay);

 BOOL    DriverIsExisting(void);
