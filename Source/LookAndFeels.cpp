#include "LookAndFeels.h"

//-------------------------------------------------------------------------------------------------------
// BlinkingToggleButton
//-------------------------------------------------------------------------------------------------------

void BlinkingToggleButton::paint(juce::Graphics &g)
{
	if (getState() == juce::Button::ButtonState::buttonDown && !getToggleState())
	{
		g.setColour(isEnabled() ? juce::Colours::white : juce::Colours::white.darker(0.8f));
		g.drawRect(getLocalBounds().reduced(2, 2), 1.0f);
		g.fillRect(getLocalBounds().reduced(4, 4));

		g.setColour(juce::Colour::fromRGB(0x20, 0x20, 0x20));

		g.setFont(8);
		g.drawText(getButtonText(), getLocalBounds(), juce::Justification::centred);
		return;
	}


	if (getToggleState() && m_isBlinked)
	{
		g.setColour(juce::Colour::fromRGB(0xcf, 0x62, 0x1d)); // FilterVisualizer::m_outWaveformColor
		g.fillRect(getLocalBounds().reduced(4, 4));
		g.setColour(juce::Colour::fromRGB(0x20, 0x20, 0x20));
	}

	g.setColour(isEnabled() ? juce::Colours::white : juce::Colours::white.darker(0.8f));
	g.drawRect(getLocalBounds().reduced(2, 2), 1.0f);

	g.setFont(8);
	g.drawText(getButtonText(), getLocalBounds(), juce::Justification::centred);
}

//-------------------------------------------------------------------------------------------------------

void BlinkingToggleButton::buttonStateChanged()
{
	if (getToggleState())
	{
		if (isTimerRunning())
			return;
		startTimerHz(2);
		m_isBlinked = true;
	}
	else
	{
		stopTimer();
		m_isBlinked = false;
	}
	repaint();
}

//-------------------------------------------------------------------------------------------------------

void BlinkingToggleButton::timerCallback()
{
	m_isBlinked = !m_isBlinked;
	repaint();
}

//-------------------------------------------------------------------------------------------------------

BlinkingToggleButton::BlinkingToggleButton()
{
}

//-------------------------------------------------------------------------------------------------------
// OrangeGateLookAndFeel
//-------------------------------------------------------------------------------------------------------

void OrangeGateLookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider &slider)
{
    auto bounds = slider.getLocalBounds();
    auto arcRadius = std::min(bounds.getWidth(), bounds.getHeight()) / 2.0f - 5.0f;

	float rotaryAngle = juce::MathConstants<float>::pi * 0.6f;
    auto toAngle = -rotaryAngle + sliderPos * (rotaryAngle * 2.0f);

	// backplate
	juce::Path backgroundPath;
	backgroundPath.addCentredArc(
		bounds.getCentreX(),
		bounds.getCentreY(),
		arcRadius + 5.0f,
		arcRadius + 5.0f,
		0.0f,
		-juce::MathConstants<float>::halfPi,
		juce::MathConstants<float>::halfPi,
		true
	);
	auto taperBounds = bounds.withTrimmedTop(bounds.getHeight() / 2).withTrimmedBottom(5);
	taperBounds = taperBounds.withSizeKeepingCentre(2 * ((int)arcRadius + 5), taperBounds.getHeight());

	backgroundPath.addRoundedRectangle<int>(taperBounds, 5.0f);
	backgroundPath.addRectangle<int>(taperBounds.removeFromTop(20));
	
	g.setColour(Theme::BackPlateColor);
	g.fillPath(backgroundPath);

	// rail
    juce::Path backgroundArc;
    backgroundArc.addCentredArc(
		bounds.getCentreX(),
        bounds.getCentreY(),
        arcRadius,
        arcRadius,
        0.0f,
        -rotaryAngle,
        rotaryAngle,
        true);

    g.setColour(Theme::RailPaddingColor);
    g.strokePath(backgroundArc, juce::PathStrokeType(5.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
	g.setColour(Theme::RailColor);
    g.strokePath(backgroundArc, juce::PathStrokeType(1.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

	// thumb
	juce::Line<float> thumbLine{
		bounds.getCentre().getPointOnCircumference(arcRadius - 5.0f, toAngle),
		bounds.getCentre().getPointOnCircumference(arcRadius + 5.0f, toAngle)
	};
	
	g.setColour(Theme::RailColor);
	g.drawLine(thumbLine, 10.0f);
	g.setColour(juce::Colours::white);
	g.drawLine(thumbLine, 8.0f);

	// label
	auto textBounds = slider.getLocalBounds().removeFromBottom(slider.getHeight() / 2);

	g.setFont(8);
	juce::String val;
	if(slider.getNumDecimalPlacesToDisplay() == 0)
		val = juce::String((int)slider.getValue()) + slider.getTextValueSuffix();
	else
		val = juce::String(slider.getValue(), 2) + slider.getTextValueSuffix();

	g.drawText(val, textBounds.removeFromTop(10), juce::Justification::centred);

	g.setFont(juce::Font(font));
	g.setFont(12);
	juce::String name = slider.getName();
	g.drawText(name, textBounds.removeFromTop(15), juce::Justification::centred);
}

//-------------------------------------------------------------------------------------------------------
void OrangeGateLookAndFeel::drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider &slider)
{
	float sliderOffset{ 5.0f };

	float sliderWidth = slider.getWidth();
	float sliderHeight = slider.getHeight() - (sliderOffset * 2.0f);
	float thumbPosRatio = 1.0f - (slider.getValue() - slider.getRange().getStart()) / (slider.getRange().getEnd() - slider.getRange().getStart());

	// backplate
	juce::Point<float> a{ sliderWidth / 2.0f, sliderOffset };
	juce::Point<float> b{ sliderWidth / 2.0f , sliderHeight + sliderOffset };
	juce::Path railPath;
	railPath.startNewSubPath(a.toFloat());
	railPath.lineTo(b.toFloat());

	g.setColour(Theme::BackPlateColor);
    g.strokePath(railPath, juce::PathStrokeType(10.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));


	// rail
	g.setColour(Theme::RailPaddingColor);
    g.strokePath(railPath, juce::PathStrokeType(5.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
	g.setColour(Theme::RailColor);
	g.drawLine({ a, b }, 1.0f);

	//thumb
	if (slider.isEnabled())
	{
		juce::Point<float> c{ 5.0f, thumbPosRatio * sliderHeight + sliderOffset };
		juce::Point<float> d{ sliderWidth - 5.0f , thumbPosRatio * sliderHeight + sliderOffset };
		g.setColour(Theme::RailColor);
		g.drawLine({ c, d }, 10.0f);
		g.setColour(juce::Colours::white);
		g.drawLine({ c, d }, 8.0f);
	}
}

//-------------------------------------------------------------------------------------------------------
// VisualizerLookAndFeel
//-------------------------------------------------------------------------------------------------------

void VisualizerLookAndFeel::drawLinearSlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider &slider)
{
	g.setColour(juce::Colours::white);

	float sliderWidth = slider.getWidth() - 2;
	float sliderHeight = slider.getHeight();
	float thumbPosRatio = (slider.getValue() - slider.getRange().getStart()) / (slider.getRange().getEnd() - slider.getRange().getStart());

	// rail
	juce::Point<float> a{ 1.0f, sliderHeight / 2.0f };
	juce::Point<float> b{ sliderWidth, sliderHeight / 2.0f };
	g.drawLine({ a, b });

	//thumb
	juce::Point<float> c{ thumbPosRatio * sliderWidth, 5.0f };
	juce::Point<float> d{ thumbPosRatio * sliderWidth, sliderHeight - 5.0f };
	g.drawLine({ c, d });
}

//-------------------------------------------------------------------------------------------------------

void VisualizerLookAndFeel::drawToggleButton(juce::Graphics &g, juce::ToggleButton &button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
	g.setColour(button.isEnabled() ? juce::Colours::white : juce::Colours::white.darker(0.8f));
	
	g.drawRect(button.getLocalBounds().reduced(2, 2), 1.0f);

	if (button.getToggleState())
	{
		g.fillRect(button.getLocalBounds().reduced(4, 4));
		g.setColour(juce::Colour::fromRGB(0x20, 0x20, 0x20));
	}

	g.setFont(8);
	g.drawText(button.getButtonText(), button.getLocalBounds(), juce::Justification::centred);

}

//-------------------------------------------------------------------------------------------------------

void VisualizerLookAndFeel::drawComboBox(juce::Graphics &g, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox &cbx)
{
	g.setColour(cbx.isEnabled() ? juce::Colours::white : juce::Colours::white.darker(0.8f));

	auto bounds = cbx.getLocalBounds().reduced(2, 2);
	
	g.drawRect(bounds, 1.0f);

	// arrow
	auto arrowBounds = bounds.removeFromRight(15).reduced(5, 4);
	auto a = arrowBounds.getTopLeft();
	auto c = arrowBounds.getTopRight();
	auto b = arrowBounds.removeFromRight(arrowBounds.getWidth() / 2).getBottomLeft();

	g.drawLine(juce::Line<int>(a, b).toFloat(), 1.0f);
	g.drawLine(juce::Line<int>(b, c).toFloat(), 1.0f);
}

//-------------------------------------------------------------------------------------------------------

void VisualizerLookAndFeel::drawPopupMenuItem(juce::Graphics &g, const juce::Rectangle<int> &area, bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu, const juce::String &text, const juce::String &shortcutKeyText, const juce::Drawable *icon, const juce::Colour *textColour)
{
	g.setColour(juce::Colour::fromRGB(0x10, 0x10, 0x10));
	g.fillRect(area);

	g.setColour(juce::Colours::white);
	g.setFont(8);
	g.drawText(text, area, juce::Justification::centred);
}

//-------------------------------------------------------------------------------------------------------

void VisualizerLookAndFeel::drawPopupMenuBackground(juce::Graphics &g, int width, int height)
{
	juce::Rectangle<int> bounds(width, height);
	g.setColour(juce::Colour::fromRGB(0x10, 0x10, 0x10));
	g.fillRect(bounds);
}

//-------------------------------------------------------------------------------------------------------

void VisualizerLookAndFeel::positionComboBoxText(juce::ComboBox &c, juce::Label &l)
{
	l.setFont(juce::Font(8));
	l.setJustificationType(juce::Justification::centred);
	l.setBounds(c.getLocalBounds());
}

//-------------------------------------------------------------------------------------------------------
