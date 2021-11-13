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

    connect(&modelObj, &Model::sendPixmap, ui->label, &QLabel::setPixmap);
    connect(&modelObj, &Model::sendCoords, ui->coordLabel, &QLabel::setText);

    connect(this, &MainWindow::updateGrid, &modelObj, &Model::updatePixmap);
    connect(this, &MainWindow::updatePixel, &modelObj, &Model::updateSprite);
    connect(this, &MainWindow::updateCoords, &modelObj, &Model::getCoords);

    pressed = false;
    emit updateGrid();
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

    // in image range check
    if(relativeXPosOfImage >= 0 && relativeXPosOfImage < (ui->label->width()))
        if(relativeYPosOfImage>=0 && relativeYPosOfImage<(ui->label->height())){
            // send x and y pixel space to draw  - draw here
             if(pressed){

                 QColor red(255,0,0,255); // model concern???

                  emit updatePixel(relativeXPosOfImage, relativeYPosOfImage, red);

                  emit updateGrid();
             }

             emit updateCoords(relativeXPosOfImage, relativeYPosOfImage);

        }
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    pressed = true;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    pressed = false;
}

