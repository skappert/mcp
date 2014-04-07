#include <iostream.h>
#include <fstream.h>
#include <iomanip.h>
#include <math.h>

int channels = 101 ;       // Kanaele im Scan
double increment;
double range = 50;          // Scanning range
int nstep = 100;            // Aufloesung, mit der die Dgl geloest wird
const int nvar = 4;              // Anzahl der Zustaende
double staerkeRf1[4][4];
double staerkeRf2[4][4];
double staerkeRf3[4][4];
double RkdumbX[1000];       // X und Y-Werte des Loesungsfeldes
double RkdumbY[4][1000];    // Dimension muss jeweils >= [nvar][nstep]
double T1 = 0;              // Anfangs- und 
double T2 = 10;             // Endzeitpunkt fuer rf-Einstrahlung
double nuQ = 20;            // Quadrupolfrequenz  
double nuL = 1000;              
double nu_rf;               // Radiofrequenz
double nu_rf0;              // Startfrquenz im 3-rf-scan; idealerweise = nuL
double nu1;
double deltanuL;
const int nmax = 2000;

// Faltungsparameter:
double sigma;
double intrange = 20;
int nint = 100;
double om;
double omi;
double pop_unNorm[4];

#define M_PI 3.141592

//****************************************
//******** FUNKTIONEN UND ROUTINEN:*******
//****************************************


double fktgauss(double x, double x0, double sig)
{
    double gauss = 1/(sqrt(2*M_PI)*sig)*exp(-(x-x0)*(x-x0)/(2*sig*sig));
    return gauss;
}

double polarization(double pop1,double pop2,double pop3, double pop4)
{
	double pola = (pop1*(-1.5) + pop2*(-0.5) + pop3*0.5 + pop4*1.5)/1.5;
	return pola;
}

double energy(double m)
{
	double lage = nuL*m + nuQ/12.0*(3*m*m -15.0/4.0);
	return lage;
}

double Rf1(double m1, double m2, double om)
{
	double freq; 
	double dnu;
	double lorentz;
	if (energy(m2)>energy(m1))
	{
		freq = energy(m2) - energy(m1);
	}
	else 
	{
		freq = energy(m1) - energy(m2);
	}
	dnu  = freq - om ;
	lorentz = nu1*nu1/(dnu*dnu + nu1*nu1);
	return lorentz;
}


double Rf2(double m1, double m2, double om)
{
	double freq;  
	if (energy(m2)>energy(m1))
	{
		freq = energy(m2) - energy(m1);
	} 
	else 
	{
		freq = energy(m1) - energy(m2);
	}
	double dnu  = freq - (2*nu_rf0-om) ;
	double lorentz = nu1*nu1/(dnu*dnu + nu1*nu1);
	return lorentz;
}

 
double Rf3(double m1, double m2, double om)
{
	double freq;  
	if (energy(m2)>energy(m1))
	{
		freq = energy(m2) - energy(m1);
	} 
	else 
	{
		freq = energy(m1) - energy(m2);
	}
	double dnu  = freq - (nu_rf0) ;
	double lorentz = nu1*nu1/(dnu*dnu + nu1*nu1);
	return lorentz;
}
 
// HIER SOLLTE MAN SICH UM 2-QT-TRANSITIONS KUEMMERN
//double staerke2(double m1, double m2){
//  double freq;  
//  if (energy(m2)>energy(m1)){
//    freq = energy(m2) - energy(m1);
//  } else {
//    freq = energy(m1) - energy(m2);
//  };
//  double dnu  = freq/2 - nu_rf ;
//  double breite2qt = nu1*nu1/(nuQ*6);  
//  double lorentz =  breite2qt*breite2qt/(dnu*dnu + breite2qt*breite2qt);
//  return lorentz;
//};

void derivs(double x,double* v, double* dvdx)
  // Hier wird die Dgl definiert
{
    double a[nvar][nvar];
    // Matrixelemente

    a[0][0] = 0;
    a[0][1] = 2*sqrt((double)3) * (staerkeRf1[0][1] + staerkeRf2[0][1] + staerkeRf3[0][1]);
    a[0][2] = 0;
    a[0][3] = 0;
    a[1][0] = 2*sqrt((double)3) * (staerkeRf1[1][0] + staerkeRf2[1][0] + staerkeRf3[1][0]);
    a[1][1] = 0;
    a[1][2] = 2 * (staerkeRf1[1][2] + staerkeRf2[1][2] + staerkeRf3[1][2]);
    a[1][3] = 0;
    a[2][0] = 0;
    a[2][1] = 2 * (staerkeRf1[2][1] + staerkeRf2[2][1] + staerkeRf3[2][1]);
    a[2][2] = 0;
    a[2][3] = 2*sqrt((double)3) * (staerkeRf1[2][3] + staerkeRf2[2][3] + staerkeRf3[2][3]);
    a[3][0] = 0;
    a[3][1] = 0;
    a[3][2] = 2*sqrt((double)3) * (staerkeRf1[3][2] + staerkeRf2[3][2] + staerkeRf3[3][2]);
    a[3][3] = 0;

    // Dgl's:
    for (int i=0; i< nvar ; i++)
	{
		dvdx[i] = 0;
		for (int j=0; j< nvar ; j++)
		{
			dvdx[i] += a[i][j]*(v[j]-v[i]) ;
		}
    }
}


