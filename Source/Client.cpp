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
	DBG("CLIENT DESTRUCTOR CALLED!");
	mClientServer->stopThread(500);
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
	
	mClientServer->startThread();
}

// ===========================================
//const juce::String& threadName

Client::ClientServer::ClientServer(Client& parentClient) : Thread("clientServer"), owner(parentClient)
{
	
	int portOffset = parentClient.mPort;
	juce::String command =
		"/usr/local/bin/jacktrip"
		" -n " + juce::String(parentClient.mChannels) +
		" -s " +
		"--clientname " + parentClient.mName +
		" -o " + juce::String(portOffset) +
		" --iostat 1";

	
	command = "jacktrip --help";
	
	std::cout << command + "\n";
//	Will return false if not started, create a loop for this
	childProcess.start(command, wantStdOut);
//	childProcess.waitForProcessToFinish(10000);
//	mThread.startThread();
	
//	for (int i = 0; i < 100000; i++)
//	{
//		std::cout << childProcess.readAllProcessOutput();
//		sleep(10);
//
//	}

}


Client::ClientServer::~ClientServer()
{
//	stopServer();
}

void Client::ClientServer::run()
{
	
	if(threadShouldExit())
	{
		DBG("thread SHOULD exit");
	} else {
		DBG("thread SHOULD NOT exit");
	}
	
	
	while(!threadShouldExit())
	{
		DBG("THREAD RUNNING! \n");
		
		if(mAllowRestart)
			startServer();

		while(mProcessRunning)
		{
			juce::String output = childProcess.readAllProcessOutput();
			wait(500);

//			int error = childProcess.getExitCode();
		}
//
		wait(300);
	}
	
	stopServer();
}

bool Client::ClientServer::isRunning()
{
	return childProcess.isRunning();
}


void Client::ClientServer::startServer()
{
	DBG("attempting to start server");
	
	if(childProcess.start (mCommand, wantStdOut | wantStdErr))
	{
		DBG("server opened");
		mProcessRunning = true;
	}
}

void Client::ClientServer::stopServer()
{
	childProcess.kill();
}
