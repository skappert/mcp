// HardwareDefaults.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHardwareDefaults dialog

class CHardwareDefaults : public CDialog
{
// Construction
public:
	CHardwareDefaults(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHardwareDefaults)
	enum { IDD = IDD_HARDWARE };
	int		m_ferddichsubadd;
	int		m_flukegpib;
	int		m_gelbeboxslot;
	int		m_gelbebox2slot;
	int		m_gpibslot;
	int		m_hp1gpib;
	int		m_hp2gpib;
	int		m_tempgpib;
	int		m_fieldgpib;
	int		m_listslot;
	int		m_premaslot;
	int		m_presetslot;
	int		m_protonslot;
	int		m_protonsubadd;
	int		m_rs1gpib;
	int		m_rs2gpib;
	int		m_scalerslot;
	int		m_triggersubadd;
	int		m_hpvoltslot;
	int		m_hvvoltslot;
	UINT	m_flukedelay;
	UINT	m_gpibdelay;
	UINT	m_massdelay;
	UINT	m_premadelay;
	UINT	m_fvoltslot;
	CString m_dip_ht;
	CString m_dip_pc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHardwareDefaults)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHardwareDefaults)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
