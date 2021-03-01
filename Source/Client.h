//
//  Client.hpp
//  JT-SessionManager - App
//
//  Created by Carl Moore on 2/17/21.
//

#ifndef CLIENT_H
#define CLIENT_H

#include <JuceHeader.h>
//#include <stdio.h>
//
//#include "ClientList.h"
//#include "InfoPanel.hpp"
//#include "Section.hpp"
//#include "Session.hpp"
#include "SignalRouter.h"

// ===========================================


class Client : juce::Component
{
public:
	
	Client(juce::String name, int port, int channels, bool autoConnectAudio, bool zeroUnderrun, bool autoManage, bool startOnCreate=true);
	
	~Client();
	
	juce::String getName() { return mName; };
	
	int getPort() { return mPort; };
	
	void setPort();
	
	bool compareName(juce::String name);
	
	bool checkIfActive();
	
	std::map<std::string, float> getClientStats();

    SignalRouter mSignalRouter;
    juce::Array<juce::String> mSignalDestinations;
    
    void addSignalDest(juce::String d);
    void removeSignalDest(juce::String d);
	
private:
	const juce::String mName;
//	const std::string threadName = "test";


	int mPort;
	int mChannels;
	
	bool mAutoConnectAudio;
	bool mZeroUnderrun;
	bool mAutoManage;
	
	void startServer();
    void stopServer();
	
	// ==============================================================
	
	// manages the running jacktrip thread
	class ClientServer : public juce::ChildProcess,
						 public juce::Thread
//	private juce::Listener
	{
	public:
		ClientServer(Client& parentClient);
		
		~ClientServer();
		
		bool isRunning();
		
		void run() override;
        
        ChildProcess* getChildProcessPtr();
		
	private:
		Client& owner;
//		juce::Thread& serverThread;
		
		ChildProcess childProcess;
	//	std::thread* mServerThread;
		
		bool mProcessRunning;
		bool mAllowRestart = true;
		
		juce::String generateCommand();
		
		//	---metrics---
		float mQuality;
		int mSkew;
		
		void startServer();
		void stopServer();
		void filterOutput();
	};
	
	ClientServer* mClientServer;
};

#endif /* CLIENT_H */
