// minuit.cpp : implementation file
//
 
#include "stdafx.h"
#include "MCP for NT.h"
#include "MinuitChild.h"
#include "minuit.h"
#include "LSIChild.h"
#include "LSIView.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "MCP for NTDoc.h"
#include "ActionObjects.h"
#include "DataChild.h"
#include "DataView.h"
#include "MCP for NTView.h"

#define FontDim 15

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <stdarg.h>
#include "user.h"

#define Pi  3.14159265359

#define NSIGMA			(5.)                  /*Integrationsfenster*/
#define NTAU 			(7.)                  /*Integrationsfenster*/
#define SIMPSON_EPS		(1E-4)                /*Integrationsgenauigkeit*/
#define MAX_STEPS		(10.)

#define sqr( a ) a * a

#define M_unendlich par[1]
#define omega		par[2]
#define omega1		par[3]
#define tau1		par[4]
#define tau2		par[5]

#define xyz_offset1		par[0]
#define xyz_offset2		par[1]
#define xyz_offset3		par[2]
#define xyz_sign_x		par[3]
#define xyz_amp_z		par[4]
#define xyz_M_unendlich par[5]
#define xyz_omega_x		par[6]
#define xyz_omega_y		par[7]
#define xyz_omega_z		par[8]
#define xyz_omega1		par[9]
#define xyz_tau1		par[10]
#define xyz_tau2		par[11]

/* Konstanten und Typdefinitionen: */
/*---------------------------------*/
/*   Ursprung: National Bureau of Standards 92(2),March-April87 */

  const double c	= 2.99792458e8;       /* m/s */
  const double c2	= c*c;
  const double m	= 1.6605402e-27;     /* kg (Masseneinheit) */
  const double c2m	= c2*m;
  const double e	= 1.60217733e-19;     /* A*s */
  const double h	= 6.6260755e-34;	/* unbedingt genauer !!!*/
  const double h2	= h*h;
  const double AMU	= 1.6605402e-27; /* kg */

const       MaxCommands = 200;

const CString covmes[]={"NO ERROR MATRIX", "ERR MATRIX APPROXIMATE",
    "ERR MATRIX NOT POS-DEF", " ERROR MATRIX ACCURATE"};


IMPLEMENT_DYNCREATE(CMinuit, CEditView)

CMinuit::CMinuit()
{
}

