#include "irobotdialog.h"
#include "ui_irobotdialog.h"

IRobotDialog::IRobotDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IRobotDialog)
{
    ui->setupUi(this);
}

IRobotDialog::~IRobotDialog()
{
    delete ui;
}

void IRobotDialog::on_butRobotForward_clicked()
{

}

void IRobotDialog::on_butRobotRight_clicked()
{

}

void IRobotDialog::on_butRobotLeft_clicked()
{

}

void IRobotDialog::on_butRobotBack_clicked()
{

}
