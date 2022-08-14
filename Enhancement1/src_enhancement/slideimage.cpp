/*
 * slideimage.cpp
 * Author: Harrison Bergeron
 * Date: July 14, 2022
 * Course: CS-499-H6772 Computer Science Capstone 22EW6
 */

#include "slideimage.h"

SlideImage::SlideImage(QString image_name, QWidget* parent)
    : QWidget(parent)
{
    // Load the image associated with the filename string; store success value.
    loaded = image.load(image_name);
}

SlideImage::~SlideImage()
{
    // The image variable will automatically be destructed, no need for delete.
}

void SlideImage::paintEvent(QPaintEvent* event)
{
    // Create a painter to display the image for this widget.
    QPainter painter(this);
    // Draw the image at (0, 0) relative coordinates for this widget and
    // stretch to the widget's current size.
    painter.drawPixmap(0, 0, width(), height(), image);
}

bool SlideImage::IsLoaded()
{
    return loaded;
}
