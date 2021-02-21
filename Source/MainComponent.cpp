#include "MainComponent.h"

MainComponent::MainComponent()
{
	addAndMakeVisible(mClientList);
	addAndMakeVisible(mInfoPanel);
	
	addAndMakeVisible(labeltest);
	labeltest.setFont (juce::Font (16.0f, juce::Font::bold));
	labeltest.setText("ASJKDHASKJDHAJS", juce::dontSendNotification);
	labeltest.setColour (juce::Label::textColourId, juce::Colours::white);
	
	setSize (1200, 600);
}

MainComponent::~MainComponent()
{
	
	
}



void MainComponent::paint (juce::Graphics& g)
{
	g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
	auto area = getLocalBounds();
	int childWidth = area.getWidth() / 3;
	mClientList.setBounds(area.removeFromLeft(childWidth*2));
	mInfoPanel.setBounds(area.removeFromLeft(childWidth));
	
//	DBG(mClientList.getSelection(<#const int rowNumber#>))
}

void MainComponent::resized()
{
	labeltest.setBounds(0, 0, getWidth()-20, 30);
//	mInfoPanel.setBounds(split, 0, getWidth(), getHeight());
}

//=============================================================
