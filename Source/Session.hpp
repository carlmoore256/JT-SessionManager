//
//  Session.hpp
//  JT-SessionManager - App
//  main class that controls the session
//  Created by Carl Moore on 2/18/21.
#pragma once

//#ifndef Session_hpp
//#define Session_hpp

#include <stdio.h>
#include <JuceHeader.h>

#include "ClientList.h"
#include "InfoPanel.hpp"
#include "Client.h"


//#endif /* Session_hpp */s

#define PORT_RNG_START 4464



class Session : public juce::Component
{
public:
	Session(ClientList* cl, InfoPanel* ip);
	~Session();
	
	void update();
	
	void saveSession();
	void loadSession();
	
	void createClient(juce::String name, int port, int channels, bool autoConnectAudio, bool zeroUnderrun, bool autoManage);
    
    void freeClients();
	
	int findEmptyPort();
	
		
private:
	ClientList* mClientList;
	InfoPanel* mInfoPanel;
	
	juce::Array<Client*> mAllClients;
	
	std::unique_ptr<juce::XmlElement> clientData;
	juce::XmlElement* mColumnList = nullptr;
	juce::XmlElement* mDataList = nullptr;
	
	Client* testClient;
	
	bool nameExists(juce::String name);
	
	bool intExists(int i);
	
	juce::String findAlternateName(juce::String name);
	
	
//	std::vector<juce::String> existingNames(); // returns vector of all names
};
