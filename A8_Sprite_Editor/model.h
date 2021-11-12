#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QMouseEvent>

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

public slots:

signals:
    void updateDrawing(int, int, QColor);

};



#endif // MODEL_H
