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

#include "ClientListbox.h"
#include "InfoPanel.hpp"
#include "Client.h"


//#endif /* Session_hpp */s

#define PORT_RNG_START 0



class Session : public juce::Component
{
public:
	Session(ClientListbox* cl, InfoPanel* ip);
	~Session();
	
	void update();
	
	void saveSession();
	void loadSession();
	
	void createClient(String name, int port, int channels, bool autoConnectAudio, bool zeroUnderrun, bool autoManage);
    
    void freeClients();
	
	int findEmptyPort();
	
	bool debugSessionUpdateTime = false;
		
private:
	ClientListbox* mClientList;
	InfoPanel* mInfoPanel;
	
	XmlElement sess_AllClientInfo;
	
//	juce::Array<Client> sess_AllClients;
	// owned array will automatically delete elements when removed from the array. So consider getting rid of freeClients()
	OwnedArray<Client> sess_AllClients;
	
	//std::unique_ptr<juce::XmlElement> mClientData;
	
	Time mUpdateTime;

	//	for loading listbox headers, and opening saved file directory automatically
	juce::File mResourceDir;
	
	std::unique_ptr<juce::XmlElement> mTableHeadPtr;
	
	bool nameExists(juce::String name);
	
	bool intExists(int i);
	
	juce::String findAlternateName(juce::String name);
		
	
	// ==== LOADING & SAVING ==========
	
	//	loads the headers of each column for mClientList from TableHeaders.xml
	void loadTableHeaders(juce::File xmlTableHeaders);
	
	
	XmlElement getClientXmlStats();
	
	// requests clients to refresh their internal stats reported at sess_AllClientInfo
	void broadcastClientUpdate();
	
	// kills all jacktrip daemons
	void killJacktripProcesses();
};
