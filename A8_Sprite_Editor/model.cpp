/*
 * Jimmy Trinh && Jacob Day
 * Software Practice II, CS 3505
 * A6: Qt Simon Game
 */
#include "model.h"
#include <iostream>

/*!
 * Model of sprite editing program. Handles what to do when creating an istance of the program. Handles
 * data inputs from controller and deciphers how to use data.
 */
using namespace std;
/**
 * @brief Model::Model Constructor for our model class
 * @param parent
 */
Model::Model(QObject *parent) : QObject(parent)
{
    fps = 1;

    timer = new QTimer(this); // asldkfjasldfkjasldfkjasldfkjsd
    connect(timer, &QTimer::timeout, this, &Model::runAnimation);
    timer->start(1000);
}

/*!
 * \brief Model::createNewSprite Method creates a new sprite and adds to list of
 * sprites.
 */
void Model::createNewSprite(){
    // call method that will handle thumbnail previews
    setListPreview();
    Sprite *temp = new Sprite(spriteDimensions);
    sprites.push_back(temp);
    sprite = temp;
    currentIndexOfSprites = sprites.length()-1;
    updatePixmap();

}

/*!
 * \brief Model::getDimensions slot method decides what to do given the dimensions of the sprite.
 * Will set this class' dimension member and and create and initialize the first sprite of this class
 * \param dim
 */
void Model::getDimensions(int dim)
{
    spriteDimensions = dim;
    scale = spriteDimensions/previewSize;
    // create the first sprite of model
    sprite = new Sprite(dim);
    // index of sprite is that of the only sprite we have
    currentIndexOfSprites = 0;
    sprites.push_back(sprite);
}

/*!
 * \brief Model::updatePixmap Wrapper method for updating pixmap by adding grid
 */
void Model::updatePixmap(){
    makeGrid(previewSize);
}

/*!
 * \brief Model::updateSprite Slot that will take given location and color and will set current
 * sprite's pixel at x and y to color given
 * \param x non-scaled x position of the grid (ranges from 0 - to preview size: 513
 * \param y non-scaled y position of the grid (ranges from 0 - to preview size: 513
 * \param color color setting sprite pixel to
 */
void Model::updateSprite(double x, double y, QColor color)
{
    // convert 0 -513 x, y to 0 - 3, 15, 31, 63, ... x, y pixle range that corresponds to what was clicked on image
    int xInPixelSpace =  x*scale;
    int yInPixelSpace =  y*scale;

    // set the pixel with actual corresponding x, y
    sprite->setPixel(xInPixelSpace,yInPixelSpace,color);

}

/*!
 * \brief Model::getCoords slot that takes given coordinates, converts them to sprite based x, y
 * and sends them to update in view
 * \param x non-scaled x position of the grid (ranges from 0 - to preview size: 513
 * \param y non-scaled y position of the grid (ranges from 0 - to preview size: 513
 */
void Model::getCoords(double x, double y){

    // 0 - 3, 15, 31, 63 .... pixle range that corresponds to what was clicked on image
    int xInPixelSpace =  x*scale;
    int yInPixelSpace =  y*scale;
    // send scaled coords to view
    emit sendCoords("x: "+QString::number(xInPixelSpace) + "  y: " + (QString::number(yInPixelSpace)));
}


/*!
 * \brief Model::makeGrid makes the grid for the sprite drawing.
 * \param canvasSize size of the canvas/preview drawing of sprite
 */
void Model::makeGrid(int canvasSize){
    QPixmap pixmap(QPixmap::fromImage(sprite->getImage()).scaled(canvasSize, canvasSize, Qt::KeepAspectRatio));
    QPainter painter(&pixmap);

    painter.setPen(QColor(0, 0, 0, 200));

    //vertical lines
    for(float x = 0; x <= pixmap.width(); x+=pixmap.width()/spriteDimensions){
        painter.drawLine(x, 0, x, pixmap.height());
    }

    //horizontal lines
    for(float y = 0; y <= pixmap.height(); y+=pixmap.height()/spriteDimensions){
        painter.drawLine(0, y, pixmap.width(), y);
    }

    // send grid to view to draw
    emit sendGrid(pixmap);
}

/*!
 * \brief Model::setFps slot handles being sent the fps, will change the timer that handles the
 * animation speed
 * \param i new fps value
 */
void Model::setFps(int updatedFps){
    fps = updatedFps;
    timer->start(1000/fps);
}

/*!
 * \brief Model::runAnimation slot that handles beginning the animation cycle
 */
void Model::runAnimation(){
    previewAnimation();
    sendIndexedSprite();

}

/*!
 * \brief Model::sendIndexedSprite Will send sprites in sprites list one by one, method is called again and again
 * as this shall be running at all times. Handles sprite image sending loop
 */
void Model::sendIndexedSprite(){
    // only send if their are sprites right now in list
    if(sprites.length() != 0)
        // send the sprite at which the class member index variable is at as of rn
        emit sendAnimationPreviewPixmap(QPixmap::fromImage(sprites[currentAnimatedSpriteIndex++]->currSprite).scaled(128, 128, Qt::KeepAspectRatio));
    // reset the index variable
    if(currentAnimatedSpriteIndex>=sprites.length())
        currentAnimatedSpriteIndex = 0;
}

/*!
 * \brief Model::previewAnimation Will send the pixmaps to the view for the preview at actual size without scale
 */
void Model::previewAnimation(){
    if(sprites.length() != 0)
        // send the sprite at which the class member index variable is at as of rn
        emit sendPreviewPixmap(QPixmap::fromImage(sprites[currentAnimatedSpriteIndex]->currSprite));
    // reset the index variable
}

/*!
 * \brief Model::setListPreview method takes the current sprite being worked on and send that sprites image
 * to the view to be shown
 */
void Model::setListPreview(){
    QLabel *temp = new QLabel();
    // set the the label of the temp to the image of the most recent preview of the sprite
    temp->setPixmap(QPixmap::fromImage(sprite->getImage().scaled(83, 83, Qt::KeepAspectRatio)));
    emit sendThumbnailLabel(temp);
}

/*!
 * \brief Model::updateAndPaintALl slot takes current sprite and fills all of the image
 * with color given
 * \param selected color to set as the fill of the image
 */
void Model::updateAndPaintALl(QColor selected){
    sprite->currSprite.fill(selected);

}



