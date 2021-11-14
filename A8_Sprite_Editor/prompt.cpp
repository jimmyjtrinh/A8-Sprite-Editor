#include "prompt.h"
#include "ui_prompt.h"
#include <iostream>

using namespace std;

Prompt::Prompt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Prompt)
{
    ui->setupUi(this);
}

Prompt::~Prompt()
{
    delete ui;
}

void Prompt::on_pushButton_clicked()
{
    cout << ui->comboBox->itemText(ui->comboBox->currentIndex()).toInt() << endl;
    emit startApp(ui->comboBox->itemText(ui->comboBox->currentIndex()).toInt());
}

