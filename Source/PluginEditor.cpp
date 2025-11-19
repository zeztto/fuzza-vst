#include "PluginEditor.h"
#include "PluginProcessor.h"

FuzzaAudioProcessorEditor::FuzzaAudioProcessorEditor(FuzzaAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p) {

  // === KNOBS (3x rotary) ===
  auto setupKnob = [this](juce::Slider& slider, juce::Label& label,
                          const juce::String& labelText) {
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xffff9500)); // Orange
    slider.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    slider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    slider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0x00000000)); // Transparent
    slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colour(0x00000000)); // Remove border
    addAndMakeVisible(slider);

    label.setText(labelText, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, juce::Colours::white);
    label.setFont(juce::FontOptions(18.0f, juce::Font::bold)); // Increased to 18 for better readability
    addAndMakeVisible(label);
  };

  setupKnob(gainSlider, gainLabel, "GAIN");
  setupKnob(mixSlider, mixLabel, "MIX");
  setupKnob(gateSlider, gateLabel, "GATE");

  gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      audioProcessor.apvts, "GAIN", gainSlider);
  mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      audioProcessor.apvts, "MIX", mixSlider);
  gateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      audioProcessor.apvts, "GATE", gateSlider);

  // === TONE PRESET BUTTONS ===
  auto setupToneButton = [this](juce::TextButton& button, const juce::String& text, int preset) {
    button.setButtonText(text);
    button.setClickingTogglesState(false);
    button.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff3a3a3a));
    button.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffff9500)); // Orange when selected
    button.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    button.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    button.onClick = [this, preset]() {
      audioProcessor.apvts.getParameter("TONE_PRESET")->setValueNotifyingHost(preset / 2.0f);
      updateToneButtons(preset);
    };
    addAndMakeVisible(button);
  };

  setupToneButton(toneWarmButton, "WARM", 0);
  setupToneButton(toneBalancedButton, "BALANCED", 1);
  setupToneButton(toneBrightButton, "BRIGHT", 2);

  toneLabel.setText("TONE", juce::dontSendNotification);
  toneLabel.setJustificationType(juce::Justification::centred);
  toneLabel.setColour(juce::Label::textColourId, juce::Colours::white);
  toneLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
  addAndMakeVisible(toneLabel);

  // === CLIPPING MODE BUTTONS ===
  auto setupClipButton = [this](juce::TextButton& button, const juce::String& text, int mode) {
    button.setButtonText(text);
    button.setClickingTogglesState(false);
    button.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff3a3a3a));
    button.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffff9500)); // Orange when selected
    button.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    button.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    button.onClick = [this, mode]() {
      audioProcessor.apvts.getParameter("CLIP_MODE")->setValueNotifyingHost(mode / 2.0f);
      updateClipButtons(mode);
    };
    addAndMakeVisible(button);
  };

  setupClipButton(clipHardButton, "HARD", 0);
  setupClipButton(clipSoftButton, "SOFT", 1);
  setupClipButton(clipAsymButton, "ASYM", 2);

  clipLabel.setText("CLIPPING", juce::dontSendNotification);
  clipLabel.setJustificationType(juce::Justification::centred);
  clipLabel.setColour(juce::Label::textColourId, juce::Colours::white);
  clipLabel.setFont(juce::FontOptions(14.0f, juce::Font::bold));
  addAndMakeVisible(clipLabel);

  // === BYPASS FOOTSWITCH ===
  bypassButton.setButtonText("BYPASS");
  bypassButton.setClickingTogglesState(true);
  bypassButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff333333));
  bypassButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffdd0000)); // Red when on
  bypassButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
  bypassButton.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
  addAndMakeVisible(bypassButton);

  bypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
      audioProcessor.apvts, "BYPASS", bypassButton);

  // Initialize button states
  updateToneButtons(1); // Balanced default
  updateClipButtons(0); // Hard default

  setSize(520, 570);
}

FuzzaAudioProcessorEditor::~FuzzaAudioProcessorEditor() {}

void FuzzaAudioProcessorEditor::updateToneButtons(int selectedPreset) {
  toneWarmButton.setToggleState(selectedPreset == 0, juce::dontSendNotification);
  toneBalancedButton.setToggleState(selectedPreset == 1, juce::dontSendNotification);
  toneBrightButton.setToggleState(selectedPreset == 2, juce::dontSendNotification);
}

