#include "sprite.h"
#include <iostream>
/*!
 *Class representing all sprite objects in sprite editing program
 */
using namespace std;

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


//void Sprite::read(const QJsonObject &json)
//{
//    if (json.contains("height") && json["height"].isInt()) // isDouble???
//        spriteHeight = json["height"].toInt();

//    if (json.contains("width") && json["width"].isInt())
//        spriteWidth = json["width"].toInt();

//    if (json.contains("numberOfFrames") && json["numberOfFrames"].isInt())
//        spriteNumFrames = json["numberOfFrames"].toInt();]

//    // PSEUDO CODE
//    // Need to make actual code that can go through array.
//    // Might need to go through QPixMap's matrix and assign each pixel an element from the "frames" array.
//    if (json.contains("frames"))
//        for (int i = 0; i < spriteNumFrames; i++)
//           if (json.contains("frame" + i) && json["frame" + i].isArray())
//              sprites = json["frame" + i].toArray();




//}
