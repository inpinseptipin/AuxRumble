/*
  ==============================================================================

    AuxOption.cpp
    Created: 14 Mar 2023 6:35:20pm
    Author:  satya

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AuxOption.h"

//==============================================================================
AuxOption::AuxOption(juce::AudioParameterInt* optionParameter)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    this->optionParameter = optionParameter;

}

AuxOption::~AuxOption()
{
}

void AuxOption::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (juce::Colours::transparentBlack);   // clear the background
}

void AuxOption::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    auto x = this->getLocalBounds().getWidth() / 10;
    auto y = this->getLocalBounds().getHeight() / 10;

    if (optionVector.size() > 0)
    {
        juce::TextButton* textButton;
        auto rowHeight = (10*y) / optionVector.size();
        for (uint32_t i = 0; i < optionVector.size(); i++)
        {
            textButton = buttonArray.getUnchecked(i);
            textButton->setBounds(this->getLocalBounds().getX(), i * rowHeight, 10 * x, rowHeight);
        }
    }
    
}

void AuxOption::setOptions(const std::vector<juce::String>& options)
{
    optionVector = options;

    for (uint32_t i = 0; i < optionVector.size(); i++)
    {
        buttonArray.add(new juce::TextButton());
        juce::TextButton* textButton = buttonArray.getUnchecked(i);
        textButton->setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::transparentBlack);
        textButton->setColour(juce::TextButton::ColourIds::textColourOnId, juce::Colours::yellow);
        textButton->setColour(juce::TextButton::ColourIds::textColourOffId, juce::Colours::white);
        textButton->setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::transparentBlack);
        textButton->setButtonText(optionVector[i]);
        addAndMakeVisible(textButton);
        textButton->setToggleState(false, juce::dontSendNotification);
        textButton->onClick = [this, i, textButton]
        {
            if (!textButton->getToggleState())
            {
                textButton->setToggleState(true, juce::dontSendNotification);
                *optionParameter = i + 1;
            }
            for (uint32_t j = 0; j < optionVector.size(); j++)
            {
                if (j != i)
                {
                    auto button = buttonArray.getUnchecked(j);
                    button->setToggleState(false,false);
                }
            }
        };
    }
    buttonArray.getUnchecked(0)->setToggleState(true,false);
    resized();
}
