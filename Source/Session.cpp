//
//  Session.cpp
//  JT-SessionManager - App
//
//  Created by Carl Moore on 2/18/21.
//

#include "Session.hpp"

Session::Session()
{
	
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
	
	Client* newClient = new Client(name, port, channels, autoConnectAudio, zeroUnderrun, autoManage);
	mClientList.push_back(newClient);
}

void Session::findEmptyPort()
{
	for (Client& client : mClientList)
	{
		int takenPort = client.getPort();
	}
}

bool Session::nameExists(juce::String name)
{
	for (Client& client : mClientList)
	{
		if(client.compareName(name))
			return true;
	}
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

















juce::String Session::crosscheckName(juce::String name)
{
//	bool validName = false;
//	juce::String newName;
	
//	int i = 0;
	
//	while (!validName)
//	{
//		for (Client& client : mClientList)
//		{
//			if(client.compareName(name))
//			{
//				name = name + juce::String(" (" + std::to_string(i) + ")");
//				i++;
//			} else {
//				break;
//			}
//		}
//	}


	return name;
}
