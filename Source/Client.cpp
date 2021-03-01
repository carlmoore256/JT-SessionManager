//
//  Client.cpp
//  JT-SessionManager - App
//
//  Created by Carl Moore on 2/17/21.
//

#include "Client.h"


// ===========================================

Client::Client(juce::String name, int port, int channels, bool autoConnectAudio, bool zeroUnderrun, bool autoManage, bool startOnCreate) : mName(name), mClientInfo("CHANGEME")
{
	mPort = port;
	mChannels = channels;
	mAutoConnectAudio = autoConnectAudio;
	mZeroUnderrun = zeroUnderrun;
	mAutoManage = autoManage;

	// initialize mClientInfo
	recordClientInfo();
	
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
	int result = mName.compare(name);
	if (result==0) //compare name returns 0 if identical
		return true;
	return false;
}

bool Client::checkIfActive()
{
	if(mClientServer != nullptr)
		return 	mClientServer->isProcessRunning();
	return false;
}

juce::XmlElement* Client::getClientInfo()
{
	recordClientInfo(); // update the info within the xmlElement
	return &mClientInfo;
}

void Client::startServer()
{
	auto threadName = mName + "_proc";
	mClientServer = new ClientServer(*this);
	// starts the thread, which then runs run(), which will start the server childProcess
	mClientServer->startThread();
}

void Client::recordClientInfo()
{
	mClientInfo.setAttribute("ID", 0); //gotta figure out this id thing, maybe add mID
	mClientInfo.setAttribute("Name", mName);
	mClientInfo.setAttribute("Port", String(mPort));
	mClientInfo.setAttribute("Channels", String(mChannels));
	mClientInfo.setAttribute("RouteAudio", String(std::to_string(mAutoConnectAudio))); // had to do this, problem with juce::String conversion from bool
	mClientInfo.setAttribute("ZeroUnderrun", String(std::to_string(mZeroUnderrun)));
	mClientInfo.setAttribute("AutoManage", String(std::to_string(mAutoManage)));
	mClientInfo.setAttribute("Connection", String(std::to_string(checkIfActive()))); // make this the connection status of connected/not connected (add a method to return str)
	mClientInfo.setAttribute("Skew", String(getSkew()));
	mClientInfo.setAttribute("Quality", String(getQuality()));
	mClientInfo.setAttribute("Select", "0"); // this will actually be handled by ClientList, REMOVE ME
}

// ===========================================
//const juce::String& threadName

Client::ClientServer::ClientServer(Client& parentClient) : Thread("clientServer"), owner(parentClient)
{
	
}


Client::ClientServer::~ClientServer()
{
//	stopServer();
}

bool Client::ClientServer::isProcessRunning()
{
	return childProcess.isRunning();
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
			bool success = startServer();

		while(mProcessRunning)
		{
			juce::String output = childProcess.readAllProcessOutput();
			wait(500);

//			int error = childProcess.getExitCode();
		}
//
		wait(300);
		
//		add restart timeout stuff here
	}
	
	stopServer();
}

float Client::ClientServer::calculateQuality()
{
	// build this up to calculate current quality
	// look into how threading lib works beforehand, we dont want to call while memory is updated
	return mQuality;
}

juce::String Client::ClientServer::generateCommand()
{
	juce::String command =
	"/usr/local/bin/jacktrip"
	" -n " + juce::String(owner.mChannels) +
	" -s " +
	"--clientname " + owner.mName +
	" -o " + juce::String(owner.mPort) +
	" --iostat 1";
	
	return command;
}


bool Client::ClientServer::startServer()
{
	DBG("attempting to start server");
	
	if(childProcess.start (generateCommand(), wantStdOut | wantStdErr))
	{
		DBG("server opened");
		mProcessRunning = true;
		return true;
	}
	return false;
}

void Client::ClientServer::stopServer()
{
	childProcess.kill();
}
