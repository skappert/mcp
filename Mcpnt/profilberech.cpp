/* Berechnung des Faltungsintegrals */
double vintegral (double stelle,double gb,double lb)
{
double min;
/* Integrationschrittanzahl und Integrationsschrittgröße */
double intanz, intschritt;
double gy, ly, gwert, lwert;
double sum = 0;
double gbh, lbh;
int z;


/* Integrationsschrittweite 1/20 der kleineren Breite */
if (gb < lb)
   min = gb;
else
   min = lb;
intschritt = min/20;
intanz = 5*gb/intschritt; /* Integrationsgrenze +- 5*Gaussbreite */

/* Umwandlung in Halbwertsbreite */
gbh = gb/2;
lbh = lb/2;

/* Durchführen der nummerischen Integration */
for (z = 1; z <= 2*intanz+1; z++)
  {
    gy = intschritt * (z-intanz-1);
	gwert = exp(-(gy*gy)/(gbh*gbh));
	ly = stelle - gy;
	lwert = 1/(1+(ly*ly)/(lbh*lbh));
	sum = sum + lwert*gwert;
  }
    ergeb = sum/(2*intanz+2);
	return ergeb;
}

void voigtberech (double voigtwerte[], double gb, double lb)
{
double fnorm;
int i;
double stelle;
double schritt;

/* Wert im Peakzentrum zur Normalisation berechnen */
	vintegral (0, gb, lb);
	fnorm = ergeb/1.17068;

/* Berechnung der Schrittgroesse, bei einer Schrittanzahl von 1001*/
	schritt = (2*grenze)/1000;



	for ( i = 0; i < 1001;i++)
	{
        stelle = -grenze + i*schritt;
		vintegral (stelle, gb, lb);
		voigtwerte[i] = ergeb/fnorm;
	}
}

