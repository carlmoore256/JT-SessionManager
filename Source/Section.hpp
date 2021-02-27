//
//  Section.hpp
//  JT-SessionManager - App
//
//  Created by Carl Moore on 2/19/21.
//
#pragma once
//
//#ifndef Section_hpp
//#define Section_hpp

#include <JuceHeader.h>
#include <stdio.h>

//#endif /* Section_hpp */

class Section : public juce::Component
{
public:
	Section(int div_x=1, int div_y=1, int pad_x=0, int pad_y=0, int innerPad_x=0, int innerPad_y=0);
	~Section();

	//	MAKE THIS PRIVATE
	juce::Array<juce::Rectangle<int>> sectionCells;

	//	void paint(Graphics&) override;
	void resized() override;
	
	void setProperties(juce::Colour _color = juce::Colours::lightgrey);
	
	juce::Array<juce::Rectangle<int>> getComponentBounds(int div_x, int div_y, int pad_x, int pad_y, int innerPad_x=0, int innerPad_y=0);

	void setSectionCells(int div_x, int div_y, int pad_x, int pad_y, int innerPad_x=0, int innerPad_y=0);

	//	given a Rectangle<int>, return an array of sub rectangles
	juce::Array<juce::Rectangle<int>> getContainerCells(juce::Rectangle<int> area, int div_x, int div_y, int pad_x, int pad_y, int innerPad_x, int innerPad_y);
	
	juce::Array<Section*> getAllSubContainers(int, int, int, int, int, int);
	
	//	updates containerCells given a rect container (usually getLocalBounds())
	void updateSectionCells(juce::Rectangle<int> container);
	
	juce::Array<juce::Rectangle<int>> getRowCells(int index)
	{
		juce::Array<juce::Rectangle<int>> row;
		for (int i = 0; i < mDivX; i++)
			row[i] = containerCells[index + i];
		return row;
	}
	
	//	index into sectionCells and get the cell at the specified row and col
	juce::Rectangle<int> getCellAt(int row, int column)
	{
		jassert(row <= mDivX && column <= mDivY);
		return sectionCells[(row * mDivX) + column];
	}
	
	//	get an entire row merged
	juce::Rectangle<int> getMergedRow(int row)
	{
		juce::Rectangle<int> rowMerged;
		
		if(mDivX > 0)
		{
			rowMerged = sectionCells[row*mDivX];
			rowMerged = rowMerged.getUnion(sectionCells[(row*mDivX)+mDivX-1]);
			
		} else { rowMerged = sectionCells[row]; }
		
		return rowMerged;
	}
	
	//	provide a range of rows to merge
	juce::Rectangle<int> getMergedRows(int rowStart, int rowEnd)
	{
		jassert(rowStart < rowEnd && rowEnd <= mDivY * mDivX);
		return getMergedRow(rowStart).getUnion(getMergedRow(rowEnd));
	}
	
	
	juce::Colour color;
private:
	
	int mDivX, mDivY;
	int mPadX, mPadY;
	int mInnerPadX, mInnerPadY;
	
	
	juce::Array<juce::Rectangle<int>> containerCells;

};



