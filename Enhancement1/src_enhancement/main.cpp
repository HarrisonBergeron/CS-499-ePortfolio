/*
 * main.cpp
 * Author: Harrison Bergeron
 * Date: July 14, 2022
 * Course: CS-499-H6772 Computer Science Capstone 22EW6
 */

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    // Only use the first argument (executable name) and discard the rest
    // for security purposes. No other arguments are accepted.
    char* arg = argv[0];
    argc = 1;
    // Create a Qt application instance with required command line argument.
    QApplication a(argc, &arg);
    // Create the custom window instance set the default size.
    MainWindow window;
    window.setWindowTitle("Harrison Bergeron CS 250 Software "
                          "Engineering and Design Enhancement");
    window.resize(900, 600);
    // Display window.
    window.show();
    // Begin event loop and terminate when done.
    return a.exec();
}
