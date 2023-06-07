#pragma once

//-------------------------------------------------------------------------------------------------------

#include "OrangeGate.h"

//-------------------------------------------------------------------------------------------------------

class BlinkingToggleButton
	: public juce::ToggleButton
	, private juce::Timer
{
private:
	void buttonStateChanged() override;
	void timerCallback() override;

	bool m_isBlinked{ false };

public:
	BlinkingToggleButton();

	void paint(juce::Graphics &g) override;
};

//-------------------------------------------------------------------------------------------------------

class OrangeGateLookAndFeel : public juce::LookAndFeel_V4
{
private:
	juce::Typeface::Ptr font = juce::Typeface::createSystemTypefaceFor(BinaryData::RosaSansRegular_ttf, BinaryData::RosaSansRegular_ttfSize);

public:
	void drawRotarySlider(juce::Graphics &g,
					      int  	x,
					      int  	y,
					      int  	width,
					      int  	height,
					      float sliderPosProportional,
					      float rotaryStartAngle,
					      float rotaryEndAngle,
					      juce::Slider &slider
	) override;

	void drawLinearSlider(juce::Graphics &g,
		int x,
		int y,
		int width, 
		int height,
		float sliderPos,
		float minSliderPos,
		float maxSliderPos,
		const juce::Slider::SliderStyle style, 
		juce::Slider &slider
	) override;
};

//-------------------------------------------------------------------------------------------------------

class VisualizerLookAndFeel : public juce::LookAndFeel_V4
{
public:
	void drawLinearSlider(juce::Graphics &g,
		int x,
		int y,
		int width,
		int height,
		float sliderPos,
		float minSliderPos,
		float maxSliderPos,
		const juce::Slider::SliderStyle style,
		juce::Slider &slider
	) override;

	void drawToggleButton(juce::Graphics &g,
		juce::ToggleButton &button,
		bool shouldDrawButtonAsHighlighted,
		bool shouldDrawButtonAsDown
	) override;

	void drawComboBox(
		juce::Graphics &g,
		int width,
		int height,
		bool isButtonDown,
		int buttonX,
		int buttonY,
		int buttonW,
		int buttonH,
		juce::ComboBox &cbx
	) override;

	void drawPopupMenuItem(
		juce::Graphics &g,
		const juce::Rectangle<int> &area,
		bool isSeparator,
		bool isActive,
		bool isHighlighted,
		bool isTicked,
		bool hasSubMenu, 
		const juce::String &text, 
		const juce::String &shortcutKeyText,
		const juce::Drawable *icon,
		const juce::Colour *textColour
	) override;

	void drawPopupMenuBackground(
		juce::Graphics &g,
		int width,
		int height
	) override;

	void positionComboBoxText(
		juce::ComboBox &c,
		juce::Label &l
	) override;
};

//-------------------------------------------------------------------------------------------------------

namespace Theme
{
	static const juce::Colour TextColor = juce::Colour(0xff, 0xff, 0xff);
	static const juce::Colour BackgroundColor = juce::Colour(0xff, 0x66, 0x19);
	static const juce::Colour ShadedBackgroundColor = juce::Colour(0x2e, 0x13, 0x05);
	static const juce::Colour AccentColor = juce::Colour(0xff, 0xff, 0xff);

	static const juce::Colour RailColor = juce::Colour(0x20, 0x20, 0x20);
	static const juce::Colour RailPaddingColor = juce::Colours::black.withAlpha(0.1f);
	static const juce::Colour BackPlateColor = juce::Colours::black.withAlpha(0.15f);
};

//-------------------------------------------------------------------------------------------------------
