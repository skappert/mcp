// MCP for NTView.h : interface of the CMCPforNTView class
//
/////////////////////////////////////////////////////////////////////////////
#define WM_DATAREADY WM_USER+303

#define ERGO	0
#define GO		1
#define AUTOGO	2

class CMeasThread;
class MonitorView;

class CMCPforNTView : public CListView
{
protected: // create from serialization only
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	CMCPforNTView();

	DECLARE_DYNCREATE(CMCPforNTView)

// Attributes
public:
	afx_msg void	OnUpdateEditCopyCut(CCmdUI* pCmdUI);
	afx_msg void	OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void	OnEditCopy();
	afx_msg void	OnEditCut();
	afx_msg void	OnEditPaste();
	afx_msg LRESULT OnLSIKilled(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnScalerKilled(WPARAM wparam,LPARAM pWin);
	afx_msg LRESULT OnTimer(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnMeasurementKilled(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT OnMeasurementHalted(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT	OnSave(WPARAM wparam,LPARAM lparam);
	afx_msg void	OnGetFocus();
	afx_msg void	OnKillFocus();
	BOOL			IsSelected();
	BOOL			SaveBackup(void);
	BOOL			SaveAuto(void);
	void*			CopyActionObject(ActionObject* pActionObj);
	ActionObject*	GetSelectedAction(void);
	POSITION		GetSelectedPosition(void);
	TrackObject*	GetSelectedTrack(void);
	void			Remove(ActionObject* pActionObject);
	void			RearrangeTrackNum(void);
	PM_SpectrumObj* FindScaler(USHORT Track,USHORT SubReg);
	int				GetSelectedItem();
	CPtrList		ActionPointerList;

	UINT m_nClipboardFormat;
	CMCPforNTDoc* GetDocument();
	CImageList	*m_xpimagelistSmall;

	BOOL		m_bDragging;
   	CImageList	*m_pimagelist;
	CLSIView*	pCLSIView;
	
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMCPforNTView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL	TransferItem(int indexDrag, int indexDrop,BOOL IsFirst);
	void	OnButtonUp(void);
	virtual ~CMCPforNTView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void	IsRunningErgo(CCmdUI* pCmdUI);
	void	IsRunningGo(CCmdUI* pCmdUI);
	void	IsRunningAutoGo(CCmdUI* pCmdUI);
	void	IsRunningSave(CCmdUI* pCmdUI);
	void	program_scan(USHORT track, USHORT step);
	void	start_scan(void);
	void	track_start(USHORT track);
	void	track_end(USHORT track);
	USHORT	GetSteps(USHORT track);
	USHORT	GetScans(USHORT track);
	USHORT	GetRealScans(USHORT track);
	USHORT	GetNumScaler(USHORT track);
// Generated message map functions
public:

	//{{AFX_MSG(CMCPforNTView)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPopupNewArithscaler();
	afx_msg void OnPopupNewFdac();
	afx_msg void OnPopupNewFlukeswitch();
	afx_msg void OnPopupNewHpdac();
	afx_msg void OnPopupNewHpdekade();
	afx_msg void OnPopupNewIsoldemass();
	afx_msg void OnPopupNewRsdekade();
	afx_msg void OnPopupNewScaler();
	afx_msg void OnPopupNewTrack();
	afx_msg void OnGo();
	afx_msg void OnErgo();
	afx_msg void OnKill();
	afx_msg void OnLsibox();
	afx_msg void OnPopupNewTrigger();
	afx_msg void OnPopupNewAlexmotor();
	afx_msg void OnPopupNewKepkovoltage();
	afx_msg void OnPopupNewIsoldevoltage();
	afx_msg void OnPopupNewPremavoltage();
	afx_msg void OnConfigure();
	afx_msg void OnGpibreader();
	afx_msg void OnOutbit();
	afx_msg void OnFieldreader();
	afx_msg void OnTempreader();
	afx_msg void OnPopupFieldwriter();
	afx_msg void OnPopupGpibwriter();
	afx_msg void OnTempwriter();
	afx_msg void OnPopupNewHvdac();
	afx_msg void OnAutogo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL			HALTNOW;
private:
	int				indexdrag;
	int				indexdrop;
	BOOL			SaveWindow;
	
	DWORD			StartTime;
	double			Time;
	CFont*			NewFont1;
	DataView*		pDView;
	CMeasThread*	MeasDlg;
	ULONG			mytimer;
	ULONG			DataCount;
	ULONG			DataArrived;
	ULONG			DataSum;
	ULONG			ChannelSum;
	USHORT			maxstep;
	USHORT			maxtrack;
	USHORT			maxscan;
	USHORT			maxcycle;
	USHORT			cycle;
	USHORT			scan;
	USHORT			lastscan;
	USHORT			track;
	USHORT			channel;
	USHORT			scaler;
	USHORT			NumScaler;
	USHORT			RUNMODE;	
	USHORT			DataQueuePos;
};

#ifndef _DEBUG  // debug version in MCP for NTView.cpp
inline CMCPforNTDoc* CMCPforNTView::GetDocument()
   { return (CMCPforNTDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
