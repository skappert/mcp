#define DLLExport _declspec(dllexport)

DLLExport BOOL WINAPI GetInitialPars(int ParNumber, char* ParName, double* Value, double* Error);
DLLExport double WINAPI Userfitfunction(double x, double par[]);
DLLExport CString WINAPI UserfitfunctionInfo();