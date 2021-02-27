#include "MainComponent.h"

MainComponent::MainComponent()
{
	addAndMakeVisible(mClientList);
	addAndMakeVisible(mInfoPanel);
		
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
//	mInfoPanel.setBounds(split, 0, getWidth(), getHeight());
}

//=============================================================
