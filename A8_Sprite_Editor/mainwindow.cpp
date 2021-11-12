#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>
#include <QMouseEvent>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMouseTracking(true); // E.g. set in your constructor of your widget.
}

MainWindow::~MainWindow()
{
    delete ui;
}



// Implement in your widget
void MainWindow::mouseMoveEvent(QMouseEvent *event){
//    qDebug() << event->pos();
   // if (event->pos().x()-ui->label->x()<(ui->label->width()))
        // if(ui->label->y()-event->pos().y()>0)
    int subX = event->pos().x()-ui->label->x();
    int subY = event->pos().y()-ui->label->y();
    if(subX>=0 && subX<(ui->label->width()))
        if(subY>=0 && subY<(ui->label->height()))
    cout <<subX << " " << subY<< endl;
}






void MainWindow::on_pushButton_pressed()
{
    ui->label->setPixmap(QPixmap::fromImage(sprite.getImage()));
}

