//
//  InfoPanel.hpp
//  JT-SessionManager - App
//
//  Created by Carl Moore on 2/19/21.
//

#ifndef InfoPanel_hpp
#define InfoPanel_hpp
#include <JuceHeader.h>
#include <stdio.h>
#include "Section.hpp"

#endif /* InfoPanel_hpp */



class InfoPanel : public Section
//public Section
{
public:
	InfoPanel();
	~InfoPanel();
	
	void updateInfoDisplay(juce::String name, bool isConnected, int port, int skew, float quality);
	
	void clearInfoDisplay();
	
	void resized() override;
	
	
private:
	juce::Label mPanelTitle;
	juce::Label mConnectStatus;
	juce::Label mPort ;
	juce::Label mSkew;
	juce::Label mQuality;
	
	juce::TextButton mDisableEnable;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InfoPanel)
};



