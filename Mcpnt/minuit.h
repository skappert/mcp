// Minuit.h : interface of the CMinuit class
//
/////////////////////////////////////////////////////////////////////////////
// CMinuit view

#define  FOLDRES 250
#define  OFFSET 2
#define NSIGMARES			(14.)                  /*Definitionsfenster*/
#define NTAURES 			(13.)                  /*Definitionsfenster*/


typedef struct { double Am, Haeufigkeit, I,
                        ISV, A_u, B_u, A_o, B_o; } IsotopStruct;
typedef struct { double dE, dU, F_u, F_o, Am, Hoehe, RelInt; } LinienStruct;

typedef struct { double dE, F; } AufspaltungStruct;

#define MaxNoOfLines        50
#define MaxNoOfSplits       20

#define nmax				2000
#define nvarmax				4
#define nstepmax			100
//#define FWHMG_DEF			0.035
//#define FWHML_DEF			0.017
//#define FWHMG_KEP			0.035 * 47.3
//#define FWHML_KEP			0.017 * 47.3

typedef struct _FITPAR
{
	double x;
	double y;
} FITPAR;

#define MNMAXINT 40         /* maximale Parameteranzahl */
#define MNMAXEXT 80
#define LEVENBERG
#ifndef MIN
  #define MIN(a, b) ((a) <= (b) ? (a) : (b))
#endif
#ifndef MAX
  #define MAX(a, b) ((a) >= (b) ? (a) : (b))
#endif
#ifndef M_PI_2
  #define M_PI_2 1.57079632679489661923
#endif
#ifndef M_PI
  #define M_PI 3.14159265359
#endif


#define MAXARG 6
#define PAGEWIDTH 80
#define BIGEDM 123456.
#define UNDEFI -54321.
#define UPDFLT 1

#define d_sign(a, b) ((b) >= 0 ? fabs(a) : -fabs(a))

class DataView;

class CMinuit : public CEditView
{
public:	

	CMinuit();	// protected constructor used by dynamic creation
	CMinuit(DataView* pParent,CString Title,
				 int fitfunc,
				 int sign,
				 int em_start,
				 int eb_start,
				 BOOL in_equalspaced,
				 int numofpeaks,
				 FITPAR FitPar[20]);
	DECLARE_DYNCREATE(CMinuit)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMinuit)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL


// Implementation
protected:
	virtual ~CMinuit();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	void KillMe();
	void ResizeWin(ULONG x,ULONG y);	// Generated message map functions
	float FitFunc[2][500];
	DataView* pResView;
protected:
	//{{AFX_MSG(CMinuit)
	afx_msg void OnChange();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	/* Prototypen */
	/* für Levenberg */
	int BerechneLinien(double Wellenzahl,
							 double I,double Am,
							 double A_u,double B_u,
							 double A_o,double B_o,
							 double J_grund,double J_an,
							 double ISV,double U_beschl,
							 double ReferenzMasse,double Haeufigkeit);
	int gaussj(double a[MNMAXINT][MNMAXINT], double b[]);
	void derivative(double par[], double x_wert, double dyda[]);
	void mrqcof(double par[], double alpha[MNMAXINT][MNMAXINT], double beta[], double *chiq);
	void mrqmin(double beta[MNMAXINT]);
	void marquardt_fit(void);

	/* der Rest */
	double fitfunction(double x, double par[]);
	/* so muss die zu fittende Funktion aussehen */
	void __stdcall fcn(double grad[], double *fval, double par[],int iflag);
	void stand(void);
	void resi(void);
	void mnpout(const int iuext, char **chnam, double *val, double *err, double *xlolim, double *xuplim);
	/* Hiermit koennen die aktuellen Minuit-Parameter abgefragt werden.
	uebergabe: parameter-index (0 bis irgendwas)
	rueckgabe: parameter-name, wert, fehler, obere grenze, untere grenze */
	void mnexecute(char *com1, char *com2, int narg, double arguments[]);
	/* Ausführen eines MINUIT-Kommandos. Parameter:
	kommando1, kommando2, anzahl der kommando-argumente, argumente */
	void mninteractive(void);
	/* ruft MINUIT im interaktiven Modus auf ==> MINUIT Kommando-Ebene */
	void mninit(void);
	/* Initialisiert Minuit. Muss als allererstes!! aufgerufen werden */
	void mnrset(int iopt);
	void mnparm(char *name, double uk, double wk, double lowlim, double uplim);
	/* Definition eines Parameter. Wird wiederholt aufgerufen, bis alle */
	/* Parameter definiert sind. Parameter: name, wert, fehler.*/
	/* rueckgabe: 0: OK; 1: Fehler */
	void log_printf(const char *format, ...);
	void printf(const char *format, ...);
	void log_flush(void);
	void log_printfmov(const char *format, ...);
	void log_over(const char *format, ...);

