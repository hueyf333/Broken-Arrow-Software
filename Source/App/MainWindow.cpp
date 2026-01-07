#include "MainWindow.h"
#include "../UI/CustomLookAndFeel.h"

namespace KnobSmith
{

MainWindow::MainWindow(juce::String name)
    : DocumentWindow(name,
                    juce::Desktop::getInstance().getDefaultLookAndFeel()
                                                .findColour(juce::ResizableWindow::backgroundColourId),
                    DocumentWindow::allButtons)
{
    setUsingNativeTitleBar(true);
    
    mainComponent = std::make_unique<MainComponent>();
    setContentOwned(mainComponent.get(), true);
    
    menuBarModel = std::make_unique<MenuBarModel>(*mainComponent);
    
#if JUCE_MAC
    setMenuBar(menuBarModel.get());
#else
    setMenuBar(menuBarModel.get(), 25);
#endif

    setResizable(true, true);
    centreWithSize(getWidth(), getHeight());

    setVisible(true);
}

void MainWindow::closeButtonPressed()
{
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}

} // namespace KnobSmith
