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
	void loadSession(juce::File sessionFile);
	
	void createClient(juce::String name, int port, int channels, bool autoConnectAudio, bool zeroUnderrun, bool autoManage);
    
    void freeClients();
	
	int findEmptyPort();
	
		
private:
	ClientList* mClientList;
	InfoPanel* mInfoPanel;
	
	juce::Array<Client*> mAllClients;
	
	std::unique_ptr<juce::XmlElement> clientData;

	//	for loading listbox headers, and opening saved file directory automatically
	juce::File mResourceDir;
	
	juce::XmlElement* mColumnList = nullptr;
	juce::XmlElement* mDataList = nullptr;

	
	bool nameExists(juce::String name);
	
	bool intExists(int i);
	
	juce::String findAlternateName(juce::String name);
		
	
	// ==== LOADING / SAVING ==========
	
	//	loads the headers of each column for mClientList from TableHeaders.xml
	void loadTableHeaders(juce::File xmlTableHeaders);
	
//	std::vector<juce::String> existingNames(); // returns vector of all names
};
