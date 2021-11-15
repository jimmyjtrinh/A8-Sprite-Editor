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
    setMouseTracking(false);
    ui->centralwidget->setMouseTracking(false);
    ui->label->setMouseTracking(false);

    //Model to View
    connect(&modelObj, &Model::sendPixmap, ui->label, &QLabel::setPixmap);
    connect(&modelObj, &Model::sendCoords, ui->coordLabel, &QLabel::setText);
    connect(&modelObj, &Model::sendAnimationPreviewPixmap, ui->previewImageLabel, &QLabel::setPixmap);
    connect(&modelObj, &Model::sendLabel, this, &MainWindow::addWidgetToScrollBar);



    //View to Model
    connect(this, &MainWindow::updateGrid, &modelObj, &Model::updatePixmap);
    connect(this, &MainWindow::updatePixel, &modelObj, &Model::updateSprite);
    connect(this, &MainWindow::updateCoords, &modelObj, &Model::getCoords);
    connect(this, &MainWindow::addNewSprite, &modelObj, &Model::createNewSprite);
    connect(this, &MainWindow::changeFps, &modelObj, &Model::setFps);
    connect(this, &MainWindow::paintAll, &modelObj, &Model::updateAndPaintALl);



    connect(&startingPrompt, &Prompt::startApp, &modelObj, &Model::getDimensions);
    connect(&startingPrompt, &Prompt::startApp, this, &MainWindow::openWindow);

    pressed = false;
    relativeXPosOfImage = 0;
    relativeYPosOfImage = 0;

    selectedColor = QColor(255,0,0,255);
    currColor = &selectedColor;
    ERASER = QColor(0,0,0,0);



    color = QColor(255, 0, 0, 255);
    backupColor = color;

    colorPreview = QPixmap(1,1);
    colorPreview.fill(color);
    ui->colorPreview->setPixmap(colorPreview.scaled(ui->colorPreview->width(),ui->colorPreview->height(),Qt::IgnoreAspectRatio));

    startingPrompt.show();
    this->hide();


    container = new QWidget();
    boxLayout = new QVBoxLayout();
    ui->allSpriteThumbnails->setAlignment(Qt::AlignTop);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::openWindow(int x)
{
    this->show();
    startingPrompt.hide();
    emit updateGrid();
    setMouseTracking(true);
    ui->centralwidget->setMouseTracking(true);
    ui->label->setMouseTracking(true);
}

/*!
 * \brief MainWindow::mouseMoveEvent Event that is "activated" any time mouse is drag clicked
 * sends x and y to model to be used to draw
 * \param event
 */
void MainWindow::mouseMoveEvent(QMouseEvent *event){
    // relative position of x and y of picture (0 to 255) values
    relativeXPosOfImage = event->pos().x()-ui->label->x();
    relativeYPosOfImage = event->pos().y()-ui->label->y() - ui->menubar->height();

    if (isInCanvas()){
        if(pressed){

            update();
        }
        emit updateCoords(relativeXPosOfImage, relativeYPosOfImage);
    }
}

void MainWindow::update(){

    emit updatePixel(relativeXPosOfImage, relativeYPosOfImage, *currColor);
    emit updateGrid();
}

bool MainWindow::isInCanvas(){
    return relativeXPosOfImage >= 0 && relativeXPosOfImage < (ui->label->width()) && relativeYPosOfImage>=0 && relativeYPosOfImage<(ui->label->height());
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if (isInCanvas()){
        update();
    }
    pressed = true;

}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    pressed = false;
}


void MainWindow::on_colorButton_clicked()
{
    QColor temp = QColorDialog::getColor(QColor(255,0,0,255), this);
    if(temp.isValid())
    {
        selectedColor = temp;
    }

    colorPreview.fill(selectedColor);
    ui->colorPreview->setPixmap(colorPreview.scaled(ui->colorPreview->width(),ui->colorPreview->height(),Qt::IgnoreAspectRatio));
}


void MainWindow::on_eraserButton_toggled(bool checked)
{
    if (checked){
        currColor = &ERASER;
    } else {
        currColor = &selectedColor;
    }
}


void MainWindow::on_addNewFrameButton_clicked()
{
    emit addNewSprite();
}


void MainWindow::on_speedSlider_valueChanged(int value)
{
    emit changeFps(value);
}

void MainWindow::addWidgetToScrollBar(QLabel* lab){
    lab->setFrameShape(QFrame::Box);
    boxLayout->addWidget(lab);
    container->setLayout(boxLayout);
    ui->allSpriteThumbnails->setWidget(container);
}


void MainWindow::on_paintBucketButton_clicked()
{
    emit paintAll(selectedColor);
    emit updateGrid();
}


