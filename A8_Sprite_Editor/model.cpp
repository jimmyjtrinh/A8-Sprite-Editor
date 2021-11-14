/*
 * Jimmy Trinh && Jacob Day
 * Software Practice II, CS 3505
 * A6: Qt Simon Game
 */
#include "model.h"
#include <iostream>

using namespace std;
/**
 * @brief Model::Model Constructor for our model class
 * @param parent
 */
Model::Model(QObject *parent) : QObject(parent)
{
    //    spriteDimensions = 32;

    //    // scale used to transform (256 * 256 image coords to 31 * 31 coords)
    //    scale = spriteDimensions/513.0;
    //    sprite = Sprite();

    fps =1;



}

/**
 * @brief Model::showBlue changes blue button to light blue
 */
void Model::showBlue(){
    emit blueSignal("Blue siganl working good :)");
    //    timer.singleShot(delay - 200, this, &Model::stopShowingBlue);
}

void Model::createNewSprite(){
    Sprite temp = Sprite(spriteDimensions);

    sprites.push_back(sprite);
    sprite = temp;
    currentIndexOfSprites++;

    updatePixmap();

}


void Model::getDimensions(int dim)
{
    cout << "got dimesions " << dim << endl;
    spriteDimensions = dim;
    scale = spriteDimensions/513.0;
    sprite = Sprite(dim);
    cout << "set up sprite" << endl;



   // sprites.push_back(sprite);
    currentIndexOfSprites = 0;

}

void Model::updatePixmap(){
    makeGrid(513);
}

void Model::updateSprite(double x, double y, QColor color)
{
    // 0 - 31 pixle range that corresponds to what was clicked on image
    int xInPixelSpace =  x*scale;
    int yInPixelSpace =  y*scale;

    sprite.setPixel(xInPixelSpace,yInPixelSpace,color);
}

void Model::getCoords(double x, double y){

    // 0 - 31 pixle range that corresponds to what was clicked on image
    int xInPixelSpace =  x*scale;
    int yInPixelSpace =  y*scale;

    emit sendCoords("x: "+QString::number(xInPixelSpace) + "  y: " + (QString::number(yInPixelSpace)));
}



void Model::makeGrid(int canvasSize){
    cout << "got to make grid" << endl;
    QPixmap pixmap(QPixmap::fromImage(sprite.getImage()).scaled(canvasSize, canvasSize, Qt::KeepAspectRatio));
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

    emit sendPixmap(pixmap);
}


void Model::setFps(int i){
    fps = i;
 //   runAnimation(); - broken need to fix way adding in sprites to model array
}

void Model::runAnimation(){

        for(int i = 0; i<sprites.length(); i++){
            currentAnimatedSpriteIndex = i;
            QTimer::singleShot((1000*i)/fps, this, &Model::sendIndexedSprite);


    }
}

void Model::sendIndexedSprite(){

    emit sendAnimationPreviewPixmap(QPixmap::fromImage(sprites[currentAnimatedSpriteIndex].currSprite).scaled(128, 128, Qt::KeepAspectRatio));
}



