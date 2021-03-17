#pragma once

#include <JuceHeader.h>
#include "ClientListbox.h"
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
	ClientListbox mClientList;
	InfoPanel mInfoPanel;
	Session mSession;
	
	juce::Label nameInput_l;
	
	juce::TextButton createClient_b;
	juce::TextButton saveSession_b;
	juce::TextButton loadSession_b;
//	juce::Label createClient_l;
	
	void renderTopBar(juce::Array<juce::Rectangle<int>> rowCells, int innerPad=15);
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

