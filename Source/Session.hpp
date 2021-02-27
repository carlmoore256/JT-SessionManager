//
//  Session.hpp
//  JT-SessionManager - App
//  main class that controls the session
//  Created by Carl Moore on 2/18/21.

//#include "JT-SessionManager.h"

#ifndef Session_hpp
#define Session_hpp

#include <JuceHeader.h>
//#include <stdio.h>
//
#include "Client.h"
#include "ClientList.h"
#include "InfoPanel.hpp"
//#include "Section.hpp"
//#include "SignalRouter.h"

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
	
	Client* testClient;
	
	bool nameExists(juce::String name);
	
	bool intExists(int i);
	
	juce::String findAlternateName(juce::String name);
	
	
//	std::vector<juce::String> existingNames(); // returns vector of all names
};

#endif /* Session_hpp */
