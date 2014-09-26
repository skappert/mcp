// NetworkWriter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNetworkWriter dialog

class CNetworkWriter : public CDialog
{
// Construction
public:
	CNetworkWriter(CWnd* pParent = NULL);   // standard constructor

	void HandleNetMessage( CString topic, double value );

// Dialog Data
	//{{AFX_DATA(CNetworkWriter)
	enum { IDD = IDD_NETWRITE };
	CString	m_name;
	CString	m_value;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkWriter)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNetworkWriter)
	afx_msg void OnWritenet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	// hold reference to subscription objects
	DipSubscription **sub;

	// DIP object
	DipFactory *dip;

	/**
	* handler for connect/disconnect/data reception events
	* Nested class
	* */
	class GeneralDataListener:public DipSubscriptionListener
	{
	private:
		// allow us to access subscription objects
		CNetworkWriter* client;

	public:
		GeneralDataListener(CNetworkWriter *c):client(c){};

		/**
		* handle changes to subscribed to publications
		* Simply prints the contents of the received data.
		* @param subscription - the subsciption to the publications thats changed.
		* @param message - object containing publication data
		* */
		void handleMessage(DipSubscription *subscription, DipData &message)
		{
			client->HandleNetMessage( subscription->getTopicName(), message.extractDouble("value") );
		}


		/**
		* called when a publication subscribed to is available.
		* @param arg0 - the subsctiption who's publication is available.
		* */
		void connected(DipSubscription *arg0)
		{
			TRACE1("\nPublication source %s available\n", arg0->getTopicName());
		}


		/**
		* called when a publication subscribed to is unavailable.
		* @param arg0 - the subsctiption who's publication is unavailable.
		* @param arg1 - string providing more information about why the publication is unavailable.
		* */
		void disconnected(DipSubscription *arg0, char *arg1)
		{
			TRACE1("\nPublication source %s unavailable\n", arg0->getTopicName());
		}

		void handleException(DipSubscription* subscription, DipException& ex)
		{
			TRACE2("Subs %s has error %s\n", subscription->getTopicName(), ex.what());
		}

	};

	//A handle on the DIP Data recipient.
	GeneralDataListener *handler;
};
