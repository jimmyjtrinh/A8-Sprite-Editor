#ifndef SPRITE_H
#define SPRITE_H

#include <QImage>
#include <QPixmap>


class Sprite
{
public:
    QImage* currSprite;
    Sprite();

    void setPixel(int x, int y, const QColor& color);
    QImage getImage();
};

#endif // SPRITE_H
