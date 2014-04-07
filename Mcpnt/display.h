// Display.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDisplay dialog

class CDisplay : public CDialog
{
// Construction
public:
	CDisplay(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDisplay)
	enum { IDD = IDD_SETUP };
	int		m_axisblue;
	int		m_axisgreen;
	int		m_axisred;
	int		m_axislines;
	int		m_axistext;
	int		m_datablue;
	int		m_datalines;
	int		m_dataxmax;
	int		m_dataxmin;
	int		m_dataymax;
	int		m_dataymin;
	int		m_errorbars;
	int		m_fitblue;
	int		m_fitgreen;
	int		m_fitred;
	int		m_fitfunres;
	int		m_fitlines;
	int		m_halfticklenx;
	int		m_numbers;
	int		m_points;
	int		m_selectblue;
	int		m_selectgreen;
	int		m_halftickleny;
	int		m_selectred;
	int		m_textblue;
	int		m_textgreen;
	int		m_textred;
	int		m_ticklabelsx;
	int		m_ticklabelsy;
	int		m_ticklenx;
	int		m_tickleny;
	int		m_ticklines;
	int		m_titleposx;
	int		m_titleposy;
	int		m_xaxistitlex;
	int		m_xaxistitley;
	int		m_yaxistitlex;
	int		m_yaxistitley;
	int		m_datared;
	int		m_datagreen;
	BOOL	m_autolsi;
	BOOL	m_autoplot;
	BOOL	m_autoscript;
	BOOL	m_autotemplate;
	int		m_backblue;
	int		m_backgreen;
	int		m_backred;
	int		m_lsidx;
	int		m_lsidy;
	int		m_lsix;
	int		m_lsiy;
	int		m_numberblue;
	int		m_numbergreen;
	int		m_numberred;
	int		m_plotdx;
	int		m_plotdy;
	int		m_plotx;
	int		m_ploty;
	int		m_scriptdx;
	int		m_scriptdy;
	int		m_scriptx;
	int		m_scripty;
	int		m_templatedx;
	int		m_templatedy;
	int		m_templatex;
	int		m_templatey;
	BOOL	m_automonitor;
	int		m_monitordx;
	int		m_monitordy;
	int		m_monitorx;
	int		m_monitory;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDisplay)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDisplay)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
