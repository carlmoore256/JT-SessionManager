#include "MainComponent.h"

MainComponent::MainComponent() : Section(8, 8), mSession(&mClientList, &mInfoPanel)
{
	addAndMakeVisible(mClientList);
	addAndMakeVisible(mInfoPanel);
	
	addAndMakeVisible(nameInput_l);
	nameInput_l.setText("Enter Name...", juce::dontSendNotification);
	nameInput_l.setEditable(true);
	nameInput_l.setColour(juce::Label::backgroundColourId, juce::Colours::white);
	nameInput_l.setColour(juce::Label::textColourId, juce::Colours::black);

	addAndMakeVisible(createClient_b);
	createClient_b.setButtonText("Create Client");
	createClient_b.addListener(this);
	
	addAndMakeVisible(saveSession_b);
	saveSession_b.setButtonText("Save Session");
	saveSession_b.addListener(this);
	
	addAndMakeVisible(loadSession_b);
	loadSession_b.setButtonText("Load Session");
	loadSession_b.addListener(this);
		
	setSize (1200, 600);
}

MainComponent::~MainComponent()
{
	//	not necessary but good practice, when using custom listeners this will be important
	createClient_b.removeListener(this);
	saveSession_b.removeListener(this);
	loadSession_b.removeListener(this);
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
	
	
//	createClient_b.setBounds(getCellAt(0, 1).reduced(10,10));
//	loadSession_b.setBounds(getCellAt(0, 2).reduced(10, 10));
	
//	render the top bar, provide row
	renderTopBar(getRowCells(0));
}

//=============================================================

void MainComponent::buttonClicked(juce::Button* button)
{
	if(button == &createClient_b)
		mSession.createClient(nameInput_l.getText(), -1, 1, true, true, true);
		DBG("CREATE A CLIENT!");
	

//	saveSession and loadSession both require a file popup
//	make these popup methods fileSavePopup and fileLoadPopup
	
//	JUCE component:
//	FileChooserDialogBox
	
//	if(button == &saveSession_b)
//		mSession.saveSession(fileSavePopup());
//
//	if(button == &loadSession_b)
//		mSession.loadSession(fileLoadPopup());

}

//=============================================================


void MainComponent::renderTopBar(juce::Array<juce::Rectangle<int>> rowCells, int innerPad)
{
	nameInput_l.setBounds(rowCells[0].getUnion(rowCells[1]).reduced(innerPad));
	//	set the bounds of buttons
	createClient_b.setBounds(rowCells[2].reduced(innerPad));
	saveSession_b.setBounds(rowCells[3].reduced(innerPad));
	loadSession_b.setBounds(rowCells[4].reduced(innerPad));
	
	
}
