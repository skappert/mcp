/* RPCServer Definitions */

#define CF_DOUBLE   CF_PRIVATEFIRST
#define CF_SHORT    CF_PRIVATEFIRST+1
#define CF_BYTE     CF_PRIVATEFIRST+2
#define CF_LONG     CF_PRIVATEFIRST+3
#define CF_FLOAT    CF_PRIVATEFIRST+5
#define CF_BI		CF_PRIVATEFIRST+6
#define CF_AI		CF_PRIVATEFIRST+7
#define CF_BR		CF_PRIVATEFIRST+8
#define CF_AR		CF_PRIVATEFIRST+9

#ifdef __cplusplus
#define PRFX extern "C"
#else
#define PRFX extern 
#endif

PRFX void FAR PASCAL  FreeRPCAPI();
PRFX short FAR PASCAL SyncRPC(const char *Name,const char *Property,const char *Cycle,short ArraySize,void *Data,long DataSize,short DataFormat,short DataType);
PRFX short FAR PASCAL CreateRPCHotlink(HWND hWnd,const char *Name,const char *Property,const char *Cycle,long RefreshPeriod,short ArraySize,short SubScrID,short DataType);
PRFX short FAR PASCAL GetRPCHotlinkIDs(short *SubScrIDs);
PRFX short FAR PASCAL GetRPCHotlinkData(short SubScrID,void *Data,short DataSize,short DataFormat,short *CycleNo,char *User);
PRFX short FAR PASCAL RemoveRPCHotlink(HWND hWnd,short SubScrID);

PRFX short FAR PASCAL GetRPCLastError(char *ErrMsg,short ErrStrLen);

/* ErrorServer Definitions */

PRFX short FAR PASCAL RegisterError(const char *fapplicationname,short errorvalue,const char *fdesc,BOOL activate);

/* KnobServer definitions */

PRFX short FAR PASCAL KnobAttach(HWND hwnd,const char *Name);
PRFX short FAR PASCAL KnobRemove(HWND hwnd,const char *Name);
