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
	
	//	returns xmlelement containing my info, for saving and displaying purposes. Consider making this a variable that is updated instead, which could improve performance by avoiding malloc
	juce::XmlElement getClientInfo();
	
//	std::map<std::string, float> getClientStats();
	
private:
	const juce::String mName;
//	const std::string threadName = "test";


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
		
		bool isRunning();
		
		void run() override;
		
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
