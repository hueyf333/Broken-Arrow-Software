#include "PluginProcessor.h"
#include "PluginEditor.h"

AmpegSVTAudioProcessor::AmpegSVTAudioProcessor()
    : AudioProcessor(BusesProperties()
                     .withInput("Input", juce::AudioChannelSet::mono(), true)
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout())
{
}

AmpegSVTAudioProcessor::~AmpegSVTAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout AmpegSVTAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    // Input section
    layout.add(std::make_unique<juce::AudioParameterFloat>("inputPad", "Input Pad", 
        juce::NormalisableRange<float>(-20.0f, 0.0f, 0.1f), 0.0f, "dB"));
    layout.add(std::make_unique<juce::AudioParameterFloat>("inputTrim", "Input Trim",
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f, "dB"));
    layout.add(std::make_unique<juce::AudioParameterBool>("gateEnable", "Gate Enable", false));
    layout.add(std::make_unique<juce::AudioParameterFloat>("gateThreshold", "Gate Threshold",
        juce::NormalisableRange<float>(-80.0f, -20.0f, 0.1f), -60.0f, "dB"));
    
    // Preamp section
    layout.add(std::make_unique<juce::AudioParameterFloat>("preampDrive", "Preamp Drive",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.3f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("preampBias", "Preamp Bias",
        juce::NormalisableRange<float>(-1.0f, 1.0f, 0.01f), 0.0f));
    
    // Tone stack
    layout.add(std::make_unique<juce::AudioParameterFloat>("bass", "Bass",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("mid", "Mid",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("treble", "Treble",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    layout.add(std::make_unique<juce::AudioParameterChoice>("midFreq", "Mid Frequency",
        juce::StringArray{"220 Hz", "450 Hz", "800 Hz", "1.6 kHz", "3 kHz"}, 2));
    
    // Voicing switches
    layout.add(std::make_unique<juce::AudioParameterBool>("brightSwitch", "Bright", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("ultraLo", "Ultra Lo", false));
    layout.add(std::make_unique<juce::AudioParameterBool>("ultraHi", "Ultra Hi", false));
    
    // Power amp
    layout.add(std::make_unique<juce::AudioParameterFloat>("powerDrive", "Power Drive",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("master", "Master",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.7f));
    
    // Cabinet
    layout.add(std::make_unique<juce::AudioParameterFloat>("cabMix", "Cab Mix",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f));
    
    // Output
    layout.add(std::make_unique<juce::AudioParameterFloat>("outputGain", "Output Gain",
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f, "dB"));
    layout.add(std::make_unique<juce::AudioParameterBool>("bypass", "Bypass", false));
    
    return layout;
}

const juce::String AmpegSVTAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AmpegSVTAudioProcessor::acceptsMidi() const
{
    return false;
}

bool AmpegSVTAudioProcessor::producesMidi() const
{
    return false;
}

bool AmpegSVTAudioProcessor::isMidiEffect() const
{
    return false;
}

double AmpegSVTAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AmpegSVTAudioProcessor::getNumPrograms()
{
    return 4; // Clean, Classic Grind, Pushed, Deep/Ultra-Lo
}

int AmpegSVTAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AmpegSVTAudioProcessor::setCurrentProgram(int index)
{
    // Load preset based on index
    switch (index)
    {
        case 0: // Clean
            apvts.getParameter("preampDrive")->setValueNotifyingHost(0.2f);
            apvts.getParameter("powerDrive")->setValueNotifyingHost(0.3f);
            apvts.getParameter("bass")->setValueNotifyingHost(0.6f);
            apvts.getParameter("mid")->setValueNotifyingHost(0.5f);
            apvts.getParameter("treble")->setValueNotifyingHost(0.5f);
            break;
        case 1: // Classic Grind
            apvts.getParameter("preampDrive")->setValueNotifyingHost(0.6f);
            apvts.getParameter("powerDrive")->setValueNotifyingHost(0.6f);
            apvts.getParameter("bass")->setValueNotifyingHost(0.5f);
            apvts.getParameter("mid")->setValueNotifyingHost(0.6f);
            apvts.getParameter("treble")->setValueNotifyingHost(0.6f);
            break;
        case 2: // Pushed Power Section
            apvts.getParameter("preampDrive")->setValueNotifyingHost(0.4f);
            apvts.getParameter("powerDrive")->setValueNotifyingHost(0.8f);
            apvts.getParameter("bass")->setValueNotifyingHost(0.6f);
            apvts.getParameter("mid")->setValueNotifyingHost(0.5f);
            apvts.getParameter("treble")->setValueNotifyingHost(0.4f);
            break;
        case 3: // Deep/Ultra-Lo
            apvts.getParameter("preampDrive")->setValueNotifyingHost(0.3f);
            apvts.getParameter("powerDrive")->setValueNotifyingHost(0.4f);
            apvts.getParameter("bass")->setValueNotifyingHost(0.7f);
            apvts.getParameter("mid")->setValueNotifyingHost(0.4f);
            apvts.getParameter("treble")->setValueNotifyingHost(0.3f);
            apvts.getParameter("ultraLo")->setValueNotifyingHost(1.0f);
            break;
    }
}

const juce::String AmpegSVTAudioProcessor::getProgramName(int index)
{
    switch (index)
    {
        case 0: return "Clean";
        case 1: return "Classic Grind";
        case 2: return "Pushed Power";
        case 3: return "Deep/Ultra-Lo";
        default: return {};
    }
}

void AmpegSVTAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    // Not implemented for factory presets
}

void AmpegSVTAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = 2;
    
    // Prepare all DSP modules
    inputStage.prepare(spec);
    tubePreamp.prepare(spec);
    toneStack.prepare(spec);
    powerAmp.prepare(spec);
    cabSimulator.prepare(spec);
    oversampler.prepare(spec);
    
    // Prepare limiter
    outputLimiter.prepare(spec);
    outputLimiter.setThreshold(0.0f);
    outputLimiter.setRelease(50.0f);
    
    // Setup parameter smoothing
    const float rampTime = 0.05f; // 50ms ramp
    inputGainSmooth.reset(sampleRate, rampTime);
    outputGainSmooth.reset(sampleRate, rampTime);
    driveSmooth.reset(sampleRate, rampTime);
    masterSmooth.reset(sampleRate, rampTime);
    bassSmooth.reset(sampleRate, rampTime);
    midSmooth.reset(sampleRate, rampTime);
    trebleSmooth.reset(sampleRate, rampTime);
    
    // Prepare bypass buffer
    bypassBuffer.setSize(2, samplesPerBlock);
}

void AmpegSVTAudioProcessor::releaseResources()
{
}

bool AmpegSVTAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // Mono in, stereo out
    if (layouts.getMainInputChannelSet() == juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo())
        return true;
    
    // Stereo in, stereo out also supported
    if (layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo()
        && layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo())
        return true;
    
    return false;
}

void AmpegSVTAudioProcessor::updateParameters()
{
    // Input stage
    inputStage.setPad(apvts.getRawParameterValue("inputPad")->load());
    inputStage.setTrim(apvts.getRawParameterValue("inputTrim")->load());
    inputStage.setGateEnabled(apvts.getRawParameterValue("gateEnable")->load() > 0.5f);
    inputStage.setGateThreshold(apvts.getRawParameterValue("gateThreshold")->load());
    
    // Preamp - with smoothing
    float preampDrive = apvts.getRawParameterValue("preampDrive")->load();
    driveSmooth.setTargetValue(preampDrive);
    tubePreamp.setDrive(driveSmooth.getNextValue());
    tubePreamp.setBias(apvts.getRawParameterValue("preampBias")->load());
    
    // Tone stack - with smoothing
    bassSmooth.setTargetValue(apvts.getRawParameterValue("bass")->load());
    midSmooth.setTargetValue(apvts.getRawParameterValue("mid")->load());
    trebleSmooth.setTargetValue(apvts.getRawParameterValue("treble")->load());
    
    toneStack.setBass(bassSmooth.getNextValue());
    toneStack.setMid(midSmooth.getNextValue());
    toneStack.setTreble(trebleSmooth.getNextValue());
    
    int midFreqIndex = apvts.getRawParameterValue("midFreq")->load();
    toneStack.setMidFrequency(static_cast<ToneStack::MidFrequency>(midFreqIndex));
    
    toneStack.setBrightSwitch(apvts.getRawParameterValue("brightSwitch")->load() > 0.5f);
    toneStack.setUltraLoSwitch(apvts.getRawParameterValue("ultraLo")->load() > 0.5f);
    toneStack.setUltraHiSwitch(apvts.getRawParameterValue("ultraHi")->load() > 0.5f);
    
    // Power amp - with smoothing
    float powerDrive = apvts.getRawParameterValue("powerDrive")->load();
    float master = apvts.getRawParameterValue("master")->load();
    masterSmooth.setTargetValue(master);
    
    powerAmp.setDrive(powerDrive);
    powerAmp.setMaster(masterSmooth.getNextValue());
    
    // Cabinet
    cabSimulator.setMix(apvts.getRawParameterValue("cabMix")->load());
    
    // Output gain
    float outputGainDb = apvts.getRawParameterValue("outputGain")->load();
    outputGainSmooth.setTargetValue(juce::Decibels::decibelsToGain(outputGainDb));
    
    // Bypass
    bypassed = apvts.getRawParameterValue("bypass")->load() > 0.5f;
}

void AmpegSVTAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // Clear unused output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    
    // Update parameters
    updateParameters();
    
    // Store input for bypass
    if (bypassed)
    {
        bypassBuffer.makeCopyOf(buffer);
    }
    
    // Measure input level
    float maxInputLevel = 0.0f;
    for (int ch = 0; ch < totalNumInputChannels; ++ch)
    {
        maxInputLevel = std::max(maxInputLevel, buffer.getMagnitude(ch, 0, buffer.getNumSamples()));
    }
    inputLevel.store(maxInputLevel);
    
    // Process audio
    if (!bypassed)
    {
        const int numSamples = buffer.getNumSamples();
        bool wasClipping = false;
        
        // Sum to mono if stereo input
        if (totalNumInputChannels == 2)
        {
            auto* leftData = buffer.getWritePointer(0);
            auto* rightData = buffer.getReadPointer(1);
            
            for (int i = 0; i < numSamples; ++i)
                leftData[i] = (leftData[i] + rightData[i]) * 0.5f;
        }
        
        // Process through signal chain
        auto* monoData = buffer.getWritePointer(0);
        
        for (int i = 0; i < numSamples; ++i)
        {
            float sample = monoData[i];
            
            // Input stage
            sample = inputStage.processSample(sample);
            
            // Tube preamp (with oversampling)
            sample = tubePreamp.processSample(sample);
            
            // Tone stack
            sample = toneStack.processSample(sample);
            
            // Power amp (with oversampling)
            sample = powerAmp.processSample(sample);
            
            // Check for clipping
            if (std::abs(sample) > 0.95f)
                wasClipping = true;
            
            monoData[i] = sample;
        }
        
        clipping.store(wasClipping);
        
        // Duplicate mono to stereo and apply cab simulation
        if (totalNumOutputChannels >= 2)
        {
            buffer.copyFrom(1, 0, buffer, 0, 0, numSamples);
            
            auto* leftData = buffer.getWritePointer(0);
            auto* rightData = buffer.getWritePointer(1);
            
            for (int i = 0; i < numSamples; ++i)
            {
                leftData[i] = cabSimulator.processSample(leftData[i], 0);
                rightData[i] = cabSimulator.processSample(rightData[i], 1);
            }
        }
        else
        {
            auto* monoDataCab = buffer.getWritePointer(0);
            for (int i = 0; i < numSamples; ++i)
            {
                monoDataCab[i] = cabSimulator.processSample(monoDataCab[i], 0);
            }
        }
        
        // Apply output gain
        float outputGain = outputGainSmooth.getNextValue();
        for (int ch = 0; ch < totalNumOutputChannels; ++ch)
        {
            auto* channelData = buffer.getWritePointer(ch);
            for (int i = 0; i < numSamples; ++i)
                channelData[i] *= outputGain;
        }
        
        // Output limiter for protection
        juce::dsp::AudioBlock<float> block(buffer);
        juce::dsp::ProcessContextReplacing<float> context(block);
        outputLimiter.process(context);
    }
    else
    {
        // Bypass - copy back the stored input
        buffer.makeCopyOf(bypassBuffer);
    }
    
    // Measure output level
    float maxOutputLevel = 0.0f;
    for (int ch = 0; ch < totalNumOutputChannels; ++ch)
    {
        maxOutputLevel = std::max(maxOutputLevel, buffer.getMagnitude(ch, 0, buffer.getNumSamples()));
    }
    outputLevel.store(maxOutputLevel);
}

bool AmpegSVTAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* AmpegSVTAudioProcessor::createEditor()
{
    return new juce::GenericAudioProcessorEditor(*this);
}

void AmpegSVTAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AmpegSVTAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmpegSVTAudioProcessor();
}
