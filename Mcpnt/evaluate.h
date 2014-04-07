
#define VARLEN          15              /* Max length of variable names */
#define MAXVARS         50              /* Max user-defined variables */
#define TOKLEN          30              /* Max token length */

#define VAR             1
#define DEL             2
#define NUM             3

double _scal( double x,... ); 
double _asym( double a,... );
double _asymerr( double a,... );
double _sqr( double x, ... );
double _sin( double x, ... );
double _cos( double x, ...);
double _tan( double x, ... );
double _asin( double x, ... );
double _acos( double x, ... );
double _atan( double x, ... );
double _sinh( double x, ... );
double _cosh( double x, ... );
double _tanh( double x, ... );
double _exp( double x, ... );
double _log( double x, ... );
double _log10( double x, ... );
double _sqrt( double x, ... );
double _floor( double x, ... );
double _ceil( double x, ... );
double _fabs( double x, ... );
double _deg( double x, ... );
double _rad( double x, ... );

typedef struct
{
   char name[VARLEN + 1];               /* Variable name */
   double value;                          /* Variable value */
} VARIABLE;

typedef struct
{
   char* name;                          /* Function name */
   int   args;                          /* Number of arguments to expect */
   double (*func)(double,...);                     /* Pointer to function */
} FUNCTION;

/* The following macros are ASCII dependant, no EBCDIC here! */
#define iswhite(c)  (c == ' ' || c == '\t')
#define isnumer(c)  ((c >= '0' && c <= '9') || c == '.')
#define isalpha(c)  ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') \
                    || c == '_')
#define isdelim(c)  (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' \
                    || c == '^' || c == '(' || c == ')' || c == ',' || c == '=')

/* Codes returned from the evaluator */
#define E_OK           0        /* Successful evaluation */
#define E_SYNTAX       1        /* Syntax error */
#define E_UNBALAN      2        /* Unbalanced parenthesis */
#define E_DIVZERO      3        /* Attempted division by zero */
#define E_UNKNOWN      4        /* Reference to unknown variable */
#define E_MAXVARS      5        /* Maximum variables exceeded */
#define E_BADFUNC      6        /* Unrecognised function */
#define E_NUMARGS      7        /* Wrong number of arguments to funtion */
#define E_NOARG        8        /* Missing an argument to a funtion */
#define E_EMPTY        9        /* Empty expression */

static int  Level1( double* r );
static void Level2( double* r );
static void Level3( double* r );
static void Level4( double* r );
static void Level5( double* r );
static void Level6( double* r );


VARIABLE        Vars[MAXVARS];       /* Array for user-defined variables */
char   *expression;          /* Pointer to the user's expression */
char   token[TOKLEN + 1];   /* Holds the current token */
int             type;                /* Type of the current token */
jmp_buf         jb;                  /* jmp_buf for errors */

