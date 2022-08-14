/*
 * mainwindow.h
 * Author: Harrison Bergeron
 * Date: July 14, 2022
 * Course: CS-499-H6772 Computer Science Capstone 22EW6
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QMainWindow>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QPushButton>
#include <QLabel>

#include "slideimage.h"

/*
 * Represents the window containing the slide show. Contains all of the widgets
 * like slide images, text, and button controls. This class exists because it is
 * efficient to have one class that manages all of the UI and widgets for a Qt
 * application. In this case, the main window that opens upon launching the
 * application serves that purpose. It derives all of its window behavior from
 * the QMainWindow class.
 */
class MainWindow : public QMainWindow
{
    // This is a Qt widget class.
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    // Central widget of the window containing all other windows.
    QFrame* central_frame;
    // Layout for central_frame.
    QVBoxLayout* frame_vertical_layout;
    // Layout for slides where only one is displayed at a time.
    QStackedLayout* slide_stacked_layout;
    // Layout to partition button controls in the bottom center of the window.
    QHBoxLayout* controls_layout;

    // Buttons for switching slides back and forth.
    QPushButton* prev_control_button;
    QPushButton* next_control_button;

    // Add a new slide to the show with a title and description and image.
    void AddSlide(const QString& title, const QString& description,
                  const QString& image_file);

private slots:
    // Callbacks that are invoked when previous and next buttons are pressed.
    // Switch the index on the slide_stacked_layout.
    void PreviousClicked();
    void NextClicked();
};
#endif // MAINWINDOW_H
