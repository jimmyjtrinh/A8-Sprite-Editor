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

using std::rand;

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);
    void runAnimation();

//used with the view
public slots:

    void updatePixmap();
    void updateSprite(double, double, QColor);
    void getCoords(double, double);
    void getDimensions(int);
    void createNewSprite();
    void setFps(int);

//internal model slots
private slots:


signals:

    void sendPixmap(QPixmap);
    void sendAnimationPreviewPixmap(QPixmap);
    void sendCoords(QString);


private:
    int fps;
    int currentAnimatedSpriteIndex;
    QVector<Sprite*> sprites;
    int currentIndexOfSprites;
    Sprite sprite;
    int spriteDimensions;
    double scale;
    QTimer *timer;

    void sendIndexedSprite();

    void makeGrid(int);


};

#endif // MODEL_H
