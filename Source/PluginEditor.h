#pragma once

#include "PluginProcessor.h"
#include <juce_audio_processors/juce_audio_processors.h>

class FuzzaAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
  FuzzaAudioProcessorEditor(FuzzaAudioProcessor &);
  ~FuzzaAudioProcessorEditor() override;

  void paint(juce::Graphics &) override;
  void resized() override;

private:
  FuzzaAudioProcessor &audioProcessor;

  juce::ToggleButton bypassButton;
  juce::Slider gainSlider;
  juce::Slider toneSlider;

  juce::Label gainLabel;
  juce::Label toneLabel;

  std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>
      bypassAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      gainAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      toneAttachment;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FuzzaAudioProcessorEditor)
};
