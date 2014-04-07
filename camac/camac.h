#define CAMAC _declspec(dllexport)

#define SENDF		(UCHAR)0x01
#define SENDNAF		(UCHAR)0x02
#define SENDDNAF	(UCHAR)0x03
#define READD		(UCHAR)0x04
#define READCSR		(UCHAR)0x05
#define READENCL	(UCHAR)0x06
#define READGPIB	(UCHAR)0x07
#define WAITINT		(UCHAR)0x10

// Direkter IO-Zugriff ; adr ist Offset zur 
// Basisaddresse, die in der Registry unter GenPort
// eingetragen ist (änderung der Basisaddresse 
// ebenfalls dort.

// Bei allen Send-Befehlen wird der Erfolg mit TRUE/FALSE angezeigt

CAMAC ULONG  WINAPI ReadLongIOCamac(ULONG adr);
CAMAC USHORT WINAPI SendLongIOCamac(ULONG adr,ULONG data);
CAMAC USHORT WINAPI ReadWordIOCamac(ULONG adr);
CAMAC USHORT WINAPI SendWordIOCamac(ULONG adr,USHORT data);
CAMAC UCHAR  WINAPI ReadByteIOCamac(ULONG adr);
CAMAC USHORT WINAPI SendByteIOCamac(ULONG adr,UCHAR data);

// Die folgenden Befehle gelten nur für HYTEC 1330,1331
// und kompatible

CAMAC USHORT WINAPI SendDNAFCamac(ULONG d,UCHAR n,UCHAR a, UCHAR f);
CAMAC USHORT WINAPI SendDCamac(ULONG d);
CAMAC USHORT WINAPI SendNAFCamac(UCHAR n,UCHAR a, UCHAR f);
CAMAC USHORT WINAPI SendFCamac(UCHAR f);
CAMAC USHORT WINAPI RepeatLastCamac(void);

CAMAC ULONG  WINAPI ReadDCamac(void);
CAMAC UCHAR  WINAPI ReadENCLCamac(void);
CAMAC UCHAR  WINAPI ReadCSRCamac(void);

CAMAC USHORT WINAPI LAMEnableCamac(UCHAR a);
CAMAC USHORT WINAPI LAMDisableCamac(UCHAR a);
CAMAC UCHAR  WINAPI GetLAMCamac(UCHAR a);

CAMAC USHORT WINAPI ListDNAFCamac(ULONG type,ULONG d,UCHAR n,UCHAR a, UCHAR f);
CAMAC USHORT WINAPI ListStart(void);
CAMAC USHORT WINAPI ListReset(void);
CAMAC USHORT WINAPI DataReset(void);
CAMAC ULONG  WINAPI ListReadd(ULONG* data);
CAMAC ULONG  WINAPI ListReadS(char* data, USHORT* length);
CAMAC ULONG  WINAPI ShowListPtr(void);
CAMAC ULONG  WINAPI ShowActualListPtr(void);
CAMAC ULONG  WINAPI ShowDataPtr(void);
CAMAC ULONG  WINAPI ShowActualDataPtr(void);
CAMAC ULONG  WINAPI ShowStringPtr(void);
CAMAC ULONG  WINAPI ShowActualStringPtr(void);
CAMAC BOOL	 WINAPI GPIBReady(void);
CAMAC BOOL   WINAPI ListReady(void);

CAMAC ULONG  WINAPI ListDelayCamac(USHORT slot, USHORT delay);

CAMAC BOOL   WINAPI DriverIsExisting(void);
