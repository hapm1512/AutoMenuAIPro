#include "MainWindow.h"
#include "../UI/Theme/AppTheme.h"

MainWindow::MainWindow()
    : DocumentWindow ("AutoMenuAIPro",
                      AppTheme::background(),
                      juce::DocumentWindow::closeButton)
{
    setUsingNativeTitleBar (false);
    setResizable (false, false);

    setContentOwned (new Dashboard(), true);
    centreWithSize (AppTheme::windowWidth, AppTheme::windowHeight);
    setVisible (true);
}

void MainWindow::closeButtonPressed()
{
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}