private:
	BOOL	FASTCALC;
	int		faltselect;
	double	FWHMG_set,FWHML_set,TAU_set;
	double	FWHMG_last,FWHML_last,TAU_last;
	double	foldedfunction[FOLDRES];
	double	foldedfunctionvoigt[FOLDRES];
	double  def_reg,def_reg_voigt;
	double	scalingvoigt;
	double	scalingvoigtexp;
	double	scalinggaussexp;
	double	scalinglorentzexp;


	LinienStruct Linien[MaxNoOfLines];
	int		AnzahlLinien;
	double	Wellenzahl;
	double	I;
	double	Am;
	double	J_grund;
	double	J_an;
	double	ISV;
	double	U_beschl;
	double	ReferenzMasse;
	double	Haeufigkeit;
	double	Kepco;

	double	A_u_last;
	double	B_u_last;
	double	A_o_last;
	double	B_o_last;	
	double	ISV_last;	
	double	U_beschl_last;

	USHORT		numofpeaks;
	int			sign;
	BOOL		equalspaced;
	USHORT		fitfunc;

	CString		PrintString;
	BOOL		OLDCOMMAND;
	CStringList CommandList;
	POSITION    LastCommand;
	int com_count;
	double mnamin,lambda_start,mar_lambda;
	USHORT start_index, end_index, chiq_function;
	char paraname[MNMAXEXT][11];
	int lwarn, limset, lnewmn, exit_minuit;
	int nvarl[MNMAXEXT], niofex[MNMAXEXT], nexofi[MNMAXINT], ipfix[MNMAXINT];
	USHORT npar, nu, nfcn, nfcnmx, nfcnfr, istrat, itaur;
	int call_limit_flag, print_level, converged, usergrad, err_matrix, npfix, ke1cr;
	double edm, epsmac, epsma2, epsi, apsi, dcovar, up, xmidcr, xdircr;
	double argum[MAXARG], u[MNMAXEXT], alim[MNMAXEXT], blim[MNMAXEXT];
	double vhmat[MNMAXINT][MNMAXINT], vthmat[MNMAXINT][MNMAXINT], pmat[MNMAXINT][MNMAXINT];
	double grd[MNMAXINT], grds[MNMAXINT], g2[MNMAXINT], g2s[MNMAXINT];
	double gstep[MNMAXINT], gsteps[MNMAXINT], gin[MNMAXEXT], dgrd[MNMAXINT];
	double erp[MNMAXINT], ern[MNMAXINT], werr[MNMAXINT], globcc[MNMAXINT];
	double x[MNMAXINT], xt[MNMAXINT], dirin[MNMAXINT], xs[MNMAXINT], xts[MNMAXINT], dirins[MNMAXINT];
	double pstar[MNMAXINT], pstst[MNMAXINT], pbar[MNMAXINT];

	/************  für beta-NMR  *****************/
	int channels;// = 101 ;					// Kanaele im Scan
	double increment;
	double range;// = 50;					// Scanning range
	int nstep;// = 100;						// Aufloesung, mit der die Dgl geloest wird
	int nvar;// = 4;						// Anzahl der Zustaende
	double staerkeRf1[nvarmax][nvarmax];
	double staerkeRf2[nvarmax][nvarmax];
	double staerkeRf3[nvarmax][nvarmax];
	double RkdumbX[nstepmax];				// X und Y-Werte des Loesungsfeldes
	double RkdumbY[nvarmax][nstepmax];		// Dimension muss jeweils >= [nvar][nstep]
	double T1;// = 0;							// Anfangs- und 
	double T2;// = 10;							// Endzeitpunkt fuer rf-Einstrahlung
	double nuQ;// = 20;						// Quadrupolfrequenz  
	double nuL;// = 1000;              
	double nu_rf;							// Radiofrequenz
	double nu_rf0;							// Startfrquenz im 3-rf-scan; idealerweise = nuL
	double nu1;
	double deltanuL;
	// Faltungsparameter:
	double sigma;
	double intrange;// = 20;
	int nint;// = 100;
	double om;
	double omi;
	double pop_unNorm[nvarmax];
	/************  Ende für beta-NMR  *****************/

	DataView* pDataView;
	BOOL IsActivated;
	CMinuitChild* pMinuitChild;

