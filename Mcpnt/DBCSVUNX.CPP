#include "stdafx.h"
#include "MCP for NT.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "dbcsvunx.h"
#include "errors.h"

#define LOINT(l)       ((short)(l))
#define HIINT(l)       ((short)(((long)(l) >> 16) & 0xFFFF))

short ReadString(FILE *fp,char *str,short *lower);
short NextLine(FILE *fp,char *str,short *lower);
short fstrcmp(char *str1,char *str2);
short fstrcmpi(char *str1,char *str2);
void fstrcpy(char *str1,char *str2);
short fatoi(char *fstr);
short fstrlen(char *fstr);
void fstrcat(char *dest,char *src);
unsigned long HexToDec(char *str);

short ReadString(FILE *fp,char *str,short *lower)
{
	char c,ch,*string;
	short cc;

	string=str;*lower=0;
	while (1)
	{
		cc=fread(&c,1,1,fp);
		ch=toupper(c);
		if (c!=ch) *lower=1;
		if (ch!=',' && ch!=10 && cc==1)
		{
			*string=ch;string++;
		}
		else break;
	}
	*string=0;
	if (cc==1) cc=ch;else cc=-1;
	return cc;
}


short NextLine(FILE *fp,char *str,short *lower)
{
	short cc;

	do
	{
		do
		{
			cc=ReadString(fp,str,lower);
		}
		while (cc==',');
		if (cc>=0) cc=ReadString(fp,str,lower);
	}
	while (cc>0 && str[0]=='%');
	return cc;
}



short ReadDataBase(DataBaseElemsStruct *elems)
{
	FILE *DataBaseFP;
	char str[100];
	void *addr;
	short cc,e,col,i,j,recsread;
	short error;

	short nrlines,*colflags,size;
	short msb,lsb,lower;
	char *file;
	short in;
	double r;

	recsread=0;
	nrlines=0;while (strlen(elems[nrlines+1].ColName)) nrlines++;
	size=LOINT(elems[0].Type);
	file=elems[0].ColName;
	colflags=(short *)malloc(nrlines*sizeof(short));

	for (i=0;i<nrlines;i++) colflags[i]=(-1);
	DataBaseFP=fopen(file,"r");
	if (!DataBaseFP) { error = -no_such_file; goto err;};
	col=0;
	do
	{
		cc=ReadString(DataBaseFP,str,&lower);
		if (!col && str[0]=='%') cc=NextLine(DataBaseFP,str,&lower);
		i=0;
		while ((fstrcmp((char *)str,(char *)elems[i+1].ColName) || colflags[i]!=-1)
					&& i<nrlines) i++;
		if (i<nrlines)
		{
			colflags[i]=col;
		}
		col++;
	}
	while (cc==',');
/*
	for (i=0;i<nrlines;i++)
		if (colflags[i]<0) { error = -non_existent; goto err;};
*/
	while (1)
	{
		col=0;
		do
		{
			cc=ReadString(DataBaseFP,str,&lower);
			if (!col && str[0]=='%') cc=NextLine(DataBaseFP,str,&lower);
			if (cc<0 && (!strlen(str) || str[0]=='%')) break;
			for (i=0;i<nrlines;i++)
				if (colflags[i]==col)
			{
				addr=(void *)((char *)(elems[i+1].Address)+recsread*size);
				switch (LOINT(elems[i+1].Type))
				{
				case STR:fstrcpy((char *)addr,(char *)str);break;
				case INT:
					if (str[strlen(str)-1]=='H')
					{
						str[strlen(str)-1]=0;
						in=(short)HexToDec(str);
					}
					else in=atoi(str);
					*(short *)addr=in;break;
				case FLT:
					if (str[strlen(str)-1]=='H')
					{
						str[strlen(str)-1]=0;
						r=(double)HexToDec(str);
					}
					else r=atof(str);
					*(double *)addr=r;break;
				case HEX:
					e=strlen(str)/2;
					for (j=0;j<e;j++)
					{
						msb=toupper(str[j*2]);lsb=toupper(str[j*2+1]);
						if (msb>64) msb=msb-55;else msb=msb-48;
						if (lsb>64) lsb=lsb-55;else lsb=lsb-48;
						*(unsigned char *)((char *)(addr)+j)=(unsigned char)msb*16+lsb;
					}
					break;
				}
			}
			col++;
		}
		while (cc==',');
		if (cc<0) break;
		recsread++;
	}
	fclose(DataBaseFP);
	elems[0].Type=size+65536*(unsigned long)recsread;
	free(colflags);
	return recsread;
err:
	fclose(DataBaseFP);
	free(colflags);
	return error;
}




short SearchDataBase(DataBaseElemsStruct *elems,char *searchstr,char *colname)
{
	short c,e,i,line,recs,size;
	char *p1,*p2,*p3;

	recs=HIINT(elems[0].Type);
	size=LOINT(elems[0].Type);
	line=1;
	while (fstrcmp((char *)colname,(char *)elems[line].ColName) && strlen(elems[line].ColName)) line++;
	if (!strlen(elems[line].ColName)) return -non_existent;
	i=-1;
	p2=(char *)elems[line].Address;
	p3=searchstr;
	do
	{
		i++;p1=p3;
		c=0;
		do
		{
			e=(*p1!=*(p2+c));
			c++;p1++;
		}
		while (!e && (*p1 || *(p2+c)));
		p2=p2+size;
	}
	while (e && i<recs-1);
	if (e) return -non_existent;
	return i;
}



