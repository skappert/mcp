#include "stdafx.h"
#include "MCP for NT.h"
#include "MCP for NTDoc.h"
#include "ActionObjects.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <stdlib.h>
#include <math.h>
#include <setjmp.h>
#include "evaluate.h"


#define ERR(n) {ERRORNUM=n; ERPOS=expression-ERANC-1; strcpy(ERTOK,(char*)token); longjmp(jb,1);};


#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif
#ifndef M_E
#define M_E     2.71828182845904523536
#endif

double deg( double x );
double rad( double x );

static int   ERRORNUM;                 /* The error number */
static char  ERTOK[TOKLEN + 1];     /* The token that generated the error */
static int   ERPOS;                 /* The offset from the start of the expression */
static char* ERANC;                 /* Used to calculate ERPOS */

static CMCPforNTDoc* pDocument;
static USHORT index;
static USHORT TheScan;
static BOOL EVALUATENEXT;
char* ErrMsgs[] =
{
   "Syntax error",
   "Unbalanced parenthesis",
   "Division by zero",
   "Unknown variable",
   "Maximum variables exceeded",
   "Unrecognised funtion",
   "Wrong number of arguments to funtion",
   "Missing an argument",
   "Empty expression"
};


VARIABLE Consts[] =
{
   /* name, value */
   { "pi",      M_PI },
   { "e",       M_E },
   { 0 }
};

FUNCTION Funcs[] =
{
	{ "c",		2,	 _scal },
	{ "asym",	3,	 _asym },
	{ "asymerr",3,	 _asymerr },
	{ "sqrt",   1,   _sqrt },
	{ "sqr",    1,   _sqr },
	{ "log",    1,   _log },
	{ "log10",  1,   _log10 },
	{ "exp",    1,   _exp },
	{ "floor",  1,   _floor },
	{ "ceil",   1,   _ceil },
	{ "abs",    1,   _fabs },
	{ "deg",    1,   _deg },
	{ "rad",    1,   _rad },
	{ "sin",    1,   _sin },
	{ "cos",    1,   _cos },
	{ "tan",    1,   _tan },
	{ "asin",   1,   _asin },
	{ "acos",   1,   _acos },
	{ "atan",   1,   _atan },
	{ "sinh",   1,   _sinh },
	{ "cosh",   1,   _cosh },
	{ "tanh",   1,   _tanh },
	{ 0,0,0 }
};


/*************************************************************************
**                                                                       **
** Some custom math functions...   Note that they must be prototyped     **
** above (if your compiler requires it)                                  **
**                                                                       **
** deg( x )             Converts x radians to degrees.                   **
** rad( x )             Converts x degrees to radians.                   **
**                                                                       **
 *************************************************************************/

