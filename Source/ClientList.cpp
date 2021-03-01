//
//  ClientList.cpp
//  JT-SessionManager - App
//
//  Created by Carl Moore on 2/17/21.
//

#include "ClientList.h"


ClientList::ClientList()
{
//	loadData();
	mNumRows = 2;
	addAndMakeVisible (mTable);

	mTable.setColour (juce::ListBox::outlineColourId, juce::Colours::grey);
	mTable.setOutlineThickness (1);

	mTable.getHeader().setSortColumnId (1, true);
	mTable.setMultipleSelectionEnabled (false);
}

ClientList::~ClientList()
{
	
}

void ClientList::updateClientList(juce::Array<Client*> allClients)
{
	
	mNumRows = allClients.size();
//	mNumRows 
}

void ClientList::resized()
{
	mTable.setBoundsInset (juce::BorderSize<int> (8));
}

void ClientList::addHeaderColumn(juce::String colName, int colID, int width, int minWidth)
{
	mTable.getHeader().addColumn(colName, // name of column
								 colID, // column id
								 width, // column init width
								 minWidth, // column min width
								 400, // column max width
								 juce::TableHeaderComponent::defaultFlags,
								 -1); // insert index
}

void ClientList::selectedRowsChanged(int lastRowSelected)
{
	DBG("row selected: " + std::to_string(lastRowSelected));
	mCurrentlySelectedRow = lastRowSelected;
	mNewSelection = true;
}


void ClientList::paintRowBackground (juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
	auto alternateColour = getLookAndFeel().findColour (juce::ListBox::backgroundColourId).interpolatedWith (getLookAndFeel().findColour (juce::ListBox::textColourId), 0.03f);
	if (rowIsSelected)
		g.fillAll (juce::Colours::lightblue);
	else if (rowNumber % 2)
		g.fillAll (alternateColour);
}


