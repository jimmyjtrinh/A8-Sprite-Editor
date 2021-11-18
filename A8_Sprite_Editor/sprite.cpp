/*
 * Jimmy Trinh && Jacob Day && Amitoj Singh && Michael Shin
 * Software Practice II, CS 3505
 * A8: Qt Sprite Editor
 */
#include "sprite.h"
/*!
 *Class representing all sprite objects in sprite editing program
 */

/*!
 * \brief Sprite::Sprite constructs sprite and initializes all mebers of a sprite with size 32
 */
Sprite::Sprite()
{
    int initialSpriteSize = 32;
    // set all sprites to have a qimage of size 32
    currSprite = QImage(initialSpriteSize, initialSpriteSize, QImage::Format_RGBA64);
    // fill image to be empty
    QColor blank(0,0,0,0);
    currSprite.fill(blank);
}

/*!
 * \brief Sprite::Sprite constructs sprite and initializes all mebers of a sprite with size of what is passed in
 * \param dimensions size of sprite
 */
Sprite:: Sprite(int dimensions)
{
    currSprite = QImage(dimensions, dimensions, QImage::Format_RGBA64);
    QColor blank(0,0,0,0);
    currSprite.fill(blank);
}

/*!
 * \brief Sprite::initializeSprite resets sprite to be in the same state it was when constructed
 */
void Sprite::initializeSprite()
{
    QColor blank(0,0,0,0);
    currSprite.fill(blank);
}

/*!
 * \brief Sprite::~Sprite destructor
 */
Sprite::~Sprite()
{
}

/*!
 * \brief Sprite::Sprite copy constructor, sets this sprite to other sprite by copying
 * others parameters
 * \param other sprite who's parameters will be set to this'
 */
Sprite::Sprite(const Sprite& other)
{
    this->currSprite = other.currSprite;
}



/*!
 * \brief Sprite::setPixel take given x and y and set pixel at coords to color
 * \param x x coord of image to change
 * \param y y coord of image to change
 * \param color color to change pixel at coords to
 */
void Sprite::setPixel(int x, int y, const QColor &color)
{
    currSprite.setPixelColor(x,y,color);

}

/*!
 * \brief Sprite::getImage returns image of this sprite
 */
QImage Sprite::getImage()
{
    return currSprite;
}

/*!
 * \brief Sprite::getPixel gets color at given pixel
 * \param x x location of acquired pixel
 * \param y y location of acquired pixel
 * \return color at location
 */
QColor Sprite::getPixel(int x, int y)
{
    return currSprite.pixelColor(x, y);
}

/*!
 * \brief Sprite::paintBucket method takes x and y and paints color at given x and y using bfs
 * \param x x location of sprite to paint
 * \param y y location of sprite to paint
 * \param color color being painted to
 */
void Sprite::paintBucket(int x, int y, const QColor &color, const QColor &startColor)
{
    if(x < 0 || y < 0 || x >= currSprite.width() || y >= currSprite.height())
    {
        return;
    }

    if(currSprite.pixelColor(x,y) != startColor)
    {
        return;
    }

    currSprite.setPixelColor(x,y,color);
    this->paintBucket(x-1, y, color, startColor);
    this->paintBucket(x+1, y, color, startColor);
    this->paintBucket(x, y-1, color, startColor);
    this->paintBucket(x, y+1, color, startColor);
}
