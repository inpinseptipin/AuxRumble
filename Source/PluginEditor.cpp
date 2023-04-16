/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AuxRumbleAudioProcessorEditor::AuxRumbleAudioProcessorEditor(AuxRumbleAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), 
    mixSlider(audioProcessor.parameterMap.getFloatParameter("Mix")),
    crossoverSlider(audioProcessor.parameterMap.getFloatParameter("CFC")),
    intensitySlider(audioProcessor.parameterMap.getFloatParameter("BQ")),
    optionMenu(audioProcessor.parameterMap.getIntParameter("Option"))
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);
    mixSlider.setFeel(AuxSlider::Feel::mixDial);
    mixSlider.setPrefix("");
    mixSlider.setSuffix("%");
    mixSlider.setText("Mix");
    mixSlider.setFontSize(36);
    mixSlider.setInterval(0.1);
    addAndMakeVisible(mixSlider);

    crossoverSlider.setFeel(AuxSlider::Feel::filterDial);
    crossoverSlider.setPrefix("");
    crossoverSlider.setSuffix("Hz");
    crossoverSlider.setText("Fc");
    crossoverSlider.setFontSize(26);
    crossoverSlider.setInterval(1);
    addAndMakeVisible(crossoverSlider);

    intensitySlider.setFeel(AuxSlider::Feel::filterDial);
    intensitySlider.setPrefix("");
    intensitySlider.setSuffix("");
    intensitySlider.setText("Q");
    intensitySlider.setFontSize(28);
    intensitySlider.setInterval(0.05);
    addAndMakeVisible(intensitySlider);

    optionMenu.setOptions({ "Mix","Dry","Wet" });
    addAndMakeVisible(optionMenu);

    
}

AuxRumbleAudioProcessorEditor::~AuxRumbleAudioProcessorEditor()
{
}

//==============================================================================
void AuxRumbleAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colour::fromRGBA(11, 12, 13, 255));
    background = juce::ImageCache::getFromMemory(BinaryData::background_jpg, BinaryData::background_jpgSize);
    logo = juce::ImageCache::getFromMemory(BinaryData::logo_png, BinaryData::logo_pngSize);
    juce::Rectangle<float> rect;
    rect.setX(this->getBounds().getX());
    rect.setY(this->getBounds().getY());
    rect.setWidth(this->getBounds().getWidth());
    rect.setHeight(this->getBounds().getHeight());
    g.drawImage(background,rect,juce::RectanglePlacement::stretchToFit,false);
    rect.setX(2.75*this->getLocalBounds().getWidth()/10);
    rect.setY(3 * this->getLocalBounds().getHeight() / 10);
    rect.setWidth(5*this->getLocalBounds().getWidth()/10);
    rect.setHeight(4*this->getLocalBounds().getHeight() / 10);
    g.drawImage(logo, rect, juce::RectanglePlacement::centred, false);
}

void AuxRumbleAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto x = this->getWidth() / 10;
    auto y = this->getHeight() / 10;
    mixSlider.setBounds(2*x,6*y,6*x,4*y);
    crossoverSlider.setBounds(0, 0, 3 * x, 5 * y);
    optionMenu.setBounds(8 * x, 6 * y, 2 * x, 4 * y);
    intensitySlider.setBounds(7 * x, 0, 3 * x, 5 * y);
}
