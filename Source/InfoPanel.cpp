//
//  InfoPanel.cpp
//  JT-SessionManager - App
//	Displays client information in the window
//  Created by Carl Moore on 2/19/21.
//
#include "InfoPanel.hpp"

InfoPanel::InfoPanel(int padding, int innerPadding) : Section(3, 8, padding, padding, innerPadding, innerPadding)
{
	mPanelColor = juce::Colour (0x420000);
	
	clearInfoDisplay();
	
	addAndMakeVisible(mPanelTitle);
	addAndMakeVisible(mPort);
	addAndMakeVisible(mSkew);
	addAndMakeVisible(mQuality);
	
//	addAndMakeVisible(mConsoleLabel);
//	mConsoleLabel.setText("Console Output", juce::dontSendNotification);
	
	mPanelTitle.setFont (juce::Font (16.0f, juce::Font::bold));
	
	mPanelTitle.setColour (juce::Label::textColourId, juce::Colours::white);
	
	mPanelTitle.setJustificationType (juce::Justification::centred);
	
	
	addAndMakeVisible(mConsole);
}

InfoPanel::~InfoPanel()
{}

void InfoPanel::paint(juce::Graphics& g)
{

	auto area = getLocalBounds();

	area.reduce(mInnerPad, mInnerPad);
//	updateSectionCells(area);
	
	g.setColour(juce::Colours::darkgrey);
	g.fillRect(area);
	
	
//	g.setColour(mPanelColor);
	g.setColour(juce::Colours::red);
	
//	g.fillRect(getCellAt(0,0));
	
	g.fillRect(getMergedRow(0));
	
		
//	this will work for other components
//	g.fillRect(area.reduced(30,30));
	
	auto innerArea = area.reduced(mInnerPad, mInnerPad);
	
	auto consoleBounds = innerArea.withTop((int)(innerArea.getHeight()/1.2));

	mConsole.setBounds(consoleBounds);
}

void InfoPanel::resized()
{
	
	auto area = getLocalBounds();

	area.reduce(mInnerPad, mInnerPad);
	updateSectionCells(area);
	
	mPanelTitle.setBounds(sectionCells[0].getUnion(sectionCells[1]));
	
	
//	mPanelTitle.setBounds(0,0,getWidth(),100);
	mPort.setBounds(sectionCells[3]);
	mSkew.setBounds(sectionCells[6]);
	
}

void InfoPanel::clearInfoDisplay()
{
	mPanelTitle.setText("Select or Create a Client", juce::dontSendNotification);
	mPort.setText("-", juce::dontSendNotification);
	mSkew.setText("-", juce::dontSendNotification);
	mQuality.setText("-", juce::dontSendNotification);
	mConnectStatus.setText("-", juce::dontSendNotification);
}

void InfoPanel::updateDisplay(XmlElement* clientXml)
{
	// set the name
	mPanelTitle.setText(clientXml->getStringAttribute("Name"), juce::dontSendNotification);
	mPort.setText(clientXml->getStringAttribute("Port"), juce::dontSendNotification);
	mSkew.setText(clientXml->getStringAttribute("Skew"), juce::dontSendNotification);
	mQuality.setText(clientXml->getStringAttribute("Quality"), juce::dontSendNotification);
	mConnectStatus.setText(clientXml->getStringAttribute("Connection"), juce::dontSendNotification);
}


//void InfoPanel::updateInfoDisplay(juce::String name, bool isConnected, int port, int skew, float quality)
//{
//	mPanelTitle.setText(name, juce::dontSendNotification);
//	mPort.setText(std::to_string(port), juce::dontSendNotification);
//	mSkew.setText(std::to_string(skew), juce::dontSendNotification);
//	mQuality.setText(std::to_string(quality), juce::dontSendNotification);
//
//	if (isConnected)
//	{
//		mConnectStatus.setText("CONNECTED", juce::dontSendNotification);
//	} else {
//		mConnectStatus.setText("NOT CONNECTED", juce::dontSendNotification);
//	}
//}