double _scal( double x,... )
{
	POSITION pos;
	CString TheObject;
	double data = 0;
	BOOL FOUND = FALSE;
	double Track;
	CString TrackStr;
	va_list arg_ptr;
	va_start(arg_ptr,x);
	Track	= va_arg(arg_ptr,double);

	if(!pDocument->ActionObjList.IsEmpty())
	{
		ActionObject* pActionObj = (ActionObject*)pDocument->ActionObjList.GetHead();
		pos = pDocument->ActionObjList.GetHeadPosition();
		do
		{
			pActionObj = (ActionObject*)pDocument->ActionObjList.GetNext(pos);
			if (pActionObj!=NULL)
			{
				TheObject = pActionObj->GetName();
				if (TheObject.Find(__PM_SpectrumObj)!=-1)
				{
					PM_SpectrumObj* pScalerObj = (PM_SpectrumObj*)pActionObj;
					if ((pScalerObj->SubRegNo==(int)x)&&
						(pScalerObj->pTrack->MyPosition==(int)Track))
					{
						if(!EVALUATENEXT)data = pScalerObj->y[index];
						else data = pScalerObj->y[index]*(TheScan)/(TheScan-1);
						FOUND = TRUE;
					}
				}
			}
		}
		while ((pos!=NULL)&&(!FOUND));
	}
	return(data);
} 
double _asym( double a,... )
{
	double b;
	double c;
	double sumscal;
	va_list arg_ptr;
	va_start(arg_ptr,a);
	b = va_arg(arg_ptr,double);
	c = va_arg(arg_ptr,double);
	double scal_a = _scal(a,c);
	double scal_b = _scal(b,c);
	sumscal = (scal_a+scal_b);
	if(sumscal!=0)return((scal_a-scal_b)/sumscal);
	else return 0;
}
double _asymerr( double a,... )
{
	double b;
	double c;
	double sumscal;
	va_list arg_ptr;
	va_start(arg_ptr,a);
	b = va_arg(arg_ptr,double);
	c = va_arg(arg_ptr,double);
	double scal_a = _scal(a,c);
	double scal_b = _scal(b,c);
	sumscal = (scal_a+scal_b);
	if(sumscal!=0)return (1/((scal_a+scal_b)*(scal_a+scal_b))*
		sqrt((2*scal_b*sqrt(scal_a))*(2*scal_b*sqrt(scal_a))+
		(2*scal_a*sqrt(scal_b))*(2*scal_a*sqrt(scal_b))));
	else return 0;
}
double _sqr( double x ,...)
{
   return(x*x);
} 
double _sin( double x ,...)
{
   return(sin(x));
}
double _cos( double x ,...)
{
   return(cos(x));
}
double _tan( double x ,...)
{
   return(tan(x));
}
double _asin( double x ,...)
{
   return(asin(x));
}
double _acos( double x ,...)
{
   return(acos(x));
}
double _atan( double x ,...)
{
   return(atan(x));
}
double _sinh( double x ,...)
{
   return(sinh(x));
}
double _cosh( double x ,...)
{
   return(cosh(x));
}
double _tanh( double x ,...)
{
   return(tanh(x));
}
double _exp( double x ,...)
{
   return(exp(x));
}
double _log( double x ,...)
{
   if(x!=0)return(log(x));
   else return 1;
}
double _log10( double x ,...)
{
   if(x!=0)return(log10(x));
   else return 1;
}
double _sqrt( double x ,...)
{
   return(sqrt(x));
}
double _floor( double x ,...)
{
   return(floor(x));
}
double _ceil( double x ,...)
{
   return(ceil(x));
}
double _fabs( double x ,...)
{
   return(fabs(x));
}
double _deg( double x ,...)
{
   return( x * 180.0 / M_PI );
}
double _rad( double x ,...)
{
   return( x * M_PI / 180.0 );
}


/*************************************************************************
**                                                                       **
** GetSymbol( char* s )                                                  **
**                                                                       **
** This routine obtains a value from the program's environment.          **
** This works for DOS and VMS (and other OS's???)
**                                                                       **
 ************************************************************************/

GetSymbol( char* s, double* v )
{
   char* e;

   e = getenv( s );
   if( !e )
      return( 0 );
   *v = atof( e );
   return( 1 );
}



/*************************************************************************
**                                                                       **
** ClearAllVars()                                                        **
**                                                                       **
** Erases all user-defined variables from memory. Note that constants    **
** can not be erased or modified in any way by the user.                 **
**                                                                       **
** Returns nothing.                                                      **
**                                                                       **
 *************************************************************************/

void ClearAllVars()
{
   int i;

   for( i = 0; i < MAXVARS; i++ )
   {
      *Vars[i].name = 0;
      Vars[i].value = 0;
   }
}


/*************************************************************************
**                                                                       **
** ClearVar( char* name )                                                **
**                                                                       **
** Erases the user-defined variable that is called NAME from memory.     **
** Note that constants are not affected.                                 **
**                                                                       **
** Returns 1 if the variable was found and erased, or 0 if it didn't     **
** exist.                                                                **
**                                                                       **
 *************************************************************************/

ClearVar( char* name )
{
   int i;

   for( i = 0; i < MAXVARS; i++ )
      if( *Vars[i].name && ! strcmp( name, Vars[i].name ) )
      {
         *Vars[i].name = 0;
         Vars[i].value = 0;
         return( 1 );
      }
   return( 0 );
}


/*************************************************************************
**                                                                       **
** GetValue( char* name, double* value )                                   **
**                                                                       **
** Looks up the specified variable (or constant) known as NAME and       **
** returns its contents in VALUE.                                        **
**                                                                       **
** First the user-defined variables are searched, then the constants are **
** searched.                                                             **
**                                                                       **
** Returns 1 if the value was found, or 0 if it wasn't.                  **
**                                                                       **
 *************************************************************************/

