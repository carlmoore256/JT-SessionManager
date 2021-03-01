//
//  Session.cpp
//  JT-SessionManager - App
//
//  Created by Carl Moore on 2/18/21.
//

#include "Session.hpp"

Session::Session(ClientList* cl, InfoPanel* ip) : mClientList(cl), mInfoPanel(ip)
{
    DBG("Session: constructor called");
    
	createClient("testClient", 0, 1, true, true, true);
    connectClientSignals();
    connectHostSignals();
}

Session::~Session()
{
    DBG("Session: destructor called");

    freeClients();
}
void Session::saveSession()
{
	DBG("Session: saving session...");
}
void Session::loadSession()
{
	DBG("Session: loading session...");
}

void Session::update()
{
	int selectedClient = mClientList->getLatestSelection();
//	then take this selected client, and send update to infoPanel
	
	
//	mInfoPanel
}

void Session::createClient(juce::String name, int port, int channels, bool autoConnectAudio, bool zeroUnderrun, bool autoManage)
{
	if(nameExists(name))
		name = findAlternateName(name);
	
	if (port == -1) // -1 specified in interface to be no input
		port = findEmptyPort();
    
	Client* newClient = new Client(name, port, channels, autoConnectAudio, zeroUnderrun, autoManage);
	mAllClients.add(newClient);
}

void Session::freeClients()
{
    for (Client* client : mAllClients)
    {
        // need a public method for stopping a client's server.
        // otherwise we'll always crash here
//        client->stopServer
        delete client;
    }
}

// finds the lowest value empty port
int Session::findEmptyPort()
{
	int emptyPort = PORT_RNG_START;
	
	juce::Array<int> takenPorts;
	
	for (Client* client : mAllClients)
		takenPorts.add(client -> getPort());
	
	
	while (takenPorts.contains(emptyPort))
		emptyPort++;
	
	return emptyPort;
}

void Session::connectClientSignals()
{
    // nested for loops to run through mAllClients, pull up each one's signal router
    // - inner loop makes connections to all other entries in mAllClients. use thisClient.mSignalRouter.setDest() and thisClient.mSignalRouter.connect();
    // - add connected clients to thisClient.mSignalDestinations as we go
    
    // test one client connecting to a dummy client
    Client* thisClient = mAllClients[0];
    
    thisClient->mSignalRouter.setDest("dummy");
    thisClient->mSignalRouter.generateCommand(SignalRouter::CommandType::connect, "receive_1", "send_1");
    thisClient->mSignalRouter.issueCommand();
    thisClient->mSignalDestinations.add(thisClient->getName());
}

void Session::disconnectClientSignals()
{
    // run through all clients and disconnect them from each other
}

void Session::connectHostSignals()
{
    // connect the host's microphone to all client sends
    // connect all client receives to host's speakers
    
    // test one client
    Client* thisClient = mAllClients[0];
    
    mSignalRouter.setSource("system");
    mSignalRouter.setDest(thisClient->getName());
    mSignalRouter.generateCommand(SignalRouter::CommandType::connect, "capture_1", "send_1");
    mSignalRouter.issueCommand();

    mSignalRouter.setSource(thisClient->getName());
    mSignalRouter.setDest("system");
    mSignalRouter.generateCommand(SignalRouter::CommandType::connect, "receive_1", "playback_1");
    mSignalRouter.issueCommand();
}

void Session::disconnectHostSignals()
{
    // disconnect host's microphone from all client sends
    // disconnect all client receives from host's speakers

    // test one client
    Client* thisClient = mAllClients[0];
    
    mSignalRouter.setSource("system");
    mSignalRouter.setDest(thisClient->getName());
    mSignalRouter.generateCommand(SignalRouter::CommandType::disconnect, "capture_1", "send_1");
    mSignalRouter.issueCommand();

    mSignalRouter.setSource(thisClient->getName());
    mSignalRouter.setDest("system");
    mSignalRouter.generateCommand(SignalRouter::CommandType::disconnect, "receive_1", "playback_1");
    mSignalRouter.issueCommand();
}

bool Session::nameExists(juce::String name)
{
	for (Client* client : mAllClients)
		if(client -> compareName(name))
			return true;
	return false;
}

juce::String Session::findAlternateName(juce::String name)
{
	juce::String altName = name;
	int i = 0;
	while (nameExists(name))
	{
		altName = name + juce::String(" (" + std::to_string(i) + ")");
		i++;
	}
	return altName;
}
