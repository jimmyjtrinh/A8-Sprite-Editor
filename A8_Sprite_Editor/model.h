/*
 * Jimmy Trinh && Jacob Day
 * Software Practice II, CS 3505
 * A6: Qt Simon Game
 */
#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <stdlib.h>
#include <QQueue>
#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include "sprite.h"
#include <QTimer>
#include <QLabel>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

using std::rand;

/*!
 * \brief The Model class Model of sprite editing program. Handles what to do when creating an istance of the program. Handles
 * data inputs from controller and deciphers how to use data.
 */
class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);
    void runAnimation();

//used with the view
public slots:
    // updates the pixmap
    void updatePixmap();
    // updates the pixels of a sprite to a color
    void updateSprite(double, double, QColor, int);
    // takes in coordinates of mouse relative to gui
    void getCoords(double, double);
    // takes in dimensions and sets them in this class
    void getDimensions(int);
    // creates new sprite whenever clicked in view is done
    void createNewSprite();
    // takes in fps and changes this classes fps
    void setFps(int);
    // takes given in qcolor and sets all pixels of sprite to color
    void updateAndPaintALl(QColor);

    void previewAnimation();
    void clearingSprite();
    void save(QString);
    void open(QString);

    // takes given index and sets current sprite to that one in list
    void changeSpriteToIndex(int);


//internal model slots
private slots:


signals:
    // send given pixmap of grid to view for canvas
    void sendGrid(QPixmap);
    // send animation preview thumbnail pixmap to view for label
    void sendAnimationPreviewPixmap(QPixmap);
    // send coords as strings to view for its labels
    void sendCoords(QString);
    // send thumbnails to users
    void sendThumbnailLabel(QPixmap);
    // send hte animation for previwing at the actual pixel sizes
    void sendPreviewPixmap(QPixmap);

    void updateCurrentSpriteThumbnail(QPixmap, int);


private:
    int fps;
    int currentAnimatedSpriteIndex;
    int spriteDimensions;
    double scale;
    int currentIndexOfSprites;
    double previewSize = 513.0;

    QVector<Sprite*> sprites;
    Sprite *sprite;

    QTimer *timer;

    // sends what is currently the sprite that should be previewed in animation loop after doing some work
    void sendIndexedSprite();

    void setListPreview();
    // make grid given canvas size
    void makeGrid(int);


    void write(QJsonObject &json) const;
    void read(QJsonObject &json) ;


    QJsonObject jsonObj;


};

#endif // MODEL_H
