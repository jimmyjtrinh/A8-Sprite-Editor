#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMouseTracking(true);
    ui->centralwidget->setMouseTracking(true);
    ui->label->setMouseTracking(true);

    pressed = false;

    makeGrid(ui->label->height());
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief MainWindow::mouseMoveEvent Event that is "activated" any time mouse is drag clicked
 * sends x and y to model to be used to draw
 * \param event
 */
void MainWindow::mouseMoveEvent(QMouseEvent *event){
    // relative position of x and y of picture (0 to 255) values
    double relativeXPosOfImage = event->pos().x()-ui->label->x();
    double relativeYPosOfImage = event->pos().y()-ui->label->y();


    // scale used to transform (256 * 256 image coords to 31 * 31 coords)
    double scale = spriteDimensions/513.0;
    // 0 - 31 pixle range that corresponds to what was clicked on image
    int xInPixelSPace =  relativeXPosOfImage*scale;
    int yInPixelSPace =  relativeYPosOfImage*scale;


    // in image range check
    if(relativeXPosOfImage >= 0 && relativeXPosOfImage < (ui->label->width()))
        if(relativeYPosOfImage>=0 && relativeYPosOfImage<(ui->label->height())){
            // send x and y pixel space to draw  - draw here
             //cout <<relativeXPosOfImage << " " << relativeYPosOfImage<< endl;
             cout << xInPixelSPace << " "<< yInPixelSPace<< endl;

             if(pressed){
                 QColor red(255,0,0,255); // model concern???
                  sprite.setPixel(xInPixelSPace, yInPixelSPace, red);

                  makeGrid(ui->label->height());
             }
        }
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    pressed = true;

//    QColor red(255,0,0,255); // model concern???
//     sprite.setPixel(xInPixelSPace, yInPixelSPace, red);
//     makeGrid(ui->label->height(), 32);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    pressed = false;
}


void MainWindow::makeGrid(int canvasSize){
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

    ui->label->setPixmap(pixmap);
}

