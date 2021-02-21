//
//  ClientList.hpp
//  JT-SessionManager - App
//
//  Created by Carl Moore on 2/17/21.
//
//#pragma once

#ifndef ClientList_h
#define ClientList_h

#include <JuceHeader.h>
#include <stdio.h>
#include "Client.h"

#endif /* ClientList_h */


class ClientList : public juce::Component,
				   public juce::TableListBoxModel
{
public:
	ClientList();
	
	~ClientList();
	
	void AddClient();
	
//	void selectedRowsChanged(int lastRowSelected) override;
	
	int getNumRows() override;
	
	void resized() override;
	
	void selectedRowsChanged (int lastRowSelected) override
	{
		DBG(lastRowSelected);
	}
	
	void paintRowBackground (juce::Graphics&, int, int, int, bool) override;
	
	void paintCell (juce::Graphics& g, int rowNumber, int columnId,
					int width, int height, bool rowIsSelected) override;
	
	void sortOrderChanged (int newSortColumnId, bool isForwards) override;
	
	
	int getColumnAutoSizeWidth (int columnId) override;
	
	int getSelection (const int rowNumber) const;
	
	void setSelection (const int rowNumber, const int newSelection);
	
	juce::String getText (const int columnNumber, const int rowNumber) const;
	
	void setText (const int columnNumber, const int rowNumber, const juce::String& newText);

	Component* refreshComponentForCell (int rowNumber, int columnId, bool /*isRowSelected*/,
										Component* existingComponentToUpdate) override
	{
		if (columnId == 9)  // [8]
		{
			auto* selectionBox = static_cast<SelectionColumnCustomComponent*> (existingComponentToUpdate);

			if (selectionBox == nullptr)
				selectionBox = new SelectionColumnCustomComponent (*this);

			selectionBox->setRowAndColumn (rowNumber, columnId);
			return selectionBox;
		}

		if (columnId == 8)  // [9]
		{
			auto* textLabel = static_cast<EditableTextCustomComponent*> (existingComponentToUpdate);

			if (textLabel == nullptr)
				textLabel = new EditableTextCustomComponent (*this);

			textLabel->setRowAndColumn (rowNumber, columnId);
			return textLabel;
		}

		jassert (existingComponentToUpdate == nullptr);
		return nullptr;     // [10]
	};
	

private:
	juce::TableListBox mTable { {}, this };
	juce::Font font { 14.0f };
	
	std::unique_ptr<juce::XmlElement> clientData;
	juce::XmlElement* mColumnList = nullptr;
	juce::XmlElement* mDataList = nullptr;
	
	int mNumRows = 0;
	
	class EditableTextCustomComponent  : public juce::Label
	{
	public:
		EditableTextCustomComponent (ClientList& td)
			: owner (td)
		{
			setEditable (false, true, false);
		}

		void mouseDown (const juce::MouseEvent& event) override
		{
			owner.mTable.selectRowsBasedOnModifierKeys (row, event.mods, false);

			Label::mouseDown (event);
		}

		void textWasEdited() override
		{
			owner.setText (columnId, row, getText());
		}

		void setRowAndColumn (const int newRow, const int newColumn)
		{
			row = newRow;
			columnId = newColumn;
			setText (owner.getText(columnId, row), juce::dontSendNotification);
		}

	private:
		ClientList& owner;
		int row, columnId;
		juce::Colour textColour;
	};
	
	//===================================================================
	class SelectionColumnCustomComponent    : public Component
	{
	public:
		SelectionColumnCustomComponent (ClientList& td)
			: owner (td)
		{
			addAndMakeVisible (toggleButton);

			toggleButton.onClick = [this] { owner.setSelection (row, (int) toggleButton.getToggleState()); };
		}

		void resized() override
		{
			toggleButton.setBoundsInset (juce::BorderSize<int> (2));
		}

		void setRowAndColumn (int newRow, int newColumn)
		{
			row = newRow;
			columnId = newColumn;
			toggleButton.setToggleState ((bool) owner.getSelection (row), juce::dontSendNotification);
		}

	private:
		ClientList& owner;
		juce::ToggleButton toggleButton;
		int row, columnId;
	};

	//===================================================================
	class DataSorter
	{
	public:
		DataSorter (const juce::String& attributeToSortBy, bool forwards)
			: attributeToSort (attributeToSortBy),
			  direction (forwards ? 1 : -1)
		{}

		int compareElements (juce::XmlElement* first, juce::XmlElement* second) const
		{
			auto result = first->getStringAttribute (attributeToSort)
								.compareNatural (second->getStringAttribute (attributeToSort)); // [1]

			if (result == 0)
				result = first->getStringAttribute ("ID")
							   .compareNatural (second->getStringAttribute ("ID"));             // [2]

			return direction * result;                                                          // [3]
		}

	private:
		juce::String attributeToSort;
		int direction;
	};

	//==============================================================================
	void loadData();

	juce::String getAttributeNameForColumnId (const int columnId) const;

};
