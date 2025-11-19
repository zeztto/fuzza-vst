#include "PluginEditor.h"
#include "PluginProcessor.h"

FuzzaAudioProcessorEditor::FuzzaAudioProcessorEditor(FuzzaAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p) {

  // === KNOBS (4x rotary) ===
  auto setupKnob = [this](juce::Slider& slider, juce::Label& label,
                          const juce::String& labelText, const juce::String& paramID) {
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xffff9500)); // Orange
    slider.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    slider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    slider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0x00000000)); // Transparent
    addAndMakeVisible(slider);

    label.setText(labelText, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, juce::Colours::white);
    label.setFont(juce::Font(14.0f, juce::Font::bold));
    label.attachToComponent(&slider, false);
    addAndMakeVisible(label);
  };

  setupKnob(gainSlider, gainLabel, "GAIN", "GAIN");
  setupKnob(toneSlider, toneLabel, "TONE", "TONE");
  setupKnob(mixSlider, mixLabel, "MIX", "MIX");
  setupKnob(gateSlider, gateLabel, "GATE", "GATE");

  gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      audioProcessor.apvts, "GAIN", gainSlider);
  toneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      audioProcessor.apvts, "TONE", toneSlider);
  mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      audioProcessor.apvts, "MIX", mixSlider);
  gateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      audioProcessor.apvts, "GATE", gateSlider);

  // === CLIPPING MODE SELECTOR ===
  clipModeSelector.addItem("Hard", 1);
  clipModeSelector.addItem("Soft", 2);
  clipModeSelector.addItem("Asymmetric", 3);
  clipModeSelector.setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xff2a2a2a));
  clipModeSelector.setColour(juce::ComboBox::textColourId, juce::Colours::white);
  clipModeSelector.setColour(juce::ComboBox::outlineColourId, juce::Colour(0xff666666));
  clipModeSelector.setColour(juce::ComboBox::arrowColourId, juce::Colours::orange);
  addAndMakeVisible(clipModeSelector);

  clipModeLabel.setText("CLIPPING", juce::dontSendNotification);
  clipModeLabel.setJustificationType(juce::Justification::centred);
  clipModeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
  clipModeLabel.setFont(juce::Font(12.0f, juce::Font::bold));
  addAndMakeVisible(clipModeLabel);

  clipModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
      audioProcessor.apvts, "CLIP_MODE", clipModeSelector);

  // === BYPASS FOOTSWITCH (Pedal Style) ===
  bypassButton.setButtonText("BYPASS");
  bypassButton.setClickingTogglesState(true);
  bypassButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff333333));
  bypassButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffdd0000)); // Red when on
  bypassButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
  bypassButton.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
  addAndMakeVisible(bypassButton);

  bypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
      audioProcessor.apvts, "BYPASS", bypassButton);

  setSize(500, 400);
}

FuzzaAudioProcessorEditor::~FuzzaAudioProcessorEditor() {}

void FuzzaAudioProcessorEditor::paint(juce::Graphics &g) {
  // Pedal enclosure background (dark grey with gradient)
  auto bounds = getLocalBounds();
  juce::ColourGradient gradient(
      juce::Colour(0xff1a1a1a), bounds.getX(), bounds.getY(),
      juce::Colour(0xff2d2d2d), bounds.getX(), bounds.getBottom(),
      false);
  g.setGradientFill(gradient);
  g.fillAll();

  // Border (metallic look)
  g.setColour(juce::Colour(0xff555555));
  g.drawRect(bounds, 3);

  // Top logo area
  auto logoArea = bounds.removeFromTop(60);
  g.setColour(juce::Colours::white);
  g.setFont(juce::Font(36.0f, juce::Font::bold));
  g.drawFittedText("FUZZA", logoArea, juce::Justification::centred, 1);

  // Subtitle
  g.setFont(juce::Font(12.0f));
  g.setColour(juce::Colour(0xffff9500)); // Orange
  g.drawFittedText("Modern Fuzz Pedal", logoArea.removeFromBottom(15),
                   juce::Justification::centred, 1);

  // Draw screws (pedal realism)
  auto drawScrew = [&g](int x, int y) {
    g.setColour(juce::Colour(0xff888888));
    g.fillEllipse(x - 4, y - 4, 8, 8);
    g.setColour(juce::Colour(0xff444444));
    g.drawLine(x - 3, y, x + 3, y, 1.0f);
  };

  drawScrew(15, 15);
  drawScrew(getWidth() - 15, 15);
  drawScrew(15, getHeight() - 15);
  drawScrew(getWidth() - 15, getHeight() - 15);
}

void FuzzaAudioProcessorEditor::resized() {
  auto bounds = getLocalBounds().reduced(20);

  // Top: Logo area
  bounds.removeFromTop(60);
  bounds.removeFromTop(10); // Spacing

  // Clipping mode selector (top center)
  auto clipArea = bounds.removeFromTop(30);
  clipModeSelector.setBounds(clipArea.withSizeKeepingCentre(180, 25));
  bounds.removeFromTop(5);

  // Knobs area (4 knobs in a row)
  auto knobArea = bounds.removeFromTop(140);
  int knobWidth = knobArea.getWidth() / 4;

  gainSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(15, 0));
  toneSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(15, 0));
  mixSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(15, 0));
  gateSlider.setBounds(knobArea.removeFromLeft(knobWidth).reduced(15, 0));

  bounds.removeFromTop(20); // Spacing

  // Bypass footswitch (bottom center, pedal style)
  auto bypassArea = bounds.removeFromBottom(60);
  bypassButton.setBounds(bypassArea.withSizeKeepingCentre(100, 50));
}
