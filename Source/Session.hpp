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

// main classes which control the session


class Session : public juce::Component
{
public:
	Session();
	~Session();
	
	void saveSession();
	void loadSession();
	
	void createClient(juce::String name, int port, int channels, bool autoConnectAudio, bool zeroUnderrun, bool autoManage);
	
	void findEmptyPort();
	
private:
	std::vector<Client> mClientList;
	
	bool crosscheckName(juce::String name); // checks if name exists
};
