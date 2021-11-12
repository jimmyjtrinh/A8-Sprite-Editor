#include "sprite.h"

Sprite::Sprite()
{
    currSprite = new QImage(32, 32, QImage::Format_RGBA64);
    QColor blank(0,0,0,0);
    currSprite->fill(blank);
}

void Sprite::setPixel(int x, int y, const QColor &color)
{
    currSprite->setPixelColor(x,y,color);

}

QImage Sprite::getImage()
{
    return *currSprite;
}
