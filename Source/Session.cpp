//
//  Session.cpp
//  JT-SessionManager - App
//
//  Created by Carl Moore on 2/18/21.
//

#include "Session.hpp"

Session::Session(ClientList* cl, InfoPanel* ip) : mClientList(cl), mInfoPanel(ip)
{
	createClient("test", 0, 1, true, true, true);
    connectSignals();
}

Session::~Session()
{
    freeClients();
}
void Session::saveSession()
{
	DBG("Saving Session...");
}
void Session::loadSession()
{
	DBG("Loading Session...");
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
        delete client;
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

void Session::connectSignals()
{
    // nested for loops to run through mAllClients, pull up each one's signal router
    // - inner loop makes connections to all other entries in mAllClients. use thisClient.mSignalRouter.setDest() and thisClient.mSignalRouter.connect();
    // - add connected clients to thisClient.mSignalDestinations as we go
    
    // test one client
    Client* thisClient = mAllClients[0];
    
    thisClient->mSignalRouter.setDest("dummy");
    thisClient->mSignalRouter.connect();
    thisClient->mSignalDestinations.add("dummy");
    
    DBG("signal connection");
}

void Session::disconnectSignals()
{
    // run through all clients, check the contents of each client's mSignalDestinations array
    //  - disconnect from each destination with thisClient.mSignalRouter.setDest() and thisClient.mSignalRouter.disconnect()
    //  - remove each destination from mSignalDestinations
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
