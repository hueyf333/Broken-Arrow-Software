/*
  ==============================================================================

    Broken Arrow M900 - Marshall JCM900-Style Amp Plugin
    Copyright (c) 2026 Broken Arrow Software

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BrokenArrowM900AudioProcessor::BrokenArrowM900AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
#else
    :
#endif
    apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    // Preallocate DSP modules
    inputStage = std::make_unique<InputStage>();
    preampStage = std::make_unique<PreampStage>();
    toneStack = std::make_unique<ToneStack>();
    powerAmp = std::make_unique<PowerAmp>();
    cabinetSim = std::make_unique<CabinetSimulator>();
    noiseGen = std::make_unique<NoiseGenerator>();
    outputStage = std::make_unique<OutputStage>();
}

BrokenArrowM900AudioProcessor::~BrokenArrowM900AudioProcessor()
{
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout BrokenArrowM900AudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // Input Stage
    layout.add(std::make_unique<juce::AudioParameterFloat>("inputTrim", "Input Trim",
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f, "dB"));
    layout.add(std::make_unique<juce::AudioParameterBool>("gateEnabled", "Gate Enabled", false));
    layout.add(std::make_unique<juce::AudioParameterFloat>("gateThreshold", "Gate Threshold",
        juce::NormalisableRange<float>(-60.0f, 0.0f, 0.1f), -40.0f, "dB"));

    // Preamp
    layout.add(std::make_unique<juce::AudioParameterFloat>("preampGain", "Preamp Gain",
        juce::NormalisableRange<float>(0.0f, 10.0f, 0.01f), 5.0f));
    layout.add(std::make_unique<juce::AudioParameterChoice>("mode", "Mode",
        juce::StringArray{ "Clean", "Crunch", "High Gain" }, 1));

    // Tone Stack
    layout.add(std::make_unique<juce::AudioParameterFloat>("bass", "Bass",
        juce::NormalisableRange<float>(0.0f, 10.0f, 0.01f), 5.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("mid", "Mid",
        juce::NormalisableRange<float>(0.0f, 10.0f, 0.01f), 5.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("treble", "Treble",
        juce::NormalisableRange<float>(0.0f, 10.0f, 0.01f), 5.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("presence", "Presence",
        juce::NormalisableRange<float>(0.0f, 10.0f, 0.01f), 5.0f));

    // Power Amp
    layout.add(std::make_unique<juce::AudioParameterFloat>("master", "Master",
        juce::NormalisableRange<float>(0.0f, 10.0f, 0.01f), 5.0f));
    layout.add(std::make_unique<juce::AudioParameterChoice>("sagMode", "Sag Mode",
        juce::StringArray{ "Authentic", "Tight" }, 0));

    // Cabinet Sim
    layout.add(std::make_unique<juce::AudioParameterBool>("cabEnabled", "Cab Enabled", true));
    layout.add(std::make_unique<juce::AudioParameterChoice>("irSelection", "IR Selection",
        juce::StringArray{ "4x12 Vintage", "4x12 Modern", "2x12 Combo" }, 0));

    // Output
    layout.add(std::make_unique<juce::AudioParameterFloat>("output", "Output",
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f, "dB"));

    // Oversampling
    layout.add(std::make_unique<juce::AudioParameterChoice>("oversampling", "Oversampling",
        juce::StringArray{ "1x", "2x", "4x" }, 0));

    // Noise
    layout.add(std::make_unique<juce::AudioParameterBool>("noiseEnabled", "Noise Enabled", false));
    layout.add(std::make_unique<juce::AudioParameterFloat>("noiseLevel", "Noise Level",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.1f));

    return layout;
}

//==============================================================================
void BrokenArrowM900AudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32>(getTotalNumOutputChannels());

    // Prepare all DSP modules
    inputStage->prepare(spec);
    preampStage->prepare(spec);
    toneStack->prepare(spec);
    powerAmp->prepare(spec);
    cabinetSim->prepare(spec);
    noiseGen->prepare(spec);
    outputStage->prepare(spec);

    // Setup oversampling (default to 1x, can be changed)
    oversampler = std::make_unique<juce::dsp::Oversampling<float>>(
        static_cast<size_t>(getTotalNumOutputChannels()),
        2, // 4x oversampling stages
        juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR,
        true, // use steep filter
        false // don't normalize gain
    );
    oversampler->initProcessing(static_cast<size_t>(samplesPerBlock));

    // Preallocate oversampled buffer
    oversampledBuffer.setSize(getTotalNumOutputChannels(), samplesPerBlock * 4);
}

void BrokenArrowM900AudioProcessor::releaseResources()
{
    oversampler->reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BrokenArrowM900AudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // Support mono and stereo
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void BrokenArrowM900AudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any output channels that don't have input
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Update parameters from APVTS
    updateDSPParameters();

    // Get input level for metering
    float inputRMS = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
    inputLevel.store(juce::Decibels::gainToDecibels(inputRMS, -60.0f), std::memory_order_relaxed);

    // Check if we need to change oversampling factor
    int newOversamplingFactor = 1 << apvts.getRawParameterValue("oversampling")->load();
    if (newOversamplingFactor != currentOversamplingFactor)
    {
        // Crossfade or safe re-init (for now, just switch immediately)
        currentOversamplingFactor = newOversamplingFactor;
    }

    // Process with or without oversampling
    if (currentOversamplingFactor > 1)
    {
        // Upsample
        juce::dsp::AudioBlock<float> block(buffer);
        auto oversampledBlock = oversampler->processSamplesUp(block);

        // Process through DSP chain
        inputStage->process(oversampledBlock);
        preampStage->process(oversampledBlock);
        toneStack->process(oversampledBlock);
        powerAmp->process(oversampledBlock);

        // Add noise if enabled
        if (apvts.getRawParameterValue("noiseEnabled")->load() > 0.5f)
            noiseGen->process(oversampledBlock);

        // Cabinet sim
        if (apvts.getRawParameterValue("cabEnabled")->load() > 0.5f)
            cabinetSim->process(oversampledBlock);

        outputStage->process(oversampledBlock);

        // Downsample
        oversampler->processSamplesDown(block);
    }
    else
    {
        // Process at 1x sample rate
        juce::dsp::AudioBlock<float> block(buffer);

        inputStage->process(block);
        preampStage->process(block);
        toneStack->process(block);
        powerAmp->process(block);

        // Add noise if enabled
        if (apvts.getRawParameterValue("noiseEnabled")->load() > 0.5f)
            noiseGen->process(block);

        // Cabinet sim
        if (apvts.getRawParameterValue("cabEnabled")->load() > 0.5f)
            cabinetSim->process(block);

        outputStage->process(block);
    }

    // Get output level for metering and clip detection
    float outputRMS = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
    float outputPeak = buffer.getMagnitude(0, 0, buffer.getNumSamples());
    outputLevel.store(juce::Decibels::gainToDecibels(outputRMS, -60.0f), std::memory_order_relaxed);
    isClipping.store(outputPeak > 0.99f, std::memory_order_relaxed);
}

void BrokenArrowM900AudioProcessor::updateDSPParameters()
{
    // Update all DSP modules with current parameter values
    inputStage->setInputTrim(apvts.getRawParameterValue("inputTrim")->load());
    inputStage->setGateEnabled(apvts.getRawParameterValue("gateEnabled")->load() > 0.5f);
    inputStage->setGateThreshold(apvts.getRawParameterValue("gateThreshold")->load());

    preampStage->setGain(apvts.getRawParameterValue("preampGain")->load());
    preampStage->setMode(static_cast<int>(apvts.getRawParameterValue("mode")->load()));

    toneStack->setBass(apvts.getRawParameterValue("bass")->load());
    toneStack->setMid(apvts.getRawParameterValue("mid")->load());
    toneStack->setTreble(apvts.getRawParameterValue("treble")->load());
    toneStack->setPresence(apvts.getRawParameterValue("presence")->load());

    powerAmp->setMaster(apvts.getRawParameterValue("master")->load());
    powerAmp->setSagMode(static_cast<int>(apvts.getRawParameterValue("sagMode")->load()));

    cabinetSim->setIRSelection(static_cast<int>(apvts.getRawParameterValue("irSelection")->load()));

    outputStage->setOutputGain(apvts.getRawParameterValue("output")->load());

    noiseGen->setLevel(apvts.getRawParameterValue("noiseLevel")->load());
}

//==============================================================================
bool BrokenArrowM900AudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* BrokenArrowM900AudioProcessor::createEditor()
{
    return new BrokenArrowM900AudioProcessorEditor(*this);
}

//==============================================================================
void BrokenArrowM900AudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void BrokenArrowM900AudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
const juce::String BrokenArrowM900AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BrokenArrowM900AudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool BrokenArrowM900AudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

int BrokenArrowM900AudioProcessor::getNumPrograms()
{
    return 1;
}

int BrokenArrowM900AudioProcessor::getCurrentProgram()
{
    return 0;
}

void BrokenArrowM900AudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String BrokenArrowM900AudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void BrokenArrowM900AudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BrokenArrowM900AudioProcessor();
}
