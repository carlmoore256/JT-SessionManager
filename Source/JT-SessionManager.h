//
//  JT-SessionManager.h
//  JT-SessionManager - App
//
//  Created by William Brent on 2/27/21.
//

/*
    should we go with a master header file strategy? it's easier to make sense of, but will slow compile times during the span of time while these headers are actively under development. apparently, there's a pretty even split on whether master header or include-what-you-need-per source file isbest practice. decent discussion on the topic here:

    https://stackoverflow.com/questions/15664393/master-include-files-good-or-bad-practice
 
        and here:
 
    http://www.cplusplus.com/forum/beginner/250945/
 
 */

//#ifndef JT_SessionManager_h
//#define JT_SessionManager_h

#include <JuceHeader.h>
#include <stdio.h>

#include "Client.h"
#include "ClientList.h"
#include "InfoPanel.hpp"
#include "Section.hpp"
#include "Session.hpp"
#include "SignalRouter.h"

//#endif /* JT_SessionManager_h */
