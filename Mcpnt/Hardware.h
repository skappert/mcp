#define UNL 63
#define UNT 79
BOOL ListSendGPIB(CString Command,int Address,int CamacSlot);
BOOL ListSendReadGPIB(CString Command,int Address,int CamacSlot);
BOOL SendGPIB(CString Command,int Address,int CamacSlot);
void OnBit(USHORT Slot,USHORT Bit);
void OffBit(USHORT Slot,USHORT Bit);
void ToggleBit(USHORT Slot,USHORT Bit);
void ListOnBit(USHORT Slot,USHORT Bit);
void ListOffBit(USHORT Slot,USHORT Bit);
void ListToggleBit(USHORT Slot,USHORT Bit);
void CamacReset();
void DelayCamac(USHORT delay);
CString SendReadGPIB(CString Command,int Address,int CamacSlot);

void ListArmTrigger(int Slot);
void ListDeArmTrigger(int Slot);
void ListLoadTimer(int Slot,double Time);
void ListLoadTimerWait(int Slot,double Time);
void ListStartTimer(int Slot);
void ListEnableLAMTimer(int Slot);
void ListDisableLAMTimer(int Slot);
void ListResetLAMTimer(int Slot);
void LoadTimer(int Slot,double Time);
void StartTimer(int Slot);
void EnableLAMTimer(int Slot);
void DisableLAMTimer(int Slot);
void ResetLAMTrigger(int Slot);
void EnableLAMTrigger(int Slot);
void DisableLAMTrigger(int Slot);
void ResetLAMTimer(int Slot);

