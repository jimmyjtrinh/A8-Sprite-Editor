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


    //ui->label->setPixmap(QPixmap::fromImage(sprite.getImage()));
       QPixmap pixmap(QPixmap::fromImage(sprite.getImage()).scaled(512, 512, Qt::KeepAspectRatio));
       QPainter painter(&pixmap);

       painter.setPen(QColor(255, 255, 255, 200));

       //vertical lines
       for(float x = 0; x <= pixmap.width(); x+=pixmap.width()/32){
           painter.drawLine(x, 0, x, pixmap.height());
       }

       //horizontal lines
       for(float y = 0; y <= pixmap.height(); y+=pixmap.height()/32){
           painter.drawLine(0, y, pixmap.width(), y);
       }

       ui->label->setPixmap(pixmap);


//   ui->centralwidget->setMouseTracking(true);
//     ui->label->setMouseTracking(true);



     // E.g. set in your constructor of your widget.
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
    double scale = 32.0/512.0;
    // 0 - 31 pixle range that corresponds to what was clicked on image
    int xInPixelSPace =  relativeXPosOfImage*scale;
    int yInPixelSPace =  relativeYPosOfImage*scale;


   // if(event->type()==QEvent::MouseButtonPress)



    // in image range check
    if(relativeXPosOfImage >= 0 && relativeXPosOfImage < (ui->label->width()))
        if(relativeYPosOfImage>=0 && relativeYPosOfImage<(ui->label->height())){
            // send x and y pixel space to draw  - draw here
             //cout <<relativeXPosOfImage << " " << relativeYPosOfImage<< endl;
             cout << xInPixelSPace<< " "<< yInPixelSPace<< endl;
             QColor red(255,0,0,255);
                 sprite.setPixel(xInPixelSPace, yInPixelSPace, red);
                 ui->label->setPixmap(QPixmap::fromImage(sprite.getImage()));}
}



void MainWindow::on_pushButton_pressed()
{

//    //ui->label->setPixmap(QPixmap::fromImage(sprite.getImage()));
//       QPixmap pixmap(QPixmap::fromImage(sprite.getImage()).scaled(512, 512, Qt::KeepAspectRatio));
//       QPainter painter(&pixmap);

//       painter.setPen(QColor(255, 255, 255, 200));

//       //vertical lines
//       for(float x = 0; x <= pixmap.width(); x+=pixmap.width()/32){
//           painter.drawLine(x, 0, x, pixmap.height());
//       }

//       //horizontal lines
//       for(float y = 0; y <= pixmap.height(); y+=pixmap.height()/32){
//           painter.drawLine(0, y, pixmap.width(), y);
//       }

//       ui->label->setPixmap(pixmap);

    //ui->label->setPixmap(QPixmap::fromImage(sprite.getImage()));
}

