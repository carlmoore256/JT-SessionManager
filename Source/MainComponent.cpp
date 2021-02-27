#include "MainComponent.h"

MainComponent::MainComponent() : Section(8, 8), mSession(&mClientList, &mInfoPanel)
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
	mSession.update();
	
	g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
	
	g.setColour(juce::Colours::red);
	auto topBar = getMergedRow(0);
	g.fillRect(topBar);
		
	
	//	get a rectangle starting at the 1st row, extending to the 6th
	//	for the body of the GUI
	auto centerSection = getMergedRows(1, 6);

	int childWidth = centerSection.getWidth() / 3;
	mClientList.setBounds(centerSection.removeFromLeft(childWidth*2));
	mInfoPanel.setBounds(centerSection.removeFromLeft(childWidth));
	
//	DBG(mClientList.getSelection(const int rowNumber))
}

void MainComponent::resized()
{
	updateSectionCells(getLocalBounds());
}

//=============================================================
