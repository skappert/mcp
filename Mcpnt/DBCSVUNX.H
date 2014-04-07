#define STR 0
#define INT 1
#define FLT 2
#define HEX 3

typedef struct
{
	char ColName[65];
	long Type;
	void *Address;
}DataBaseElemsStruct;

short ReadDataBase(DataBaseElemsStruct *elems);
short SearchDataBase(DataBaseElemsStruct *elems,char *searchstr,char *colname);
short GetDataBaseProperties(char *properties,short size,char *file);
short SearchDataBaseFromFile(char *filename,char *searchstr,char *colname,
		char *datacol,short datacollen,char *data,short datalen);
short GetDataBaseSize(char *filename);
unsigned long HexToDec(char *str);
