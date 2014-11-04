// LSIView.h : header file
//
#define LsiWelcome		"Welcome to LSI-Box for "
#define DayMessage1		"Bonjour Madame > "
#define DayMessage2		"Bonjour Monsieur > "
#define NightMessage1	"Bonsoir Madame > "
#define NightMessage2	"Bonsoir Monsieur > "
#define WrongCommand    "Sorry, but I can not do that --> try help"

class CMCPforNTView;

typedef struct
	{
		char*	comm;	
		int		NumArgsMin;
		int		NumArgsMax;             
		CString (*func)(CMCPforNTView* ,...);   
		char*	Help;				
	} KOMMANDO;

	CString _exit(CMCPforNTView* pView,...);
	CString _help(CMCPforNTView* pView,...);
	CString _mem(CMCPforNTView* pView,...);
	CString _uptime(CMCPforNTView* pView,...);
	CString _global(CMCPforNTView* pView,...);
	CString _ver(CMCPforNTView* pView,...);
	CString _about(CMCPforNTView* pView,...);
	CString _who(CMCPforNTView* pView,...);
	CString _er(CMCPforNTView* pView,...);
	CString _go(CMCPforNTView* pView,...);
	CString _ergo(CMCPforNTView* pView,...);
	CString _autogo(CMCPforNTView* pView,...);
	CString _append(CMCPforNTView* pView,...);
	CString _ddprog(CMCPforNTView* pView,...);
	CString _clock(CMCPforNTView* pView,...);
	CString _click(CMCPforNTView* pView,...);
	CString _isotope(CMCPforNTView* pView,...);
	CString _element(CMCPforNTView* pView,...);
	CString _wavenumber(CMCPforNTView* pView,...);
	CString _cmdlog(CMCPforNTView* pView,...);
	CString _debug(CMCPforNTView* pView,...);
	CString _monitor(CMCPforNTView* pView,...);
	CString _setmonitor(CMCPforNTView* pView,...);
	CString _domonitor(CMCPforNTView* pView,...);
	CString _setclicks(CMCPforNTView* pView,...);
	CString _callcrosslist(CMCPforNTView* pView,...);
	CString _sweeptime(CMCPforNTView* pView,...);
	CString _cls(CMCPforNTView* pView,...);
	CString _disp(CMCPforNTView* pView,...);
	CString _dispnorm(CMCPforNTView* pView,...);
	CString _dispsum(CMCPforNTView* pView,...);
	CString _dispsumnorm(CMCPforNTView* pView,...);
	CString _dispprotons(CMCPforNTView* pView,...);
	CString _counteroffset(CMCPforNTView* pView,...);
	CString _setpmpred(CMCPforNTView* pView,...);
	CString _display(CMCPforNTView* pView,...);
	CString _ousers232(CMCPforNTView* pView,...);
	CString _odisp(CMCPforNTView* pView,...);
	CString _ores(CMCPforNTView* pView,...);
	CString _ochannel(CMCPforNTView* pView,...);
	CString _oread(CMCPforNTView* pView,...);
	CString _ostatus(CMCPforNTView* pView,...);
	CString _otrigger(CMCPforNTView* pView,...);
	CString _oexttrigger(CMCPforNTView* pView,...);
	CString _omeasdc(CMCPforNTView* pView,...);
	CString _ofasttrigger(CMCPforNTView* pView,...);
	CString _osavespec(CMCPforNTView* pView,...);
	CString _osavetimespec(CMCPforNTView* pView,...);
	CString _osavespecerr(CMCPforNTView* pView,...);
	CString _osavetimespecerr(CMCPforNTView* pView,...);
	CString _odelay(CMCPforNTView* pView,...);
	CString _omode(CMCPforNTView* pView,...);
	CString _savespec(CMCPforNTView* pView,...);
	CString _savespecerr(CMCPforNTView* pView,...);
	CString _saveasymspec(CMCPforNTView* pView,...);
	CString _sum(CMCPforNTView* pView,...);
	CString _norm(CMCPforNTView* pView,...);
	CString _asym(CMCPforNTView* pView,...);
	CString _vdisp(CMCPforNTView* pView,...);
	CString _list(CMCPforNTView* pView,...);
	CString _save(CMCPforNTView* pView,...);
	CString _asave(CMCPforNTView* pView,...);
	CString _setautosave(CMCPforNTView* pView,...);
	CString _autosave(CMCPforNTView* pView,...);
	CString _cy(CMCPforNTView* pView,...);
	CString _scaler(CMCPforNTView* pView,...);
	CString _sc(CMCPforNTView* pView,...);
	CString _lswitch(CMCPforNTView* pView,...);
	CString _lnoswitch(CMCPforNTView* pView,...);
	CString _lno(CMCPforNTView* pView,...);
	CString _l0(CMCPforNTView* pView,...);
	CString _l1(CMCPforNTView* pView,...);
	CString _v0(CMCPforNTView* pView,...);
	CString _lv0(CMCPforNTView* pView,...);
	CString _f0hp8(CMCPforNTView* pView,...);
	CString _f0hp3(CMCPforNTView* pView,...);
	CString _f0hp32(CMCPforNTView* pView,...);
	CString _f0rs(CMCPforNTView* pView,...);
	CString _hpoff(CMCPforNTView* pView,...);
	CString _hpfm(CMCPforNTView* pView,...);
	CString _hpam(CMCPforNTView* pView,...);
	CString _rsfm(CMCPforNTView* pView,...);
	CString _amp8(CMCPforNTView* pView,...);
	CString _amp3(CMCPforNTView* pView,...);
	CString _amp32(CMCPforNTView* pView,...);
	CString _amprs(CMCPforNTView* pView,...);
	CString _rsmodoff(CMCPforNTView* pView,...);
	CString _rsmodfreq(CMCPforNTView* pView,...);
	CString _rs0modfreq(CMCPforNTView* pView,...);
	CString _rs0fm(CMCPforNTView* pView,...);
	CString _rs0modoff(CMCPforNTView* pView,...);
	CString _getmass(CMCPforNTView* pView,...);
	CString _getstatus(CMCPforNTView* pView,...);
	CString _getmassfactor(CMCPforNTView* pView,...);
	CString _getfield(CMCPforNTView* pView,...);
	CString _getht(CMCPforNTView* pView,...);
	CString _setnetwrite(CMCPforNTView* pView,...);
	CString _unsetnetwrite(CMCPforNTView* pView,...);
	CString _voltages(CMCPforNTView* pView,...);
	CString _out(CMCPforNTView* pView,...);
	CString _in(CMCPforNTView* pView,...);
	CString _iec(CMCPforNTView* pView,...);
	CString _iecrec(CMCPforNTView* pView,...);
	CString _iecsend(CMCPforNTView* pView,...);
	CString _iecsendrec(CMCPforNTView* pView,...);
	CString _comsend(CMCPforNTView* pView,...);
	CString _comsendrec(CMCPforNTView* pView,...);
	CString _v(CMCPforNTView* pView,...);
	CString _lv(CMCPforNTView* pView,...);
	CString _c(CMCPforNTView* pView,...);
	CString _n(CMCPforNTView* pView,...);
	CString _lramp(CMCPforNTView* pView,...);
	CString _fhp8(CMCPforNTView* pView,...);
	CString _fhp3(CMCPforNTView* pView,...);
	CString _fhp32(CMCPforNTView* pView,...);
	CString _frs(CMCPforNTView* pView,...);
	CString _repeatsteps(CMCPforNTView* pView,...);
	CString _tomass(CMCPforNTView* pView,...);
	CString _tofluke(CMCPforNTView* pView,...);
	CString _setmass(CMCPforNTView* pView,...);
	CString _setfluke(CMCPforNTView* pView,...);
	CString _show(CMCPforNTView* pView,...);
	CString _info(CMCPforNTView* pView,...);
	CString _hardware(CMCPforNTView* pView,...);
	CString _camacclear(CMCPforNTView* pView,...);
	CString _clrinhibit(CMCPforNTView* pView,...);
	CString _setinhibit(CMCPforNTView* pView,...);
	CString _delete(CMCPforNTView* pView,...);
	CString _newtrack(CMCPforNTView* pView,...);
	CString _eichungtrack(CMCPforNTView* pView,...);
	CString _deftrack(CMCPforNTView* pView,...);
	CString _isoltrack(CMCPforNTView* pView,...);
	CString _slowisoltrack(CMCPforNTView* pView,...);
	CString _asymtrack(CMCPforNTView* pView,...);
	CString _superisoltrack(CMCPforNTView* pView,...);  
	CString _deltrack(CMCPforNTView* pView,...);
	CString _copytrack(CMCPforNTView* pView,...);
	CString _newaction(CMCPforNTView* pView,...);
	CString _moveaction(CMCPforNTView* pView,...);
	CString _configaction(CMCPforNTView* pView,...);
	CString _confighwaction(CMCPforNTView* pView,...);
	CString _configtrack(CMCPforNTView* pView,...);
	CString _updownsweep(CMCPforNTView* pView,...);
	CString _noupdownsweep(CMCPforNTView* pView,...);
	CString _configdata(CMCPforNTView* pView,...);
	CString _movetrack(CMCPforNTView* pView,...);
	CString _print(CMCPforNTView* pView,...);
	CString _channels(CMCPforNTView* pView,...);
	CString _beamgate(CMCPforNTView* pView,...);
	CString _refbit(CMCPforNTView* pView,...);
	CString _warnprotons(CMCPforNTView* pView,...);
	CString _readprotons(CMCPforNTView* pView,...);
	CString _badpulses(CMCPforNTView* pView,...);
	CString _redostep(CMCPforNTView* pView,...);
	CString _newredostepmode(CMCPforNTView* pView,...);
	CString _slowreset(CMCPforNTView* pView,...);
	CString _rampreset(CMCPforNTView* pView,...);
	CString _configreset(CMCPforNTView* pView,...);
	CString _uselp(CMCPforNTView* pView,...);
	CString _usedd(CMCPforNTView* pView,...);
	CString _useirqdd(CMCPforNTView* pView,...);
	CString _whatdd(CMCPforNTView* pView,...);
	CString _probedd(CMCPforNTView* pView,...);
	CString _irqdd(CMCPforNTView* pView,...);
	CString _irqokdd(CMCPforNTView* pView,...);
	CString _resetdd(CMCPforNTView* pView,...);
	CString _nextirqdd(CMCPforNTView* pView,...);
	CString _protonlimit(CMCPforNTView* pView,...);
	CString _ignoreprotons(CMCPforNTView* pView,...);
	CString _lookforprotons(CMCPforNTView* pView,...);
	CString _closebeamgate(CMCPforNTView* pView,...);
	CString _opengate(CMCPforNTView* pView,...);
	CString _closegate(CMCPforNTView* pView,...);
	CString _radioactive(CMCPforNTView* pView,...);
	CString _reference(CMCPforNTView* pView,...);
	CString _motor(CMCPforNTView* pView,...);
	CString _motorcm(CMCPforNTView* pView,...);
	CString _motornew(CMCPforNTView* pView,...);
	CString _setmotorcount(CMCPforNTView* pView,...);
	CString _getmotorcount(CMCPforNTView* pView,...);
	CString _fastmotor(CMCPforNTView* pView,...);
	CString _normalmotor(CMCPforNTView* pView,...);
	CString _motordelay(CMCPforNTView* pView,...);
	CString _stepdelay(CMCPforNTView* pView,...);
	CString _accelerate(CMCPforNTView* pView,...);
	CString _accsteps(CMCPforNTView* pView,...);
	CString _steps(CMCPforNTView* pView,...);
	CString _distance(CMCPforNTView* pView,...);
	CString _stepmode(CMCPforNTView* pView,...);
	CString _distmode(CMCPforNTView* pView,...);
	CString _direction(CMCPforNTView* pView,...);
	CString _minmotorcount(CMCPforNTView* pView,...);
	CString _maxmotorcount(CMCPforNTView* pView,...);
	CString _settaperad(CMCPforNTView* pView,...);
	CString _settapeth(CMCPforNTView* pView,...);
	CString _break(CMCPforNTView* pView,...);
	CString _bothmotors(CMCPforNTView* pView,...);
	CString _motorclicktime(CMCPforNTView* pView,...);
	CString _motorwaitclicks(CMCPforNTView* pView,...);
	CString _motorwaitfactor(CMCPforNTView* pView,...);
	CString _kepcofactor(CMCPforNTView* pView,...);
	CString _asymfactor(CMCPforNTView* pView,...);
	CString _warndelay(CMCPforNTView* pView,...);
	CString _regression(CMCPforNTView* pView,...);
	CString _dispkepco(CMCPforNTView* pView,...);
	CString _dispdelta(CMCPforNTView* pView,...);
	CString _dispisolde(CMCPforNTView* pView,...);
	CString _protondelay(CMCPforNTView* pView,...);
	CString _parttrigger(CMCPforNTView* pView,...);
	CString _triggerdelay(CMCPforNTView* pView,...);
	CString _range(CMCPforNTView* pView,...);
	CString _measperstep(CMCPforNTView* pView,...);
	CString _measatscan(CMCPforNTView* pView,...);
	CString _premafactor(CMCPforNTView* pView,...);
	CString _flukedelay(CMCPforNTView* pView,...);
	CString _isoldedelay(CMCPforNTView* pView,...);
	CString _computerdelay(CMCPforNTView* pView,...);
	CString _setmultino(CMCPforNTView* pView,...);
	CString _changescbit(CMCPforNTView* pView,...);
	CString _clearscbit(CMCPforNTView* pView,...);
	CString _halt(CMCPforNTView* pView,...);
	CString _kill(CMCPforNTView* pView,...);
	CString _running(CMCPforNTView* pView,...);
	CString _closemonitor(CMCPforNTView* pView,...);
	CString _closestatus(CMCPforNTView* pView,...);
	CString _mot(CMCPforNTView* pView,...);
	CString _grabframe(CMCPforNTView* pView,...);
	CString _dispframe(CMCPforNTView* pView,...);
	CString _setframename(CMCPforNTView* pView,...);
	CString _saveframe(CMCPforNTView* pView,...);
	CString _loadframe(CMCPforNTView* pView,...);
	CString _system(CMCPforNTView* pView,...);
	CString _setccdmode(CMCPforNTView* pView,...);
	CString _setccdarea(CMCPforNTView* pView,...);
	CString _cdisp(CMCPforNTView* pView,...);
	CString _camacfout(CMCPforNTView* pView,...);
	CString _camacfin(CMCPforNTView* pView,...);
	CString _usestrobe(CMCPforNTView* pView,...);
	CString _load(CMCPforNTView* pView,...);
	CString _domath(CMCPforNTView* pView,...);
	CString _f(CMCPforNTView* pView,...);
	CString _b(CMCPforNTView* pView,...);



	KOMMANDO	Commands[];
