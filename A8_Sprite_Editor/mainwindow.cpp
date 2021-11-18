/*
 * Jimmy Trinh && Jacob Day && Amitoj Singh && Michael Shin
 * Software Practice II, CS 3505
 * A8: Qt Sprite Editor
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QPainter>
#include <QMessageBox>

/*!
 * MainWindow class that functions as the view/controller of the sprite editing program. Will handle all things connection
 * and view. Gui component of sprite editing program
 */

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
    connect(&modelObj, &Model::sendPreviewPixmap, ui->previewActualSizeLabel, &QLabel::setPixmap);
    connect(&modelObj, &Model::updateCurrentSpriteThumbnail, this, &MainWindow::updateButtonThumbnail);
    connect(&modelObj, &Model::errorWhenParsingJsonFile, this, &MainWindow::handleJsonError);
    connect(&modelObj, &Model::clearButtonThumbnails, this, &MainWindow::clearThumbnailList);


    //View to Model
    connect(this, &MainWindow::updateGrid, &modelObj, &Model::updatePixmap);
    connect(this, &MainWindow::updatePixel, &modelObj, &Model::updateSprite);
    connect(this, &MainWindow::updateCoords, &modelObj, &Model::getCoords);
    connect(this, &MainWindow::addNewSprite, &modelObj, &Model::createNewSprite);
    connect(this, &MainWindow::changeFps, &modelObj, &Model::setFps);
    connect(this, &MainWindow::paintAll, &modelObj, &Model::updateAndPaintALl);
    connect(this, &MainWindow::showPreview, &modelObj, &Model::previewAnimation);
    connect(this, &MainWindow::clearSprite, &modelObj, &Model::clearCurrentSprite);
    connect(this, &MainWindow::saveName, &modelObj, &Model::save);
    connect(this, &MainWindow::openName, &modelObj, &Model::open);
    connect(this, &MainWindow::sendIndexOfToBePreviewed, &modelObj, &Model::changeSpriteToIndex);
    connect(this, &MainWindow::paintBucket, &modelObj, &Model::paintSprite);


    //Prompt to View
    connect(&startingPrompt, &Prompt::startApp, &modelObj, &Model::getDimensionsAndInitializeFirstSprite);
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

    selectedColor = QColor(0,0,0,255);
    currColor = &selectedColor;
    ERASER = QColor(0,0,0,0);
    ui->colorPreviewButton->setStyleSheet(QString("background-color: %1").arg(selectedColor.name()));

    startingPrompt.show();
    this->hide();

    container = new QWidget();
    boxLayout = new QVBoxLayout();
    ui->allSpriteThumbnails->setAlignment(Qt::AlignTop);

    ui->previewActualSizeLabel->hide();
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
    // do nothing if image is being previewed
    if(!ui->previewActualSizeButton->isChecked()){

        // relative position of x and y of picture (0 to 513) values
        relativeXPosOfImage = event->pos().x()-ui->label->x();
        relativeYPosOfImage = event->pos().y()-ui->label->y() - ui->menubar->height();
        // if in canvas and mouse hovered or click let view know
        if (isInCanvas()){
            if (mouseHasBeenClicked)
                updateCanvasDrawing();

            emit updateCoords(relativeXPosOfImage, relativeYPosOfImage);
        }
    }
}


/*!
 * \brief MainWindow::updateCanvasDrawing slot handles the drawing of the canvas, will redraw grid and current sprite's picture
 */
void MainWindow::updateCanvasDrawing(){

    if(isPainting)
    {
        if(canPaint){
            canPaint = false;
            emit paintBucket(relativeXPosOfImage, relativeYPosOfImage, *currColor);
        }
    }
    else
    {
        emit updatePixel(relativeXPosOfImage, relativeYPosOfImage, *currColor, penSize);
    }
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
    // if you are in canvas and you click, update the sprite to reflect it by calling updateCanvasDrawing
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
    canPaint = true;
}

/*!
 * \brief MainWindow::on_colorButton_clicked method defines what occurs when the color button is clicked. Will handle showing color-picking
 * dialog. Will set this classes color members accordingly
 */
