//
//  Section.cpp
//  JT-SessionManager - App
//
//  Created by Carl Moore on 2/19/21.
//

#include "Section.hpp"


Section::Section(int div_x, int div_y, int pad_x, int pad_y, int innerPad_x, int innerPad_y)
{
	mDivX = div_x;
	mDivY = div_y;
	mPadX = pad_x;
	mPadY = pad_y;
	mInnerPadX = innerPad_x;
	mInnerPadY = innerPad_y;
}

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
	
//	auto area = getBoundsInParent();
	auto area = getLocalBounds();
	area.reduce(pad_x, pad_y);
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
			
			thisCell.reduce(innerPad_x, innerPad_y);
			
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
		Section* newSection = new Section(4);
		newSection -> setBounds(cell);
		subSections.add(newSection);
	}
	return subSections;
}


juce::Array<juce::Rectangle<int>> Section::getContainerCells(juce::Rectangle<int> area, int div_x, int div_y, int pad_x, int pad_y, int innerPad_x, int innerPad_y)
{
	assert(div_x > 0 && div_y > 0);
	
	juce::Array<juce::Rectangle<int>> cells;
	
	area.reduce(pad_x, pad_y);
	int cellWidth = area.getWidth() / div_x;
	int cellHeight = area.getHeight() / div_y;
	
//	FIX THIS SO THAT ROWS ARE CREATED FIRST
//	for (int y=0; y<div_y; y++)
//	{
//		juce::Rectangle<int> row = area;
//
//		row.removeFromTop(cellHeight * (div_y - (y + 1)));
////		row.removeFromBott
////		row.removeFromBottom(cellHeight * (div_y * (div_y - y)));
//
//	}
	
	for(int i = 0; i < div_x; i++)
	{
		juce::Rectangle<int> thisColumn = area;

		thisColumn.removeFromRight(cellWidth * (div_x - (i + 1)));
		thisColumn.removeFromLeft(cellWidth * i);
		
		for(int j = 0; j < div_y; j++)
		{
			juce::Rectangle<int> thisCell = thisColumn;
			thisCell.removeFromTop(j * cellHeight);
			thisCell.setHeight(cellHeight);
			thisCell.reduce(innerPad_x, innerPad_y);
			
			cells.add(thisCell);
		}
	}
	return cells;
}

void Section::updateSectionCells(juce::Rectangle<int> container)
{
	sectionCells = getContainerCells(container, mDivX, mDivY, mPadX, mPadY, mInnerPadX, mInnerPadY);
}
