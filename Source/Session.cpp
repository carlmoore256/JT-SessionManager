//
//  Session.cpp
//  JT-SessionManager - App
//
//  Created by Carl Moore on 2/18/21.
//

#include "Session.hpp"

Session::Session()
{
	createClient("test", 0, 1, true, true, true);
}

Session::~Session()
{
	
}
void Session::saveSession()
{
	
}
void Session::loadSession()
{
	
}

void Session::createClient(juce::String name, int port, int channels, bool autoConnectAudio, bool zeroUnderrun, bool autoManage)
{
	if(nameExists(name))
		name = findAlternateName(name);
	
	if (port == -1) // -1 specified in interface to be no input
		port = findEmptyPort();
	
	Client newClient(name, port, channels, autoConnectAudio, zeroUnderrun, autoManage);
	
	testClient = &newClient;
	DBG(testClient->getName() + " TESTT \n");
//	Client newClient(name, port, channels, autoConnectAudio, zeroUnderrun, autoManage);
//	testClient = newClient;
	
	mAllClients.add(&newClient);
}

// finds the lowest value empty port
int Session::findEmptyPort()
{
	int emptyPort = PORT_RNG_START;
	
	juce::Array<int> takenPorts;
	
//	for (int i = 0; i<mAllClients.size(); i++)
//		takenPorts.add(mAllClients[i].getPort());
	for (Client* client : mAllClients)
		takenPorts.add(client -> getPort());
	
	
	while (takenPorts.contains(emptyPort))
		emptyPort++;
	
	return emptyPort;
}

void Session::checkTestClient()
{
	DBG("CHECKING CLIENT!");
	DBG(testClient->getName());
}

bool Session::nameExists(juce::String name)
{
//	for (int i = 0; i<mAllClients.size(); i++)
//		if(mAllClients[i].compareName(name))
//			return true;
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
