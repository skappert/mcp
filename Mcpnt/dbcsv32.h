#define STR 0
#define INT 1
#define FLT 2
#define HEX 3

typedef struct
{
	char ColName[65];
	long Type;
	long Address;
}DataBaseElemsStruct;

short FAR PASCAL ReadDataBase(DataBaseElemsStruct *elems);
short FAR PASCAL GetDataBaseSize(char *filename);
short FAR PASCAL SearchDataBase(DataBaseElemsStruct *elems,char *searchstr,char *colname);
short FAR PASCAL GetDataBaseProperties(char *properties,short size,char *file);
short FAR PASCAL SearchDataBaseFromFile(char *filename,char *searchstr,char *colname,
		char *datacol,short datacollen,char *data,short datalen);
unsigned long FAR PASCAL HexToDec(char *str);
short FAR PASCAL SaveDataBase(DataBaseElemsStruct *elems,short Size);


