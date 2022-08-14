/*
 * slideimage.h
 * Author: Harrison Bergeron
 * Date: July 14, 2022
 * Course: CS-499-H6772 Computer Science Capstone 22EW6
 */

#ifndef SLIDEIMAGE_H
#define SLIDEIMAGE_H

#include <QWidget>
#include <QPainter>
#include <QString>
#include <QPixmap>

/*
 * Represents a stretchable image widget that displays a picture slide in a
 * slide show, not including the text. This class exists because Qt does not
 * provide a straightforward method for displaying images that stretch to fit
 * the parent widget. By implementing a custom widget like this, a custom
 * paintEvent() function may be implemented to add this desired functionality.
 * The class extends QWidget to inherit widget functionality.
 */
class SlideImage : public QWidget
{
    // This is a Qt widget class.
    Q_OBJECT

public:
    SlideImage(QString image_name, QWidget* parent = nullptr);
    ~SlideImage();

    // Called when the widget needs to be displayed.
    void paintEvent(QPaintEvent* event) override;

    // Returns false if image file doesn't exist or is invalid.
    bool IsLoaded();
private:
    // Image that is loaded with the image_name string in the constructor
    // and is displayed stretched to fit the widget dimensions.
    QPixmap image;
    // Boolean to tell whether the image was successfully loaded from the disk.
    bool loaded;
};

#endif // SLIDEIMAGE_H
