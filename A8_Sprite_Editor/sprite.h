#ifndef SPRITE_H
#define SPRITE_H

#include <QImage>
#include <QPixmap>


class Sprite
{
public:
    QImage currSprite;
    Sprite();
    ~Sprite();
    Sprite(int);
    Sprite(const Sprite&);

    void setPixel(int x, int y, const QColor& color);
    QImage getImage();

private:
    void initializeSprite();
};

#endif // SPRITE_H
