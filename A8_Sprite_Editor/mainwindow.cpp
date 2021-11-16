#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>

/*!
 * MainWindow class that functions as the view/controller of the sprite editing program. Will handle all things connection
 * and view. Gui component of sprite editing program
 */
using namespace std;
/*!
 * \brief MainWindow::MainWindow Constructor initializes members and sets up connections for view
 * \param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMouseTrackingBool(false);

    //Model to View
    connect(&modelObj, &Model::sendGrid, ui->label, &QLabel::setPixmap);
    connect(&modelObj, &Model::sendCoords, ui->coordLabel, &QLabel::setText);
    connect(&modelObj, &Model::sendAnimationPreviewPixmap, ui->previewImageLabel, &QLabel::setPixmap);
    connect(&modelObj, &Model::sendThumbnailLabel, this, &MainWindow::addWidgetToScrollBar);



    //View to Model
    connect(this, &MainWindow::updateGrid, &modelObj, &Model::updatePixmap);
    connect(this, &MainWindow::updatePixel, &modelObj, &Model::updateSprite);
    connect(this, &MainWindow::updateCoords, &modelObj, &Model::getCoords);
    connect(this, &MainWindow::addNewSprite, &modelObj, &Model::createNewSprite);
    connect(this, &MainWindow::changeFps, &modelObj, &Model::setFps);
    connect(this, &MainWindow::paintAll, &modelObj, &Model::updateAndPaintALl);


    //Prompt to View
    connect(&startingPrompt, &Prompt::startApp, &modelObj, &Model::getDimensions);
    connect(&startingPrompt, &Prompt::startApp, this, &MainWindow::closePromptAndOpenEditor);

    initializeMembers();
}

/*!
 * \brief MainWindow::initializeMembers method takes all field initializations and groups them into one
 */
void MainWindow::initializeMembers(){
    mouseHasBeenClicked = false;
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

/*!
 * \brief MainWindow::~MainWindow Destructor
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief MainWindow::openWindow Slot method that shows the main sprite editing software and handles inputs from
 * starting prompt
 * \param x
 */
void MainWindow::closePromptAndOpenEditor(int x)
{
    // show sprite editor
    this->show();
    // hide the starting size prompt
    startingPrompt.hide();
    emit updateGrid();
    // set mouse tracking of all to true as we want to track where mouse is now
   setMouseTrackingBool(true);
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

    if (isInCanvas() && mouseHasBeenClicked)
        updateCanvasDrawing();

    emit updateCoords(relativeXPosOfImage, relativeYPosOfImage);
}


/*!
 * \brief MainWindow::updateCanvasDrawing slot handles the drawing of the canvas, will redraw grid and current sprite's picture
 */
void MainWindow::updateCanvasDrawing(){

    emit updatePixel(relativeXPosOfImage, relativeYPosOfImage, *currColor);
    emit updateGrid();
}

/*!
 * \brief MainWindow::isInCanvas Method takes given x and y coords and checks if these are in range of drawing canvas
 * \return
 */
bool MainWindow::isInCanvas(){
    return relativeXPosOfImage >= 0 && relativeXPosOfImage < (ui->label->width()) && relativeYPosOfImage>=0 && relativeYPosOfImage<(ui->label->height());
}

/*!
 * \brief MainWindow::mousePressEvent Defines what to do when mouse is pressed, if in canvas, drawing will be drawed on
 * \param event
 */
void MainWindow::mousePressEvent(QMouseEvent *event){
    if (isInCanvas()){
        updateCanvasDrawing();
    }
    mouseHasBeenClicked = true;

}

/*!
 * \brief MainWindow::mouseReleaseEvent method defines what occurs when mouse is released
 * \param event
 */
void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    mouseHasBeenClicked = false;
}

/*!
 * \brief MainWindow::on_colorButton_clicked method defines what occurs when the color button is clicked. Will handle showing color-picking
 * dialog. Will set this classes color members accordingly
 */
void MainWindow::on_colorButton_clicked()
{
    QColor temp = QColorDialog::getColor(QColor(255,0,0,255), this);
    // gets color and cheks if it is valid
    if(temp.isValid())
    {
        // set the selected color if it is
        selectedColor = temp;
    }
    // update color label that displays color
    colorPreview.fill(selectedColor);
    ui->colorPreview->setPixmap(colorPreview.scaled(ui->colorPreview->width(),ui->colorPreview->height(),Qt::IgnoreAspectRatio));
}

/*!
 * \brief MainWindow::on_eraserButton_toggled method defines what occurs when eraser button is selected. Changes method color parameters according
 * to how eraser is selected
 * \param checked
 */
void MainWindow::on_eraserButton_toggled(bool checked)
{
    if (checked){
        currColor = &ERASER; // if button checked, make the color that draws the "eraser" color
    } else {
        // not checked thus eraser button is not active anymore, leave currColor to be the color selected b4 in dialog
        currColor = &selectedColor;
    }
}

/*!
 * \brief MainWindow::on_addNewFrameButton_clicked Defines what occurs when new frame is selected
 */
void MainWindow::on_addNewFrameButton_clicked()
{
    // let model know a new sprite is requested
    emit addNewSprite();
}

/*!
 * \brief MainWindow::on_speedSlider_valueChanged method changes the fps by sending whatever value is selected to view
 * \param value fps value to be changed to
 */
void MainWindow::on_speedSlider_valueChanged(int value)
{
    emit changeFps(value); // let model know you want to changeFps to this (value)
}

/*!
 * \brief MainWindow::addWidgetToScrollBar View is connected to this, when this is invoked will take label and add to scroll area,
 * thus creating a collection of labels in scroll area
 * \param lab
 */
void MainWindow::addWidgetToScrollBar(QLabel* lab){
    // set the outlining of label
    lab->setFrameShape(QFrame::Box);
    lab->setFixedSize(83,83);
    // add widget to layout
    boxLayout->addWidget(lab);
    container->setLayout(boxLayout);
    // add container holding layout and wigets to scroll area
    ui->allSpriteThumbnails->setWidget(container);
}

/*!
 * \brief MainWindow::on_paintBucketButton_clicked Defines method that decides how model gets paint bucket request
 */
void MainWindow::on_paintBucketButton_clicked()
{
    emit paintAll(selectedColor);
    emit updateGrid();
}

/*!
 * \brief MainWindow::setMouseTrackingBool sets the mouse tracking of this program to given bool
 * \param b bool to set mouse tracking to
 */
void MainWindow::setMouseTrackingBool(bool b){
    setMouseTracking(b);
    ui->centralwidget->setMouseTracking(b);
    ui->label->setMouseTracking(b);
}




