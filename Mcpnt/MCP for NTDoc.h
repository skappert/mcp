// MCP for NTDoc.h : interface of the CMCPforNTDoc class
//
/////////////////////////////////////////////////////////////////////////////
#define	NewHeader "MCP data file, created with %s Version %s, compiled: %s %s -- last Go file: "
#define	CompatibleHeader "MCP data file, Version 1.0 (NT) Version (20) mode, compiled: 5.6.1997 -- last saved file: "
#define ActualVersion	21
#define CompatibleVersion		20

class ActionObject;
class TrackObject;
class MonitorView;

typedef struct
{
   USHORT step;
   USHORT scan;
   ActionObject* pAction;
} DataQueueMember;

class CMCPforNTDoc : public CDocument
{
public: // create from serialization only
	CMCPforNTDoc();
protected:
	DECLARE_DYNCREATE(CMCPforNTDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated  function overrides
	//{{AFX_(CMCPforNTDoc)
	public:
	 BOOL OnNewDocument();
	 void Serialize(CArchive& ar);
	//}}AFX_

// Implementation
public:
	 ~CMCPforNTDoc();
#ifdef _DEBUG
	 void AssertValid() const;
	 void Dump(CDumpContext& dc) const;
#endif
public:
	void	DispatchActionObj	(CArchive& ar);
	void	SaveActionObj		(CArchive& ar);
	CString	GetInfo(void);
	void	UpdateAllData(int track);
	void	SetNewCenter(USHORT track,USHORT step);
	void	SetNewMinMax(USHORT track,double minstep, double maxstep);
	bool	OnSave(CString SaveString);
	void	TakeTimeStamp(void);
	CTime	CStringToCTime( CString timeString );

public:
	BOOL			SaveActualVersion;
	BOOL			MeasurementRunning;
	BOOL			TrackRunning;
	BOOL			DataValid;
	CPtrList		pChildWin;
	CPtrList		ActionObjList;
	TrackObject*	pActualTrack;
	unsigned char	Control;
	CString			Header;
	int				Version;
	CString			Time;
	CString			TheElement;
	double			WaveNumber;
	int				NoOfCycles;
	int				NoOfMaximumCycles;
	BOOL			DoMonitor;
	int				TrackNum;


	DataQueueMember	DataQueue[32000];
	ULONG			DataQueueIndex;
	int				AddMe;
protected:
	CTime			GoTime;
// Generated message map functions
protected:
	//{{AFX_MSG(CMCPforNTDoc)
	afx_msg void OnFileSaveold();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
