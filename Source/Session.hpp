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
	
	void saveSession(File saveFile);
	void loadSession(File openFile);
	
	void createClient(String name, int port, int channels, bool autoConnectAudio, bool zeroUnderrun, bool autoManage);
    
    void freeClients();
	
	int findEmptyPort();
	
	bool debugSessionUpdateTime = false;
		
private:
	ClientList* mClientList;
	InfoPanel* mInfoPanel;
	
//	XmlElement* sess_AllClientInfo;
	XmlElement sess_AllClientInfo;
	
//	juce::Array<Client> sess_AllClients;s
	// owned array will automatically delete elements when removed from the array. So consider getting rid of freeClients()
	OwnedArray<Client> sess_AllClients;
	
	//std::unique_ptr<juce::XmlElement> mClientData;
	
	Time mUpdateTime;

	//	for loading listbox headers, and opening saved file directory automatically
	juce::File mResourceDir;
	
//	juce::XmlElement* mColumnList = nullptr;
//	juce::XmlElement* mDataList = nullptr;
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
};
