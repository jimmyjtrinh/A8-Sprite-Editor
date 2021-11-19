/*
 * Jimmy Trinh && Jacob Day && Amitoj Singh && Michael Shin
 * Software Practice II, CS 3505
 * A8: Qt Sprite Editor
 *
 * VIDEO DEMO LINK:
 * https://www.youtube.com/watch?v=K606owc6uvY
 */
#ifndef SPRITE_H
#define SPRITE_H

#include <QImage>
#include <QPixmap>
#include <QJsonObject>

/*!
 * \brief The Sprite class Class representing all sprite objects in sprite editing program
 */
class Sprite
{
public:

    Sprite();
    // rule of thirds
    ~Sprite();
    Sprite(int);
    Sprite(const Sprite&);


    // image related code
    QImage currSprite;
    QImage getImage();
    QColor getPixel(int x, int y);

    // method does paint bucket on pixel
    void paintBucket(int, int, const QColor&, const QColor&);
    // set pixel
    void setPixel(int x, int y, const QColor& color);

private:
    void initializeSprite();
};

#endif // SPRITE_H
