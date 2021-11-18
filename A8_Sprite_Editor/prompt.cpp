/*
 * Jimmy Trinh && Jacob Day && Amitoj Singh && Michael Shin
 * Software Practice II, CS 3505
 * A8: Qt Sprite Editor
 */
#include "prompt.h"
#include "ui_prompt.h"
/*!
 * Class responsible for sprite dimension acquiring. Class for initial prompt of sprite editing program.
 * Handles sending given user inputs to model to decipher.
 */

/*!
 * \brief Prompt::Prompt constructs prompt widget.
 * \param parent
 */
Prompt::Prompt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Prompt)
{
    ui->setupUi(this);
}

/*!
 * \brief Prompt::~Prompt destructor
 */
Prompt::~Prompt()
{
    delete ui;
}

/*!
 * \brief Prompt::on_pushButton_clicked Defines what is to occur when push button is clicked.
 * Will begin main application when this occurs
 */
void Prompt::on_pushButton_clicked()
{
    emit startApp(ui->comboBox->itemText(ui->comboBox->currentIndex()).toInt());
}