short fstrcmp(char *str1,char *str2)
{
	short i;

	i=0;
	while (*(str1+i) || *(str2+i))
	{
		if (*(str1+i)!=*(str2+i)) return 1;
		i++;
	}
	return 0;
}

short fstrcmpi(char *str1,char *str2)
{
	short i;

	i=0;
	while (*(str1+i) || *(str2+i))
	{
		if (toupper((short)*(str1+i))!=toupper((short)*(str2+i))) return 1;
		i++;
	}
	return 0;
}


void fstrcpy(char *str1,char *str2)
{
	short i;

	i=0;
	while (*(str2+i))
	{
		*(str1+i)=*(str2+i);
		i++;
	}
	*(str1+i)=0;
}

short fatoi(char *fstr)
{
	char str[80];

	fstrcpy((char *)str,fstr);
	return (atoi(str));
}
short fstrlen(char *fstr)
{
	short i;

	for (i=0;fstr[i];i++);

	return i;
}
void fstrcat(char *dest,char *src)
{
	short i,j;

	for (i=0;dest[i];i++);

	for (j=0;src[j];i++,j++) dest[i]=src[j];
	dest[i]=0;
}

short GetDataBaseProperties(char *properties,short size,char *file)
{
	FILE *DataBaseFP;
	char str[50];
	short off,cc,col;
	short lower;

	DataBaseFP=fopen(file,"r");
	if (!DataBaseFP) return -no_such_file;
	col=0;off=0;
	do
	{
		cc=ReadString(DataBaseFP,str,&lower);
		if (!col && str[0]=='%') cc=NextLine(DataBaseFP,str,&lower);
		if (!lower && strlen(str))
		{
			strcpy(properties+off,str);
			off=off+size;
		}
		col++;
	}
	while (cc==',');

	fclose(DataBaseFP);
	return (short)(off/size);
}



unsigned long HexToDec(char *str)
{
	short i,d;
	unsigned long off,value;

	off=1;value=0;
	for (i=strlen(str)-1;i>=0;i--)
	{
		if (str[i]>='A') d=str[i]-'A'+10;else d=str[i]-'0';
		value=value+off*d;
		off=off*16;
	}
	return value;
}



short SearchDataBaseFromFile(char *filename,char *searchstr,char *colname,
		char *datacol,short datacollen,char *data,short datalen)
{
	FILE *DataBaseFP;
	char str[100];
	short cc,col,colnrsearch,colnrdata[100],lower,found,maxdatacol,x;

	DataBaseFP=fopen(filename,"r");
	if (!DataBaseFP) return -no_such_file;

	maxdatacol=0;while (strlen(&datacol[maxdatacol*datacollen])) maxdatacol++;
	col=0;colnrsearch=-1;for (x=0;x<100;x++) colnrdata[x]=-1;
	do
	{
		cc=ReadString(DataBaseFP,str,&lower);
		if (!col && str[0]=='%') cc=NextLine(DataBaseFP,str,&lower);
		if (!strcmp(str,colname)) colnrsearch=col;
		for (x=0;x<maxdatacol;x++)
			if (!strcmp(str,&datacol[x*datacollen])) colnrdata[x]=col;
		col++;
	}
	while (cc==',');
	if (colnrsearch==-1) { fclose(DataBaseFP); return -non_existent;};
	for (x=0;x<maxdatacol;x++)
		if (colnrdata[x]==-1) { fclose(DataBaseFP);return -non_existent;};

	found=0;
	while (1)
	{
		col=0;
		do
		{
			cc=ReadString(DataBaseFP,str,&lower);
			if (!col && str[0]=='%') cc=NextLine(DataBaseFP,str,&lower);
			if (cc<0 && (!strlen(str) || str[0]=='%')) break;
			if (col==colnrsearch && !strcmp(str,searchstr)) found=1;
			for (x=0;x<maxdatacol;x++)
				if (col==colnrdata[x]) strcpy(&data[x*datalen],str);
			col++;
		}
		while (cc==',');
		if (cc<0 || found) break;
	}
	fclose(DataBaseFP);
	if (found) return 0;else return -non_existent;
}



short GetDataBaseSize(char *filename)
{
	FILE *DataBaseFP;
	char str[80];
	short cc,col,line,lower;

	DataBaseFP=fopen(filename,"r");
	if (!DataBaseFP) return -no_such_file;
	line=-1;
	while (1)
	{
		col=0;
		do
		{
			cc=ReadString(DataBaseFP,str,&lower);
			if (!col && str[0]=='%') cc=NextLine(DataBaseFP,str,&lower);
			if (cc<0 && (!strlen(str) || str[0]=='%')) break;
			col++;
		}
		while (cc==',');
		if (cc<0) break;
		line++;
	}
	fclose(DataBaseFP);
	return line;
}
