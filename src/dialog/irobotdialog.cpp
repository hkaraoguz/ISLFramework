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
