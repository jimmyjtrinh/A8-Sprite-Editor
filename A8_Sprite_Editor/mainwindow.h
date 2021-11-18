#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sprite.h"
#include "model.h"
#include <QColorDialog>
#include <QColor>
#include "prompt.h"
#include <QVBoxLayout>
#include <QLayout>
#include <QFileDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/*!
 * \brief The MainWindow class MainWindow class that functions as the view/controller of the sprite editing program. Will handle all things connection
 * and view. Gui component of sprite editing program
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    // event for when the mouse moves, will be used to send coordinates and handle inserting pixels to model
    void mouseMoveEvent(QMouseEvent *event);
    // event handiling when mouse is pressed - will set members and draw accordingly
    void mousePressEvent(QMouseEvent *event);
    // event when mouse is released - will set members accordingly
    void mouseReleaseEvent(QMouseEvent *event);
    // event that occurs when prompt is done with, close it and open main editor
    void closePromptAndOpenEditor(int);
    // event that handles color setting
    void on_colorButton_clicked();
    // event that handles when eraser is selected
    void on_eraserButton_toggled(bool checked);
    // event handles what to do when button for new frame is selected - will let model know
    void on_addNewFrameButton_clicked();
    // event that handles anytime slider value is changed - sends to model
    void on_speedSlider_valueChanged(int value);
    // event that adds widgets  - model connects to this
    void addWidgetToScrollBar(QPixmap, int);
    // event that handles what to send to model when paint bucket is selected
    void on_paintBucketButton_clicked();
    // event that handles when the preview button is selected
    void on_previewActualSizeButton_toggled(bool checked);
    // event that handles when clear is selected
    void on_clearButton_clicked();
    // event that handles changing pen thickness
    void on_smallestPenWidthSize_clicked();
    // event that handles when medium pen width is selected
    void on_mediumPenWidthSize_clicked();
    // event that handles when largest pen width button is selected
    void on_largestPenWIdthSize_clicked();
    // event that handles showing shortcuts
    void on_pushButton_2_clicked();
    // event that handles when brush button is clicked
    void on_brushButton_clicked();
    // event that handles what to do when save is pressed
    void on_actionSave_As_triggered();
    // event that handles what to do and send to model when open is triggered
    void on_actionOpen_triggered();
    // event that handles when color preview button is clicked
    void on_colorPreviewButton_clicked();
    // takes name of button that is passed in and translates name to number and sends to model
    void translateButtonNameToNumber();

    void updateButtonThumbnail(QPixmap,int);

    void on_actionQuit_triggered();

    void on_actionHelp_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    Model modelObj;
    Prompt startingPrompt;
    QVBoxLayout* boxLayout;
    QWidget* container;
    QPixmap colorPreview;

    bool mouseHasBeenClicked;
    double relativeXPosOfImage;
    double relativeYPosOfImage;
    bool paintSelected;
    int spriteDimensions = 32;
    int penSize = 1;
    QVector<QPushButton*> previewThumbnails;

    QColor* currColor;
    QColor selectedColor;
    QColor ERASER;
    QColor backupColor;

    // checks if mouse location is on top of canvas
    bool isInCanvas();
    // method wraps all signals that would update canvas
    void updateCanvasDrawing();
    // method initializes all members to what they need to be
    void initializeMembers();
    // sets all things in gui to be setmousetracking(bool)
    void setMouseTrackingBool(bool);




signals:
    // lets model know that grid must be updated
    void updateGrid();
    // lets model know that pixel (x,y) of canvas label must be updated to color
    void updatePixel(double, double, QColor, int);
    // lets model know to update the coords to given (x,y) of canvas label interms of sprite pixel
    void updateCoords(double, double);
    // lets model know user requested to add new sprite
    void addNewSprite();
    // lets model know fps is to be changhed to this
    void changeFps(int);
    // lets model know to fill canvas to this
    void paintAll(QColor);
    // lets the model know to start showing preview
    void showPreview();
    // lets the model know to clear the current previewed sprite
    void clearSprite();
    // lets the model know user is to save giving this name
    void saveName(QString);
    // lets the model know user is to open given this name
    void openName(QString);
    // sends the model index of sprite that is wanted to be previewed
    void sendIndexOfToBePreviewed(int);





};
#endif // MAINWINDOW_H
