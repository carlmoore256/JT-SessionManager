//
//  Section.hpp
//  JT-SessionManager - App
//
//  Created by Carl Moore on 2/19/21.
//

#ifndef Section_hpp
#define Section_hpp

#include <JuceHeader.h>
#include <stdio.h>

#endif /* Section_hpp */

class Section : public juce::Component
{
public:
	Section();
	~Section();

	juce::Array<juce::Rectangle<int>> sectionCells;

	//	void paint(Graphics&) override;
	void resized() override;
	
	void setProperties(juce::Colour _color = juce::Colours::lightgrey);

	juce::Array<juce::Rectangle<int>> getComponentBounds(int div_x, int div_y, int pad_x, int pad_y, int innerPad_x=0, int innerPad_y=0);

	void setSectionCells(int div_x, int div_y, int pad_x, int pad_y, int innerPad_x=0, int innerPad_y=0);

	juce::Array<juce::Rectangle<int>> getSubContainers(juce::Rectangle<int>, int, int, int, int, int, int);

	juce::Array<Section*> getAllSubContainers(int, int, int, int, int, int);

	juce::Colour color;
private:
	juce::Array<juce::Rectangle<int>> containerCells;
};



