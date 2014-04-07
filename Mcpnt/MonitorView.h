// MonitorView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MonitorView view
#define CHAR_S 83
#define CHAR_N 78
#define CHAR_P 80
 
#define MaxMonitors 4


class MonitorView : public CView

{
	DECLARE_DYNCREATE(MonitorView)
public:

	BOOL IsSelected();
	UINT m_nClipboardFormat;
    // protected constructor used by dynamic creation
	MonitorView();

	BOOL MonitorFree();
	void ResizeWin(ULONG x,ULONG y);
	static MonitorView* pDataView;
	void AddDataPoint(USHORT channel,float y,float err);
	void SetXAxis(float Xmin,float Xmax);
	void SetYAxis(float Ymin,float Ymax);
	void SetWindowTitle(CString Title);
	void AddView(USHORT NoOfChannels,
				CString XTitle, CString XUnit,
				CString YTitle, CString YUnit,
				float Start,float Stop,ActionObject* pSpectrumObj,USHORT scans,BOOL showexpected);
	void DeleteAllViews(void);
   	void SetXTitle(CString XTitle,CString XUnit);
	void SetYTitle(CString YTitle,CString YUnit);
	void SetDrawType(int type,BOOL intonly)
		{data_type = type;IntOnly = intonly;};
	void RedrawNow(void);
	void ForceRedraw(void);
	void CalcNextScan(USHORT scan);
	float	DataPoint[MaxMonitors][3][MAXPOINTS];
	float	EstimatedDataPoint[MaxMonitors][3][MAXPOINTS];
	int		ActualIndex[MaxMonitors];
	BOOL	ShowExpected[MaxMonitors];
	int		actual_monitor;

	CString WindowTitle;
	CString MainTitle,XAxisTitle,YAxisTitle[MaxMonitors];
	CString XAxisUnit,YAxisUnit[MaxMonitors];

private:
	CMCPforNTApp* pApp;
	HCURSOR curs;
	BOOL IntOnly;
	BOOL UserIsSelecting;
	BOOL DrawOnlyDataPoints;
	BOOL IsActivated;
	BOOL FirstScan;
	MonitorChild* pDataChild;
	ActionObject* pAction[MaxMonitors];

	int data_type;

	float minX,maxX,minY[MaxMonitors],maxY[MaxMonitors];
	double StartX,StopX;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MonitorView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
private:
	virtual ~MonitorView();
	inline float	GetStep(float Min,float Max);
	inline void		DrawBoundingRect(CDC* pDC,CRect rect);
	inline void		DrawTitles(CDC* pDC,CRect rect);
	inline int		ConvertY(USHORT ActualView,float Yin,CRect rect,float YMin,float YMax);
	inline float	InvConvertX(int x,CRect rect,float XMin,float XMax);
	inline float	InvConvertY(USHORT ActualView,int y,CRect rect,float YMin,float YMax);
	inline int		ConvertErr(USHORT ActualView,float Yin,CRect rect,float YMin,float YMax);
	inline int		ConvertX(float Xin,CRect rect,float XMin,float XMax);
	inline void		DrawSelectedDataPoint(CDC* pDC,CRect rect,float x,float y,float err,float XMin,float XMax,float YMin,float YMax,USHORT Data);
	inline void		DrawXTicks(CDC *pDC,CRect rect,float XMin,float XMax);
	inline void		DrawYTicks(USHORT ActualView,CDC *pDC,CRect rect,float YMin,float YMax);

	void			DrawTheData(USHORT Data,CDC* pDC,CRect rect,float Xmin,float Xmax,float Ymin,float Ymax);
	void			DrawTheEstimatedData(USHORT Data,CDC* pDC,CRect rect,float Xmin,float Xmax,float Ymin,float Ymax);
	void			DrawDataPoint(CDC* pDC,CRect rect,USHORT index,int actual_type,USHORT Data);
	void			DrawEstimatedDataPoint(CDC* pDC,CRect rect,USHORT index,int actual_type,USHORT Data);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	USHORT NumScans;
	short NumOfChannels;
	short NumOfViews; 
	CWnd* m_pParent;
	USHORT SelectNum;
	//{{AFX_MSG(MonitorView)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPopupShowproperties();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
