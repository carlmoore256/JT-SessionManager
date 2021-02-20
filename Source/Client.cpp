//
//  Client.cpp
//  JT-SessionManager - App
//
//  Created by Carl Moore on 2/17/21.
//

#include "Client.h"


// ===========================================

Client::Client(juce::String name, int port, int channels, bool autoConnectAudio, bool zeroUnderrun, bool autoManage, bool startOnCreate) : mName(name)
{
//	mName = name;
	mPort = port;
	mChannels = channels;
	mAutoConnectAudio = autoConnectAudio;
	mZeroUnderrun = zeroUnderrun;
	mAutoManage = autoManage;
	
	if (startOnCreate)
	{
		// start the server thread
		startServer();
	}
}

Client::~Client()
{
//	delete mClientServer;
}

bool Client::compareName(juce::String name)
{
//	juce::String.compareNatural()
	auto result = mName.compare(name);
	return bool(result);
}

bool Client::checkIfActive()
{
	return false;
}

std::map<std::string, float> Client::getClientStats()
{
	std::map<std::string, float> map;
	
	map["port"] = (float)mPort;
	map["channels"] = (float)mChannels;
//	map["quality"] = mQuality;
//	map["skew"] = (float)mSkew;
	
	return map;
}

void Client::startServer()
{
	auto threadName = mName + "_proc";
	mClientServer = new ClientServer(*this);
}

// ===========================================
//const juce::String& threadName

Client::ClientServer::ClientServer(Client& parentClient) : Thread("clientServer"), owner(parentClient)
{
//	std::string name, int port, int channels, bool autoConnectAudio, bool zeroUnderrun, bool autoManage
	std::string command =
		"jacktrip -c --clientname " + parentClient.mName.toStdString() +
		" -n " + std::to_string(parentClient.mChannels) +
		" -o " + std::to_string(parentClient.mPort - 4464) +
		" --iostat 1";
	
//	Will return false if not started, create a loop for this
	childProcess.start(command);
		
}


Client::ClientServer::~ClientServer()
{
	terminateServer();
}

void Client::ClientServer::run()
{
	while(!threadShouldExit())
	{
		DBG("RUNNING!");
		wait(500);
	}
}

bool Client::ClientServer::isRunning()
{
	return childProcess.isRunning();
}

void Client::ClientServer::runCommand(std::string command)
{
//	system
}

void Client::ClientServer::terminateServer()
{
	
}
