#include "MainWindow.h"

MainWindow::MainWindow()
    : DocumentWindow ("AutoMenuAIPro",
                      juce::Colours::black,
                      DocumentWindow::closeButton)
{
    setUsingNativeTitleBar (false);
    setResizable (true, true);

    setContentOwned (new Dashboard(), true);

    setResizeLimits (980, 500, 1280, 720);
    centreWithSize (1100, 540);
    setVisible (true);
}

void MainWindow::closeButtonPressed()
{
    juce::JUCEApplication::getInstance()->systemRequestedQuit();
}
