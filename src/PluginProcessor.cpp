/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AuxRumbleAudioProcessor::AuxRumbleAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    crossoverFc = new juce::AudioParameterFloat("CFC", "CFC", 60, 480, 120);
    crossoverQ = new juce::AudioParameterFloat("CQ", "CQ", 0.707, 10, 0.707);
    bandpassFc = new juce::AudioParameterFloat("BFC", "BFC", 100, 1500, 1000);
    bandpassQ = new juce::AudioParameterFloat("BQ", "BQ", 0.707, 10, 2);
    mix = new juce::AudioParameterFloat("Mix", "Mix", 0, 100, 50);
    optionParameter = new juce::AudioParameterInt("Option", "Option", 1, 3, 1);
    parameterMap.addAudioProcessor(this);
    parameterMap.addParameter(crossoverFc);
    parameterMap.addParameter(crossoverQ);
    parameterMap.addParameter(bandpassFc);
    parameterMap.addParameter(bandpassQ);
    parameterMap.addParameter(mix);
    parameterMap.addParameter(optionParameter);
  
}

AuxRumbleAudioProcessor::~AuxRumbleAudioProcessor()
{
}

//==============================================================================
const juce::String AuxRumbleAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AuxRumbleAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AuxRumbleAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AuxRumbleAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AuxRumbleAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AuxRumbleAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AuxRumbleAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AuxRumbleAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AuxRumbleAudioProcessor::getProgramName (int index)
{
    return {};
}

void AuxRumbleAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AuxRumbleAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    if (lowPass.size() != this->getNumOutputChannels())
    {
        lowPass.resize(this->getNumOutputChannels());
        highPass.resize(this->getNumOutputChannels());
        highpassOutput.resize(this->getNumOutputChannels());
    }

    for (uint32_t i = 0; i < lowPass.size(); i++)
    {
        lowPass[i].prepareToPlay(*crossoverFc, *bandpassQ, sampleRate, AuxRumble::IIR::Type::Lowpass);
        highPass[i].prepareToPlay(*crossoverFc, *bandpassQ, sampleRate, AuxRumble::IIR::Type::Highpass);
    }
    bandPass.prepareToPlay(*crossoverFc, *bandpassQ, sampleRate, AuxRumble::IIR::Type::Bandpass);

 
}

void AuxRumbleAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AuxRumbleAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AuxRumbleAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.


    

    for (uint32_t i = 0; i < lowPass.size(); i++)
    {
        lowPass[i].prepareToPlay(*crossoverFc, *bandpassQ, this->getSampleRate(), AuxRumble::IIR::Type::Lowpass);
        highPass[i].prepareToPlay(*crossoverFc, *bandpassQ, this->getSampleRate(), AuxRumble::IIR::Type::Highpass);
        highpassOutput[i] = 0;
    }

    


    for (uint32_t i = 0; i < buffer.getNumSamples(); i++)
    {
        lowpassOutput = 0;
        
       
            
        for (uint32_t j = 0; j < buffer.getNumChannels(); j++)
        {
            lowpassOutput += lowPass[j].processSample(buffer.getSample(j, i));
            highpassOutput[j] = highPass[j].processSample(buffer.getSample(j, i));
        }
        lowpassOutput /= buffer.getNumChannels();
        lowpassOutput = atanf(5**bandpassQ/2*lowpassOutput);

        lowpassOutput = bandPass.processSample(lowpassOutput);


        for (uint32_t j = 0; j < buffer.getNumChannels(); j++)
        {
            if(*optionParameter == 1)
                buffer.setSample(j, i, (lowpassOutput + highpassOutput[j] + buffer.getSample(j,i)) * *mix / 100);
            if (*optionParameter == 3)
                buffer.setSample(j, i, (lowpassOutput + highpassOutput[j]) * *mix / 100);
        }
    }
}

//==============================================================================
bool AuxRumbleAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AuxRumbleAudioProcessor::createEditor()
{
    return new AuxRumbleAudioProcessorEditor (*this);
}

//==============================================================================
void AuxRumbleAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    parameterMap.getStateInformation(destData);
}

void AuxRumbleAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    parameterMap.setStateInformation(data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AuxRumbleAudioProcessor();
}
