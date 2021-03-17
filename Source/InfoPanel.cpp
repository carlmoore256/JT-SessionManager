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
	mConsole.setReadOnly(true);
	mConsole.setMultiLine(true);
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
	
	Colour c(38, 39, 41);
	g.setColour(c);
	
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
	mPort.setText("Port: -", juce::dontSendNotification);
	mSkew.setText("Skew: -", juce::dontSendNotification);
	mQuality.setText("Quality: -", juce::dontSendNotification);
	mConnectStatus.setText("Connection: -", juce::dontSendNotification);
}

void InfoPanel::updateDisplay(XmlElement* clientXml)
{
	// set the name
	mPanelTitle.setText("Name: " + clientXml->getStringAttribute("Name"), juce::dontSendNotification);
	mPort.setText("Port: " + clientXml->getStringAttribute("Port"), juce::dontSendNotification);
	mSkew.setText("Skew: " + clientXml->getStringAttribute("Skew"), juce::dontSendNotification);
	mQuality.setText("Quality: " + clientXml->getStringAttribute("Quality"), juce::dontSendNotification);
	mConnectStatus.setText("Connection: " + clientXml->getStringAttribute("Connection"), juce::dontSendNotification);
	
	mConsole.setText(clientXml->getStringAttribute("stdout"));
}
