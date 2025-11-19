#include "PluginEditor.h"
#include "PluginProcessor.h"

FuzzaAudioProcessorEditor::FuzzaAudioProcessorEditor(FuzzaAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p) {
  // Gain Slider
  gainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
  gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
  addAndMakeVisible(gainSlider);

  gainLabel.setText("Gain", juce::dontSendNotification);
  gainLabel.setJustificationType(juce::Justification::centred);
  gainLabel.attachToComponent(&gainSlider, false);
  addAndMakeVisible(gainLabel);

  gainAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.apvts, "GAIN", gainSlider);

  // Tone Slider
  toneSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
  toneSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
  addAndMakeVisible(toneSlider);

  toneLabel.setText("Tone", juce::dontSendNotification);
  toneLabel.setJustificationType(juce::Justification::centred);
  toneLabel.attachToComponent(&toneSlider, false);
  addAndMakeVisible(toneLabel);

  toneAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.apvts, "TONE", toneSlider);

  // Volume Slider
  volumeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
  volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
  addAndMakeVisible(volumeSlider);

  volumeLabel.setText("Volume", juce::dontSendNotification);
  volumeLabel.setJustificationType(juce::Justification::centred);
  volumeLabel.attachToComponent(&volumeSlider, false);
  addAndMakeVisible(volumeLabel);

  volumeAttachment =
      std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
          audioProcessor.apvts, "VOLUME", volumeSlider);

  setSize(400, 300);
}

FuzzaAudioProcessorEditor::~FuzzaAudioProcessorEditor() {}

void FuzzaAudioProcessorEditor::paint(juce::Graphics &g) {
  g.fillAll(juce::Colours::black);

  g.setColour(juce::Colours::white);
  g.setFont(30.0f);
  g.drawFittedText("Fuzza", getLocalBounds().removeFromTop(50),
                   juce::Justification::centred, 1);
}

void FuzzaAudioProcessorEditor::resized() {
  auto area = getLocalBounds().removeFromBottom(200);
  int dialWidth = area.getWidth() / 3;

  gainSlider.setBounds(area.removeFromLeft(dialWidth).reduced(10));
  toneSlider.setBounds(area.removeFromLeft(dialWidth).reduced(10));
  volumeSlider.setBounds(area.removeFromLeft(dialWidth).reduced(10));
}
