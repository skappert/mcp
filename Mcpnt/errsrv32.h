#define ERRORSRV_PORT		8099
#define MAX_MESSAGE_SIZE	1024

#define KNOBSRV_PORT		8098
#define BUFFER_SIZE          100

typedef struct
{
	char FecName[20];
	char FullName[20];
	char EqName[16];
	short EqNumber;
	char Type[25];
	char Unit[10];
	char Increment[10];
	char Control[40];
}RPCDataStruct;

#define KNOB_MESSAGE	WM_USER+2
