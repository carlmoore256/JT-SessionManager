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
    DBG("Client: constructor called");
    
	mPort = port;
	mChannels = channels;
	mAutoConnectAudio = autoConnectAudio;
	mZeroUnderrun = zeroUnderrun;
	mAutoManage = autoManage;
    mSignalRouter.setSource(name);
    	
	if (startOnCreate)
	{
		// start the server thread
		startServer();
	}
}

Client::~Client()
{
	DBG("Client: destructor called");
    
    // ClientServer's destructor should be called before Client's, so any running threads should have been stopped by here. therefore, it's safe to delete the ClientServer pointer
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
	return mClientServer->isRunning();
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
	
    // create a new instance
    mClientServer = new ClientServer(*this);
	
	mClientServer->startThread();
}

void Client::stopServer()
{
    juce::ChildProcess* thisProcessPtr;
    
    // get the pointer to the private child process. if it's running, getChildProcessPtr() will give us the pointer so we can kill the process. if not running, we get nullptr
    thisProcessPtr = mClientServer->getChildProcessPtr();
    
    if(thisProcessPtr)
    {
        DBG("Client: killing active process");
        thisProcessPtr->kill();
    }
    else
        DBG("Client: invalid pointer");
}

// ===========================================
//const juce::String& threadName

Client::ClientServer::ClientServer(Client& parentClient) : Thread("clientServer"), owner(parentClient)
{
    DBG("Client::ClientServer: constructor called");
}

Client::ClientServer::~ClientServer()
{
    DBG("Client::ClientServer: destructor called");

    stopServer();
}

juce::ChildProcess* Client::ClientServer::getChildProcessPtr()
{
    if(childProcess.isRunning())
        return &childProcess;
    else
        return nullptr;
}

bool Client::ClientServer::isRunning()
{
	return childProcess.isRunning();
}

void Client::ClientServer::run()
{
	
	if(threadShouldExit())
	{
//		DBG("Client::ClientServer: thread SHOULD exit");
	}
    else
    {
//		DBG("Client::ClientServer: thread SHOULD NOT exit");
	}
	
	
	while(!threadShouldExit())
	{
//		DBG("Client::ClientServer: thread running");
		
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
		
//		add restart timeout stuff here
	}
	
	stopServer();
}

juce::String Client::ClientServer::generateCommand()
{
    juce::String binaryPath = BIN_PATH;
    juce::String command;
    
    command =
    binaryPath +
	"jacktrip"
	" -n " + juce::String(owner.mChannels) +
	" -s " +
	"--clientname " + owner.mName +
	" -o " + juce::String(owner.mPort) +
	" --iostat 1";
	
	return command;
}


void Client::ClientServer::startServer()
{
	DBG("Client::ClientServer: attempting to start server");
	
	if(childProcess.start (generateCommand(), wantStdOut | wantStdErr))
	{
		DBG("Client::ClientServer: server started");
		mProcessRunning = true;
	}
}

void Client::ClientServer::stopServer()
{
	childProcess.kill();
}

void Client::addSignalDest(juce::String d)
{
    mSignalDestinations.add(d);
}

void Client::removeSignalDest(juce::String d)
{
    int idx;
    
    idx = mSignalDestinations.indexOf(d);
    mSignalDestinations.remove(idx);
}
