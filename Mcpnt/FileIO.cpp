#include "stdafx.h"
#include "MCP for NT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static BOOL FIRST=FALSE;
static unsigned char lastChar;

CString ReadString(CArchive& ar)
{
	CString TheString;
	char    String[255];
	unsigned char    TheChar;
	int     Index=0;
	do
	{
		ar >> TheChar;
	}
	while (TheChar!='"')/**/;
	do
	{
		ar >> TheChar;
		String[Index]=TheChar;	
		++Index;

	}
	while (TheChar!='"')/**/;
	String[Index-1]=0;
	TheString=String;
	FIRST=FALSE;
	return TheString;
}

double ReadFloat(CArchive& ar)
{
	double TheFloat;
	CString TheString;
	char    String[255];

	unsigned char    TheChar;
	int     Index=1;

	do
	{
		if (FIRST)
		{
			FIRST=FALSE;
			TheChar=lastChar;
		}
		else ar >> TheChar;
	}
	while((!isdigit(TheChar))&&(TheChar!='-'));
	String[0]=TheChar;	
	do
	{
		ar >> TheChar;
		String[Index]=TheChar;	
		++Index;
	}
	while ((TheChar=='-')||(TheChar=='E')||(TheChar=='e')||(TheChar=='.')||isdigit(TheChar))/**/;
	lastChar=TheChar;
	FIRST=TRUE;
	String[Index-1]=0;
	TheString=String;
	TheFloat=atof((LPCSTR)TheString);
	return TheFloat;
}

char ReadChar(CArchive& ar)
{
	unsigned char TheChar;

	do
	{
		if (FIRST)
		{
			FIRST   = FALSE;
			TheChar = lastChar;
		}
		else ar >> TheChar;
	}
	while (iscntrl(TheChar))/**/;

	return TheChar;
}

char ReadNameBegin(CArchive& ar)
{
	unsigned char TheChar;
	do
	{
		TheChar=ReadChar(ar);
	}while (TheChar!='[');
	return 0;
}
char ReadNameEnd(CArchive& ar)
{
	unsigned char TheChar;
	do
	{
		TheChar=ReadChar(ar);
	}while (TheChar!=']');
	return 0;
}
char ReadControlBegin(CArchive& ar)
{
	unsigned char TheChar;
	do
	{
		TheChar=ReadChar(ar);
	}while (TheChar!='(');
	return 0;;
}
char ReadControlEnd(CArchive& ar)
{
	unsigned char TheChar;
	do
	{
		TheChar=ReadChar(ar);
	}while (TheChar!=')');
	return 0;
}
char ReadDataFileBegin(CArchive& ar)
{
	return ReadChar(ar);
}
char ReadDataFileEnd(CArchive& ar)
{
	return ReadChar(ar);
}
char ReadStructBegin(CArchive& ar)
{
	unsigned char TheChar;
	do
	{
		TheChar=ReadChar(ar);
	}while (TheChar!='<');
	return 0;
}
char ReadStructEnd(CArchive& ar)
{
	unsigned char TheChar;
	do
	{
		TheChar=ReadChar(ar);
	}while (TheChar!='>');
	return 0;
}
char ReadSeparator(CArchive& ar)
{
	return ReadChar(ar);
}


/***************  Write-Functions  ***************/

void WriteString(CArchive& ar,CString TheString)
{
	USHORT i;
	ar << (UCHAR)'"';
	for(i=0;i<TheString.GetLength();i++)
	{
		ar <<(UCHAR)TheString.GetAt(i);
	}
	ar << (UCHAR)'"';
}

void WriteLine(CArchive& ar)
{
	ar <<(UCHAR)13;
	ar <<(UCHAR)10;
}
void WriteFloat(CArchive& ar,double TheFloat)
{
	USHORT i;
	CString TheString;
	TheString.Format("%.8g",TheFloat);
	for(i=0;i<TheString.GetLength();i++)
	{
		ar <<(UCHAR)TheString.GetAt(i);
	}
}

void WriteChar(CArchive& ar, char TheChar)
{
	ar << (UCHAR)TheChar;
}

void WriteNameBegin(CArchive& ar)
{
	ar << (UCHAR)'[';
}
void WriteNameEnd(CArchive& ar)
{
	ar << (UCHAR)']';
}
void WriteControlBegin(CArchive& ar)
{
	ar << (UCHAR)'(';
}
void WriteControlEnd(CArchive& ar)
{
	ar << (UCHAR)')';
}
void WriteDataFileBegin(CArchive& ar)
{
	ar << (UCHAR)'@';
}
void WriteDataFileEnd(CArchive& ar)
{
	ar << (UCHAR)'@';
}
void WriteStructBegin(CArchive& ar)
{
	ar << (UCHAR)'<';
}
void WriteStructEnd(CArchive& ar)
{
	ar << (UCHAR)'>';
}
void WriteSeparator(CArchive& ar)
{
	ar << (UCHAR)',';
}
