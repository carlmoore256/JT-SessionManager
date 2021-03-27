//
//  Client.cpp
//  JT-SessionManager - App
//
//  Created by Carl Moore on 2/17/21.
//

#include "Client.h"
#include <stdio.h>

// ===========================================

Client::Client(juce::String name, int port, int channels, bool autoConnectAudio, bool zeroUnderrun, bool autoManage, bool startOnCreate) : mName(name), mClientStats("CLIENT")
{
	mPort = port;
	mChannels = channels;
	mAutoConnectAudio = autoConnectAudio;
	mZeroUnderrun = zeroUnderrun;
	mAutoManage = autoManage;

	
	cp = new ChildProcess();
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
	
	delete cp;
//    delete mClientServer;
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
	mClientServer = new ClientServer(*this, cp);
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

ClientServer::ClientServer(Client& parentClient, ChildProcess* cp) : Thread("clientServer"), owner(parentClient), mChildProcess(cp)
{
    mSkew = 0;
//	startJTprocess();
//	String test = popenTest();
//	DBG(owner.mName + " test popen op: " + test);
}


ClientServer::~ClientServer()
{
//	stopServer();
}

bool ClientServer::isProcessRunning()
{
	return mChildProcess->isRunning();
}

void ClientServer::run()
{
	while(!threadShouldExit())
	{
		if(!mJacktripRunning && mJacktripShouldRun)
		{
			mJacktripRunning = startJTprocess();
//			if (mJacktripRunning)
//			{
//				DBG("M JACK TRIP IS RUNNING!");
////				DBG(getProcessOutput());
//			}
			
//			if (mChildProcess.isRunning())
//			{
//				DBG(owner.mName + " is running? ");
//			}
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
//		if(mChildProcess.isRunning())
		DBG(owner.mName + " thead running!");
		DBG(owner.mName + " output: " + getProcessOutput());
		
//		add restart timeout stuff here
		
		wait(100);
	}
//	stopServer();
	DBG("\n THREAD EXITING!!! " + owner.mName + "\n");
}

float ClientServer::calculateQuality()
{
	// build this up to calculate current quality
	// look into how threading lib works beforehand, we dont want to call while memory is updated
	return mQuality;
}

bool ClientServer::stopJackTrip()
{
    return mChildProcess->kill();
}

String ClientServer::getProcessOutput()
{
	MemoryOutputStream result;
	
//	for(int i=0;i<100;i++)
//	{
	char buffer [512];
	const int length = mChildProcess->readProcessOutputNonblocking(&buffer, sizeof(buffer));

	result.write(&buffer, length);
//	}

	return result.toString();
}

juce::String ClientServer::generateCommand()
{
	String nameNoSpaces = owner.mName.removeCharacters(" ");
	
	juce::String command =
	"/usr/local/bin/jacktrip"
	" -n " + juce::String(owner.mChannels) +
	" -s " +
	"--clientname " + nameNoSpaces +
	" -o " + juce::String(owner.mPort) +
	" --iostat 1";
//	DBG(command);
	return command;
}


bool ClientServer::startJTprocess()
{
	if(mChildProcess->start (generateCommand(), wantStdOut | wantStdErr))
	{
		DBG("\n " + owner.mName + " " + generateCommand() + "\n");
		
		String test = getProcessOutput();

		DBG("\n" + owner.mName + " STARTED SERVER OUTPUT: " + test + " \n");

		DBG("server opened");
		mProcessRunning = true;
		return true;
	}
	else
	{
		DBG("failed to start server for " + owner.mName);
		return false;
	}

}

void ClientServer::stopJTprocess()
{
	DBG("stopping clientServer childProces (JT)");
	mChildProcess->kill();
}

String ClientServer::popenTest()
{
	auto command = generateCommand().toUTF8().getAddress();
	DBG(String(command));
	
	FILE* pipe = popen(command, "r");
	
	if (!pipe)
		return "nothing in pipe";
	
	char buffer[128];
	String result = "";
	
	fgets(buffer, 128, pipe);
	
	result += String(buffer);
	
//	while(!feof(pipe))
//	{
		if(fgets(buffer, 128, pipe) != NULL)
			result += String(buffer);
//	}
	pclose(pipe);
	return result;
}