void ClientList :: paintCell (juce::Graphics& g, int rowNumber, int columnId,
				int width, int height, bool rowIsSelected)
{
	g.setColour (rowIsSelected ? juce::Colours::darkblue : getLookAndFeel().findColour (juce::ListBox::textColourId));
	g.setFont (font);
	DBG("paintCell - rowNum " + juce::String(rowNumber) + " colId " + juce::String(columnId));
	
	juce::String cellText;
	
	switch (columnId) {
		case 0:
			cellText = juce::String(columnId);
			break;

		case 1:
			cellText = cl_AllClients[rowNumber]->getName();
			
		case 2:
			cellText = juce::String(cl_AllClients[rowNumber]->getPort());
			
		case 3:
			cellText = juce::String(cl_AllClients[rowNumber]->getNumChannels());
			
		default:
			break;
	}
	
	for (Client* client : cl_AllClients)
	{

	}
	
	if (auto* rowElement = mDataList->getChildElement (rowNumber))
	{
		auto text = rowElement->getStringAttribute (getAttributeNameForColumnId (columnId));

		g.drawText (text, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
	}

	g.setColour (getLookAndFeel().findColour (juce::ListBox::backgroundColourId));
	g.fillRect (width - 1, 0, 1, height);
}


void ClientList::sortOrderChanged (int newSortColumnId, bool isForwards)
{
	if (newSortColumnId != 0)
	{
		DataSorter sorter (getAttributeNameForColumnId (newSortColumnId), isForwards);
		mDataList->sortChildElements (sorter);

		mTable.updateContent();
	}
}

int ClientList::getColumnAutoSizeWidth(int columnId)
{
	if (columnId == 9)
		return 50;

	int widest = 32;

		for (auto i = getNumRows(); --i >= 0;)
		{
			if (auto* rowElement = mDataList->getChildElement (i))
			{
				auto text = rowElement->getStringAttribute (getAttributeNameForColumnId (columnId));

				widest = juce::jmax (widest, font.getStringWidth (text));
			}
		}

	return widest + 8;
}

int ClientList::getSelection (const int rowNumber) const
{
	return mDataList->getChildElement (rowNumber)->getIntAttribute ("Select");
}

void ClientList::setSelection (const int rowNumber, const int newSelection)
{
	mDataList->getChildElement (rowNumber)->setAttribute ("Select", newSelection);
}

juce::String ClientList::getText (const int columnNumber, const int rowNumber) const
{
	return mDataList->getChildElement (rowNumber)->getStringAttribute (getAttributeNameForColumnId (columnNumber));
}

void ClientList::setText(const int columnNumber, const int rowNumber, const juce::String& newText)
{
	DBG("CALLING SET TEXT");
	const auto& columnName = mTable.getHeader().getColumnName (columnNumber);
	mDataList->getChildElement (rowNumber)->setAttribute (columnName, newText);
}

int ClientList::getLatestSelection()
{
//	return mNewSelection;
	bool newSelection = mNewSelection;
	mNewSelection = false;
	
	if (newSelection)
	{
		return mCurrentlySelectedRow;
	} else {
		// return -1 so session knows not to update (possibly change this,
		// so that session is constantly updating no matter what
//		return -1;
		
		return mCurrentlySelectedRow;
	}
}


// ====== PRIVATE ========


juce::String ClientList::getAttributeNameForColumnId (const int columnId) const
{
	forEachXmlChildElement(*mColumnList, columnXml)
	{
		if (columnXml->getIntAttribute("columnId") == columnId)
			return columnXml->getStringAttribute("name");
	}
	return {};
}



//===============GRAVEYARD==================


// move to session
//void ClientList::loadData()
//{
//	//	auto dir = juce::File::getCurrentWorkingDirectory();
//	//	juce::String filePath = juce::File::getCurrentWorkingDirectory().getFullPathName();
//	auto locationType = juce::File::SpecialLocationType::currentApplicationFile;
//	auto dir = juce::File::getSpecialLocation(locationType);
//
//	int numTries = 0;
//
//	while (! dir.getChildFile ("Resources").exists() && numTries++ < 15)
//		dir = dir.getParentDirectory();
//
//	auto tableFile = dir.getChildFile ("Resources").getChildFile ("SessionData.xml");
//
//	std::cout << tableFile.getFullPathName();
//
//	if (tableFile.exists())
//	{
//		DBG("TABLE FILE EXITS");
//		clientData = juce::XmlDocument::parse (tableFile);            // [3]
//
//		mDataList   = clientData->getChildByName ("DATA");
//		mColumnList = clientData->getChildByName ("HEADERS");          // [4]
//
//		mNumRows = mDataList->getNumChildElements();                      // [5]
//	}
//}

// eventually move to session
//void ClientList::loadTableHeaders()
//{
//	// reset all column headers before displaying, migtht result in bugs
//	DBG("resetting columns...");
//	resetColumns();
//
//	auto locationType = juce::File::SpecialLocationType::currentApplicationFile;
//	auto dir = juce::File::getSpecialLocation(locationType);
//
//	int numTries = 0;
//
//	while (! dir.getChildFile ("Resources").exists() && numTries++ < 15)
//		dir = dir.getParentDirectory();
//
//	auto tableFile = dir.getChildFile ("Resources").getChildFile ("SessionData.xml");
//
//	std::cout << tableFile.getFullPathName();
//
//	if (tableFile.exists())
//	{
//		DBG("TABLE FILE EXITS");
//		clientData = juce::XmlDocument::parse (tableFile);            // [3]
//
////		doesn't make sense here to make a list of headers, since these will be defined by the software, when TableHeaders.xml is loaded, which provides headers
//		mDataList   = clientData->getChildByName ("DATA");
//		mColumnList = clientData->getChildByName ("HEADERS");          // [4]
//
//		mNumRows = mDataList->getNumChildElements();                      // [5]
//	}
//}
