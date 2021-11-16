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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void openWindow(int);


    void on_colorButton_clicked();

 //   void on_eraserButton_clicked();

    void on_eraserButton_toggled(bool checked);



    void on_addNewFrameButton_clicked();

    void on_speedSlider_valueChanged(int value);

    void addWidgetToScrollBar(QLabel*);

    void on_paintBucketButton_clicked();

private:
    Ui::MainWindow *ui;
    Model modelObj;
    Prompt startingPrompt;

    //for testing purposes ONLY PLS REMOVE I BEG OF U
    Sprite sprite;
    bool mouseHasBeenClicked;

    double relativeXPosOfImage;
    double relativeYPosOfImage;
    QPixmap colorPreview;

    int spriteDimensions = 32;

    QColor* currColor;
    QColor selectedColor;
    QColor ERASER;


    QColor color;
    QColor backupColor;

    bool isInCanvas();
    void updateCanvasDrawing();

    QVBoxLayout* boxLayout;
    QWidget* container;

    bool paintSelected;

    void initializeMembers();
    void setMouseTrackingBool(bool);


signals:
    void updateGrid();
    void updatePixel(double, double, QColor);
    void updateCoords(double, double);
    void addNewSprite();
    void changeFps(int);
    void paintAll(QColor);



};
#endif // MAINWINDOW_H
