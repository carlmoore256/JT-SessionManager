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
	delete mClientServer;
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
	DBG("STARTING SERVER!");
//	std::string name, int port, int channels, bool autoConnectAudio, bool zeroUnderrun, bool autoManage
	int portOffset = parentClient.mPort;
	juce::String command =
		"jacktrip"
		" -n " + std::to_string(parentClient.mChannels) +
		" -s " +
		"--clientname " + parentClient.mName.toStdString() +
		" -o " + std::to_string(portOffset) +
		" --iostat 1";
	
	command = "jacktrip --help";
	
	std::cout << command + "\n";
//	Will return false if not started, create a loop for this
	childProcess.start(command, wantStdOut);
//	childProcess.waitForProcessToFinish(10000);
//	mThread.startThread();
	
	for (int i = 0; i < 100000; i++)
	{
		std::cout << childProcess.readAllProcessOutput();
		sleep(10);

	}
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
//		childProcess.readProcessOutput();
		wait(500);
	}
}

bool Client::ClientServer::isRunning()
{
	return childProcess.isRunning();
}

//void Client::ClientServer::runCommand(std::string command)
//{
////	system
//}

void Client::ClientServer::terminateServer()
{
	
}
