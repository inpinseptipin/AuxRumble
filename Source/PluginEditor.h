/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Components/AuxSlider.h"
#include "Components/AuxOption.h"

//==============================================================================
/**
*/
class AuxRumbleAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    AuxRumbleAudioProcessorEditor (AuxRumbleAudioProcessor&);
    ~AuxRumbleAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AuxRumbleAudioProcessor& audioProcessor;
    AuxSlider mixSlider;
    AuxSlider crossoverSlider;
    AuxSlider intensitySlider;
    AuxOption optionMenu;
    juce::Image background;
    juce::Image logo;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AuxRumbleAudioProcessorEditor)
};
