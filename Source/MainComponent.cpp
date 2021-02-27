#include "MainComponent.h"

MainComponent::MainComponent() : Section(8, 8), mSession(&mClientList, &mInfoPanel)
{
	addAndMakeVisible(mClientList);
	addAndMakeVisible(mInfoPanel);
	
	addAndMakeVisible(createClient_b);
	createClient_b.setButtonText("Create Client");
	
		
	setSize (1200, 600);
}

MainComponent::~MainComponent()
{
	
	
}


void MainComponent::paint (juce::Graphics& g)
{
	mSession.update();
	
	g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
	
	g.setColour(juce::Colours::green);
	auto topBar = getMergedRow(0);
	g.fillRect(topBar);
	
	auto bottomBar = getMergedRows(7,8);
	
	g.setColour(juce::Colours::green);
	g.fillRect(bottomBar);

}

void MainComponent::resized()
{
	//	update the section cells
	updateSectionCells(getLocalBounds());
	
	//	get a rectangle starting at the 1st row, extending to the 6th
	//	for the body of the GUI
	auto centerSection = getMergedRows(1, 6);

	//	set the bounds of client list and info panel
	int childWidth = centerSection.getWidth() / 3;
	mClientList.setBounds(centerSection.removeFromLeft(childWidth*2));
	mInfoPanel.setBounds(centerSection.removeFromLeft(childWidth));
	
	//	set the bounds of buttons
	createClient_b.setBounds(getCellAt(0, 1).reduced(10,10));
}

//=============================================================

void MainComponent::buttonClicked(juce::Button* button)
{
	DBG("BUTTON CLICKED");
}