void MainWindow::on_colorButton_clicked()
{
    QColor temp = QColorDialog::getColor(selectedColor, this);
    // gets color and cheks if it is valid
    if(temp.isValid())
    {
        // set the selected color if it is
        selectedColor = temp;
        // update button color that shows color
        ui->colorPreviewButton->setStyleSheet(QString("background-color: %1").arg(temp.name()));
    }
    // if this button is clicked uncheck eraser button
    if(ui->eraserButton->isChecked())
        ui->eraserButton->click();
}

/*!
 * \brief MainWindow::on_eraserButton_toggled method defines what occurs when eraser button is selected. Changes method color parameters according
 * to how eraser is selected
 * \param checked
 */
void MainWindow::on_eraserButton_toggled(bool checked)
{

    if (checked)
        currColor = &ERASER; // if button checked, make the color that draws the "eraser" color
    else
        // not checked thus eraser button is not active anymore, leave currColor to be the color selected b4 in dialog
        currColor = &selectedColor;
    // untogle paint button if this is checked and so is that
    if(ui->paintBucketButton->isChecked() && checked)
        ui->paintBucketButton->setChecked(false);
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
void MainWindow::addWidgetToScrollBar(QPixmap lab, int i){
    QPushButton* temp = new QPushButton;
    temp->setCheckable(false);
    temp->setObjectName(QString::number(i));
    previewThumbnails.push_back(temp);

    connect(temp, &QPushButton::pressed, this, &MainWindow::translateButtonNameToNumber);

    QIcon ButtonIcon(lab);
    temp->setIcon(ButtonIcon);
    temp->setIconSize(lab.size());
    temp->setFixedSize(lab.width()+2, lab.height()+10);


    // add widget to layout
    boxLayout->addWidget(temp);
    container->setLayout(boxLayout);
    // add container holding layout and wigets to scroll area
    ui->allSpriteThumbnails->setWidget(container);
}

/*!
 * \brief MainWindow::translateButtonNameToNumber is the slot that is called by all buttons when clicked in scroll area. Will take name of
 * given button that calls it and parse the name to a number and sends to model to react to this selection made
 */
void MainWindow::translateButtonNameToNumber(){
    QPushButton * button = qobject_cast<QPushButton*>(sender());
    emit sendIndexOfToBePreviewed(button->objectName().toInt());
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


/*!
 * \brief MainWindow::on_previewActualSizeButton_toggled enables the preview at of actual size label
 * \param b bool see if button is clicked
 */
void MainWindow::on_previewActualSizeButton_toggled(bool checked)
{
    if(checked){
        ui->previewActualSizeLabel->show();
        emit showPreview();
    }
    else {
        ui->previewActualSizeLabel->hide();
    }
}

/*!
 * \brief MainWindow::on_clearButton_clicked handles clear button being clicked, send signal to model to respond to click
 */
void MainWindow::on_clearButton_clicked()
{
    emit clearSprite();
}

/*!
 * \brief MainWindow::on_smallestPenWidthSize_clicked smallest pen size is wanted, change pensize to match request
 */
void MainWindow::on_smallestPenWidthSize_clicked()
{
    penSize = 1;
}

/*!
 * \brief MainWindow::on_mediumPenWidthSize_clicked medium pen size is wanted, change pensize to 2 to match request
 */
void MainWindow::on_mediumPenWidthSize_clicked()
{
    penSize = 2;
}

/*!
 * \brief MainWindow::on_largestPenWIdthSize_clicked largest pen size is wanted, change pensize to 3 to match request
 */
void MainWindow::on_largestPenWIdthSize_clicked()
{
    penSize = 3;

}

/*!
 * \brief MainWindow::on_pushButton_2_clicked Event handler that shows shortcuts when button is pressed
 */
void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Shortcuts");
      msgBox.setInformativeText("\nctrl + S \t Save \nctrl + O \t Open \nctrl + Q \t Quit \nctrl + H \t Help \nctrl + N \t Open new project \n\n");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

/*!
 * \brief MainWindow::on_brushButton_clicked event handles when paint brush is selected, if selected will draw on canvas when told to
 */
void MainWindow::on_brushButton_clicked()
{
    ui->eraserButton->setChecked(false);
    ui->paintBucketButton->setChecked(false);
}

/*!
 * \brief MainWindow::on_actionSave_As_triggered method invoked when save as is clicked. Saves sprite frames by taking
 * input string and emitting to model
 */
void MainWindow::on_actionSave_As_triggered()
{
    QString saveFile = QFileDialog::getSaveFileName(this, tr("Sprite Save As"), "", tr("Sprite (*.ssp)"));
    if(!saveFile.isEmpty()){
        emit saveName(saveFile);
    }

}

/*!
 * \brief MainWindow::on_actionSave_As_triggered method invoked when open as is clicked. Opens sprite by taking
 * input string and emitting to model
 */
void MainWindow::on_actionOpen_triggered()
{
    QString openFile = QFileDialog::getOpenFileName(this, tr("Open Sprite"), "", tr("Sprite (*.ssp)"));
    if(!openFile.isEmpty()){
        emit openName(openFile);
    }
}

void MainWindow::clearThumbnailList()
{
    boxLayout =  new QVBoxLayout();
    container = new QWidget();
    previewThumbnails.clear();
}

/*!
 * \brief MainWindow::handleJsonError This is ran when there is an error while reading the Json file
 */
void MainWindow:: handleJsonError(){
    QMessageBox::information(this, "Parsing Error", "There is an error with the input Json file. Canvas size must have height match width");
}

/*!
 * \brief MainWindow::on_colorPreviewButton_clicked method that shows color dialog when color preview button is pressed
 */
void MainWindow::on_colorPreviewButton_clicked()
{
    on_colorButton_clicked();
}

/*!
 * \brief MainWindow::updateButtonThumbnail Takes index'd button in scroll area and changes picture of button to index
 * \param lab image of button to change to
 * \param index index of button that is being chenged
 */
void MainWindow::updateButtonThumbnail(QPixmap lab, int index){
    QPushButton* temp = previewThumbnails[index];

    QIcon ButtonIcon(lab);
    temp->setIcon(ButtonIcon);
    temp->setIconSize(lab.size());
    temp->setFixedSize(lab.width()+2, lab.height()+10);
}

/*!
 * \brief MainWindow::on_actionQuit_triggered Quits application
 */
void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

/*!
 * \brief MainWindow::on_actionHelp_triggered method that invokes help message box
 */
void MainWindow::on_actionHelp_triggered()
{
    QMessageBox::information(this, "Help", "\n\n- Drawing Tools -\n"
                                           "Brush Tool - Select to start drawing on the canvas.\n"
                                           "Eraser Tool - Select to erase.\n"
                                           "Clear Tool - Select to clear the canvas.\n"
                                           "Bucket Tool - Select fill a space with color.\n"
                                           "Color Button - Select to choose colors.\n"
                                           "Pen Size - Select pen size.\n\n"
                                           "- Viewing Animations -\n"
                                           "Animations can be viewed in the top right corner of the program.\n"
                                           "Use the slider or spinbox underneath to chagne the frames per second that it plays at.\n"
                                           "New Frame - Press New Frame button to add frames to the animation.\n\n"
                                           "- Saving and Loading -\n"
                                           "Use “File” to create a new canvas, open and save a .ssp file.\n\n");

}

/*!
 * \brief MainWindow::on_actionAbout_triggered method that invokes about screen
 */
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "About", "Qt sprite editor program based on C++ created in Qt Creator.\n"
                                            "This program was created by Jimmy Trinh, Jacob Day, Amitoj Singh, "
                                            "and Mikey Shin in CS 3505 at the University of Utah.\n"
                                            "Last edit was on November 18, 2021.");

}

/*!
 * \brief MainWindow::on_paintBucketButton_toggled method
 * \param checked
 */
void MainWindow::on_paintBucketButton_toggled(bool checked)
{
    if(ui->eraserButton->isChecked() && checked)
        ui->eraserButton->setChecked(false);
    isPainting = checked;
}

/*!
 * \brief MainWindow::on_actionNew_Project_triggered creates a new instance of a editor
 */
void MainWindow::on_actionNew_Project_triggered()
{
    MainWindow w2 = new MainWindow;
    w2.show();
}


