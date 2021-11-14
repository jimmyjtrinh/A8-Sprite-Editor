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

using std::rand;

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

//used with the view
public slots:

    void showBlue(); //changes blue button to light blue

    void updatePixmap();
    void updateSprite(double, double, QColor);
    void getCoords(double, double);
    void getDimensions(int);
    void createNewSprite();
    void setFps(int);

//internal model slots
private slots:


signals:

    void blueSignal(QString);
    void sendPixmap(QPixmap);
    void sendCoords(QString);


private:
    int fps;
    QVector<Sprite> sprites;
    int currentIndexOfSprites;
    Sprite sprite;
    int spriteDimensions;
    double scale;


    void makeGrid(int);
};

#endif // MODEL_H
