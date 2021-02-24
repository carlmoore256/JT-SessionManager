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

	juce::Colour color;
private:
	
	int mDivX, mDivY;
	int mPadX, mPadY;
	int mInnerPadX, mInnerPadY;
	
	juce::Array<juce::Rectangle<int>> containerCells;

};



