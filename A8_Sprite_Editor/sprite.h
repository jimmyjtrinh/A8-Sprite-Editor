#ifndef SPRITE_H
#define SPRITE_H

#include <QImage>
#include <QPixmap>

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

private:
    void initializeSprite();
};

#endif // SPRITE_H
