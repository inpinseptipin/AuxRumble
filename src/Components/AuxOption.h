/*
  ==============================================================================

    AuxOption.h
    Created: 14 Mar 2023 6:35:20pm
    Author:  satya

  ==============================================================================
*/

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

//==============================================================================
/*
*/
class AuxOption  : public juce::Component
{
public:
    AuxOption(juce::AudioParameterInt* optionParameter);
    ~AuxOption() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void setOptions(const std::vector<juce::String>& options);
private:
    std::vector<juce::String> optionVector;
    juce::AudioParameterInt* optionParameter;
    juce::OwnedArray<juce::TextButton> buttonArray;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AuxOption)
};
