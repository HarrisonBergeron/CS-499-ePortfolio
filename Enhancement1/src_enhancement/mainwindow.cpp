/*
 * mainwindow.cpp
 * Author: Harrison Bergeron
 * Date: July 14, 2022
 * Course: CS-499-H6772 Computer Science Capstone 22EW6
 */

#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // Create frame to contain everything in the GUI.
    central_frame = new QFrame();
    frame_vertical_layout = new QVBoxLayout();
    // No space between window border and widgets.
    frame_vertical_layout->setContentsMargins(0, 0, 0, 0);
    central_frame->setLayout(frame_vertical_layout);

    // Create stacked layout for each slide and its text, can hold multiple
    // consecutive slide instances while only showing one at a time.
    slide_stacked_layout = new QStackedLayout();
    frame_vertical_layout->addLayout(slide_stacked_layout);

    // Add all of the slides with titles, descriptions, and images.
    AddSlide("#1 Rancho Valencia Resort & Spa in San Diego.",
             "Fitness and wellness classes like yoga to "
             "help with the mind and body.",
             "Destination1.jpg");
    AddSlide("#2 Mii Amo Spa in Arizona.",
             "A place to relax and reduce the toxins "
             "in the body with massages.",
             "Destination2.jpg");
    AddSlide("#3 Lake Austin Spa Resort in Austin, Texas.",
             "Activities like canoeing and hiking with wellness classes.",
             "Destination3.jpg");
    AddSlide("#4 Blackberry Mountain in Tennessee.",
             "Wellness retreat that focuses on health "
             "with plenty of fitness activities.",
             "Destination4.jpg");
    AddSlide("#5 Cal-a-Vie in California.",
             "A resort with wellness and fitness classes "
             "that focus on mindfulness.",
             "Destination5.jpg");

    // Create a layout to partition button controls.
    controls_layout = new QHBoxLayout();
    frame_vertical_layout->addLayout(controls_layout);

    // Ratio of space on the left and right sides of button controls.
    const int kButtonStretchSides = 5;
    // Ratio of space taken up by buttons themselves.
    const int kButtonStretch = 2;
    prev_control_button = new QPushButton("Previous");
    next_control_button = new QPushButton("Next");
    // Place flexible "stretch" spacing around and between button controls.
    // Ratio is 5:2:1:2:5
    controls_layout->addStretch(kButtonStretchSides);
    controls_layout->addWidget(prev_control_button, kButtonStretch);
    controls_layout->addStretch(1);
    controls_layout->addWidget(next_control_button, kButtonStretch);
    controls_layout->addStretch(kButtonStretchSides);
    // Using Qt signals and slots, connect the "clicked" events from the next
    // and previous buttons to the "PreviousClicked" and "NextClicked"
    // callbacks. Pressing these buttons will invoke slide switching functions.
    connect(prev_control_button, &QPushButton::clicked,
            this, &MainWindow::PreviousClicked);
    connect(next_control_button, &QPushButton::clicked,
            this, &MainWindow::NextClicked);

    // Add additional spacing below button controls for appearance.
    frame_vertical_layout->addSpacing(8);

    // Place all GUI content in the window.
    setCentralWidget(central_frame);
}

MainWindow::~MainWindow()
{
    // Qt will handle deallocation of widgets upon destruction.
}

void MainWindow::AddSlide(const QString& title, const QString& description,
                          const QString& image_file)
{
    // Initialize "CSS" font styles as reusable constants.
    const QString kBackgroundColor = "background-color: rgb(127, 179, 255);";
    const QString kFontStyleTitle = "font-size: 20px;font-weight: bold;";
    const QString kFontStyleDescription = "font-size: 16px;";

    // Create a frame to hold the slide image and text.
    QFrame* slide_frame = new QFrame();
    // Layout to partition image, title, and description widgets vertically.
    QVBoxLayout* slide_layout = new QVBoxLayout();
    // Set spacing and margins to 0 so widgets are adjacent and snug.
    slide_layout->setSpacing(0);
    slide_layout->setContentsMargins(0, 0, 0, 0);
    slide_frame->setLayout(slide_layout);

    // Create a new resizable slide image (matches frame size dynamically).
    SlideImage* slide_image = new SlideImage(image_file);
    // Only add the image if it is loaded successfully.
    if (slide_image->IsLoaded())
    {
        // Expanding size policy = stretches horizontally and vertically.
        slide_image->setSizePolicy(QSizePolicy::Expanding,
                                   QSizePolicy::Expanding);
        slide_layout->addWidget(slide_image);
    }
    // Otherwise, add a label about the error.
    else
    {
        delete slide_image;
        QLabel* error_label = new QLabel(image_file + " failed to load.");
        error_label->setSizePolicy(QSizePolicy::Expanding,
                                   QSizePolicy::Expanding);
        slide_layout->addWidget(error_label);
    }

    // Larger title text for each slide image.
    QLabel* title_label = new QLabel(title);
    // Grows horizontally but takes up minimal vertical space.
    title_label->setSizePolicy(QSizePolicy::Expanding,
                               QSizePolicy::Minimum);
    // Text background color + sets font style in "CSS".
    title_label->setStyleSheet(kBackgroundColor + kFontStyleTitle);
    slide_layout->addWidget(title_label);

    // Smaller description text to provide more information for slides.
    QLabel* description_label = new QLabel(description);
    // Grows horizontally but takes up minimal vertical space.
    description_label->setSizePolicy(QSizePolicy::Expanding,
                                     QSizePolicy::Minimum);
    // Text background color + sets font style in "CSS".
    description_label->setStyleSheet(kBackgroundColor + kFontStyleDescription);
    slide_layout->addWidget(description_label);

    // Add slide frame to stacked layout to allow for switching the current
    // visible slide via an index variable.
    slide_stacked_layout->addWidget(slide_frame);
}

void MainWindow::PreviousClicked()
{
    const int kCount = slide_stacked_layout->count();
    int index = slide_stacked_layout->currentIndex();
    // If index is already at min (0), set to max, else decrement to prev slide.
    if (index == 0)
        index = kCount - 1;
    else
        index--;
    // Switch to the new slide with the updated index.
    slide_stacked_layout->setCurrentIndex(index);
}

void MainWindow::NextClicked()
{
    const int kCount = slide_stacked_layout->count();
    int index = slide_stacked_layout->currentIndex();
    // If index is already at max, set to min (0), else increment to next slide.
    if (index == kCount - 1)
        index = 0;
    else
        index++;
    // Switch to the new slide with the updated index.
    slide_stacked_layout->setCurrentIndex(index);
}
