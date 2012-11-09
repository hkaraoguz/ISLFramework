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

    connect(this,SIGNAL(setFirstOrientation()), robot, SLOT(handleSetFirstOrientation()));

    initView();
}

IRobotDialog::~IRobotDialog()
{
    delete ui;
}

void IRobotDialog::on_butRobotForward_clicked()
{

   double horSpeed = ui->lEditHorSpeed->text().toDouble();

    robot->setMotion(horSpeed,0);

}

void IRobotDialog::on_butRobotRight_clicked()
{

    double angSpeed = ui->lEditAngularSpeed->text().toDouble();

     robot->setMotion(0,-angSpeed);

}

void IRobotDialog::on_butRobotLeft_clicked()
{
    double angSpeed = ui->lEditAngularSpeed->text().toDouble();

    robot->setMotion(0,angSpeed);

}

void IRobotDialog::on_butRobotBack_clicked()
{

     double horSpeed = ui->lEditHorSpeed->text().toDouble();

    robot->setMotion(-horSpeed,0);


}

void IRobotDialog::on_butRobotStop_clicked()
{
    robot->setMotion(0,0);

}
void IRobotDialog::initView(){

    ui->lEditAngularSpeed->setText("0.3");

    ui->lEditHorSpeed->setText("0.15");

    ui->lEditCompassPort->setText("ttyUSB2");

    ui->lEditRobotPort->setText("ttyUSB0");

    ui->lEditKinectPort->setText("ttyUSB1");
}

void IRobotDialog::on_butIRobotConnect_clicked()
{
    robot->initIrobotConnection(ui->lEditRobotPort->text());
}

void IRobotDialog::on_butIRobotSetFirstOrientation_clicked()
{
    emit setFirstOrientation();


    ui->lEditFirstOrientation->setText(QString::number(robot->getFirstOrientation()));

}
