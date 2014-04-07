#define CF_PRIVATEFIRST     0x0200
#define CF_TEXT     1
#define CF_DOUBLE   CF_PRIVATEFIRST
#define CF_INTEGER  CF_PRIVATEFIRST+1
#define CF_BYTE     CF_PRIVATEFIRST+2
#define CF_LONG     CF_PRIVATEFIRST+3
#define CF_SHORT    CF_PRIVATEFIRST+1
#define CF_FLOAT    CF_PRIVATEFIRST+5

#define RPC_WRITE   -1
#define RPC_READ     1

short InitRPC(char *PathEntry);
void FreeRPC();
void GetRPCLastError(char* buffer,short dummy);
short SyncRPC(char *Name,char *Property,char *Cycle,short Size,void *Data,short Datasize,short DataFormat, short dummy);

