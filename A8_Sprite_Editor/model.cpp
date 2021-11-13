/*
 * Jimmy Trinh && Jacob Day
 * Software Practice II, CS 3505
 * A6: Qt Simon Game
 */
#include "model.h"

/**
 * @brief Model::Model Constructor for our model class
 * @param parent
 */
Model::Model(QObject *parent) : QObject(parent)
{

}

/**
 * @brief Model::showBlue changes blue button to light blue
 */
void Model::showBlue(){
    emit blueSignal("Blue siganl working good :)");
//    timer.singleShot(delay - 200, this, &Model::stopShowingBlue);
}

void Model::updatePixmap(){
    makeGrid(513);
}

void Model::updateSprite(double x, double y, QColor color)
{
    // scale used to transform (256 * 256 image coords to 31 * 31 coords)
    double scale = spriteDimensions/513.0;
    // 0 - 31 pixle range that corresponds to what was clicked on image
    int xInPixelSpace =  x*scale;
    int yInPixelSpace =  y*scale;

    sprite.setPixel(xInPixelSpace,yInPixelSpace,color);
}

void Model::getCoords(double x, double y){

    double scale = spriteDimensions/513.0;
    // 0 - 31 pixle range that corresponds to what was clicked on image
    int xInPixelSpace =  x*scale;
    int yInPixelSpace =  y*scale;

    emit sendCoords(QString::number(xInPixelSpace) + "        " + (QString::number(yInPixelSpace)));
}



void Model::makeGrid(int canvasSize){
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

