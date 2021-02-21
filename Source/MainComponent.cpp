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
	
//	mClientList.setBounds(getLocalBounds());
	int split = (int)(getWidth()/3) * 2;
	mClientList.setBounds(0, 0, split, getHeight());
	mInfoPanel.setBounds(split, 0, getWidth(), getHeight());
}

void MainComponent::resized()
{
	// This is called when the MainComponent is resized.
	// If you add any child components, this is where you should
	// update their positions.
}

//=============================================================