GetValue( char* name, double* value )
{
   int i;

   /* First check for an environment variable reference... */
   if( *name == '_' )
      return( GetSymbol( name + 1, value ) );

   /* Now check the user-defined variables. */
   for( i = 0; i < MAXVARS; i++ )
      if( *Vars[i].name && ! strcmp( name, Vars[i].name ) )
      {
         *value = Vars[i].value;
         return( 1 );
      }

   /* Now check the programmer-defined constants. */
   for( i = 0; *Consts[i].name; i++ )
      if( *Consts[i].name && ! strcmp( name, Consts[i].name ) )
      {
         *value = Consts[i].value;
         return( 1 );
      }
   return( 0 );
}


/*************************************************************************
**                                                                       **
** SetValue( char* name, double* value )                                   **
**                                                                       **
** First, it erases any user-defined variable that is called NAME.  Then **
** it creates a new variable called NAME and gives it the value VALUE.   **
**                                                                       **
** Returns 1 if the value was added, or 0 if there was no more room.     **
**                                                                       **
 *************************************************************************/

SetValue( char* name, double* value )
{
   int  i;
   ClearVar( name );
   for( i = 0; i < MAXVARS; i++ )
      if( ! *Vars[i].name )
      {
         strcpy( Vars[i].name, name );
         Vars[i].name[VARLEN] = 0;
         Vars[i].value = *value;
         return( 1 );
      }
   return( 0 );
}


/*************************************************************************
**                                                                       **
** Parse()   Internal use only                                           **
**                                                                       **
** This function is used to grab the next token from the expression that **
** is being evaluated.                                                   **
**                                                                       **
 *************************************************************************/

static void Parse(void)
{
   char* t;

   type = 0;
   t = (char*)token;
   while( iswhite( *expression ) )
      expression++;
   if( isdelim( *expression ) )
   {
      type = DEL;
      *t++ = *expression++;
   }
   else if( isnumer( *expression ) )
   {
      type = NUM;
      while( isnumer( *expression ) )
         *t++ = *expression++;
   }
   else if( isalpha( *expression ) )
   {
      type = VAR;
      while( isalpha( *expression ) )
        *t++ = *expression++;
      token[VARLEN] = 0;
   }
   else if( *expression )
   {
      *t++ = *expression++;
      *t = 0;
      ERR( E_SYNTAX );
   }
   *t = 0;
   while( iswhite( *expression ) )
      expression++;
}


/*************************************************************************
**                                                                       **
** Level1( double* r )   Internal use only                                 **
**                                                                       **
** This function handles any variable assignment operations.             **
** It returns a value of 1 if it is a top-level assignment operation,    **
** otherwise it returns 0                                                **
**                                                                       **
 *************************************************************************/

static int Level1( double* r )
{
   char t[VARLEN + 1];

   if( type == VAR )
      if( *expression == '=' )
      {
         strcpy( t, (char*)token );
         Parse();
         Parse();
         if( !*token )
         {
            ClearVar( t );
            return(1);
         }
         Level2( r );
         if( ! SetValue( t, r ) )
            ERR( E_MAXVARS );
         return( 1 );
      }
   Level2( r );
   return( 0 );
}


/*************************************************************************
**                                                                       **
** Level2( double* r )   Internal use only                                 **
**                                                                       **
** This function handles any addition and subtraction operations.        **
**                                                                       **
 *************************************************************************/

static void Level2( double* r )
{
   double t = 0;
   char o;

   Level3( r );
   while( (o = *token) == '+' || o == '-' )
   {
      Parse();
      Level3( &t );
      if( o == '+' )
         *r = *r + t;
      else if( o == '-' )
         *r = *r - t;
   }
}


/*************************************************************************
**                                                                       **
** Level3( double* r )   Internal use only                                 **
**                                                                       **
** This function handles any multiplication, division, or modulo.        **
**                                                                       **
 *************************************************************************/

static void Level3( double* r )
{
   double t;
   char o;

   Level4( r );
   while( (o = *token) == '*' || o == '/' || o == '%' )
   {
      Parse();
      Level4( &t );
      if( o == '*' )
         *r = *r * t;
      else if( o == '/' )
      {
         if( t == 0 )
            ERR( E_DIVZERO );
         *r = *r / t;
      }
      else if( o == '%' )
      {
         if( t == 0 )
            ERR( E_DIVZERO );
         *r = fmod( *r, t );
      }
   }
}


