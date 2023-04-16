/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AuxParam.h"
#include "AuxIIR.h"

//==============================================================================
/**
*/
class AuxRumbleAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    AuxRumbleAudioProcessor();
    ~AuxRumbleAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    AuxPort::ParameterMap parameterMap;
private:
    //==============================================================================
    juce::AudioParameterFloat* crossoverFc;
    juce::AudioParameterFloat* crossoverQ;
    juce::AudioParameterFloat* bandpassFc;
    juce::AudioParameterFloat* bandpassQ;
    juce::AudioParameterFloat* mix;
    juce::AudioParameterInt* optionParameter;


    std::vector<AuxPort::IIR::Butterworth> lowPass;
    std::vector<AuxPort::IIR::Butterworth> highPass;
    AuxPort::IIR::Butterworth bandPass;


  

    float lowpassOutput = 0;
    std::vector<float> highpassOutput;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AuxRumbleAudioProcessor)
};
