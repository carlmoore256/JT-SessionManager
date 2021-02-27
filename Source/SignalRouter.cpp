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

void SignalRouter::connect()
{
    // generate jack_connect command here
}

void SignalRouter::disconnect()
{
    // generate jack_disconnect command here
}
