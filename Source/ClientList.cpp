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
	addAndMakeVisible (mTable);

	mTable.setColour (juce::ListBox::outlineColourId, juce::Colours::grey);
	mTable.setOutlineThickness (1);

	mTable.getHeader().setSortColumnId (1, true);
	mTable.setMultipleSelectionEnabled (false);
}

ClientList::~ClientList()
{
	
}

void ClientList::setInitPtrs(juce::Array<Client*> allClients, XmlElement* clientXml)
{
	cl_AllClients = allClients;
	cl_ClientXml = clientXml;
}

void ClientList::resized()
{
	mTable.setBoundsInset (juce::BorderSize<int> (8));
}

void ClientList::setColumnHeaders(XmlElement* headerList)
{
	// iterate through the columns and update the elements
	forEachXmlChildElement(*headerList, colXml)
	{
		auto name = colXml->getStringAttribute("name");
		juce::String columnId = colXml->getStringAttribute("columnId");
		juce::String width = colXml->getStringAttribute("width");
		
		addHeaderColumn(name,
						columnId.getIntValue(),
						width.getIntValue(),
						25); // min width
	}
	// set the global var headerlist to the pointer passed from sess
	mHeaderList = headerList;
};

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

// consider calling this from the mainComponent, or passing reference of graphics g to session, which can then call this override
void ClientList :: paintCell (juce::Graphics& g, int rowNumber, int columnId,
				int width, int height, bool rowIsSelected)
{
	g.setColour (rowIsSelected ? juce::Colours::darkblue : getLookAndFeel().findColour (juce::ListBox::textColourId));
	g.setFont (font);
	//DBG("paintCell - rowNum " + juce::String(rowNumber) + " colId " + juce::String(columnId));
	
	auto test = cl_ClientXml->getChildElement(1);
	//DBG(test->getStringAttribute ("Name"));

	// might be inefficient, look into how to implement getNextElement()
	if (auto rowElement = cl_ClientXml->getChildElement (rowNumber))
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
		cl_ClientXml->sortChildElements (sorter);

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
			if (auto* rowElement = cl_ClientXml->getChildElement (i))
			{
				auto text = rowElement->getStringAttribute (getAttributeNameForColumnId (columnId));

				widest = juce::jmax (widest, font.getStringWidth (text));
			}
		}

	return widest + 8;
}

int ClientList::getSelection (const int rowNumber) const
{
	return cl_ClientXml->getChildElement (rowNumber)->getIntAttribute ("Select");
}

void ClientList::setSelection (const int rowNumber, const int newSelection)
{
	cl_ClientXml->getChildElement (rowNumber)->setAttribute ("Select", newSelection);
}

juce::String ClientList::getText (const int columnNumber, const int rowNumber) const
{
	return cl_ClientXml->getChildElement (rowNumber)->getStringAttribute (getAttributeNameForColumnId (columnNumber));
}

void ClientList::setText(const int columnNumber, const int rowNumber, const juce::String& newText)
{
	DBG("CALLING SET TEXT");
	const auto& columnName = mTable.getHeader().getColumnName (columnNumber);
	cl_ClientXml->getChildElement (rowNumber)->setAttribute (columnName, newText);
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


String ClientList::getAttributeNameForColumnId (const int columnId) const
{
	forEachXmlChildElement(*mHeaderList, columnXml)
	{
		if (columnXml->getIntAttribute("columnId") == columnId)
			return columnXml->getStringAttribute("name");
	}
	return {};
}



//===============GRAVEYARD==================







