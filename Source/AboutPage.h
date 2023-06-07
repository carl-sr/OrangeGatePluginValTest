#pragma once

//-------------------------------------------------------------------------------------------------------

#include "OrangeGate.h"

#include "BackgroundImage.h"

//-------------------------------------------------------------------------------------------------------

class AboutPage : public juce::Component
{
private:
    BackgroundImage m_backgroundImage;
    juce::DrawableRectangle m_overlayRect;
    juce::Label m_topLineLbl;
    juce::Label m_middleLineLbl;
    juce::Label m_bottomLineLbl;
    juce::Label m_licenseLineLbl;
    juce::HyperlinkButton m_urlButton{ "", juce::URL("https://vainaudio.com/orange-gate") };
    
    juce::TextButton m_closeButton;
    juce::DrawablePath m_xButtonPath;

    std::unique_ptr<juce::XmlElement> m_svg;
    std::unique_ptr<juce::Drawable> m_logo;

public:
    AboutPage();

    void paint(juce::Graphics &g) override;
    void resized() override;

    std::function<void()> onCloseClicked;
};

//-------------------------------------------------------------------------------------------------------
