#ifndef PROMPT_H
#define PROMPT_H

#include <QWidget>

namespace Ui {
class Prompt;
}

class Prompt : public QWidget
{
    Q_OBJECT

public:
    explicit Prompt(QWidget *parent = nullptr);
    ~Prompt();

signals:
    void startApp(int);

private slots:
    void on_pushButton_clicked();

private:
    Ui::Prompt *ui;
};

#endif // PROMPT_H
