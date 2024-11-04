/*
  ==============================================================================

    AuxSlider.h
    Created: 14 Mar 2023 12:19:14am
    Author:  satya

  ==============================================================================
*/

#pragma once

#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

//==============================================================================
/*
*/
class AuxSlider  : public juce::Component
{
public:
    enum Feel
    {
        mixDial,filterDial
    };
    AuxSlider(juce::AudioParameterFloat* sliderParameter);
    ~AuxSlider() override;
    void setFeel(const Feel& feel);
    void setText(const juce::String& text);
    void setPrefix(const juce::String& prefix);
    void setSuffix(const juce::String& suffix);
    void setFontSize(const uint32_t& fontSize);
    void setInterval(const float& interval);
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider slider;
    Feel feel;
    float normalizedValue = 0;
    float interval;
    juce::String text;
    juce::String suffix;
    juce::String prefix;
    uint32_t fontSize;
    juce::AudioParameterFloat* sliderParameter;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AuxSlider)
};

class LookandFeel
{
public:
    static void drawArc(juce::Graphics& g, const double& x, const double& y, const double& width, const double& height, const double& normalizedValue);
    static void drawCircle(juce::Graphics& g, const double& x, const double& y, const double& width, const double& height, const double& normalizedValue);
};