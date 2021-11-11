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
    cout << event->pos().x() << " " << event->pos().y() << endl;
}





void MainWindow::on_pushButton_pressed()
{
    ui->label->setPixmap(QPixmap::fromImage(sprite.getImage()));
}

