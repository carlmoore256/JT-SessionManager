//
//  Client.hpp
//  JT-SessionManager - App
//
//  Created by Carl Moore on 2/17/21.
//
#pragma once

//#ifndef Client_hpp
//#define Client_hpp

#include <stdio.h>
#include <JuceHeader.h>
//#include <thread>
//#include "boost/process.hpp"

//#endif /* Client_hpp */

// ===========================================


class Client : juce::Component
{
public:
	
	Client(juce::String name, int port, int channels, bool autoConnectAudio, bool zeroUnderrun, bool autoManage, bool startOnCreate=true);
	
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
	
	//	returns xmlelement containing my info, for saving and displaying purposes. Consider making this a variable that is updated instead, which could improve performance by avoiding malloc
	juce::XmlElement* getClientInfo();
	
private:
	const juce::String mName;
	
	// change this to a pointer reference to the session childElement, so whenever this is updated, we're only updating the session's version of mAllClientInfo
	XmlElement mClientInfo;

	int mPort;
	int mChannels;
	
	bool mAutoConnectAudio;
	bool mZeroUnderrun;
	bool mAutoManage;
	
	void startServer();
	
	// function to allocate new clientInfo to the heap
	void recordClientInfo();
	
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
		
		//	---metrics---
		int mSkew;
		
		String test = "TEST!!!";

		
	private:
		Client& owner;
		
		//	---metrics---
		float mQuality;
		
		ChildProcess mChildProcess;
		
		bool mProcessRunning;
		bool mAllowRestart = true;
		
		juce::String generateCommand();
		
		bool startServer();
		void stopServer();
		void filterOutput();
	};
	
	ClientServer* mClientServer;
};
