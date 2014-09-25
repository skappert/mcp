
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "Dip.h"
#include "DipSubscription.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>


using namespace std;

//How long the test will run in ms. Default is 1 minute.
unsigned int lifeTime;


/**
* Mathias Dutour 2008.
* Simple demo client that subscribes to data coming from servers (!)
* This client will have as a name "Client<ClientNumericID>"
* and will subscribe to "dip/test/API/Server<from 0 to noServers>_<from 0 to noPubs>"
* Feel free to modify for your own purpose.
* */

class Client {

private:
	// hold reference to subscription objects
	DipSubscription **sub;

	// DIP object
	DipFactory *dip;

	//The default name of the publication to subscribe to: 
	// dip/test/defaultPubRoot0, dip/test/defaultPubRoot1, etc.
	string pubNameRoot;

	//The default number of publications to subscribe to.
	unsigned int noPubs;

	//Used to identify the Client.
	string ClientNumericID;

	//Used to know how many Servers to connect to
	unsigned int noServers;

/**
* handler for connect/disconnect/data reception events
* Nested class
* */
class GeneralDataListener:public DipSubscriptionListener{
private:

	// allow us to access subscription objects
	Client * client;

	bool flip;

public:
	GeneralDataListener(Client *c):client(c),flip(true){};

	/**
	* handle changes to subscribed to publications
	* Simply prints the contents of the received data.
	* @param subscription - the subsciption to the publications thats changed.
	* @param message - object containing publication data
	* */
	void handleMessage(DipSubscription *subscription, DipData &message){

		if (flip==true ? cout<<"/": cout<<"\\") flip = !flip;
		//cout<<"Received data from "<<subscription->getTopicName()<<endl;
		/*
		try{
		cout<<"Received data from "<<subscription->getTopicName()<<endl;

		int val = 0;
		int & noFields = val;
		const char ** tags = message.getTags(noFields);
		for (int i =0; i<noFields; i++) {
		cout<<"Tag "<<*tags<<" is of type "<<message.getValueType(*tags)<<endl;
		tags++;
		}
		cout<<"====================================================="<<endl;
		} catch(DipException e){
		cout<<"Unexpected exception occured: "<<e.what()<<endl;
		}
		*/
		}


	/**
	* called when a publication subscribed to is available.
	* @param arg0 - the subsctiption who's publication is available.
	* */
	void connected(DipSubscription *arg0) {
		//printf("\nPublication source %s available\n", arg0->getTopicName());
		}


	/**
	* called when a publication subscribed to is unavailable.
	* @param arg0 - the subsctiption who's publication is unavailable.
	* @param arg1 - string providing more information about why the publication is unavailable.
	* */
	void disconnected(DipSubscription *arg0, char *arg1) {
		//printf("\nPublication source %s unavailable\n", arg0->getTopicName());
		}

	void handleException(DipSubscription* subscription, DipException& ex){
		//printf("Subs %s has error %s\n", subscription->getTopicName(), ex.what());
		}

	};

	//A handle on the DIP Data recipient.
	GeneralDataListener *handler;

public:

