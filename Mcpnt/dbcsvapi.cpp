	
#include "afxwin.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "malloc.h"
#include "ctype.h"
#include "dbcsv32.h"
#include "errors.h"

#define LOINT(l)       ((short)(l))
#define HIINT(l)       ((short)(((long)(l) >> 16) & 0xFFFF))

short ReadString(char **dptr,char *str,short *lower);
short NextLine(char **dptr,char *str,short *lower);
short ReadStr(HFILE fp,char *str,short *lower);
short ReadWriteString(HFILE fp,HFILE fpw,char *str,short *lower);
short NextRWLine(HFILE fp,HFILE fpw,char *str,short *lower);

short latoi(char *str);
long latol(char *str);
void latof(LPSTR str,double *val);
void lftoa(double *val,char * str);

HANDLE Inst;

short ReadString(char **dptr,char *str,short *lower)
{
	char ch,*string;
	short cc;
	char globalc;    

	string=str;*lower=0;
	while (1)
	{
		globalc=**dptr;*dptr=*dptr+1;
		if (!globalc) cc=0;else cc=1;
		ch=toupper(globalc);
		if (globalc!=ch) *lower=1;
		if (ch!=',' && ch!=10 && cc==1)
		{
			if (ch!=13) {*string=ch;string++;}
		}
		else break;
	}
	*string=0;
	if (cc==1) cc=ch;else cc=-1;
	return cc;
}
short NextLine(char **dptr,char *str,short *lower)
{
	short cc;

	do
	{
		do
		{
			cc=ReadString(dptr,str,lower);
		}
		while (cc==',');
		if (cc>=0) cc=ReadString(dptr,str,lower);
	}
	while (cc>0 && str[0]=='%');
	return cc;
}
short FAR PASCAL ReadDataBase(DataBaseElemsStruct *elems)
{
	HFILE DataBaseFP;
	void *addr;
	short cc,e,col,i,j,recsread;
	short error;
	HANDLE hn;

	short nrlines,*colflags,size;
	short msb,lsb,lower;
	short in;
	double r;
	char *sp,*dp,globalstr[200];
    char *fdata,*dptr;
	unsigned short len;

	for (i=0;lstrlen(elems[i].ColName);i++)
	{
		for (j=0;elems[i].ColName[j]!=' ' && elems[i].ColName[j];j++);
		elems[i].ColName[j]=0;
		if (j==0) break;
	}
	recsread=0;
	nrlines=0;while (lstrlen(elems[nrlines+1].ColName)) nrlines++;
	size=LOINT(elems[0].Type);
	lstrcpy((char *)globalstr,(char *)elems[0].ColName);

	hn=GlobalAlloc(GHND,nrlines*sizeof(short));
	colflags=(short *)GlobalLock(hn);

	for (i=0;i<nrlines;i++) colflags[i]=(-1);
	DataBaseFP=_lopen((LPCSTR)globalstr,OF_READ);
	if (DataBaseFP==-1) { error = -no_such_file; goto err;};
	len=(short)_llseek(DataBaseFP,0L,FILE_END);
	_llseek(DataBaseFP,0L,FILE_BEGIN);
	fdata=(char *)malloc(len+1);
	if (fdata==NULL)
	{
		_lclose(DataBaseFP);
		return -out_of_memory;
	}
	if (_lread(DataBaseFP,(LPSTR)fdata,len)!=len)
	{
		_lclose(DataBaseFP);
		free(fdata);
		return -file_error;
	}
	_lclose(DataBaseFP);
	fdata[len]=0;
	dptr=fdata;
	
	col=0;
	do
	{
		cc=ReadString(&dptr,(char *)globalstr,(short *)&lower);
		if (!col && globalstr[0]=='%') cc=NextLine(&dptr,(char *)globalstr,(short *)&lower);
		i=0;
		while ((lstrcmp((char *)globalstr,(char *)elems[i+1].ColName) || colflags[i]!=-1)
					&& i<nrlines) i++;
		if (i<nrlines)
		{
			colflags[i]=col;
		}
		col++;
	}
	while (cc==',');
	while (1)
	{
		col=0;
		do
		{
			cc=ReadString(&dptr,(char *)globalstr,(short *)&lower);
			if (!col && globalstr[0]=='%') cc=NextLine(&dptr,(char *)globalstr,(short *)&lower);
			if (cc<0 && (!lstrlen((LPSTR)globalstr) || globalstr[0]=='%')) break;
			for (i=0;i<nrlines;i++)
				if (colflags[i]==col)
			{
				addr=(void *)((char *)(elems[i+1].Address)+recsread*size);
				switch (LOINT(elems[i+1].Type))
				{
				case STR:lstrcpy((char *)addr,(char *)globalstr);break;
				case INT:
					if (globalstr[lstrlen((LPSTR)globalstr)-1]=='H')
					{
						globalstr[lstrlen((LPSTR)globalstr)-1]=0;
						in=(short)HexToDec((char *)globalstr);
					}
					else in=latoi((char *)globalstr);
					sp=(char *)&in;
					dp=(char *)addr;
					for (j=0;j<sizeof(short);j++)
					{
						dp[j]=sp[j];
					}
					break;
				case FLT:
					j=lstrlen((LPSTR)globalstr)-1;
					if (globalstr[j]=='H')
					{
						globalstr[lstrlen((LPSTR)globalstr)-1]=0;
						r=(double)HexToDec((char *)globalstr);
					}
					else
					{
						latof((LPSTR)globalstr,(double *)&r);
					}
					sp=(char *)&r;
					dp=(char *)addr;
					for (j=0;j<sizeof(double);j++)
					{
						dp[j]=sp[j];
					}
					break;
				case HEX:
					e=lstrlen((LPSTR)globalstr)/2;
					for (j=0;j<e;j++)
					{
						msb=toupper(globalstr[j*2]);lsb=toupper(globalstr[j*2+1]);
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
	free(fdata);
	elems[0].Type=size+65536*(unsigned long)recsread;
	GlobalUnlock(hn);
	GlobalFree(hn);
	return recsread;
err:
	free(fdata);
	GlobalUnlock(hn);
	GlobalFree(hn);
	return error;
}
short FAR PASCAL SearchDataBase(DataBaseElemsStruct *elems,char *searchstr,char *colname)
{
	short c,e,i,line,recs,size;
	char *p1,*p2,*p3;

	recs=HIINT(elems[0].Type);
	size=LOINT(elems[0].Type);
	line=1;
	while (lstrcmp(colname,(char *)elems[line].ColName) && lstrlen(elems[line].ColName)) line++;
	if (!lstrlen(elems[line].ColName)) return -non_existent;
	i=-1;
	p2=(char *)(elems[line].Address);
	p3=searchstr;
	do
	{
		i++;p1=p3;
		c=0;
		do
		{
			if (*(p2+c)=='*') return i;
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
short FAR PASCAL SearchDataBaseFromFile(char *filename,char *searchstr,char *colname,
		char *datacol,short datacollen,char *data,short datalen)
{
	HFILE DataBaseFP;
	char str[200];
	short cc,col,colnrsearch,colnrdata[100],lower,found,maxdatacol,x,i,j;
	char globalstr[200];
	char *fdata,*dptr;
	unsigned short len;

	for (i=0;lstrlen(&datacol[i*datacollen]);i++)
	{
		for (j=0;datacol[i*datacollen+j]!=' ' && datacol[i*datacollen+j];j++);
		datacol[i*datacollen+j]=0;
		if (j==0) break;
	}

	lstrcpy((char *)globalstr,filename);
	DataBaseFP=_lopen((LPCSTR)globalstr,OF_READ);
	if (DataBaseFP==-1) return -no_such_file;
	len=(short)_llseek(DataBaseFP,0L,FILE_END);
	_llseek(DataBaseFP,0L,FILE_BEGIN);
	fdata=(char *)malloc(len+1);
	if (fdata==NULL)
	{
		_lclose(DataBaseFP);
		return -out_of_memory;
	}
	if (_lread(DataBaseFP,(LPSTR)fdata,len)!=len)
	{
		_lclose(DataBaseFP);
		free(fdata);
		return -file_error;
	}
	_lclose(DataBaseFP);
	fdata[len]=0;
	dptr=fdata;
	
	maxdatacol=0;while (lstrlen(&datacol[maxdatacol*datacollen])) maxdatacol++;
	col=0;colnrsearch=-1;for (x=0;x<100;x++) colnrdata[x]=-1;
	do
	{
		cc=ReadString(&dptr,(char *)str,(short *)&lower);
		if (!col && str[0]=='%') cc=NextLine(&dptr,(char *)str,(short *)&lower);
		if (!lstrcmp((char *)str,colname)) colnrsearch=col;
		for (x=0;x<maxdatacol;x++)
			if (!lstrcmp((char *)str,&datacol[x*datacollen])) colnrdata[x]=col;
		col++;
	}
	while (cc==',');
	if (colnrsearch==-1) { _lclose(DataBaseFP); return -non_existent;};
	for (x=0;x<maxdatacol;x++)
		if (colnrdata[x]==-1) { _lclose(DataBaseFP);return -non_existent;};

	found=0;
	while (1)
	{
		col=0;
		do
		{
			cc=ReadString(&dptr,(char *)str,(short *)&lower);
			if (!col && str[0]=='%') cc=NextLine(&dptr,(char *)str,(short *)&lower);
			if (cc<0 && (!lstrlen((char *)str) || str[0]=='%')) break;
			if (col==colnrsearch && !lstrcmp((char *)str,searchstr)) found=1;
			for (x=0;x<maxdatacol;x++)
			{
				if (col==colnrdata[x])
				{
					lstrcpy(&data[x*datalen],(char *)str);
					for (j=lstrlen(&data[x*datalen]);j<datalen;j++) data[x*datalen+j]=0;
				}
			}
			col++;
		}
		while (cc==',');
		if (cc<0 || found) break;
	}
	free(fdata);
	if (found) return 0;else return -non_existent;
}
short FAR PASCAL GetDataBaseProperties(char *properties,short size,char *file)
{
	HFILE DataBaseFP;
	char str[50];
	short off,cc,col;
	short lower;
	char globalstr[200];
	char *fdata,*dptr;
	unsigned short len;

	lstrcpy((char *)globalstr,file);
	DataBaseFP=_lopen((LPCSTR)globalstr,OF_READ);
	if (DataBaseFP==-1) return -no_such_file;
	len=(short)_llseek(DataBaseFP,0L,FILE_END);
	_llseek(DataBaseFP,0L,FILE_BEGIN);
	fdata=(char *)malloc(len+1);
	if (fdata==NULL)
	{
		_lclose(DataBaseFP);
		return -out_of_memory;
	}
	if (_lread(DataBaseFP,(LPSTR)fdata,len)!=len)
	{
		_lclose(DataBaseFP);
		free(fdata);
		return -file_error;
	}
	_lclose(DataBaseFP);
	fdata[len]=0;
	dptr=fdata;
	
	col=0;off=0;
	do
	{
		cc=ReadString(&dptr,(char *)str,(short *)&lower);
		if (!col && str[0]=='%') cc=NextLine(&dptr,(char *)str,(short *)&lower);
		if (!lower && lstrlen((char *)str))
		{
			lstrcpy(properties+off,(char *)str);
			off=off+size;
		}
		col++;
	}
	while (cc==',');

	free(fdata);
	return (short)(off/size);
}
unsigned long FAR PASCAL HexToDec(char *str)
{
	short i,d;
	unsigned long off,value;

	off=1;value=0;
	for (i=lstrlen(str)-1;i>=0;i--)
	{
		if (str[i]>='A') d=str[i]-'A'+10;else d=str[i]-'0';
		value=value+off*d;
		off=off*16;
	}
	return value;
}
short FAR PASCAL GetDataBaseSize(char *filename)
{
	HFILE DataBaseFP;
	char str[200];
	short cc,col,line,lower;
	char globalstr[200];
	char *fdata,*dptr;
	unsigned long len;

	lstrcpy((LPSTR)globalstr,filename);
	DataBaseFP=_lopen((LPCSTR)globalstr,OF_READ);
	if (DataBaseFP==-1) return -no_such_file;
	len=_llseek(DataBaseFP,0L,FILE_END);
	_llseek(DataBaseFP,0L,FILE_BEGIN);
	fdata=(char *)malloc(len+1);
	if (fdata==NULL)
	{
		_lclose(DataBaseFP);
		return -out_of_memory;
	}
	if (_lread(DataBaseFP,(LPSTR)fdata,len)!=len)
	{
		_lclose(DataBaseFP);
		free(fdata);
		return -file_error;
	}
	_lclose(DataBaseFP);
	fdata[len]=0;
	dptr=fdata;

	line=-1;
	while (1)
	{
		col=0;
		do
		{
			cc=ReadString(&dptr,(char *)str,(short *)&lower);
			if (!col && str[0]=='%') cc=NextLine(&dptr,(char *)str,(short *)&lower);
			if (cc<0 && (!lstrlen((char *)str) || str[0]=='%')) break;
			col++;
		}
		while (cc==',');
		if (cc<0) break;
		line++;
	}
	free(fdata);
	return line;
}
short ReadStr(HFILE fp,char *str,short *lower)
{
	char ch,*string;
	short cc;
	char globalc;

	string=str;*lower=0;
	while (1)
	{
		cc=_lread(fp,(LPSTR)&globalc,1);
		ch=globalc;
		if (globalc!=ch) *lower=1;
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
short ReadWriteString(HFILE fp,HFILE fpw,char *str,short *lower)
{
	char ch,*string;
	short cc;
	char globalc,cr=13;

	string=str;*lower=0;
	while (1)
	{
		cc=_lread(fp,(LPSTR)&globalc,1);
		if (cc==1 && globalc!=13)
		{
			if (globalc==10) _lwrite(fpw,(LPCSTR)&cr,1);
			_lwrite(fpw,(LPCSTR)&globalc,1);
		}
		ch=(char)toupper((short)globalc);
		if (globalc!=ch) *lower=1;
		if (ch!=',' && ch!=10 && cc==1)
		{
			if (ch!=13) {*string=ch;string++;}
		}
		else break;
	}
	*string=0;
	if (cc==1) cc=ch;else cc=-1;
	return cc;
}
short NextRWLine(HFILE fp,HFILE fpw,char *str,short *lower)
{
	short cc;

	do
	{
		do
		{
			cc=ReadWriteString(fp,fpw,str,lower);
		}
		while (cc==',');
		if (cc>=0) cc=ReadWriteString(fp,fpw,str,lower);
	}
	while (cc>0 && str[0]=='%');
	return cc;
}
short FAR PASCAL SaveDataBase(DataBaseElemsStruct *elems,short Size)
{
	HFILE DataBaseFP,FPw;
	char *addr;
	char str[200];
	short cc,col,i,j,recsread,maxcol;
	short error;
	short nrlines,*colflags,size;
	short lower;
	short in;
	double r;
	char globalfile[80];
	char globalstr[80],cr=13;
	HANDLE hn;
	OFSTRUCT OfData;

	for (i=0;lstrlen(elems[i].ColName);i++)
	{
		for (j=0;elems[i].ColName[j]!=' ' && elems[i].ColName[j];j++);
		elems[i].ColName[j]=0;
		if (j==0) break;
	}
	recsread=0;
	nrlines=0;while (lstrlen(elems[nrlines+1].ColName)) nrlines++;
	size=LOINT(elems[0].Type);
	lstrcpy((char *)globalstr,elems[0].ColName);
	hn=GlobalAlloc(GHND,nrlines*sizeof(short));
	colflags=(short *)GlobalLock(hn);

   	for (i=0;i<nrlines;i++) colflags[i]=(-1);
	DataBaseFP=_lopen((LPCSTR)globalstr,OF_READ);
	if (DataBaseFP==-1) { error = -no_such_file; goto err;};
	for (i=0;i<lstrlen(elems[0].ColName)-3;i++) globalfile[i]=elems[0].ColName[i];
	globalfile[i]=0;
	lstrcat((LPSTR)globalfile,"TMP");
	FPw=OpenFile((LPCSTR)globalfile,(LPOFSTRUCT)&OfData,OF_CREATE);
	if (FPw==-1) { error = -file_error; goto err;};
	col=0;
	do
	{
		cc=ReadWriteString(DataBaseFP,FPw,(char *)str,(short *)&lower);
		if (!col && str[0]=='%') cc=NextRWLine(DataBaseFP,FPw,(char *)str,(short *)&lower);
		i=0;
		while ((lstrcmp((char *)str,(char *)elems[i+1].ColName) || colflags[i]!=-1)
					&& i<nrlines) i++;
		if (i<nrlines)
		{
			colflags[i]=col;
		}
		col++;
	}
	while (cc==',');

	for (i=0;i<nrlines;i++)
		if (colflags[i]<0) { error = -non_existent; goto err;};

    maxcol=0;
	while (1)
	{
		col=0;
		do
		{
			cc=ReadStr(DataBaseFP,(char *)globalstr,(short *)&lower);
			do
			{
				if (!col && globalstr[0]=='%')
				{
					_lwrite(FPw,(LPSTR)globalstr,lstrlen((LPSTR)globalstr));globalstr[0]=(char)cc;
					if (cc==10) _lwrite(FPw,(LPCSTR)&cr,1);
					_lwrite(FPw,(LPSTR)globalstr,1);
					do
					{
						cc=ReadWriteString(DataBaseFP,FPw,(char *)globalstr,(short *)&lower);
					}
					while (cc==',');

					cc=ReadStr(DataBaseFP,(char *)globalstr,(short *)&lower);
				}
			}
			while (cc>0 && globalstr[0]=='%');

			if (cc<0 && (!lstrlen((LPSTR)globalstr) || globalstr[0]=='%')) break;

			for (i=0;i<nrlines;i++)
			{
			  if (colflags[i]==col)
			  {
				addr=(char *)((char *)(elems[i+1].Address)+recsread*size);
				switch (LOINT(elems[i+1].Type))
				{
				case STR:
					for (j=0;addr[j] && addr[j]!=' ';j++) globalstr[j]=addr[j];
					globalstr[j]=0;
					break;
				case INT:
					in=*(short *)addr;
					wsprintf((LPSTR)globalstr,(LPSTR)"%d",in);
					break;
				case FLT:
					r=*(double *)addr;
					lftoa((double *)&r,(LPSTR)globalstr);
					break;
				}
			  }
			}

			_lwrite(FPw,(LPSTR)globalstr,lstrlen((LPSTR)globalstr));globalstr[0]=(char)cc;
			if (cc==10) _lwrite(FPw,(LPCSTR)&cr,1);
			_lwrite(FPw,(LPSTR)globalstr,1);
			if (col>maxcol) maxcol=col;
			col++;
		}
		while (cc==',');
		if (cc<0) break;
		recsread++;
	}
	while (recsread<Size)
	{
		for (col=0;col<=maxcol;col++)
		{
			for (i=0;i<nrlines;i++)
			{
			  if (colflags[i]==col)
			  {
				addr=(char *)((char *)(elems[i+1].Address)+recsread*size);
				switch (LOINT(elems[i+1].Type))
				{
				case STR:
					for (j=0;addr[j] && addr[j]!=' ';j++) globalstr[j]=addr[j];
					globalstr[j]=0;
					break;
				case INT:
					in=*(short *)addr;
					wsprintf((LPSTR)globalstr,(LPSTR)"%d",in);
					break;
				case FLT:
					r=*(double *)addr;
					lftoa((double *)&r,(LPSTR)globalstr);
					break;
				}
				_lwrite(FPw,(LPSTR)globalstr,lstrlen((LPSTR)globalstr));
			  }
			}
			if (col<maxcol) _lwrite(FPw,(LPSTR)",",1);
			else _lwrite(FPw,(LPSTR)"\n",1);
		}
		recsread++;
	}
	_lclose(DataBaseFP);
	_lclose(FPw);
	elems[0].Type=size+65536*(unsigned long)recsread;
	GlobalUnlock(hn);
	GlobalFree(hn);

	DataBaseFP=_lopen((LPCSTR)globalfile,OF_READ);
	FPw=OpenFile((LPCSTR)elems[0].ColName,(LPOFSTRUCT)&OfData,OF_CREATE);
	do
	{
		cc=_lread(DataBaseFP,(LPSTR)&cr,1);
		if (cc==1) _lwrite(FPw,(LPSTR)&cr,1);

	}while (cc==1);
	_lclose(DataBaseFP);
	_lclose(FPw);
	FPw=OpenFile((LPCSTR)globalfile,(LPOFSTRUCT)&OfData,OF_DELETE);
	_lclose(FPw);

	return recsread;
err:
	_lclose(DataBaseFP);
	_lclose(FPw);
	GlobalUnlock(hn);
	GlobalFree(hn);

	return error;
}

short latoi(char *str)
{
	short i,s,l;
	short v,m;

	for (l=0;str[l];l++);

	for (s=0,m=1,v=0,i=l-1;i>=0;i--,m=m*10)
	{
		if (!isdigit(str[i]))
		{
			if (i==0)
			{
				if (str[i]=='-') s=-1;
				if (str[i]=='+') s=1;
				if (s==0) return 0;
			}
			else return 0;
		}
		else v=v+(str[i]-'0')*m;
	}
	if (s==0 || s==1) return v; else return -v;
}
long latol(char *str)
{
	short i,s,l;
	long v,m;

	for (l=0;str[l];l++);

	for (s=0,m=1,v=0,i=l-1;i>=0;i--,m=m*10)
	{
		if (!isdigit(str[i]))
		{
			if (i==0)
			{
				if (str[i]=='-') s=-1;
				if (str[i]=='+') s=1;
				if (s==0) return 0;
			}
			else return 0;
		}
		else v=v+(str[i]-'0')*m;
	}
	if (s==1 || s==0) return v; else return -v;
}
void latof(LPSTR str,double *val)
{
	short i,s,l,dp;
	double m,v;
	char *ptrs,*ptrd;

	for (m=1,dp=0,l=0;str[l];l++)
	{
		if (dp) m=m/10;
		if (str[l]=='.') dp=l;
	}
	for (s=0,v=0,i=l-1;i>=0;i--)
	{
		if (!isdigit(str[i]))
		{
			if (i==0)
			{
				if (str[i]=='-') s=-1;
				if (str[i]=='+') s=1;
				if (s==0) {v=0;goto out;}
			}
			else if (i!=dp) {v=0;goto out;}
		}
		else
		{
			v=v+(str[i]-'0')*m;
			m=m*10;
		}
	}
	if (s==-1) v=-v;
out:
	ptrs=(char *)&v;
	ptrd=(char *)val;
	for (i=0;i<sizeof(double);i++)
	{
		ptrd[i]=ptrs[i];
	}
}
void lftoa(double *val,LPSTR str)
{
	short i,f,count;
	double r,v;
	char *ptrs,*ptrd;

	v=1;count=0;
	ptrs=(char *)val;
	ptrd=(char *)&r;

	for (i=0;i<sizeof(double);i++)
	{
		ptrd[i]=ptrs[i];
	}
	if (r==0)
	{
		str[0]=0;
		return;
	}
	if (r<0)
	{
		r=-r;
		str[count++]='-';
	}
	if (r>=v)
	{
		while (r>=(v*10)) v=v*10;
	}
	else
	{
		str[count++]='0';
		v=0.1;
	}
	do
	{
		if (v==0.1) str[count++]='.';
		f=(short)(double)(r/v);
		str[count++]=f+'0';
		r=r-(double)f*v;v=v/10;
	}while ((r>0 || v>1) && v>0.0000001);
	if (v<0.1)
	{
		while (str[count-1]=='0') count--;
	}
	str[count]=0;
}
