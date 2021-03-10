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

#ifndef BIN_PATH
#define BIN_PATH "/usr/local/bin/"
#endif

class SignalRouter : juce::ChildProcess
{
public:
    SignalRouter();
    ~SignalRouter();

    enum CommandType
    {
        disconnect,
        connect,
        listPort
    };
    
    void setSource(juce::String s);
    void setDest(juce::String d);
    void generateCommand(CommandType t, juce::String jackPort, juce::String destJackPort);
    void issueCommand();
    
private:

    // keep track of which client this router belongs to
    // this will be used as the signal source
    juce::String mSource;
    juce::String mDest;
    juce::String mCommand;
    
    ChildProcess mChildProcess;
};

#endif /* SIGNALROUTER_H */
