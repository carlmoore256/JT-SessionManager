//
//  Client.hpp
//  JT-SessionManager - App
//
//  Created by Carl Moore on 2/17/21.
//
#pragma once

//#ifndef Client_hpp
//#define Client_hpp

// this is only for sleep(). probably a JUCE equivalent we should be using
#include <unistd.h>

#include <stdio.h>
#include <JuceHeader.h>
//#include <thread>
//#include "boost/process.hpp"

//#endif /* Client_hpp */

// ===========================================


class Client : juce::Component
{
public:
	
	// clientInfo is its chunk on the ledger that client updates with stats
	Client(juce::String name, int port, int channels, bool autoConnectAudio, bool zeroUnderrun, bool autoManage, bool startOnCreate);
	
	~Client();
	
	juce::String getName() { return mName; };
	
	int getPort() { return mPort; };
	
	int getNumChannels() { return mChannels; };
	
	void setPort();
	
	bool compareName(juce::String name);
	
	bool checkIfActive();
	
	int getSkew()
	{
		if(mClientServer != nullptr)
			return mClientServer->mSkew;
        
		return 0;
	}
	
	float getQuality()
	{
		if(mClientServer != nullptr)
			return mClientServer->calculateQuality();
		return 0.;
	};
	
	String getOutput()
	{
		if(mClientServer != nullptr)
			return mClientServer->getProcessOutput();
		return "";
	};
	    
	//	returns xmlelement containing my info, for saving and displaying purposes. Consider making this a variable that is updated instead, which could improve performance by avoiding malloc
	juce::XmlElement* getClientInfo();
	
	// function to allocate new clientInfo to the heap
	void recordClientInfo();
	
private:
	const juce::String mName;
	
	// change this to a pointer reference to the session childElement, so whenever this is updated, we're only updating the session's version of mAllClientInfo
	XmlElement mClientStats;

	int mPort;
	int mChannels;
	
	bool mAutoConnectAudio;
	bool mZeroUnderrun;
	bool mAutoManage;
	
	void startServer();
	
	// ==============================================================
	
	// manages the running jacktrip thread
	class ClientServer : public juce::ChildProcess,
						 public juce::Thread
//	private juce::Listener
	{
	public:
		ClientServer(Client& parentClient);
		
		~ClientServer();
		
		bool isProcessRunning();
		
		void run() override;
		
		float calculateQuality();
        
        bool stopJackTrip();
		
		String getProcessOutput();
		
		//	---metrics---
		int mSkew;
		
	private:
		Client& owner;
		
		//	---metrics---
		float mQuality;
		
		ChildProcess mChildProcess;
		
		bool mProcessRunning;
		bool mAllowRestart = true;
		
		// is the jacktrip process actually running
		bool mJacktripRunning = false;
		// should it run (useful for auto restart)
		bool mJacktripShouldRun = true;
		
		juce::String generateCommand();
		
		bool startServer();
		void stopServer();
		void filterOutput();
	};
	
	ClientServer* mClientServer;
};
