/*
 * Jimmy Trinh && Jacob Day && Amitoj Singh && Michael Shin
 * Software Practice II, CS 3505
 * A8: Qt Sprite Editor
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
    // set pixel
    void setPixel(int x, int y, const QColor& color);
    // image related code
    QImage currSprite;
    QImage getImage();

    QColor getPixel(int x, int y);

    void paintBucket(int, int, const QColor&);



private:
    void initializeSprite();
};

#endif // SPRITE_H
