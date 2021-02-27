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
    
private:

};

#endif /* SIGNALROUTER_H */