void rk4(double* y,double* dydx, int n, double x,double h, double* yout)
{
  // Runge-Kutta Algorithmus
	int i;
	double dym[nmax];
	double dyt[nmax];
	double yt[nmax];

	double hh = h*0.5;
	double h6 = h/6.0;
	double xh = x+hh;

	for (i=0; i< n ; i++)
	{
		yt[i] = y[i]+hh*dydx[i];
	}
	derivs(xh,yt,dyt);
  
	for (i=0; i<n ; i++)
	{
		yt[i] = y[i]+hh*dyt[i];
    }
	derivs(xh,yt,dym);
	for (i=0; i< n ; i++)
	{
		yt[i] = y[i]+h*dym[i];
		dym[i] = dyt[i]+dym[i];
	}
  
	derivs(x+h,yt,dyt);

	for (i=0; i< n ; i++)
	{
		yout[i] = y[i]+h6*(dydx[i]+dyt[i]+2.0*dym[i]);
	}    
}
  
void rkdumb(double* vstart, int n, double x1, double x2, int nstep){

	double v[nmax];
	double vout[nmax];
	double dv[nmax];
	double x,h;    

	for (int i=0; i< n ; i++)
	{
		v[i] = vstart[i];     
		RkdumbY[i][1] = v[i];
	}
       
	RkdumbX[1] = x1;
	x = x1;
	h = (x2-x1)/nstep;
    for (int k=0; k<nstep ; k++)
	{
		derivs(x,v,dv);
		rk4(v,dv,n,x,h,vout);	
		x = x+h;
		RkdumbX[k+1] = x;
		for (int i=0; i<n ; i++)
		{          
			v[i] =vout[i];
			RkdumbY[i][k+1] = v[i];	  
        }
    }
}

//************************************
//*******   HAUPTPROGRAMM     ********
//************************************


int main() 
{
	// Parameter einlesen
	//  cout<< "nuL ?"<< endl;         // ist im Moment auf 1000 fixiert
	//  cin>>  nuL ; 
	cout<< "nu1 ?"<< endl;
	cin>>  nu1 ; 
	cout<< "nu_rf0 - nuL ?"<< endl;
	cin>>  deltanuL ; 
	nu_rf0 = nuL + deltanuL;
	cout<< "Gauss Breite ?"<< endl;
	cin>>  sigma ; 

	int i;
 
	// Startwerte setzen
	double wert0[nvar]; 
	for (i=0; i<nvar  ; i++)
	{
		wert0[i] = i/6.0;                //  lineare Besetzungsverteilung
		cout << wert0[i] << endl;
	}   
	cout << "Anfangspol =  "<< polarization(wert0[0],wert0[1],wert0[2],wert0[3])<< endl;

	//Output file wird definiert   
	ofstream OutputFile("spc.dat", ios::out ); 
	/*  
	if ( !OutputFile )               // If the file cannot be opened
	{                
		cerr << "Output file could not be opened\n"; 
		exit( -1);
	}
	*/

	//  HAUPTSCHLEIFE UEBER DIE KANAELE IM SPEKTRUM:
	for (int k=0; k<channels  ; k++)
	{    
		increment = range/channels;
		double sum = 0;
		nu_rf = nuL -range/2 + k*increment; 

		// BEGIN DER FALTUNGSSCHLEIFE:
		double intstart = nu_rf - intrange*0.5;
		for (int n=0; n<nint+1; n++)
		{ 
			double omi = intstart + ((double(n+0.5)))*(intrange)/((double(nint))+1);

			//UEBERGANGSSTAERKEN DEFINIEREN:
			for (int n1=0; n1<nvar  ; n1++)
			{   
				for (int n2=0; n2<nvar  ; n2++)
				{    
					staerkeRf1[n1][n2] = Rf1(n1-1.5,n2-1.5,omi);
					staerkeRf2[n1][n2] = Rf2(n1-1.5,n2-1.5,omi);
					staerkeRf3[n1][n2] = Rf3(n1-1.5,n2-1.5,omi);
				}
			}	 
			// RUNGE-KUTTA AUFRUFEN:
			rkdumb(wert0,nvar,T1,T2,nstep-1);         
      
			//Normieren der Besetzungszahlen:
			double popu[nvar];
			double norm = 0;
			for (i=0; i<nvar  ; i++)
			{    
				norm += RkdumbY[i][nstep-1];;
			}
			//normierte Besetzung berechnen
			for (i=0; i<nvar  ; i++)
			{    
				popu[i] = RkdumbY[i][nstep-1]/norm;	
			}	   
			//Polarisation berechnen
			double nuclpol = polarization(popu[0],popu[1],popu[2],popu[3]);

			//FALTUNG DER POLARISATION MIT GAUSS
			sum += fktgauss(omi ,nu_rf, sigma) * nuclpol * (intrange)/(((double)(nint))+1);
    
		}         // Faltung zu Ende
		// Auf File schreiben
		OutputFile << nu_rf << " "<< sum << endl; 
	}             // KANALSCHLEIFE ZU ENDE
  
  OutputFile.close();

return 0;
}











