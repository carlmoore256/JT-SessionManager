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
	
	void setPort();
	
	bool compareName(juce::String name);
	
	bool checkIfActive();
	
	std::map<std::string, float> getClientStats();
	
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
						 private juce::Thread
	{
	public:
		ClientServer(Client& parentClient);
		
		~ClientServer();
		
		bool isRunning();
		
		virtual void run() override;
		
	private:
		Client& owner;
//		juce::Thread& serverThread;
		
		ChildProcess childProcess;
	//	std::thread* mServerThread;
		
		
		
		//	---metrics---
		float mQuality;
		int mSkew;
		
		void filterOutput();
		void runCommand(std::string command);
	//	void startServer();
		void terminateServer();
	};
	
	ClientServer* mClientServer;
};
