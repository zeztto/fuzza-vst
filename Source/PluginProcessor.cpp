#include "PluginProcessor.h"
#include "PluginEditor.h"

FuzzaAudioProcessor::FuzzaAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(
          BusesProperties()
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
#endif
      apvts(*this, nullptr, "Parameters", createParameterLayout()) {
}

FuzzaAudioProcessor::~FuzzaAudioProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout
FuzzaAudioProcessor::createParameterLayout() {
  juce::AudioProcessorValueTreeState::ParameterLayout layout;

  layout.add(std::make_unique<juce::AudioParameterBool>(
      juce::ParameterID{"BYPASS", 1}, "Bypass", false));

  layout.add(std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"GAIN", 1}, "Gain",
      juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f, 0.5f), 50.0f));

  // Tone: Realistic fuzz pedal range (500Hz - 5000Hz)
  // Higher values = brighter tone, lower values = darker/warmer tone
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"TONE", 1}, "Tone",
      juce::NormalisableRange<float>(500.0f, 5000.0f, 1.0f, 0.3f), 2500.0f));

  // Mix: Dry/Wet blend (0% = dry, 100% = wet)
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"MIX", 1}, "Mix",
      juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 100.0f));

  // Gate: Noise gate threshold (0% = off, higher = more aggressive)
  layout.add(std::make_unique<juce::AudioParameterFloat>(
      juce::ParameterID{"GATE", 1}, "Gate",
      juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f));

  // Clipping mode: 0 = Hard, 1 = Soft, 2 = Asymmetric
  layout.add(std::make_unique<juce::AudioParameterChoice>(
      juce::ParameterID{"CLIP_MODE", 1}, "Clip Mode",
      juce::StringArray{"Hard", "Soft", "Asymmetric"}, 0));

  return layout;
}

const juce::String FuzzaAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool FuzzaAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool FuzzaAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool FuzzaAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double FuzzaAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int FuzzaAudioProcessor::getNumPrograms() {
  return 1; // NB: some hosts don't cope very well if you tell them there are 0
            // programs, so this should be at least 1, even if you're not really
            // implementing programs.
}

int FuzzaAudioProcessor::getCurrentProgram() { return 0; }

void FuzzaAudioProcessor::setCurrentProgram(int index) {}

const juce::String FuzzaAudioProcessor::getProgramName(int index) { return {}; }

void FuzzaAudioProcessor::changeProgramName(int index,
                                            const juce::String &newName) {}

void FuzzaAudioProcessor::prepareToPlay(double sampleRate,
                                        int samplesPerBlock) {
  // Initialize tone filters (low-pass filters)
  toneFilterLeft.reset();
  toneFilterRight.reset();

  toneFilterLeft.prepare({sampleRate, (juce::uint32)samplesPerBlock, 1});
  toneFilterRight.prepare({sampleRate, (juce::uint32)samplesPerBlock, 1});

  // Set filter type to low-pass
  toneFilterLeft.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
  toneFilterRight.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
}

void FuzzaAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FuzzaAudioProcessor::isBusesLayoutSupported(
    const BusesLayout &layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  // Some plugin hosts, such as certain GarageBand versions, will only
  // load plugins that support stereo bus layouts.
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

  // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}
#endif

void FuzzaAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                       juce::MidiBuffer &midiMessages) {
  juce::ScopedNoDenormals noDenormals;
  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

  // In case we have more outputs than inputs, this code clears any output
  // channels that didn't contain input data, (because these aren't
  // guaranteed to be empty - they may contain garbage).
  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  // Check bypass state
  auto bypass = apvts.getRawParameterValue("BYPASS")->load() > 0.5f;

  // If bypassed, skip all processing
  if (bypass)
    return;

  auto gainParam = apvts.getRawParameterValue("GAIN")->load();
  auto tone = apvts.getRawParameterValue("TONE")->load();
  auto mixParam = apvts.getRawParameterValue("MIX")->load() / 100.0f; // 0.0 ~ 1.0
  auto gateParam = apvts.getRawParameterValue("GATE")->load() / 100.0f; // 0.0 ~ 1.0
  auto clipMode = static_cast<int>(apvts.getRawParameterValue("CLIP_MODE")->load());

  // Auto-compensation algorithm
  float inputGain = juce::jmax(1.0f, gainParam);  // Minimum 1x (clean)
  float makeupGain = 2.0f / std::sqrt(inputGain); // Sqrt-based compensation

  // Gate threshold (0.0 = no gate, 0.1 = aggressive)
  float gateThreshold = gateParam * 0.1f;

  // Update tone filter cutoff frequency
  toneFilterLeft.setCutoffFrequency(tone);
  toneFilterRight.setCutoffFrequency(tone);

  // Fuzz Algorithm with Mix, Gate, and Multiple Clipping Modes
  for (int channel = 0; channel < totalNumInputChannels; ++channel) {
    auto *channelData = buffer.getWritePointer(channel);

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
      float dry = channelData[sample]; // Store dry signal
      float wet = dry;

      // Gate: Kill signal below threshold
      if (std::abs(wet) < gateThreshold) {
        wet = 0.0f;
      } else {
        // Apply input gain
        wet *= inputGain;

        // Clipping modes
        switch (clipMode) {
          case 0: // Hard clipping
            wet = juce::jlimit(-1.0f, 1.0f, wet);
            break;

          case 1: // Soft clipping (tanh)
            wet = std::tanh(wet);
            break;

          case 2: // Asymmetric clipping (vintage style)
            if (wet > 0.0f)
              wet = std::tanh(wet * 1.5f);
            else
              wet = juce::jlimit(-1.0f, 1.0f, wet);
            break;
        }

        // Apply tone filter (low-pass)
        if (channel == 0)
          wet = toneFilterLeft.processSample(0, wet);
        else if (channel == 1)
          wet = toneFilterRight.processSample(0, wet);

        // Apply automatic makeup gain
        wet *= makeupGain;
      }

      // Mix dry and wet signals
      channelData[sample] = (dry * (1.0f - mixParam)) + (wet * mixParam);
    }
  }
}

bool FuzzaAudioProcessor::hasEditor() const {
  return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *FuzzaAudioProcessor::createEditor() {
  return new FuzzaAudioProcessorEditor(*this);
}

void FuzzaAudioProcessor::getStateInformation(juce::MemoryBlock &destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries
  auto state = apvts.copyState();
  std::unique_ptr<juce::XmlElement> xml(state.createXml());
  copyXmlToBinary(*xml, destData);
}

void FuzzaAudioProcessor::setStateInformation(const void *data,
                                              int sizeInBytes) {
  // You should use this method to restore your parameters from this memory
  // block, whose contents will have been created by the getStateInformation()
  // call.
  std::unique_ptr<juce::XmlElement> xmlState(
      getXmlFromBinary(data, sizeInBytes));
  if (xmlState.get() != nullptr)
    if (xmlState->hasTagName(apvts.state.getType()))
      apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new FuzzaAudioProcessor();
}
