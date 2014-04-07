// DataView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DataView view
#define CHAR_R 82
#define CHAR_S 83
#define CHAR_E 69
#define CHAR_L 76
#define CHAR_N 78
#define CHAR_P 80
#define CHAR_Z 90

class DataView : public CView

{
	DECLARE_DYNCREATE(DataView)
public:
	afx_msg void OnUpdateEditCopyCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnFilePrintPreview();
	BOOL IsSelected();
	void DoCutCopyPaste(CArchive &ar, BOOL bCut);
	UINT m_nClipboardFormat;
    // protected constructor used by dynamic creation
	DataView();
	DataView(CMinuit* pMinuitParent);
	DataView(ActionObject* pSpectrumObj);
	
	void	ResizeWin(ULONG x,ULONG y);
	static	DataView* pDataView;
	void	AddDataPoint(float x,float y,float err);
	void	DataSelecting(CPoint point);
	void	DrawXORLine(CDC* pDC,int pos, int height);
	void	Reset();
	void	SetXAxis(float Xmin,float Xmax);
	void	SetYAxis(float Ymin,float Ymax);
	void	SetTitle(CString title);
	void	SetWindowTitle(CString Title);
   	void	SetXTitle(CString XTitle,CString XUnit);
	void	SetYTitle(CString YTitle,CString YUnit);
	void	SetDrawType(int type,BOOL intonly)
		{data_type = type;IntOnly = intonly;};
	void	RedrawNow(void);
	
	float	DataPoint[3][MAXPOINTS];
	int		ActualIndex;
	CPoint	SelectPoint;
	int		SelectOfs;
	int		OldPos;
	
	BOOL	FIT;
	BOOL	STARTATZERO;
	BOOL	LOGVIEW;
	void	DrawFitFunction(CDC* pDC,CRect rect);
	float	minX,maxX,minY,maxY;
	CString WindowTitle;
	CString MainTitle,XAxisTitle,YAxisTitle;
	CString XAxisUnit,YAxisUnit;
	int		data_type;

private:
	double cor;
	CMCPforNTApp* pApp;
	HCURSOR curs;
	CRect MouseMoveStart;
	CRect SelectOldrect;
	BOOL IsFirstMove;
	BOOL UserIsSelecting;
	BOOL DrawSingleDataPoint;
	BOOL IsActivated;
	BOOL DataSelect;
	DataChild* pDataChild;
	CMinuit* pMinuit;
	CMinuit* pMinuitPar;

	float LastShownDataPoint;

	float fminX,fmaxX,fminY,fmaxY;

	ActionObject* pActionObject;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DataView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
	afx_msg LRESULT OnMinuitKilled(WPARAM wparam,LPARAM pWin);

// Implementation
private:
	virtual ~DataView();
	inline float	GetStep(float Min,float Max);
	inline void		DrawBoundingRect(CDC* pDC,CRect rect);
	inline void		DrawTitles(CDC* pDC,CRect rect,CString Title,CString XTitle,CString YTitle);
	inline int		ConvertY(float Yin,CRect rect,float YMin,float YMax);
	inline int		TextConvertY(float Yin,CRect rect,float YMin,float YMax);
	inline float	InvConvertX(int x,CRect rect,float XMin,float XMax);
	inline float	InvConvertY(int y,CRect rect,float YMin,float YMax);
	inline int		ConvertErr(float Yin,CRect rect,float YMin,float YMax);
	inline int		ConvertX(float Xin,CRect rect,float XMin,float XMax);
	inline void		DrawSelectedDataPoint(CDC* pDC,CRect rect,float x,float y,float err,float XMin,float XMax,float YMin,float YMax);
	inline void		DrawXTicks(CDC *pDC,CRect rect,float XMin,float XMax);
	inline void		DrawYTicks(CDC *pDC,CRect rect,float YMin,float YMax);


	void			DrawTheData(CDC* pDC,CRect rect,float Xmin,float Xmax,float Ymin,float Ymax);
	void			DrawDataPoint(CDC* pDC,CRect rect,USHORT index,int actual_type);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	BOOL COMMONWIDTH;
	BOOL IntOnly;
	CWnd* m_pParent;
	BOOL SCALER;
	BOOL ARITHSCALER;
	BOOL SelectMax;
	BOOL SelectBase;
	BOOL SelectHfs;
	USHORT SelectNum;
	BOOL DontDestroy;
	BOOL DontRedrawAll;
	int fitfunc;
	int sign;
	int em_start;
	int eb_start;
	BOOL equalspaced;
	int numofpeaks;
	FITPAR FitPar[20];
	//{{AFX_MSG(DataView)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPopupResizetofullscale();
	afx_msg void OnPopupShowproperties();
	afx_msg void OnPopupSavespec();
	afx_msg void OnPopupMinuitfitsession();
	afx_msg void OnDestroy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPopupLoadspec();
	afx_msg void OnPopupSavewmf();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