/////////////////////////////////////////////////////////////////////////////
// CLSIView view

class CLSIView : public CRichEditView
{
protected:
           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLSIView)

// Attributes
public:
	

	CLSIView();  
	CLSIView(CView* pParent); 
	void ResizeWin(ULONG x,ULONG y);
	BOOL	lsi_exit;

// Operations
public:
	void Dispatch(CString TheCommand);
	void Help();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLSIView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
	
// Implementation
protected:
	virtual ~CLSIView();
	void printf(const char *format, ...);
	void printfh(const char *format, ...);
	void log_over(const char *format, ...);
	void log_printf(const char *format, ...);
	void log_flush(void);
	CString FindRest(CString InputString);
	CString getline(void);
	

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
private:
	CString		TheMessage;
	BOOL		TOGGLE;
	CString		PrintString;
	BOOL		OLDCOMMAND;
	CStringList CommandList;
	POSITION    LastCommand;

	CView*		pMCPView;
	CLSIChild*	pCLSIChild;
	BOOL		IsActivated;
	CFont*		NewFont,OldFont;
	CFont*		NewFont1;
	SHORT		CharCount;

	// Generated message map functions
protected:
	//{{AFX_MSG(CLSIView)
	afx_msg void OnChange();
	afx_msg void OnUpdate();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