/*************************************************************************
**                                                                       **
** Level4( double* r )   Internal use only                                 **
**                                                                       **
** This function handles any "to the power of" operations.               **
**                                                                       **
 *************************************************************************/

static void Level4( double* r )
{
   double t;

   Level5( r );
   if( *token == '^' )
   {
      Parse();
      Level5( &t );
      *r = pow( *r, t );
   }
}


/*************************************************************************
**                                                                       **
** Level5( double* r )   Internal use only                                 **
**                                                                       **
** This function handles any unary + or - signs.                         **
**                                                                       **
 *************************************************************************/

static void Level5( double* r )
{
   char o = 0;

   if( *token == '+' || *token == '-' )
   {
      o = *token;
      Parse();
   }
   Level6( r );
   if( o == '-' )
      *r = -*r;
}


/*************************************************************************
**                                                                       **
** Level6( double* r )   Internal use only                                 **
**                                                                       **
** This function handles any literal numbers, variables, or functions.   **
**                                                                       **
 *************************************************************************/

static void Level6( double* r )
{
   int  i;
   int  n;
   double a[3];

   if( *token == '(' )
   {
      Parse();
      if( *token == ')' )
         ERR( E_NOARG );
      Level1( r );
      if( *token != ')' )
         ERR( E_UNBALAN );
      Parse();
   }
   else
   {
      if( type == NUM )
      {
         *r = (double) atof( (char*)token );
         Parse();
      }
      else if( type == VAR )
      {
         if( *expression == '(' )
         {
            for( i = 0; Funcs[i].args; i++ )
               if( ! strcmp((char*) token, Funcs[i].name ) )
               {
                  Parse();
                  n = 0;
                  do
                  {
                     Parse();
                     if( *token == ')' || *token == ',' )
                        ERR( E_NOARG );
                     a[n] = 0;
                     Level1( &a[n] );
                     n++;
                  } while( n < 4 && *token == ',' );
                  Parse();
                  if( n != Funcs[i].args )
                  {
                     strcpy((char*) token, Funcs[i].name );
                     ERR( E_NUMARGS );
                  }
                  *r = Funcs[i].func( a[0], a[1], a[2]);
                  return;
               }
               if( !Funcs[i].args )
                  ERR( E_BADFUNC );
            }
            else if( ! GetValue((char*) token, r ) )
               ERR( E_UNKNOWN );
         Parse();
      }
      else
         ERR( E_SYNTAX );
   }
}


/*************************************************************************
**                                                                       **
** Evaluate( char* e, double* result, int* a )                           **
**                                                                       **
** This function is called to evaluate the expression E and return the   **
** answer in RESULT.  If the expression was a top-level assignment, a    **
** value of 1 will be returned in A, otherwise it will contain 0.        **
**                                                                       **
** Returns E_OK if the expression is valid, or an error code.            **
**                                                                       **
 *************************************************************************/

int Evaluate( CMCPforNTDoc* pDoc,char* e, double* result, int* a, USHORT ind )
{
   EVALUATENEXT = FALSE;
   double inde = ind;
   SetValue("i",&inde);
   pDocument = pDoc;
   index = ind;
   if( setjmp( jb ) ) 
	   return( ERRORNUM );
   expression = (char*)e;
   ERANC = e;
   strlwr(expression );
   *result = 0;
   Parse();
   if( ! *token ) ERR( E_EMPTY );
   *a = Level1( result );
   return( E_OK );
}

int EvaluateNext( CMCPforNTDoc* pDoc,char* e, double* result, int* a, USHORT ind,USHORT scan)
{
   EVALUATENEXT = TRUE;
   TheScan = scan;
   double inde = ind;
   SetValue("i",&inde);
   pDocument = pDoc;
   index = ind;
   if( setjmp( jb ) ) 
	   return( ERRORNUM );
   expression = (char*)e;
   ERANC = e;
   strlwr(expression );
   *result = 0;
   Parse();
   if( ! *token ) ERR( E_EMPTY );
   *a = Level1( result );
   return( E_OK );
}