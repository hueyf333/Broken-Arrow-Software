#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SuperBassmanAudioProcessor::SuperBassmanAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::mono(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
       apvts (*this, nullptr, "Parameters", createParameterLayout())
{
}

SuperBassmanAudioProcessor::~SuperBassmanAudioProcessor()
{
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout SuperBassmanAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    // Input Stage
    layout.add (std::make_unique<juce::AudioParameterFloat> ("inputGain", "Input Gain", 
        juce::NormalisableRange<float> (-12.0f, 12.0f, 0.1f), 0.0f, "dB"));
    layout.add (std::make_unique<juce::AudioParameterBool> ("inputPad", "Input Pad", false));
    layout.add (std::make_unique<juce::AudioParameterBool> ("dcBlock", "DC Block", true));
    layout.add (std::make_unique<juce::AudioParameterBool> ("hpfEnable", "HPF Enable", true));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("hpfFreq", "HPF Frequency",
        juce::NormalisableRange<float> (20.0f, 200.0f, 1.0f), 40.0f, "Hz"));
    
    // Channel Selection
    layout.add (std::make_unique<juce::AudioParameterChoice> ("channel", "Channel",
        juce::StringArray { "Normal", "Bright", "Overdrive" }, 0));
    
    // Preamp Controls
    layout.add (std::make_unique<juce::AudioParameterFloat> ("preampGain", "Preamp Gain",
        juce::NormalisableRange<float> (0.0f, 10.0f, 0.1f), 5.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("preampBias", "Preamp Bias",
        juce::NormalisableRange<float> (-1.0f, 1.0f, 0.01f), 0.0f));
    
    // Tone Stack (Fender-style)
    layout.add (std::make_unique<juce::AudioParameterFloat> ("bass", "Bass",
        juce::NormalisableRange<float> (0.0f, 10.0f, 0.1f), 5.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("mid", "Mid",
        juce::NormalisableRange<float> (0.0f, 10.0f, 0.1f), 5.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("treble", "Treble",
        juce::NormalisableRange<float> (0.0f, 10.0f, 0.1f), 5.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("presence", "Presence",
        juce::NormalisableRange<float> (0.0f, 10.0f, 0.1f), 5.0f));
    
    // Overdrive Channel
    layout.add (std::make_unique<juce::AudioParameterFloat> ("drive", "Drive",
        juce::NormalisableRange<float> (0.0f, 10.0f, 0.1f), 0.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("driveBlend", "Drive Blend",
        juce::NormalisableRange<float> (0.0f, 100.0f, 1.0f), 0.0f, "%"));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("driveTone", "Drive Tone",
        juce::NormalisableRange<float> (0.0f, 10.0f, 0.1f), 5.0f));
    
    // Power Amp
    layout.add (std::make_unique<juce::AudioParameterFloat> ("masterVolume", "Master Volume",
        juce::NormalisableRange<float> (0.0f, 10.0f, 0.1f), 5.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("powerSag", "Power Sag",
        juce::NormalisableRange<float> (0.0f, 10.0f, 0.1f), 3.0f));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("powerCompression", "Power Compression",
        juce::NormalisableRange<float> (0.0f, 10.0f, 0.1f), 3.0f));
    
    // Cabinet Simulation
    layout.add (std::make_unique<juce::AudioParameterBool> ("cabEnable", "Cab Enable", true));
    layout.add (std::make_unique<juce::AudioParameterChoice> ("cabType", "Cab Type",
        juce::StringArray { "8x10 Sealed", "4x10 Ported", "1x15 Sealed" }, 0));
    layout.add (std::make_unique<juce::AudioParameterFloat> ("cabMix", "Cab Mix",
        juce::NormalisableRange<float> (0.0f, 100.0f, 1.0f), 100.0f, "%"));
    
    // Output Stage
    layout.add (std::make_unique<juce::AudioParameterFloat> ("outputGain", "Output Gain",
        juce::NormalisableRange<float> (-24.0f, 24.0f, 0.1f), 0.0f, "dB"));
    layout.add (std::make_unique<juce::AudioParameterBool> ("limiterEnable", "Limiter Enable", true));
    
    // Global
    layout.add (std::make_unique<juce::AudioParameterBool> ("bypass", "Bypass", false));
    
    return layout;
}

//==============================================================================
const juce::String SuperBassmanAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SuperBassmanAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SuperBassmanAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SuperBassmanAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SuperBassmanAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SuperBassmanAudioProcessor::getNumPrograms()
{
    return 1;
}

int SuperBassmanAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SuperBassmanAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SuperBassmanAudioProcessor::getProgramName (int index)
{
    return {};
}

void SuperBassmanAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SuperBassmanAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32> (samplesPerBlock);
    spec.numChannels = 2;
    
    inputStage.prepare (spec);
    channelRouting.prepare (spec);
    tubePreamp.prepare (spec);
    toneStack.prepare (spec);
    overdriveChannel.prepare (spec);
    powerAmp.prepare (spec);
    cabinetSim.prepare (spec);
    outputStage.prepare (spec);
    oversamplingEngine.prepare (spec);
}

void SuperBassmanAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SuperBassmanAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // Mono in, stereo out
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::mono())
        return false;

    return true;
  #endif
}
#endif

void SuperBassmanAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    // Get parameters
    bool bypass = apvts.getRawParameterValue ("bypass")->load();
    
    // Measure input level
    float inputPeak = buffer.getMagnitude (0, buffer.getNumSamples());
    inputLevel.store (inputPeak);
    inputClipping.store (inputPeak > 1.0f);
    
    if (bypass)
    {
        // In bypass, just pass through with output gain
        float outputGainDb = apvts.getRawParameterValue ("outputGain")->load();
        float outputGainLinear = juce::Decibels::decibelsToGain (outputGainDb);
        
        // If mono input, duplicate to stereo
        if (buffer.getNumChannels() == 1 && getTotalNumOutputChannels() == 2)
        {
            buffer.copyFrom (1, 0, buffer, 0, 0, buffer.getNumSamples());
        }
        
        buffer.applyGain (outputGainLinear);
    }
    else
    {
        // Process signal chain
        juce::dsp::AudioBlock<float> block (buffer);
        juce::dsp::ProcessContextReplacing<float> context (block);
        
        // Input stage
        inputStage.process (context, apvts);
        
        // Channel routing
        channelRouting.process (context, apvts);
        
        // Tube preamp with oversampling
        oversamplingEngine.processWithOversampling (context, [this] (juce::dsp::ProcessContextReplacing<float>& ctx) {
            tubePreamp.process (ctx, apvts);
        });
        
        // Tone stack
        toneStack.process (context, apvts);
        
        // Overdrive channel
        int channelType = static_cast<int> (apvts.getRawParameterValue ("channel")->load());
        if (channelType == 2) // Overdrive channel
        {
            oversamplingEngine.processWithOversampling (context, [this] (juce::dsp::ProcessContextReplacing<float>& ctx) {
                overdriveChannel.process (ctx, apvts);
            });
        }
        
        // Power amp with oversampling
        oversamplingEngine.processWithOversampling (context, [this] (juce::dsp::ProcessContextReplacing<float>& ctx) {
            powerAmp.process (ctx, apvts);
        });
        
        // Cabinet simulation
        cabinetSim.process (context, apvts);
        
        // Output stage
        outputStage.process (context, apvts);
        
        // If mono input, duplicate to stereo output
        if (buffer.getNumChannels() == 1 && getTotalNumOutputChannels() == 2)
        {
            buffer.copyFrom (1, 0, buffer, 0, 0, buffer.getNumSamples());
        }
    }
    
    // Measure output level
    float outputPeak = buffer.getMagnitude (0, buffer.getNumSamples());
    outputLevel.store (outputPeak);
    outputClipping.store (outputPeak > 1.0f);
}

//==============================================================================
bool SuperBassmanAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* SuperBassmanAudioProcessor::createEditor()
{
    return new SuperBassmanAudioProcessorEditor (*this);
}

//==============================================================================
void SuperBassmanAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void SuperBassmanAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SuperBassmanAudioProcessor();
}
