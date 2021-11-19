/*
 * Jimmy Trinh && Jacob Day && Amitoj Singh && Michael Shin
 * Software Practice II, CS 3505
 * A8: Qt Sprite Editor
 */
#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include "sprite.h"
#include <QLabel>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

/*!
 * \brief The Model class Model of sprite editing program. Handles what to do when creating an istance of the program. Handles
 * data inputs from controller and deciphers how to use data.
 */
class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

    // Updates the animation in the right corner
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
    void getDimensionsAndInitializeFirstSprite(int);
    // creates new sprite whenever clicked in view is done
    void createNewSprite();
    // takes in fps and changes this classes fps
    void setFps(int);
    // takes given in qcolor and sets all pixels of sprite to color
    void updateAndPaintALl(QColor);
    // will handle and begin previewing cycle
    void previewAnimation();
    // clear sprite that is held at sprite variable and update view
    void clearCurrentSprite();
    // save given string file name
    void save(QString);
    // open given string file name
    void open(QString);
    // takes given index and sets current sprite to that one in list
    void changeSpriteToIndex(int);
    // apply paint method at sprite x,y with color
    void paintSprite(int,int,const QColor&);

signals:
    // send given pixmap of grid to view for canvas
    void sendGrid(QPixmap);
    // send animation preview thumbnail pixmap to view for label
    void sendAnimationPreviewPixmap(QPixmap);
    // send coords as strings to view for its labels
    void sendCoords(QString);
    // send thumbnails to users
    void sendThumbnailLabel(QPixmap, int );
    // send the animation for previwing at the actual pixel sizes
    void sendPreviewPixmap(QPixmap);
    // sends the current sprite to the thumbnail to see changes in real time
    void updateCurrentSpriteThumbnail(QPixmap, int);
    // signal that tells view to show error message for json being invalid
    void errorWhenParsingJsonFile();
    // signal letting scroll area know that it should be emptied
    void clearButtonThumbnails();

private:
    int fps;
    int currentAnimatedSpriteIndex;
    int spriteDimensions;
    double scale;
    int currentIndexOfSprites;
    double previewSize = 513.0;
    int numFrames; // number of sprites
    int prefferedThumbnailSize = 83;

    QJsonObject jsonObj;
    QVector<Sprite*> sprites;
    Sprite *sprite;
    QTimer *timer;

    // sends what is currently the sprite that should be previewed in animation loop after doing some work
    void sendIndexedSprite();
    // creates new Qlabel for the thumbnails then updates pixmap
    void createNewThumbnailPreview();
    // make grid given canvas size
    void makeGrid(int);
    // method writes to given json object
    void write(QJsonObject &json) const;
    // method reads from json object and sets this member variable accordingly
    void read(QJsonObject &json) ;
    // helper for scaling the x and y coordinates
    int transformCoordToXandYSpriteSpace(int);
};

#endif // MODEL_H
