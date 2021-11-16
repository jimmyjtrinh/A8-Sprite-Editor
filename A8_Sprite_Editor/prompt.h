#ifndef PROMPT_H
#define PROMPT_H

#include <QWidget>

namespace Ui {
/*!
 * Class responsible for sprite dimension acquiring. Class for initial prompt of sprite editing program.
 * Handles sending given user inputs to model to decipher.
 */
class Prompt;
}

class Prompt : public QWidget
{
    Q_OBJECT

public:
    explicit Prompt(QWidget *parent = nullptr);
    ~Prompt();

signals:
    // signal begins main application gui
    void startApp(int);

private slots:
    // slot for sending dimensions to where need
    void on_pushButton_clicked();

private:
    Ui::Prompt *ui;
};

#endif // PROMPT_H
