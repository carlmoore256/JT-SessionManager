#ifndef MAINCOMPONENT_H
#define MAINCOMPONENT_H

#include <JuceHeader.h>
//#include <stdio.h>
//
//#include "Client.h"
#include "ClientList.h"
#include "InfoPanel.hpp"
#include "Section.hpp"
#include "Session.hpp"
//#include "SignalRouter.h"

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
	
	juce::Label nameInput_l;
	
	juce::TextButton createClient_b;
	juce::TextButton saveSession_b;
	juce::TextButton loadSession_b;
//	juce::Label createClient_l;
	
	void renderTopBar(juce::Array<juce::Rectangle<int>> rowCells, int innerPad=15);
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

#endif /* MAINCOMPONENT_H */
