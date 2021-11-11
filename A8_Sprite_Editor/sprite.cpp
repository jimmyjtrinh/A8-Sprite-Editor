#include "sprite.h"

Sprite::Sprite()
{
    currSprite = new QImage(32, 32, QImage::Format_RGBA64);
    QColor blank(0,0,0,255);
    currSprite->fill(blank);
    currSprite->scaled(10000, 10000);
}

void Sprite::setPixel(int x, int y, const QColor &color)
{
    currSprite->setPixelColor(x,y,color);

}

QImage Sprite::getImage()
{
    return currSprite->scaled(1000,1000,Qt::KeepAspectRatio);
}
