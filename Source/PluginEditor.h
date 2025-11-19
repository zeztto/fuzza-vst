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

  // Knobs
  juce::Slider gainSlider;
  juce::Slider toneSlider;
  juce::Slider mixSlider;
  juce::Slider gateSlider;

  // Labels
  juce::Label gainLabel;
  juce::Label toneLabel;
  juce::Label mixLabel;
  juce::Label gateLabel;

  // Bypass footswitch button (pedal style)
  juce::TextButton bypassButton;

  // Clipping mode selector
  juce::ComboBox clipModeSelector;
  juce::Label clipModeLabel;

  // Attachments
  std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>
      bypassAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      gainAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      toneAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      mixAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
      gateAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>
      clipModeAttachment;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FuzzaAudioProcessorEditor)
};
