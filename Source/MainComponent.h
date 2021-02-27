#pragma once

#include <JuceHeader.h>
#include "ClientList.h"
#include "InfoPanel.hpp"
#include "Session.hpp"
#include "Section.hpp"


//#endif /* ClientList_hpp */

//	put all your controls and content here

class MainComponent  : public Section,
						public juce::Button::Listener // eventually remove this and figure out inheritance problems. Only here because classes that inherit from this have abstract class errors if the listener is inherited
{
public:

	MainComponent();
	~MainComponent() override;

	void paint (juce::Graphics&) override;
	void resized() override;
	
	void buttonClicked(juce::Button* button) override;

private:
	ClientList mClientList;
	InfoPanel mInfoPanel;
	Session mSession;
	
	juce::TextButton createClient_b;
	
//	juce::Label createClient_l;
	
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

