//
//  Client.cpp
//  JT-SessionManager - App
//
//  Created by Carl Moore on 2/17/21.
//

#include "Client.h"


// ===========================================

Client::Client(juce::String name, int port, int channels, bool autoConnectAudio, bool zeroUnderrun, bool autoManage, bool startOnCreate) : mName(name), mClientStats("CLIENT")
{
	mPort = port;
	mChannels = channels;
	mAutoConnectAudio = autoConnectAudio;
	mZeroUnderrun = zeroUnderrun;
	mAutoManage = autoManage;

//	if (startOnCreate)
	startServer();
	
	// initialize mClientInfo
	recordClientInfo();
}

Client::~Client()
{
    // watch out for this, figure out why jt can't be stopped
    while(!mClientServer->stopJackTrip())
        sleep(100);

    DBG("JackTrip Stopped!");

    mClientServer->stopThread(500);
//    delete mClientServer;

    DBG("CLIENT DESTRUCTOR END!");
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
	return &mClientStats;
}

void Client::startServer()
{
	auto threadName = mName + "_proc";
	mClientServer = new ClientServer(*this);
	// starts the thread, which then runs run(), which will start the server childProcess
	mClientServer->startThread();
}

// records stats to
void Client::recordClientInfo()
{
//	DBG("recording " + mName + " info");
	mClientStats.setAttribute("ID", 0); //gotta figure out this id thing, maybe add mID
	mClientStats.setAttribute("Name", mName);
	mClientStats.setAttribute("Port", String(mPort));
	mClientStats.setAttribute("Channels", String(mChannels));
	mClientStats.setAttribute("RouteAudio", String(std::to_string(mAutoConnectAudio))); // had to do this, problem with juce::String conversion from bool
	mClientStats.setAttribute("ZeroUnderrun", String(std::to_string(mZeroUnderrun)));
	mClientStats.setAttribute("AutoManage", String(std::to_string(mAutoManage)));
	mClientStats.setAttribute("Connection", String(std::to_string(checkIfActive()))); // make this the connection status of connected/not connected (add a method to return str)
	mClientStats.setAttribute("Skew", String(getSkew()));
	mClientStats.setAttribute("Quality", String(getQuality()));
	mClientStats.setAttribute("Select", "0"); // this will actually be handled by ClientList, REMOVE ME
	mClientStats.setAttribute("stdout", "\n" + getOutput());
}

// ===========================================
//const juce::String& threadName

Client::ClientServer::ClientServer(Client& parentClient) : Thread("clientServer"), owner(parentClient)
{
    mSkew = 0;
}


Client::ClientServer::~ClientServer()
{
//	stopServer();
}

bool Client::ClientServer::isProcessRunning()
{
	return mChildProcess.isRunning();
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
		if(!mJacktripRunning && mJacktripShouldRun)
		{
			mJacktripRunning = startServer();
		}

//        juce::String output = mChildProcess.readAllProcessOutput();
		
        /*
		while(mProcessRunning)
		{
			juce::String output = mChildProcess.readAllProcessOutput();
			wait(500);

//			int error = childProcess.getExitCode();
		}
         */
//
		wait(100);
//		DBG(owner.mName + "output: " + getProcessOutput());
//		add restart timeout stuff here
	}
	
//	stopServer();
}

float Client::ClientServer::calculateQuality()
{
	// build this up to calculate current quality
	// look into how threading lib works beforehand, we dont want to call while memory is updated
	return mQuality;
}

bool Client::ClientServer::stopJackTrip()
{
    return mChildProcess.kill();
}

String Client::ClientServer::getProcessOutput()
{
	MemoryOutputStream result;
	
	for(int i = 0; i < 100; i++)
	{
		char buffer [512];
		const int length = readProcessOutput(buffer, sizeof(buffer));
//		DBG("len of buff " + length);
	//	if(length <= 0)
	//		return "";
		result.write(buffer, length);
	}

	return result.toString();
}

juce::String Client::ClientServer::generateCommand()
{
	// overly complicated bs I gotta figure out
//	String nameNoSpaces = owner.mName.replaceCharacter(test.getCharPointer(), "");
	juce::String command =
	"/usr/local/bin/jacktrip"
	" -n " + juce::String(owner.mChannels) +
	" -s " +
	"--clientname " + owner.mName +
	" -o " + juce::String(owner.mPort) +
	" --iostat 1";
	
//	command = String("ls -a");
	return command;
}


bool Client::ClientServer::startServer()
{
	DBG("attempting to start server");
	
	if(mChildProcess.start (generateCommand(), wantStdOut | wantStdErr))
	{
		DBG("server opened");
		mProcessRunning = true;
		return true;
	}
	return false;
}

void Client::ClientServer::stopServer()
{
	DBG("stopping clientServer childProces (JT)");
	mChildProcess.kill();
}
