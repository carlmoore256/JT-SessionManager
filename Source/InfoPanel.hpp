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
	InfoPanel(int padding = 15, int innerPadding = 5);
	~InfoPanel();
	
	void updateInfoDisplay(juce::String name, bool isConnected, int port, int skew, float quality);
	
	void clearInfoDisplay();
	
	void paint(juce::Graphics& g) override;
	
	void resized() override;
	
	
private:
	juce::Label mPanelTitle;
	juce::Label mConnectStatus;
	juce::Label mPort ;
	juce::Label mSkew;
	juce::Label mQuality;
	
//	juce::Label mConsoleLabel;
	
	juce::TextEditor mConsole;
	
	juce::TextButton mDisableEnable;
	
	juce::Array<juce::Rectangle<int>> mSectionCells;
	
	int mInnerPad = 15;
	
	juce::Colour mPanelColor;

//	juce::Array<juce::Rectangle<int>> getSectionCells(int div_x, int div_y, int pad_x, int pad_y, int innerPad_x=0, int innerPad_y=0);
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InfoPanel)
};



