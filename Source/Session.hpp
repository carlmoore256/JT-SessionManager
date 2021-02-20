//
//  Session.hpp
//  JT-SessionManager - App
//
//  Created by Carl Moore on 2/18/21.
//

#ifndef Session_hpp
#define Session_hpp

#include <stdio.h>
#include <JuceHeader.h>
#include "Client.h"

#endif /* Session_hpp */

#define PORT_RNG_START 4464

// main classes which control the session


class Session : public juce::Component
{
public:
	Session();
	~Session();
	
	void saveSession();
	void loadSession();
	
	void createClient(juce::String name, int port, int channels, bool autoConnectAudio, bool zeroUnderrun, bool autoManage);
	
	int findEmptyPort();
	
private:
//	std::vector<Client> mClientList;
	juce::Array<Client*> mAllClients;
	
	bool nameExists(juce::String name);
	
	bool intExists(int i);
	
	juce::String findAlternateName(juce::String name);
	
	
//	std::vector<juce::String> existingNames(); // returns vector of all names
};
