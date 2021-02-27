//
//  SignalRouter.h
//  JT-SessionManager - App
//
//  Created by William Brent on 2/27/21.
//

#ifndef SIGNALROUTER_H
#define SIGNALROUTER_H

#include <JuceHeader.h>
//#include <stdio.h>
//
//#include "Client.h"
//#include "ClientList.h"
//#include "InfoPanel.hpp"
//#include "Section.hpp"
//#include "Session.hpp"

class SignalRouter
{
public:
    SignalRouter();
    ~SignalRouter();

    void setSource(juce::String s);
    void setDest(juce::String d);
    void connect();
    void disconnect();
    
private:

    // keep track of which client this router belongs to
    // this will be used as the signal source
    juce::String mSource;
    juce::String mDest;
};

#endif /* SIGNALROUTER_H */