CMinuit::CMinuit(DataView* pParent,CString Title,
				 int in_fitfunc,
				 int in_sign,
				 int em_start,
				 int eb_start,
				 BOOL in_equalspaced,
				 int in_numofpeaks,
				 FITPAR FitPar[20])
{
	// TODO: add construction code here
	int i;

	HAVEOWNER		= TRUE;
	CharCount		= 0;
	OLDCOMMAND		= FALSE;
	FASTCALC		= TRUE;
	pResView		= NULL;
	STOPME			= FALSE;

	A_u_last		= 0;
	B_u_last		= 0;
	A_o_last		= 0;
	B_o_last		= 0;	
	ISV_last		= 0;	
	U_beschl_last	= 0;

	pDataView		= pParent;
	numofpeaks		= in_numofpeaks;
	sign			= in_sign;
	fitfunc			= in_fitfunc;
	equalspaced		= in_equalspaced;

	com_count = 0;
	lambda_start=0.1;
	start_index = 0;
	end_index=pDataView->ActualIndex-1;
	chiq_function = 0;
	CRect rect;
	lwarn=TRUE;
	limset=FALSE;

	IsActivated=FALSE;
	istrat=1;
	itaur=0;
	
	pMinuitChild = new CMinuitChild(this);
	rect.top            = 20;
	rect.left           = 10;
	rect.right          = 550;
	rect.bottom         = 600;
	pMinuitChild->CMDIChildWnd::Create(NULL,
						"Minuit fit: "+Title, 
						WS_CHILD|WS_VISIBLE|WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 
						rect, 
						NULL, 
						NULL );

	this->Create(NULL,NULL,dwStyleDefault,rect,pMinuitChild,AFX_IDW_PANE_FIRST+1,NULL);
	IsActivated=TRUE;
	pMinuitChild->GetClientRect(rect);
	ResizeWin(rect.right,rect.bottom);
	LOGFONT logFont1;
    logFont1.lfHeight = FontDim;
    logFont1.lfWidth = 0;
    logFont1.lfEscapement = 0;
    logFont1.lfOrientation = 0;
    logFont1.lfWeight = FW_NORMAL;
    logFont1.lfItalic = 0;
    logFont1.lfUnderline = 0;
    logFont1.lfStrikeOut = 0;
    logFont1.lfCharSet = ANSI_CHARSET;
    logFont1.lfOutPrecision = OUT_DEFAULT_PRECIS;
    logFont1.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    logFont1.lfQuality = PROOF_QUALITY;
    logFont1.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	strcpy(logFont1.lfFaceName, "Courier New");
	NewFont = new CFont();
	NewFont->CreateFontIndirect(&logFont1);
	SetFont(NewFont,FALSE);

	mninit();
	
	char ParName[30];

	double max  = pDataView->DataPoint[0][pDataView->ActualIndex-1];
	double min  = pDataView->DataPoint[0][0];
	double FWHMG_DEF = fabs(max-min)/40;
	double FWHML_DEF = fabs(max-min)/40;
	double FWHMG_KEP = fabs(max-min)/60;
	double FWHML_KEP = fabs(max-min)/60;
	double TAU_DEF	 = fabs(max-min)/70;

	/*Gauss/Lorentz-FIT */
	if (fitfunc==20||fitfunc==21)
	{
		
	    if (!equalspaced)
		{
			mnparm("b",FitPar[numofpeaks].y,fabs(eb_start*FitPar[numofpeaks].y/10),0,0);
			mnparm("m",0,em_start,0,0);
			if (fitfunc == 20)
			{
				mnparm("FWHMG",FWHMG_DEF,FWHMG_DEF/10,0,0);
			}
			else 
			{
				mnparm("FWHML",FWHML_DEF,FWHML_DEF/10,0,0);
			}
			for (i=0;i<numofpeaks;i++)
			{
				sprintf(ParName,"%u.pos",i+1);
				mnparm(ParName,FitPar[i].x,fabs(FitPar[i].x/10),0,0);
				sprintf(ParName,"%u.amp",i+1);
				if (fitfunc == 20)
				{
					mnparm(ParName,fabs(FitPar[i].y-FitPar[numofpeaks].y),fabs(pDataView->maxY-pDataView->minY)/100,0,0);
				}
				else 
				{
					mnparm(ParName,fabs(FitPar[i].y-FitPar[numofpeaks].y),fabs(pDataView->maxY-pDataView->minY)/100,0,0);
				}
			}
		}
		else
		{
			
			mnparm("b",FitPar[2].y,fabs(eb_start*FitPar[2].y/10),0,0);
			mnparm("m",0,em_start,0,0);
			if (fitfunc == 20)
			{
				mnparm("FWHMG",FWHMG_DEF,FWHMG_DEF/10,0,0);
			}
			else 
			{
				mnparm("FWHML",FWHML_DEF,FWHML_DEF/10,0,0);
			}
			mnparm("1st.pos",FitPar[1].x,fabs(FitPar[1].x/10),0,0);
			if(fitfunc == 20)
			{
				mnparm("all.delta",((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),fabs((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),0,0);
 				for (i=0;i<numofpeaks;i++)
				{
					sprintf(ParName,"%u.amp",i+1);
					mnparm(ParName,fabs(FitPar[1].y-FitPar[2].y),fabs(FitPar[1].y/10),0,0);
				}
			}
			else
			{
				
				mnparm("all.delta",((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),fabs((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),0,0);
 				for (i=0;i<numofpeaks;i++)
				{
					sprintf(ParName,"%u.amp",i+1);
					mnparm(ParName,fabs(FitPar[1].y-FitPar[2].y),fabs(FitPar[1].y/10),0,0);
				}

			}

		}
	}

	/* Voigt-FIT */
	if (fitfunc==22)
	{
		
		if(!equalspaced)
		{
			mnparm("b",FitPar[numofpeaks].y,fabs(eb_start*FitPar[numofpeaks].y/10),0,0);
			mnparm("m",0,em_start,0,0);
			mnparm("FWHMG",FWHMG_DEF,FWHMG_DEF/10,0,0);
			mnparm("FWHML",FWHML_DEF,FWHML_DEF/10,0,0);
			for (i=0;i<numofpeaks;i++)
			{
				sprintf(ParName,"%u.pos",i+1);
				mnparm(ParName,FitPar[i].x,fabs(FitPar[i].x/10),0,0);
				sprintf(ParName,"%u.amp",i+1);
				mnparm(ParName,fabs(FitPar[i].y-FitPar[numofpeaks].y),fabs(FitPar[i].y-FitPar[numofpeaks].y)/10,0,0);
				
			}
		}
		else
		{
			mnparm("b",FitPar[2].y,fabs(eb_start*FitPar[2].y/10),0,0);
			mnparm("m",0,em_start,0,0);
			mnparm("FWHMG",FWHMG_DEF,FWHMG_DEF/10,0,0);
			mnparm("FWHML",FWHML_DEF,FWHML_DEF/10,0,0);
			mnparm("1st.pos",FitPar[1].x,fabs(FitPar[1].x/10),0,0);
			mnparm("all.delta",((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),fabs((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),0,0);
 			for (i=0;i<numofpeaks;i++)
			{
				sprintf(ParName,"%u.amp",i+1);
				mnparm(ParName,fabs(FitPar[1].y-FitPar[2].y),fabs(FitPar[1].y/10)/10,0,0);
			}
		}
	}

	/* Gauss(*)exp-FIT */
	if (fitfunc==32)
	{
		
		if(!equalspaced)
		{
			mnparm("b",FitPar[numofpeaks].y,fabs(eb_start*FitPar[numofpeaks].y/10),0,0);
			mnparm("m",0,em_start,0,0);
			mnparm("FWHMG",FWHMG_DEF,FWHMG_DEF/10,0,0);
			mnparm("TAU",TAU_DEF,TAU_DEF/10,0,0);
			for (i=0;i<numofpeaks;i++)
			{
				sprintf(ParName,"%u.pos",i+1);
				mnparm(ParName,FitPar[i].x,fabs(FitPar[i].x/10),0,0);
				sprintf(ParName,"%u.amp",i+1);
				mnparm(ParName,fabs(FitPar[i].y-FitPar[numofpeaks].y),fabs(FitPar[i].y-FitPar[numofpeaks].y)/10,0,0);
				
			}
		}
		else
		{
			mnparm("b",FitPar[2].y,fabs(eb_start*FitPar[2].y/10),0,0);
			mnparm("m",0,em_start,0,0);
			mnparm("FWHMG",FWHMG_DEF,FWHMG_DEF/10,0,0);
			mnparm("TAU",TAU_DEF,TAU_DEF/10,0,0);
			mnparm("1st.pos",FitPar[1].x,fabs(FitPar[1].x/10),0,0);
			mnparm("all.delta",((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),fabs((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),0,0);
 			for (i=0;i<numofpeaks;i++)
			{
				sprintf(ParName,"%u.amp",i+1);
				mnparm(ParName,fabs(FitPar[1].y-FitPar[2].y),fabs(FitPar[1].y/10)/10,0,0);
			}
		}
	}

	/* Lorentz(*)exp-FIT */
	if (fitfunc==33)
	{
		
		if(!equalspaced)
		{
			mnparm("b",FitPar[numofpeaks].y,fabs(eb_start*FitPar[numofpeaks].y/10),0,0);
			mnparm("m",0,em_start,0,0);
			mnparm("FWHML",FWHML_DEF,FWHML_DEF/10,0,0);
			mnparm("TAU",TAU_DEF,TAU_DEF/10,0,0);
			for (i=0;i<numofpeaks;i++)
			{
				sprintf(ParName,"%u.pos",i+1);
				mnparm(ParName,FitPar[i].x,fabs(FitPar[i].x/10),0,0);
				sprintf(ParName,"%u.amp",i+1);
				mnparm(ParName,fabs(FitPar[i].y-FitPar[numofpeaks].y),fabs(FitPar[i].y-FitPar[numofpeaks].y)/10,0,0);
				
			}
		}
		else
		{
			mnparm("b",FitPar[2].y,fabs(eb_start*FitPar[2].y/10),0,0);
			mnparm("m",0,em_start,0,0);
			mnparm("FWHML",FWHML_DEF,FWHML_DEF/10,0,0);
			mnparm("TAU",TAU_DEF,TAU_DEF/10,0,0);
			mnparm("1st.pos",FitPar[1].x,fabs(FitPar[1].x/10),0,0);
			mnparm("all.delta",((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),fabs((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),0,0);
 			for (i=0;i<numofpeaks;i++)
			{
				sprintf(ParName,"%u.amp",i+1);
				mnparm(ParName,fabs(FitPar[1].y-FitPar[2].y),fabs(FitPar[1].y/10)/10,0,0);
			}
		}
	}

	/* Voigt(*)exp-FIT */
	if (fitfunc==34)
	{
		
		if(!equalspaced)
		{
			mnparm("b",FitPar[numofpeaks].y,fabs(eb_start*FitPar[numofpeaks].y/10),0,0);
			mnparm("m",0,em_start,0,0);
			mnparm("FWHMG",FWHMG_DEF,FWHMG_DEF/10,0,0);
			mnparm("FWHML",FWHML_DEF,FWHML_DEF/10,0,0);
			mnparm("TAU",TAU_DEF,TAU_DEF/10,0,0);
			for (i=0;i<numofpeaks;i++)
			{
				sprintf(ParName,"%u.pos",i+1);
				mnparm(ParName,FitPar[i].x,fabs(FitPar[i].x/10),0,0);
				sprintf(ParName,"%u.amp",i+1);
				mnparm(ParName,fabs(FitPar[i].y-FitPar[numofpeaks].y),fabs(FitPar[i].y-FitPar[numofpeaks].y)/10,0,0);
			}
		}
		else
		{
			mnparm("b",FitPar[2].y,fabs(eb_start*FitPar[2].y/10),0,0);
			mnparm("m",0,em_start,0,0);
			mnparm("FWHMG",FWHMG_DEF,FWHMG_DEF/10,0,0);
			mnparm("FWHML",FWHML_DEF,FWHML_DEF/10,0,0);
			mnparm("TAU",TAU_DEF,TAU_DEF/10,0,0);
			mnparm("1st.pos",FitPar[1].x,fabs(FitPar[1].x/10),0,0);
			mnparm("all.delta",((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),fabs((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),0,0);
 			for (i=0;i<numofpeaks;i++)
			{
				sprintf(ParName,"%u.amp",i+1);
				mnparm(ParName,fabs(FitPar[1].y-FitPar[2].y),fabs(FitPar[1].y/10)/10,0,0);
			}
		}
	}


	/*Gauss/Lorentz-FIT */
	if (fitfunc==0||fitfunc==1)
	{
		
	    if (!equalspaced)
		{
			mnparm("b",FitPar[numofpeaks].y,fabs(eb_start*FitPar[numofpeaks].y/10),0,0);
			mnparm("m",0,em_start,0,0);
			for (i=0;i<numofpeaks;i++)
			{
				sprintf(ParName,"%u.pos",i+1);
				mnparm(ParName,FitPar[i].x,fabs(FitPar[i].x/10),0,0);
				sprintf(ParName,"%u.amp",i+1);
				if (fitfunc == 0)
				{
					mnparm(ParName,fabs(FitPar[i].y-FitPar[numofpeaks].y),fabs(pDataView->maxY-pDataView->minY)/10,0,0);
					sprintf(ParName,"%u.FWHMG",i+1);
					mnparm(ParName,FWHMG_DEF,FWHMG_DEF/10,0,0);
				}
				else 
				{
					mnparm(ParName,fabs(FitPar[i].y-FitPar[numofpeaks].y),fabs(pDataView->maxY-pDataView->minY)/10,0,0);
					sprintf(ParName,"%u.FWHML",i+1);
					mnparm(ParName,FWHML_DEF,FWHML_DEF/10,0,0);
				}
			}
		}
		else
		{
			
			mnparm("b",FitPar[2].y,fabs(eb_start*FitPar[2].y/10),0,0);
			mnparm("m",0,em_start,0,0);
			mnparm("1st.pos",FitPar[1].x,fabs(FitPar[1].x/10),0,0);

			if(fitfunc == 0)
			{
				mnparm("all.delta",((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),fabs((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),0,0);
 				for (i=0;i<numofpeaks;i++)
				{
					sprintf(ParName,"%u.amp",i+1);
					mnparm(ParName,fabs(FitPar[1].y-FitPar[2].y),fabs(FitPar[1].y-FitPar[2].y)/10,0,0);
					sprintf(ParName,"%u.FWHMG",i+1);
					mnparm(ParName,FWHMG_DEF,FWHMG_DEF/10,0,0);
				}
			}
			else
			{
				
				mnparm("all.delta",((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),fabs((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),0,0);
 				for (i=0;i<numofpeaks;i++)
				{
					sprintf(ParName,"%u.amp",i+1);
					mnparm(ParName,fabs(FitPar[1].y-FitPar[2].y),fabs(FitPar[1].y-FitPar[2].y)/10,0,0);
					sprintf(ParName,"%u.FWHML",i+1);
					mnparm(ParName,FWHML_DEF,FWHML_DEF/10,0,0);
				}

			}

		}
	}

	/* Voigt-FIT */
	if (fitfunc==2)
	{
		
		if(!equalspaced)
		{
			mnparm("b",FitPar[numofpeaks].y,fabs(eb_start*FitPar[numofpeaks].y/10),0,0);
			mnparm("m",0,em_start,0,0);
			for (i=0;i<numofpeaks;i++)
			{
				sprintf(ParName,"%u.pos",i+1);
				mnparm(ParName,FitPar[i].x,fabs(FitPar[i].x/10),0,0);
				sprintf(ParName,"%u.amp",i+1);
				mnparm(ParName,fabs(FitPar[i].y-FitPar[numofpeaks].y),fabs(FitPar[i].y-FitPar[numofpeaks].y)/10,0,0);
				sprintf(ParName,"%u.FWHMG",i+1);
				mnparm(ParName,FWHMG_DEF,FWHMG_DEF/10,0,0);
				sprintf(ParName,"%u.FWHML",i+1);
				mnparm(ParName,FWHML_DEF,FWHML_DEF/10,0,0);
			}
		}
		else
		{
			mnparm("b",FitPar[2].y,fabs(eb_start*FitPar[2].y/10),0,0);
			mnparm("m",0,em_start,0,0);
			mnparm("1st.pos",FitPar[1].x,fabs(FitPar[1].x/10),0,0);
			mnparm("all.delta",((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),fabs((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),0,0);
 			for (i=0;i<numofpeaks;i++)
			{
				sprintf(ParName,"%u.amp",i+1);
				mnparm(ParName,fabs(FitPar[1].y-FitPar[2].y),fabs(FitPar[1].y-FitPar[2].y)/10,0,0);
				sprintf(ParName,"%u.FWHMG",i+1);
				mnparm(ParName,FWHMG_DEF,FWHMG_DEF/10,0,0);
				sprintf(ParName,"%u.FWHML",i+1);
				mnparm(ParName,FWHML_DEF,FWHML_DEF/10,0,0);
			}
		}
	}

	/* Gauss(*)exp-FIT */
	if (fitfunc==12)
	{
		
		if(!equalspaced)
		{
			mnparm("b",FitPar[numofpeaks].y,fabs(eb_start*FitPar[numofpeaks].y/10),0,0);
			mnparm("m",0,em_start,0,0);
			for (i=0;i<numofpeaks;i++)
			{
				sprintf(ParName,"%u.pos",i+1);
				mnparm(ParName,FitPar[i].x,fabs(FitPar[i].x/10),0,0);
				sprintf(ParName,"%u.amp",i+1);
				mnparm(ParName,fabs(FitPar[i].y-FitPar[numofpeaks].y),fabs(FitPar[i].y-FitPar[numofpeaks].y)/10,0,0);
				sprintf(ParName,"%u.FWHMG",i+1);
				mnparm(ParName,FWHMG_DEF,FWHMG_DEF/10,0,0);
				sprintf(ParName,"%u.TAU",i+1);
				mnparm(ParName,TAU_DEF,TAU_DEF/10,0,0);
			}
		}
		else
		{
			mnparm("b",FitPar[2].y,fabs(eb_start*FitPar[2].y/10),0,0);
			mnparm("m",0,em_start,0,0);
			mnparm("1st.pos",FitPar[1].x,fabs(FitPar[1].x/10),0,0);
			mnparm("all.delta",((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),fabs((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),0,0);
 			for (i=0;i<numofpeaks;i++)
			{
				sprintf(ParName,"%u.amp",i+1);
				mnparm(ParName,fabs(FitPar[1].y-FitPar[2].y),fabs(FitPar[1].y-FitPar[2].y)/10,0,0);
				sprintf(ParName,"%u.FWHMG",i+1);
				mnparm(ParName,FWHMG_DEF,FWHMG_DEF/10,0,0);
				sprintf(ParName,"%u.TAU",i+1);
				mnparm(ParName,TAU_DEF,TAU_DEF/10,0,0);
			}
		}
	}

	/* Lorentz(*)exp-FIT */
	if (fitfunc==13)
	{
		
		if(!equalspaced)
		{
			mnparm("b",FitPar[numofpeaks].y,fabs(eb_start*FitPar[numofpeaks].y/10),0,0);
			mnparm("m",0,em_start,0,0);
			for (i=0;i<numofpeaks;i++)
			{
				sprintf(ParName,"%u.pos",i+1);
				mnparm(ParName,FitPar[i].x,fabs(FitPar[i].x/10),0,0);
				sprintf(ParName,"%u.amp",i+1);
				mnparm(ParName,fabs(FitPar[i].y-FitPar[numofpeaks].y),fabs(FitPar[i].y-FitPar[numofpeaks].y)/10,0,0);
				sprintf(ParName,"%u.FWHML",i+1);
				mnparm(ParName,FWHML_DEF,FWHML_DEF/10,0,0);
				sprintf(ParName,"%u.TAU",i+1);
				mnparm(ParName,TAU_DEF,TAU_DEF/10,0,0);
			}
		}
		else
		{
			mnparm("b",FitPar[2].y,fabs(eb_start*FitPar[2].y/10),0,0);
			mnparm("m",0,em_start,0,0);
			mnparm("1st.pos",FitPar[1].x,fabs(FitPar[1].x/10),0,0);
			mnparm("all.delta",((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),fabs((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),0,0);
 			for (i=0;i<numofpeaks;i++)
			{
				sprintf(ParName,"%u.amp",i+1);
				mnparm(ParName,fabs(FitPar[1].y-FitPar[2].y),fabs(FitPar[1].y-FitPar[2].y)/10,0,0);
				sprintf(ParName,"%u.FWHML",i+1);
				mnparm(ParName,FWHML_DEF,FWHML_DEF/10,0,0);
				sprintf(ParName,"%u.TAU",i+1);
				mnparm(ParName,TAU_DEF,TAU_DEF/10,0,0);
			}
		}
	}

	/* Voigt(*)exp-FIT */
	if (fitfunc==14)
	{
		
		if(!equalspaced)
		{
			mnparm("b",FitPar[numofpeaks].y,fabs(eb_start*FitPar[numofpeaks].y/10),0,0);
			mnparm("m",0,em_start,0,0);
			for (i=0;i<numofpeaks;i++)
			{
				sprintf(ParName,"%u.pos",i+1);
				mnparm(ParName,FitPar[i].x,fabs(FitPar[i].x/10),0,0);
				sprintf(ParName,"%u.amp",i+1);
				mnparm(ParName,fabs(FitPar[i].y-FitPar[numofpeaks].y),fabs(FitPar[i].y-FitPar[numofpeaks].y)/10,0,0);
				sprintf(ParName,"%u.FWHMG",i+1);
				mnparm(ParName,FWHMG_DEF,FWHMG_DEF/10,0,0);
				sprintf(ParName,"%u.FWHML",i+1);
				mnparm(ParName,FWHML_DEF,FWHML_DEF/10,0,0);
				sprintf(ParName,"%u.TAU",i+1);
				mnparm(ParName,TAU_DEF,TAU_DEF/10,0,0);
			}
		}
		else
		{
			mnparm("b",FitPar[2].y,fabs(eb_start*FitPar[2].y/10),0,0);
			mnparm("m",0,em_start,0,0);
			mnparm("1st.pos",FitPar[1].x,fabs(FitPar[1].x/10),0,0);
			mnparm("all.delta",((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),fabs((FitPar[0].x-FitPar[1].x)/(numofpeaks-1)),0,0);
 			for (i=0;i<numofpeaks;i++)
			{
				sprintf(ParName,"%u.amp",i+1);
				mnparm(ParName,fabs(FitPar[1].y-FitPar[2].y),fabs(FitPar[1].y-FitPar[2].y)/10,0,0);
				sprintf(ParName,"%u.FWHMG",i+1);
				mnparm(ParName,FWHMG_DEF,FWHMG_DEF/10,0,0);
				sprintf(ParName,"%u.FWHML",i+1);
				mnparm(ParName,FWHML_DEF,FWHML_DEF/10,0,0);
				sprintf(ParName,"%u.TAU",i+1);
				mnparm(ParName,TAU_DEF,TAU_DEF/10,0,0);
			}
		}
	}

	/* Decay-FIT */
	if (fitfunc==3)
	{
		while (min+(start_index*(max-min)/(pDataView->ActualIndex-1))<FitPar[numofpeaks-1].x)start_index++;

		mnparm("b",FitPar[numofpeaks].y,fabs(eb_start*FitPar[numofpeaks].y/10),0,0);
		for (i=0;i<numofpeaks;i++)
		{
			sprintf(ParName,"%u.amp",i+1);
			mnparm(ParName,fabs(FitPar[numofpeaks-1].y-FitPar[numofpeaks].y)/numofpeaks,fabs(FitPar[numofpeaks-1].y-FitPar[numofpeaks].y)/10,0,0);
			sprintf(ParName,"%u.tau",i+1);
			mnparm(ParName,fabs(max-min)/10,fabs(max-min)/100,0,0);
		}

	}

	/* Linear-FIT */
	if (fitfunc==4)
	{
		double m = (pDataView->DataPoint[1][pDataView->ActualIndex-1]-pDataView->DataPoint[1][0])/
			(pDataView->DataPoint[0][pDataView->ActualIndex-1]-pDataView->DataPoint[0][0]);
		double b = pDataView->DataPoint[1][0]-m*pDataView->DataPoint[0][0];
		mnparm("b",b,eb_start,0,0);
		mnparm("m",m,em_start,0,0);
	}


	/* HFS-FIT */
	if (fitfunc==5||fitfunc==6||fitfunc==7)
	{
		int i,j=0;
		double dUmin,accel,amp;

		Wellenzahl		= FitPar[4].y;
		I				= FitPar[5].y;
		Am				= FitPar[6].y;
		J_grund			= FitPar[7].y;
		J_an			= FitPar[8].y;
		ISV				= FitPar[9].y;
		U_beschl		= FitPar[10].y;
		ReferenzMasse	= FitPar[11].y;
		Haeufigkeit		= FitPar[12].y;

		mnparm("b",FitPar[14].y,fabs(eb_start*FitPar[14].y/10),0,0);
		mnparm("m",0,em_start,0,0);
		if (fitfunc==5||fitfunc==7)mnparm("FWHMG",FWHMG_KEP,FWHMG_KEP/10,0,0);
		if (fitfunc==6||fitfunc==7)mnparm("FWHML",FWHML_KEP,FWHML_KEP/10,0,0);		
		mnparm("A_ground",FitPar[0].y,fabs(FitPar[0].y/8),0,0);
		mnparm("B_ground",FitPar[1].y,fabs(FitPar[1].y/8),0,0);
		mnparm("A_excited",FitPar[2].y,fabs(FitPar[2].y/8),0,0);
		mnparm("B_excited",FitPar[3].y,fabs(FitPar[3].y/8),0,0);
	    
		AnzahlLinien = BerechneLinien(
						Wellenzahl,
						I,
						Am,
						FitPar[0].y*1e6,FitPar[1].y*1e6,
						FitPar[2].y*1e6,FitPar[3].y*1e6,
						J_grund,
						J_an,
						ISV,
						U_beschl,
						ReferenzMasse,
						Haeufigkeit);
		
		dUmin = Linien[0].dU;

		for(i=1;i<AnzahlLinien;i++)
		{
			if(Linien[i].dU < dUmin) 
			{
				dUmin = Linien[i].dU;
				j = i;
			}
		}
		accel = U_beschl+dUmin+FitPar[15].x; 

		mnparm("U_accel",accel,10,0,0);

		amp = fabs(FitPar[15].y-FitPar[14].y)/Linien[j].RelInt;

		for(i=0;i<AnzahlLinien;i++)
		{
			sprintf(ParName,"%u.amp",i+1);
			mnparm(ParName,Linien[i].RelInt*amp,Linien[i].RelInt*amp/10,0,0);
		}
	}

	if (fitfunc==8||fitfunc==9||fitfunc==10)
	{
		int i,j=0;
		double dUmin,accel,amp;

		Wellenzahl		= FitPar[4].y;
		I				= FitPar[5].y;
		Am				= FitPar[6].y;
		J_grund			= FitPar[7].y;
		J_an			= FitPar[8].y;
		ISV				= FitPar[9].y;
		U_beschl		= FitPar[10].y;
		ReferenzMasse	= FitPar[11].y;
		Haeufigkeit		= FitPar[12].y;

		mnparm("b",FitPar[14].y,fabs(eb_start*FitPar[14].y/10),0,0);
		mnparm("m",0,em_start,0,0);
		if (fitfunc==8||fitfunc==10)mnparm("FWHMG",FWHMG_KEP,FWHMG_KEP/10,0,0);
		if (fitfunc==9||fitfunc==10)mnparm("FWHML",FWHML_KEP,FWHML_KEP/10,0,0);		
		mnparm("A_ground",FitPar[0].y,fabs(FitPar[0].y/8),0,0);
		mnparm("B_ground",FitPar[1].y,fabs(FitPar[1].y/8),0,0);
		mnparm("A_excited",FitPar[2].y,fabs(FitPar[2].y/8),0,0);
		mnparm("B_excited",FitPar[3].y,fabs(FitPar[3].y/8),0,0);
	    
		AnzahlLinien = BerechneLinien(
						Wellenzahl,
						I,
						Am,
						FitPar[0].y*1e6,FitPar[1].y*1e6,
						FitPar[2].y*1e6,FitPar[3].y*1e6,
						J_grund,
						J_an,
						ISV,
						U_beschl,
						ReferenzMasse,
						Haeufigkeit);
		
		dUmin = Linien[0].dU;

		for(i=1;i<AnzahlLinien;i++)
		{
			if(Linien[i].dU < dUmin) 
			{
				dUmin = Linien[i].dU;
				j = i;
			}
		}
		accel = U_beschl+dUmin+FitPar[15].x; 

		mnparm("U_accel",accel,10,0,0);

		amp = fabs(FitPar[15].y-FitPar[14].y)/Linien[j].RelInt;

		for(i=0;i<AnzahlLinien;i++)
		{
			sprintf(ParName,"%u.amp",i+1);
			mnparm(ParName,Linien[i].RelInt*amp,Linien[i].RelInt*amp/10,0,0);
		}
		mnparm("exp.tau",1,0.1,0,0);
	}
		
	if (fitfunc==11)
	{
		int ParNumber;
		double		Value;
		double		Error;
		for(ParNumber=0;GetInitialPars(ParNumber,&ParName[0],&Value,&Error);ParNumber++)
		{
			mnparm(ParName,Value,Error,0,0);
		}
	}

	/* cos*exp */
	if (fitfunc==100)
	{
		mnparm("amplitude",5e4,10,0,0);
		mnparm("offset",2400,10,0,0);
		mnparm("omega1",0.02,1,0,0);
		mnparm("tau",1000,1,0,0);
		mnparm("phase",0,1,0,0);
	}

	/* Mx Bloch */
	if (fitfunc==101)
	{
		mnparm("offset",2400,10,0,0);
		mnparm("M",3e4,1000,0,0);
		mnparm("omega",2000,1,0,0);
		mnparm("omega1",3,1,0,0);
		mnparm("tau1",0.3,0.1,0,0);
		mnparm("tau2",0.15,0.1,0,0);
	}

	/* My Bloch */
	if (fitfunc==102)
	{
		mnparm("offset",2400,10,0,0);
		mnparm("M",3e4,1000,0,0);
		mnparm("omega",2000,1,0,0);
		mnparm("omega1",3,1,0,0);
		mnparm("tau1",0.3,0.1,0,0);
		mnparm("tau2",0.15,0.1,0,0);
	}

	/* Mz Bloch */
	if (fitfunc==103)
	{
		mnparm("offset",2400,10,0,0);
		mnparm("M",3e4,1000,0,0);
		mnparm("omega",2000,1,0,0);
		mnparm("omega1",3,1,0,0);
		mnparm("tau1",0.3,0.1,0,0);
		mnparm("tau2",0.15,0.1,0,0);
	}
/*
#define xyz_offset1		par[0]
#define xyz_offset2		par[1]
#define xyz_offset3		par[2]
#define xyz_sign_x		par[3]
#define xyz_amp_z		par[4]
#define xyz_M_unendlich par[5]
#define xyz_omega_x		par[6]
#define xyz_omega_y		par[7]
#define xyz_omega_z		par[8]
#define xyz_omega1		par[9]
#define xyz_tau1		par[10]
#define xyz_tau2		par[11]
*/
	/* Mxyz Bloch */
	if (fitfunc==104)
	{
		mnparm("offset1",2000,10,0,0);
		mnparm("offset2",2000,10,0,0);
		mnparm("offset3",2000,10,0,0);
		mnparm("sign_x",-1,0,0,0);
		mnparm("amp_z",-100,10,0,0);
		mnparm("M",3e4,1000,0,0);
		mnparm("omega_x",2060,1,0,0);
		mnparm("omega_y",2060,1,0,0);
		mnparm("omega_z",2060,1,0,0);
		mnparm("omega1",3,1,0,0);
		mnparm("tau1",0.3,0.1,0,0);
		mnparm("tau2",0.15,0.1,0,0);
	}

	CalcAndShowFitFunc();

	mninteractive();
}

CMinuit::~CMinuit()
{
	if(pResView!=NULL)pResView->GetParent()->DestroyWindow();
}

BEGIN_MESSAGE_MAP(CMinuit, CEditView)
	//{{AFX_MSG_MAP(CMinuit)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


inline double laserflabor(double f,double b)
	{
	  return f*sqrt((1+b)/(1-b));
	};

  double Min(double r1, double r2)
    {
      if (r1<r2) { return r1; };
      return r2;
    };

  long Round(double d)
    {
      long l1 = (long) d;
      long l2 = (long) (d+0.5);
      if (l2>l1) return l2;
      return l1;
    };

  char UpCase(char ch)
    /* wandelt kleine Buchstaben in grosse Buchstaben um ! */
    {
      if ((ch>=97) && (ch<=122)) { return (ch-32); }
      return ch;
    };




/*=======================================================================*/
/*                 physikalische Hilfsfunktionen                         */
/*=======================================================================*/

  /* Die folgende Formeln fuer die Intensitaeten von HFS-Uebergaengen */
  /* sind aus dem Buch: H. Kopfermann, Kernmomente, S.51ff            */

  double P(double F, double J, double I)
    {
      return ( (F+J)*(F+J+1)-I*(I+1) );
    };

  double Q(double F, double J, double I)
    {
      return ( I*(I+1)-(F-J)*(F-J+1) );
    };

  double R(double F, double J, double I)
    {
      return ( F*(F+1)+J*(J+1)-I*(I+1) );
    };

  double Int_JJ_FFp1(double F, double J, double I)
    /* Intensitaet fuer J->J, F->F+1 */
    {
      return ( P(F+1,J,I)*Q(F,J,I)/(F+1) );
    };

  double Int_JJ_FF(double F, double J, double I)
    /* Intensitaet fuer J->J, F->F */
    {
      return ( R(F,J,I)*R(F,J,I)*(2*F+1)/F/(F+1) );
    };

  double Int_JJ_FFm1(double F, double J, double I)
    /* Intensitaet fuer J->J, F->F-1 */
    {
      return ( P(F,J,I)*Q(F-1,J,I)/F );
    };

  double Int_JJpm1_FFp1(double F, double J, double I)
    /* Intensitaet fuer J->J+/-1, F->F+1 */
    {
      return ( P(F+1,J,I)*P(F+2,J,I)/(F+1) );
    };

  double Int_JJpm1_FF(double F, double J, double I)
    /* Intensitaet fuer J->J+/-1, F->F */
    {
      return ( P(F+1,J,I)*Q(F-1,J,I)*(2*F+1)/F/(F+1) );
    };

  double Int_JJpm1_FFm1(double F, double J, double I)
    /* Intensitaet fuer J->J+/-1, F->F-1 */
    {
      return ( Q(F-1,J,I)*Q(F-2,J,I)/F );
    };

  double Frequenz(double Wellenzahl)
    /* Wellenzahl in 1/cm */
    {
      return (c*Wellenzahl*1e2);
    };

  double Beta(double U, double A)
     /* U = Spannung, A = Massenzahl */
    /* Berechnung ist nun RELATIVISTISCH (24.4.1994) ! */
    {
      double q = 1.0;             // Ladung der Ionen immer 1 !

      U = fabs(U);
      return sqrt( ((q*e*U)*(q*e*U) + 2.0*q*e*U*A*AMU*c2) / ((A*AMU*c2)*(A*AMU*c2) + (q*e*U)*(q*e*U) + 2.0*q*e*U*A*AMU*c2));
    };

  double U(double b, double A)
    /* b = v/c, A = Massenzahl */
    /* Berechnung ist nun RELATIVISTISCH (1.5.1994) ! */
    {
      return (sqrt((A*AMU*c2)*(A*AMU*c2)/(1.0-b*b))-A*AMU*c2)/e;
    };

  double K(double F, double J, double I)
    /* J = Spin, I = Kernspin */
    {
      return (F*(F+1)-J*(J+1)-I*(I+1));
    };

  double E_hfs(double A, double B, double F, double J, double I)
    /* A-Faktor, B-Faktor, Spins */
    {
      double K_Faktor,mu,q;

      K_Faktor = K(F,J,I);
      q = 0.0;
      mu = 0.5*A*K_Faktor;
      if (Min(J,I)>=0.9999)
        {
          q = B * (1.5*K_Faktor*(K_Faktor+1)-2*I*(I+1)*J*(J+1))
                  / (4*I*(2*I-1)*J*(2*J-1));
        };
      return ( mu+q );
    };

  double U_hfs(double fo, double dE, double Am, double Am_ref, double U0)
    /* fo = Resonanzfrequenz im Ruhesystem, dE = Aufspaltungsfrequenz (Hz), */
    /* Am = relative Massenzahl, Am_ref = Massenzahl der Referenz, */
    /* U0 = Spannung der Referenz (Volt) */
    /* liefert negative Spannung ! */
    {
      double fres,b1,b2;
	  /*
      if (U0<0.0) { U0 = fabs(U0); };
      b1 = Beta(U0,Am_ref);   // bestimme Beta der Referenzlinie 
      x = (dE/fo+1);
      x = x*x;
      x = x*(1-b1)/(1+b1);
      b2 = (1-x)/(1+x);
      return -U(b2,Am);
	  
	  */
	  b2 = Beta(U0,Am);
	  fres = fo/sqrt((1-b2)/(1+b2));
	  b1 = (fres*fres-(fo+dE)*(fo+dE))/(fres*fres+(fo+dE)*(fo+dE));
	  return -U(b1,Am);
	  
    };

  int Aufspaltungen(double I, double J)
    {
      double Spin;
      int  Nr = 0;

      Spin = fabs(I-J);
      while (Spin<=(I+J))    /* ACHTUNG: eventuell Rundungsfehler beachten */
        { Nr++; Spin = Spin + 1.0; };
      return Nr;
    };

  int LinieErlaubt(double F_u, double F_o)
    /* Auswahlregel fuer HFS */
    {
      if (fabs(F_u-F_o)<=1)  /* ACHTUNG: eventuell Rundungsfehler beachten */
        {
          if ((F_u==0.0) && (F_o==0))
            { return 0; }
          else
            { return 1; };
        }
      return 0;
    };

  double RelIntensitaet(double F_u, double F_o, double J_u, double J_o, double I)
    /* Voraussetzung: dJ<=1, dF<=1 ! */
    {
      if (J_u==J_o)
        {  /* J->J Uebergaenge: */
          if (F_u==F_o) { return Int_JJ_FF(F_u,J_u,I); };
          if (F_u>F_o) { return Int_JJ_FFm1(F_u,J_u,I); };
          if (F_u<F_o) { return Int_JJ_FFp1(F_u,J_u,I); };
        }
      else
        {  /* J->J+/-1 Uebergaenge: */
          if (F_u==F_o) { return Int_JJpm1_FF(F_u,Min(J_u,J_o),I); };
          if (F_u>F_o) { return Int_JJpm1_FFm1(F_u,Min(J_u,J_o),I); };
          if (F_u<F_o) { return Int_JJpm1_FFp1(F_u,Min(J_u,J_o),I); };
        }
      return 0.0;
    };
 
int CMinuit::BerechneLinien(double Wellenzahl,
							 double I,double Am,
							 double A_u,double B_u,
							 double A_o,double B_o,
							 double J_grund,double J_an,
							 double ISV,double U_beschl,
							 double ReferenzMasse,double Haeufigkeit)
    /* ACHTUNG: Funktion greift auf alle globalen Variablen zu ! */
    {
	  int AnzLinien;
		
	  if(A_u		== A_u_last&&	
		B_u			== B_u_last&&
		A_o			== A_o_last&&	
		B_o			== B_o_last&&	
		ISV			== ISV_last&&	
		U_beschl	== U_beschl_last)
	  {
		  AnzLinien = AnzahlLinien;
		  return AnzLinien;
	  }

		A_u_last		= A_u;
		B_u_last		= B_u;
		A_o_last		= A_o;
		B_o_last		= B_o;	
		ISV_last		= ISV;	
		U_beschl_last	= U_beschl;

      AufspaltungStruct AS_u[MaxNoOfSplits];
      AufspaltungStruct AS_o[MaxNoOfSplits];
      double	F_min,f0,SumSP_u,SumSP_o,SumF_u,SumF_o,MaxInt;
      int		j1,ju,jo,AnzahlAufsp_u,AnzahlAufsp_o,StartAnzahlLinien;

      f0 = Frequenz(Wellenzahl);
      AnzLinien = 0;
          StartAnzahlLinien = AnzLinien;
      /* (i) Berechnung der HFS-Aufspaltung (dE in Hz): */
          AnzahlAufsp_u = Aufspaltungen(I,J_grund);
          F_min = fabs(I-J_grund);
          for (j1=0; j1<AnzahlAufsp_u; j1++)
            {
              AS_u[j1].F = F_min+j1;
              AS_u[j1].dE = E_hfs(A_u,B_u,AS_u[j1].F,J_grund,I);
            };
          AnzahlAufsp_o = Aufspaltungen(I,J_an);
          F_min = fabs(I-J_an);
          for (j1=0; j1<AnzahlAufsp_o; j1++)
            {
              AS_o[j1].F = F_min+j1;
              AS_o[j1].dE = E_hfs(A_o,B_o,AS_o[j1].F,J_an,I);
            };
      /* (ii) Berechnung der erlaubten Linien: dF = 0, +/-1, 0->0 verboten */
          SumSP_u = 0.0;    /* Variablen zur Linien-Schwerpunkt Berechnung */
          SumSP_o = 0.0;
          SumF_u  = 0.0;
          SumF_o  = 0.0;
          for (ju=0; ju<AnzahlAufsp_u; ju++)
            {
              for (jo=0; jo<AnzahlAufsp_o; jo++)
                {
                  if ((LinieErlaubt(AS_u[ju].F,AS_o[jo].F)==1) && (AnzLinien<=MaxNoOfLines))
                    {
                     Linien[AnzLinien].dE = (AS_o[jo].dE-AS_u[ju].dE);
 /* temp: noch dE */ Linien[AnzLinien].dU = (AS_o[jo].dE-AS_u[ju].dE);
      /* (iii) Umrechnung auf Spannungen (Dopplerverschiebung der Isotope): */
      /*       und Isotopieverschiebung addieren: */
					  Linien[AnzLinien].dE = Linien[AnzLinien].dE + ISV;
                      Linien[AnzLinien].dU = Linien[AnzLinien].dU + ISV;
                      /* falls U_beschl==0, keine Umrechnung in Volt !!! */
                      /*                    sondern Umrechnung in MHz !! */
                      if (U_beschl!=0.0)
                        { Linien[AnzLinien].dU = U_hfs(f0,Linien[AnzLinien].dU,Am,ReferenzMasse,U_beschl); }
                      else
                        { Linien[AnzLinien].dU = Linien[AnzLinien].dU*1e-6; };
                      Linien[AnzLinien].F_u = AS_u[ju].F;
                      Linien[AnzLinien].F_o = AS_o[jo].F;
                      Linien[AnzLinien].Am = Am;
                      Linien[AnzLinien].RelInt = RelIntensitaet(Linien[AnzLinien].F_u,Linien[AnzLinien].F_o,
                                                                  J_grund,J_an,I);
                      Linien[AnzLinien].Hoehe = Haeufigkeit;
                      SumSP_u = SumSP_u + AS_u[ju].dE*AS_u[ju].F;
                      SumF_u  = SumF_u + AS_u[ju].F;
                      SumSP_o = SumSP_o + AS_o[jo].dE*AS_o[jo].F;
                      SumF_o  = SumF_o + AS_o[jo].F;
                      AnzLinien++;
                    }
                };
            };
      /* Bestimmung der relativen Linienstaerken: */
          MaxInt = Linien[StartAnzahlLinien].RelInt;
          for (j1=StartAnzahlLinien+1; j1<AnzLinien; j1++)
            { if (MaxInt<Linien[j1].RelInt) { MaxInt = Linien[j1].RelInt; }; };
          for (j1=StartAnzahlLinien; j1<AnzLinien; j1++)
            {
              Linien[j1].RelInt = Linien[j1].RelInt/MaxInt;
              Linien[j1].Hoehe = Linien[j1].Hoehe*Linien[j1].RelInt;
            };

	return AnzLinien;
    };


void CMinuit::CalcAndShowFitFunc(void)
{
	CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp(); 
	int i;
	

	pDataView->FIT = TRUE;
	double max  = pDataView->DataPoint[0][end_index];
	double min  = pDataView->DataPoint[0][start_index];
	double step = (max-min)/pApp->FitFunRes;
	double pos  = min;
	for (i=0;i<pApp->FitFunRes;i++)
	{
		FitFunc[0][i]=(float)pos;
		FitFunc[1][i]=(float)fitfunction(pos,u);
		pos=pos+step;
	}
	pDataView->Invalidate(TRUE);
}


void CMinuit::ResizeWin(ULONG x,ULONG y)
{
	if (IsActivated)MoveWindow(0,0,x,y,TRUE);
}

void CMinuit::stand(void)
{
	log_printf("\r\nUser functions are directly available through\r\n minuit interface, help lists all commands\r\n");
}


void CMinuit::resi(void)
{
	USHORT i;
	CString Chisqr;
	double chi2;
	if(pResView==NULL)
	{
		pResView = new DataView(this);
		pResView->SetWindowTitle("Residuum of "+pDataView->WindowTitle);
		pResView->SetXTitle(pDataView->XAxisTitle,pDataView->XAxisUnit);
		pResView->SetYTitle("Residuum of "+pDataView->YAxisTitle,pDataView->YAxisUnit);	
	}
	else pResView->Reset();
	fcn(gin, &chi2, u, 4);
	Chisqr.Format("Chi^2 = %g    Chi^2 red. = %g",chi2,(chi2)/((end_index-start_index)-nu));
	pResView->SetTitle(Chisqr);
	for(i=start_index;i<=end_index;i++)
	{
		pResView->AddDataPoint((float)pDataView->DataPoint[0][i],
								(float)(pDataView->DataPoint[1][i]-fitfunction(pDataView->DataPoint[0][i],u)),
								(float)pDataView->DataPoint[2][i]);
	}
	pResView->RedrawNow();
}


 double CMinuit::gauss(double x, double FWHMG)
{
  return exp(-x*x/( 0.6005612*FWHMG*FWHMG));
}

 double CMinuit::lorentz(double x, double FWHML)
{
  return (FWHML*FWHML/4.)/(FWHML*FWHML/4.+x*x);
}

 double CMinuit::efunc(double x, double TAU)
{
  return exp(-x/fabs(TAU));
}

 double CMinuit::falt_voigt(double x, double pos)
{  
  return gauss(x-pos,FWHMG_set)*lorentz(x,FWHML_set);
}

 double CMinuit::falt_gauss_e(double x, double pos)
{  
  return efunc(x-pos,TAU_set)*gauss(x,FWHMG_set);
}

 double CMinuit::falt_lorenz_e(double x, double pos)
{  
  return efunc(x-pos,TAU_set)*lorentz(x,FWHML_set);
}

double CMinuit::falt_voigt_e(double x, double pos)
{  
  return efunc(x-pos,TAU_set)*voigt(x,FWHMG_set,FWHML_set);
}

double CMinuit::trapez_integral(double a, double b, double x, int n)
  {
    int i;
    static int glit;
    static double s;
    double ix, sum, del;

    if (n==1)
	{
		glit=1;
		switch(faltselect) 
		{
		case 0:
			{
				return (s=0.5*(b-a)*(falt_voigt(a, x)+falt_voigt(b, x)));
			}break;
		case 1:
			{
				return (s=0.5*(b-a)*(falt_gauss_e(a, x)+falt_gauss_e(b, x)));
			}break;
		case 2:
			{
				return (s=0.5*(b-a)*(falt_lorenz_e(a, x)+falt_lorenz_e(b, x)));
			}break;
		case 3:
			{
				return (s=0.5*(b-a)*(falt_voigt_e(a, x)+falt_voigt_e(b, x)));
			}break;
		default:
			{
				return 0;
			}
			break;
		}
		
    }
    else
	{
		del=(b-a)/ (double) glit;
		ix=a+del/2.;
		sum=0;
		for (i=1; i<=glit; i++)
		{
			switch(faltselect) 
			{
			case 0:
				{
					sum+=falt_voigt(ix, x);	
				}break;
			case 1:
				{
					sum+=falt_gauss_e(ix, x);
				}break;
			case 2:
				{
					sum+=falt_lorenz_e(ix, x);
				}break;
			case 3:
				{
					sum+=falt_voigt_e(ix, x);
				}break;
			default:
				break;
			}
			ix+=del;
		}
		glit*=2;
		return s=0.5*(s+(b-a)*sum/(double) (glit/2));
    }
  }

double CMinuit::trapez_integral_voigt(double a, double b, double x, int n)
  {
    int i;
    static int glit;
    static double s;
    double ix, sum, del;

    if (n==1)
	{
		glit=1;
	
		return (s=0.5*(b-a)*(falt_voigt(a, x)+falt_voigt(b, x)));	
    }
    else
	{
		del=(b-a)/ (double) glit;
		ix=a+del/2.;
		sum=0;
		for (i=1; i<=glit; i++)
		{
			sum+=falt_voigt(ix, x);	
			ix+=del;
		}
		glit*=2;
		return s=0.5*(s+(b-a)*sum/(double) (glit/2));
    }
  }

 double CMinuit::simpson_integral(double a, double b, double x)
  {
    int i;
    double temp_double, st, ost, os;

    ost=os=-1e30;
    for (i=1; i<=MAX_STEPS; i++)
	{
      st=trapez_integral(a, b, x, i);
      temp_double=(4.*st-ost)/3.;
      if (fabs(temp_double-os)<=SIMPSON_EPS*fabs(os))
        return temp_double;
      os=temp_double;
      ost=st;
    }
	/* ("WARNING IN SIMPSON: Too many steps.");*/
    return temp_double;
  }

 double CMinuit::simpson_integral_voigt(double a, double b, double x)
  {
    int i;
    double temp_double, st, ost, os;

    ost=os=-1e30;
    for (i=1; i<=MAX_STEPS; i++)
	{
      st=trapez_integral_voigt(a, b, x, i);
      temp_double=(4.*st-ost)/3.;
      if (fabs(temp_double-os)<=SIMPSON_EPS*fabs(os))
        return temp_double;
      os=temp_double;
      ost=st;
    }
	/* ("WARNING IN SIMPSON: Too many steps.");*/
    return temp_double;
  }
/*************************** Die gefalteten Funktionen *********************************/


 double CMinuit::voigt(double x, double FWHMG, double FWHML)
{
	FWHMG_set	= FWHMG;
	FWHML_set	= FWHML;
	if (FASTCALC)
	{
		USHORT i;
		double x_i;
		double x_step;
		
		if((FWHMG != FWHMG_last)||(FWHML != FWHML_last))
		{
			scalingvoigt = 1e-99;
			def_reg_voigt = max(FWHMG*NSIGMARES,FWHML*NSIGMARES);
			x_step = 2*def_reg_voigt/FOLDRES; 
			for(i=0;i<FOLDRES;i++)
			{
				x_i=-(def_reg_voigt)+i*x_step;
				foldedfunctionvoigt[i] = simpson_integral_voigt(x_i-NSIGMA*FWHML_set, x_i+NSIGMA*FWHML_set, x_i);
				if(foldedfunctionvoigt[i]>scalingvoigt) scalingvoigt = foldedfunctionvoigt[i];
			}
		}
		FWHMG_last = FWHMG;
		FWHML_last = FWHML;

		double pos = (FOLDRES/2)+(x/(2*def_reg_voigt/FOLDRES));
		double a = fabs((pos-(long)pos));
		double b = (1-a);
		if ((pos < OFFSET)||(pos>=FOLDRES-OFFSET)) return (double)0;
		else return ((a*foldedfunctionvoigt[(USHORT)(pos+1)])+(b*foldedfunctionvoigt[(USHORT)(pos)]))/scalingvoigt;
	}
	else 
	{
		if((FWHMG != FWHMG_last)||(FWHML != FWHML_last))
		{
			scalingvoigt = simpson_integral_voigt(-NSIGMA*FWHML_set, NSIGMA*FWHML_set, 0);
		}
		FWHMG_last = FWHMG;
		FWHML_last = FWHML;
		return simpson_integral_voigt(x-NSIGMA*FWHML_set, x+NSIGMA*FWHML_set, x)/scalingvoigt;
	}
}

 double CMinuit::gaussexp(double x, double FWHMG, double TAU)
{
	faltselect			= 1;
	FWHMG_set			= FWHMG;

	TAU_set		= fabs(TAU);
	if (FASTCALC)
	{
		USHORT i;
		double x_i;
		double x_step;

		if((FWHMG != FWHMG_last)||(TAU != TAU_last))
		{
			def_reg = max(FWHMG*NSIGMARES,TAU_set*NTAURES);
			x_step = 2*def_reg/FOLDRES; 
			scalinggaussexp = 1e-99;
			for(i=0;i<FOLDRES;i++)
			{
				if(TAU>=0)x_i=-(def_reg)+i*x_step;
				else x_i=(def_reg)-i*x_step;
				foldedfunction[i] = simpson_integral(x_i, x_i+NTAU*TAU_set, x_i);
				if(foldedfunction[i]>scalinggaussexp) scalinggaussexp = foldedfunction[i];
			}
		}
		FWHMG_last	= FWHMG;
		TAU_last	= TAU;
			
		double pos = (FOLDRES/2)+(x/(2*def_reg/FOLDRES));
		double a = fabs((pos-(long)pos));
		double b = (1-a);
		if ((pos < OFFSET)||(pos>=FOLDRES-OFFSET))return (double)0;
		else return ((a*foldedfunction[(USHORT)(pos+1)])+(b*foldedfunction[(USHORT)(pos)]))/scalinggaussexp;
	}
	else 
	{
		if((FWHMG != FWHMG_last)||(TAU != TAU_last))
		{
			scalinggaussexp = simpson_integral(0, NTAU*TAU_set, 0);
		}
		FWHMG_last	= FWHMG;
		TAU_last	= TAU;
		if(TAU<0) x = -x;
		return simpson_integral(x, x+NTAU*TAU_set, x)/scalinggaussexp;
	}
}

 
 double CMinuit::lorentzexp(double x, double FWHML, double TAU)
{
	faltselect	= 2;
	FWHML_set	= FWHML;
	TAU_set		= fabs(TAU);
	if (FASTCALC)
	{
		USHORT i;
		double x_i;
		double x_step;

		if((FWHML_set != FWHML_last)||(TAU != TAU_last))
		{
			def_reg = max(FWHML_set*NSIGMARES,TAU_set*NTAURES);
			x_step = 2*def_reg/FOLDRES; 
			scalinglorentzexp = 1e-99;
			for(i=0;i<FOLDRES;i++)
			{
				if(TAU>=0)x_i=-(def_reg)+i*x_step;
				else x_i=(def_reg)-i*x_step;
				foldedfunction[i] = simpson_integral(x_i, x_i+NTAU*TAU_set, x_i);
				if(foldedfunction[i]>scalinglorentzexp) scalinglorentzexp = foldedfunction[i];
			}
		}
		FWHML_last	= FWHML_set;
		TAU_last	= TAU;

		double pos = (FOLDRES/2)+(x/(2*def_reg/FOLDRES));
		double a = fabs((pos-(long)pos));
		double b = (1-a);
		if ((pos < OFFSET)||(pos>=FOLDRES-OFFSET))return (double)0;
		else return ((a*foldedfunction[(USHORT)(pos+1)])+(b*foldedfunction[(USHORT)(pos)]))/scalinglorentzexp;
	}
	else 
	{
		if((FWHML_set != FWHML_last)||(TAU != TAU_last))
		{
			scalinglorentzexp = simpson_integral(0,NTAU*TAU_set, 0);
		}
		FWHML_last	= FWHML_set;
		TAU_last	= TAU;
		if(TAU<0) x = -x;
		return simpson_integral(x, x+NTAU*TAU_set, x)/scalinglorentzexp;
	}
}

 double CMinuit::voigtexp(double x, double FWHMG, double FWHML, double TAU)
{
	faltselect	= 3;
	FWHMG_set	= FWHMG;
	FWHML_set	= FWHML;
	TAU_set		= fabs(TAU);

	if (FASTCALC)
	{
		USHORT i;
		double x_i;
		double x_step;

		if((FWHMG != FWHMG_last)||(FWHML != FWHML_last)||(TAU != TAU_last))
		{
			def_reg = max(FWHMG*NSIGMARES,TAU_set*NTAURES);
			def_reg = max(FWHML*NSIGMARES,def_reg);
			x_step = 2*def_reg/FOLDRES; 
			scalingvoigtexp = 1e-99;
			for(i=0;i<FOLDRES;i++)
			{
				if(TAU>=0)x_i=-(def_reg)+i*x_step;
				else x_i=(def_reg)-i*x_step;
				foldedfunction[i] = simpson_integral(x_i, x_i+NTAU*TAU_set, x_i);
				if(foldedfunction[i]>scalingvoigtexp) scalingvoigtexp = foldedfunction[i];
			}
		}
		FWHMG_last	= FWHMG;
		FWHML_last	= FWHML;
		TAU_last	= TAU;

		double pos = (FOLDRES/2)+(x/(2*def_reg/FOLDRES));
		double a = fabs((pos-(long)pos));
		double b = (1-a);
		if ((pos < OFFSET)||(pos>=FOLDRES-OFFSET))return (double)0;
		else return ((a*foldedfunction[(USHORT)(pos+1)])+(b*foldedfunction[(USHORT)(pos)]))/scalingvoigtexp;
	}
	else 
	{
		if((FWHMG != FWHMG_last)||(FWHML != FWHML_last)||(TAU != TAU_last))
		{
			scalingvoigtexp = simpson_integral(0, NTAU*TAU_set, 0);
		}
		FWHMG_last	= FWHMG;
		FWHML_last	= FWHML;
		TAU_last	= TAU;
		if(TAU<0) x = -x;
		return simpson_integral(x, x+NTAU*TAU_set, x)/scalingvoigtexp;
	}
}

/*************************** Ende gefaltete Funktionen *********************************/


double CMinuit::fitfunction(double x, double par[])
{
  int i;
  double y=0;
  
  if (fitfunc==11) /* UserFCN */
  {
		return Userfitfunction(x, &par[0]);
  }
  
  if (fitfunc==20) /* Gauss */
  {
		if(!equalspaced)
		{
			for (i=0;i < numofpeaks;i++)
				{
					y = y + par[i*2+4]*sign*gauss(x-par[i*2+3],par[2]);
				}
				y = y + par[1]*x+par[0];
			return y;
		}
		else
		{
			for (i=0;i < numofpeaks;i++)
				{
					y = y + par[i+5]*sign*gauss(x-(par[3]+i*par[4]),par[2]);
				}
				y = y + par[1]*x+par[0];
			return y;
		}
  }

  if (fitfunc==21) /* Lorentz */
  {
		if(!equalspaced)
		{
			for (i=0;i < numofpeaks;i++)
				{
					y = y + par[i*2+4]*sign*lorentz(x-par[i*2+3],par[2]);
				}
				y = y + par[1]*x+par[0];
			return y;
		}
		else
		{
			for (i=0;i < numofpeaks;i++)
				{
					y = y + par[i+5]*sign*lorentz(x-(par[3]+i*par[4]),par[2]);
				}
				y = y + par[1]*x+par[0];
			return y;
		}
  }

  if (fitfunc==22) /* Voigt */
  {	
    if(!equalspaced)
	{

		for (i=0;i < numofpeaks;i++)
		{
			y = y + par[2*i+5]*sign*voigt(x-par[2*i+4], par[2],par[3]);
		}
		y = y + par[1]*x+par[0];
		return y;
	}
	else
	{
		for (i=0;i < numofpeaks;i++)
		{
			y = y + par[i+6]*sign*voigt(x-(par[4]+i*par[5]),par[2],par[3]);
		}
		y = y + par[1]*x+par[0];
		return y;
	}
  }

    
  if (fitfunc==32) /* Gauss(x)exp */
  {	
    if(!equalspaced)
	{

		for (i=0;i < numofpeaks;i++)
		{
			y = y + par[i*2+5]*sign*gaussexp(x-par[i*2+4], par[2],par[3]);
		}
		y = y + par[1]*x+par[0];
		return y;
	}
	else
	{
		for (i=0;i < numofpeaks;i++)
		{
			y = y + par[6+i]*sign*gaussexp(x-(par[4]+i*par[5]),par[2],par[3]);
		}
		y = y + par[1]*x+par[0];
		return y;
	}
  }
	  
  if (fitfunc==33) /* Lorentz(x)exp */
  {	
    if(!equalspaced)
	{

		for (i=0;i < numofpeaks;i++)
		{
			y = y + par[i*2+5]*sign*lorentzexp(x-par[i*2+4], par[2],par[3]);
		}
		y = y + par[1]*x+par[0];
		return y;
	}
	else
	{
		for (i=0;i < numofpeaks;i++)
		{
			y = y + par[6+i]*sign*lorentzexp(x-(par[4]+i*par[5]),par[2],par[3]);
		}
		y = y + par[1]*x+par[0];
		return y;
	}
  }


  if (fitfunc==34) /* Voigt(x)exp */
  {	
    if(!equalspaced)
	{

		for (i=0;i < numofpeaks;i++)
		{
			y = y + par[i*2+6]*sign*voigtexp(x-par[i*2+5], par[2],par[3],par[4]);
		}
		y = y + par[1]*x+par[0];
		return y;
	}
	else
	{
		for (i=0;i < numofpeaks;i++)
		{
			y = y + par[i+7]*sign*voigtexp(x-(par[5]+i*par[6]),par[2],par[3],par[4]);
		}
		y = y + par[1]*x+par[0];
		return y;
	}
  }


  if (fitfunc==0) /* Gauss */
  {
		if(!equalspaced)
		{
			for (i=0;i < numofpeaks;i++)
				{
					y = y + par[i*3+3]*sign*gauss(x-par[i*3+2],par[i*3+4]);
				}
				y = y + par[1]*x+par[0];
			return y;
		}
		else
		{
			for (i=0;i < numofpeaks;i++)
				{
					y = y + par[4+2*i]*sign*gauss(x-(par[2]+i*par[3]),par[5+2*i]);
				}
				y = y + par[1]*x+par[0];
			return y;
		}
  }

  if (fitfunc==1) /* Lorentz */
  {
		if(!equalspaced)
		{
			for (i=0;i < numofpeaks;i++)
				{
					y = y + par[i*3+3]*sign*lorentz(x-par[i*3+2], par[i*3+4]);
				}
				y = y + par[1]*x+par[0];
			return y;
		}
		else
		{
			for (i=0;i < numofpeaks;i++)
				{
					y = y + par[4+2*i]*sign*lorentz(x-(par[2]+i*par[3]),par[5+2*i]);
				}
				y = y + par[1]*x+par[0];
			return y;
		}
  }

  if (fitfunc==2) /* Voigt */
  {	
    if(!equalspaced)
	{

		for (i=0;i < numofpeaks;i++)
		{
			y = y + par[i*4+3]*sign*voigt(x-par[i*4+2], par[i*4+4],par[i*4+5]);
		}
		y = y + par[1]*x+par[0];
		return y;
	}
	else
	{
		for (i=0;i < numofpeaks;i++)
		{
			y = y + par[4+3*i]*sign*voigt(x-(par[2]+i*par[3]),par[5+3*i],par[6+3*i]);
		}
		y = y + par[1]*x+par[0];
		return y;
	}
  }

    
  if (fitfunc==12) /* Gauss(x)exp */
  {	
    if(!equalspaced)
	{

		for (i=0;i < numofpeaks;i++)
		{
			y = y + par[i*4+3]*sign*gaussexp(x-par[i*4+2], par[i*4+4],par[i*4+5]);
		}
		y = y + par[1]*x+par[0];
		return y;
	}
	else
	{
		for (i=0;i < numofpeaks;i++)
		{
			y = y + par[4+3*i]*sign*gaussexp(x-(par[2]+i*par[3]),par[5+3*i],par[6+3*i]);
		}
		y = y + par[1]*x+par[0];
		return y;
	}
  }
	  
  if (fitfunc==13) /* Lorentz(x)exp */
  {	
    if(!equalspaced)
	{

		for (i=0;i < numofpeaks;i++)
		{
			y = y + par[i*4+3]*sign*lorentzexp(x-par[i*4+2], par[i*4+4],par[i*4+5]);
		}
		y = y + par[1]*x+par[0];
		return y;
	}
	else
	{
		for (i=0;i < numofpeaks;i++)
		{
			y = y + par[4+3*i]*sign*lorentzexp(x-(par[2]+i*par[3]),par[5+3*i],par[6+3*i]);
		}
		y = y + par[1]*x+par[0];
		return y;
	}
  }


  if (fitfunc==14) /* Voigt(x)exp */
  {	
    if(!equalspaced)
	{

		for (i=0;i < numofpeaks;i++)
		{
			y = y + par[i*4+3]*sign*voigtexp(x-par[i*4+2], par[i*4+4],par[i*4+5],par[i*4+6]);
		}
		y = y + par[1]*x+par[0];
		return y;
	}
	else
	{
		for (i=0;i < numofpeaks;i++)
		{
			y = y + par[4+3*i]*sign*voigtexp(x-(par[2]+i*par[3]),par[5+3*i],par[6+3*i],par[7+3*i]);
		}
		y = y + par[1]*x+par[0];
		return y;
	}
  }

  if (fitfunc==3) /* Decay */
  {	
		double y=0;
		for (i=0;i < numofpeaks;i++)
		{
			y = y+sign*par[1+i*2]*exp(-(x-pDataView->DataPoint[0][start_index])/par[2+i*2]);
		}
		return par[0]+y;
  }

  if (fitfunc==4) /* Linear */
  {
	  return par[1]*x+par[0];
  }

  if (fitfunc==5) /* HFS Gauss */
  {
	  double y=0;
	  int i;
	  CString Data;
	  AnzahlLinien = BerechneLinien(
						Wellenzahl,
						I,
						Am,
						par[3]*1e6,par[4]*1e6,
						par[5]*1e6,par[6]*1e6,
						J_grund,
						J_an,
						ISV,
						par[7],
						ReferenzMasse,
						Haeufigkeit);
	  for (i=0;i<AnzahlLinien;i++)
	  {
			y = y + sign*par[8+i]*gauss(x+Linien[i].dU,par[2]);
	  }
	  return y+par[1]*x+par[0];
  }

  if (fitfunc==6) /* HFS Lorentz */
  {
	  double y=0;
	  int i;
	  CString Data;
	  AnzahlLinien = BerechneLinien(
						Wellenzahl,
						I,
						Am,
						par[3]*1e6,par[4]*1e6,
						par[5]*1e6,par[6]*1e6,
						J_grund,
						J_an,
						ISV,
						par[7],
						ReferenzMasse,
						Haeufigkeit);
	  for (i=0;i<AnzahlLinien;i++)
	  {
			y = y + sign*par[8+i]*lorentz(x+Linien[i].dU,par[2]);
	  }
	  return y+par[1]*x+par[0];
  }

  if (fitfunc==7) /* HFS Voigt */
  {
	  double y=0;
	  int i;
	  CString Data;
	  AnzahlLinien = BerechneLinien(
						Wellenzahl,
						I,
						Am,
						par[4]*1e6,par[5]*1e6,
						par[6]*1e6,par[7]*1e6,
						J_grund,
						J_an,
						ISV,
						par[8],
						ReferenzMasse,
						Haeufigkeit);
	  for (i=0;i<AnzahlLinien;i++)
	  {
			y = y + sign*par[9+i]*voigt(x+Linien[i].dU,par[2],par[3]);
	  }
	  return y+par[1]*x+par[0];
  }

  if (fitfunc==8) /* HFS Gauss (*) exp */
  {
	  double y=0;
	  int i;
	  CString Data;
	  AnzahlLinien = BerechneLinien(
						Wellenzahl,
						I,
						Am,
						par[3]*1e6,par[4]*1e6,
						par[5]*1e6,par[6]*1e6,
						J_grund,
						J_an,
						ISV,
						par[7],
						ReferenzMasse,
						Haeufigkeit);
	  for (i=0;i<AnzahlLinien;i++)
	  {
			y = y + sign*par[8+i]*gaussexp(-Linien[i].dU-x,par[2],par[8+AnzahlLinien]);
	  }
	  return y+par[1]*x+par[0];
  }

  if (fitfunc==9) /* HFS Lorentz (*) exp */
  {
	  double y=0;
	  int i;
	  CString Data;
	  AnzahlLinien = BerechneLinien(
						Wellenzahl,
						I,
						Am,
						par[3]*1e6,par[4]*1e6,
						par[5]*1e6,par[6]*1e6,
						J_grund,
						J_an,
						ISV,
						par[7],
						ReferenzMasse,
						Haeufigkeit);
	  for (i=0;i<AnzahlLinien;i++)
	  {
			y = y + sign*par[8+i]*lorentzexp(-Linien[i].dU-x,par[2],par[8+AnzahlLinien]);
	  }
	  return y+par[1]*x+par[0];
  }



  if (fitfunc==10) /* HFS Voigt (x) exp */
  {
	  double y=0;
	  int i;
	  CString Data;
	  AnzahlLinien = BerechneLinien(
						Wellenzahl,
						I,
						Am,
						par[4]*1e6,par[5]*1e6,
						par[6]*1e6,par[7]*1e6,
						J_grund,
						J_an,
						ISV,
						par[8],
						ReferenzMasse,
						Haeufigkeit);
	  for (i=0;i<AnzahlLinien;i++)
	  {
			y = y + sign*par[9+i]*voigtexp(x+Linien[i].dU,par[2],par[3],par[9+AnzahlLinien]);
	  }
	  return y+par[1]*x+par[0];
  }

  if (fitfunc==100) /* cos * exp */
  {
	return par[0]*cos(par[2]*x-par[4])*exp(-x/par[3])+par[1];
  }

  if (fitfunc==101) /* Mx Bloch */
  {
	double delta		= x-omega;
	double sqrdelta		= sqr(delta);
	double sqrtau2		= sqr(tau2);
	double sqromega1	= sqr(omega1);
	double sqrD			= 1 + sqromega1*tau1*tau2;
	double y =  M_unendlich*(omega1*sqrtau2)*delta/(sqrD+sqrtau2*sqrdelta)+par[0];
	return y;
  }

  if (fitfunc==102) /* My Bloch */
  {
	double delta		= x-omega;
	double sqrdelta		= sqr(delta);
	double sqrtau2		= sqr(tau2);
	double sqromega1	= sqr(omega1);
	double sqrD			= 1 + sqromega1*tau1*tau2;
	double y =  M_unendlich*omega1*tau2/(sqrD+sqrtau2*sqrdelta)+par[0];
	return y;
  }

  if (fitfunc==103) /* Mz Bloch */
  {
	double delta		= x-omega;
	double sqrdelta		= sqr(delta);
	double sqrtau2		= sqr(tau2);
	double sqromega1	= sqr(omega1);
	double sqrD			= 1 + sqromega1*tau1*tau2;
	double y =  M_unendlich*(1+((1-sqrD)/(sqrD+sqrtau2*sqrdelta)))+par[0]-M_unendlich;
	return y;
  }

  if (fitfunc==104) /* Mxyz Bloch */
  {
	#define OFFS_Y 3000 
	#define OFFS_Z 5000

	#define DELTA_Y 2000 
	#define DELTA_Z 4000

	double y;
	double xyz_delta;
	double xyz_sqrdelta;
	double xyz_sqrtau2		= sqr(xyz_tau2);
	double xyz_sqromega1	= sqr(xyz_omega1);
	double xyz_sqrD			= 1 + xyz_sqromega1*xyz_tau1*xyz_tau2;
	if(x<OFFS_Y)				
	{
		xyz_delta		= (x-0)-xyz_omega_x;
		xyz_sqrdelta	= sqr(xyz_delta);
		y =  xyz_sign_x*xyz_M_unendlich*(xyz_omega1*xyz_sqrtau2)*xyz_delta/(1+xyz_sqromega1+xyz_tau1+xyz_sqrtau2+xyz_sqrtau2*xyz_sqrdelta)+xyz_offset1;
	}
	if(x>=OFFS_Y&&x<OFFS_Z)	
	{
		xyz_delta		= (x-DELTA_Y)-xyz_omega_y;
		xyz_sqrdelta	= sqr(xyz_delta);
		y =  xyz_M_unendlich*xyz_omega1*xyz_tau2/(1+xyz_sqromega1+xyz_tau1+xyz_sqrtau2+xyz_sqrtau2*xyz_sqrdelta)+xyz_offset2;
	}
	if(x>=OFFS_Z)			
	{
		xyz_delta		= (x-DELTA_Z)-xyz_omega_z;
		xyz_sqrdelta	= sqr(xyz_delta);
		y =  xyz_amp_z*(1+((1-xyz_sqrD)/(xyz_sqrD+xyz_sqrtau2*xyz_sqrdelta)))+xyz_offset3-xyz_amp_z;
	}
	return y;
  }

  return 0;
}

CString CMinuit::getline(void)
{
	int len;
	char string[81];
	CString InString;
	len=GetEditCtrl().GetLine(GetEditCtrl().LineFromChar(-1),&string[0],80);
	string[len]='\0';
	InString = string;
	return InString;
}


void __stdcall CMinuit::fcn(double grad[], double *fval, double par[],int iflag)
{
  USHORT i;
  double temp, func;
  *fval=0;
  switch (chiq_function){
    case 0: for (i=start_index; i<=end_index; i++){
              temp=(pDataView->DataPoint[1][i]-fitfunction(pDataView->DataPoint[0][i], par))/pDataView->DataPoint[2][i];
              *fval+=temp*temp;
             }
             break;
    case 1: for (i=start_index; i<=end_index; i++){
              func=fitfunction(pDataView->DataPoint[0][i], par);
              temp=pDataView->DataPoint[1][i]-func;
              *fval+=temp*temp/(func>0. ? func : 1.);
             }
             break;
    case 2: for (i=start_index; i<=end_index; i++){
              func=fitfunction(pDataView->DataPoint[0][i], par);
              *fval+=
		pDataView->DataPoint[1][i]>0. ? (func-pDataView->DataPoint[1][i]+pDataView->DataPoint[1][i]*log(pDataView->DataPoint[1][i]/func)) : func;
             }
             *fval*=2.;
             break;
  }
}


BOOL CMinuit::look_for_break(void)
{
  MSG* pMsg = new(MSG);
  if(PeekMessage(pMsg,GetSafeHwnd(), WM_KEYDOWN, WM_KEYDOWN, PM_REMOVE))
  {
	if (pMsg->message == WM_KEYDOWN)
	{	
		if(pMsg->wParam == VK_ESCAPE)
		{
			delete pMsg;
			return TRUE;
		}
	}
  }
  delete pMsg;
  return FALSE;
}


void CMinuit::log_printf(const char *format, ...)
{
  va_list arg_ptr;
  char OutString[5000];
  va_start (arg_ptr, format);
  vsprintf(OutString,format, arg_ptr);
  PrintString=PrintString+OutString;	  
  va_end(arg_ptr);
}

void CMinuit::printf(const char *format, ...)
{
  va_list arg_ptr;
  char OutString[2000];
  va_start (arg_ptr, format);
  vsprintf(OutString,format, arg_ptr);
  int Start=GetEditCtrl().LineIndex(GetEditCtrl().LineFromChar(-1));
  int Len=GetEditCtrl().LineLength(GetEditCtrl().LineFromChar(-1));
  GetEditCtrl().SetSel(Start,Start+Len,FALSE);
  GetEditCtrl().ReplaceSel((CString)OutString);  
  va_end(arg_ptr);
  GetEditCtrl().SetSel(-1,0,FALSE); 
  GetEditCtrl().ReplaceSel(PrintString);
  PrintString="";
}

void CMinuit::log_flush(void)
{
  GetEditCtrl().SetSel(-1,0,FALSE); 
  GetEditCtrl().ReplaceSel(PrintString);
  PrintString="";
}


void CMinuit::log_printfmov(const char *format, ...)
{
  va_list arg_ptr;
  char OutString[2000];
  CString PrintString;
  va_start (arg_ptr, format);
  vsprintf(OutString,format, arg_ptr);
  GetEditCtrl().SetSel(-1,0,FALSE); 
  GetEditCtrl().ReplaceSel((CString)OutString);  
  va_end(arg_ptr);
}


void CMinuit::log_over(const char *format, ...)
{
  va_list arg_ptr;
  char OutString[2000];
  va_start (arg_ptr, format);
  vsprintf(OutString,format, arg_ptr);
  int Start=GetEditCtrl().LineIndex(GetEditCtrl().LineFromChar(-1));
  int Len=GetEditCtrl().LineLength(GetEditCtrl().LineFromChar(-1));
  GetEditCtrl().SetSel(Start,Start+Len,FALSE);
  GetEditCtrl().ReplaceSel((CString)OutString);  
  va_end(arg_ptr);
}


 void CMinuit::read_startwerte(char* filename){
  FILE *f;
  CString name;
  char pname[256];
  double parameter, error;

  
  f=fopen(filename, "r");
  if (f==NULL) log_printf("error reading %s\r\n", filename);
  else
  {
	  mninit();
	  log_printf("\r\nreading parameter definitions...\r\n");
	  while (fscanf(f, "%10s%lg%lg", pname, &parameter, &error)!=EOF){
		mnparm(pname, parameter, error,0,0);
	  }
	  fclose(f);
  }
}

 void CMinuit::save_parameter(char *filename){
  FILE *outfile;
  int i;
  char buffer2[15], buffer3[15];
  CString cx2,cx3;
  double x2, x3;
  int l;

  if(strcmp(filename,"")!=0)
  {
	  if ((outfile=fopen(filename, "w"))==NULL)
		log_printf("error creating %s\r\n", filename);
	  else{
			  for (i=0; i<nu; ++i){         /* loop over parameters */
				if (nvarl[i]<0) continue;
				l=niofex[i]-1;
				if (l>=0){    /* variable parameter. */
				  cx2=cx3="";

				  if (nvarl[i]<=1){
							{
							  log_printf("%3d %-10.10s  %+E  %+E\r\n", i+1, paraname[i], u[i], werr[l]);
							  fprintf(outfile,"%-10.10s  %+E  %+E\r\n", paraname[i], u[i], werr[l]);
							}
							  continue;
							 }
							 else
							   x2=alim[i], x3=blim[i];
				  if (cx2==""){sprintf(buffer2, "%+E", x2); cx2=buffer2;}
				  if (cx3==""){sprintf(buffer3, "%+E", x3); cx3=buffer3;}
				  {
				  log_printf("%3d %-10.10s  %+E  %+E  %s  %s\r\n", i+1, paraname[i], u[i], werr[l], cx2, cx3);
				  fprintf(outfile,"%-10.10s  %+E  %+E  %s  %s\r\n", paraname[i], u[i], werr[l], cx2, cx3);
				  }
   
				  if (nvarl[i]<=1) /* check if parameter is at limit */
					continue;
				   if (fabs(cos(x[l]))<0.001)
					log_printf("WARNING: ABOVE PARAMETER IS AT LIMIT.\r\n");
				}
				else{    /* print constant or fixed parameter. */
					{
					log_printf("%3d %-10.10s  %+E    ", i+1, paraname[i], u[i]);
					fprintf(outfile,"%-10.10s  %+E    ", paraname[i], u[i]);
					}
				  if (nvarl[i]>0){
					  {
						log_printf("fixed");
						fprintf(outfile,"1");
					  }
					if (nvarl[i]==4)
					{
					  log_printf("     %+E  %+E", alim[i], blim[i]);
					  fprintf(outfile,"     %+E  %+E", alim[i], blim[i]);
					}
					{
					log_printf("\r\n");
					fprintf(outfile,"\r\n");
					}
				  }
				  else
				  {
					log_printf("constant\r\n");
					fprintf(outfile,"0\r\n");
				  }
				}
			  }
			}
	  	fclose(outfile);
	  }
  }

 int CMinuit::save_function(char *filename){
  FILE *outfile;
  USHORT i;
  CMCPforNTApp* pApp = (CMCPforNTApp*)AfxGetApp();
  if ((outfile=fopen(filename, "w"))==NULL)
  {
	log_printf("error creating %s\n", filename);
    return -1;
  }
  double max  = pDataView->DataPoint[0][end_index];
  double min  = pDataView->DataPoint[0][start_index];
  double step = (max-min)/pApp->FitFunRes;
  double pos  = min;
  double FitX,FitY;
  for (i=0;i<pApp->FitFunRes;i++)
  {
		FitX=pos;
		FitY=(double)fitfunction(pos,u);
		pos=pos+step;
		fprintf(outfile, "%+.8E %+.8E\n", FitX, FitY);
  }
  fclose(outfile);
  return 0;
}



void CMinuit::mnpint(double *pexti, const int i, double *pinti)
{
/* Calculates the internal parameter value PINTI corresponding
   to the external value PEXTI for parameter I.
*/
  CString temp_str;
  double yy;

  *pinti=*pexti;
  if (nvarl[i]==4){
    yy=(*pexti-alim[i])*2/(blim[i]-alim[i])-1;
    if (yy*yy>=1-epsma2){
      if (yy<0){
        *pinti=-M_PI_2+sqrt(epsma2)*8;
        temp_str="IS AT ITS LOWER ALLOWED LIMIT.";
      }
      else{
        *pinti=M_PI_2-sqrt(epsma2)*8;
        temp_str="IS AT ITS UPPER ALLOWED LIMIT.";
      }
      *pexti=alim[i]+(blim[i]-alim[i])*0.5*(sin(*pinti)+1);
      limset=TRUE;
      if (yy*yy>1)
        temp_str="BROUGHT BACK INSIDE LIMITS.";
      if (lwarn)
		  log_printf("WARNING IN %s: VARIABLE %d %s\r\n", cfrom, i+1, temp_str);
    }
    else
      *pinti=asin(yy);
  }
}


__inline void CMinuit::mnexin(double pint[]){
/* Transforms the external parameter values U to internal values
    in the dense array PINT.
*/
  int i;
  int iext;

  limset=FALSE;
  for (i=0; i<npar; ++i){
    iext=nexofi[i]-1;
    mnpint(&u[iext], iext, &pint[i]);
  }
}


void CMinuit::check_amin(void){
  if (mnamin==UNDEFI){
    if (print_level>=0)
		log_printf("FIRST CALL TO USER FUNCTION AT NEW START POINT, WITH IFLAG=4.\r\n");
    mnexin(x); 
	fcn(gin, &mnamin, u, 4); nfcn++;
    edm=BIGEDM;
  }
}


#ifdef LEVENBERG
int CMinuit::gaussj(double a[MNMAXINT][MNMAXINT], double b[]){
#define SWAP(a, b) {temp=(a); (a)=(b); (b)=temp;}
  int indxc[MNMAXINT], indxr[MNMAXINT], ipiv[MNMAXINT];
  int i, icol, irow, j, k, l;
  double big, dum, pivinv, temp;

  memset(ipiv, 0, sizeof(ipiv));
  for (i=0; i<npar; i++){
    big=0.;
    for (j=0; j<npar; j++)
      if (ipiv[j]!=1)
        for (k=0; k<npar; k++){
          if (ipiv[k]==0){
            if (fabs(a[j][k])>=big){
              big=fabs(a[j][k]);
              irow=j; icol=k;
            }
          }
          else
            if (ipiv[k]>1){
              err_matrix=0;
              return 1;
            }
        }
        ipiv[icol]++;
        if (irow!=icol){
          for (k=0; k<npar; k++)
            SWAP(a[irow][k], a[icol][k])
          SWAP(b[irow], b[icol])
        }
        indxr[i]=irow; indxc[i]=icol;
        if (a[icol][icol]==0.){
          err_matrix=0;
          return 1;
        }
        pivinv=1./a[icol][icol];
        a[icol][icol]=1;
        for (k=0; k<npar; k++)
          a[icol][k]*=pivinv;
        b[icol]*=pivinv;
        for (k=0; k<npar; k++)
          if (k!=icol){
            dum=a[k][icol];
            a[k][icol]=0.0;
            for (l=0; l<npar; l++)
              a[k][l]-=a[icol][l]*dum;
            b[k]-=b[icol]*dum;
    }
  }
  for (l=npar-1; l>=0; l--){
    if (indxr[l]!=indxc[l])
      for (k=0; k<npar; k++)
        SWAP(a[k][indxr[l]], a[k][indxc[l]]);
  }
  err_matrix=3;
  return 0;
#undef SWAP
}


void CMinuit::derivative(double par[], double x_wert, double dyda[]){
  int i;
  double t0, t1, save, delta0, delta1;
 
  for (i=0; i<npar; i++){
    if (fabs(par[i])>1e-8) delta0=par[i]/1000.;
    else delta0=1e-11;
    delta1=delta0/10.;
    save=par[i];
    par[i]=save+delta0; mninex(par); t0=fitfunction(x_wert, u);
    par[i]=save-delta0; mninex(par); t0=(t0-fitfunction(x_wert, u))/(2*delta0);
    par[i]=save+delta1; mninex(par); t1=fitfunction(x_wert, u);
    par[i]=save-delta1; mninex(par); t1=(t1-fitfunction(x_wert, u))/(2*delta1);
    par[i]=save;
    dyda[i]=(t0*delta1*delta1-t1*delta0*delta0)/(delta1*delta1-delta0*delta0);
  }
}


void CMinuit::mrqcof(double par[], double alpha[MNMAXINT][MNMAXINT], double beta[], double *chiq){
  int i, j, k;
  double wt, sig2i, dy, dyda[MNMAXINT], func;

  memset(alpha, 0, sizeof(double)*MNMAXINT*MNMAXINT);
  memset(beta, 0, sizeof(double)*MNMAXINT);
  *chiq=0;
  for (i=start_index; i<=end_index; i++){
    derivative(par, pDataView->DataPoint[0][i], dyda);
    mninex(par); func=fitfunction(pDataView->DataPoint[0][i], u);
    dy=pDataView->DataPoint[1][i]-func;
    switch (chiq_function){
      default: sig2i=1./(pDataView->DataPoint[2][i]*pDataView->DataPoint[2][i]); break;
      case 1: sig2i= func>0 ? 1./func : 1.; break;
      case 2: log_printf("Likelihood-fits not supported for MARQUARD\r\n"); exit(1);
    }
    for (j=0; j<npar; j++){
      wt=dyda[j]*sig2i;
      for (k=0; k<=j; k++)
        alpha[j][k]+=wt*dyda[k];
      beta[j]+=dy*wt;
    }
    *chiq+=dy*dy*sig2i;
  }
  nfcn+=4*npar+1;
  for (i=1; i<npar; i++)
    for (j=0; j<i; j++)
      alpha[j][i]=alpha[i][j];
}


void CMinuit::mrqmin(double beta[MNMAXINT]){
  int i;
  double chiq, atry[MNMAXINT], da[MNMAXINT];

  memcpy(vhmat, vthmat, sizeof(vhmat));
  memcpy(da, beta, sizeof(da));
  for (i=0; i<npar; i++)
    vhmat[i][i]*=(1.+mar_lambda);
  if (gaussj(vhmat, da) && lwarn)
    log_printf("WARNING IN MRQMIN: singular matrix\r\n");
  for (i=0; i<npar; i++)
    atry[i]=x[i]+da[i];
  mrqcof(atry, vhmat, da, &chiq);
  if (chiq<mnamin){
    mar_lambda*=0.1;
    mnamin=chiq;
    memcpy(vthmat, vhmat, sizeof(vhmat));
    memcpy(beta, da, sizeof(da));
    memcpy(x, atry, sizeof(atry));
  }
  else
    mar_lambda*=10;
}


void CMinuit::marquardt_fit(void){
#define MAX_FAIL 6
  double rhotol, chiold;
  int i, j, npfn, fail;
  if (npar<=0) return;
  npfn=nfcnfr=nfcn;
  check_amin();
  cstatu="INITIATE  ";
  mar_lambda=lambda_start;
  rhotol=up*1e-4;
  if (print_level>0)
    log_printf("START MARQUARDT MIZATION. LAMBDA=%.3E. CONVERGENCE WHEN EDM<%.3E\r\n", lambda_start, rhotol);
  mrqcof(x, vthmat, grd, &mnamin);
  if (print_level>0)
    mnprint(-1);
  cstatu="IMPROVEMNT";
  chiold=mnamin;
  fail=0;
  do{
    mrqmin(grd); mninex(x); mnvert(vhmat, npar); mnwerr();
    if (mnamin<chiold){
      fail=(int)0;
	  edm=(int)0;
      for (i=0; i<npar; ++i){
        for (j=0; j<i; ++j)
          edm+=grd[i]*vhmat[i][j]*grd[j];
        edm+=grd[i]*grd[i]*vhmat[i][i];
      }
      edm*=2;
      if (print_level>1)
        mnprint(-1);
      chiold=mnamin;
    }
    else{
      fail++;
      if (print_level>2)
        mnprint(-1);
    }
  } while (edm>rhotol && nfcn-npfn<=nfcnmx && fail<MAX_FAIL);
  converged=-1;
  if (fail==MAX_FAIL){
    cstatu="FAILED    ";
    if (print_level>=0)
      log_printf("MARQUARDT FAILS TO FIND IMPROVEMENT.\r\n");
  }
  if (nfcn-npfn>nfcnmx){
    cstatu="CALL LIMIT";
    call_limit_flag=1;
    if (print_level>=0)
      log_printf("CALL LIMIT EXCEEDED IN MARQUARDT.\r\n");
  }
  if (edm<=rhotol){
    if (print_level>=0)
      log_printf("MARQUARDT MINIMIZATION HAS CONVERGED.\r\n");
    converged=1;
    cstatu="CONVERGED ";
  }
  else if (print_level>=0)
    log_printf("MARQUARDT TERMINATED WITHOUT CONVERGENCE.\r\n");
  if (print_level>0)
    mnprint(-1);
#undef MAX_FAIL
}
#endif


double CMinuit::mncalf(double emat[MNMAXINT][MNMAXINT], double pvec[]){
/* Transforms the function FCN by dividing out the quadratic part
    in order to find further minima. Calculates
    ycalf=(f-fmin)/(x-xmin)*v*(x-xmin)
*/
  int i, j;
  double f, denom=0;

  mninex(pvec); fcn(gin, &f, u, 4); nfcn++;
  memset(grd, 0, sizeof(grd));
  for (i=0; i<npar; i++)
    for (j=0; j<npar; j++)
      grd[i]+=emat[i][j]*(xt[j]-pvec[j]);
  for (i=0; i<npar; i++)
    denom+=grd[i]*(xt[i]-pvec[i]);
  if (denom<=0){
    dcovar=denom=1;
    err_matrix=0;
  }
  return (f-apsi)/denom;
}


int CMinuit::mncros(double *aopt){
/* Find point where MNEVAL=AMIN+UP, along the line through XMID
   with direction XDIR, where X is parameter KE1.
   Crossing point is at U(KE1)=XMID+AOPT*XDIR
*/
#define tla 0.001
  int i, inew, i1, i2, noless, ipt;
  double aim, aminsv, aulim, dfda, zlim, fnext, determ;
  double anear, afar, bmin, bmax, ecarmn, ecarmx, coeff[3], alsb[3], flsb[3];

  aminsv=mnamin;
  aim=mnamin+up;
  ipt=2;
  zlim=alim[ke1cr];  /* find the largest allowed A */
  if (xdircr>0)
    zlim=blim[ke1cr];
  aulim=MIN(100, (zlim-xmidcr)/xdircr);
  *aopt=limset=0;  /* LSB = Line Search Buffer, first point */
  if (aulim<tla) limset=TRUE;
  if (mneval(0, &fnext)) goto L900;
  if (limset && fnext<=aim) goto L930;
  ipt++;
  alsb[0]=0;
  flsb[0]=fnext;
  fnext=MAX(fnext, aminsv+up*0.1);
  *aopt=sqrt(up/(fnext-aminsv))-1;
  if (fabs(fnext-aim)<up*0.01)
    goto L800;
  if (*aopt<-0.5) *aopt=-0.5;
  limset=FALSE;
  if (*aopt>aulim){
    *aopt=aulim;
    limset=TRUE;
  }
  if (mneval(*aopt, &fnext)) goto L900;
  if (limset && fnext<=aim) goto L930;
  alsb[1]=*aopt;
  flsb[1]=fnext;
  ipt++;
  dfda=(flsb[1]-flsb[0])/(alsb[1]-alsb[0]);
  if (dfda>0)  /* DFDA must be positive on the contour */
    goto L460;
L300:
  for (i=1; i<=15-ipt; i++){
    alsb[0]=alsb[1];
    flsb[0]=flsb[1];
    *aopt=alsb[0]+(double)i*0.2;
    limset=FALSE;
    if (*aopt>aulim){
      *aopt=aulim;
      limset=TRUE;
    }
    if (mneval(*aopt, &fnext)) goto L900;
    if (limset && fnext<=aim)
      goto L930;
    alsb[1]=*aopt;
    flsb[1]=fnext;
    ipt++;
    dfda=(flsb[1]-flsb[0])/(alsb[1]-alsb[0]);
    if (dfda>0)
      goto L460;
  }
  if (lwarn)
    log_printf("WARNING IN MNCROS: Cannot find slope of the right sign.\r\n");
  goto L950;
L460:  /* we have two points with the right slope */
  *aopt=alsb[1]+(aim-flsb[1])/dfda;
  if (MIN(fabs(*aopt-alsb[0]), fabs(*aopt-alsb[1]))<tla)
    goto L800;
  if (ipt>=15)
    goto L950;
  bmin=MIN(alsb[0], alsb[1])-1;
  if (*aopt<bmin)
    *aopt=bmin;
  bmax=MAX(alsb[0], alsb[1])+1;
  if (*aopt>bmax)
    *aopt=bmax;
  limset=FALSE;  /* Try a third point */
  if (*aopt>aulim){
    *aopt=aulim;
    limset=TRUE;
  }
  if (mneval(*aopt, &fnext)) goto L900;
  if (limset && fnext<=aim)
    goto L930;
  alsb[2]=*aopt;
  flsb[2]=fnext;
  ipt++;
  inew=2;
  i1=i2=noless=0;
  if (flsb[0]<aim) noless++;
  if (flsb[1]<aim) noless++;
  if (flsb[2]<aim) noless++;
  ecarmx=ecarmn=fabs(flsb[0]-aim);
  if (fabs(flsb[1]-aim)>ecarmx){ecarmx=fabs(flsb[1]-aim); i1=1;}
  if (fabs(flsb[1]-aim)<ecarmn){ecarmn=fabs(flsb[1]-aim); i2=1;}
  if (fabs(flsb[2]-aim)>ecarmx){ecarmx=fabs(flsb[2]-aim); i1=2;}
  if (fabs(flsb[2]-aim)<ecarmn){ecarmn=fabs(flsb[2]-aim); i2=2;}
  if (noless==1 || noless==2)  /* if at least one on each side of AIM, fit a parabola */
    goto L500;
/* if all three are above AIM, third must be closest to AIM */
  if (noless==0 && i2!=2)
    goto L950;
/* if all three below, and third is not best, then slope */
/* has again gone negative, look for positive slope. */
  if (noless==3 && i2!=2){
    alsb[1]=alsb[2];
    flsb[1]=flsb[2];
    goto L300;
  }
/* in other cases, new straight line thru last two points */
  alsb[i1]=alsb[2];
  flsb[i1]=flsb[2];
  dfda=(flsb[1]-flsb[0])/(alsb[1]-alsb[0]);
  goto L460;
L500:  /* parabola fit */
  mnpfit(alsb, flsb, coeff);
  determ=coeff[1]*coeff[1]-coeff[2]*4*(coeff[0]-aim);
  if (determ<=0)
    goto L950;
  *aopt=(-coeff[1]+sqrt(determ))/(coeff[2]*2);
  if (fabs(*aopt-alsb[inew])<tla)
    goto L800;
  if (ipt>=15)  /* Evaluate function at parabolic optimum */
    goto L950;
  limset=FALSE;
  if (*aopt>aulim){
    *aopt=aulim;
    limset=TRUE;
  }
  if (mneval(*aopt, &fnext)) goto L900;
  if (limset && fnext<=aim)
    goto L930;
  ipt++;
/* Replace unneeded point by new one */
/* find nearest, farthest, (and hence middle) points, */
  i1=i2=0;
  anear=afar=alsb[0];
  if (alsb[1]<anear){anear=alsb[1]; i2=1;}
  if (alsb[1]>afar){afar=alsb[1]; i1=1;}
  if (alsb[2]<anear){anear=alsb[2]; i2=2;}
  if (alsb[2]>afar){afar=alsb[2]; i1=2;}
  if ((flsb[3-i2-i1]-aim)*(flsb[i2]-aim)>0)
    inew=i2;
  else
    inew=i1;
  alsb[inew]=*aopt;
  flsb[inew]=fnext;
  goto L500;
L800: return 0;   /* Contour has been located, return point to MINOS */
L900: if (call_limit_flag) return 2;      /* error in the minimization */
L950: return 3;                              /* cannot find next point */
L930: return 1;                         /* parameter up against limit */
}


void CMinuit::mncuve(void){
/* Makes sure that the current point is a local minimum and that the error
    matrix exists, or at least something good enough for MINOS and MNCONT
*/
  int i;
  double dxdi, wint;

  if (converged<1){
    log_printf("FUNCTION MUST BE MINIMIZED BEFORE CALLING %s\r\n", cfrom);
    apsi=epsi;
    mnmigr();
  }
  if (err_matrix<3){
    mnhess();
    if (err_matrix==0){
      if (lwarn)
        log_printf("WARNING IN %s: NO ERROR MATRIX.  WILL IMPROVISE.\r\n", cfrom);
      memset(vhmat, 0, sizeof(vhmat));
      for (i=0; i<npar; ++i){
        if (g2[i]<=0){
          wint=werr[i];
          if (nvarl[nexofi[i]-1]>1){
            dxdi=mndxdi(i);
            if (fabs(dxdi)<0.001) wint=0.01;
            else wint/=fabs(dxdi);
          }
          g2[i]=up/(wint*wint);
        }
        vhmat[i][i]=2/g2[i];
      }
     err_matrix=(int)1;
	 dcovar=(int)1;
    }
    else
      mnwerr();
  }
}


void CMinuit::mnderi(void){
/* Calculates the first derivatives of FCN (GRD), either by finite
   differences or by transforming the user-supplied derivatives to
   internal coordinates, according to whether ISW(3) is zero or one.
*/
  int i, j, ncyc;
  double tlrstp, tlrgrd, step, stepb4, stpmax, stpmin, xtf, fs1, fs2, grbfor;
  double dfmin, epspri, optstp;

  check_amin();
  if (!usergrad){
    dfmin=epsma2*8*(fabs(mnamin)+up);
    switch (istrat){
      case 0: ncyc=2; tlrstp=0.5; tlrgrd=0.1; break;
      case 1: ncyc=3; tlrstp=0.3; tlrgrd=0.05; break;
      default: ncyc=5; tlrstp=0.1; tlrgrd=0.02;
    }
    for (i=0; i<npar; i++){   /* loop over variable parameters */
      epspri=epsma2+fabs(grd[i]*epsma2);
      xtf=x[i];
      stepb4=0;
      for (j=1; j<=ncyc; j++){ /* loop as little as possible here! */
        optstp=sqrt(dfmin/(fabs(g2[i])+epspri));  /* theoretically best step */
        step=MAX(optstp, fabs(gstep[i]*0.1));     /* step cannot decrease by more than a factor of ten */
        if (gstep[i]<0 && step>0.5)  /* but if parameter has limits, max step size = 0.5 */
          step=0.5;
        stpmax=fabs(gstep[i])*10;  /* and not more than ten times the previous step */
        if (step>stpmax) step=stpmax;
        stpmin=fabs(epsma2*x[i])*8;  /* minimum step size allowed by machine precision */
        if (step<stpmin) step=stpmin;
        if (fabs((step-stepb4)/step)<tlrstp)   /* end of iterations if step change less than factor 2 */
          break;
        gstep[i]=d_sign(step, gstep[i]);  /* take step positive */
        stepb4=step;
        x[i]=xtf+step;
        mninex(x); fcn(gin, &fs1, u, 4); nfcn++;
        x[i]=xtf-step;  /* take step negative */
        mninex(x); fcn(gin, &fs2, u, 4); nfcn++;
        grbfor=grd[i];
        grd[i]=(fs1-fs2)/(step*2);
        g2[i]=(fs1+fs2-mnamin*2)/(step*step);
        x[i]=xtf;
        if (fabs(grbfor-grd[i])/(fabs(grd[i])+dfmin/step)<tlrgrd)
          break;   /* see if another iteration is necessary */
      }
    }
    mninex(x);
  }
  else { /* derivatives calc by fcn */
    for (i=0; i<npar; i++){
      j=nexofi[i]-1;
      if (nvarl[j]>1)
        grd[i]=gin[j]*(blim[j]-alim[j])*0.5*cos(x[i]);
      else
        grd[i]=gin[j];
    }
  }
}


__inline double CMinuit::mndxdi(const int ipar){
/* calculates the transformation factor between external and internal
   parameter values.
*/
  int i;

  i=nexofi[ipar]-1;
  if (nvarl[i]>1)
    return fabs((blim[i]-alim[i])*cos(x[ipar]))*0.5;
  else
    return 1;
}


void CMinuit::mneig(double a[MNMAXINT][MNMAXINT], const int n, double work[], const double precis){
/* PRECIS is the machine precision EPSMAC 
   ACHTUNG: work2 eingeführt. work wurde mit max 2*MNMAXINT indiziert !
               iter statt j eingeführt, wurde doppelt benutzt !
*/
  int i, j, k, l, m, i1, iter;
  double b, c, f, h, r, s, hh, gl, pr, pt, work2[MNMAXINT];

  for (i=n-1; i>0; i--){
    f=a[i][i-1];
    gl=0;
    if (i>1)
     for (k=0; k<=i-2; k++)
       gl+=a[i][k]*a[i][k];
    h=gl+f*f;
    if (gl<=1e-35){ work[i]=0; work2[i]=f; }
    else{
      gl=sqrt(h);
      if (f>=0) gl=-gl;
      work2[i]=gl;
      h-=f*gl;
      a[i][i-1]=f-gl;
      f=0;
      for (j=0; j<i; j++){
        a[j][i]=a[i][j]/h;
        gl=0;
        for (k=0; k<=j; k++)
          gl+=a[j][k]*a[i][k];
        if (j<i-1)
          for (k=j+1; k<i; k++)
            gl+=a[k][j]*a[i][k];
        work2[j]=gl/h;
        f+=gl*a[j][i];
      }
      hh=f/(h+h);
      for (j=0; j<i; j++){
        f=a[i][j];
        work2[j]=gl=work2[j]-hh*f;
        for (k=0; k<=j; k++)
          a[j][k]-=f*work2[k]+gl*a[i][k];
      }
      work[i]=h;
    }
  }
  work[0]=work2[0]=0;
  for (i=0; i<n; i++){
    if (work[i]!=0 && i!=0)
      for (j=0; j<i; j++){
        gl=0;
        for (k=0; k<i; k++)
          gl+=a[i][k]*a[k][j];
        for (k=0; k<i; k++)
          a[k][j]-=gl*a[k][i];
      }
    work[i]=a[i][i];
    a[i][i]=1;
    if (i!=0)
      for (j=0; j<i; j++)
        a[i][j]=a[j][i]=0;
  }
  for (i=1; i<n; ++i)
    work2[i-1]=work2[i];
  work2[n-1]=b=f=0;
  for (l=0; l<n; l++){
    iter=0;
    h=precis*(fabs(work[l])+fabs(work2[l]));
    if (b<h) b=h;
    for (m=l; m<n; m++)
      if (fabs(work2[m])<=b)
        break;
    if (m!=l)
      do {
        if (iter++ ==50){
          if (lwarn)
            log_printf("WARNING IN MNEIGEN: Too many iterations.\r\n");
          return;
        }
        pt=(work[l+1]-work[l])/(work2[l]*2);
        r=sqrt(pt*pt+1);
        pr=pt+r;
        if (pt<0)
          pr=pt-r;
        h=work[l]-work2[l]/pr;
        for (i=l; i<n; i++)
          work[i]-=h;
        f+=h;
        pt=work[m];
        c=1, s=0;
        i=m;
        for (i1=l; i1<m; i1++){
          j=i, i--;
          gl=c*work2[i];
          h=c*pt;
          if (fabs(pt)>=fabs(work2[i])){
            c=work2[i]/pt, r=sqrt(c*c+1);
            work2[j]=s*pt*r, s=c/r, c=1./r;
          }
          else{
            c=pt/work2[i], r=sqrt(c*c+1);
            work2[j]=s*work2[i]*r, s=1./r, c/=r;
          }
          pt=c*work[i]-s*gl;
          work[j]=h+s*(c*gl+s*work[i]);
          for (k=0; k<n; k++){
            h=a[k][j];
            a[k][j]=s*a[k][i]+c*h;
            a[k][i]=c*a[k][i]-s*h;
          }
        }
        work2[l]=s*pt;
        work[l]=c*pt;
      } while (fabs(work2[l])>b);
    work[l]+=f;
  }

  for (i=0; i<n-1; ++i){
    k=i;
    pt=work[i];
    for (j=i+1; j<n; ++j){
      if (work[j]<pt){
        k=j;
        pt=work[j];
      }
    }
    if (k!=i){
      work[k]=work[i];
      work[i]=pt;
      for (j=0; j<n; ++j){
        pt=a[j][i];
        a[j][i]=a[j][k];
        a[j][k]=pt;
      }
    }
  }
}


void CMinuit::mnemat(){
/* Calculates the external error matrix from the internal */
  unsigned int i, j, k, kk, iz, nperln;

  if (err_matrix){
    log_printf("\r\nEXTERNAL ERROR MATRIX. NDIM=%d NPAR=%d ERR DEF=%f\r\n", MNMAXINT, npar, up);
    nperln=MIN(PAGEWIDTH/11, 7);
    if (npar>nperln)
      log_printf("ELEMENTS ABOVE DIAGONAL ARE NOT PRINTED.\r\n");
    for (i=0; i<npar; ++i)
      for (j=0; j<=i; ++j)
        pmat[j][i]=pmat[i][j]=mndxdi(i)*vhmat[i][j]*mndxdi(j)*up;
    for (i=1; i<=npar; ++i){
      iz=npar;    /* IZ is number of columns to be printed in row I */
      if (npar>=nperln)
        iz=i;
      for (k=1; nperln<0 ? k>=iz : k<=iz; k+=nperln){
        j=MIN(k+nperln-1, iz);
        for (kk=k; kk<=j; ++kk)
          log_printf("%+.3E ", pmat[i-1][kk-1]);
        log_printf("\r\n");
      }
    }
  }
}


int CMinuit::mneval(const double anext, double *fnext){
/* Evaluates the function being analyzed by MNCROS, which is generally
    the minimum of FCN with respect to all remaining variable parameters.
*/
  u[ke1cr]=xmidcr+anext*xdircr;
  mninex(x); fcn(gin, fnext, u, 4); nfcn++;
  itaur=1;
  mnamin=*fnext;
  call_limit_flag=0;
  mnmigr();
  itaur=0;
  *fnext=mnamin;
  if (call_limit_flag || converged<1)
    return 1;
  else
    return 0;
}


int CMinuit::mnfixp(const int iint){
/* removes parameter IINT from the internal (variable) parameter
   list, and arranges the rest of the list to fill the hole.
*/
  int nold, iext, i, j, lc, inew, jnew;
  double yy[MNMAXINT];

  iext=nexofi[iint];
  if (npfix>=MNMAXINT){
    log_printf("MINUIT CANNOT FIX PARAMETER %d\r\n"
           "MAXIMUM NUMBER THAT CAN BE FIXED IS %d\r\n", iext, MNMAXINT);
    return 1;
  }
  niofex[iext-1]=0; /* reduce number of variable parameters by one */
  nold=npar;
  npar--;
  ++npfix;      /* save values in case parameter is later restored */
  ipfix[npfix-1]=iext;
  lc=iint;
  xs[npfix-1]=x[lc];
  xts[npfix-1]=xt[lc];
  dirins[npfix-1]=werr[lc];
  grds[npfix-1]=grd[lc];
  g2s[npfix-1]=g2[lc];
  gsteps[npfix-1]=gstep[lc];
  for (i=iext+1; i<=nu; i++) /* shift values for other parameters to fill hole */
    if (niofex[i-1]>0){
      lc=niofex[i-1]-1;
      niofex[i-1]=lc;
      nexofi[lc-1]=i;
      x[lc-1]=x[lc];
      xt[lc-1]=xt[lc];
      dirin[lc-1]=dirin[lc];
      werr[lc-1]=werr[lc];
      grd[lc-1]=grd[lc];
      g2[lc-1]=g2[lc];
      gstep[lc-1]=gstep[lc];
    }
  if (err_matrix && npar>0){
    for (i=0; i<nold; ++i)
      yy[i]=vhmat[i][iint];
    inew=0;
    for (i=0; i<nold; ++i){
      if (i!=iint){
        jnew=0;
        for (j=0; j<=i; ++j)
          if (j!=iint){
            vhmat[inew][jnew]=vhmat[i][j]-yy[j]*yy[i]/yy[iint];
            jnew++;
          }
        inew++;
      }
    }
  }
  return 0;
}


void CMinuit::mnfree(const int k){
/* Restores one or more fixed parameter(s) to variable status by inserting it
   into the internal parameter list at the appropriate place.
   K=0 means restore all parameters
   K=1 means restore the last parameter fixed
   K=-I means restore external parameter I (if possible)
   IR=external number of parameter being restored
   IS=internal number of parameter being restored
*/
  int i, ka, ik, ir, is, lc;
  double xv, xtv, dirinv, grdv, g2v, gstepv;

  if (npfix<1)
    log_printf("CALL TO MNFREE IGNORED.  THERE ARE NO FIXED PARAMETERS\r\n");
  if (k==1 || k==0)
    goto L40;
  ka=abs(k);  /* release parameter with specified external number */
  if (niofex[ka-1]!=0){
    log_printf("CALL TO MNFREE IGNORED.  PARAMETER SPECIFIED IS ALREADY VARIABLE.\r\n");
    return;
  }
  if (npfix>=1)
    for (ik=1; ik<=npfix; ++ik){
      if (ipfix[ik-1]==ka)
        goto L24;
    }
  log_printf("PARAMETER %d NOT FIXED.  CANNOT BE RELEASED.\r\n", ka);
  return;
L24:
  if (ik==npfix)
    goto L40;
  xv=xs[ik-1];    /* move specified parameter to end of list */
  xtv=xts[ik-1];
  dirinv=dirins[ik-1];
  grdv=grds[ik-1];
  g2v=g2s[ik-1];
  gstepv=gsteps[ik-1];
  for (i=ik; i<npfix; ++i){
    ipfix[i-1]=ipfix[i];
    xs[i-1]=xs[i];
    xts[i-1]=xts[i];
    dirins[i-1]=dirins[i];
    grds[i-1]=grds[i];
    g2s[i-1]=g2s[i];
    gsteps[i-1]=gsteps[i];
  }
  ipfix[npfix-1]=ka;
  xs[npfix-1]=xv;
  xts[npfix-1]=xtv;
  dirins[npfix-1]=dirinv;
  grds[npfix-1]=grdv;
  g2s[npfix-1]=g2v;
  gsteps[npfix-1]=gstepv; /* restore last parameter in fixed list: IPFIX(NPFIX) */
L40:
  if (npfix>=1){
    ir=ipfix[npfix-1];
    is=0;
    for (ik=nu; ik>=ir; ik--){
      if (niofex[ik-1]>0){
        lc=niofex[ik-1]+1;
        is=lc-1;
        niofex[ik-1]=lc;
        nexofi[lc-1]=ik;
        x[lc-1]=x[lc-2];
        xt[lc-1]=xt[lc-2];
        dirin[lc-1]=dirin[lc-2];
        werr[lc-1]=werr[lc-2];
        grd[lc-1]=grd[lc-2];
        g2[lc-1]=g2[lc-2];
        gstep[lc-1]=gstep[lc-2];
      }
    }
    npar++;
    if (is==0)
      is=npar;
    niofex[ir-1]=is;
    nexofi[is-1]=ir;
    npfix--;
    x[is-1]=xs[npfix];
    xt[is-1]=xts[npfix];
    dirin[is-1]=werr[is-1]=dirins[npfix];
    grd[is-1]=grds[npfix];
    g2[is-1]=g2s[npfix];
    gstep[is-1]=gsteps[npfix];
    err_matrix=0;
    dcovar=1;
    if (itaur<1)
      log_printf("PARAMETER %d, %s RESTORED TO VARIABLE.\r\n", ir, paraname[ir-1]);
    if (k==0)
      goto L40;
  }
  mnexin(x); /* if different from internal, external values are taken */
}


void CMinuit::mngrad(void){
/* Interprets the SET GRAD command, which informs MINUIT whether the
    first derivatives of FCN will be calculated by the user inside FCN.
    It can check the user's derivative calculation by comparing it with a
    finite difference approximation.
*/
  int i, lc, lnone;
  double fzero, gf[MNMAXINT];
  CString cwd;

  if (argum[0]>0)
    usergrad=1;
  else{
    for (i=0; i<nu; ++i)   /* get user-calculated first derivatives from FCN */
      gin[i]=UNDEFI;
    mninex(x); fcn(gin, &fzero, u, 2); nfcn++;
    mnderi();
    memcpy(gf, grd, sizeof(grd));
    usergrad=0; i=istrat; istrat=2; mnhes1(); istrat=i;
    log_printf("CHECK OF GRADIENT CALCULATION IN FCN:\r\n\r\n"
               "  PARAMETER    G(IN FCN)     G(MINUIT)     DG(MINUIT)  AGREEMENT\r\n");
    usergrad=1;
    lnone=FALSE;
    for (lc=0; lc<npar; ++lc){
      i=nexofi[lc]-1;
      cwd="GOOD";
      if (fabs(gf[lc]-grd[lc])>dgrd[lc]){
        cwd="BAD";
        usergrad=0;
      }
      if (gin[i]==UNDEFI){
        cwd="NONE";
        lnone=TRUE;
        usergrad=(int)0;
		gf[lc]=(int)0;
      }
      log_printf("%d %-10.10s %+E %+E %+E   %s\r\n", i+1, paraname[i], gf[lc], grd[lc], dgrd[lc], cwd);
    }
    log_printf("\r\n");
    if (lnone)
      log_printf("FCN DID NOT CALCULATE THE DERIVATIVE.\r\n");
    if (! usergrad)
      log_printf("MINUIT DOES NOT ACCEPT DERIVATIVE CALCULATIONS BY FCN.\r\n"
                 "TO FORCE ACCEPTANCE, ENTER \"SET GRAD 1\".\r\n");
  }
}


void CMinuit::mnhelp(char *comd){
  if (*comd=='\0')
    log_printf("List of MINUIT interactive commands:\r\n"
         "EXIt      Exit from Interactive Minuit\r\n"
         "FIX       Cause parameter(s) to remain constant\r\n"
         "HESse     Calculate the Hessian or error matrix\r\n"
         "IMProve   Search for a new minimum around current minimum\r\n"
         "MIGrad    Minimize by the method of Migrad\r\n"
#ifdef LEVENBERG
         "MAR-fit   Minimize by the method of Levenberg-Marquardt\r\n"
#endif
         "MINImize  MIGRAD + SIMPLEX method if Migrad fails\r\n"
         "MINOs     Exact (non-linear) parameter error analysis\r\n"
         "RELease   Make previously FIXed parameters variable again\r\n"
         "REStore   Release all parameters fixed\r\n"
         "SEEk      Minimize by the method of Monte Carlo\r\n"
         "SET       Set various MINUIT constants or conditions\r\n"
         "SHOw      Show values of current constants or conditions\r\n"
         "SIMplex   Minimize by the method of Simplex\r\n"
         "STAnd     Call to USER function\r\n"
		 "PLOres    Plot the residuum\r\n"
		 "LOAdpun   Read a new punchfile\r\n"
		 "SAVepun   Write the punchfile to disk\r\n"
		 "FITsave   Write the fit to disk\r\n");
  if (strncmp(comd, "FIX", 3)==0)
    log_printf("FIX <parno> [parno] ... [parno]\r\n"
         "Causes parameter(s) <parno> to be removed from the list of variable parameters,\r\n"
         "and their value(s) will remain constant during subsequent minimizations, etc.,\r\n"
         "until another command changes their status.\r\n");
  if (strncmp(comd, "HES", 3)==0)
    log_printf("HESse [maxcalls]\r\n"
         "Calculate, by finite differences, the Hessian or error matrix. That is, it\r\n"
         "calculates the fullmatrix of second derivatives of the function with respect\r\n"
         "to the currently variable parameters, and inverts it, printing out the\r\n"
         "resulting error matrix. The optional argument [maxcalls] specifies the\r\n"
         "(approximate) maximum number of function calls after which the calculation\r\n"
         "will be stopped.\r\n");
  if (strncmp(comd, "IMP", 3)==0)
    log_printf("IMProve [maxcalls]\r\n"
         "If a previous minimization has converged, and the current values of the\r\n"
         "parameters therefore correspond to a local minimum of the function, this\r\n"
         "command requests a search for additional distinct local minima. The optional\r\n"
         "argument [maxcalls] specifies the (approximate) maximum number of function\r\n"
         "calls after which the calculation will be stopped.\r\n");
  if (strncmp(comd, "MIG", 3)==0)
    log_printf("MIGrad [maxcalls] [tolerance]\r\n"
         "Causes minimization of the function by the method of Migrad, the most efficient\r\n"
         "and complete single method, recommended for general functions (see also\r\n"
         "MINImize). The minimization produces as a by-product the error matrix\r\n"
         "of the parameters, which is usually reliable unless warning messages are\r\n"
         "produced. The optional argument [maxcalls] specifies the (approximate)\r\n"
         "maximum number of function calls after which the calculation will be stopped\r\n"
         "even if it has not yet converged. The optional argument [tolerance] specifies\r\n"
         "required tolerance on the function value at the minimum. The default tolerance\r\n"
         "is 0.1, and the minimization will stop when the estimated vertical distance to\r\n"
         "the minimum (EDM) is less than 0.001*[tolerance]*UP (see [SET ERRordef]).\r\n");
  if (strncmp(comd, "MINI", 4)==0)
    log_printf("MINImize [maxcalls] [tolerance]\r\n"
         "Causes minimization of the function by the method of Migrad, as does the\r\n"
         "MIGrad command, but switches to the SIMplex method if Migrad fails to\r\n"
         "converge. Arguments are as for MIGrad. Note that command requires four\r\n"
         "characters to be unambiguous.\r\n");
  if (strncmp(comd, "MINO", 4)==0)
    log_printf("MINOs [maxcalls] [parno] [parno] ...\r\n"
         "Causes a Minos error analysis to be performed on the parameters whose numbers\r\n"
         "[parno] are specified. If none are specified, Minos errors are calculated for\r\n"
         "all variable parameters. Minos errors may be expensive to calculate, but are\r\n"
         "very reliable since they take account of non-linearities in the problem as well\r\n"
         "as parameter correlations, and are in general asymmetric. The optional argument\r\n"
         "[maxcalls] specifies the (approximate) maximum number of function calls per\r\n"
         "parameter requested, after which the calculation will stop for that parameter.\r\n");
  if (strncmp(comd, "REL", 3)==0)
    log_printf("RELease <parno> [parno] ... [parno]\r\n"
         "If <parno> is the number of a previously variable parameter which has been\r\n"
         "fixed by a command: FIX <parno>, then that parameter will return to variable\r\n"
         "status. Otherwise a warning message is printed and the command is ignored.\r\n"
         "Note that this command operates only on parameters which were at one time\r\n"
         "variable and have been FIXed. It cannot make constant parameters variable.\r\n");
  if (strncmp(comd, "RES", 3)==0)
    log_printf("REStore\r\n"
         "This command restores all previously FIXed parameters to variable status.\r\n");
  if (strncmp(comd, "SEE", 3)==0)
    log_printf("SEEk [maxcalls] [devs]\r\n"
         "Causes a Monte Carlo minimization of the function, by choosing random values of\r\n"
         "the variable parameters, chosen uniformly over a hypercube centered at the\r\n"
         "current best value. The region size is by default 3 standard deviations on each\r\n"
         "side, but can be changed by specifying the value of [devs].\r\n");
  if (strncmp(comd, "SET", 3)==0)
    log_printf("SET EPSmachine <accuracy>\r\n"
         "Informs Minuit that the relative doubleing point arithmetic precision is\r\n"
         "<accuracy>. The \"SET EPSmachine\" command can be used to override Minuit\r\n"
         "default value, when the user knows that the FCN function value is not\r\n"
         "calculated to the nominal machine accuracy. Typical values of <accuracy> are\r\n"
         "between 1E-5 and 1E-14.\r\n"
         "SET ERRordef <up>\r\n"
         "Sets the value of UP (default value=1), defining parameter errors. Minuit\r\n"
         "defines parameter errors as the change in parameter value required to\r\n"
         "change the function value by UP. Normally, for chisquared fits UP=1, and\r\n"
         "for negative log likelihood,  UP=0.5.\r\n"
         "SET GRAdient [force]\r\n"
         "Informs Minuit that the user function is prepared to calculate its own first\r\n"
         "derivatives and return their values in the array GRAD when IFLAG=2 (see specs\r\n"
         "of FCN). If [force] is not specified, Minuit will calculate the FCN\r\n"
         "derivatives by finite differences at the current point and compare\r\n"
         "with the user calculation at that point, accepting the user values only if\r\n"
         "they agree. If [force]=1, Minuit does not do its own derivative\r\n"
         "calculation, and uses the derivatives calculated in FCN.\r\n"
         "SET LIMits [parno] [lolim] [uplim]\r\n"
         "Allows the user to change the limits on one or all parameters. If no\r\n"
         "arguments are specified, all limits are removed from all parameters.\r\n"
         "If [parno] alone is specified, limits are removed from parameter [parno].\r\n"
         "If all arguments are specified, then parameter [parno] will be bounded\r\n"
         "between [lolim] and [uplim]. Limits can be specified in either order,\r\n"
         "Minuit will take the smaller as [lolim] and the larger as [uplim].\r\n"
         "However, if [lolim] is equal to [uplim], an error condition results.\r\n"
         "SET NOGradient\r\n"
         "The inverse of \"SET GRAdient\", instructs Minuit not to use the first\r\n"
         "derivatives calculated by the user in FCN.\r\n"
         "SET NOWarnings\r\n"
         "Supresses Minuit warning messages.\r\n"
         "SET PARameter <parno> <value>\r\n"
         "Sets the value of parameter <parno> to <value>. The parameter in question may\r\n"
         "be variable, fixed, or constant, but must be defined.\r\n"
         "SET PRIntout <level>\r\n"
         "Sets the print level, determining how much output will be produced. Allowed\r\n"
         "values and their meanings are displayed after a SHOw PRInt command, and\r\n"
         "are currently:\r\n"
         " [-1]  no output except from SHOW commands\r\n"
         "  [0]  minimum output\r\n"
         "  [1]  default value, normal output\r\n"
         "  [2]  additional output giving intermediate results.\r\n"
         "  [3]  maximum output, showing progress of minimizations.\r\n"
         "Note: See also the SET WARnings command.\r\n"
         "SET RANdomgenerator <seed>\r\n"
         "Sets the seed of the random number generator used in SEEk. This can be any\r\n"
         "int.\r\n"
         "SET STRategy <level>\r\n"
         "Sets the strategy to be used in calculating first and second derivatives and\r\n"
         "in certain minimization methods. In general, low values of <level> mean\r\n"
         "fewer function calls and high values mean more reliable minimization.\r\n"
         "Currently allowed values are 0, 1 (default), and 2.\r\n"
         "SET WARnings\r\n"
         "Instructs Minuit to output warning messages when suspicious conditions\r\n"
         "arise which may indicate unreliable results. This is the default.\r\n"
		 "SET STArtpoint\r\n"
         "Instructs Minuit to use range from startpoint to stoppoint\r\n"
		 "SET STOppoint\r\n"
         "Instructs Minuit to use range from startpoint to stoppoint\r\n"
		 "SET FAStcalc\r\n"
         "Instructs Minuit to use lookup tables for folded functions\r\n"
		 "fast but reduced accuracy\r\n"
#ifdef LEVENBERG
         "SET LAMbda <value>\r\n"
         "Sets the value of lambda for Levenberg-Marquardt minimization\r\n"
#endif
         );
  if (strncmp(comd, "SHO", 3)==0)
    log_printf("SHOw <option_name>\r\n"
         "All SET XXXX commands have a corresponding SHOw XXXX command.\r\n"
         "In addition, the SHOw commands listed starting here have no corresponding\r\n"
         "SET command for obvious reasons.\r\n\r\n"
         "SHOw CORrelations: Calculates and prints the parameter correlations from the\r\n"
         "                   error matrix.\r\n"
         "SHOw COVariance: Prints the (external) covariance (error) matrix.\r\n"
         "SHOw EIGenvalues: Calculates and prints the eigenvalues of the covariance\r\n"
         "                  matrix.\r\n"
         "SHOw FCNvalue: Prints the current value of FCN.\r\n"
         "SHOw MINos: Prints the minos errors\r\n");
  if (strncmp(comd, "SIM", 3)==0)
    log_printf("SIMplex [maxcalls] [tolerance]\r\n"
         "Performs a function minimization using the simplex method of Nelder and Mead.\r\n"
         "Minimization terminates either when the function has been called\r\n"
         "(approximately) [maxcalls] times, or when the estimated vertical distance\r\n"
         "to minimum (EDM) is less than [tolerance]. The default value of [tolerance]\r\n"
         "is 0.1*UP(see SET ERRordef).\r\n");
  if (strncmp(comd, "STA", 3)==0)
    log_printf("STAndard\r\n"
         "Causes Minuit to execute a subroutine supplied by the user.");
  if (strncmp(comd, "PLO", 3)==0)
    log_printf("PLOres\r\n"
         "Plots the residuum of the fit in a new window.");
  if (strncmp(comd, "LOA", 3)==0)
    log_printf("LOAdpun\r\n"
         "Reads a new punchfile from disk");
  if (strncmp(comd, "SAV", 3)==0)
    log_printf("SAVepun\r\n"
         "Saves the punchfile to disk");
  if (strncmp(comd, "FIT", 3)==0)
    log_printf("FITsave\r\n"
         "Saves the fit to disk");

}


void CMinuit::mnhes1(void){
/* Called from MNHESS and MNGRAD. Calculate first derivatives (GRD)
   and uncertainties (DGRD) and appropriate step sizes GSTEP
*/
  int i,icyc, ncyc;
  double dmin, d, dfmin, dgmin, change, chgold, grdold;
  double fs1, fs2, optstp, grdnew, xtf;

  switch (istrat){
    case 0: ncyc=1; break;
    case 2: ncyc=6; break;
    default: ncyc=2;
  }
  dfmin=epsma2*4*(fabs(mnamin)+up);
  for (i=0; i<npar; i++){  /* main loop over parameters */
    xtf=x[i];
    dmin=epsma2*4*fabs(xtf);
    optstp=sqrt(dfmin/(fabs(g2[i])+epsma2+(fabs(grd[i]*epsma2))));
    d=fabs(gstep[i])*0.2;
    if (d>optstp)
      d=optstp;
    if (d<dmin)
      d=dmin;
    chgold=1e4;
    icyc=1;
    do {  /* iterate reducing step size */
      x[i]=xtf+d; mninex(x); fcn(gin, &fs1, u, 4); nfcn++;
      x[i]=xtf-d; mninex(x); fcn(gin, &fs2, u, 4); nfcn++;
      x[i]=xtf;
      grdold=grd[i];
      grdnew=(fs1-fs2)/(d*2);
      dgmin=epsmac*(fabs(fs1)+fabs(fs2))/d;
      if (grdnew==0) break;
      change=fabs((grdold-grdnew)/grdnew);
      if (change>chgold && icyc>1) break;
      chgold=change;
      grd[i]=grdnew;
      gstep[i]=d_sign(d, gstep[i]);
      /* decrease step until first derivative changes by <5% */
      if (change<0.05 || fabs(grdold-grdnew)<dgmin || d<dmin)
        break;
      d*=0.2;
      icyc++;
    } while (icyc<=ncyc);
    dgrd[i]=MAX(dgmin, fabs(grdold-grdnew));
  }
  mninex(x);  /* end of first deriv. loop */
}


void CMinuit::mnhess(void){
/* Calculates the full second-derivative matrix of FCN by taking finite
   differences. When calculating diagonal elements, it may iterate so that
   step size is nearly that which gives function change=UP/10. The first
   derivatives of course come as a free side effect, but with a smaller
   step size in order to obtain a known accuracy.
*/
  int i, j, icyc, ncyc, iext, multpy;
  double dmin, dxdi, wint, tlrg2, d, dlast, g2bfor, aimsag;
  double yy[MNMAXINT], fs1, tlrstp, fs2, g2i, sag, xtf, xti, xtj;

  check_amin();
  switch (istrat){
    case 0: ncyc=3; tlrstp=0.5; tlrg2=0.1; break;
    case 1: ncyc=5; tlrstp=0.3; tlrg2=0.05; break;
    default: ncyc=7; tlrstp=0.1; tlrg2=0.02;
  }
  if (print_level>=2)
    log_printf("START COVARIANCE MATRIX CALCULATION.\r\n");
  cfrom="HESSE   ";
  nfcnfr=nfcn;
  cstatu="OK        ";
  mninex(x); fcn(gin, &fs1, u, 4); nfcn++;
  mnamin=fs1;
/* diagonal elements */
/* ISW(2) = 1 if approx, 2 if not posdef, 3 if ok */
/* AIMSAG is the sagitta we are aiming for in second deriv calc. */
  aimsag=sqrt(epsma2)*(fabs(mnamin)+up);
  memset(vhmat, 0, sizeof(vhmat));
/* Loop over variable parameters for second derivatives */
  for (i=0; i<npar; i++){
    iext=nexofi[i];
    if (g2[i]==0){
      if (lwarn)
        log_printf("WARNING IN HESSE: Second derivative enters zero, param %d\r\n", iext);
      wint=werr[i];
      if (nvarl[iext-1]>1){
        dxdi=mndxdi(i);
        if (fabs(dxdi)<0.001)
          wint=0.01;
        else
          wint/=fabs(dxdi);
      }
      g2[i]=up/(wint*wint);
    }
    xtf=x[i];
    dmin=epsma2*8*fabs(xtf);
    d=fabs(gstep[i]);  /* find step which gives sagitta = AIMSAG */
    for (icyc=1; icyc<=ncyc; icyc++){  /* loop here only if SAG=0. */
      for (multpy=1; multpy<=5; multpy++){  /* take two steps */
        x[i]=xtf+d; mninex(x); fcn(gin, &fs1, u, 4); nfcn++;
        x[i]=xtf-d; mninex(x); fcn(gin, &fs2, u, 4); nfcn++;
        x[i]=xtf;
        sag=(fs1+fs2-mnamin*2)*0.5;
        if (sag!=0)
          goto L30;
        if (gstep[i]<0){
          if (d>=0.5)
            goto L26;
          d*=10;
          if (d>0.5) d=0.51;
          continue;
        }
        d*=10;
      }
L26: if (lwarn)
        log_printf("WARNING IN HESSE: Second derivative zero for parameter %d\r\n", iext);
     goto L390;
L30:  /* SAG is not zero */
      g2bfor=g2[i];
      g2[i]=sag*2/(d*d);
      grd[i]=(fs1-fs2)/(d*2);
      gstep[i]=d_sign(d, gstep[i]);
      dirin[i]=d;
      yy[i]=fs1;
      dlast=d;
      d=sqrt(aimsag*2/fabs(g2[i]));
      /* if parameter has limits, max int step size = 0.5 */
      if (gstep[i]<0)
        d=MIN(d, 0.5);
      if (d<dmin)
        d=dmin;
      if (fabs((d-dlast)/d)<tlrstp || fabs((g2[i]-g2bfor)/g2[i])<tlrg2)
        goto L50;
      d=MAX(MIN(d, dlast*10), dlast*0.1);
    }
L50: vhmat[i][i]=g2[i];
  }
  mninex(x);       /* end of diagonal second derivative loop */
  if (istrat>0)     /* refine the first derivatives */
    mnhes1();
  err_matrix=3;
  dcovar=0;
  if (npar!=1)  /* off-diagonal elements */
    for (i=0; i<npar; ++i)
      for (j=0; j<i; ++j){
        xti=x[i]; xtj=x[j];
        x[i]=xti+dirin[i];
        x[j]=xtj+dirin[j];
        mninex(x); fcn(gin, &fs1, u, 4); nfcn++;
        x[i]=xti;  x[j]=xtj;
        vhmat[j][i]=vhmat[i][j]=(fs1+mnamin-yy[i]-yy[j])/(dirin[i]*dirin[j]);
      }
  mninex(x);
  mnpsdf();  /* verify matrix positive-definite */
  memcpy(pmat, vhmat, sizeof(vhmat));
  if (mnvert(pmat, npar)){
    if (lwarn)
      log_printf("WARNING IN HESSE: Matrix inversion fails.\r\n");
    goto L390;
  }
  edm=0;  /* calculate edm */
  for (i=0; i<npar; ++i){  /* off-diagonal elements */
    for (j=0; j<i; ++j){
      edm+=grd[i]*pmat[i][j]*2*grd[j];
      vhmat[j][i]=vhmat[i][j]=pmat[i][j]*2;
    }
    vhmat[i][i]=pmat[i][i]*2;  /* diagonal elements */
    edm+=pmat[i][i]*(grd[i]*grd[i]);
  }
  if (print_level>=1 && err_matrix==3 && itaur==0)
    log_printf("COVARIANCE MATRIX CALCULATED SUCCESSFULLY\r\n");
  return;
L390:  /* failure to invert 2nd deriv matrix */
  err_matrix=1;
  dcovar=1;
  cstatu="FAILED    ";
  if (print_level>=0)
    log_printf("MNHESS FAILS AND WILL RETURN DIAGONAL MATRIX.\r\n");
  memset(vhmat, 0, sizeof(vhmat));
  for (i=0; i<npar; ++i){
    g2i=g2[i];
    if (g2i<=0)
      g2i=1;
    vhmat[i][i]=2/g2i;
  }
}


void CMinuit::mnimpr(void){
/* Attempts to improve on a good local minimum by finding a better one.
   The quadratic part of FCN is removed by MNCALF and this
   transformed function is minimized using the simplex method from
   several random starting points.
   ref. -- Goldstein and Price, Math.Comp. 25, 569 (1971)
*/
  int npfn, i, j, jhold, nloop, loop=0, jh, jl;
  double amax, ystar, ystst, pb, ep, wg, xi, sigsav, sig2, reg;
  double dsav[MNMAXINT], y[MNMAXINT+1];

  if (npar<=0) return;
  check_amin();
  cstatu="UNCHANGED ";
  itaur=1;
  epsi=up*0.01;
  npfn=nfcn;
  nloop=(int)argum[1];
  if (nloop<=0) nloop=npar+4;
  wg=1./(double)npar;
  sigsav=edm;
  apsi=mnamin;
  memcpy(pmat, vhmat, sizeof(vhmat));
  memcpy(xt, x, sizeof(x));
  memcpy(dsav, werr, sizeof(werr));
  if (mnvert(pmat, npar))
    goto L280;
  memcpy(vthmat, pmat, sizeof(vthmat)); /* Save inverted matrix in VT */
L20:
  for (i=0; i<npar; ++i){
    dirin[i]=dsav[i]*2;
    x[i]=xt[i]+dirin[i]*2*((double) rand()/RAND_MAX-0.5);
  }
  loop++;
  reg=2;
  if (print_level>=0)
    log_printf("START ATTEMPT NO. %d TO FIND NEW MINIMUM\r\n", loop);
L30:
  jl=jh=npar;  /* set up random simplex */
  y[npar]=amax=mnamin=mncalf(vthmat, x);
  for (i=0; i<npar; i++){
    xi=x[i];
    x[i]-=dirin[i]*((double) rand()/RAND_MAX-0.5);
    y[i]=mncalf(vthmat, x);
    if (y[i]<mnamin){
      mnamin=y[i];
      jl=i;
    }
    else
      if (y[i]>amax){
        amax=y[i];
        jh=i;
      }
    for (j=0; j<npar; ++j)
      pmat[j][i]=x[j];
    x[i]=pmat[i][npar]=xi;
  }
  edm=mnamin;
  sig2=edm;
L50:  /* start main loop */
  if (mnamin<0)
    goto L95;
  if (err_matrix<=2)
    goto L280;
  ep=mnamin*0.1;
  if (sig2<ep && edm<ep)
    goto L100;
  sig2=edm;
  if (nfcn-npfn>nfcnmx)
    goto L300;
  for (i=0; i<npar; ++i) {  /* calculate new point * by reflection */
    pb=0;
    for (j=0; j<npar+1; ++j)
      pb+=wg*pmat[i][j];
    pbar[i]=pb-wg*pmat[i][jh];
    pstar[i]=pbar[i]*2-pmat[i][jh];
  }
  ystar=mncalf(vthmat, pstar);
  if (ystar>=mnamin)
    goto L70;
  for (i=0; i<npar; ++i)  /* point * better than jl, calculate new point ** */
    pstst[i]=pstar[i]*2-pbar[i];
  ystst=mncalf(vthmat, pstst);
  if (ystst<y[jl])
    goto L67;
  mnrazz(ystar, pstar, y, &jh, &jl);
  goto L50;
L67:
  mnrazz(ystst, pstst, y, &jh, &jl);
  goto L50;
L70:  /* point is not as good as jl */
  if (ystar>=y[jh])
    goto L73;
  jhold=jh;
  mnrazz(ystar, pstar, y, &jh, &jl);
  if (jhold!=jh)
    goto L50;
L73:  /* calculate new point */
  for (i=0; i<npar; ++i)
    pstst[i]=(pmat[i][jh]+pbar[i])*0.5;
  ystst=mncalf(vthmat, pstst);
  if (ystst>y[jh])
    goto L30;
  if (ystst<mnamin)  /* point is better than jh */
    goto L67;
  mnrazz(ystst, pstst, y, &jh, &jl);
  goto L50;
L95:  /* end main loop */
  if (print_level>=0)
    log_printf("AN IMPROVEMENT ON THE PREVIOUS MINIMUM HAS BEEN FOUND\r\n");
  reg=0.1;
L100:  /* ask if point is new */
  mninex(x); fcn(gin, &mnamin, u, 4); nfcn++;
  for (i=0; i<npar; ++i){
    dirin[i]=reg*dsav[i];
    if (fabs(x[i]-xt[i])>dirin[i])
      goto L150;
  }
  goto L230;
L150:
  nfcnmx+=npfn-nfcn;
  npfn=nfcn;
  mnsimp();
  if (mnamin>=apsi)
    goto L325;
  for (i=0; i<npar; ++i){
    dirin[i]=dsav[i]*0.1;
    if (fabs(x[i]-xt[i])>dirin[i])
      goto L250;
  }
L230:
  if (mnamin<apsi)
    goto L350;
  goto L325;
L250:  /* truly new minimum */
  lnewmn=TRUE;
  if (err_matrix!=0){
    err_matrix=1;
    dcovar=MAX(dcovar, 0.5);
  }
  else
    dcovar=1;
  itaur=0;
  nfcnmx+=npfn-nfcn;
  cstatu="NEW MINIMU";
  if (print_level>=0)
    log_printf("IMPROVE HAS FOUND A TRULY NEW MINIMUM\r\n");
  return;
L280:  /*return to previous region */
  if (print_level>0)
    log_printf("COVARIANCE MATRIX WAS NOT POSITIVE-DEFINITE\r\n");
  goto L325;
L300: call_limit_flag=1;
L325:
  for (i=0; i<npar; ++i){
    dirin[i]=dsav[i]*0.01;
    x[i]=xt[i];
  }
  mnamin=apsi;
  edm=sigsav;
L350:
  mninex(x);
  if (print_level>0)
    log_printf("IMPROVE HAS RETURNED TO REGION OF ORIGINAL MINIMUM\r\n");
  cstatu="UNCHANGED ";
  mnrset(0);
  if (err_matrix<2)
    goto L380;
  if (loop<nloop && !call_limit_flag)
    goto L20;
L380:
  mnprint(5);
  itaur=0;
}


__inline void CMinuit::mninex(double pint[]){
/* Transforms from internal coordinates (PINT) to external parameters (U).
   The minimizing routines which work in internal coordinates call this
   routine before calling FCN.
*/
  int i, j;

  for (j=0; j<npar ; ++j){
    i=nexofi[j]-1;
    if (nvarl[i]==1)
      u[i]=pint[j];
    else
      u[i]=alim[i]+(sin(pint[j])+1)*0.5*(blim[i]-alim[i]);
  }
}


void CMinuit::mninit(void){
/* This is the main initialization subroutine for MINUIT */
  int i;
  lnewmn=nfcn=nfcnfr=usergrad=converged=npfix=nu=npar=0;
  print_level=(int)1;
  up=(int)1;
  epsmac=DBL_EPSILON;
  epsma2=sqrt(DBL_EPSILON)*2.;
  
  memset(u, 0, sizeof(u));
  memset(niofex, 0, sizeof(niofex));
  for (i=0; i<MNMAXEXT; ++i)
  {
    strcpy(paraname[i], "UNDEFINED ");
    nvarl[i]=-1;
  }
  mnrset(1);
  
  cfrom="CLEAR   ";
  cstatu="UNDEFINED ";
  
  log_printf("\r\n\r\nMINUIT RELEASE 93.08 INITIALIZED.  DIMENSIONS %d/%d"
             "  EPSMAC=%.3E\r\nCPP-Version 97.04, Stephan Kappertz\r\n\r\n\r\n", MNMAXEXT, MNMAXINT, epsmac);
  
}


void CMinuit::mnlims(void){
/* Interprets the SET LIM command, to reset the parameter limits */
  int i2, kint, ifx;
  double dxdi;

  cfrom="SET LIM "; cstatu="NO CHANGE ";
  nfcnfr=nfcn;
  i2=(int)argum[0]-1;
  if (i2>=MNMAXEXT || i2<0) return;
  if (nvarl[i2]<=0){
    log_printf("PARAMETER %d IS NOT VARIABLE.\r\n", i2+1);
    return;
  }
  kint=niofex[i2]-1;
  ifx=-1;
  if (kint<0){    /* see if parameter was fixed */
    log_printf("REQUEST TO CHANGE LIMITS ON FIXED PARAMETER.\r\n");
    for (ifx=0; ifx<npfix; ifx++)
      if (i2+1==ipfix[ifx])
        break;
  }
  if (argum[1]==argum[2]){
    if (nvarl[i2]!=1){  /* remove limits */
      if (print_level>0)
        log_printf("LIMITS REMOVED FROM PARAMETER %d\r\n", i2+1);
      cstatu="NEW LIMITS";
      if (ifx>=0)
        gsteps[ifx]=fabs(gsteps[ifx]);
      else{
        dxdi=mndxdi(kint);
        if (fabs(dxdi)<0.01)
          dxdi=0.01;
        gstep[kint]=fabs(gstep[kint]*dxdi);
        grd[kint]*=dxdi;
       }
       nvarl[i2]=1;
    }
    else
      log_printf("NO LIMITS SPECIFIED.  PARAMETER %d IS ALREADY UNLIMITED.  NO CHANGE.\r\n", i2+1);
  }
  else{    /* put on limits */
    alim[i2]=MIN(argum[1], argum[2]);
    blim[i2]=MAX(argum[1], argum[2]);
    nvarl[i2]=4;
    if (print_level>0)
      log_printf("PARAMETER %d LIMITS SET TO %+E, %+E\r\n", i2+1, alim[i2], blim[i2]);
    cstatu="NEW LIMITS";
    if (ifx>=0)
      gsteps[ifx]=-0.1;
    else
      gstep[kint]=-0.1;
  }
  if (strcmp(cstatu, "NO CHANGE ")){
    mnexin(x);
    mnrset(1);
  }
}


void CMinuit::mnline(double start[], double fstart, double step[], double slope){
/* Perform a line search from position START along direction STEP,
   where the length of vector STEP gives the expected position of
   minimum.
   FSTART is value of function at START
   SLOPE (if non-zero) is df/dx along STEP at START
   TOLER is initial tolerance of minimum in direction STEP (auf 0.05 gesetzt)
   SLAMBG and ALPHA control the maximum individual steps allowed.
   The first step is always =1. The max length of second step is SLAMBG.
   The max size of subsequent steps is the maximum previous successful
   step multiplied by ALPHA + the size of most recent successful step,
   but cannot be smaller than SLAMBG.
*/
  int i, nvmax, nxypt;
  double slam, denom, flast, fvmin, xvmin, ratio, f1, f2, f3;
  double fvmax, toler8, overal, undral, slamin, slamax;
  double coeff[3], fvals[3], xvals[3], xpq[12], ypq[12];

  overal=1e3;  /* starting values for overall limits on total step SLAM */
  undral=-100;
  fvmin=ypq[0]=fstart;  /* set up linear search along STEP */
  xvmin=xpq[0]=slamin=nxypt=0;
  for (i=0; i<npar; ++i){
    if (step[i]!=0){
      ratio=fabs(start[i]/step[i]);
      if (slamin==0) slamin=ratio;
      if (ratio<slamin) slamin=ratio;
    }
    x[i]=start[i]+step[i];
  }
  if (slamin==0)
    slamin=epsmac;
  slamin*=epsma2;
  mninex(x); fcn(gin, &f1, u, 4); nfcn++;
  nxypt++;
  xpq[nxypt]=1;
  ypq[nxypt]=f1;
  if (f1<fstart){
    fvmin=f1;
    xvmin=1;
  }
  slam=1;  /* quadr interp using slope GDEL and two points */
  toler8=0.05;
  slamax=5;
  flast=f1;
L25:  /* can iterate on two-points (cut) if no imprvmnt */
  denom=(flast-fstart-slope*slam)*2/(slam*slam);
  slam=1;
  if (denom!=0) slam=-slope/denom;
  if (slam<0 || slam>slamax) slam=slamax;
  if (slam<toler8) slam=toler8;
  if (slam<slamin || (fabs(slam-1)<toler8 && f1<fstart)) goto L100;
  if (fabs(slam-1)<toler8) slam=toler8+1;
  if (nxypt>=11) goto L100;
  for (i=0; i<npar; ++i)
    x[i]=start[i]+slam*step[i];
  mninex(x); fcn(gin, &f2, u, 4); nfcn++;
  nxypt++;
  xpq[nxypt]=slam;
  ypq[nxypt]=f2;
  if (f2<fvmin){
    fvmin=f2;
    xvmin=slam;
  }
  if (fstart==fvmin){
    flast=f2;
    toler8=0.05*slam;
    overal=slam-toler8;
    slamax=overal;
    goto L25;
  }
  xvals[0]=xpq[0]; fvals[0]=ypq[0];
  xvals[1]=xpq[nxypt-1]; fvals[1]=ypq[nxypt-1]; 
  xvals[2]=xpq[nxypt]; fvals[2]=ypq[nxypt];
L50:  /* begin iteration, calculate desired step */
  slamax=MAX(slamax, fabs(xvmin)*2);
  mnpfit(xvals, fvals, coeff);
  if (coeff[2]<=0){
    if (coeff[2]*2*xvmin+coeff[1]<=0)
      slam=xvmin+slamax;
    else
      slam=xvmin-slamax;
  }
  else{
    slam=-coeff[1]/(coeff[2]*2);
    if (slam>xvmin+slamax)
      slam=xvmin+slamax;
    if (slam<xvmin-slamax)
      slam=xvmin-slamax;
  }
  if (slam>0){
    if (slam>overal)
      slam=overal;
  }
  else
    if (slam<undral)
      slam=undral;
L52:  /* come here if step was cut below */
  for (i=0; i<3; i++){
    if (fabs(slam-xvals[i])<MAX(toler8, fabs(toler8*slam)))
      goto L100;
  }
  if (nxypt>=11)  /* take the step */
    goto L100;
  for (i=0; i<npar; ++i)
    x[i]=start[i]+slam*step[i];
  mninex(x); fcn(gin, &f3, u, 4); nfcn++;
  nxypt++;
  xpq[nxypt]=slam;
  ypq[nxypt]=f3;
  fvmax=fvals[0];  /* find worst previous point out of three */
  nvmax=0;
  if (fvals[1]>fvmax){
    fvmax=fvals[1];
    nvmax=1;
  }
  if (fvals[2]>fvmax){
    fvmax=fvals[2];
    nvmax=2;
  }
/* if latest point worse than all three previous, cut step */
  if (f3>=fvmax){
    if (nxypt>=11)
      goto L100;
    if (slam>xvmin) overal=MIN(overal, slam-toler8);
    if (slam<xvmin) undral=MAX(undral, slam+toler8);
    slam=(slam+xvmin)*0.5;
    goto L52;
  }
/* prepare another iteration, replace worst previous point */
  xvals[nvmax]=slam;
  fvals[nvmax]=f3;
  if (f3<fvmin){
    fvmin=f3;
    xvmin=slam;
  }
  else{
    if (slam>xvmin) overal=MIN(overal, slam-toler8);
    if (slam<xvmin) undral=MAX(undral, slam+toler8);
  }
  if (nxypt<11)
    goto L50;
L100:  /* end of iteration */
  mnamin=fvmin;
  for (i=0; i<npar; i++){
    dirin[i]=step[i]*xvmin;
    x[i]=start[i]+dirin[i];
  }
  mninex(x);
}


void CMinuit::mnmatu(const int kode){
/* prints the covariance matrix v when KODE=1. Always prints the
   global correlations, and calculates and prints the individual
   correlation coefficients
*/
  int i, j, it, ncoef, nparm, nsofar;
  double vline[MNMAXINT];

  if (err_matrix==0){
    log_printf("%s\r\n", covmes[0]);
    return;
  }
  if (npar>0){
    if (kode==1){     /* external error matrix */
      mnemat();
      if (err_matrix<3)
        log_printf("%s\r\n", covmes[err_matrix]);
    }
    if (npar>1){
      mnwerr();
      ncoef=MIN((PAGEWIDTH-11)/7, 9);  /* NCOEF is number of coeff. that fit on one line */
      nparm=MIN(npar, ncoef);
      log_printf("\r\nPARAMETER CORRELATION COEFFICIENTS\r\nNO.GLOBAL");
      for (i=0; i<nparm; ++i)
        log_printf("%6d ", nexofi[i]);
      log_printf("\r\n");
      for (i=0; i<npar; ++i){
        for (j=0; j<npar; ++j)
          vline[j]=vhmat[i][j]/sqrt(fabs(vhmat[i][i]*vhmat[j][j]));
        nparm=MIN(npar, ncoef);
        log_printf("%2d %.4f ", nexofi[i], globcc[i]);
        for (j=0; j<nparm; ++j)
          log_printf("%+.3f ", vline[j]);
        log_printf("\r\n");
        if (i<nparm) continue;
        for (j=1; j<=9; ++j){
          nsofar=nparm;
          nparm=MIN(npar, nsofar+ncoef);
          log_printf("   ");
          for (it=nsofar; it<nparm; ++it)
            log_printf("%+.3f ", vline[it]);
          log_printf("\r\n");
          if (i<nparm)
            break;
        }
      }
      if (err_matrix<3)
        log_printf("%s\r\n", covmes[err_matrix]);
    }
  }
}


void CMinuit::mnmigr(void){
/* Performs a local function minimization using basically the method
   of Davidon-Fletcher-Powell as modified by Fletcher ref. Fletcher,
   Comp.J. 13,317 (1970) "switching method"
*/
  int i, j, npsdf, npfn, iter, iswtr, lined2, nfcnmg, nrstrt;
  double gdel, gami, d, vsum, fzero, dsum, gssq, fs, ri, delgam;
  double rhotol, gdgssq, gvg, vgi;
  double flnu[MNMAXINT], step[MNMAXINT], gs[MNMAXINT], vg[MNMAXINT], xxs[MNMAXINT];
  if (npar<=0) return;
  check_amin();
  cfrom="MIGRAD  ";
  npfn=nfcnfr=nfcnmg=nfcn;
  cstatu="INITIATE  ";
  iswtr=print_level-itaur*2;
  converged=-1;
  nrstrt=npsdf=lined2=0;
  rhotol=apsi*0.001;
  if (iswtr>0)
    log_printf("START MIGRAD MINIMIZATION.  STRATEGY %d.  CONVERGENCE WHEN EDM<%E\r\n", istrat, rhotol);
  if (istrat<2 || err_matrix>=3)   /* initialization strategy */
    goto L2;
L1:   /* come (back) here to restart completely */
  if (nrstrt>istrat){
    cstatu="FAILED    ";
    converged=-1;
    goto L230;
  }
  mnhess();  /* get full covariance and gradient */
  mnwerr();
  npsdf=0;
  if (err_matrix)
    goto L10;
L2:  /* get gradient at start point */
  mninex(x);
  if (usergrad){
    fcn(gin, &fzero, u, 2); nfcn++;
  }
  mnderi();
  if (err_matrix)
    goto L10;
  memset(step, 0, sizeof(step));
  memcpy(xxs, x, sizeof(x));  /* sometimes start with diagonal matrix */
  lined2++;              /* do line search if second derivative negative */
  if (lined2<(istrat+1)*npar)
    for (i=0; i<npar; ++i)
      if (g2[i]<=0){
        step[i]=-d_sign(gstep[i], grd[i]);
        mnline(xxs, mnamin, step, step[i]*grd[i]);
        goto L2;
      }
  memset(vhmat, 0, sizeof(vhmat));
  for (i=0; i<npar; ++i){ /* make diagonal error matrix */
    if (g2[i]<=0)
      g2[i]=1;
    vhmat[i][i]=2/g2[i];
  }
  dcovar=1;
L10:  /* ready to start first iteration */
  nrstrt++;
  if (nrstrt>istrat+1){
    cstatu="FAILED    ";
    goto L230;
  }
  fs=mnamin;
  edm=0; /* get EDM and set up loop */
  memcpy(gs, grd, sizeof(grd));
  memcpy(xxs, x, sizeof(x));
  for (i=0; i<npar; ++i){
    for (j=0; j<i; ++j)
      edm+=gs[i]*vhmat[i][j]*gs[j];
    edm+=gs[i]*gs[i]*0.5*vhmat[i][i];
  }
  edm*=0.5*(dcovar*3+1);
  if (edm<0){
    if (lwarn)
      log_printf("WARNING IN MIGRAD: STARTING MATRIX NOT POS-DEFINITE.\r\n");
    err_matrix=0;
    dcovar=1;
    goto L2;
  }
  if (err_matrix==0)
    edm=BIGEDM;
  iter=0;
  mninex(x);
  mnwerr();
  if (iswtr>=1)
    mnprint(3);
  if (iswtr>=2)
    mnmatu(0);
L24:  /* start main loop */
  if (nfcn-npfn>=nfcnmx)
    goto L190;
  gdel=gssq=0;
  for (i=0; i<npar; ++i){
    ri=0;
    gssq+=gs[i]*gs[i];
    for (j=0; j<npar; ++j)
      ri+=vhmat[i][j]*gs[j];
    step[i]=-ri*0.5;
    gdel+=step[i]*gs[i];
  }
  if (gssq==0)
    goto L300;
  if (gdel>=0){ /* if gdel positive, V not posdef */
    if (npsdf==1)
      goto L1;
    mnpsdf();
    npsdf=1;
    goto L24;
  }
  mnline(xxs, fs, step, gdel);   /* do line search */
  if (mnamin==fs)
    goto L200;
  cfrom="MIGRAD  ";
  nfcnfr=nfcnmg;
  cstatu="PROGRESS  ";
  mninex(x);   /* get gradient at new point */
  if (usergrad){
    fcn(gin, &fzero, u, 2); nfcn++;
  }
  mnderi();
  npsdf=0;   /* calculate new EDM */
L81:
  edm=gvg=delgam=gdgssq=0;
  for (i=0; i<npar; ++i){
    ri=vgi=0;
    for (j=0; j<npar; ++j){
      vgi+=vhmat[i][j]*(grd[j]-gs[j]);
      ri+=vhmat[i][j]*grd[j];
    }
    vg[i]=vgi*0.5;
    gami=grd[i]-gs[i];
    gdgssq+=gami*gami;
    gvg+=gami*vg[i];
    delgam+=dirin[i]*gami;
    edm+=grd[i]*ri*0.5;
  }
  edm*=0.5*(dcovar*3+1);
  if (edm<0 || gvg<=0){   /* if EDM negative, not positive-definite */
    cstatu="NOT POSDEF";
    if (npsdf==1)
      goto L230;
    mnpsdf();
    npsdf=1;
    goto L81;
  }
  iter++;  /* print information about this iteration */
  if (iswtr>=3 || (iswtr==2 && iter%10==1)){
    mnwerr();
    mnprint(3);
  }
  cstatu="IMPROVEMNT";  /* update covariance matrix */
  dsum=vsum=0;
  for (i=0; i<npar; ++i)
    for (j=0; j<=i; ++j){
      d=dirin[i]*dirin[j]/delgam-vg[i]*vg[j]/gvg;
      dsum+=fabs(d);
      vhmat[i][j]+=d*2;
      vhmat[j][i]=vhmat[i][j];
      vsum+=fabs(vhmat[i][j]);
    }
/* smooth local fluctuations by averaging DCOVAR */
  dcovar=(dcovar+dsum/vsum)*0.5;
  if (iswtr>=3)
    log_printf("RELATIVE CHANGE IN COV. MATRIX=%.2f%%\r\n", dcovar*100);
  if (delgam>gvg){
    for (i=0; i<npar; ++i)
      flnu[i]=dirin[i]/delgam-vg[i]/gvg;
    for (i=0; i<npar; ++i)
      for (j=0; j<=i; ++j){
        vhmat[i][j]+=gvg*2*flnu[i]*flnu[j];
        vhmat[j][i]=vhmat[i][j];
      }
  }
  if (edm<rhotol*0.1) /* and see if converged */
    goto L300;
  memcpy(xxs, x, sizeof(x));
  memcpy(gs, grd, sizeof(grd));  /* if not, prepare next iteration */
  fs=mnamin;
  if ((err_matrix==0 && dcovar<0.5) || (err_matrix==3 && dcovar>0.1))
    err_matrix=1;
  if (err_matrix==1 && dcovar<0.05)
    err_matrix=3;
  goto L24;
/* end main loop */
L190:  /* call limit in MNMIGR */
  call_limit_flag=1;
  if (print_level>=0)
    log_printf("CALL LIMIT EXCEEDED IN MIGRAD.\r\n");
  cstatu="CALL LIMIT";
  goto L230;
L200:  /* fails to improve */
  if (iswtr>=1)
    log_printf("MIGRAD FAILS TO FIND IMPROVEMENT\r\n");
  memcpy(x, xxs, sizeof(xxs));
  if (edm<rhotol)
   goto L300;
  if (edm<fabs(epsma2*mnamin)){
    if (iswtr>=0)
      log_printf("MACHINE ACCURACY LIMITS FURTHER IMPROVEMENT.\r\n");
    goto L300;
  }
  if (istrat==0){
    if (print_level>=0)
      log_printf("MIGRAD FAILS WITH STRATEGY=0.  WILL TRY WITH STRATEGY=1.\r\n");
    istrat=1;
  }
  goto L1;
L230:  /* fails to converge */
  if (iswtr>=0)
    log_printf("MIGRAD TERMINATED WITHOUT CONVERGENCE.\r\n");
  if (err_matrix==3)
    err_matrix=1;
  converged=-1;
  goto L400;
L300:   /* apparent convergence */
  if (iswtr>=0)
    log_printf("MIGRAD MINIMIZATION HAS CONVERGED.\r\n");
  if (itaur==0){
    if (istrat==2 || (istrat==1 && err_matrix<3)){
      if (print_level>=0)
        log_printf("MIGRAD WILL VERIFY CONVERGENCE AND ERROR MATRIX.\r\n");
      mnhess();
      mnwerr();
      npsdf=0;
      if (edm>rhotol)
        goto L10;
    }
  }
  cstatu="CONVERGED ";
  converged=1;
L400:  /* come here in any case */
  cfrom="MIGRAD  ";
  nfcnfr=nfcnmg;
  mninex(x);
  mnwerr();
  if (iswtr>=0)
    mnprint(3);
  if (iswtr>=1)
    mnmatu(1);
}


void CMinuit::mnmnos(void){
/* Performs a MINOS error analysis on those parameters for which it
   is requested on the MINOS command.
*/
  int ilax, iin, knt, nbad, ngood, nfcnmi;

  if (npar<=0)
    goto L700;
  ngood=nbad=0;
  nfcnmi=nfcn;
  for (knt=1; knt<=npar; ++knt){  /* loop over parameters requested */
    if ((int)argum[1]==0)
      ilax=nexofi[knt-1];
    else{
      if (knt>MAXARG) break;
      ilax=(int)argum[knt];
      if (ilax==0) break;
      if (ilax>0 && ilax<=nu){
        if (niofex[ilax-1]>0)
          goto L565;
      }
      log_printf("PARAMETER NUMBER %d NOT VARIABLE. IGNORED.\r\n", ilax);
      continue;
    }
L565:
    mnmnot(ilax-1); /* calculate one pair of ME's */
    if (lnewmn)
      goto L650;
    iin=niofex[ilax-1];  /* update NGOOD and NBAD */
    if (erp[iin-1]>0) ngood++;
    else nbad++;
    if (ern[iin-1]<0) ngood++;
    else nbad++;
  }
  cfrom="MINOS   ";   /* printout final values */
  nfcnfr=nfcnmi;
  cstatu="UNCHANGED ";
  if (ngood==0 && nbad==0)
    goto L700;
  if (ngood>0 && nbad==0)
    cstatu="SUCCESSFUL";
  if (ngood==0 && nbad>0)
    cstatu="FAILURE   ";
  if (ngood>0 && nbad>0)
    cstatu="PROBLEMS  ";
  if (print_level>=0)
    mnprint(4);
  if (print_level>=2)
    mnmatu(0);
  return;
L650:  /* new minimum found */
  cfrom="MINOS   ";
  nfcnfr=nfcnmi;
  cstatu="NEW MINIMU";
  if (print_level>=0)
    mnprint(4);
  log_printf("NEW MINIMUM FOUND.  GO BACK TO MINIMIZATION STEP.\r\n\r\n");
  return;
L700:
  log_printf("THERE ARE NO MINOS ERRORS TO CALCULATE.\r\n");
}


void CMinuit::mnmnot(const int ilax){
/* Performs a MINOS error analysis on one parameter. The parameter ILAX
   is varied, and the minimum of the function with respect to the other
   parameters is followed until it crosses the value FMIN+UP.
   Wird mit ilax-1 aufgerufen.
*/
  int i, it, nfmxin, isw2, isw4, marc, nlimit, iercr;
  double sigsav, dc, abest, ut, xunit, du1, sig, delu, aopt, eros, sav;
  double xdev[MNMAXINT], w[MNMAXINT], gcc[MNMAXINT];
  CString csig;

  isw2=err_matrix;
  isw4=converged;
  sigsav=edm;
  dc=dcovar;
  lnewmn=FALSE;
  apsi=epsi*0.5;
  abest=mnamin;
  nfmxin=nfcnmx;
  memcpy(vthmat, vhmat, sizeof(vhmat));
  memcpy(xt, x, sizeof(x));
  memcpy(gcc, globcc, sizeof(globcc));
  memcpy(w, werr, sizeof(werr));
  it=niofex[ilax]-1;
  erp[it]=ern[it]=0;
  mninex(xt);
  ut=u[ilax];
  if (nvarl[ilax]==1){
    alim[ilax]=ut-w[it]*100;
    blim[ilax]=ut+w[it]*100;
  }
  xunit=sqrt(up/vthmat[it][it]);
  marc=0;
  for (i=0; i<npar; ++i)
    if (i!=it){
      xdev[marc]=xunit*vthmat[i][it];
      marc++;
    }
  if (mnfixp(it)){  /* fix the parameter in question */
    log_printf("MINUIT ERROR. CANNOT FIX PARAMETER %d\r\n", ilax+1);
    goto L700;
  }
/* Nota Bene: from here on, NPAR=MPAR-1. Remember: MNFIXP squeezes
   IT out of X, XT, WERR, and VHMAT, not W, VTHMAT */
  for (sig=-1; sig<2; sig+=2){
    if (sig==1) csig="POSI";
    else csig="NEGA";
    if (print_level>1)
      log_printf("DETERMINATION OF %sTIVE MINOS ERROR FOR PARAMETER %d, %s\r\n", csig, ilax+1, paraname[ilax]);
    nlimit=nfcn+nfmxin;
/*    istrat=MAX(istrav-1, 0); weggelassen, hat nicht richtig funktioniert */
    du1=w[it];
    u[ilax]=ut+sig*du1;
    u[ilax]=MIN(u[ilax], blim[ilax]);
    u[ilax]=MAX(u[ilax], alim[ilax]);
    delu=u[ilax]-ut;
    if (fabs(delu)/(fabs(ut)+fabs(u[ilax]))<epsmac)  /* stop if already at limit with negligible step size */
      goto L440;
    for (i=0; i<npar; ++i)
      x[i]=xt[i]+delu/w[it]*xdev[i];
    if (print_level>1)
      log_printf("PARAMETER %d SET TO %E + %E = %E\r\n", ilax+1, ut, delu, u[ilax]);
    ke1cr=ilax;  /* loop to hit AMIN+UP */
    xmidcr=u[ilax];
    xdircr=delu;
    mnamin=abest;
    nfcnmx=nlimit-nfcn;
    iercr=mncros(&aopt);
    if (abest-mnamin>up*0.01)
      goto L650;
    switch (iercr){
       case 1: goto L440;
       case 2: goto L450;
       case 3: goto L460;
    }
    eros=sig*du1+aopt*xdircr;    /* error successfully calculated */
    if (print_level>1)
      log_printf("THE %sTIVE MINOS ERROR OF PARAMETER %d, %s IS %+E\r\n\r\n", csig, ilax+1, paraname[ilax], eros);
    goto L480;
L440:  /* failure returns */
    if (print_level>0)
      log_printf("THE %sTIVE MINOS ERROR OF PARAMETER %d, %s EXCEEDS ITS LIMIT.\r\n\r\n", csig, ilax+1, paraname[ilax]);
    eros=UNDEFI;
    goto L480;
L450:
    if (print_level>0)
      log_printf("THE %sTIVE MINOS ERROR %d REQUIRES MORE THAN %d FUNCTION CALLS.\r\n\r\n", csig, ilax, nfmxin);
    eros=0;
    goto L480;
L460:
    if (print_level>0)
      log_printf("%sTIVE MINOS ERROR NOT CALCULATED FOR PARAMETER %d\r\n\r\n", csig, ilax+1);
    eros=0;
L480:
    if (sig<0) ern[it]=eros;
    else erp[it]=eros;
  }
/* parameter finished. reset v, normal termination */
  itaur=1;
  mnfree(1);
  memcpy(vhmat, vthmat, sizeof(vthmat));
  memcpy(werr, w, sizeof(w));
  memcpy(globcc, gcc, sizeof(gcc));
  memcpy(x, xt, sizeof(xt));
  mninex(x);
  edm=sigsav;
  mnamin=abest;
  err_matrix=isw2;
  converged=isw4;
  dcovar=dc;
  goto L700;
L650:  /* new minimum */
  itaur=lnewmn=1;
  err_matrix=converged=0;
  dcovar=1;
  sav=u[ilax];
  mnfree(1);
  u[ilax]=sav;
  mnexin(x);
  edm=BIGEDM;
L700:  /* in any case */
  itaur=0;
  nfcnmx=nfmxin;
}


void CMinuit::mnparm(char *name, double uk, double wk,double lowlim, double uplim){
/* User-callable. Implements one parameter definition, that is:
   NAME parameter name
   UK starting value
   WK starting step size or uncertainty
   return 0 if no problems
   return 1 if MNPARM unable to implement definition
*/
  if (nu==MNMAXEXT){
    log_printf("MINUIT USER ERROR.  TOO MANY PARAMETERS\r\n");
    exit(1);
  }
  strncpy(paraname[nu], name, 10);
  u[nu]=uk;
  
  alim[nu]=blim[nu]=nvarl[nu]=0;
  // Stephan 15.4.98
  alim[nu]=MIN(lowlim, uplim);
  blim[nu]=MAX(lowlim, uplim);
  if(lowlim!=uplim)nvarl[nu]=4;
  //
  if (wk<=0)
    log_printf("%2d %-10.10s %+E constant\r\n", nu+1, name, uk);
  else{
    if (npar==MNMAXINT){
      log_printf("MINUIT USER ERROR.  TOO MANY PARAMETERS\r\n");
      exit(1);
    }
    log_printf("%2d %-10.10s %+E %+E no limits\r\n", nu+1, name, uk, wk);
    nvarl[nu]++;
    nexofi[npar]=nu+1;
    niofex[nu]=npar+1;
    xt[npar]=x[npar]=uk;
    werr[npar]=dirin[npar]=wk;
    g2[npar]=up*2/(wk*wk);
    gstep[npar]=MAX(epsma2*8*fabs(x[npar]), dirin[npar]*0.1);
    grd[npar]=g2[npar]*dirin[npar];
    npar++;
  }
  nu++;
}


void CMinuit::mnpfit(double parx2p[], double pary2p[], double coef[]){
/* fit a parabola to 3 points
   parx2p(i) x value of point i
   pary2p(i) y value of point i
   coef2p(1...3) coefficients of the fitted parabola
   y=coef2p(1)+coef2p(2)*x+coef2p(3)*x**2
*/
  int i;
  double a, s, t, y, s2, x2, x3, x4, y2, xm, xy, x2y;

  x2=x3=x4=y=y2=xy=x2y=coef[0]=coef[1]=coef[2]=0;
  xm=(parx2p[0]+parx2p[1]+parx2p[2])/3; /* center x values for reasons of machine precision */
  for (i=0; i<3; i++){
    s=parx2p[i]-xm; t=pary2p[i];
    s2=s*s, x2+=s2, x3+=s*s2,  x4+=s2*s2;
    y+=t, y2+=t*t, xy+=s*t, x2y+=s2*t;
  }
  a=(3*x4-x2*x2)*x2-3*x3*x3;
  if (a!=0){
    coef[2]=(x2*(3*x2y-x2*y)-3*x3*xy)/a;
    coef[1]=(xy-x3*coef[2])/x2;
    coef[0]=(y-x2*coef[2])/3+xm*(xm*coef[2]-coef[1]);
    coef[1]-=xm*2*coef[2];
  }
}


void CMinuit::mnpout(const int iext, char **chnam, double *val, double *err, double *xlolim, double *xuplim){
/* Provides the user with information concerning the current
   status of parameter number IUEXT.
   CHNAM: the name of the parameter
   VAL: the current (external) value of the parameter
   ERR: the current estimate of the parameter uncertainty
   XLOLIM: the lower bound (or zero if no limits)
   XUPLIM: the upper bound (or zero if no limits)
*/
  int iint;

  *val=*xlolim=*xuplim=*err=0;
  *chnam="undefined";
  if (iext>=0 && iext<nu){
    iint=niofex[iext];
    if (nvarl[iext]>=0){
      *chnam=paraname[iext];
      *val=u[iext];
      if (iint>0)
        *err=werr[iint-1];
      if (nvarl[iext]==4){
        *xlolim=alim[iext];
        *xuplim=blim[iext];
      }
    }
  }
}


void CMinuit::mnprint(int prcode){
/* Prints the values of the parameters at the time of the call.
   also prints other relevant information such as function value,
   estimated distance to minimum, parameter errors, step sizes.
   According to the value of IKODE, the printout is:
   IKODE=INKODE=0 only info about function value
   1 parameter values, errors, limits
   2 values, errors, step sizes, internal values
   3 values, errors, step sizes, first derivs.
   4 values, parabolic errors, MINOS errors
   when INKODE=5, MNPRIN chooses IKODE=1,2, or 3, according to ISW(2)
*/
  char buffer2[15], buffer3[15];
  CString cx2,cx3;
  double x2, x3;
  int i, l;

  if (nu==0){
    log_printf("THERE ARE CURRENTLY NO PARAMETERS DEFINED\r\n");
    return;
  }
  if (prcode==5)
    prcode=MIN(err_matrix+1, 3);
  if (mnamin==UNDEFI) log_printf("\r\nFCN=unknown      ");
  else log_printf("\r\nFCN=%E ", mnamin);
  log_printf("FROM %s  STATUS=%s %5d CALLS  %5d TOTAL\r\n", cfrom, cstatu, nfcn-nfcnfr, nfcn);
  if (edm==BIGEDM) log_printf("                 EDM=unknown   ");
  else log_printf("                 EDM=%.4E", edm);
#ifdef LEVENBERG
  if (prcode==-1){
    log_printf(" LAMBDA=%E\r\n", mar_lambda);
    prcode=1;
  }
  else{
    log_printf(" STRATEGY=%d   ", istrat);
    if (err_matrix==0 || err_matrix==2 || dcovar==0)
      log_printf("       %s\r\n", covmes[err_matrix]);
    else
      log_printf("ERROR MATRIX UNCERTAINTY=%3d%%\r\n", (int)(MIN(dcovar, 1)*100));
  }
#else
  log_printf(" STRATEGY=%d   ", istrat);
  if (err_matrix==0 || err_matrix==2 || dcovar==0)
    log_printf("       %s\r\n", covmes[err_matrix]);
  else
    log_printf("ERROR MATRIX UNCERTAINTY=%3d%%\r\n", (int)(MIN(dcovar, 1)*100));
#endif
  if (prcode==0)
    return;
  log_printf(" EXT PARAMETER               ");
  if (prcode!=4)
    switch (err_matrix)
    {
      case 0: log_printf("  CURRENT GUESS "); break;
      case 3: log_printf("                "); break;
      default: log_printf("   APPROXIMATE  ");
    }
  switch (prcode) 
  {
    case 1: log_printf("         PHYSICAL LIMITS\r\n"); break;
    case 2: log_printf("   INTERNAL       INTERNAL\r\n"); break;
    case 3: log_printf("      STEP          FIRST\r\n"); break;
    case 4: log_printf("    PARABOLIC            MINOS ERRORS\r\n");
  }
  log_printf(" NO.   NAME        VALUE     ");
  switch (prcode) {
    case 1: log_printf("      ERROR         NEGATIVE       POSITIVE\r\n"); break;
    case 2: log_printf("      ERROR       STEP SIZE        VALUE\r\n"); break;
    case 3: log_printf("      ERROR           SIZE        DERIVATIVE\r\n"); break;
    case 4: log_printf("      ERROR        NEGATIVE       POSITIVE\r\n");
  }
  for (i=0; i<nu; ++i){         /* loop over parameters */
    if (nvarl[i]<0) continue;
    l=niofex[i]-1;
    if (l>=0){    /* variable parameter. */
      cx2=cx3="";
      switch (prcode){
        case 1: if (nvarl[i]<=1){
                  log_printf("%3d %-10.10s  %+E  %+E\r\n", i+1, paraname[i], u[i], werr[l]);
                  continue;
                 }
                 else
                   x2=alim[i], x3=blim[i];
                 break;
        case 2: x2=dirin[l]; x3=x[l]; break;
        case 3: x2=dirin[l]; x3=grd[l];
                if (nvarl[i]>1 && fabs(cos(x[l]))<0.001)
                  cx3="** at limit **";
                break;
        case 4: x2=ern[l]; x3=erp[l];
                 if (x2==0) cx2="             0";
                 if (x2==UNDEFI) cx2="   at limit  ";
                 if (x3==0) cx3="             0";
                 if (x3==UNDEFI) cx3="   at limit  ";
				 break;
		default:break;
      }
      if (cx2==""){sprintf(buffer2, "%+E", x2); cx2=buffer2;}
      if (cx3==""){sprintf(buffer3, "%+E", x3); cx3=buffer3;}
      log_printf("%3d %-10.10s  %+E  %+E  %s  %s\r\n", i+1, paraname[i], u[i], werr[l], cx2, cx3);
      if (nvarl[i]<=1 || prcode==3) /* check if parameter is at limit */
        continue;
       if (fabs(cos(x[l]))<0.001)
        log_printf("WARNING: ABOVE PARAMETER IS AT LIMIT.\r\n");
    }
    else{    /* print constant or fixed parameter. */
      log_printf("%3d %-10.10s  %+E    ", i+1, paraname[i], u[i]);
      if (nvarl[i]>0){
        log_printf("  fixed ");
        if (nvarl[i]==4 && prcode==1)
          log_printf("     %+E  %+E", alim[i], blim[i]);
        log_printf("\r\n");
      }
      else
        log_printf("constant\r\n");
    }
  }
  if (up!=UPDFLT)
    log_printf("ERR DEF=%f\r\n", up);
}


void CMinuit::mnpsdf(void){
/* calculates the eigenvalues of v to see if positive-def. if not,
   adds constant along diagonal to make positive.
*/
  int i, j;
  double epspdf, dgmin, pmin, pmax, padd, dg, s[MNMAXINT];

  epspdf=MAX(1e-6, epsma2);
  dgmin=vhmat[0][0];
  for (i=1; i<npar; ++i)
    if (vhmat[i][i]<dgmin)
      dgmin=vhmat[i][i];
  if (dgmin<=0){
    dg=epspdf+1-dgmin;
    if (lwarn)
      log_printf("WARNING IN %s: Negative diagonal element in Error Matrix\r\n"
                 "%E added to diagonal\r\n", cfrom, dg);
  }
  else
    dg=0;
  for (i=0; i<npar; ++i){ /* Store VHMAT in P, make sure diagonal pos. */
    vhmat[i][i]+=dg;
    s[i]=1./sqrt(vhmat[i][i]);
    for (j=0; j<=i; j++)
      pmat[i][j]=pmat[j][i]=vhmat[i][j]*s[i]*s[j];
  }
  mneig(pmat, npar, pstar, epspdf);
  pmin=pmax=pstar[0];
  for (i=1; i<npar; i++){
    if (pstar[i]<pmin) pmin=pstar[i];
    if (pstar[i]>pmax) pmax=pstar[i];
  }
  pmax=MAX(fabs(pmax), 1);
  if ((pmin<=0 && lwarn) || print_level>=2){
    log_printf("EIGENVALUES OF SECOND-DERIVATIVE MATRIX:\r\n");
    for (i=0; i<npar; ++i)
      log_printf("%f ", pstar[i]);
    log_printf("\r\n");
  }
  if (pmin<=epspdf*pmax){
    if (err_matrix==3)
      err_matrix=2;
    padd=pmax*0.001-pmin;
    for (i=0; i<npar; ++i)
      vhmat[i][i]*=padd+1;
    cstatu="NOT POSDEF";
    if (lwarn)
      log_printf("WARNING IN %s: MATRIX FORCED POS-DEF BY ADDING %E TO DIAGONAL.\r\n", cfrom, padd);
  }
}


void CMinuit::mnrazz(const double ynew, double pnew[MNMAXINT], double y[], int *jh, int *jl){
/* Called only by MNSIMP and MNIMPR to add a new point and remove an
   old one from the current simplex, and get the estimated distance to minimum.
*/
  double pbig, plit;
  int i, j;

  for (i=0; i<npar; ++i)
    pmat[i][*jh]=pnew[i];
  y[*jh]=ynew;
  if (ynew<mnamin){
    memcpy(x, pnew, sizeof(x));
    mninex(x);
    mnamin=ynew;
    cstatu="PROGRESS  ";
    *jl=*jh;
  }
  *jh=0;
  for (j=1; j<=npar; j++)
    if (y[j]>y[*jh])
      *jh=j;
  edm=y[*jh]-y[*jl];
  if (edm<=0)
    log_printf("FUNCTION VALUE DOES NOT SEEM TO DEPEND ON ANY OF THE VARIABLE PARAMETERS.\r\n"
               "VERIFY THAT STEP SIZES ARE BIG ENOUGH AND CHECK FCN LOGIC.\r\n");
  else{
    for (i=0; i<npar; ++i){
      plit=pbig=pmat[i][0];
      for (j=1; j<=npar; ++j){
        if (pmat[i][j]>pbig) pbig=pmat[i][j];
        if (pmat[i][j]<plit) plit=pmat[i][j];
      }
      dirin[i]=pbig-plit;
    }
  }
}


void CMinuit::mnrset(int iopt){
/* Called from MNCLER and whenever problem changes, for example after
   SET LIMITS, SET PARAM, CALL FCN. If IOPT=1, resets function value
   and errors to UNDEFINED. If IOPT=0, sets only MINOS errors to undefined
*/
  cstatu="RESET     ";
  if (iopt>0){
    mnamin=UNDEFI;
    edm=BIGEDM;
    converged=err_matrix=call_limit_flag=0;
    dcovar=1;
  }
  memset(erp, 0, sizeof(erp));
  memset(ern, 0, sizeof(ern));
  memset(globcc, 0, sizeof(globcc));
  if (err_matrix!=0){
    err_matrix=1;
    dcovar=MAX(dcovar, 0.5);
  }
}


void CMinuit::mnseek(void){
/* Performs a rough (but global) minimization by monte carlo search. Each
   time a new minimum is found, the search area is shifted to be centered
   at the best value.  Random points are chosen uniformly over a hypercube
   determined by current step sizes. The Metropolis algorithm accepts a
   worse point with probability exp(-d/UP), where d is the degradation.
   Improved points are of course always accepted.  Actual steps are random
   multiples of the nominal steps (DIRIN).
*/
  int ipar, istep, ifail, mxfail, mxstep;
  double dxdi, ftry, alpha, flast, xmid[MNMAXINT], xbest[MNMAXINT];
  mxfail=(int)argum[0];
  if (mxfail<=0) mxfail=npar*20+100;
  mxstep=mxfail*10;
  check_amin();
  alpha=argum[1];
  if (alpha<=0) alpha=3;
  if (print_level>0)
    log_printf("MNSEEK: MONTE CARLO MINIMIZATION USING METROPOLIS ALGORITHM\r\n"
               "TO STOP AFTER %d SUCCESSIVE FAILURES, OR %d STEPS\r\n"
               "MAXIMUM STEP SIZE IS %f ERROR BARS.\r\n", mxfail, mxstep, alpha);
  cstatu="INITIAL  ";
  if (print_level>=2)
    mnprint(2);
  cstatu="UNCHANGED ";
  ifail=0;
  flast=mnamin;
  for (ipar=0; ipar<npar; ipar++){  /* set up step sizes, starting values */
    dirin[ipar]=alpha*2*werr[ipar];
    if (nvarl[nexofi[ipar]-1]>1){
      dxdi=mndxdi(ipar);  /* parameter with limits */
      if (dxdi==0) dxdi=1;
      dirin[ipar]=alpha*2*werr[ipar]/dxdi;
      if (fabs(dirin[ipar])>6.2831859999999997)
        dirin[ipar]=6.2831859999999997;
    }
    xmid[ipar]=xbest[ipar]=x[ipar];
  }
  for (istep=1; istep<=mxstep; ++istep) {  /* search loop */
    if (ifail>=mxfail) break;
    for (ipar=0; ipar<npar; ++ipar)
      x[ipar]=xmid[ipar]+((double) rand()/RAND_MAX+(double) rand()/RAND_MAX-1)*0.5*dirin[ipar];
    mninex(x); fcn(gin, &ftry, u, 4); nfcn++;
    if (ftry<flast){
      if (ftry<mnamin){
        cstatu="IMPROVEMNT";
        mnamin=ftry;
        memcpy(xbest, x, sizeof(x));
        ifail=0;
        if (print_level>=2)
          mnprint(2);
      }
    }
    else{
      ifail++;    /* Metropolis algorithm */
      if (exp((mnamin-ftry)/up)<(double) rand()/RAND_MAX)
        continue;
    }
    memcpy(xmid, x, sizeof(x));  /* Accept new point, move there */
    flast=ftry;
  }
  if (print_level>1)
    log_printf("MNSEEK: %d SUCCESSIVE UNSUCCESSFUL TRIALS.\r\n", ifail);
  memcpy(x, xbest, sizeof(xbest));
  mninex(x);
  if (print_level>0)
    mnprint(2);
  if (print_level==0)
    mnprint(0);
}


void CMinuit::mnsimp(void){
/* Performs a minimization using the simplex method of Nelder
   and Mead (ref. -- Comp. J. 7,308 (1965))
*/
#define alpha 1
#define beta 0.5
#define gamma 2
#define rhomin 4
#define rhomax 8
#define rho1 (1+alpha)
#define rho2 (1+alpha+alpha*gamma)
  int i, j, npfn, jhold, ncycl, kg, nf, ns, jh, jl;
  double dmin, dxdi, yrho, f, ynpp1, aming, ypbar, bestx, ystar;
  double y1, y2, ystst, pb, wg, absmin, rho, sig2;
  double prho[MNMAXINT], y[MNMAXINT+1];
  if (npar<=0) return;
  check_amin();
  cfrom="SIMPLEX ";
  npfn=nfcnfr=nfcn;
  cstatu="UNCHANGED ";
  wg=1./(double)npar;
  if (print_level>=0)
    log_printf("\r\nSTART SIMPLEX MINIMIZATION.  CONVERGENCE WHEN EDM<%E\r\n", epsi);
  for (i=0; i<npar; ++i){
    dirin[i]=werr[i];
    dxdi=mndxdi(i);
    if (dxdi!=0)
      dirin[i]=werr[i]/dxdi;
    dmin=epsma2*fabs(x[i]);
    if (dirin[i]<dmin)
      dirin[i]=dmin;
  }
L1: /* choose the initial simplex using single-parameter searches */
  ynpp1=y[npar]=absmin=mnamin;
  jl=npar;
  for (i=0; i<npar; i++){
    aming=mnamin;
    pbar[i]=bestx=x[i];
    kg=ns=nf=0;
L4: x[i]=bestx+dirin[i];
    mninex(x); fcn(gin, &f, u, 4); nfcn++;
    if (f<=aming)
      goto L6;
    if (kg==1)  /* failure */
      goto L8;
    kg=-1;
    nf++;
    dirin[i]*=-0.4;
    if (nf<3)
      goto L4;
    ns=6;
L6:  /* success */
    bestx=x[i];
    dirin[i]*=3;
    aming=f;
    cstatu="PROGRESS  ";
    kg=1;
    ns++;
    if (ns<6)
      goto L4;
L8:  /* local minimum found in ith direction */
    y[i]=aming;
    if (aming<absmin){
      jl=i;
      absmin=aming;
    }
    x[i]=bestx;
    for (j=0; j<npar; j++)
      pmat[j][i]=x[j];
  }
  jh=npar;
  mnamin=y[jl];
  mnrazz(ynpp1, pbar, y, &jh, &jl);
  for (i=0; i<npar; ++i)
    x[i]=pmat[i][jl];
  mninex(x);
  cstatu="PROGRESS  ";
  if (print_level>=1)
    mnprint(5);
  edm=sig2=BIGEDM;
  ncycl=0;
L50:  /* start main loop */
  if (sig2<epsi && edm<epsi)
    goto L76;
  sig2=edm;
  if (nfcn-npfn>nfcnmx)
    goto L78;
/* calculate new point * by reflection */
  for (i=0; i<npar; ++i){
    pb=0;
    for (j=0; j<npar+1; ++j)
      pb+=wg*pmat[i][j];
    pbar[i]=pb-wg*pmat[i][jh];
    pstar[i]=(alpha+1)*pbar[i]-alpha*pmat[i][jh];
  }
  
  mninex(pstar); fcn(gin, &ystar, u, 4); nfcn++;
  if (ystar>=mnamin)
    goto L70;
/* point * better than jl, calculate new point ** */
  for (i=0; i<npar; ++i)
    pstst[i]=gamma*pstar[i]+(1-gamma)*pbar[i];
  mninex(pstst); fcn(gin, &ystst, u, 4); nfcn++;
/* try a parabola through ph, pstar, pstst.  min = prho */
  y1=(ystar-y[jh])*rho2;
  y2=(ystst-y[jh])*rho1;
  rho=(rho2*y1-rho1*y2)*0.5/(y1-y2);
  if (rho<rhomin)
    goto L66;
  if (rho>rhomax)
    rho=rhomax;
  for (i=0; i<npar; ++i)
    prho[i]=rho*pbar[i]+(1-rho)*pmat[i][jh];
  mninex(prho);
  fcn(gin, &yrho, u, 4);
  ++nfcn;
  if (yrho<y[jl] && yrho<ystst)
    goto L65;
  if (ystst<y[jl])
    goto L67;
  if (yrho>y[jl])
    goto L66;
L65:  /* accept minimum point of parabola, PRHO */
  
  mnrazz(yrho, prho, y, &jh, &jl);
  goto L68;
L66:
  if (ystst<y[jl])
    goto L67;
  
  mnrazz(ystar, pstar, y, &jh, &jl);
  goto L68;
L67:
  
  mnrazz(ystst, pstst, y, &jh, &jl);
L68:
  ++ncycl;
  if (print_level<2)
    goto L50;
  if (print_level>=3 || ncycl%10==0)
    mnprint(5);
  goto L50;
L70:  /* point * is not as good as jl */
  if (ystar>=y[jh])
    goto L73;
  jhold=jh;
  
  mnrazz(ystar, pstar, y, &jh, &jl);
  if (jhold!=jh)
    goto L50;
L73:  /* calculate new point */
  for (i=0; i<npar; ++i)
    pstst[i]=beta*pmat[i][jh]+(1-beta)*pbar[i];
  mninex(pstst);
  
  fcn(gin, &ystst, u, 4);
  ++nfcn;
  if (ystst>y[jh])
    goto L1;
  if (ystst<mnamin)  /* point ** is better than jh */
    goto L67;
  
  mnrazz(ystst, pstst, y, &jh, &jl);
  goto L50;
L76:  /*  end main loop */
  if (print_level>=0)
    log_printf("SIMPLEX MINIMIZATION HAS CONVERGED.\r\n");
  converged=1;
  goto L80;
L78:
  if (print_level>=0)
    log_printf("SIMPLEX TERMINATES WITHOUT CONVERGENCE.\r\n");
  cstatu="CALL LIMIT";
  converged=-1;
  call_limit_flag=1;
L80:
  for (i=0; i<npar; ++i){
    pb=0;
    for (j=0; j<npar+1; ++j)
      pb+=wg*pmat[i][j];
    pbar[i]=pb-wg*pmat[i][jh];
  }
  mninex(pbar); fcn(gin, &ypbar, u, 4); nfcn++;
  if (ypbar<mnamin)
    mnrazz(ypbar, pbar, y, &jh, &jl);
  mninex(x);
  if (nfcnmx+npfn-nfcn<npar*3)
    goto L90;
  if (edm>epsi*2)
    goto L1;
L90:
  if (print_level>=0)
    mnprint(5);
}


int CMinuit::mnvert(double a[MNMAXINT][MNMAXINT], const int n){
/* inverts a symmetric matrix. matrix is first scaled to have all
   ones on the diagonal (equivalent to change of units) but no
   pivoting is done since matrix is positive-definite.
*/
  int i, j, k;
  double q[MNMAXINT], s[MNMAXINT], pp[MNMAXINT];

  for (i=0; i<n; ++i){ /* scale matrix by sqrt of diag elements */
    if (a[i][i]<=0)
      return 1;
    else
      s[i]=1./sqrt(a[i][i]);
  }
  for (i=0; i<n; ++i)
    for (j=0; j<n; ++j)
      a[i][j]*=s[i]*s[j];
  for (i=0; i<n; ++i){  /* start main loop  */
    q[i]=1./a[i][i];  /* preparation for elimination step1 */
    pp[i]=1;
    a[i][i]=0;
    if (i!=0)
      for (j=0; j<=i-1; ++j){
        pp[j]=a[j][i];
        q[j]=a[j][i]*q[i];
        a[j][i]=0;
       }
    if (i<n-1)
      goto L51;
    else
      if (i==n-1)
        goto L60;
L51:for (j=i+1; j<n; ++j){
      pp[j]=a[i][j];
      q[j]=-a[i][j]*q[i];
      a[i][j]=0;
    }
L60:for (j=0; j<n; ++j)  /* elimination proper */
      for (k=j; k<n; ++k)
        a[j][k]+=pp[j]*q[k];
  }
  for (j=0; j<n; ++j)  /* elements of left diagonal and unscaling */
    for (k=0; k<=j; ++k)
      a[j][k]=a[k][j]=a[k][j]*s[k]*s[j];
  return 0;
}


void CMinuit::mnwerr(void){
/* Calculates the WERR, external parameter errors, and the global correlation
   coefficients, to be called whenever a new covariance matrix is available.
   calculate external error if v exists
*/
  int i, j;
  double denom, ba, al, dx, du;

  if (err_matrix!=0){
    for (j=0; j<npar; ++j){
      dx=sqrt(fabs(vhmat[j][j]*up));
      i=nexofi[j]-1;
      if (nvarl[i]>1){
        al=alim[i];
        ba=blim[i]-al;
        du=al+(sin(x[j]+dx)+1)*0.5*ba-u[i];
        if (dx>1)
          du=ba;
        dx=(fabs(du)+fabs(al+(sin(x[j]-dx)+1)*0.5*ba-u[i]))*0.5;
      }
      werr[j]=dx;
    }
    memcpy(pmat, vhmat, sizeof(vhmat));
    memset(globcc, 0, sizeof(globcc));
    if (mnvert(pmat, npar)==0)
      for (j=0; j<npar; ++j){
        denom=pmat[j][j]*vhmat[j][j];
        if (denom>=0 && denom<=1)
          globcc[j]=0;
        else
          globcc[j]=sqrt(1.-1./denom);
      }
  }
}


void CMinuit::exec_show_command(const int com_index){
  int save, i;

  switch(com_index){
    case 0: check_amin(); mnprint(0); break;   /* show fcn */
    case 1: check_amin(); mnprint(5); break;   /* show param */
    case 2: check_amin(); mnprint(1); break;   /* show limits */
    case 3: mnmatu(1); break;
    case 4: mnmatu(0); break;
    case 5: log_printf("ALLOWED PRINT LEVELS ARE:\r\n"
                       "-1: NO OUTPUT EXCEPT FROM \"SHOW\"\r\n"
                       " 0: REDUCED OUTPUT\r\n 1: NORMAL OUTPUT\r\n"
                       " 2: EXTRA OUTPUT FOR PROBLEM CASES\r\n"
                       " 3: MAXIMUM OUTPUT\r\n"
                       "CURRENT PRINTOUT LEVEL IS %d\r\n", print_level);
            break;
    case 6:
    case 7: if (usergrad)  /* show nograd, grad */
              log_printf("GRAD IS SET.  USER COMPUTES DERIVATIVES IN FCN.\r\n");
            else
              log_printf("NOGRAD IS SET.  DERIVATIVES NOT COMPUTED IN FCN.\r\n");
            break;
    case 8: log_printf("ERRORS CORRESPOND TO FUNCTION CHANGE OF %E\r\n", up); break;
    case 12: log_printf("ALLOWED STRATEGIES ARE:\r\n"  /* show strategy */
                        "0: MINIMIZE THE NUMBER OF CALLS TO FUNCTION\r\n"
                        "1: TRY TO BALANCE SPEED AGAINST RELIABILITY\r\n"
                        "2: MAKE SURE MINIMUM TRUE, ERRORS CORRECT\r\n"
                        "NOW USING STRATEGY %d\r\n", istrat);
             break;
    case 13: save=print_level;   /* show eigenvalues */
             print_level=3;
             if (err_matrix==0) log_printf("%s\r\n", covmes[0]);
             else mnpsdf();
             print_level=save;
             break;
    case 14: log_printf("doubleING-POINT NUMBERS ASSUMED ACCURATE TO %E\r\n", epsmac);
             break;
    case 15: for (i=0; i<npar; i++)
               if (erp[i]>0 || ern[i]<0){
                 mnprint(4); return;
               }
             log_printf("THERE ARE NO MINOS ERRORS CURRENTLY VALID.\r\n");
             break;
#ifdef LEVENBERG
    case 16: log_printf("LEVENBERG-MARQUARDT PARAMETER LAMBDA: %E\r\n", lambda_start); break;
#endif
	case 17: log_printf("Set start index\r\n"); break;
	case 18: log_printf("Set stop index\r\n"); break;
	case 19: log_printf("Enable/disable folded function optimization\r\n"); break;
    default: log_printf("ABOVE COMMAND IS ILLEGAL.  IGNORED\r\n");
  }
}


void CMinuit::exec_set_command(const int com_index){
  int i, save;

  switch(com_index){
    case 1: i=(int)argum[0]-1;
            if (i>=nu || i<0 || nvarl[i]<0){
              log_printf("UNDEFINED PARAMETER NUMBER.  IGNORED.\r\n");
              break;
            }
            u[i]=argum[1];
            mnexin(x);
            save=err_matrix;
            mnrset(1); /* Keep approximate covariance matrix, even if new param value */
            err_matrix=MIN(save, 1);
            cfrom="SET PARM";
            nfcnfr=nfcn;
            cstatu="NEW VALUES";
            break;
    case 2: mnlims(); break;
    case 5: print_level=MIN(MAX((int) argum[0], -1), 3); break;
    case 6: usergrad=0; break;
    case 7: mngrad(); break;
    case 8: if (argum[0]!=up){
              if (argum[0]<=0) up=UPDFLT;
              else up=argum[0];
              memset(ern, 0, sizeof(ern));
              memset(erp, 0, sizeof(erp));
              mnwerr();
            }
            break;
    case 9: lwarn=FALSE; break;
    case 10: lwarn=TRUE; break;
    case 11: srand((int) argum[0]);
             if (print_level>0)
               log_printf("MINUIT RANDOM NUMBER SEED SET TO %d\r\n", (int) argum[0]);
             break;
    case 12: istrat=MIN(MAX((int)argum[0], 0), 2); break;  /* set strategy */
    case 14: if (argum[0]>0 && argum[0]<0.1){               /* set epsmac */
               epsmac=argum[0];
               epsma2=sqrt(epsmac)*2;
             }
             break;
#ifdef LEVENBERG
    case 16: lambda_start=MIN(MAX(argum[0], 1e-10), 0.75); break;
#endif
	case 17: start_index= (USHORT)MAX(0,MIN(argum[0],pDataView->ActualIndex-1)); break;
	case 18: end_index	= (USHORT)MIN(pDataView->ActualIndex-1,argum[0]); break;
    case 19: if(argum[0]==0)
				 {
					 FASTCALC = FALSE;
					 FWHMG_last=FWHML_last=TAU_last=1e-99;
				 }
				 else 
				 {
					 FASTCALC = TRUE;
					 FWHMG_last=FWHML_last=TAU_last=1e-99;
				 }
				if(FASTCALC)log_printf("Folded functions lookup table enabled\r\n");
				else log_printf("Folded functions lookup table disabled\r\n");
				break;
	default: log_printf("ABOVE COMMAND IS ILLEGAL.  IGNORED\r\n");
  }
}


void CMinuit::mnexecute(char *com1, char *com2, int narg, double arguments[])
{
/* Interprets a command and takes appropriate action */
  static char *com_name[]={
   "MINImize", "SEEk    ", "SIMplex ", "MIGrad  ", "MINOs   ", "SET xxx ",
   "SHOw xxx", "FIX     ", "REStore ", "RELease ", "HESse   ", "IMProve ",
   "STAndard", "HELP    ", "EXIt    ","PLOres","LOAdpun","SAVepun","FITsave",
#ifdef LEVENBERG
"MAR-fit ",
#endif
""};
  static char *com_set_name[]={
		"FCN", "PAR", "LIM", "COV", "COR", "PRI", "NOG", "GRA",
		"ERR", "NOW", "WAR", "RAN", "STR", "EIG", "EPS", "MIN",
#ifdef LEVENBERG
		"LAM",
#endif
		"STA","STO","FAS",
		""};

  int i, iext, temp, nsuper, iint, lfreed, ltofix, lfixed;
  CString chwhy;

  narg=MIN(narg, MAXARG);
  memset(argum, 0, sizeof(argum));
  memcpy(argum, arguments, sizeof(argum[0])*narg);
  com_count++;
  if (print_level>=0){
    log_printf("\r\n**********\r\n** %4d ** %s %s", com_count, com1, com2);
    for (i=0; i<narg; ++i)
      log_printf(" %.3f ", argum[i]);
    log_printf("\r\n**********\r\n");
  }
  nfcnmx=(USHORT)argum[0];
  if (nfcnmx<=0) nfcnmx=npar*100+200+npar*npar*5;
  epsi=argum[1];
  if (epsi<=0) epsi=up*0.1;
  lnewmn=call_limit_flag=i=0;
  while ((*com_name[i]!='\0') && strncmp(com1, com_name[i], 3)) i++;
  if (*com_name[i]=='\0'){
    log_printf("UNKNOWN COMMAND IGNORED\r\n");
    return;
  }
  if (strncmp(com1, "MINO", 4)==0) i=4;
  if (i!=5 && i!=6){ /* set show */
    cfrom=com_name[i];
    nfcnfr=nfcn;
  }
  switch (i){
    case 0:
    case 3: goto L400;
    case 1: mnseek(); return;
    case 2: mnsimp(); return;
    case 4: goto L500;
    case 5:
    case 6: i=0;
            while ((*com_set_name[i]!='\0') && strncmp(com2, com_set_name[i], 3)) i++;
            if (*com_set_name[i]=='\0')
              log_printf("THE FORMAT OF THE COMMAND IS UNKNOWN.\r\n\r\n");
            else
              if (strncmp(com1, "SET", 3)) exec_show_command(i);
              else exec_set_command(i);
            return;
    case 7: goto L900;
    case 8: goto L1000;
    case 9: ltofix=FALSE; goto L901;
    case 10: goto L1400;
    case 11: goto L1600;
    case 12: stand(); return;
    case 13: mnhelp(com2); return;
    case 14: exit_minuit=TRUE; return;
	case 15: resi(); return;
	case 16: read_startwerte(com2); return;
	case 17: save_parameter(com2); return;
	case 18: save_function(com2); return;
#ifdef LEVENBERG
    case 19: marquardt_fit(); return;
#endif
  }
L400:
  temp=nfcn;
  apsi=epsi;
  mnmigr();
  mnwerr();
  if (converged>=1 || call_limit_flag || strncmp(com1, "MIG", 3)==0)
    return;
  nfcnmx+=temp-nfcn;
  temp=nfcn;
  mnsimp();
  if (!call_limit_flag){
    nfcnmx+=temp-nfcn;
    mnmigr();
    mnwerr();
  }
  return;
L500:
  epsi=up*0.1;
  nsuper=nfcn+(npar+1)*2*nfcnmx;
  do{
    mncuve();
    mnmnos();
    if (!lnewmn) return;
    mnrset(0);
    mnmigr();
    mnwerr();
  } while (nfcn<nsuper);
  log_printf("TOO MANY FUNCTION CALLS. MINOS GIVES UP\r\n");
  return;
L900: ltofix=TRUE;  /* fix */
L901: lfreed=lfixed=FALSE;   /* (also release) */
  if (narg==0){
    log_printf("NO PARAMETERS REQUESTED\r\n");
    return;
  }
  for (i=0; i<narg; ++i){
    iext=(int)argum[i];
    chwhy="IS UNDEFINED";
    if (iext<=0 || iext>nu || nvarl[iext-1]<0)
      goto L930;
    chwhy="IS CONSTANT";
    if (nvarl[iext-1]==0)
      goto L930;
    iint=niofex[iext-1];
    if (ltofix){
      chwhy="ALREADY FIXED";
      if (iint==0)
        goto L930;
      if (mnfixp(iint-1)==0)
        lfixed=TRUE;
    }
    else{
      chwhy="ALREADY VARIABLE";
      if (iint>0)
        goto L930;
      mnfree(-iext);
      lfreed=TRUE;
    }
    continue;
L930: log_printf("PARAMETER %d %s. IGNORED.\r\n", iext, chwhy);
  }
  if (lfreed || lfixed)
    mnrset(0);
  if (lfreed){
    err_matrix=converged=0;
    dcovar=1;
    edm=BIGEDM;
  }
  mnwerr();
  if (print_level>1)
    mnprint(5);
  return;
L1000: /* restore */
  lfreed=npfix>0;
  mnfree(0);
  if (lfreed){
    mnrset(0);
    err_matrix=0;
    dcovar=1;
    edm=BIGEDM;
  }
  return;
L1400:  /* hesse */
  mnhess();
  mnwerr();
  if (print_level>=0)
    mnprint(2);
  if (print_level>0)
    mnmatu(1);
  return;
L1600: /* improve */
  mncuve();
  mnimpr();
  if (lnewmn)
    goto L400;
  return;
}


void CMinuit::mninteractive(void)
{
	log_printf("\r\nENTER MINUIT COMMAND: \r\n");
	log_flush();
}

void CMinuit::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CMinuit diagnostics

#ifdef _DEBUG
void CMinuit::AssertValid() const
{
	CEditView::AssertValid();
}

void CMinuit::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMinuit message handlers


void CMinuit::OnChange() 
{
	// TODO: Add your control notification handler code here
	//Damit kein Dokument geupdated wird
}

void CMinuit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CString InputString;
	CString CopyString;
	int narg;
	char TempString[3][81];
	char com1[81],com2[81];
	double temp_argum[MAXARG];
	switch(nChar)
	{
		case VK_RETURN:
		{
			int Actual	= GetEditCtrl().LineFromChar(-1);
			int End		= GetEditCtrl().GetLineCount()-1;
			if (Actual!=End)
			{
				int Start=GetEditCtrl().LineIndex(GetEditCtrl().GetLineCount()-1);
				int Len=GetEditCtrl().LineLength(GetEditCtrl().GetLineCount()-1);
				GetEditCtrl().SetSel(Start+Len,Start+Len,TRUE);
			}

			strcpy(com1,"");
			strcpy(com2,"");

			InputString=getline();
			if(InputString!="")CommandList.AddTail(InputString);
			else InputString = "help";

			LastCommand = CommandList.GetTailPosition();
			if (CommandList.GetCount()>MaxCommands) CommandList.RemoveHead();
			LastCommand = CommandList.GetTailPosition();
			InputString.MakeUpper();

			narg=sscanf((LPCSTR)InputString, "%s %s",&TempString[0][0],
											 &TempString[1][0]);
			exit_minuit=FALSE;
			if (TempString[0][0]!='\0')strcpy(com1,TempString[0]);
			if (narg>1) 
			{
				if (!isdigit(TempString[1][0]))
				{
					strcpy(com2,TempString[1]);
					narg=sscanf((LPCSTR)InputString, "%s %s %lf %lf %lf %lf %lf %lf",
																&TempString[2][0],
																&TempString[2][0],
																&temp_argum[0],
																&temp_argum[1],
																&temp_argum[2],
																&temp_argum[3],
																&temp_argum[4],
																&temp_argum[5]);
					narg=narg-2;
				}
				else
				{
   					narg=sscanf((LPCSTR)InputString, "%s %lf %lf %lf %lf %lf %lf",
																&TempString[2][0],
																&temp_argum[0],
																&temp_argum[1],
																&temp_argum[2],
														 		&temp_argum[3],
																&temp_argum[4],
																&temp_argum[5]);
					narg=narg-1;
				}
			}
			else 
			{
				narg=0;
			}
			CWaitCursor Wait;
			mnexecute(com1, com2, narg, temp_argum);
			Wait.Restore();
			CalcAndShowFitFunc();
			if(pResView!=NULL) 
			{
				resi();
				GetParent()->BringWindowToTop();
				SetFocus();
			}
			log_printf("\r\nENTER MINUIT COMMAND: \r\n");
			log_flush();
			//CEditView::OnKeyDown(nChar, nRepCnt, nFlags);
			if (exit_minuit) GetParent()->DestroyWindow();
			break;
		}

		case VK_UP:
		{
			CString TheCommand;
			if(!CommandList.IsEmpty())
			{
				TheCommand = CommandList.GetPrev(LastCommand);
				printf(TheCommand);
				CharCount = TheCommand.GetLength();
				OLDCOMMAND=TRUE;
				if(LastCommand==NULL)
				{
					LastCommand = CommandList.GetHeadPosition();
				}
			}
			break;
		}

		case VK_DOWN:
		{
			CString TheCommand;
			if(!CommandList.IsEmpty())
			{
				TheCommand = CommandList.GetNext(LastCommand);
				printf(TheCommand);
				CharCount = TheCommand.GetLength();
				OLDCOMMAND=TRUE;
				if(LastCommand==NULL)
				{
					LastCommand = CommandList.GetTailPosition();
				}
			}
			break;
		}

		case VK_LEFT:
		{
			CharCount--;
			if(CharCount < 0) 
			{
				CharCount = 0;
			}
			break;
		}

		case VK_RIGHT:
		{
			CharCount++;
			break;
		}

		case VK_BACK:
		{
			CharCount--;
			if(CharCount < 0) 
			{
				CharCount = 0;
			}
			break;
		}	

		case VK_DELETE:
		{
			break;	
		}

		case VK_CONTROL:
		{
			CEditView::OnKeyDown(nChar, nRepCnt, nFlags);
			break;
		}

		default: 
		{
			int Actual	= GetEditCtrl().LineFromChar(-1);
			int End		= GetEditCtrl().GetLineCount()-1;
			if (Actual!=End)
			{
				int Start=GetEditCtrl().LineIndex(GetEditCtrl().GetLineCount()-1);
				int Len=GetEditCtrl().LineLength(GetEditCtrl().GetLineCount()-1);
				GetEditCtrl().SetSel(Start+Len,Start+Len,FALSE);
			}
			
			CEditView::OnKeyDown(nChar, nRepCnt, nFlags);
			break;
		}
	}
}

void CMinuit::KillMe()
{
	HAVEOWNER = FALSE;
	GetParent()->DestroyWindow();
}

void CMinuit::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete NewFont;
	if(HAVEOWNER)
	{
		pDataView->FIT=FALSE;
		pDataView->Invalidate(TRUE);
	}
	CEditView::PostNcDestroy();
}