	/**
	* set up the subscriptions to dip/test/pub1 dip/test/pub2
	* */
	Client(const int argc, const char ** argv):
	  pubNameRoot("dip/test/API/Server"),
		  noPubs(5), //By default, Client subscribes to 5 publications.
		  ClientNumericID("0"), //To identify the Client instance. Must be unique.
		  noServers(1) //To identify to how many Servers to subscribe to (using a common naming convention on the DIP publication names with the sample Server.)
		  {
		  lifeTime = 60000; //Client will die after 1 minute.

		  //Check whether a specific DNS shall be used.
		  if (argc == 7) {
			  cout<<"Using custom settings:"<<endl;
			  noPubs = atoi(argv[2]);
			  pubNameRoot = argv[3];
			  ClientNumericID = argv[4];
			  noServers = atoi(argv[5]);
			  lifeTime = atoi(argv[6]);
			  } else {
				  cout<<"You could pass on the command line :"<<endl;
				  cout<<"<DNS> <noPubs> <pubRootName> <ClientNumericID> <noServers> <lifetime(ms)>"<<endl;
			  }
		  cout<<"The number of publications to subscribe to is:"<<noPubs<<endl;
		  cout<<"The publications name root to use          is:"<<pubNameRoot<<endl;
		  cout<<"The Client unique numeric ID               is:"<<ClientNumericID<<endl;
		  cout<<"The number of Servers to connect to        is:"<<noServers<<endl;
		  cout<<"The lifetime of thisClient                 is:"<<lifeTime<<endl;

		  cout<<"======================================================================="<<endl;

		  string clientName = "Client"; 
		  clientName+=ClientNumericID;

		  //Asking the DIP Factory object.
		  dip = Dip::create(clientName.c_str());
		  //Creating an array of DipSubscriptions.
		  sub = new DipSubscription*[noPubs*noServers];

		  //The following call forces the selection for the DNS of your choice rather than the default (hardcoded in DIP),
		  // or the one you have specified via the DIPNS environment variable.
		  if (argc >= 2) {
			  cout<<"The DIP DNS running on :"<<argv[1]<<endl;
			  dip->setDNSNode(argv[1]);
			  }
		  else {
			  cout<<"The DIP DNS running the default DNS."<<endl;
			  }

		  //Instanciating the handler for DIP data and messages.
		  handler = new GeneralDataListener(this);



		  //Creating subscriptions to "dip/test/API/Server<from 0 to noServers>_<from 0 to noPubs>"
		  unsigned int k = 0; //(index to store DIP Subscriptions).
		  for (unsigned int i=0; i < noServers; i++){
			  for (unsigned int j=0; j < noPubs; j++){
				  //Creating the names of the DIP publications names to subscribe to.
				  ostringstream oss;
				  oss << pubNameRoot << i;
				  oss << "_" << j;
				  //Creating the DIP subscriptions themselves.
				  sub[k] = dip->createDipSubscription(oss.str().c_str(), handler);
				  //cout<<clientName<<" has just created a subscription to :"<<oss.str()<<endl;
				  k++;
				  } //noPubs
			  }//noServers
		  }

	  ~Client(){
	  //Cleaning subscriptions 
	  for (unsigned int i=0; i < noPubs; i++){
		  cout<<"Client is cleaning pending Subscriptions."<<endl;
		  dip->destroyDipSubscription(sub[i]);
		  }
	  delete handler;
	  delete dip;
	  cout<<"Client's done."<<endl;

	}

		//friend class GeneralDataListener; //Uncomment if you need to allow access.
	};


	/**
	* Start the client
	* */
	int main(const int argc, const char ** argv){

		Client * theClient = new Client(argc,argv);

		DipBool myBool		 = true;
		DipByte myByte		 = 2;
		DipShort myShort	 = 2;
		DipInt myInt		 = 2;
		DipLong myLong		 = 2l;
		DipFloat myFloat	 = 2.0f;
		DipDouble myDouble	 = 2.0;
		cout<<"Size of a DipBool   = "<<sizeof(myBool)<<endl;
		cout<<"Size of a DipByte   = "<<sizeof(myByte)<<endl;
		cout<<"Size of a DipShort  = "<<sizeof(myShort)<<endl;
		cout<<"Size of a DipInt    = "<<sizeof(myInt)<<endl;
		cout<<"Size of a DipLong   = "<<sizeof(myLong)<<endl;
		cout<<"Size of a DipFloat  = "<<sizeof(myFloat)<<endl;
		cout<<"Size of a DipDouble = "<<sizeof(myDouble)<<endl;
/*
typedef bool DipBool;
typedef unsigned char DipByte;	//On 8bits
typedef short DipShort;			//On 32bits
typedef int DipInt;				//On 32bits
typedef superlong DipLong;		//On 64bits
typedef float DipFloat;			//On 32bits
typedef double DipDouble;		//On 32bits
*/
		//Main loop.
		//	while(true){
#ifdef WIN32
		Sleep(lifeTime);
#else
		sleep(lifeTime/1000);
#endif
		//		}
		cout<<"Client's lifetime has expired, leaving... "<<endl;

		delete theClient;
		return(0);
		}

