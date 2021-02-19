#pragma once

#include <JuceHeader.h>
#include "ClientList.h"


//#endif /* ClientList_hpp */

//	put all your controls and content here

class MainComponent  : public juce::Component
{
public:

	MainComponent();
	~MainComponent() override;


	void paint (juce::Graphics&) override;
	void resized() override;

private:
	ClientList mClientList;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


