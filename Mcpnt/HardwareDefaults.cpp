// HardwareDefaults.cpp : implementation file
//

#include "stdafx.h"
#include "MCP for NT.h"
#include "HardwareDefaults.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHardwareDefaults dialog


CHardwareDefaults::CHardwareDefaults(CWnd* pParent /*=NULL*/)
	: CDialog(CHardwareDefaults::IDD, pParent)
	, m_dip_ht_value(_T(""))
	, m_dip_pc_value(_T(""))
{
	//{{AFX_DATA_INIT(CHardwareDefaults)
	m_ferddichsubadd = 0;
	m_flukegpib = 1;
	m_gelbeboxslot = 1;
	m_gelbebox2slot = 1;
	m_gpibslot = 1;
	m_hp1gpib = 1;
	m_hp2gpib = 1;
	m_tempgpib = 1;
	m_fieldgpib = 1;
	m_listslot = 1;
	m_premaslot = 1;
	m_presetslot = 1;
	m_protonslot = 1;
	m_protonsubadd = 0;
	m_rs1gpib = 1;
	m_rs2gpib = 1;
	m_scalerslot = 1;
	m_triggersubadd = 0;
	m_hpvoltslot = 1;
	m_hvvoltslot = 1;
	m_flukedelay = 0;
	m_gpibdelay = 0;
	m_massdelay = 0;
	m_premadelay = 0;
	m_fvoltslot = 1;
	m_dip_ht = _T("");
	m_dip_pc = _T("");
	//}}AFX_DATA_INIT
}


void CHardwareDefaults::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHardwareDefaults)
	DDX_Text(pDX, IDC_FERDDICHSUBADD, m_ferddichsubadd);
	DDV_MinMaxInt(pDX, m_ferddichsubadd, 0, 15);
	DDX_Text(pDX, IDC_FLUKEGPIB, m_flukegpib);
	DDV_MinMaxInt(pDX, m_flukegpib, 1, 31);
	DDX_Text(pDX, IDC_GELBEBOXSLOT, m_gelbeboxslot);
	DDV_MinMaxInt(pDX, m_gelbeboxslot, 1, 24);
	DDX_Text(pDX, IDC_GPIBSLOT, m_gpibslot);
	DDV_MinMaxInt(pDX, m_gpibslot, 1, 24);
	DDX_Text(pDX, IDC_HP1GPIB, m_hp1gpib);
	DDV_MinMaxInt(pDX, m_hp1gpib, 1, 31);
	DDX_Text(pDX, IDC_HP2GPIB, m_hp2gpib);
	DDV_MinMaxInt(pDX, m_hp2gpib, 1, 31);
	DDX_Text(pDX, IDC_LISTSLOT, m_listslot);
	DDV_MinMaxInt(pDX, m_listslot, 1, 24);
	DDX_Text(pDX, IDC_PREMASLOT, m_premaslot);
	DDV_MinMaxInt(pDX, m_premaslot, 1, 24);
	DDX_Text(pDX, IDC_PRESETSLOT, m_presetslot);
	DDV_MinMaxInt(pDX, m_presetslot, 1, 24);
	DDX_Text(pDX, IDC_PROTONSLOT, m_protonslot);
	DDV_MinMaxInt(pDX, m_protonslot, 1, 24);
	DDX_Text(pDX, IDC_PROTONSUBADD, m_protonsubadd);
	DDV_MinMaxInt(pDX, m_protonsubadd, 0, 15);
	DDX_Text(pDX, IDC_RS1GPIB, m_rs1gpib);
	DDV_MinMaxInt(pDX, m_rs1gpib, 1, 31);
	DDX_Text(pDX, IDC_RS2GPIB, m_rs2gpib);
	DDV_MinMaxInt(pDX, m_rs2gpib, 1, 31);
	DDX_Text(pDX, IDC_SCALERSLOT, m_scalerslot);
	DDV_MinMaxInt(pDX, m_scalerslot, 1, 24);
	DDX_Text(pDX, IDC_TRIGGERSUBADD, m_triggersubadd);
	DDV_MinMaxInt(pDX, m_triggersubadd, 0, 15);
	DDX_Text(pDX, IDC_HPVOLTSLOT, m_hpvoltslot);
	DDV_MinMaxInt(pDX, m_hpvoltslot, 1, 24);
	DDX_Text(pDX, IDC_HVVOLTSLOT, m_hvvoltslot);
	DDV_MinMaxInt(pDX, m_hvvoltslot, 1, 24);
	DDX_Text(pDX, IDC_FLUKEDELAY, m_flukedelay);
	DDV_MinMaxUInt(pDX, m_flukedelay, 0, 9999);
	DDX_Text(pDX, IDC_GPIBDELAY, m_gpibdelay);
	DDV_MinMaxUInt(pDX, m_gpibdelay, 0, 9999);
	DDX_Text(pDX, IDC_MASSDELAY, m_massdelay);
	DDV_MinMaxUInt(pDX, m_massdelay, 0, 9999);
	DDX_Text(pDX, IDC_PREMADELAY, m_premadelay);
	DDV_MinMaxUInt(pDX, m_premadelay, 0, 9999);
	DDX_Text(pDX, IDC_FVOLTSLOT, m_fvoltslot);
	DDV_MinMaxUInt(pDX, m_fvoltslot, 1, 24);
	DDX_Text(pDX, IDC_TEMPGPIB, m_tempgpib);
	DDX_Text(pDX, IDC_FIELDGPIB, m_fieldgpib);
	DDX_Text(pDX, IDC_GELBEBOX2SLOT, m_gelbebox2slot);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_DIP_HT, m_dip_ht);
	DDV_MaxChars(pDX, m_dip_ht, 200);
	DDX_Text(pDX, IDC_DIP_PC, m_dip_pc);
	DDV_MaxChars(pDX, m_dip_pc, 200);
	DDX_Text(pDX, IDC_DIP_HT_VALUE, m_dip_ht_value);
	DDX_Text(pDX, IDC_DIP_PC_VALUE, m_dip_pc_value);
}


BEGIN_MESSAGE_MAP(CHardwareDefaults, CDialog)
	//{{AFX_MSG_MAP(CHardwareDefaults)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHardwareDefaults message handlers
