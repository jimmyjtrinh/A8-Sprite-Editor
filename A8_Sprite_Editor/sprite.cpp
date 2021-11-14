#include "sprite.h"
#include <iostream>

using namespace std;

Sprite::Sprite()
{
    currSprite = QImage(32, 32, QImage::Format_RGBA64);
    QColor blank(0,0,0,0);
    currSprite.fill(blank);
//    initializeSprite();
}

Sprite:: Sprite(int dimensions)
{
    currSprite = QImage(dimensions, dimensions, QImage::Format_RGBA64);
    QColor blank(0,0,0,0);
    currSprite.fill(blank);
//    initializeSprite();
}

void Sprite::initializeSprite()
{
    QColor blank(0,0,0,0);
    currSprite.fill(blank);
}

Sprite::~Sprite()
{
//    delete currSprite;
}

void Sprite::setPixel(int x, int y, const QColor &color)
{
    currSprite.setPixelColor(x,y,color);

}

QImage Sprite::getImage()
{
    return currSprite;
}
