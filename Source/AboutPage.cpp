#include "AboutPage.h"
#include "LookAndFeels.h"

//-------------------------------------------------------------------------------------------------------

static const char *ABOUT_TEXT_LINE_1{ "Shhhhhhhh" };
static const std::size_t ABOUT_TEXT_SIZE_1{ 20 };

static const char *ABOUT_TEXT_LINE_2{ "Listen To Me" };
static const std::size_t ABOUT_TEXT_SIZE_2{ 17 };

static const char *ABOUT_TEXT_LINE_3{ "Vain Audio 2023" };
static const std::size_t ABOUT_TEXT_SIZE_3{ 12 };

static const char *LICENSE_TEXT{ "VST is a registered trademark of Steinberg Media Technologies GmbH" };
static const std::size_t LICENSE_TEXT_SIZE{ 10 };

//-------------------------------------------------------------------------------------------------------

AboutPage::AboutPage()
{
    addAndMakeVisible(m_backgroundImage, 0);
    m_backgroundImage.setPaintStripes(false);

    addAndMakeVisible(m_overlayRect);
    m_overlayRect.setFill(juce::Colours::black.withAlpha(0.4f));

    addAndMakeVisible(m_closeButton);
    m_closeButton.setAlpha(0.0f);
    m_closeButton.onClick = [this]() { if (onCloseClicked) onCloseClicked(); };

    addAndMakeVisible(m_xButtonPath);
    m_xButtonPath.setStrokeFill(juce::Colours::white);
    m_xButtonPath.setFill(juce::Colours::transparentWhite);
    m_xButtonPath.setStrokeThickness(3.0f);

    m_svg = juce::XmlDocument::parse(BinaryData::logo_large_svg);
    m_logo = juce::Drawable::createFromSVG(*m_svg.get());
    addAndMakeVisible(m_logo.get());

    addAndMakeVisible(m_topLineLbl);
    addAndMakeVisible(m_middleLineLbl);
    addAndMakeVisible(m_bottomLineLbl);
    addAndMakeVisible(m_licenseLineLbl);
    addAndMakeVisible(m_urlButton);

    m_topLineLbl.setFont(juce::Font(ABOUT_TEXT_SIZE_1, juce::Font::bold));
    m_topLineLbl.setText(ABOUT_TEXT_LINE_1, juce::NotificationType::dontSendNotification);
    m_topLineLbl.setJustificationType(juce::Justification::right);

    m_middleLineLbl.setFont(juce::Font(ABOUT_TEXT_SIZE_2));
    m_middleLineLbl.setText(ABOUT_TEXT_LINE_2, juce::NotificationType::dontSendNotification);
    m_middleLineLbl.setJustificationType(juce::Justification::right);

    m_bottomLineLbl.setFont(juce::Font(ABOUT_TEXT_SIZE_3, juce::Font::italic));
    m_bottomLineLbl.setText(ABOUT_TEXT_LINE_3, juce::NotificationType::dontSendNotification);
    m_bottomLineLbl.setJustificationType(juce::Justification::right);

    m_licenseLineLbl.setFont(juce::Font(LICENSE_TEXT_SIZE));
    m_licenseLineLbl.setText(LICENSE_TEXT, juce::NotificationType::dontSendNotification);
    m_licenseLineLbl.setJustificationType(juce::Justification::right);
}

//-------------------------------------------------------------------------------------------------------

void AboutPage::paint(juce::Graphics &g)
{
    g.fillAll(Theme::BackgroundColor);

}

//-------------------------------------------------------------------------------------------------------

void AboutPage::resized()
{
    m_backgroundImage.setBounds(getLocalBounds());

    const static int buttonDim{ 30 };
    m_closeButton.setBounds(getLocalBounds().removeFromTop(buttonDim).removeFromRight(buttonDim));

    m_logo->setBounds(getLocalBounds().reduced(2, 2));

    auto b = m_closeButton.getBoundsInParent().reduced(7, 7).toFloat();
    juce::Path p;
    p.startNewSubPath(b.getBottomLeft());
    p.lineTo(b.getTopRight());

    p.startNewSubPath(b.getBottomRight());
    p.lineTo(b.getTopLeft());

    m_xButtonPath.setPath(p);

    auto textWidth = 340.0f;

    // don't change these values
    const juce::Rectangle textShadowBounds{ 246.7f, 103.5f, textWidth, 92.5f };
    m_overlayRect.setRectangle(textShadowBounds);

    auto textBounds = textShadowBounds.toNearestInt();

    m_topLineLbl.setBounds(textBounds.removeFromTop(ABOUT_TEXT_SIZE_1));
    m_middleLineLbl.setBounds(textBounds.removeFromTop(ABOUT_TEXT_SIZE_2));
    m_bottomLineLbl.setBounds(textBounds.removeFromTop(ABOUT_TEXT_SIZE_3));
    m_urlButton.setBounds(m_bottomLineLbl.getBounds());

    textBounds.removeFromBottom(LICENSE_TEXT_SIZE);
    m_licenseLineLbl.setBounds(textBounds.removeFromBottom(LICENSE_TEXT_SIZE));
}

//-------------------------------------------------------------------------------------------------------
