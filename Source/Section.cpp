//
//  Section.cpp
//  JT-SessionManager - App
//
//  Created by Carl Moore on 2/19/21.
//

#include "Section.hpp"


Section::Section() {}
Section::~Section() {}

void Section::setProperties(juce::Colour _color)
{
	color = _color;
}

void Section::resized() {}

juce::Array<juce::Rectangle<int>> Section::getComponentBounds(int div_x, int div_y, int pad_x, int pad_y, int innerPad_x, int innerPad_y)
{
	assert(div_x > 0 && div_y > 0);
	
	juce::Array<juce::Rectangle<int>> allBounds;
	
	auto area = getBoundsInParent();
	area.reduce(innerPad_x, innerPad_y);
	int columnWidth = area.getWidth() / div_x;
	int cellHeight = area.getHeight() / div_y;
	
	for(int i = 0; i < div_x; i++)
	{
		juce::Rectangle<int> thisColumn = area;

		thisColumn.removeFromRight(columnWidth * (div_x - (i + 1)));
		thisColumn.removeFromLeft(columnWidth * i);
		
		for(int j = 0; j < div_y; j++)
		{
			juce::Rectangle<int> thisCell = thisColumn;
			thisCell.removeFromTop(j * cellHeight);
			thisCell.setHeight(cellHeight);
			
			thisCell.reduce(pad_x, pad_y);
			
			allBounds.add(thisCell);
		}
	}
	
	//containerCells = allBounds;
	return allBounds;
}

void Section::setSectionCells(int div_x, int div_y, int pad_x, int pad_y, int innerPad_x, int innerPad_y)
{
	sectionCells = getComponentBounds(div_x, div_y, pad_x, pad_y, innerPad_x, innerPad_y);
}

juce::Array<Section*> Section::getAllSubContainers(int div_x, int div_y, int pad_x, int pad_y, int innerPad_x, int innerPad_y)
{
	// returns an array of sub-sections for each cell
	juce::Array<juce::Rectangle<int>> mainContentCells = getComponentBounds(div_x, div_y, pad_x, pad_y, innerPad_x, innerPad_y);
	
	juce::Array<Section*> subSections;
	
	for(juce::Rectangle<int> cell : mainContentCells)
	{
		Section* newSection = new Section();
		newSection -> setBounds(cell);
		subSections.add(newSection);
	}
	return subSections;
}
