//
//  InfoPanel.cpp
//  JT-SessionManager - App
//	Displays client information in the window
//  Created by Carl Moore on 2/19/21.
//
#include "InfoPanel.hpp"

InfoPanel::InfoPanel()
{
}

InfoPanel::~InfoPanel()
{

}

void InfoPanel::resized()
{
	
}

void InfoPanel::clearInfoDisplay()
{
	mPanelTitle.setText("Select or Create a Client", juce::dontSendNotification);
	mPort.setText("-", juce::dontSendNotification);
	mPort.setText("-", juce::dontSendNotification);
	mPort.setText("-", juce::dontSendNotification);
	mConnectStatus.setText("-", juce::dontSendNotification);
}


void InfoPanel::updateInfoDisplay(juce::String name, bool isConnected, int port, int skew, float quality)
{
	mPanelTitle.setText(name, juce::dontSendNotification);
	mPort.setText(std::to_string(port), juce::dontSendNotification);
	mSkew.setText(std::to_string(skew), juce::dontSendNotification);
	mQuality.setText(std::to_string(quality), juce::dontSendNotification);
	
	if (isConnected)
	{
		mConnectStatus.setText("CONNECTED", juce::dontSendNotification);
	} else {
		mConnectStatus.setText("NOT CONNECTED", juce::dontSendNotification);
	}
}
