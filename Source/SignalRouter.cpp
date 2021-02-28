//
//  SignalRouter.cpp
//  JT-SessionManager - App
//
//  Created by William Brent on 2/27/21.
//

#include "SignalRouter.h"

SignalRouter::SignalRouter()
{
}

SignalRouter::~SignalRouter()
{
}

//=================================
void SignalRouter::setSource(juce::String s)
{
    mSource = s;
}

void SignalRouter::setDest(juce::String d)
{
    mDest = d;
}

void SignalRouter::generateCommand(CommandType t, juce::String sourceJackPort, juce::String destJackPort)
{
    juce::String binaryPath;
    
    binaryPath = BIN_PATH;
//    binary = "/usr/local/bin/";
    
    switch(t)
    {
        case disconnect:
            binaryPath += "jack_disconnect";
            break;
        case connect:
            binaryPath += "jack_connect";
            break;
        default:
            break;
    }
    
    // generate jack_connect command here
    mCommand =
    binaryPath + " "
    + juce::String(mSource) + ":" + sourceJackPort
    + " "
    + juce::String(mDest) + ":" + destJackPort;
}

void SignalRouter::issueCommand()
{
    DBG(mCommand);
    
    if(mChildProcess.start (mCommand, wantStdOut | wantStdErr))
    {
        DBG("signal routing command issued");
    }
}
