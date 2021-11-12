#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "sprite.h"
#include "model.h"

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


    void on_pushButton_pressed();

signals:
   //void sendCoordAndColor();

private:
    Ui::MainWindow *ui;
    Model modelObj;

    //for testing purposes ONLY PLS REMOVE I BEG OF U
    Sprite sprite;
    bool pressed;



};
#endif // MAINWINDOW_H
