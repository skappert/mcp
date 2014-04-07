CString	ReadString		  (CArchive& ar);
double	ReadFloat		  (CArchive& ar);
char	ReadChar		  (CArchive& ar);
char	ReadNameBegin	  (CArchive& ar);
char	ReadNameEnd		  (CArchive& ar);
char	ReadControlBegin  (CArchive& ar);
char	ReadControlEnd	  (CArchive& ar);
char	ReadDataFileBegin (CArchive& ar);
char	ReadDataFileEnd	  (CArchive& ar);
char	ReadStructBegin	  (CArchive& ar);
char	ReadStructEnd	  (CArchive& ar);
char	ReadSeparator	  (CArchive& ar);

void WriteString(CArchive& ar,CString TheString);
void WriteLine(CArchive& ar);
void WriteFloat(CArchive& ar,double TheFloat);
void WriteChar(CArchive& ar, char TheChar);
void WriteNameBegin(CArchive& ar);
void WriteNameEnd(CArchive& ar);
void WriteControlBegin(CArchive& ar);
void WriteControlEnd(CArchive& ar);
void WriteDataFileBegin(CArchive& ar);
void WriteDataFileEnd(CArchive& ar);
void WriteStructBegin(CArchive& ar);
void WriteStructEnd(CArchive& ar);
void WriteSeparator(CArchive& ar);

