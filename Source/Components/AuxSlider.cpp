/*
  ==============================================================================

    AuxSlider.cpp
    Created: 14 Mar 2023 12:19:14am
    Author:  satya

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AuxSlider.h"

//==============================================================================
AuxSlider::AuxSlider(juce::AudioParameterFloat* sliderParameter)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(slider);
    slider.setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::transparentBlack);
    slider.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::transparentBlack);
    slider.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::transparentBlack);
    slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::transparentBlack);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    this->sliderParameter = sliderParameter;
    slider.setRange(this->sliderParameter->range.start,this->sliderParameter->range.end,0.01);
    slider.setValue(*sliderParameter);
    slider.onValueChange = [this]
    {
        *this->sliderParameter = slider.getValue();
        normalizedValue = *this->sliderParameter / (slider.getRange().getEnd() - slider.getRange().getStart());
        repaint();
    };


    
}

AuxSlider::~AuxSlider()
{
}

void AuxSlider::setFeel(const Feel& feel)
{
    this->feel = feel;
}

void AuxSlider::setText(const juce::String& text)
{
    this->text = text;
}

void AuxSlider::setPrefix(const juce::String& prefix)
{
    this->prefix = prefix;
}

void AuxSlider::setSuffix(const juce::String& suffix)
{
    this->suffix = suffix;
}

void AuxSlider::setFontSize(const uint32_t& fontSize)
{
    this->fontSize = fontSize;
}

void AuxSlider::setInterval(const float& interval)
{
    this->interval = interval;
    slider.setRange(this->sliderParameter->range.start, this->sliderParameter->range.end, this->interval);

}

void AuxSlider::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */
    
    g.fillAll(juce::Colours::transparentBlack);

 /*   g.setColour(juce::Colours::white);
    g.drawRect(this->getLocalBounds(), 1);*/

    g.setColour(juce::Colours::yellow);
    g.setFont(juce::Font("Papyrus", fontSize, 1));
    g.drawText(prefix + " " + this->text + " : " + juce::String(normalizedValue * (sliderParameter->range.end - sliderParameter->range.start)) + suffix, this->getWidth() / 4, this->getHeight() / 2, this->getWidth() / 2, this->getHeight() / 10, juce::Justification::horizontallyCentred, false);

    if(feel == Feel::mixDial)
        LookandFeel::drawArc(g, this->getWidth()/2, this->getHeight(), this->getBounds().getWidth(), this->getBounds().getHeight(), this->normalizedValue);
    else if (feel = Feel::filterDial)
        LookandFeel::drawCircle(g, this->getLocalBounds().getCentreX(), this->getLocalBounds().getCentreY(), this->getLocalBounds().getWidth(), this->getLocalBounds().getHeight(), this->normalizedValue);
    
}

void AuxSlider::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    auto x = this->getWidth() / 10;
    auto y = this->getHeight() / 10;

    slider.setBounds(0, 0, 10 * x, 10 * y);
}

void LookandFeel::drawArc(juce::Graphics& g, const double& x, const double& y, const double& width, const double& height, const double& normalizedValue)
{
    juce::Path backgroundPath;
    float startAngle = juce::MathConstants<float>::pi*3.5;
    float endAngle = juce::MathConstants<float>::pi*4.5;
    float toAngle = startAngle + normalizedValue * (endAngle - startAngle);
    float radius = juce::jmin(width, height);
    radius -= 20;
    juce::Path strokedPath;
    juce::Path sliderPath;
    backgroundPath.addCentredArc(x, y, radius, radius, 0.0f, startAngle, endAngle, true);

    sliderPath.addCentredArc(x, y, radius, radius, 0.0f, startAngle, toAngle, true);
    strokedPath.addCentredArc(x, y, radius - 15, radius - 15, 0.0f, startAngle, toAngle, true);
    g.setColour(juce::Colour::fromRGBA(18,18,18,150));
    g.strokePath(backgroundPath, juce::PathStrokeType(25, juce::PathStrokeType::curved));

    g.setColour(juce::Colour::fromRGBA(252,2,232,120));
    g.strokePath(sliderPath, juce::PathStrokeType(25,juce::PathStrokeType::curved));

    g.setColour(juce::Colour::fromRGBA(252, 2, 232, 255));
    g.strokePath(strokedPath,juce::PathStrokeType(5, juce::PathStrokeType::curved));

    strokedPath.addCentredArc(x, y, radius + 15, radius + 15, 0.0f, startAngle, toAngle, true);
    g.strokePath(strokedPath, juce::PathStrokeType(5, juce::PathStrokeType::curved));



}

void LookandFeel::drawCircle(juce::Graphics& g, const double& x, const double& y, const double& width, const double& height, const double& normalizedValue)
{
    juce::Path backgroundPath;
    float startAngle = 0;
    float endAngle = juce::MathConstants<float>::pi * 2;
    float toAngle = startAngle + normalizedValue * (endAngle - startAngle);
    float radius = juce::jmin(width / 2, height / 2);
    radius -= 5;
    juce::Path sliderPath;
    backgroundPath.addCentredArc(x, y, radius, radius, 0.0f, startAngle, endAngle, true);
    sliderPath.addCentredArc(x, y, radius, radius, 0.0f, startAngle, toAngle, true);

    g.setColour(juce::Colour::fromRGBA(18, 18, 18, 150));
    g.strokePath(backgroundPath, juce::PathStrokeType(5, juce::PathStrokeType::curved));

    g.setColour(juce::Colour::fromRGBA(252, 2, 232, 255));
    g.strokePath(sliderPath, juce::PathStrokeType(5, juce::PathStrokeType::curved));
}