void FuzzaAudioProcessorEditor::updateClipButtons(int selectedMode) {
  clipHardButton.setToggleState(selectedMode == 0, juce::dontSendNotification);
  clipSoftButton.setToggleState(selectedMode == 1, juce::dontSendNotification);
  clipAsymButton.setToggleState(selectedMode == 2, juce::dontSendNotification);
}

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
  auto logoArea = bounds.removeFromTop(70);
  logoArea.removeFromTop(5); // Small top padding
  g.setColour(juce::Colours::white);
  g.setFont(juce::FontOptions(48.0f, juce::Font::bold));
  g.drawFittedText("FUZZA", logoArea, juce::Justification::centred, 1);

  // Subtitle
  g.setFont(juce::FontOptions(16.0f));
  g.setColour(juce::Colour(0xffff9500)); // Orange
  g.drawFittedText("Modern Fuzz Pedal", logoArea.removeFromBottom(18),
                   juce::Justification::centred, 1);

  // Draw knob module boxes (3 boxes for visual grouping)
  auto knobBoxBounds = getLocalBounds().reduced(25);
  knobBoxBounds.removeFromTop(80 + 15 + 70 + 25 + 70 + 35); // Skip to knob area

  int boxWidth = (knobBoxBounds.getWidth() - 40) / 3; // 3 boxes with spacing
  int boxHeight = 175; // Label + knob + value
  int spacing = 20;

  auto drawKnobBox = [&g](int x, int y, int w, int h) {
    // Draw subtle box with rounded corners
    juce::Rectangle<float> box(x, y, w, h);
    g.setColour(juce::Colour(0x33ffffff)); // Semi-transparent white
    g.drawRoundedRectangle(box, 8.0f, 1.5f);

    // Subtle inner glow
    g.setColour(juce::Colour(0x11ffffff));
    g.fillRoundedRectangle(box.reduced(1), 7.0f);
  };

  int startX = knobBoxBounds.getX();
  int startY = knobBoxBounds.getY();

  drawKnobBox(startX, startY, boxWidth, boxHeight);
  drawKnobBox(startX + boxWidth + spacing, startY, boxWidth, boxHeight);
  drawKnobBox(startX + (boxWidth + spacing) * 2, startY, boxWidth, boxHeight);

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
  auto bounds = getLocalBounds().reduced(25);

  // Unified grid parameters
  int columnWidth = (bounds.getWidth() - 40) / 3; // 3 columns
  int columnSpacing = 20; // Uniform spacing between columns

  // Top: Logo area
  bounds.removeFromTop(80);
  bounds.removeFromTop(15); // Space after logo

  // Tone preset buttons row - using unified grid
  auto toneRow = bounds.removeFromTop(70);
  toneLabel.setBounds(toneRow.removeFromTop(25));
  auto toneButtons = toneRow.removeFromTop(40);

  toneWarmButton.setBounds(toneButtons.removeFromLeft(columnWidth));
  toneButtons.removeFromLeft(columnSpacing);
  toneBalancedButton.setBounds(toneButtons.removeFromLeft(columnWidth));
  toneButtons.removeFromLeft(columnSpacing);
  toneBrightButton.setBounds(toneButtons);

  bounds.removeFromTop(25); // Space between button rows

  // Clipping mode buttons row - using unified grid
  auto clipRow = bounds.removeFromTop(70);
  clipLabel.setBounds(clipRow.removeFromTop(25));
  auto clipButtons = clipRow.removeFromTop(40);

  clipHardButton.setBounds(clipButtons.removeFromLeft(columnWidth));
  clipButtons.removeFromLeft(columnSpacing);
  clipSoftButton.setBounds(clipButtons.removeFromLeft(columnWidth));
  clipButtons.removeFromLeft(columnSpacing);
  clipAsymButton.setBounds(clipButtons);

  bounds.removeFromTop(35); // More space before knobs

  // Knob module boxes - using unified grid
  auto knobModuleArea = bounds.removeFromTop(175);

  // GAIN module
  auto gainModule = knobModuleArea.removeFromLeft(columnWidth);
  gainLabel.setBounds(gainModule.removeFromTop(24));
  gainModule.removeFromTop(3);
  gainSlider.setBounds(gainModule.reduced(12, 0));

  knobModuleArea.removeFromLeft(columnSpacing);

  // MIX module
  auto mixModule = knobModuleArea.removeFromLeft(columnWidth);
  mixLabel.setBounds(mixModule.removeFromTop(24));
  mixModule.removeFromTop(3);
  mixSlider.setBounds(mixModule.reduced(12, 0));

  knobModuleArea.removeFromLeft(columnSpacing);

  // GATE module
  auto gateModule = knobModuleArea;
  gateLabel.setBounds(gateModule.removeFromTop(24));
  gateModule.removeFromTop(3);
  gateSlider.setBounds(gateModule.reduced(12, 0));

  bounds.removeFromTop(35); // More space above bypass

  // Bypass footswitch (bottom center with vertical centering)
  auto bypassArea = bounds.removeFromTop(70);
  bypassButton.setBounds(bypassArea.withSizeKeepingCentre(150, 48));
}