protected:
	BOOL	HAVEOWNER;
	BOOL	STOPME;
	SHORT	CharCount;
	CFont* NewFont;
	CString cfrom; 
	CString cstatu;

	/* Prototypen */
	//double falt_func(double x, double pos, double FWHMG, double FWHML);
	//double trapez_integral(double a, double b, double x, int n, double FWHMG, double FWHML);
	//double simpson_integral(double a, double b, double x, double FWHMG, double FWHML);
	//void polynom_interpolation(double xa[], double ya[], double *y, double *dy);
	//double romberg_integral(double a, double b, double x, double FWHMG, double FWHML);
	//double faltfitfunc(double x, double FWHMG, double FWHML);
	void CalcAndShowFitFunc(void);
	void mnpint(double *pexti, const int i, double *pinti);
	void check_amin(void);
	CString getline(void);
	void exec_set_command(const int com_index);
	void exec_show_command(const int com_index);
	void mnline(double start[], double fstart, double step[], double slope);
	double mncalf(double emat[MNMAXINT][MNMAXINT], double pvec[]);
	void __inline __stdcall mnexin(double pint[]) ;
	void __stdcall mnprint(int prcode) ;
	void __stdcall mnfree(const int k) ;
	void __stdcall mnmatu(const int kode) ;
	void mnwerr(void);
	double __inline __stdcall mndxdi(const int ipar);
	void __stdcall mnpfit(double parx2p[], double pary2p[], double coef2p[]) ;
	void __inline __stdcall mninex(double pint[]);
	void mnpsdf(void);
	void mnmnos(void);
	void mnlims(void);
	int __stdcall mnfixp(const int iint) ;
	void __stdcall mnmnot(const int ilax) ;
	void mnmigr(void);
	void mnderi(void);
	void mnhess(void);
	void mnhes1(void);
	int __stdcall mnvert(double a[MNMAXINT][MNMAXINT], const int n) ;
	void __stdcall mneig(double a[MNMAXINT][MNMAXINT], const int n, double work[], const double precis) ;
	void mnemat(void);
	void mncuve(void);
	void mnseek(void);
	int __stdcall mncros(double *aopt) ;
	int __stdcall mneval(const double anext, double *fnext) ;
	void __stdcall mnhelp(char *comd) ;
	void mnimpr(void);
	void __stdcall mnrazz(const double ynew, double pnew[MNMAXINT], double y[], int *jh, int *jl) ;
	void mnsimp(void);
	void mngrad(void);
	void read_startwerte(char* filename);
	void save_parameter(char* filename);
	int	 save_function(char *filename);
	
	double	gauss(double x, double FWHMG);
	double	lorentz(double x, double FWHML);
	double	efunc(double x, double TAU);
	double	falt_voigt(double x, double pos);
	double	falt_gauss_e(double x, double pos);
	double	falt_lorenz_e(double x, double pos);
	double	falt_voigt_e(double x, double pos);
	double	trapez_integral(double a, double b, double x, int n);
	double	simpson_integral_voigt(double a, double b, double x);
	double	trapez_integral_voigt(double a, double b, double x, int n);
	double	simpson_integral(double a, double b, double x);
	double	voigt(double x, double FWHMG, double FWHML);
	double	gaussexp(double x, double FWHMG, double TAU);
	double	lorentzexp(double x, double FWHML, double TAU);
	double	voigtexp(double x, double FWHMG, double FWHML, double TAU);
	BOOL	look_for_break(void);
};
