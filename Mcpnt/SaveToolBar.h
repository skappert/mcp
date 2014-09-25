#pragma once


// CSaveToolBar

class CSaveToolBar : public CMFCToolBar
{
	DECLARE_DYNAMIC(CSaveToolBar)

public:
	CSaveToolBar();
	virtual ~CSaveToolBar();

	virtual void OnReset();

public:  
	CDialogBar m_wndDlgBar;

protected:
	DECLARE_MESSAGE_MAP()
};


