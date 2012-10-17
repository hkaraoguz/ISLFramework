#include "irobotdialog.h"
#include "ui_irobotdialog.h"
#include "irobot.h"

IRobotDialog::IRobotDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IRobotDialog)
{
    ui->setupUi(this);
}

IRobotDialog::IRobotDialog(QWidget *parent, Irobot *aRobot):QDialog(parent),ui(new Ui::IRobotDialog)
{

    ui->setupUi(this);

    robot = aRobot;




}

IRobotDialog::~IRobotDialog()
{
    delete ui;
}

void IRobotDialog::on_butRobotForward_clicked()
{

    robot->setMotion(0.15,0);

}

void IRobotDialog::on_butRobotRight_clicked()
{
     robot->setMotion(0,-0.3);

}

void IRobotDialog::on_butRobotLeft_clicked()
{
    robot->setMotion(0,0.3);

}

void IRobotDialog::on_butRobotBack_clicked()
{

    robot->setMotion(-0.15,0);


}

void IRobotDialog::on_butRobotStop_clicked()
{
    robot->setMotion(0,0);

}
